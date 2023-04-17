#include "captureScreen.h"

void screenshot::captureScreen()
{
    std::string file_screenshot = "screenshot_" + std::to_string(time(NULL)) + ".bmp";

    HDC hdcScreen;
    HDC hdcDIB;
    HBITMAP hbmDIB;
    BITMAPINFO bmi;
    LPVOID lpvBits;
    HANDLE hFile;
    DWORD dwBytesWritten;


    hdcScreen = GetDC(NULL);
    hdcDIB = CreateCompatibleDC(hdcScreen);
    ZeroMemory(&bmi, sizeof(BITMAPINFO));
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = GetSystemMetrics(SM_CXSCREEN);
    bmi.bmiHeader.biHeight = GetSystemMetrics(SM_CYSCREEN);
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 24;
    bmi.bmiHeader.biCompression = BI_RGB;
    hbmDIB = CreateDIBSection(hdcScreen, &bmi, DIB_RGB_COLORS, &lpvBits, NULL, 0);
    SelectObject(hdcDIB, hbmDIB);
    BitBlt(hdcDIB, 0, 0, bmi.bmiHeader.biWidth, bmi.bmiHeader.biHeight, hdcScreen, 0, 0, SRCCOPY);

    // Добавление ватермарки на контекст
    water::addWatermark(hdcScreen, hdcDIB, bmi);

    hFile = CreateFile(("C:\\Users\\" + (std::string)std::getenv("USERNAME") + "\\Documents\\vos.team\\screenshot\\" + file_screenshot).c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    BITMAPFILEHEADER hdr = { };
    hdr.bfType = 0x4d42; // Буквы 'B' и 'M'
    hdr.bfOffBits = sizeof(hdr) + sizeof(BITMAPINFOHEADER);
    hdr.bfSize = hdr.bfOffBits + bmi.bmiHeader.biWidth * bmi.bmiHeader.biHeight * 3;
    WriteFile(hFile, &hdr, sizeof(hdr), &dwBytesWritten, NULL);
    WriteFile(hFile, &bmi.bmiHeader, sizeof(BITMAPINFOHEADER), &dwBytesWritten, NULL);
    WriteFile(hFile, lpvBits, bmi.bmiHeader.biWidth * bmi.bmiHeader.biHeight * 3, &dwBytesWritten, NULL);
    CloseHandle(hFile);
    DeleteObject(hbmDIB);
    DeleteDC(hdcDIB);
    ReleaseDC(NULL, hdcScreen);
}