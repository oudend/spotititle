#ifndef UNICODE
#define UNICODE
#endif 
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)
#pragma comment(lib, "msimg32.lib")

//a

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

#include "resource1.h"

#include "Spotify.h"


#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <time.h>

int WINDOW_WIDTH = 300;
int WINDOW_HEIGHT = 600;

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_IMPLEMENTATION
#define NK_GDI_IMPLEMENTATION
#include "nuklear/nuklear.h"
#include "nuklear/nuklear_gdi.h"

const char* displayText = "not listening";

const char* nextDisplayText = "not listening";
const char* currentSong = "";

unsigned long int idleUpdateDelay = 2000;

unsigned long int rateLimitDelay = 10000;

unsigned long int nextDisplayTextDelay = idleUpdateDelay;
unsigned long int nextDisplayTextProgress = 0;
unsigned long int progress;

static int showLyrics = 0;

char SP_DC[200];

Spotify spotify;

HANDLE hTimer = NULL;
HANDLE hTimerQueue = NULL;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

Spotify::LyricsData lyricsData;
Spotify::CurrentSongData songData;

void calculateSubtitles(HWND hwnd) {
    songData = spotify.getCurrentlyPlaying();

    if (!songData.listening) {
        nextDisplayTextDelay = idleUpdateDelay;
        displayText = "not listening";
        nextDisplayText = displayText;

        RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE | RDW_ERASE);
        UpdateWindow(hwnd);
        return;
    }

    //if (songData.listening) {
    const char* name = songData.name;
    progress = songData.progress;

    if (progress >= nextDisplayTextProgress) {
        displayText = nextDisplayText;
        RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE | RDW_ERASE);
        UpdateWindow(hwnd);
    }

    if (strcmp(name, currentSong) != 0) {
        lyricsData = spotify.getLyrics(songData.id);

        currentSong = name;
        displayText = currentSong;
        RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE | RDW_ERASE);
        UpdateWindow(hwnd);
    }

    if (lyricsData.syncType != Spotify::SyncType::LINE_SYNCED || lyricsData.lyricsTimeData.empty()) {
        nextDisplayTextDelay = idleUpdateDelay;
        displayText = "lyrics not synced";
        nextDisplayText = "lyrics not synced";
        RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE | RDW_ERASE);
        UpdateWindow(hwnd);
    }
    else
    {
        Spotify::LyricData currentLyricData = Spotify::calculateNextLyric(progress, lyricsData.lyricsTimeData);
        nextDisplayTextDelay = std::min<unsigned long int>(currentLyricData.timeLeft, idleUpdateDelay);
        nextDisplayTextProgress = currentLyricData.closestLyricsTimeData.elapsedTime;
        nextDisplayText = currentLyricData.lineLyric;
    }
}

VOID CALLBACK TimerRoutine(PVOID lpParam, BOOLEAN TimerOrWaitFired)
{
    HWND hwnd = *(HWND*)lpParam;

    progress += nextDisplayTextDelay;

    if (progress >= nextDisplayTextProgress) {
        displayText = nextDisplayText;
        RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE | RDW_ERASE);
        UpdateWindow(hwnd);
    }

    calculateSubtitles(hwnd);

    if (!ChangeTimerQueueTimer(hTimerQueue, hTimer, nextDisplayTextDelay, nextDisplayTextDelay))
        OutputDebugStringW(L"ChangeTimerQueueTimer failed\n");
}


int initializeTimer(HWND *hwnd) {
    hTimerQueue = CreateTimerQueue();
    if (NULL == hTimerQueue)
    {
        OutputDebugStringW(L"CreateTimerQueue failed (%d)\n");
        return 2;
    }

    if (!CreateTimerQueueTimer(&hTimer, hTimerQueue,
        (WAITORTIMERCALLBACK)TimerRoutine, hwnd, 1000, 1000, 0))
    {
        OutputDebugStringW(L"CreateTimerQueueTimer failed (%d)\n");
        return 3;
    }

    OutputDebugStringW(L"Call timer routine in 1 second...\n");

    return 0;
}


static LRESULT CALLBACK WindowProc2(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_SIZING:
    {
        /* Size of the client / active are is extracted and stored */
        RECT cr;
        GetClientRect(wnd, &cr);
        WINDOW_WIDTH = cr.right - cr.left;
        WINDOW_HEIGHT = cr.bottom - cr.top;
    }
    break;
    case WM_SIZE:
    {
        RECT rect;
        if (GetWindowRect(wnd, &rect))
        {
            WINDOW_WIDTH = rect.right - rect.left;
            WINDOW_HEIGHT = rect.bottom - rect.top;
        }
    }
    break;
    }

    if (nk_gdi_handle_event(wnd, msg, wparam, lparam))
        return 0;

    return DefWindowProcW(wnd, msg, wparam, lparam);
}

bool SP_DC_VALID = false;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    // Register the window class.
    const wchar_t CLASS_NAME[] = L"Subtitle Window Class";

    WNDCLASS wc = { };

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    // Create the window.

    const int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    const int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    HWND hwnd = CreateWindowEx(
        WS_EX_TOPMOST | WS_EX_TRANSPARENT,   // Optional window styles.
        CLASS_NAME,                     // Window class
        L"Spotify Lyrics",    // Window text
        WS_OVERLAPPEDWINDOW,            // Window style

        // Size and position
        screenWidth / 2 - 300, screenHeight - 100 - 20, 600, 100,

        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
    );

    if (hwnd == NULL)
    {
        return 0;
    }

    long Style = (WS_CAPTION | WS_SYSMENU);
    long a = SetWindowLongA(hwnd, GWL_STYLE, Style & ~WS_BORDER);


    SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);

    LONG lStyle = GetWindowLong(hwnd, GWL_STYLE);
    lStyle &= ~(WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU);
    SetWindowLong(hwnd, GWL_STYLE, lStyle);
    
    SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 0, LWA_COLORKEY);

    ShowWindow(hwnd, nCmdShow);

    GdiFont* font;
    struct nk_context* ctx;

    WNDCLASSW wc2;
    ATOM atom;
    RECT rect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
    DWORD style = WS_OVERLAPPEDWINDOW;// &~WS_SIZEBOX & ~WS_MAXIMIZEBOX;
    DWORD exstyle = WS_EX_APPWINDOW;
    HWND wnd;
    HDC dc;
    int running = 1;
    int needs_refresh = 1;

    /* Win32 */
    memset(&wc2, 0, sizeof(wc2));
    wc2.style = CS_DBLCLKS;
    wc2.lpfnWndProc = WindowProc2;
    wc2.hInstance = GetModuleHandleW(0);
    wc2.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc2.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc2.lpszClassName = L"NuklearWindowClass";
    atom = RegisterClassW(&wc2);

    AdjustWindowRectEx(&rect, style, FALSE, exstyle);
    wnd = CreateWindowExW(exstyle, wc2.lpszClassName, L"Spotititle tool window",
        style | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT,
        rect.right - rect.left, rect.bottom - rect.top,
        NULL, NULL, wc2.hInstance, NULL);
    dc = GetDC(wnd);

    /* GUI */
    font = nk_gdifont_create("Consolas", 14); //Arial
    ctx = nk_gdi_init(font, dc, WINDOW_WIDTH, WINDOW_HEIGHT);

    MSG msg = { };
    SetTimer(hwnd, 1, 20, NULL);

    while (running) {
        /* Input */
        MSG msg;
        nk_input_begin(ctx);
        if (needs_refresh == 0) {
            if (GetMessageW(&msg, NULL, 0, 0) <= 0)
                running = 0;
            else {
                TranslateMessage(&msg);
                DispatchMessageW(&msg);
            }
            needs_refresh = 1;
        }
        else needs_refresh = 0;

        while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT)
                running = 0;
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
            needs_refresh = 1;
        }
        nk_input_end(ctx);

        if (nk_begin(ctx, "Settings", nk_rect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT), NK_WINDOW_NO_SCROLLBAR))
        {
            static struct nk_colorf combo_color2 = { 0.509f, 0.705f, 0.2f };

            static int text_len;

            nk_layout_row_static(ctx, 30, WINDOW_WIDTH, 1);
            if (nk_widget_is_hovered(ctx))
                nk_tooltip(ctx, "SP_DC");

            nk_label(ctx, "SP_DC:", NK_TEXT_LEFT);
            {
                int i = 0;
                //nk_edit_string_zero_terminated(ctx, NK_EDIT_FIELD, SP_DC_buffer, sizeof(SP_DC_buffer) - 1, nk_filter_default);
                nk_edit_string(ctx, NK_EDIT_FIELD, SP_DC, &text_len, sizeof(SP_DC) - 1, nk_filter_default);
            }

            nk_layout_row_static(ctx, 30, WINDOW_WIDTH, 1);

            if (nk_widget_is_hovered(ctx))
                nk_tooltip(ctx, "refreshes the access token, can fix lyrics problems.");

            if (nk_button_label(ctx, "update SP_DC")) {
                Spotify::Result result = spotify.refreshAccessToken(SP_DC);

                if (result == Spotify::Result::SUCCESS) {
                    calculateSubtitles(hwnd);
                    int res = initializeTimer(&hwnd);
                    if (res) {
                        return res;
                    }
                    SP_DC_VALID = true;
                }
                else {
                    SP_DC_VALID = false;
                }
            }

            if (!SP_DC_VALID) {
                nk_layout_row_static(ctx, 30, WINDOW_WIDTH, 1);
                nk_label_colored(ctx, "invalid SP_DC", NK_TEXT_LEFT, nk_rgb(255, 0, 0));
            }

            nk_layout_row_static(ctx, 30, WINDOW_WIDTH, 1);
            nk_label(ctx, "toggle", NK_TEXT_CENTERED);
            nk_layout_row_static(ctx, 30, WINDOW_WIDTH, 1);

            nk_checkbox_label(ctx, "lyrics visible", &showLyrics);

            nk_layout_row_static(ctx, 30, WINDOW_WIDTH, 1);
            nk_label(ctx, "text color", NK_TEXT_CENTERED);

            if (nk_combo_begin_color(ctx, nk_rgb_cf(combo_color2), nk_vec2(WINDOW_WIDTH / 1.25, 400))) {
                enum color_mode { COL_RGB, COL_HSV };
                static int col_mode = COL_RGB;
                /*nk_layout_row_dynamic(ctx, 120, 1);
                combo_color2 = nk_color_picker(ctx, combo_color2, NK_RGB);*/

                nk_layout_row_dynamic(ctx, 25, 2);
                col_mode = nk_option_label(ctx, "RGB", col_mode == COL_RGB) ? COL_RGB : col_mode;
                col_mode = nk_option_label(ctx, "HSV", col_mode == COL_HSV) ? COL_HSV : col_mode;

                nk_layout_row_static(ctx, 30, WINDOW_WIDTH / 1.25, 1);
                if (col_mode == COL_RGB) {
                    combo_color2.r = nk_propertyf(ctx, "#R:", 0, combo_color2.r, 1.0f, 0.01f, 0.005f);
                    combo_color2.g = nk_propertyf(ctx, "#G:", 0, combo_color2.g, 1.0f, 0.01f, 0.005f);
                    combo_color2.b = nk_propertyf(ctx, "#B:", 0, combo_color2.b, 1.0f, 0.01f, 0.005f);
                }
                else {
                    float hsva[4];
                    nk_colorf_hsva_fv(hsva, combo_color2);
                    hsva[0] = nk_propertyf(ctx, "#H:", 0, hsva[0], 1.0f, 0.01f, 0.05f);
                    hsva[1] = nk_propertyf(ctx, "#S:", 0, hsva[1], 1.0f, 0.01f, 0.05f);
                    hsva[2] = nk_propertyf(ctx, "#V:", 0, hsva[2], 1.0f, 0.01f, 0.05f);
                    combo_color2 = nk_hsva_colorfv(hsva);
                }
                nk_combo_end(ctx);
            }
        }
        nk_end(ctx);

        nk_gdi_render(nk_rgb(30, 30, 30));
    }
    //while (GetMessage(&msg, NULL, 0, 0) > 0)
    //{
    //    TranslateMessage(&msg);
    //    DispatchMessage(&msg);
    //}

    KillTimer(hwnd, 1);

    nk_gdifont_del(font);
    ReleaseDC(wnd, dc);
    UnregisterClassW(wc.lpszClassName, wc.hInstance);

    return 0;
}

COLORREF textColor = RGB(15, 15, 15);
COLORREF backgroundColor = RGB(200, 200, 255);

RECT textRect = {0, 0, 0, 0};

bool hovered = false;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_TIMER:
    {
        if (wParam == 1) // Check the timer ID
        {
            POINT point;
            if (GetCursorPos(&point)) {
                if (point.x > textRect.left && point.x < textRect.right && point.y < textRect.bottom&& point.y > textRect.top) {
                    if (hovered == true)
                        return 0;
                    //textColor = RGB(255, 255, 255);
                    backgroundColor = RGB(0, 0, 0);
                    hovered = true;
                    RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE | RDW_ERASE);
                    UpdateWindow(hwnd);
                }
                else if (hovered == true) {
                    hovered = false;
                    textColor = RGB(15, 15, 15);
                    backgroundColor = RGB(200, 200, 255);
                    RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE | RDW_ERASE);
                    UpdateWindow(hwnd);
                }
            }
        }
        return 0;
    }
    case WM_DESTROY:
    {
        PostQuitMessage(0);
        return 0;
    }
    case WM_SETCURSOR:
    {
        HCURSOR hCursor = LoadCursorW(NULL, IDC_ARROW);
        SetCursor(hCursor);
        return 1;
    }
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        HBRUSH brush = CreateSolidBrush(RGB(0, 0, 0));

        FillRect(hdc, &ps.rcPaint, brush);

        HFONT hFont = CreateFontW(20, 0, 1, ORIENTATION_PREFERENCE_NONE, FW_SEMIBOLD,
            FW_DONTCARE, FW_DONTCARE, FW_DONTCARE, ANSI_CHARSET,
            OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH,
            TEXT("Consolas")/*"SYSTEM_FIXED_FONT"*/);
        HFONT hOldFont = (HFONT)SelectObject(hdc, hFont); // <-- add this

        const char* lpLogFont = "";

        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter; // Create a converter object
        std::wstring wcDisplayText = converter.from_bytes(displayText); // Convert the multibyte string to wide-character string


        OutputDebugStringA(lpLogFont);

        RECT rect;
        GetClientRect(hwnd, &rect);

        GetClientRect(hwnd, &textRect);

        RECT windowRect;
        GetWindowRect(hwnd, &windowRect);
                                       
        SetTextColor(hdc, textColor);
        SetBkColor(hdc, backgroundColor);

        LPWSTR lpDisplayText = &wcDisplayText[0];

        //textRect = { 0, 0, 0, 0 };


        DrawTextExW(hdc, lpDisplayText/*(char*)displayText*/, wcslen(lpDisplayText), &rect, DT_WORDBREAK | DT_CENTER | DT_NOCLIP | DT_EDITCONTROL | DT_END_ELLIPSIS, NULL);
        DrawTextExW(hdc, lpDisplayText, -1, &textRect, DT_WORDBREAK | DT_CENTER | DT_NOCLIP | DT_EDITCONTROL | DT_END_ELLIPSIS | DT_CALCRECT, NULL);

        int textWidth = textRect.right - textRect.left;
        int textHeight = textRect.bottom - textRect.top;

        RECT clientRect;
        GetClientRect(hwnd, &clientRect);

        textRect.left = (clientRect.right - textWidth) / 2;
        textRect.top = clientRect.top;

        textRect.right = textRect.left + textWidth;
        textRect.bottom = textRect.top + textHeight;

        POINT pt = { textRect.left, textRect.top };
        ClientToScreen(hwnd, &pt);

        textRect.left = pt.x;
        textRect.top = pt.y;

        pt = { textRect.right, textRect.bottom };
        ClientToScreen(hwnd, &pt);

        textRect.right = pt.x;
        textRect.bottom = pt.y;

        SelectObject(hdc, hOldFont); // <-- add this
        DeleteObject(hFont);  // <-- add this

        EndPaint(hwnd, &ps);

        break;
    }
    case WM_SIZING:
    {
        RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE | RDW_ERASE);
        UpdateWindow(hwnd);
        break;
    }

    case WM_CREATE:
    {
        HINSTANCE hInstance = ((LPCREATESTRUCT)lParam)->hInstance;
        HICON hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON));
        SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
        SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
        return 0;
    }
    return 0;

    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}