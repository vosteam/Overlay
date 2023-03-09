#include <Windows.h>
#include <chrono>
#include <Dwmapi.h> 
#pragma comment(lib, "dwmapi.lib")

const MARGINS margins = { -1 ,-1, -1, -1 };
const wchar_t g_szClassName[] = L"overlay";
bool isVisible = true;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_HOTKEY:
        if (wParam == 1) // При нажатии на горячую клавишу ALT + Z
        {
            isVisible = !isVisible; // Инвертируем значение переменной isVisible
            ShowWindow(hwnd, isVisible ? SW_SHOW : SW_HIDE); // Показываем/скрываем окно в зависимости от значения isVisible
        }
        else if (wParam == 2) // При нажатии на горячую клавишу ALT + X
        {
            // Закрытие окна
            DestroyWindow(hwnd);
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
        TextOut(hdc, x, y, L"By vos9", 7);
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

    // Регистрация класса окна
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc; // Указываем обработчик сообщений
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

    // Создание окна
    hwnd = CreateWindowEx(
        WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW | WS_EX_TOPMOST, // <--- добавляем флаг WS_EX_TOOLWINDOW
        g_szClassName,
        g_szClassName,
        WS_POPUP | WS_VISIBLE,
        0, 0, 1920, 1080,
        NULL, NULL, hInstance, NULL);

    SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 255, LWA_ALPHA);
    DwmExtendFrameIntoClientArea(hwnd, &margins);

    // Регистрация горячей клавиши
    RegisterHotKey(hwnd, 1, MOD_ALT, 0x5A); // Здесь 0x5A соответствует клавише Z
    RegisterHotKey(hwnd, 2, MOD_ALT, 0x58); // Здесь 0x58 соответствует клавише X

    // Цикл обработки сообщений
    while (GetMessage(&Msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }

    // Удаление горячей клавиши
    UnregisterHotKey(hwnd, 1);
    UnregisterHotKey(hwnd, 2);

    exit(0);
    return Msg.wParam;
}