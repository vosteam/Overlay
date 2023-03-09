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
        if (wParam == 1) // ��� ������� �� ������� �������
        {
            isVisible = !isVisible; // ����������� �������� ���������� isVisible
            ShowWindow(hwnd, isVisible ? SW_SHOW : SW_HIDE); // ����������/�������� ���� � ����������� �� �������� isVisible
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

    // ����������� ������ ����
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc; // ��������� ���������� ���������
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

    // �������� ����
    hwnd = CreateWindowEx(
        WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW | WS_EX_TOPMOST, // <--- ��������� ���� WS_EX_TOOLWINDOW
        g_szClassName,
        g_szClassName,
        WS_POPUP | WS_VISIBLE,
        0, 0, 1920, 1080,
        NULL, NULL, hInstance, NULL);

    SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 255, LWA_ALPHA);
    DwmExtendFrameIntoClientArea(hwnd, &margins);

    // ����������� ������� �������
    RegisterHotKey(hwnd, 1, MOD_ALT, 0x5A); // ����� 0x5A ������������� ������� Z

    // ���� ��������� ���������
    while (GetMessage(&Msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }

    // �������� ������� �������
    UnregisterHotKey(hwnd, 1);

    exit(0);
    return Msg.wParam;
}