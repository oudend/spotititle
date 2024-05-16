#include "SubtitleWindow.h"

void SubtitleWindow::DrawSubtitleBackground(Graphics* graphics, RectF* destRect)
{
    SolidBrush backgroundBrush(backgroundColor);
    graphics->FillRectangle(&backgroundBrush, *destRect);
}

void SubtitleWindow::DrawSubtitleText(Graphics* graphics, std::wstring* displayText, PointF* point, Font* font, RectF* destRect)
{
    StringFormat format;
    format.SetAlignment(StringAlignmentCenter);
    format.SetLineAlignment(StringAlignmentNear);

    SolidBrush textBrush(textColor);

    GraphicsPath path; // GraphicsPath object for text outline.

    Point point2(point->X, point->Y);

    FontFamily fontFamily(L"Consolas");

    path.AddString(displayText->c_str(), -1, &fontFamily, FontStyleRegular, fontSize, point2, &format);

    // Draw outline
    Pen pen(Color(100, 255, 255, 255), 2);  // Black outline
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

    SetWindowPos(this->hwnd, 0, 0, 0, width, height, SWP_FRAMECHANGED | WS_VISIBLE);
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

void SubtitleWindow::UpdateFPS()
{
    SetTimer(hwnd, 1, (1000.0f / (float)fps), NULL); //starts a timer with the id "1" which updates at an interval based on the fps variable.
}

void SubtitleWindow::Update()
{
    RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE | RDW_ERASE);
    UpdateWindow(hwnd);
}

LRESULT SubtitleWindow::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    SubtitleWindow* subtitleWindow = (SubtitleWindow*)GetWindowLongPtr(hwnd, GWLP_USERDATA); //gets the object instance through a pointer stored in the window object, this is necessary to be able to access the classes variables and functions.

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
    case WM_CTLCOLORSTATIC:
    {

    }
    break;
    case WM_NCHITTEST:
    {
        //make the entire window draggable
        return HTCAPTION;
    }
    break;
    case WM_PAINT:
    {
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
        std::wstring wcDisplayText = converter.from_bytes(subtitleWindow->displayText); //converts the displayText string to the wstring format which is necessary for gdi.

        PAINTSTRUCT ps;

        HDC hdc = BeginPaint(hwnd, &ps);

        RECT rect;
        GetClientRect(hwnd, &rect); //get the screen size.

        HDC hdcBuffer = CreateCompatibleDC(hdc);
        HBITMAP hbmBuffer = CreateCompatibleBitmap(hdc, rect.right, rect.bottom);
        HBITMAP hbmOldBuffer = (HBITMAP)SelectObject(hdcBuffer, hbmBuffer);

        Graphics graphics(hdcBuffer);

        graphics.SetSmoothingMode(SmoothingModeAntiAlias);

        FontFamily fontFamily(subtitleWindow->fontName);

        Font font(&fontFamily, subtitleWindow->fontSize, FontStyleRegular, UnitPixel);

        StringFormat format;
        format.SetAlignment(StringAlignmentCenter);
        format.SetLineAlignment(StringAlignmentNear);

        RectF boundRect;
        
        PointF point((ps.rcPaint.right - ps.rcPaint.left) * 0.5, 0);
        
        graphics.MeasureString(wcDisplayText.c_str(), -1,
            &font, point, &format, &boundRect); //saves the dimensions of the displayText if it were drawn based on input variables into the boundRect variable.

        subtitleWindow->DrawSubtitleBackground(&graphics, &boundRect); //draws the background for the displayText using the boundRect variable

        subtitleWindow->DrawSubtitleText(&graphics, &wcDisplayText, &point, &font, &boundRect); //draws the displayText

        BitBlt(hdc, 0, 0, rect.right, rect.bottom, hdcBuffer, 0, 0, SRCCOPY); //copy over the drawn subtitle into the window, by not working directly on the window buffer we also prevent visible artifacts while the output is being drawn.

        RectF windowRect(ps.rcPaint.left, ps.rcPaint.top, (ps.rcPaint.right - ps.rcPaint.left), (ps.rcPaint.bottom - ps.rcPaint.top));

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
        subtitleWindow->Update(); //calls the update function based on the fps of the SubtitleWindow at an even interval. 
        return 0;
    }
    break;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
