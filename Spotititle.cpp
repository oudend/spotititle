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

#include "libs/romaji/romaji.h"
//#include "libs/romaji/romaji.cpp"

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


static LRESULT CALLBACK WindowProcGUI(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_SIZING:
    {
        /* Size of the client / active are is extracted and stored */
        RECT rect;
        GetClientRect(hwnd, &rect);
        WINDOW_WIDTH = rect.right - rect.left;
        WINDOW_HEIGHT = rect.bottom - rect.top;
    }
    break;
    case WM_SIZE:
    {
        RECT rect;
        if (GetWindowRect(hwnd, &rect))
        {
            WINDOW_WIDTH = rect.right - rect.left;
            WINDOW_HEIGHT = rect.bottom - rect.top;
        }
    }
    break;
    }

    if (nk_gdi_handle_event(hwnd, msg, wparam, lparam))
        return 0;

    return DefWindowProcW(hwnd, msg, wparam, lparam);
}

bool SP_DC_VALID = false;
COLORREF textColor = RGB(15, 15, 15);
COLORREF backgroundColor = RGB(200, 200, 255);

int fontSize = 30;

HICON hIconSmall;
HICON hIconBig;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    // Register the window class.
    const wchar_t CLASS_NAME[] = L"Subtitle Window Class";

    WNDCLASS subtitleWc = {};

    hIconSmall = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(IDI_ICON), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
    hIconBig = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(IDI_ICON), IMAGE_ICON, 256, 256, LR_DEFAULTCOLOR);

    subtitleWc.lpfnWndProc = WindowProc;
    subtitleWc.hInstance = hInstance;
    subtitleWc.lpszClassName = CLASS_NAME;
    subtitleWc.hIcon = hIconSmall;

    RegisterClass(&subtitleWc);

    // Create the window.

    const int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    const int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    HWND subtitleHwnd = CreateWindowEx(
        WS_EX_TOPMOST,// | WS_EX_TRANSPARENT,   // Optional window styles.
        CLASS_NAME,                     // Window class
        L"Spotify Lyrics",    // Window text
        WS_OVERLAPPEDWINDOW | WS_POPUP | WS_BORDER,            // Window style

        // Size and position
        screenWidth / 2 - 300, screenHeight - 100 - 20, 600, 100,

        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
    );

    if (subtitleHwnd == NULL)
    {
        return 0;
    }

    long Style = (WS_CAPTION | WS_SYSMENU);
    long a = SetWindowLongA(subtitleHwnd, GWL_STYLE, Style & ~WS_BORDER);


    SetWindowLong(subtitleHwnd, GWL_EXSTYLE, GetWindowLong(subtitleHwnd, GWL_EXSTYLE) | WS_EX_LAYERED);

    LONG lStyle = GetWindowLong(subtitleHwnd, GWL_STYLE);
    lStyle &= ~(WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU);
    SetWindowLong(subtitleHwnd, GWL_STYLE, lStyle);
    
    SetLayeredWindowAttributes(subtitleHwnd, RGB(0, 0, 0), 0, LWA_COLORKEY);

    ShowWindow(subtitleHwnd, nCmdShow);

    std::string romaji;

    const char* kana = "å­£ç¯€ã¯æ¬¡ã€…æ­»ã‚“ã§ã„ã çµ¶å‘½ã®å£°ãŒé¢¨ã«ãªã‚‹ abcdefg blabla";

    japanese::utf8_kana_to_romaji(kana, romaji);

    GdiFont* font;
    struct nk_context* ctx;

    RECT rect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
    DWORD style = WS_OVERLAPPEDWINDOW;// &~WS_SIZEBOX & ~WS_MAXIMIZEBOX;
    DWORD exstyle = WS_EX_APPWINDOW;
    HDC dc;

    /* Win32 */
    WNDCLASSW GUIwc;
    memset(&GUIwc, 0, sizeof(GUIwc));
    GUIwc.style = CS_DBLCLKS;
    GUIwc.lpfnWndProc = WindowProcGUI;
    GUIwc.hInstance = GetModuleHandleW(0);
    GUIwc.hIcon = hIconSmall;
    GUIwc.hCursor = LoadCursor(NULL, IDC_ARROW);
    GUIwc.lpszClassName = L"ToolWindowClass";
    ATOM atom = RegisterClassW(&GUIwc);

    AdjustWindowRectEx(&rect, style, FALSE, exstyle);
    HWND GUIHwnd = CreateWindowExW(exstyle, GUIwc.lpszClassName, L"Spotititle",
        style | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT,
        rect.right - rect.left, rect.bottom - rect.top,
        NULL, NULL, GUIwc.hInstance, NULL);
    dc = GetDC(GUIHwnd);

    /* GUI */
    font = nk_gdifont_create("Consolas", 14);
    ctx = nk_gdi_init(font, dc, WINDOW_WIDTH, WINDOW_HEIGHT);

    MSG msg = { };

    //timer for checking if the cursor is hovering the window.
    //SetTimer(subtitleHwnd, 1, 20, NULL);

    int running = 1;
    int needs_refresh = 1;

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
            static struct nk_colorf currentTextColor = { 0.058f, 0.058f, 0.058f };
            static struct nk_colorf currentBackgroundColor = { 0.509f, 0.705f, 1.0f };

            static struct nk_colorf tempTextColor = { 0.509f, 0.705f, 0.2f };
            static struct nk_colorf tempBackgroundColor = { 0.78f, 0.78f, 0.2f };

            static int text_len;

            nk_layout_row_static(ctx, 30, WINDOW_WIDTH, 1);
            if (nk_widget_is_hovered(ctx))
                nk_tooltip(ctx, "SP_DC");

            nk_label(ctx, "SP_DC:", NK_TEXT_LEFT);
            {
                nk_edit_string(ctx, NK_EDIT_FIELD, SP_DC, &text_len, sizeof(SP_DC) - 1, nk_filter_default);
            }

            nk_layout_row_static(ctx, 30, WINDOW_WIDTH, 1);

            if (nk_widget_is_hovered(ctx))
                nk_tooltip(ctx, "refreshes the access token, can fix lyrics problems.");

            if (nk_button_label(ctx, "update SP_DC")) {
                Spotify::Result result = spotify.refreshAccessToken(SP_DC);

                if (result == Spotify::Result::SUCCESS) {
                    calculateSubtitles(subtitleHwnd);
                    int res = initializeTimer(&subtitleHwnd);
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
                nk_layout_row_static(ctx, 30, WINDOW_WIDTH - 100, 1);
                nk_label_colored(ctx, "invalid SP_DC", NK_TEXT_LEFT, nk_rgb(255, 0, 0));
            }

            nk_layout_row_static(ctx, 30, WINDOW_WIDTH - 100, 1);
            nk_label(ctx, "toggle", NK_TEXT_CENTERED);
            nk_layout_row_static(ctx, 30, WINDOW_WIDTH - 100, 1);

            nk_checkbox_label(ctx, "lyrics visible", &showLyrics);

            nk_layout_row_static(ctx, 30, WINDOW_WIDTH - 100, 1);
            nk_label(ctx, "text color", NK_TEXT_CENTERED);

            if (nk_combo_begin_color(ctx, nk_rgb_cf(currentTextColor), nk_vec2(WINDOW_WIDTH - 100, 400))) {
                enum color_mode { COL_RGB, COL_HSV };
                static int col_mode = COL_RGB;
                /*nk_layout_row_dynamic(ctx, 120, 1);
                currentTextColor = nk_color_picker(ctx, currentTextColor, NK_RGB);*/

                nk_layout_row_dynamic(ctx, 25, 2);
                col_mode = nk_option_label(ctx, "RGB", col_mode == COL_RGB) ? COL_RGB : col_mode;
                col_mode = nk_option_label(ctx, "HSV", col_mode == COL_HSV) ? COL_HSV : col_mode;

                nk_layout_row_static(ctx, 30, WINDOW_WIDTH - 100, 1);
                if (col_mode == COL_RGB) {
                    currentTextColor.r = nk_propertyf(ctx, "#R:", 0, currentTextColor.r, 1.0f, 0.01f, 0.005f);
                    currentTextColor.g = nk_propertyf(ctx, "#G:", 0, currentTextColor.g, 1.0f, 0.01f, 0.005f);
                    currentTextColor.b = nk_propertyf(ctx, "#B:", 0, currentTextColor.b, 1.0f, 0.01f, 0.005f);
                    textColor = RGB(currentTextColor.r * 255.0f, currentTextColor.g * 255.0f, currentTextColor.b * 255.0f);
                }
                else {
                    float hsva[4];
                    nk_colorf_hsva_fv(hsva, currentTextColor);
                    hsva[0] = nk_propertyf(ctx, "#H:", 0, hsva[0], 1.0f, 0.01f, 0.05f);
                    hsva[1] = nk_propertyf(ctx, "#S:", 0, hsva[1], 1.0f, 0.01f, 0.05f);
                    hsva[2] = nk_propertyf(ctx, "#V:", 0, hsva[2], 1.0f, 0.01f, 0.05f);
                    currentTextColor = nk_hsva_colorfv(hsva);
                    textColor = RGB(currentTextColor.r * 255.0f, currentTextColor.g * 255.0f, currentTextColor.b * 255.0f);
                }

                if (tempTextColor.r != currentTextColor.r || tempTextColor.g != currentTextColor.g || tempTextColor.b != currentTextColor.b) {
                    RedrawWindow(subtitleHwnd, NULL, NULL, RDW_INVALIDATE | RDW_ERASE);
                    UpdateWindow(subtitleHwnd);
                    tempTextColor = currentTextColor;
                }

                nk_combo_end(ctx);
            }

            nk_layout_row_static(ctx, 30, WINDOW_WIDTH - 100, 1);
            nk_label(ctx, "background color", NK_TEXT_CENTERED);

            if (nk_combo_begin_color(ctx, nk_rgb_cf(currentBackgroundColor), nk_vec2(WINDOW_WIDTH - 100, 400))) {
                enum color_mode { COL_RGB, COL_HSV };
                static int col_mode = COL_RGB;
                /*nk_layout_row_dynamic(ctx, 120, 1);
                currentBackgroundColor = nk_color_picker(ctx, currentBackgroundColor, NK_RGB);*/

                nk_layout_row_dynamic(ctx, 25, 2);
                col_mode = nk_option_label(ctx, "RGB", col_mode == COL_RGB) ? COL_RGB : col_mode;
                col_mode = nk_option_label(ctx, "HSV", col_mode == COL_HSV) ? COL_HSV : col_mode;

                nk_layout_row_static(ctx, 30, WINDOW_WIDTH / 1.25, 1);
                if (col_mode == COL_RGB) {
                    currentBackgroundColor.r = nk_propertyf(ctx, "#R:", 0, currentBackgroundColor.r, 1.0f, 0.01f, 0.005f);
                    currentBackgroundColor.g = nk_propertyf(ctx, "#G:", 0, currentBackgroundColor.g, 1.0f, 0.01f, 0.005f);
                    currentBackgroundColor.b = nk_propertyf(ctx, "#B:", 0, currentBackgroundColor.b, 1.0f, 0.01f, 0.005f);
                    backgroundColor = RGB(currentBackgroundColor.r * 255.0f, currentBackgroundColor.g * 255.0f, currentBackgroundColor.b * 255.0f);
                }
                else {
                    float hsva[4];
                    nk_colorf_hsva_fv(hsva, currentBackgroundColor);
                    hsva[0] = nk_propertyf(ctx, "#H:", 0, hsva[0], 1.0f, 0.01f, 0.05f);
                    hsva[1] = nk_propertyf(ctx, "#S:", 0, hsva[1], 1.0f, 0.01f, 0.05f);
                    hsva[2] = nk_propertyf(ctx, "#V:", 0, hsva[2], 1.0f, 0.01f, 0.05f);
                    currentBackgroundColor = nk_hsva_colorfv(hsva);
                    backgroundColor = RGB(currentBackgroundColor.r * 255.0f, currentBackgroundColor.g * 255.0f, currentBackgroundColor.b * 255.0f);
                }

                if (tempBackgroundColor.r != currentBackgroundColor.r || tempBackgroundColor.g != currentBackgroundColor.g || tempBackgroundColor.b != currentBackgroundColor.b) {
                    RedrawWindow(subtitleHwnd, NULL, NULL, RDW_INVALIDATE | RDW_ERASE);
                    UpdateWindow(subtitleHwnd);
                    tempBackgroundColor = currentBackgroundColor;
                }

                nk_combo_end(ctx);
            }

            nk_layout_row_static(ctx, 30, WINDOW_WIDTH - 100, 1);
            nk_label(ctx, "font size", NK_TEXT_CENTERED);

            nk_layout_row_static(ctx, 30, (WINDOW_WIDTH - 100) / 2, 2);
            if (nk_slider_int(ctx, 6, &fontSize, 50, 1)) {
                RedrawWindow(subtitleHwnd, NULL, NULL, RDW_INVALIDATE | RDW_ERASE);
                UpdateWindow(subtitleHwnd);
            }
            int fontSizeTemp = fontSize;
            fontSize = nk_propertyi(ctx, "size:", 6, fontSize, 50, 1, 0.05f);
            if (fontSizeTemp != fontSize) {
                RedrawWindow(subtitleHwnd, NULL, NULL, RDW_INVALIDATE | RDW_ERASE);
                UpdateWindow(subtitleHwnd);
            }
        }
        nk_end(ctx);

        nk_gdi_render(nk_rgb(30, 30, 30));
    }

    KillTimer(subtitleHwnd, 1);

    nk_gdifont_del(font);
    ReleaseDC(GUIHwnd, dc);
    UnregisterClassW(subtitleWc.lpszClassName, subtitleWc.hInstance);

    return 0;
}

RECT textRect = {0, 0, 0, 0};

bool hovered = false;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_TIMER:
        {
            //if (wParam == 1) // Check the timer ID
            //{
            //    POINT point;
            //    if (GetCursorPos(&point)) {
            //        if (point.x > textRect.left && point.x < textRect.right && point.y < textRect.bottom&& point.y > textRect.top) {
            //            if (hovered == true)
            //                return 0;
            //            backgroundColor = RGB(0, 0, 0);
            //            hovered = true;
            //            RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE | RDW_ERASE);
            //            UpdateWindow(hwnd);
            //        }
            //        else if (hovered == true) {
            //            hovered = false;
            //            textColor = RGB(15, 15, 15);
            //            backgroundColor = RGB(200, 200, 255);
            //            RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE | RDW_ERASE);
            //            UpdateWindow(hwnd);
            //        }
            //    }
            //}
            return 0;
        }
        break;
        case WM_DESTROY:
        {
            PostQuitMessage(0);
            return 0;
        }
        break;
        case WM_SETCURSOR:
        {
            HCURSOR hCursor = LoadCursorW(NULL, IDC_ARROW);
            SetCursor(hCursor);
            return 1;
        }
        break;
        case WM_NCHITTEST:
        {
            // Make the entire window draggable
            return HTCAPTION;
        }
        break;
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            HBRUSH brush = CreateSolidBrush(RGB(0, 0, 0));

            FillRect(hdc, &ps.rcPaint, brush);

            HFONT hFont = CreateFontW(fontSize, 0, 1, ORIENTATION_PREFERENCE_NONE, FW_SEMIBOLD,
                FW_DONTCARE, FW_DONTCARE, FW_DONTCARE, ANSI_CHARSET,
                OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FIXED_PITCH,
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

            DrawTextExW(hdc, lpDisplayText, wcslen(lpDisplayText), &rect, DT_WORDBREAK | DT_CENTER | DT_NOCLIP | DT_EDITCONTROL | DT_END_ELLIPSIS, NULL);
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
        }
        break;
        case WM_SIZING:
        {
            RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE | RDW_ERASE);
            UpdateWindow(hwnd);
        }
        break;

        case WM_CREATE:
        {
            SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIconSmall);
            SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIconBig);
            return 0;
        }
        break;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}