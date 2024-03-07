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

#include "json.h"

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

        struct json_value_s* root = json_parse(jsonString, strlen(jsonString));

        struct json_object_s* object = (struct json_object_s*)root->payload;

        struct json_object_s* lyricsObject = json_value_as_object(object->start->value);

        struct json_object_element_s* syncTypeElement = lyricsObject->start;

        const char* syncTypeString = json_value_as_string(syncTypeElement->value)->string;

        SyncType syncType = SyncType::UNSYNCED;

        if (strcmp(syncTypeString, "LINE_SYNCED") == 0) {
            syncType = SyncType::LINE_SYNCED;
        }

        if (syncType == SyncType::UNSYNCED)
            return { syncType, std::vector<LyricsTimeData>() };

        struct json_object_element_s* lyricsElement = syncTypeElement->next;

        struct json_array_s* lyricsLinesObject = json_value_as_array(lyricsElement->value);

        std::vector<LyricsTimeData> lyricsTimeData;

        struct json_array_element_s* lyricsData = lyricsLinesObject->start;

        for (int i = 0; i < lyricsLinesObject->length; i++) {

            struct json_object_s* lyricsDataObject = json_value_as_object(lyricsData->value);

            struct json_object_element_s* elapsedTimeElement = lyricsDataObject->start;

            const char* elapsedTimeString = json_value_as_string(elapsedTimeElement->value)->string;

            std::stringstream strValue;
            strValue << elapsedTimeString;

            unsigned long int elapsedTime;
            strValue >> elapsedTime;

            struct json_object_element_s* lineLyricElement = elapsedTimeElement->next;

            const char* lineLyric = json_value_as_string(lineLyricElement->value)->string;

            lyricsTimeData.push_back({ elapsedTime, lineLyric });

            lyricsData = lyricsData->next;
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

        const char* jsonString = readBuffer.c_str();

        curl_easy_cleanup(hnd);
        hnd = NULL;
        curl_slist_free_all(slist1);
        slist1 = NULL;

        if(readBuffer.length() == 0)
            return { NULL, NULL, NULL, false, ReturnCode::OK };


        struct json_value_s* root = json_parse(jsonString, strlen(jsonString));

        struct json_object_s* object = (struct json_object_s*)root->payload;

        struct json_object_element_s* errorElement = object->start;

        if (strcmp(errorElement->name->string, "error") == 0) {
            return { NULL, NULL, NULL, false, ReturnCode::SERVER_ERROR };
        }

        //if (strcmp(errorElement->name->string, "timestamp") == 0) { 
        //    return { NULL, NULL, NULL, false, ReturnCode::OK };
        //}

        struct json_object_element_s* progressElement = object->start->next->next;

        const char* progressString = json_value_as_number(progressElement->value)->number;

        std::stringstream strValue;
        strValue << progressString;

        unsigned long int progress;
        strValue >> progress;

        struct json_object_s* itemObject = json_value_as_object(progressElement->next->value);

        struct json_object_element_s* idElement = itemObject->start->next->next->next->next->next->next->next->next->next;

        const char* id = json_value_as_string(idElement->value)->string;

        struct json_object_element_s* nameElement = idElement->next->next;

        const char* name = json_value_as_string(nameElement->value)->string;

        return { name, id, progress, true, ReturnCode::OK };
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

        if(readBuffer.length() == 0)
            return Result::FAIL;

        struct json_value_s* root = json_parse(jsonString, strlen(jsonString));

        if (root == nullptr)
            return Result::FAIL;

        struct json_object_s* object = (struct json_object_s*)root->payload;

        struct json_object_element_s* accessTokenElement = object->start->next;

        accessToken = json_value_as_string(accessTokenElement->value)->string;

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