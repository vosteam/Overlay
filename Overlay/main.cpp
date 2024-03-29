#include "Include/stdafx.h"

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    std::string folder_path = "C:\\Users\\" + (std::string)std::getenv("USERNAME") + "\\Documents\\vos.team\\";
    if (!std::filesystem::exists(folder_path)) {
        std::filesystem::create_directories(folder_path);
        std::filesystem::create_directories(folder_path + "video");
        std::filesystem::create_directories(folder_path + "screenshot");
    }

    std::string file_screenshot = "screenshot_" + std::to_string(time(NULL)) + ".bmp";

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
            screenshot::captureScreen();
        }
        else if (wParam == 4)
        {
            std::thread t( record::screenRecorder , file_screenshot);
            t.detach();
        }
        break;
    case WM_PAINT:
    {
        timeAd::timeAdd(hwnd);
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
        0, 0, screenWidth, screenHeight,
        NULL, NULL, hInstance, NULL);


    SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 255, LWA_ALPHA);
    DwmExtendFrameIntoClientArea(hwnd, &margins);

    RegisterHotKey(hwnd, 1, MOD_ALT, 0x5A);
    RegisterHotKey(hwnd, 2, MOD_ALT, 0x58);
    RegisterHotKey(hwnd, 3, MOD_ALT, 0x50);
    RegisterHotKey(hwnd, 4, MOD_ALT, 0x56);


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
    UnregisterHotKey(hwnd, 4);

    return Msg.wParam;
}