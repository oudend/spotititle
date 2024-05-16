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
protected: //necessary to define here so that I can use the original ones defined in SubtitleWindow.h as well.
    void virtual DrawSubtitleBackground(Graphics* graphics, RectF* destRect);
    void virtual DrawSubtitleText(Graphics* graphics, std::wstring* displayText, PointF* point, Font* font, RectF* destRect);
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

    /**
    * @brief resizes bitmap to specified width and height.
    * @param bmp - a pointer to the bitmap which will be resized.
    * @param newWidth - the new width of the bitmap.
    * @param newHeight - the new height of the bitmap.
    * @return Bitmap*
    */
    Bitmap* ResizeBitmap(Bitmap* bmp, int newWidth, int newHeight);

    /**
    * @brief downloads image as bitmap based on url and resizes bitmap to specified width and height.
    * @param url - string containing the url for the image to load.
    * @param width - the new width of the image.
    * @param height - the new height of the image.
    * @return Bitmap*
    */
    Bitmap* LoadImageFromUrl(const char* url, int width = 400, int height = 400);

public:
    enum Theme
    {
        Purple = 0,
        Green = 1,
        Orange = 2,
        Blue = 3,
        Red = 4,
        Pink = 5,
        Image = 6
    };
    const int themeColors[7] = {Color::Purple, Color::ForestGreen, Color::Orange, Color::CornflowerBlue, Color::MediumVioletRed, Color::HotPink, Color::GhostWhite};
    const char* themes[7] = {"Purple", "Green", "Orange", "Blue", "Red", "Pink", "Image"};

private:
    Theme currentTheme;
public:

    Bitmap* loadedImage = nullptr;
    Bitmap* backgroundBitmap = nullptr;

    /**
    * @brief blurs Bitmap using gdip+ based on radius. 
    * @param bitmap - bitmap to blur.
    * @param radius - blur radius.
    * @return Bitmap*
    */
    Bitmap* BlurBitmap(Bitmap* bitmap, int radius = 40);

    /**
    * @brief sets the image the subtitle window will use as background based on url.
    * @param url - url to the image.
    */
    void SetImage(const char* url);

    /**
    * @brief sets the theme of the subtitle window which determines its background colors.
    * @param theme - the theme to use.
    */
    void SetTheme(Theme theme);

    void Update();

    int angle = 90;

    StylizedSubtitleWindow(const char* displayText, HINSTANCE hInstance, int nCmdShow, int width, int height, int x, int y) : SubtitleWindow(displayText, hInstance, nCmdShow, width, height, x, y) 
    {
        SetTheme(Theme::Purple);
    }
};

