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
	static Dashboard dashboard;
	static RECT client;
	static LPPOINT cursor;
	static HPEN pen;
	static HBRUSH brush;

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

			for (int i = 0; i < widget->fields.size(); i++)
			{
				std::wstring title = std::wstring(widget->fields[i].begin(), widget->fields[i].end());
				std::wstring value;

				if (widget->fields[i] == "timezone")
				{
					std::string s = *widget->data[i][0]->s;
					value = *s.c_str();
				}
				else if (widget->fields[i] == "latitude" || widget->fields[i] == "longitude")
					value = std::to_wstring(*widget->data[i][0]->d);
				else if (widget->fields[i] == "elevation")
					value = std::to_wstring(*widget->data[i][0]->i);

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

		Widget* widget = new Widget((int)dashboard.widgets.size(), { 0, 0, 10, 10 });
		widget->fields.push_back("latitude");
		dashboard.widgets.push_back(widget);

		Widget* widget2 = new Widget((int)dashboard.widgets.size(), { 15, 15, 20, 20 });
		widget2->fields.push_back("longitude");
		dashboard.widgets.push_back(widget2);

		widget = new Widget((int)dashboard.widgets.size(), { 30, 16, 45, 21 });
		widget->fields.push_back("timezone");
		dashboard.widgets.push_back(widget);

		widget = new Widget((int)dashboard.widgets.size(), { 20, 1, 24, 6 });
		widget->fields.push_back("elevation");
		dashboard.widgets.push_back(widget);

		dashboard.update();

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
