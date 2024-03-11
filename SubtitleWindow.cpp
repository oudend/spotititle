#include "SubtitleWindow.h"

void SubtitleWindow::DrawSubtitleBackground(Graphics* graphics, RectF* destRect)
{
    LinearGradientBrush backgroundBrush(*destRect, backgroundColorPrimary, backgroundColorSecondary, LinearGradientModeHorizontal);
    backgroundBrush.SetGammaCorrection(TRUE);
    graphics->FillRectangle(&backgroundBrush, *destRect);
}

void SubtitleWindow::DrawSubtitleText(Graphics* graphics, std::wstring* displayText, PointF* point, Font* font, RectF* destRect)
{
    StringFormat format;
    format.SetAlignment(StringAlignmentCenter);
    format.SetLineAlignment(StringAlignmentNear);

    LinearGradientBrush textBrush(*destRect, textColorPrimary, textColorSecondary, LinearGradientModeHorizontal);

    textBrush.SetGammaCorrection(TRUE);

    GraphicsPath path;

    //path.AddString();

    Point point2(point->X, point->Y);

    FontFamily fontFamily(L"Consolas");

    path.AddString(displayText->c_str(), -1, &fontFamily, FontStyleRegular, fontSize, point2, &format);

    // Draw outline
    Pen pen(Color(255, 255, 255, 255), 2);  // Black outline
    graphics->DrawPath(&pen, &path);

    // Fill path
    SolidBrush  solidBrush(Color(255, 255, 0, 0));  // Red text
    graphics->FillPath(&textBrush, &path);

    //graphics->DrawString(displayText->c_str(), -1, font, *point, &format, &textBrush);
}

void SubtitleWindow::SetDimensions(int width, int height)
{
    this->width = width;
    this->height = height;
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

        Graphics graphics(hdcBuffer);

        graphics.SetSmoothingMode(SmoothingModeAntiAlias);

        SolidBrush solidBrush(Color::Black);

        graphics.FillRectangle(&solidBrush, RectF(ps.rcPaint.left, ps.rcPaint.top, (ps.rcPaint.right - ps.rcPaint.left), (ps.rcPaint.bottom - ps.rcPaint.top)));

        FontFamily fontFamily(L"Consolas");

        Font font(&fontFamily, subtitleWindow->fontSize, FontStyleRegular, UnitPixel);

        StringFormat format;
        format.SetAlignment(StringAlignmentCenter);
        format.SetLineAlignment(StringAlignmentNear);

        RectF boundRect;
        
        PointF point((ps.rcPaint.right - ps.rcPaint.left) * 0.5, 0);
        
        graphics.MeasureString(wcDisplayText.c_str(), -1,
            &font, point, &format, &boundRect);

        LinearGradientBrush textBrush(boundRect, subtitleWindow->textColorPrimary, subtitleWindow->textColorSecondary, LinearGradientModeHorizontal);
        LinearGradientBrush backgroundBrush(boundRect, subtitleWindow->backgroundColorPrimary, subtitleWindow->backgroundColorSecondary, LinearGradientModeHorizontal);

        textBrush.SetGammaCorrection(TRUE);

        subtitleWindow->DrawSubtitleBackground(&graphics, &boundRect);

        subtitleWindow->DrawSubtitleText(&graphics, &wcDisplayText, &point, &font, &boundRect);

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
