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

class SubtitleWindow
{
protected:
    void virtual DrawSubtitleBackground(Graphics* graphics, RectF* destRect);
    void virtual DrawSubtitleText(Graphics* graphics, std::wstring* displayText, PointF* point, Font* font, RectF* destRect);

public:
    const char* displayText;

    Color textColor;
    Color backgroundColor;

    WNDCLASS subtitleWc;
    HWND hwnd;

    int width;
    int height;

    int fontSize = 30;

    int fps = 5;

    const WCHAR* fontName = L"Consolas";

    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    /**
    * @brief sets the width and height of the window.
    * @param width - the width of the window.
    * @param height - the height of the window.
    */
    void SetDimensions(int width, int height);

    /**
    * @brief sets the subtitle text to display for the window.
    * @param text - text to display
    */
    void SetText(const char* text);

    /**
    * @brief updates the window and its visual effects. 
    */
    void virtual Update();

    /**
    * @brief hides the window.
    */
    void Hide();

    /**
    * @brief shows the window.
    */
    void Show();

    /**
    * @brief applies fps changes.
    */
    void UpdateFPS();

    SubtitleWindow(const char* displayText, HINSTANCE hInstance, int nCmdShow, int width, int height, int x, int y) : displayText(displayText), width(width), height(height) {
        const wchar_t CLASS_NAME[] = L"Subtitle Window Class";

        subtitleWc = {};

        subtitleWc.lpfnWndProc = WindowProc;
        subtitleWc.hInstance = hInstance;
        subtitleWc.lpszClassName = CLASS_NAME;

        RegisterClass(&subtitleWc);

        hwnd = CreateWindowEx(
            WS_EX_TOPMOST | WS_EX_LAYERED, //| WS_EX_LAYERED,// | WS_EX_TRANSPARENT,   // Optional window styles.
            CLASS_NAME,                     // Window class
            L"Spotify Lyrics",    // Window text
            (WS_OVERLAPPEDWINDOW | WS_POPUP | WS_SYSMENU) & ~(WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU | WS_BORDER),            // Window style
            x, y, width, height, // Size and position
            NULL,       // Parent window    
            NULL,       // Menu
            hInstance,  // Instance handle
            NULL        // Additional application data
        );

        SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)this); // store the class context in the window to be used by WindowProc

        SetLayeredWindowAttributes(hwnd, RGB(0,0,0), 0, LWA_COLORKEY);

        GdiplusStartupInput gdiplusStartupInput;
        ULONG_PTR           gdiplusToken;
        Status st = GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

        textColor = Color(255, 255, 255);
        backgroundColor = Color(0, 0, 0);

        SetTimer(hwnd, 1, (1000.0f / (float)fps), NULL);

        Show();
    }
};

