#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <string>
#include <stdio.h>
#include <cstring>
#include <vector>
#include <locale>
#include <codecvt>

#define CURL_STATICLIB
#include <curl\curl.h>

#include "json.h"

#pragma once


namespace SongParser
{
    struct SongData {
        const char* name;
        unsigned long int timestamp;
        bool listening;
        bool success;
    };

    struct LyricsTimeData {
        unsigned long int elapsedTime;
        const char* lineLyric = "";
    };

    struct LyricsData {
        bool synced;
        std::vector<LyricsTimeData> lyricsTimeData;
        bool success;
    };

    struct CurrentLyricData
    {
        unsigned long int timeLeft;
        const char* lineLyric;
        LyricsTimeData closestLyricsTimeData;
    };

    size_t WriteCallback(char* contents, size_t size, size_t nmemb, void* userp);

    std::string makeJsonRequest(const char* url);

    LyricsData getLyricsData();

    SongData getSongData();

    CurrentLyricData calculateNextLyric(unsigned long int timestamp, std::vector<LyricsTimeData> lyricsTimeData);
};

