#pragma once
#include "Include.h"
const MARGINS margins = { -1 ,-1, -1, -1 };
const char g_szClassName[] = "overlay";
bool isVisible = true;

int screenWidth = GetSystemMetrics(SM_CXSCREEN);
int screenHeight = GetSystemMetrics(SM_CYSCREEN);