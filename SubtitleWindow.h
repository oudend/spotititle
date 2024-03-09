#pragma once

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

#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment (lib, "gdiplus.lib")

class SubtitleWindow
{
public:
    const char* displayText;

    Color textColorPrimary;
    Color textColorSecondary;

    Color backgroundColorPrimary;
    Color backgroundColorSecondary;

    //Matrix backgroundGradientRotationMatrix;
    Matrix* backgroundGradientRotationMatrix;

    WNDCLASS subtitleWc;
    HWND hwnd;

    //HFONT hFont;

    int width;
    int height;

    int fontSize = 30;

    const char* fontName = "Consolas";

    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    void SetDimensions(int width, int height);

    //void SetFont(const char* name, int size);

    //void SetFont(HFONT font);

    void SetText(const char* text);

    void Update();

    void Hide();

    void Show();

    SubtitleWindow(const char* displayText, HINSTANCE hInstance, int nCmdShow, int width, int height, int x, int y) : displayText(displayText), width(width), height(height) {
        const wchar_t CLASS_NAME[] = L"Subtitle Window Class";

        subtitleWc = {};

        subtitleWc.lpfnWndProc = WindowProc;
        subtitleWc.hInstance = hInstance;
        subtitleWc.lpszClassName = CLASS_NAME;

        RegisterClass(&subtitleWc);

        hwnd = CreateWindowEx(
            WS_EX_TOPMOST,// | WS_EX_TRANSPARENT,   // Optional window styles.
            CLASS_NAME,                     // Window class
            L"Spotify Lyrics",    // Window text
            WS_OVERLAPPEDWINDOW | WS_POPUP | WS_BORDER,            // Window style
            x, y, width, height, // Size and position
            NULL,       // Parent window    
            NULL,       // Menu
            hInstance,  // Instance handle
            NULL        // Additional application data
        );

        SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)this); // store the class context in the window to be used by WindowProc

        long Style = (WS_CAPTION | WS_SYSMENU);
        long a = SetWindowLongA(hwnd, GWL_STYLE, Style & ~WS_BORDER);

        SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);

        LONG lStyle = GetWindowLong(hwnd, GWL_STYLE);
        lStyle &= ~(WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU);
        SetWindowLong(hwnd, GWL_STYLE, lStyle);

        SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 0, LWA_COLORKEY);

        GdiplusStartupInput gdiplusStartupInput;
        ULONG_PTR           gdiplusToken;
        Status st = GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

        textColorPrimary = Color(255, 255, 255);
        textColorSecondary = Color(255, 255, 255);
        backgroundColorPrimary = Color(0, 0, 0);
        backgroundColorSecondary = Color(0, 0, 0);

        backgroundGradientRotationMatrix = new Matrix();

        SetTimer(hwnd, 66, 66, NULL);

        Show();
    }
};

