#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <wchar.h>
#include "framework.h"
#include "weather-panel.h"
#include "weather-api.h"
#include "widget.h"
#include <commctrl.h>

INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK Settings(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK Edit(HWND, UINT, WPARAM, LPARAM);
