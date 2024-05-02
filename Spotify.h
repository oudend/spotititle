#pragma once

#include <curl\curl.h>

#include <string>
#include <vector>

#include <windows.h>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <stdio.h>
#include <cstring>
#include <locale>
#include <codecvt>
#include <memory>

#include "libs/json/json.hpp"
using json = nlohmann::json;

/**
* @brief Write Callback used by libcurl to write the requested data into a variable.
*/
size_t SpotifyWriteCallback(char* contents, size_t size, size_t nmemb, void* userp);

class Spotify
{
private:
    const char* accessToken = "";
    std::string authorizationString;
    const char* SP_DC = "";

public:
    enum class Result {
        SUCCESS,
        FAIL,
    };

    enum class SyncType {
        LINE_SYNCED,
        UNSYNCED,
        NONE
    };

    enum class ReturnCode {
        OK,
        SERVER_ERROR,
    };

    struct CurrentSongData {
        const char* name;
        const char* id;
        const char* albumImageLink;
        unsigned long int progress;
        bool listening;
        ReturnCode code;
    };

    struct LyricsTimeData {
        unsigned long int elapsedTime;
        const char* lineLyric = "";
    };

    struct LyricsData {
        SyncType syncType;
        std::vector<LyricsTimeData> lyricsTimeData;

        void free_memory()
        {
            int vecSize = lyricsTimeData.size();

            for (int i = 0; i < vecSize; i++)
            {
                free((char*)lyricsTimeData[i].lineLyric);
            }
        }
    };

    struct LyricData
    {
        unsigned long int timeLeft;
        const char* lineLyric;
        LyricsTimeData closestLyricsTimeData;
    };
private:

    /**
    * @brief takes in a lyrics string with json format and parses it into the LyricsData struct.
    * @param readBuffer - string containing the json data.
    * @return LyricsData
    */
    LyricsData parseLyricsString(std::string readBuffer) {
        const char* jsonString = readBuffer.c_str();
        json data;

        try {
            data = json::parse(readBuffer);
        }
        catch (nlohmann::json::parse_error& e) {
            data.clear();
            return { SyncType::NONE, std::vector<LyricsTimeData>() };  // If a parse error is thrown, it's not a valid JSON string
        }


        std::string startTimeMsString = data["lyrics"]["lines"][0]["startTimeMs"];

        unsigned long int startTimeMs = std::stoi(startTimeMsString);

        std::vector<LyricsTimeData> lyricsTimeData;

        for (const auto& element : data["lyrics"]["lines"]) {
            LyricsTimeData subLyricsTimeData {
                std::stoi(element["startTimeMs"].template get<std::string>()),
                _strdup(element["words"].template get<std::string>().c_str())
            };

            lyricsTimeData.push_back(subLyricsTimeData);
        }

        std::string syncTypeString = data["lyrics"]["syncType"];

        SyncType syncType = SyncType::UNSYNCED;

        if (syncTypeString == "LINE_SYNCED") {
            syncType = SyncType::LINE_SYNCED;
        }

        if (syncType == SyncType::UNSYNCED) {
            return { syncType, std::vector<LyricsTimeData>() };
        }

        return { syncType, lyricsTimeData };
    }

public:

    /**
    * @brief uses timestamp and lyrics data to get the closest upcoming lyric to the timestamp
    * @param timestamp - the current timestamp, how far in milliseconds into the song to check.
    * @param lyricsTimeData - a vector of time data containing timestamps and lyrics. 
    * @return LyricsData
    */
    static LyricData calculateNextLyric(unsigned long int timestamp, std::vector<LyricsTimeData> lyricsTimeData) {
        int vecSize = lyricsTimeData.size();

        LyricsTimeData closestLyricsTimeData = lyricsTimeData[0];

        for (int i = 0; i < vecSize; i++)
        {
            LyricsTimeData currentLyricsTimeData = lyricsTimeData[i];

            closestLyricsTimeData = currentLyricsTimeData;

            if (timestamp < currentLyricsTimeData.elapsedTime) {
                break;
            }
        }

        return { closestLyricsTimeData.elapsedTime - timestamp, closestLyricsTimeData.lineLyric, closestLyricsTimeData };
    }

    /**
    * @brief uses timestamp and lyrics data to get the closest lyric before the timestamp
    * @param timestamp - the current timestamp, how far in milliseconds into the song to check.
    * @param lyricsTimeData - a vector of time data containing timestamps and lyrics.
    * @return LyricsData
    */
    static LyricData calculateCurrentLyric(unsigned long int timestamp, std::vector<LyricsTimeData> lyricsTimeData) {
        int vecSize = lyricsTimeData.size();

        LyricsTimeData closestLyricsTimeData = lyricsTimeData[0];

        for (int i = 0; i < vecSize; i++)
        {
            LyricsTimeData currentLyricsTimeData = lyricsTimeData[i];

            if (timestamp < currentLyricsTimeData.elapsedTime) {
                break;
            }

            closestLyricsTimeData = currentLyricsTimeData;
        }

        return { closestLyricsTimeData.elapsedTime - timestamp, closestLyricsTimeData.lineLyric, closestLyricsTimeData };
    }

    /**
    * @brief retrieves and parses the lyrics based on the songs spotify id.
    * @param id - the spotify id for the song. https://developer.spotify.com/documentation/web-api/concepts/spotify-uris-ids
    * @return LyricsData
    */
    LyricsData getLyrics(const char* id) {
        CURLcode ret;
        CURL* hnd;
        struct curl_slist* slist1;
        std::string readBufferWrite;

        //curl HTTP header 
        slist1 = NULL;
        slist1 = curl_slist_append(slist1, "Accept: */*");
        slist1 = curl_slist_append(slist1, "Accept-Encoding: gzip, deflate");
        slist1 = curl_slist_append(slist1, "Connection: keep-alive");
        slist1 = curl_slist_append(slist1, "User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/101.0.4951.41 Safari/537.36");
        slist1 = curl_slist_append(slist1, "app-platform: WebPlayer");
        slist1 = curl_slist_append(slist1, authorizationString.c_str()); //access token
        //curl HTTP header 

        std::string requestUrl = "https://spclient.wg.spotify.com/color-lyrics/v2/track/";
        requestUrl += id; // id of the song gets added to the url
        requestUrl += "\?format=json&market=from_token";

        hnd = curl_easy_init();
        curl_easy_setopt(hnd, CURLOPT_BUFFERSIZE, 102400L);
        curl_easy_setopt(hnd, CURLOPT_URL, requestUrl.c_str());
        curl_easy_setopt(hnd, CURLOPT_NOPROGRESS, 1L);
        curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, slist1);
        curl_easy_setopt(hnd, CURLOPT_USERAGENT, "curl/8.6.0");
        curl_easy_setopt(hnd, CURLOPT_MAXREDIRS, 50L);
        curl_easy_setopt(hnd, CURLOPT_HTTP_VERSION, (long)CURL_HTTP_VERSION_2TLS);
        curl_easy_setopt(hnd, CURLOPT_ACCEPT_ENCODING, "");
        curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "GET");
        curl_easy_setopt(hnd, CURLOPT_FTP_SKIP_PASV_IP, 1L);
        curl_easy_setopt(hnd, CURLOPT_TCP_KEEPALIVE, 1L);

        curl_easy_setopt(hnd, CURLOPT_VERBOSE, 1L);

        curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, SpotifyWriteCallback); //set write function
        curl_easy_setopt(hnd, CURLOPT_WRITEDATA, &readBufferWrite); //variable that will retrieve the data using the write function

        ret = curl_easy_perform(hnd);

        const char* jsonString = readBufferWrite.c_str();

        curl_easy_cleanup(hnd);
        hnd = NULL;
        curl_slist_free_all(slist1);
        slist1 = NULL;

        if (readBufferWrite.length() == 0) {
            return { SyncType::NONE, std::vector<LyricsTimeData>() };
        }

        return parseLyricsString(readBufferWrite);
    }

    /**
    * @brief retrieves data for the currently playing spotify song.
    * @return CurrentSongData
    */
    CurrentSongData getCurrentlyPlaying() {
        CURLcode ret;
        CURL* hnd;
        struct curl_slist* slist1;

        std::string readBuffer;

        //curl HTTP header 
        slist1 = NULL;
        slist1 = curl_slist_append(slist1, authorizationString.c_str()); //access token
        //curl HTTP header 

        hnd = curl_easy_init();
        curl_easy_setopt(hnd, CURLOPT_BUFFERSIZE, 102400L);
        curl_easy_setopt(hnd, CURLOPT_URL, "https://api.spotify.com/v1/me/player/currently-playing");
        curl_easy_setopt(hnd, CURLOPT_NOPROGRESS, 1L);
        curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, slist1);
        curl_easy_setopt(hnd, CURLOPT_USERAGENT, "curl/8.6.0");
        curl_easy_setopt(hnd, CURLOPT_MAXREDIRS, 50L);
        curl_easy_setopt(hnd, CURLOPT_HTTP_VERSION, (long)CURL_HTTP_VERSION_2TLS);
        curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "GET");
        curl_easy_setopt(hnd, CURLOPT_FTP_SKIP_PASV_IP, 1L);
        curl_easy_setopt(hnd, CURLOPT_TCP_KEEPALIVE, 1L);

        curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, SpotifyWriteCallback);
        curl_easy_setopt(hnd, CURLOPT_WRITEDATA, &readBuffer);

        ret = curl_easy_perform(hnd);

        curl_easy_cleanup(hnd);
        hnd = NULL;
        curl_slist_free_all(slist1);
        slist1 = NULL;

        if(readBuffer.length() == 0)
            return { NULL, NULL, NULL, NULL, false, ReturnCode::OK };

        json data;
        try {
            data = json::parse(readBuffer);
        }
        catch (nlohmann::json::parse_error& e) {
            data.clear();
            return { NULL, NULL, NULL, NULL,false, ReturnCode::SERVER_ERROR };  // If a parse error is thrown, it's not a valid JSON string
        }

        unsigned long int progress_ms = data.value("progress_ms", 0);

        if (data.contains("error") || data["item"].is_null()) {
            return { NULL, NULL, NULL, NULL,false, ReturnCode::SERVER_ERROR };
        }

        std::string id = data["item"].value("id", "");

        std::string name = data["item"].value("name", "");

        std::string albumImageLink = data["item"]["album"]["images"][0].value("url", "");

        //_strdup is used so the string doesn't go out of scope.
        return { _strdup(name.c_str()), _strdup(id.c_str()), _strdup(albumImageLink.c_str()), progress_ms, true, ReturnCode::OK };
    }

    /**
    * @brief refreshes the access token. Needs to be done hourly because the generated tokens only last about an hour. 
    * @param sp_dc - a secret token that can be accessed through the spotify web client. https://pkg.go.dev/github.com/mirrorfm/spotify-webplayer-token#section-readme
    * @return LyricsData
    */
    Result refreshAccessToken(const char* sp_dc) {
        CURLcode ret;
        CURL* hnd;
        struct curl_slist* slist1;

        std::string readBuffer;

        std::string sp_dc_string = "Cookie: sp_dc=";
        sp_dc_string += sp_dc;

        //curl HTTP header 
        slist1 = NULL;
        slist1 = curl_slist_append(slist1, "Accept: */*");
        slist1 = curl_slist_append(slist1, "Accept-Encoding: gzip, deflate");
        slist1 = curl_slist_append(slist1, "Connection: keep-alive");
        slist1 = curl_slist_append(slist1, sp_dc_string.c_str()); //SP_DC
        slist1 = curl_slist_append(slist1, "User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/101.0.4951.41 Safari/537.36");
        slist1 = curl_slist_append(slist1, "app-platform: WebPlayer");
        //curl HTTP header 

        hnd = curl_easy_init();
        curl_easy_setopt(hnd, CURLOPT_BUFFERSIZE, 102400L);
        curl_easy_setopt(hnd, CURLOPT_URL, "https://open.spotify.com/get_access_token\?reason=transport&productType=web_player");
        curl_easy_setopt(hnd, CURLOPT_NOPROGRESS, 1L);
        curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, slist1);
        curl_easy_setopt(hnd, CURLOPT_USERAGENT, "curl/8.6.0");
        curl_easy_setopt(hnd, CURLOPT_MAXREDIRS, 50L);
        curl_easy_setopt(hnd, CURLOPT_HTTP_VERSION, (long)CURL_HTTP_VERSION_2TLS);
        curl_easy_setopt(hnd, CURLOPT_ACCEPT_ENCODING, "");
        curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "GET");
        curl_easy_setopt(hnd, CURLOPT_FTP_SKIP_PASV_IP, 1L);
        curl_easy_setopt(hnd, CURLOPT_TCP_KEEPALIVE, 1L);

        curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, SpotifyWriteCallback);
        curl_easy_setopt(hnd, CURLOPT_WRITEDATA, &readBuffer);

        ret = curl_easy_perform(hnd);

        curl_easy_cleanup(hnd);
        hnd = NULL;
        curl_slist_free_all(slist1);
        slist1 = NULL;

        const char* jsonString = readBuffer.c_str();

        if (readBuffer.length() == 0)
            return Result::FAIL;

        json data;

        try {
            data = json::parse(readBuffer);
        }
        catch (nlohmann::json::parse_error& e) {
            data.clear();
            return Result::FAIL;  // If a parse error is thrown, it's not a valid JSON string
        }

        std::string accessTokenString = data["accessToken"]; // extract the access token from the json response.

        accessToken = _strdup(accessTokenString.c_str());

        authorizationString = "authorization: Bearer ";
        authorizationString += accessToken;


        SP_DC = sp_dc;

        return Result::SUCCESS;
    }

    Spotify(const char* sp_dc)
    {        
        refreshAccessToken(sp_dc);
    }

    Spotify()
    {
        
    }
};