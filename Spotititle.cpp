#ifndef UNICODE
#define UNICODE
#endif 
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)
#pragma comment(lib, "msimg32.lib")

/*
  Name:        Spotititle.cpp
  Created:     ¯\_(ツ)_/¯
  Author:      Martin Terner (https://github.com/oudend/spotititle)
  Description: Uses non-public spotify api for parsing and displaying subtitles based on synced lyrics for currently playing song.
*/


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

#include "libs/romaji/romaji.h"
#include "resource1.h"
#include "Spotify.h"


#include "StylizedSubtitleWindow.h";

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_IMPLEMENTATION
#define NK_GDI_IMPLEMENTATION
#include "nuklear/nuklear.h"
#include "nuklear/nuklear_gdi.h"

#define NOT_LISTENING "not listening"
#define NOT_SYNCED "lyrics not synced"
#define NOT_CONNECTED "not connected(sp_dc needed)"

#define MAX_ERROR_COUNT 10


const char* nextDisplayText = "not listening";
const char* currentSong = "";

int idleUpdateDelay = 2000;

unsigned long int nextDisplayTextDelay = idleUpdateDelay;
unsigned long int nextDisplayTextProgress = 0;
unsigned long int progress;

static int kanaToRomaji = 0;
static int showLyrics = 1;
int delayOffset = 0;

int WINDOW_WIDTH = 300;
int WINDOW_HEIGHT = 800;

char SP_DC[200];

Spotify spotify;

HANDLE hTimer = NULL;
HANDLE hTimerQueue = NULL;

Spotify::LyricsData lyricsData;
Spotify::CurrentSongData songData;

unsigned long int serverErrorCount = 0;

std::string romaji;

static LRESULT CALLBACK WindowProcGUI(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

bool SP_DC_VALID = false;

int fontSize = 30;

void calculateSubtitles(StylizedSubtitleWindow *subtitleWindow) {
    songData = spotify.getCurrentlyPlaying();

    if (songData.code == Spotify::ReturnCode::SERVER_ERROR) {
        serverErrorCount++;
    }
    else {
        serverErrorCount = 0;
    }

    if ((!songData.listening && serverErrorCount > MAX_ERROR_COUNT && songData.code == Spotify::ReturnCode::SERVER_ERROR) || (!songData.listening && songData.code == Spotify::ReturnCode::OK)) {
        nextDisplayTextDelay = idleUpdateDelay;
        nextDisplayText = NOT_LISTENING;

        subtitleWindow->SetText(NOT_LISTENING);
        return;
    }

    const char* name = songData.name;

    if (songData.code == Spotify::ReturnCode::SERVER_ERROR) {
        name = currentSong;
        progress += nextDisplayTextDelay;
    } else {
        progress = songData.progress;
    }


    if (progress >= nextDisplayTextProgress) {
        subtitleWindow->SetText(_strdup(nextDisplayText));
    }

    if (strcmp(name, currentSong) != 0) {
        lyricsData = spotify.getLyrics(songData.id);
        currentSong = name;

        if (lyricsData.syncType == Spotify::SyncType::LINE_SYNCED && !lyricsData.lyricsTimeData.empty()) {
            Spotify::LyricData currentLyricData = Spotify::calculateCurrentLyric(progress, lyricsData.lyricsTimeData);

            if (progress - currentLyricData.closestLyricsTimeData.elapsedTime > 0) //we have not reached the first line lyric yet
            {
                subtitleWindow->SetText(currentSong);
            }
            else {
                subtitleWindow->SetText(currentLyricData.lineLyric);
            }
        }
        else {
            subtitleWindow->SetText(currentSong);
            nextDisplayText = currentSong;
            return;
        }

        subtitleWindow->SetImage(songData.albumImageLink);

    }

    if (lyricsData.syncType != Spotify::SyncType::LINE_SYNCED || lyricsData.lyricsTimeData.empty()) {
        nextDisplayTextDelay = idleUpdateDelay;
        nextDisplayText = NOT_SYNCED;
        subtitleWindow->SetText(NOT_SYNCED);
    }
    else
    {
        Spotify::LyricData currentLyricData = Spotify::calculateNextLyric(progress, lyricsData.lyricsTimeData);
        nextDisplayTextDelay = std::min<unsigned long int>(currentLyricData.timeLeft + delayOffset, idleUpdateDelay);
        nextDisplayTextProgress = currentLyricData.closestLyricsTimeData.elapsedTime + delayOffset;
        nextDisplayText = currentLyricData.lineLyric;

        if (kanaToRomaji == 1) {
            romaji = "";
            try {
                japanese::utf8_kana_to_romaji(currentLyricData.lineLyric, romaji);
                nextDisplayText = romaji.c_str();
            }
            catch (const std::exception& e) {
                nextDisplayText = currentLyricData.lineLyric;
            }
        }
    }
}

VOID CALLBACK TimerRoutine(PVOID lpParam, BOOLEAN TimerOrWaitFired)
{
    calculateSubtitles((StylizedSubtitleWindow*)lpParam);

    if (!ChangeTimerQueueTimer(hTimerQueue, hTimer, nextDisplayTextDelay, nextDisplayTextDelay))
        OutputDebugStringW(L"ChangeTimerQueueTimer failed\n");
}


int initializeTimer(StylizedSubtitleWindow *subtitleWindow) {
    hTimerQueue = CreateTimerQueue();
    if (NULL == hTimerQueue)
    {
        OutputDebugStringW(L"CreateTimerQueue failed (%d)\n");
        return 2;
    }

    if (!CreateTimerQueueTimer(&hTimer, hTimerQueue,
        (WAITORTIMERCALLBACK)TimerRoutine, subtitleWindow, 1000, 1000, 0))
    {
        OutputDebugStringW(L"CreateTimerQueueTimer failed (%d)\n");
        return 3;
    }

    OutputDebugStringW(L"Call timer routine in 1 second...\n");

    return 0;
}

void setFontSize(StylizedSubtitleWindow* subtitleWindow, int fontSize)
{
    subtitleWindow->fontSize = fontSize;
}

static int theme = StylizedSubtitleWindow::Theme::Purple;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    const int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    const int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    StylizedSubtitleWindow subtitleWindow = StylizedSubtitleWindow(NOT_CONNECTED, hInstance, nCmdShow, screenWidth, screenHeight, 200, 300);

    subtitleWindow.SetTheme(static_cast<StylizedSubtitleWindow::Theme>(theme));

    setFontSize(&subtitleWindow, fontSize);

    GdiFont* font;
    struct nk_context* ctx;

    RECT rect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
    DWORD style = WS_OVERLAPPEDWINDOW | WS_VISIBLE;// (WS_OVERLAPPEDWINDOW | WS_POPUP | WS_SYSMENU) & ~(WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU | WS_BORDER) | WS_VISIBLE;// (WS_OVERLAPPED) & ~WS_CAPTION & ~WS_SYSMENU;
    DWORD exstyle = WS_EX_APPWINDOW;
    HDC dc;

    /* Win32 */
    WNDCLASSW GUIwc;
    memset(&GUIwc, 0, sizeof(GUIwc));
    GUIwc.style = CS_DBLCLKS;
    GUIwc.lpfnWndProc = WindowProcGUI;
    GUIwc.hInstance = GetModuleHandleW(0);
    GUIwc.hCursor = LoadCursor(NULL, IDC_ARROW);
    GUIwc.lpszClassName = L"ToolWindowClass";
    ATOM atom = RegisterClassW(&GUIwc);

    AdjustWindowRectEx(&rect, style, FALSE, exstyle);
    HWND GUIHwnd = CreateWindowExW(exstyle, GUIwc.lpszClassName, L"Spotititle",
        style, CW_USEDEFAULT, CW_USEDEFAULT,
        rect.right - rect.left, rect.bottom - rect.top,
        NULL, NULL, GUIwc.hInstance, NULL);
    dc = GetDC(GUIHwnd);

    /* GUI */
    font = nk_gdifont_create("Consolas", 14);
    ctx = nk_gdi_init(font, dc, WINDOW_WIDTH, WINDOW_HEIGHT);

    MSG msg = { };

    SetTimer(GUIHwnd, 1, 20, NULL);

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

            nk_label(ctx, "SP_DC:", NK_TEXT_CENTERED);
            {
                nk_edit_string(ctx, NK_EDIT_FIELD, SP_DC, &text_len, sizeof(SP_DC) - 1, nk_filter_default);
            }

            nk_layout_row_static(ctx, 30, WINDOW_WIDTH, 1);

            if (nk_widget_is_hovered(ctx))
                nk_tooltip(ctx, "refreshes the access token, can fix lyrics problems.");

            if (nk_button_label(ctx, "update SP_DC")) {
                Spotify::Result result = spotify.refreshAccessToken(SP_DC);

                if (result == Spotify::Result::SUCCESS) {
                    calculateSubtitles(&subtitleWindow);
                    int res = initializeTimer(&subtitleWindow);
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
            nk_layout_row_static(ctx, 30, WINDOW_WIDTH - 100, 1);

            if (nk_checkbox_label(ctx, "lyrics visible", &showLyrics)) {
                if (!showLyrics) {
                    subtitleWindow.Hide();
                }
                else {
                    subtitleWindow.Show();
                }
            }

            nk_layout_row_static(ctx, 30, WINDOW_WIDTH - 100, 1);

            nk_checkbox_label(ctx, "convert kana to romaji", &kanaToRomaji);

            nk_layout_row_dynamic(ctx, 30, 2);
            int prev_theme = theme;
            theme = nk_combo(ctx, subtitleWindow.themes, sizeof(subtitleWindow.themes) / sizeof(subtitleWindow.themes[0]), theme, 25, nk_vec2(200, 200));
            if (theme != prev_theme) {
                subtitleWindow.SetTheme(static_cast<StylizedSubtitleWindow::Theme>(theme));
                //setTheme(&subtitleWindow, theme);
            }


            nk_layout_row_static(ctx, 30, WINDOW_WIDTH, 1);
            nk_label(ctx, "fps", NK_TEXT_CENTERED);

            nk_layout_row_static(ctx, 30, (WINDOW_WIDTH - 100) / 2, 2);
            if (nk_slider_int(ctx, 1, &subtitleWindow.fps, 60, 1)) {
                subtitleWindow.UpdateFPS();
            }
            int fpsTemp = subtitleWindow.fps;
            subtitleWindow.fps = nk_propertyi(ctx, "fps:", 1, subtitleWindow.fps, 60, 1, 0.5f);
            if (fpsTemp != subtitleWindow.fps) {
                subtitleWindow.UpdateFPS();
            }

            nk_layout_row_static(ctx, 30, WINDOW_WIDTH, 1);
            nk_label(ctx, "font size", NK_TEXT_CENTERED);

            nk_layout_row_static(ctx, 30, (WINDOW_WIDTH - 100) / 2, 2);
            if (nk_slider_int(ctx, 6, &fontSize, 50, 1)) {
                setFontSize(&subtitleWindow, fontSize);
            }
            int fontSizeTemp = fontSize;
            fontSize = nk_propertyi(ctx, "size:", 6, fontSize, 50, 1, 0.5f);
            if (fontSizeTemp != fontSize) {
                setFontSize(&subtitleWindow, fontSize);
            }
            

            nk_layout_row_static(ctx, 30, WINDOW_WIDTH, 1);
            nk_label(ctx, "idleUpdateDelay", NK_TEXT_CENTERED);

            nk_layout_row_static(ctx, 30, (WINDOW_WIDTH - 100) / 2, 2);
            nk_slider_int(ctx, 1000, &idleUpdateDelay, 4000, 1);
            idleUpdateDelay = nk_propertyi(ctx, "delay:", 1000, idleUpdateDelay, 4000, 1, 0.5f);

            nk_layout_row_static(ctx, 30, WINDOW_WIDTH, 1);
            nk_label(ctx, "delayOffset", NK_TEXT_CENTERED);

            nk_layout_row_static(ctx, 30, (WINDOW_WIDTH - 100) / 2, 2);
            nk_slider_int(ctx, -2000, &delayOffset, 2000, 1);
            delayOffset = nk_propertyi(ctx, "offset:", -2000, delayOffset, 2000, 1, 0.5f);

            nk_layout_row_static(ctx, 30, WINDOW_WIDTH - 100, 1);
            nk_label(ctx, currentSong, NK_TEXT_CENTERED);

            nk_layout_row_static(ctx, 30, WINDOW_WIDTH - 100, 1);
            nk_label(ctx, "progress", NK_TEXT_CENTERED);

            nk_layout_row_static(ctx, 30, WINDOW_WIDTH - 100, 1);
            nk_label(ctx, std::to_string(progress).c_str(), NK_TEXT_CENTERED);

            nk_layout_row_static(ctx, 30, WINDOW_WIDTH - 100, 1);
            nk_label(ctx, "target progress", NK_TEXT_CENTERED);

            nk_layout_row_static(ctx, 30, WINDOW_WIDTH - 100, 1);
            nk_label(ctx, std::to_string(nextDisplayTextProgress).c_str(), NK_TEXT_CENTERED);

            nk_layout_row_static(ctx, 30, WINDOW_WIDTH - 100, 1);
            nk_label(ctx, "next text", NK_TEXT_CENTERED);

            nk_layout_row_static(ctx, 30, WINDOW_WIDTH - 100, 1);
            nk_label(ctx, nextDisplayText, NK_TEXT_CENTERED);
        }
        nk_end(ctx);

        nk_gdi_render(nk_rgb(30, 30, 30));
    }

    KillTimer(GUIHwnd, 1);

    nk_gdifont_del(font);
    ReleaseDC(GUIHwnd, dc);
    UnregisterClassW(GUIwc.lpszClassName, GUIwc.hInstance);
    UnregisterClassW(subtitleWindow.subtitleWc.lpszClassName, subtitleWindow.subtitleWc.hInstance);

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