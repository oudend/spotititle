#include "SubtitleWindow.h"

void SubtitleWindow::SetDimensions(int width, int height)
{
    this->width = width;
    this->height = height;
}

//void SubtitleWindow::SetFont(const char* name, int size)
//{
//    hFont = CreateFontA(size, 0, 1, ORIENTATION_PREFERENCE_NONE, FW_SEMIBOLD,
//        FW_DONTCARE, FW_DONTCARE, FW_DONTCARE, ANSI_CHARSET,
//        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FIXED_PITCH,
//        name);
//}

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

//void SubtitleWindow::SetFont(HFONT font)
//{
//    hFont = font;
//}

void SubtitleWindow::Update()
{
    backgroundGradientRotationMatrix->Rotate(2.0f); // 45 degree rotation
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
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
        std::wstring wcDisplayText = converter.from_bytes(subtitleWindow->displayText);

        PAINTSTRUCT ps;

        HDC hdc = BeginPaint(hwnd, &ps);

        RECT rect;
        GetClientRect(hwnd, &rect);

        RectF clientRect(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top);

        HDC hdcBuffer = CreateCompatibleDC(hdc);
        HBITMAP hbmBuffer = CreateCompatibleBitmap(hdc, rect.right, rect.bottom);
        HBITMAP hbmOldBuffer = (HBITMAP)SelectObject(hdcBuffer, hbmBuffer);

        //HBRUSH brush2 = CreateSolidBrush(RGB(0, 0, 0));

        //FillRect(hdc, &ps.rcPaint, brush2);

        Graphics gr(hdcBuffer);

        SolidBrush solidBrush(Color::Black);

        gr.FillRectangle(&solidBrush, RectF(ps.rcPaint.left, ps.rcPaint.top, (ps.rcPaint.right - ps.rcPaint.left), (ps.rcPaint.bottom - ps.rcPaint.top)));

        FontFamily fontFamily(L"Consolas");

        //FontFamily::GenericSerif()
        Font font(&fontFamily, subtitleWindow->fontSize, FontStyleRegular);

        StringFormat format;
        format.SetAlignment(StringAlignmentCenter);
        format.SetLineAlignment(StringAlignmentNear);

        RectF boundRect;
        
        PointF point((ps.rcPaint.right - ps.rcPaint.left) * 0.5, 0);
        
        gr.MeasureString(wcDisplayText.c_str(), -1,
            &font, point, &format, &boundRect);

        LinearGradientBrush textBrush(boundRect, subtitleWindow->textColorPrimary, subtitleWindow->textColorSecondary, LinearGradientModeHorizontal);
        LinearGradientBrush backgroundBrush(boundRect, subtitleWindow->backgroundColorPrimary, subtitleWindow->backgroundColorSecondary, LinearGradientModeHorizontal);

        textBrush.SetGammaCorrection(TRUE);
        backgroundBrush.SetGammaCorrection(TRUE);

        gr.FillRectangle(&backgroundBrush, boundRect);

        Status st = gr.DrawString(wcDisplayText.c_str(), -1, &font, point, &format, &textBrush);

        BitBlt(hdc, 0, 0, rect.right, rect.bottom, hdcBuffer, 0, 0, SRCCOPY);


        SelectObject(hdcBuffer, hbmOldBuffer);
        DeleteObject(hbmBuffer);
        DeleteDC(hdcBuffer);

        EndPaint(hwnd, &ps);
    }   
    break;
    case WM_SIZING:
    {
        subtitleWindow->Update();
    }
    break;
    case WM_TIMER:
    {
        subtitleWindow->Update();
        return 0;
    }
    break;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
