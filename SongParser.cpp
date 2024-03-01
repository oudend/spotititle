#include "SongParser.h"

//using namespace SongParser;

namespace SongParser
{
    size_t WriteCallback(char* contents, size_t size, size_t nmemb, void* userp)
    {
        ((std::string*)userp)->append((char*)contents, size * nmemb);
        return size * nmemb;
    }

    std::string makeJsonRequest(const char* url)
    {
        std::string readBuffer;

        CURLcode ret;
        CURL* hnd;
        struct curl_slist* slist1;

        slist1 = NULL;
        slist1 = curl_slist_append(slist1, "Accept: application/json");
        slist1 = curl_slist_append(slist1, "Content-Type: application/json");

        hnd = curl_easy_init();
        curl_easy_setopt(hnd, CURLOPT_BUFFERSIZE, 102400L);
        curl_easy_setopt(hnd, CURLOPT_URL, url);
        curl_easy_setopt(hnd, CURLOPT_NOPROGRESS, 1L);
        curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, slist1);
        curl_easy_setopt(hnd, CURLOPT_USERAGENT, "curl/8.6.0");
        curl_easy_setopt(hnd, CURLOPT_MAXREDIRS, 50L);
        curl_easy_setopt(hnd, CURLOPT_HTTP_VERSION, (long)CURL_HTTP_VERSION_2TLS);
        curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "GET");
        curl_easy_setopt(hnd, CURLOPT_FTP_SKIP_PASV_IP, 1L);
        curl_easy_setopt(hnd, CURLOPT_TCP_KEEPALIVE, 1L);

        curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(hnd, CURLOPT_WRITEDATA, &readBuffer);

        ret = curl_easy_perform(hnd);

        curl_easy_cleanup(hnd);
        hnd = NULL;
        curl_slist_free_all(slist1);
        slist1 = NULL;

        return readBuffer;
    }

    LyricsData getLyricsData() {
        std::string readBuffer;

        readBuffer = makeJsonRequest("http://localhost:8080/lyrics");

        if (readBuffer.length() == 0)
            return { NULL, std::vector<LyricsTimeData>(), false };

        const char* jsonString = readBuffer.c_str();

        struct json_value_s* root = json_parse(jsonString, strlen(jsonString));

        struct json_object_s* object = (struct json_object_s*)root->payload;

        struct json_object_element_s* syncedElement = object->start;

        bool synced = json_value_is_true(syncedElement->value);

        if (!synced)
            return { false, std::vector<LyricsTimeData>(), true };

        struct json_object_element_s* lyricsElement = syncedElement->next;

        struct json_array_s* lyricsObject = json_value_as_array(lyricsElement->value);

        std::vector<LyricsTimeData> lyricsTimeData;

        struct json_array_element_s* lyricsData = lyricsObject->start;

        for (int i = 0; i < lyricsObject->length; i++) {

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

        return { true, lyricsTimeData, true };
    }

    SongData getSongData() {
        std::string readBuffer;

        readBuffer = makeJsonRequest("http://localhost:8080/current");

        if (readBuffer.length() == 0) {
            return { NULL, NULL, NULL, false };
        }

        const char* jsonString = readBuffer.c_str();

        struct json_value_s* root = json_parse(jsonString, strlen(jsonString));

        struct json_object_s* object = (struct json_object_s*)root->payload;

        struct json_object_element_s* listeningElement = object->start;


        bool listening = json_value_is_true(listeningElement->value);

        if (!listening) {
            return { NULL, NULL, false, true };
        }

        struct json_object_element_s* timestampElement = listeningElement->next;

        const char* timestampString = json_value_as_number(timestampElement->value)->number;

        std::stringstream strValue;
        strValue << timestampString;

        unsigned long int timestamp;
        strValue >> timestamp;

        const char* name = json_value_as_string(timestampElement->next->value)->string;

        return { name, timestamp, true, true };
    }

    CurrentLyricData calculateNextLyric(unsigned long int timestamp, std::vector<LyricsTimeData> lyricsTimeData) {
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
}