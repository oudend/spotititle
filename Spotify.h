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

#pragma once

size_t SpotifyWriteCallback(char* contents, size_t size, size_t nmemb, void* userp);

class Spotify
{
private:
    const char* accessToken = "";
    std::string authorizationString;
    const char* SP_DC;

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

    LyricsData parseLyricsString(std::string readBuffer) {
        const char* jsonString = readBuffer.c_str();

        json data = json::parse(readBuffer);

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

    LyricsData getLyrics(const char* id) {
        CURLcode ret;
        CURL* hnd;
        struct curl_slist* slist1;
        std::string readBufferWrite;

        slist1 = NULL;
        slist1 = curl_slist_append(slist1, "Accept: */*");
        slist1 = curl_slist_append(slist1, "Accept-Encoding: gzip, deflate");
        slist1 = curl_slist_append(slist1, "Connection: keep-alive");
        slist1 = curl_slist_append(slist1, "Cookie: sp_dc=AQD6knh1dlSYR9FQqYRS8Y4WvKdVfCZpqJZt494pcxznS1GGuRFHqF9Lpmc0p8U02Tp44xrWcPm98gUuDe3Z-b4eLUnY8dCQ2L0UGRowgM2OGLx68NX-1E7TLrXw5oxEGsMCM-ULeJ9S_EP4LxGdFct5aXdW01I; sp_t=321ddf0ee6e6942fc2a04a79f085a2f5");
        slist1 = curl_slist_append(slist1, "User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/101.0.4951.41 Safari/537.36");
        slist1 = curl_slist_append(slist1, "app-platform: WebPlayer");
        slist1 = curl_slist_append(slist1, authorizationString.c_str());

        std::string requestUrl = "https://spclient.wg.spotify.com/color-lyrics/v2/track/";
        requestUrl += id;
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

        curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, SpotifyWriteCallback);
        curl_easy_setopt(hnd, CURLOPT_WRITEDATA, &readBufferWrite);

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

    CurrentSongData getCurrentlyPlaying() {
        CURLcode ret;
        CURL* hnd;
        struct curl_slist* slist1;

        std::string readBuffer;

        slist1 = NULL;
        slist1 = curl_slist_append(slist1, authorizationString.c_str());

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
            return { NULL, NULL, NULL, false, ReturnCode::OK };

        json data = json::parse(readBuffer);

        unsigned long int progress_ms = data.value("progress_ms", 0);

        std::string error = data.value("error", "");

        if (error.length() != 0) {
            return { NULL, NULL, NULL, false, ReturnCode::SERVER_ERROR };
        }

        std::string id = data["item"].value("id", "");

        std::string name = data["item"].value("name", "");

        CurrentSongData songData = { _strdup(name.c_str()), _strdup(id.c_str()), progress_ms, true, ReturnCode::OK};

        return songData;
    }

    Result refreshAccessToken(const char* sp_dc) {
        CURLcode ret;
        CURL* hnd;
        struct curl_slist* slist1;

        std::string readBuffer;

        std::string sp_dc_string = "Cookie: sp_dc=";
        sp_dc_string += sp_dc;

        slist1 = NULL;
        slist1 = curl_slist_append(slist1, "Accept: */*");
        slist1 = curl_slist_append(slist1, "Accept-Encoding: gzip, deflate");
        slist1 = curl_slist_append(slist1, "Connection: keep-alive");
        slist1 = curl_slist_append(slist1, sp_dc_string.c_str());
        slist1 = curl_slist_append(slist1, "User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/101.0.4951.41 Safari/537.36");
        slist1 = curl_slist_append(slist1, "app-platform: WebPlayer");

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

        const char* jsonString = readBuffer.c_str();

        if (readBuffer.length() == 0)
            return Result::FAIL;

        json data;

        try {
            data = json::parse(readBuffer);
        }
        catch (nlohmann::json::parse_error& e) {
            return Result::FAIL;  // If a parse error is thrown, it's not a valid JSON string
        }

        std::string accessTokenString = data["accessToken"];

        accessToken = _strdup(accessTokenString.c_str());

        authorizationString = "authorization: Bearer ";
        authorizationString += accessToken;

        curl_easy_cleanup(hnd);
        hnd = NULL;
        curl_slist_free_all(slist1);
        slist1 = NULL;

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