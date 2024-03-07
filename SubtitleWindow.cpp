#include "SubtitleWindow.h"

void SubtitleWindow::SetDimensions(int width, int height)
{
    this->width = width;
    this->height = height;
}

void SubtitleWindow::SetFont(const char* name, int size)
{
    hFont = CreateFontA(size, 0, 1, ORIENTATION_PREFERENCE_NONE, FW_SEMIBOLD,
        FW_DONTCARE, FW_DONTCARE, FW_DONTCARE, ANSI_CHARSET,
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FIXED_PITCH,
        name);
}

void SubtitleWindow::SetText(const char* text)
{
    displayText = text;
}

void SubtitleWindow::Hide()
{
    ShowWindow(hwnd, SW_HIDE);
}

void SubtitleWindow::Show()
{
    ShowWindow(hwnd, SW_SHOW);
}

void SubtitleWindow::SetFont(HFONT font)
{
    hFont = font;
}

void SubtitleWindow::Update()
{
    RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE | RDW_ERASE);
    UpdateWindow(hwnd);
}

LRESULT SubtitleWindow::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    SubtitleWindow* subtitleWindow = (SubtitleWindow*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

    switch (uMsg)
    {
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

        HFONT hOldFont = (HFONT)SelectObject(hdc, subtitleWindow->hFont);

        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter; // Create a converter object
        std::wstring wcDisplayText = converter.from_bytes(subtitleWindow->displayText); // Convert the multibyte string to wide-character string

        RECT rect;
        GetClientRect(hwnd, &rect);

        RECT windowRect;
        GetWindowRect(hwnd, &windowRect);

        SetTextColor(hdc, subtitleWindow->textColor);
        SetBkColor(hdc, subtitleWindow->backgroundColor);

        LPWSTR lpDisplayText = &wcDisplayText[0];

        DrawTextExW(hdc, lpDisplayText, wcslen(lpDisplayText), &rect, DT_WORDBREAK | DT_CENTER | DT_NOCLIP | DT_EDITCONTROL | DT_END_ELLIPSIS, NULL);

        SelectObject(hdc, hOldFont);
        //DeleteObject(subtitleWindow->hFont);

        EndPaint(hwnd, &ps);
    }
    break;
    case WM_SIZING:
    {
        subtitleWindow->Update();
    }
    break;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
