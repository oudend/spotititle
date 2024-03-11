#pragma once

#pragma comment(linker, "\"/manifestdependency:type='Win32' name='Microsoft.Windows.GdiPlus' version='1.1.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")

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
#include <string.h>
#include <limits.h>
#include <time.h>
#include <functional>

#include <curl\curl.h>

#define GDIPVER     0x0110  // Use more advanced GDI+ features

#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment (lib, "gdiplus.lib")
#include <gdipluseffects.h>

#include "SubtitleWindow.h";

class StylizedSubtitleWindow : public SubtitleWindow
{
protected:
    void virtual DrawSubtitleBackground(Graphics* graphics, RectF* destRect);
    //void virtual DrawSubtitleText(Graphics* graphics, std::wstring* displayText, PointF* point, Font* font, RectF* destRect);
private:
    static size_t WriteCallbackLoadImageFromUrl(void* contents, size_t size, size_t nmemb, std::vector<BYTE>* s) {
        size_t newLength = size * nmemb;
        try {
            s->insert(s->end(), (BYTE*)contents, (BYTE*)contents + newLength);
        }
        catch (std::bad_alloc& e) {
            return 0;
        }
        return newLength;
    }

    Bitmap* ResizeBitmap(Bitmap* bmp, int newWidth, int newHeight)
    {
        Bitmap* newBmp = new Bitmap(newWidth, newHeight, bmp->GetPixelFormat());
        Graphics graphics(newBmp);
        graphics.DrawImage(bmp, 0, 0, newWidth, newHeight);
        return newBmp;
    }

    Bitmap* LoadImageFromUrl(const char* url, int width = 400, int height = 400) {
        CURL* curl;
        CURLcode res;
        std::vector<BYTE> imageData;

        curl_global_init(CURL_GLOBAL_DEFAULT);
        curl = curl_easy_init();

        if (curl) {
            curl_easy_setopt(curl, CURLOPT_URL, url);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallbackLoadImageFromUrl);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &imageData);
            res = curl_easy_perform(curl);

            if (res != CURLE_OK) {
                fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            }

            curl_easy_cleanup(curl);
        }

        curl_global_cleanup();

        // Create a GDI+ Image object from the downloaded data
        IStream* pStream = NULL;
        HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, imageData.size());
        if (hGlobal) {
            void* pBuf = GlobalLock(hGlobal);
            if (pBuf) {
                CopyMemory(pBuf, &imageData[0], imageData.size());
                GlobalUnlock(hGlobal);
                if (SUCCEEDED(CreateStreamOnHGlobal(hGlobal, TRUE, &pStream))) {
                    //Gdiplus::Image* image = Gdiplus::Image::FromStream(pStream);
                    Gdiplus::Bitmap* bitmap = ResizeBitmap(Gdiplus::Bitmap::FromStream(pStream), width, height);
                    pStream->Release();
                    return bitmap;
                }
            }
            GlobalFree(hGlobal);
        }

        return NULL;
    }

public:
    Bitmap* backgroundImage;

    void SetImage(const char* url);

    int useImage = 1;

    void Update();

    int angle = 90;

	StylizedSubtitleWindow(const char* displayText, HINSTANCE hInstance, int nCmdShow, int width, int height, int x, int y) : SubtitleWindow(displayText, hInstance, nCmdShow, width, height, x, y)
	{

	}
};

