#pragma once
#include "../Include/Include.h"

void addWatermark(HDC hdc, HDC hdcDIB, BITMAPINFO bmi)
{
    HFONT hFont;
    LOGFONT lf = { 0 };
    lf.lfHeight = -MulDiv(20, GetDeviceCaps(hdc, LOGPIXELSY), 72); // ������ ������
    lf.lfWeight = FW_NORMAL; // �������� ������
    lf.lfCharSet = DEFAULT_CHARSET;
    lf.lfOutPrecision = OUT_TT_PRECIS;
    lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
    lf.lfQuality = DEFAULT_QUALITY;
    lf.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
    strcpy_s(lf.lfFaceName, "Arial");

    hFont = CreateFontIndirect(&lf);
    SelectObject(hdcDIB, hFont);

    std::string watermark = "vos.team";

    SetTextColor(hdcDIB, RGB(255, 0, 255, 128));
    SetBkMode(hdcDIB, TRANSPARENT);
    TextOut(hdcDIB, 10, 10, watermark.c_str(), watermark.length());
    DeleteObject(hFont);
}