#include "StylizedSubtitleWindow.h"

void StylizedSubtitleWindow::Update()
{
    angle = (angle + 1) % 360;
    RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE | RDW_ERASE);
    UpdateWindow(hwnd);
}

Bitmap* StylizedSubtitleWindow::LoadImageFromUrl(const char* url, int width, int height) {
    CURL* curl;
    CURLcode res;
    std::vector<BYTE> imageData;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallbackLoadImageFromUrl);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &imageData);
        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }

        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();

    // Create a GDI+ Image object from the downloaded data
    IStream* pStream = NULL;
    HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, imageData.size());
    if (hGlobal) {
        void* pBuf = GlobalLock(hGlobal);
        if (pBuf) {
            CopyMemory(pBuf, &imageData[0], imageData.size());
            GlobalUnlock(hGlobal);
            if (SUCCEEDED(CreateStreamOnHGlobal(hGlobal, TRUE, &pStream))) {
                //Gdiplus::Image* image = Gdiplus::Image::FromStream(pStream);
                Gdiplus::Bitmap* bitmap = ResizeBitmap(Gdiplus::Bitmap::FromStream(pStream), width, height);
                pStream->Release();
                return bitmap;
            }
        }
        GlobalFree(hGlobal);
    }

    return NULL;
}

Bitmap* StylizedSubtitleWindow::ResizeBitmap(Bitmap* bmp, int newWidth, int newHeight)
{
    Bitmap* newBmp = new Bitmap(newWidth, newHeight, bmp->GetPixelFormat());
    Graphics graphics(newBmp);
    graphics.DrawImage(bmp, 0, 0, newWidth, newHeight);
    return newBmp;
}

void StylizedSubtitleWindow::DrawSubtitleText(Graphics* graphics, std::wstring* displayText, PointF* point, Font* font, RectF* destRect)
{
    if (backgroundBitmap == nullptr) {
        SubtitleWindow::DrawSubtitleText(graphics, displayText, point, font, destRect);
        return;
    }

    StringFormat format;
    format.SetAlignment(StringAlignmentCenter);
    format.SetLineAlignment(StringAlignmentNear);

    GraphicsPath path;

    Point point2(point->X, point->Y);

    FontFamily fontFamily(L"Consolas");

    path.AddString(displayText->c_str(), -1, &fontFamily, FontStyleRegular, fontSize, point2, &format);

    SolidBrush  solidBrush(Color(150, 255, 255, 255));  // white text with 150/255 opacity
    graphics->FillPath(&solidBrush, &path);
}

void StylizedSubtitleWindow::DrawSubtitleBackground(Graphics* graphics, RectF* destRect)
{

    if (backgroundBitmap == nullptr) {
        SubtitleWindow::DrawSubtitleBackground(graphics, destRect);
        return;
    }

    //draws the same blurred image on top of another one with 50% opacity and rotates them in opposite directions based on "angle" which gets modified in Update().

    UINT ImageWidth = backgroundBitmap->GetWidth();
    UINT ImageHeight = backgroundBitmap->GetHeight();
    PointF center(ImageWidth / 2.0f, ImageHeight / 2.0f);

    int scaleY = (destRect->Width / destRect->Height);
    int scaleX = (destRect->Height / destRect->Width);

    RectF sourceRect;

    if (destRect->Width > destRect->Height)
        sourceRect = RectF(0, (ImageHeight / scaleY) / 2.0f, ImageWidth - 1, ImageHeight / scaleY);
    else
        sourceRect = RectF(0, 0, ImageWidth, ImageHeight);

    Bitmap* rotatedBitmap = new Bitmap(ImageWidth, ImageHeight);
    Graphics* rotatedGraphics = Graphics::FromImage(rotatedBitmap);
    rotatedGraphics->TranslateTransform(center.X, center.Y);
    rotatedGraphics->RotateTransform(angle);
    rotatedGraphics->ScaleTransform(2, 2);
    rotatedGraphics->TranslateTransform(-center.X, -center.Y);
    rotatedGraphics->DrawImage(backgroundBitmap, 0, 0, ImageWidth, ImageHeight);

    Bitmap* rotatedBitmap2 = new Bitmap(ImageWidth, ImageHeight);
    Graphics* rotatedGraphics2 = Graphics::FromImage(rotatedBitmap2);
    rotatedGraphics2->TranslateTransform(center.X, center.Y);
    rotatedGraphics->RotateTransform(angle);
    rotatedGraphics2->RotateTransform(360 - ((angle + 40) % 360));
    rotatedGraphics2->ScaleTransform(2, 2);
    rotatedGraphics2->TranslateTransform(-center.X, -center.Y);
    rotatedGraphics2->DrawImage(backgroundBitmap, 0, 0, ImageWidth, ImageHeight);

    float brightness = 0.1f;

    ImageAttributes imageAttDarken;
    ColorMatrix colorMatrixDarken =
    {
        1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.5f, 0.0f,
        brightness, brightness, brightness, 0.0f, 1.0f
    };
    imageAttDarken.SetColorMatrix(&colorMatrixDarken, ColorMatrixFlagsDefault, ColorAdjustTypeBitmap);


    ImageAttributes imageAtt;
    const float alphaPercent = 0.5f;  // 50% transparency
    ColorMatrix colorMatrix =
    {
        1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, alphaPercent, 0.0f,
        brightness, brightness, brightness, 0.0f, 1.0f
    };
    imageAtt.SetColorMatrix(&colorMatrix, ColorMatrixFlagsDefault, ColorAdjustTypeBitmap);

    graphics->DrawImage(rotatedBitmap, *destRect, sourceRect, Unit::UnitPixel, &imageAttDarken);
    graphics->DrawImage(rotatedBitmap2, *destRect, sourceRect, Unit::UnitPixel, &imageAtt);

    delete rotatedGraphics2;
    delete rotatedBitmap2;

    delete rotatedGraphics;
    delete rotatedBitmap;
}

Bitmap* StylizedSubtitleWindow::BlurBitmap(Bitmap* bitmap, int radius)
{
    //loadedImage = bitmap;

    Blur blurEffect;

    BlurParams blurParams;
    blurParams.radius = radius;

    blurEffect.SetParameters(&blurParams);

    UINT ImageWidth = bitmap->GetWidth();
    UINT ImageHeight = bitmap->GetHeight();

    RECT rect;
    rect.left = 0;
    rect.top = 0;
    rect.right = width;
    rect.bottom = height;

    bitmap->ApplyEffect(&blurEffect, &rect);

    return bitmap;
}

void StylizedSubtitleWindow::SetImage(const char* url) {
    loadedImage = BlurBitmap(LoadImageFromUrl(url));

    if (currentTheme == Theme::Image)
        backgroundBitmap = loadedImage;
}

void StylizedSubtitleWindow::SetTheme(Theme theme)
{
    currentTheme = theme;

    if (theme == Theme::Image && loadedImage != nullptr) {
        backgroundBitmap = loadedImage;
        return;
    }

    backgroundBitmap = new Bitmap(500, 500);
    Graphics* themeGraphics = Graphics::FromImage(backgroundBitmap);
    themeGraphics->TranslateTransform(250, 250);
    themeGraphics->RotateTransform(angle);
    themeGraphics->ScaleTransform(2, 2);
    themeGraphics->TranslateTransform(-250, -250);

    // Create a solid brush in purple color

    SolidBrush* themeBrush = new SolidBrush(themeColors[theme]);
    
    // Get the color of the purple brush
    Color themeColor;
    themeBrush->GetColor(&themeColor);

    // Calculate the darker color
    int darkRed = max(themeColor.GetRed() - 150, 0);
    int darkGreen = max(themeColor.GetGreen() - 150, 0);
    int darkBlue = max(themeColor.GetBlue() - 150, 0);

    // Create a solid brush in dark purple color
    SolidBrush* darkPurpleBrush = new SolidBrush(Color(themeColor.GetAlpha(), darkRed, darkGreen, darkBlue));

    themeGraphics->FillRectangle(darkPurpleBrush, Rect(0, 0, 500, 500));

    // Define the radius of the balls
    int radius = 10;

    // Draw the balls at different positions
    for (int i = 0; i < 500; i += 50) {
        for (int j = 0; j < 500; j += 50) {
            themeGraphics->FillEllipse(themeBrush, i, j, 2 * radius, 2 * radius);
        }
    }

    BlurBitmap(backgroundBitmap);

    delete themeGraphics;
}
