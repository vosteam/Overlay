#include "../Include/Include.h"

void timeAdd(HWND hwnd){
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