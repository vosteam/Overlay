#include <Windows.h>
#include <chrono>
#include <Dwmapi.h>
#include <iostream>
#include <fstream>
#pragma comment(lib, "dwmapi.lib")
#pragma warning(disable : 4996)

const MARGINS margins = { -1 ,-1, -1, -1 };
const char g_szClassName[] = "overlay";
bool isVisible = true;

void addWatermark(HDC hdc, HDC hdcDIB, BITMAPINFO bmi)
{
    HFONT hFont;
    LOGFONT lf = { 0 };
    lf.lfHeight = -MulDiv(20, GetDeviceCaps(hdc, LOGPIXELSY), 72); // размер шрифта
    lf.lfWeight = FW_NORMAL; // жирность шрифта
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

void captureScreen(std::string file_Name)
{
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
    addWatermark(hdcScreen, hdcDIB, bmi);

    hFile = CreateFile(("C:\\Users\\" + (std::string)std::getenv("USERNAME") + "\\Documents\\vos.team\\" + file_Name).c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
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

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    std::string file_Name = "screenshot_" + std::to_string(time(NULL)) + ".bmp";
    switch (msg)
    {
    case WM_HOTKEY:
        if (wParam == 1)
        {
            isVisible = !isVisible;
            ShowWindow(hwnd, isVisible ? SW_SHOW : SW_HIDE);
        }
        else if (wParam == 2)
        {
            DestroyWindow(hwnd);
        }
        else if (wParam == 3)
        {
            captureScreen(file_Name);
        }
        break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        TEXTMETRIC tm;
        GetTextMetrics(hdc, &tm);
        int x = 10;
        int y = tm.tmHeight + 10;

        auto currentTime = std::chrono::system_clock::now();
        auto currentTime_t = std::chrono::system_clock::to_time_t(currentTime);

        char timeStr[100];
        strftime(timeStr, 100, "Time: %H:%M:%S", std::localtime(&currentTime_t));

        TextOut(hdc, x, y, timeStr, strlen(timeStr));
        EndPaint(hwnd, &ps);
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wc;
    HWND hwnd;
    MSG Msg;

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(RGB(0, 0, 0));
    wc.lpszMenuName = NULL;
    wc.lpszClassName = g_szClassName;
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    RegisterClassEx(&wc);

    hwnd = CreateWindowEx(
        WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW | WS_EX_TOPMOST,
        g_szClassName,
        g_szClassName,
        WS_POPUP | WS_VISIBLE,
        0, 0, 1920, 1080,
        NULL, NULL, hInstance, NULL);

    SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 255, LWA_ALPHA);
    DwmExtendFrameIntoClientArea(hwnd, &margins);

    RegisterHotKey(hwnd, 1, MOD_ALT, 0x5A);
    RegisterHotKey(hwnd, 2, MOD_ALT, 0x58);
    RegisterHotKey(hwnd, 3, MOD_ALT, 0x50);

    while (GetMessage(&Msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);

        if (Msg.message == WM_PAINT)
        {
            RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    UnregisterHotKey(hwnd, 1);
    UnregisterHotKey(hwnd, 2);
    UnregisterHotKey(hwnd, 3);

    exit(0);
    return Msg.wParam;
}