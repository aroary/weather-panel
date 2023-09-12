#include <fstream>
#include <sstream>
#include "framework.h"
#include "weather-panel.h"
#include "weather-api.h"
#include "widget.h"

#define MAX_LOADSTRING 100

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

	static Dashboard     dashboard;   // Current dashboard to work with.
	static RECT          client;      // Window client area.
	static LPPOINT       cursor;      // Current cursor position.
	static HPEN          pen;         // Main pen.
	static HBRUSH        brush;       // Main brush.

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
			InvalidateRect(hWnd, &client, true);

			MessageBox(NULL, L"Dashboard updated.", L"Sync", MB_ICONINFORMATION | MB_OK);
			break;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;

	case WM_MOUSEMOVE:
		GetCursorPos(cursor);
		break;

	case WM_SIZE:
		GetClientRect(hWnd, &client);
		break;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		SetBkMode(hdc, TRANSPARENT);

		SelectObject(hdc, pen);

		int box = 20;

		// Grid lines
		for (int i = 0; i < client.right - client.left; i += box)
		{
			MoveToEx(hdc, i, client.top, NULL);
			LineTo(hdc, i, client.bottom - client.top);
		}

		// Grid lines
		for (int i = 0; i < client.bottom - client.top; i += box)
		{
			MoveToEx(hdc, client.left, i, NULL);
			LineTo(hdc, client.right - client.left, i);
		}

		// Draw widgets
		for (Widget* widget : dashboard.widgets)
		{
			UINT width = (widget->rect.right - widget->rect.left);
			UINT height = (widget->rect.bottom - widget->rect.top);
			UINT pwidth = width * box;
			UINT pheight = height * box;

			widget->rect;
			Rectangle(hdc, widget->rect.left * box, widget->rect.top * box, widget->rect.right * box, widget->rect.bottom * box);

			// Draw text values
			for (int i = 0; i < widget->fields.size(); i++)
			{
				std::wstring title = std::wstring(widget->fields[i].begin(), widget->fields[i].end());
				std::wstring value;

				// Find value and assign it to `value`.
				if (widget->data[i][0]->s != nullptr)
				{
					std::string data = *widget->data[i][0]->s;
					value = std::wstring(data.begin(), data.end());
				}
				else if (widget->data[i][0]->d != nullptr)
				{
					value = std::to_wstring(*widget->data[i][0]->d);
					value.erase(value.find_last_not_of('0') + 1, std::string::npos);
					value.erase(value.find_last_not_of('.') + 1, std::string::npos);
				}
				else if (widget->data[i][0]->i != nullptr)
					value = std::to_wstring(*widget->data[i][0]->i);
				else;

				SetTextAlign(hdc, TA_CENTER);
				TextOut(hdc, widget->rect.left * box + pwidth / 2, widget->rect.top * box, title.c_str(), lstrlen(title.c_str()));

				SetTextAlign(hdc, TA_CENTER | TA_BASELINE);
				TextOut(hdc, widget->rect.left * box + pwidth / 2, widget->rect.top * box + pheight / 2, value.c_str(), lstrlen(value.c_str()));
			}
		}

		EndPaint(hWnd, &ps);
	}
	break;

	case WM_CREATE:
	{
		GetClientRect(hWnd, &client);

		// Init config file
		std::string line; // Current line.

		configFile.seekg(0);

		if (configFile.is_open())
		{
			INIT:
			if (std::getline(configFile, line) && line == "0")
				while (std::getline(configFile, line))
				{
					std::istringstream iss(line);
					std::string command;
					iss >> command;
					if (command == "widget")
					{
						RECT position{};
						iss >> position.left >> position.top >> position.right >> position.bottom;

						Widget* widget = new Widget((int)dashboard.widgets.size(), position);

						std::string field;
						while (iss >> field)
							widget->fields.push_back(field);

						dashboard.widgets.push_back(widget);
					}
				}
			else {
				MessageBox(NULL, L"Problem reading configurations.\nPress OK to reset app configuration.", L"Configuration Error", MB_ICONEXCLAMATION | MB_OK);

				configFile.write("0", sizeof("0"));

				configFile.seekg(0);

				goto INIT;
			}

			configFile.close();
		}
		else
			MessageBox(NULL, L"Faild to read configuration file.", L"Configuration Error", MB_ICONERROR | MB_OK);

		dashboard.weather.current_weather = true;
		dashboard.weather.timezone = "GMT";
		dashboard.weather.latitude = 45.67;
		dashboard.weather.longitude = -76.54;
		dashboard.update();

		// Init GDI tools
		pen = CreatePen(PS_SOLID, 2, RGB(200, 200, 200));
		brush = CreateSolidBrush(RGB(10, 100, 200));
	}
	break;

	case WM_DESTROY:
		DeleteObject(pen);

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
