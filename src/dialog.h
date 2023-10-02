#pragma once

#include "framework.h"
#include "weather-panel.h"

#define STATEIMAGEMASKTOINDEX(i) ((i) >> 12)

INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK Settings(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK Edit(HWND, UINT, WPARAM, LPARAM);
