#include <fstream>
#include <sstream>
#include <vector>
#include "framework.h"
#include "weather-panel.h"
#include "weather-api.h"
#include "widget.h"

#define MAX_LOADSTRING 100

using std::string;
using std::wstring;
using std::vector;

// Global Variables:
HINSTANCE  hInst;                          // current instance
WCHAR      szTitle[MAX_LOADSTRING];        // The title bar text
WCHAR      szWindowClass[MAX_LOADSTRING];  // the main window class name

// Forward declarations of functions included in this code module:
ATOM              RegisterMain(HINSTANCE hInstance);
BOOL              InitInstance(HINSTANCE, int);
LRESULT CALLBACK  WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK  About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_WEATHERPANEL, szWindowClass, MAX_LOADSTRING);
	RegisterMain(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
		return FALSE;

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WEATHERPANEL));

	MSG msg;

	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0))
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

	return (int)msg.wParam;
}

ATOM RegisterMain(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex{};

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WEATHERPANEL));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_WEATHERPANEL);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Store instance handle in our global variable

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
		return FALSE;

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// Configurations and settings for the app.
	static std::fstream  configFile("weather.conf", std::ios::out | std::ios::in | std::ios::app);

	static Dashboard  dashboard;  // Current dashboard to work with.
	static RECT       client;     // Window client area.
	static POINT      cursor;     // Current cursor position.
	static HPEN       pen;        // Main pen.
	static HBRUSH     brush;      // Main brush.
	static int        box = 0;    // Box size for grid.

	static Widget* drag = nullptr; // Dragging widgets.
	static int resize = WD_NORESIZE; // Resizing widgets.

	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;

		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;

		case ID_EDIT_SYNC:
			// Update data.
			dashboard.update();

			// Repaint window.
			InvalidateRect(hWnd, &client, false);

			MessageBox(NULL, L"Dashboard syncronized.", L"Sync", MB_ICONINFORMATION | MB_OK);
			break;

		case ID_EDIT_RELOAD:
			// SendMessage(hWnd, WM_CREATE, NULL, NULL);
			MessageBox(NULL, L"Dashboard reloaded.", L"Load", MB_ICONINFORMATION | MB_OK);
			break;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;

	case WM_MOUSEMOVE:
	{
		GetCursorPos(&cursor);
		ScreenToClient(hWnd, &cursor);
		InvalidateRect(hWnd, &client, false);
	}
	break;

	case WM_SIZE:
		GetClientRect(hWnd, &client);
		break;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		HDC mdc = CreateCompatibleDC(hdc);
		HBITMAP bmp = CreateCompatibleBitmap(hdc, client.right - client.left, client.bottom - client.top);
		SelectObject(mdc, bmp);

		SetBkMode(mdc, TRANSPARENT);

		SelectObject(mdc, pen);
		SelectObject(mdc, brush);

		// Background
		Rectangle(mdc, client.left, client.top, client.right, client.bottom);

		// Grid lines
		for (int i = 0; i < client.right - client.left; i += box)
		{
			MoveToEx(mdc, i, client.top, NULL);
			LineTo(mdc, i, client.bottom - client.top);
		}

		// Grid lines
		for (int i = 0; i < client.bottom - client.top; i += box)
		{
			MoveToEx(mdc, client.left, i, NULL);
			LineTo(mdc, client.right - client.left, i);
		}

		// Draw widgets
		for (Widget* widget : dashboard.widgets)
		{
			UINT width = (widget->rect.right - widget->rect.left);
			UINT height = (widget->rect.bottom - widget->rect.top);
			UINT pwidth = width * box;
			UINT pheight = height * box;
			RECT scaledRect{ widget->rect.left * box, widget->rect.top * box, widget->rect.right * box, widget->rect.bottom * box };

			SelectObject(mdc, pen);

			Rectangle(mdc, scaledRect.left, scaledRect.top, scaledRect.right, scaledRect.bottom);

			// Mouse events
			if (cursor.x > scaledRect.left && cursor.x < scaledRect.right && cursor.y > scaledRect.top && cursor.y < scaledRect.top + box)
			{
				// Move
				if ((GetKeyState(VK_LBUTTON) & 0x8000) != 0 && drag == nullptr)
					drag = widget;
			}
			else if (cursor.x > scaledRect.left && cursor.x < scaledRect.right && cursor.y > scaledRect.bottom - box && cursor.y < scaledRect.bottom)
			{
				// Resize down
				if ((GetKeyState(VK_LBUTTON) & 0x8000) != 0 && drag == nullptr)
				{
					drag = widget;
					resize = WD_SOUTHRESIZE;
				}
			}

			// Resize left
			if (cursor.x > scaledRect.left && cursor.x < scaledRect.left + box && cursor.y > scaledRect.top && cursor.y < scaledRect.bottom)
				if ((GetKeyState(VK_LBUTTON) & 0x8000) != 0 && drag == nullptr)
				{
					drag = widget;
					resize |= WD_WESTRESIZE;
				}

			// Resize left
			if (cursor.x > scaledRect.right - box && cursor.x < scaledRect.right + box && cursor.y > scaledRect.top && cursor.y < scaledRect.bottom)
				if ((GetKeyState(VK_LBUTTON) & 0x8000) != 0 && drag == nullptr)
				{
					drag = widget;
					resize |= WD_EASTRESIZE;
				}

			SetTextAlign(mdc, TA_CENTER);

			// Draw text values
			for (int i = 0; i < widget->fields.size(); i++)
			{
				wstring title = wstring(widget->fields[i].begin(), widget->fields[i].end());
				wstring value;

				// Find value and assign it to `value`.
				if (widget->data[i][0]->s != nullptr)
				{
					string data = *widget->data[i][0]->s;
					value = wstring(data.begin(), data.end());
				}
				else if (widget->data[i][0]->d != nullptr)
				{
					value = std::to_wstring(*widget->data[i][0]->d);
					value.erase(value.find_last_not_of('0') + 1, string::npos);
					value.erase(value.find_last_not_of('.') + 1, string::npos);
				}
				else if (widget->data[i][0]->i != nullptr)
					value = std::to_wstring(*widget->data[i][0]->i);
				else if (widget->data[i][0]->v != nullptr)
				{
					vector<double> data(*widget->data[i][0]->v);
					for (UINT i = 0; i < height - 1 && i < data.size(); i++)
					{
						wstring value = std::to_wstring(data[i]);
						value.erase(value.find_last_not_of('0') + 1, string::npos);
						value.erase(value.find_last_not_of('.') + 1, string::npos);

						TextOut(mdc, widget->rect.left * box + pwidth / 2, widget->rect.top * box + box * i + box, value.c_str(), lstrlen(value.c_str()));
					}
				}

				HPEN heading = CreatePen(PS_DASH, 1, BLACK_PEN);

				SelectObject(mdc, heading);

				// Draw title
				TextOut(mdc, scaledRect.left + pwidth / 2, scaledRect.top, title.c_str(), lstrlen(title.c_str()));

				// Underline
				MoveToEx(mdc, scaledRect.left, (scaledRect.top) + box, NULL);
				LineTo(mdc, scaledRect.right, (scaledRect.top) + box);

				// Draw data
				TextOut(mdc, widget->rect.left * box + pwidth / 2, widget->rect.top * box + (pheight + box / 2) / 2, value.c_str(), lstrlen(value.c_str()));

				DeleteObject(heading);
			}
		}

		// Draggage
		if (drag != nullptr)
		{
			RECT position{ drag->rect.left * box, drag->rect.top * box, drag->rect.right * box, drag->rect.bottom * box };

			if (resize)
			{
				if (resize & WD_SOUTHRESIZE)
					position.bottom = cursor.y - cursor.y % box + box;

				if (resize & WD_EASTRESIZE)
					position.right = cursor.x - cursor.x % box + box;

				if (resize & WD_WESTRESIZE)
					position.left = static_cast<LONG>(cursor.x - cursor.x % box);
			}
			else
			{
				UINT   width = (drag->rect.right - drag->rect.left) * box;
				UINT   height = (drag->rect.bottom - drag->rect.top) * box;
				POINT  offset{ static_cast<LONG>(((cursor.x - width / 2) % box) - box), static_cast<LONG>((cursor.y - box / 2) % box) };

				position.left = (cursor.x - width / 2) - offset.x;
				position.top = (cursor.y - box / 2) - offset.y;
				position.right = (cursor.x + width / 2) - offset.x;
				position.bottom = (cursor.y + height - box / 2) - offset.y;
			}

			if ((GetKeyState(VK_LBUTTON) & 0x8000) != 0) // Drag
				InvertRect(mdc, &position);
			else // Drop
			{
				// Scale
				position.left /= box;
				position.top /= box;
				position.right /= box;
				position.bottom /= box;

				// Reset resize flag
				resize = WD_NORESIZE;

				// Move widget to new place
				dashboard.replace(drag->id, position);

				// Reset the drag flag
				drag = nullptr;
			}
		}

		BitBlt(hdc, 0, 0, client.right - client.left, client.bottom - client.top, mdc, 0, 0, SRCCOPY);
		DeleteObject(mdc);
		DeleteObject(bmp);
		EndPaint(hWnd, &ps);
	}
	break;

	case WM_CREATE:
	{
		GetClientRect(hWnd, &client);
		GetCursorPos(&cursor);
		ScreenToClient(hWnd, &cursor);

		// Init config
		if (configFile.is_open())
		{
			string line; // Current line.

			configFile.seekg(0);

			if (std::getline(configFile, line) && line == "0")
				while (std::getline(configFile, line))
				{
					std::istringstream iss(line);
					string command;
					iss >> command;
					if (command == "widget")
					{
						RECT position{};
						iss >> position.left >> position.top >> position.right >> position.bottom;

						Widget* widget = new Widget((int)dashboard.widgets.size(), position);

						string field;
						while (iss >> field)
							widget->fields.push_back(field);

						dashboard.widgets.push_back(widget);
					}
					else if (command == "box")
						iss >> box;
					else if (command == "timezone")
						iss >> dashboard.weather.timezone;
					else if (command == "latitude")
						iss >> dashboard.weather.latitude;
					else if (command == "longitude")
						iss >> dashboard.weather.longitude;
					else;
				}
			else
			{
				MessageBox(NULL, L"Problem reading configurations.\nPress OK to reset app configuration.", L"Configuration Error", MB_ICONEXCLAMATION | MB_OK);

				configFile.flush();
				configFile.seekg(0);
				configFile.write("0", sizeof("0"));
				configFile.flush();
				configFile.seekg(0);
			}
		}
		else
			MessageBox(NULL, L"Faild to read configuration file.", L"Configuration Error", MB_ICONERROR | MB_OK);

		// Initialize dashboard data.
		dashboard.weather.current_weather = true;
		dashboard.update();

		// Default box size.
		box = 20;

		// Initialize GDI tools.
		pen = CreatePen(PS_SOLID, 2, RGB(191, 191, 191));
		brush = CreateSolidBrush(RGB(255, 255, 255));
	}
	break;

	case WM_DESTROY:
		DeleteObject(pen);
		DeleteObject(brush);

		// Close configuration file handle.
		configFile.close();

		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
