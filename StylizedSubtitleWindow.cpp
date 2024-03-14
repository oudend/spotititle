#include "StylizedSubtitleWindow.h"

void StylizedSubtitleWindow::Update()
{
    angle = (angle + 1) % 360;
    RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE | RDW_ERASE);
    UpdateWindow(hwnd);
}

void StylizedSubtitleWindow::DrawSubtitleText(Graphics* graphics, std::wstring* displayText, PointF* point, Font* font, RectF* destRect)
{
    if (!useImage || backgroundImage == nullptr) {
        SubtitleWindow::DrawSubtitleText(graphics, displayText, point, font, destRect);
        return;
    }

    StringFormat format;
    format.SetAlignment(StringAlignmentCenter);
    format.SetLineAlignment(StringAlignmentNear);

    LinearGradientBrush textBrush(*destRect, textColorPrimary, textColorSecondary, LinearGradientModeHorizontal);

    textBrush.SetGammaCorrection(TRUE);

    GraphicsPath path;

    Point point2(point->X, point->Y);

    FontFamily fontFamily(L"Consolas");

    path.AddString(displayText->c_str(), -1, &fontFamily, FontStyleRegular, fontSize, point2, &format);

    SolidBrush  solidBrush(Color(150, 255, 255, 255));  // white text with 150/255 opacity
    graphics->FillPath(&solidBrush, &path);
}

void StylizedSubtitleWindow::DrawSubtitleBackground(Graphics* graphics, RectF* destRect)
{
    if (!useImage || backgroundImage == nullptr) {
        SubtitleWindow::DrawSubtitleBackground(graphics, destRect);
        return;
    }

    UINT ImageWidth = backgroundImage->GetWidth();
    UINT ImageHeight = backgroundImage->GetHeight();
    PointF center(ImageWidth / 2.0f, ImageHeight / 2.0f);

    int scaleY = (destRect->Width / destRect->Height);
    int scaleX = (destRect->Height / destRect->Width);

    RectF sourceRect;

    if (destRect->Width > destRect->Height)
        sourceRect = RectF(0, (ImageHeight / scaleY) / 2, ImageWidth, ImageHeight / scaleY);
    else
        sourceRect = RectF(0, 0, ImageWidth, ImageHeight);

    Bitmap* rotatedBitmap = new Bitmap(ImageWidth, ImageHeight);
    Graphics* rotatedGraphics = Graphics::FromImage(rotatedBitmap);
    rotatedGraphics->TranslateTransform(center.X, center.Y);
    rotatedGraphics->RotateTransform(angle);
    rotatedGraphics->ScaleTransform(2, 2);
    rotatedGraphics->TranslateTransform(-center.X, -center.Y);
    rotatedGraphics->DrawImage(backgroundImage, 0, 0, ImageWidth, ImageHeight);

    Bitmap* rotatedBitmap2 = new Bitmap(ImageWidth, ImageHeight);
    Graphics* rotatedGraphics2 = Graphics::FromImage(rotatedBitmap);
    rotatedGraphics2->TranslateTransform(center.X, center.Y);
    rotatedGraphics2->RotateTransform(360 - ((angle + 40) % 360));
    rotatedGraphics2->ScaleTransform(2, 2);
    rotatedGraphics2->TranslateTransform(-center.X, -center.Y);
    rotatedGraphics2->DrawImage(backgroundImage, 0, 0, ImageWidth, ImageHeight);


    ImageAttributes imageAttDarken;
    ColorMatrix colorMatrixDarken =
    {
        1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.2f, -0.2f, -0.2f, 0.0f, 1.0f
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
        -0.2f, -0.2f, -0.2f, 0.0f, 1.0f
    };
    imageAtt.SetColorMatrix(&colorMatrix, ColorMatrixFlagsDefault, ColorAdjustTypeBitmap);

    graphics->DrawImage(rotatedBitmap, *destRect, sourceRect, Unit::UnitPixel, &imageAttDarken);
    graphics->DrawImage(rotatedBitmap2, *destRect, sourceRect, Unit::UnitPixel, &imageAtt);

    SolidBrush  solidBrush(Color(1, 255, 255, 255));
    graphics->FillRectangle(&solidBrush, *destRect);

    delete rotatedGraphics2;
    delete rotatedBitmap2;

    delete rotatedGraphics;
    delete rotatedBitmap;
}

void StylizedSubtitleWindow::SetImage(const char* url) {
    backgroundImage = LoadImageFromUrl(url);

    Blur blurEffect;

    BlurParams blurParams;
    blurParams.radius = 40;

    blurEffect.SetParameters(&blurParams);

    UINT ImageWidth = backgroundImage->GetWidth();
    UINT ImageHeight = backgroundImage->GetHeight();

    RECT rect;
    rect.left = 0;
    rect.top = 0;
    rect.right = width;
    rect.bottom = height;

    backgroundImage->ApplyEffect(&blurEffect, &rect);
}