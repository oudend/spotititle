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

#include "Spotify.h"

class SubtitleWindow
{
public:
    const char* displayText;

    COLORREF textColor = RGB(15, 15, 15);
    COLORREF backgroundColor = RGB(200, 200, 255);

    HWND hwnd;

    HFONT hFont;

    int width;
    int height;

    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    void SetDimensions(int width, int height);

    void SetFont(const char* name, int size);

    void SetFont(HFONT font);

    void SetText(const char* text);

    void Update();

    SubtitleWindow(const char* displayText, HINSTANCE hInstance, int nCmdShow, int width, int height, int x, int y) : displayText(displayText), width(width), height(height) {
        const wchar_t CLASS_NAME[] = L"Subtitle Window Class";

        WNDCLASS subtitleWc = {};

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

        HFONT hFont = CreateFontW(30, 0, 1, ORIENTATION_PREFERENCE_NONE, FW_SEMIBOLD,
            FW_DONTCARE, FW_DONTCARE, FW_DONTCARE, ANSI_CHARSET,
            OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FIXED_PITCH,
            TEXT("Consolas"));

        ShowWindow(hwnd, nCmdShow);
    }
};

