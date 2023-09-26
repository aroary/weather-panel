#include "framework.h"
#include "weather-panel.h"
#include "dialog.h"
#include "weather-api.h"
#include "widget.h"
#include "settings.h"
#include <fstream>
#include <sstream>
#include <vector>

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
			// Clear old setup
			for (Widget* widget : dashboard.widgets)
				delete widget;

			dashboard.widgets.clear();

			// Request reload
			SendMessage(hWnd, WM_CREATE, NULL, NULL);
			break;

		case ID_EDIT_SAVE:
			dashboard.save();
			break;

		case ID_EDIT_SETTINGS:
			DialogBoxParam(hInst, MAKEINTRESOURCE(IDD_SETTINGS), hWnd, Settings, (LPARAM)&dashboard);
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

	case WM_RBUTTONUP:
	{
		POINT screen = cursor;
		ClientToScreen(hWnd, &screen);

		Widget* selection = nullptr;

		for (Widget* widget : dashboard.widgets)
			if (widget->rect.left * box <= cursor.x && widget->rect.top * box <= cursor.y && widget->rect.right * box >= cursor.x && widget->rect.bottom * box >= cursor.y)
			{
				selection = widget;
				break;
			}

		HMENU hMenu = LoadMenu(NULL, MAKEINTRESOURCE(IDR_MENU));
		UINT item = TrackPopupMenu(GetSubMenu(hMenu, selection == nullptr), TPM_TOPALIGN | TPM_LEFTALIGN | TPM_RETURNCMD, screen.x, screen.y, 0, hWnd, NULL);

		switch (item)
		{
		case ID_NEW:
		{
			selection = new Widget((int)dashboard.widgets.size(), { 0, 0, 2, 2 });

		EDIT:
			INT_PTR result = DialogBoxParam(hInst, MAKEINTRESOURCE(IDD_EDIT), hWnd, Edit, (LPARAM)selection);

			if (result == IDCANCEL)
				delete selection;
			else
			{
				if (dashboard.replace(selection, selection->rect))
					dashboard.widgets.push_back(selection);
				else
					goto EDIT;
			}

			break;
		}

		case ID_EDIT:
		{
			if (selection != nullptr) // To surpress C6011
			{
				Widget* widget = new Widget(selection->id, selection->rect); // Dummy instance
				widget->title = selection->title;
				widget->fields = selection->fields;

				INT_PTR result = DialogBoxParam(hInst, MAKEINTRESOURCE(IDD_EDIT), hWnd, Edit, (LPARAM)widget);

				if (result == IDOK && dashboard.replace(widget, widget->rect))
				{
					selection->title = widget->title;
					selection->rect = widget->rect;
					selection->fields = widget->fields;
				}

				delete widget;
			}
			break;
		}

		case ID_DELETE:
		{
			if (selection != nullptr) // To surpress C6011
				dashboard.widgets.erase(dashboard.widgets.begin() + selection->id);

			delete selection;

			dashboard.save();
			break;
		}

		default:
			break;
		}
	}
	break;

	case WM_LBUTTONUP:
	{
		// Drop
		if (drag != nullptr)
		{
			RECT position{ drag->rect.left, drag->rect.top, drag->rect.right, drag->rect.bottom };

			// Scale
			ScaleOut(box, position);

			reposition(resize, drag, position, cursor, box);

			// Scale
			ScaleIn(box, position);

			// Move widget to new place
			dashboard.replace(drag, position);

			// Reset resize flag
			resize = WD_NORESIZE;

			// Reset the drag flag
			drag = nullptr;

			// Repaint window
			InvalidateRect(hWnd, &client, false);
		}

		break;
	}

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
			if (dashboard.weather.success)
				for (int i = 0; i < widget->fields.size(); i++)
				{
					wstring title = wstring(widget->title.begin(), widget->title.end());

					HPEN heading = CreatePen(PS_DASH, 1, BLACK_PEN);

					SelectObject(mdc, heading);

					// Draw title
					TextOut(mdc, scaledRect.left + pwidth / 2, scaledRect.top, title.c_str(), lstrlen(title.c_str()));

					// Underline
					MoveToEx(mdc, scaledRect.left, (scaledRect.top) + box, NULL);
					LineTo(mdc, scaledRect.right, (scaledRect.top) + box);

					// Find value and show it.
					if (widget->data[i][0]->s != nullptr)
					{
						string data = *widget->data[i][0]->s;
						wstring value = wstring(data.begin(), data.end()) + wstring(widget->units[i].begin(), widget->units[i].end());

						TextOut(mdc, widget->rect.left * box + pwidth / 2, widget->rect.top * box + (pheight + box / 2) / 2, value.c_str(), lstrlen(value.c_str()));
					}
					else if (widget->data[i][0]->d != nullptr)
					{
						wstring value = std::to_wstring(*widget->data[i][0]->d);
						value.erase(value.find_last_not_of('0') + 1, string::npos);
						value.erase(value.find_last_not_of('.') + 1, string::npos);
						value += wstring(widget->units[i].begin(), widget->units[i].end());

						TextOut(mdc, widget->rect.left * box + pwidth / 2, widget->rect.top * box + (pheight + box / 2) / 2, value.c_str(), lstrlen(value.c_str()));
					}
					else if (widget->data[i][0]->i != nullptr)
					{
						wstring value = std::to_wstring(*widget->data[i][0]->i);

						TextOut(mdc, widget->rect.left * box + pwidth / 2, widget->rect.top * box + (pheight + box / 2) / 2, std::to_wstring(*widget->data[i][0]->i).c_str(), lstrlen(value.c_str()));
					}
					else if (widget->data[i][0]->v != nullptr)
					{
						vector<double> data(*widget->data[i][0]->v);
						for (UINT j = 0; j < height - 1 && j < data.size(); j++)
						{
							wstring value = wstring(dashboard.hourtime[j].begin(), dashboard.hourtime[j].end()) + L" " + std::to_wstring(data[j]);
							value.erase(value.find_last_not_of('0') + 1, string::npos);
							value.erase(value.find_last_not_of('.') + 1, string::npos);
							value += wstring(widget->units[i].begin(), widget->units[i].end());

							TextOut(mdc, widget->rect.left * box + pwidth / 2, widget->rect.top * box + box * j + box, value.c_str(), lstrlen(value.c_str()));
						}
					}

					DeleteObject(heading);
				}
		}

		// Draggage
		if (drag != nullptr)
		{
			RECT position{ drag->rect.left, drag->rect.top, drag->rect.right, drag->rect.bottom };

			// Scale
			ScaleOut(box, position);

			// Position
			reposition(resize, drag, position, cursor, box);

			InvertRect(mdc, &position);
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

		configure("weather.conf", [](string line) {
			std::istringstream iss(line);
			string command;
			iss >> command;
			if (command == "widget")
			{
				// Find the widget position
				RECT position{};
				iss >> position.left >> position.top >> position.right >> position.bottom;

				// Create widget
				Widget* widget = new Widget((int)dashboard.widgets.size(), position);

				// Set widget title
				string title;
				while (iss >> title)
				{
					if (title.back() == '"')
					{
						widget->title += title;
						break;
					}
					else
						widget->title += title + " ";
				}

				widget->title.erase(0, widget->title.find_first_not_of('"'));
				widget->title.erase(widget->title.find_last_not_of('"') + 1, string::npos);

				// Set widget fields
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
			else if (command == "elevation")
				iss >> dashboard.weather.elevation;
			else if (command == "temperatureunit")
				iss >> dashboard.weather.temperature_unit;
			else if (command == "windspeedunit")
				iss >> dashboard.weather.windspeed_unit;
			else if (command == "precipitationunit")
				iss >> dashboard.weather.precipitation_unit;
			else if (command == "cellselection")
				iss >> dashboard.weather.cell_selection;
			else;
			});

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

		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
