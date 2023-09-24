#include "dialog.h"
#include "framework.h"
#include "weather-panel.h"
#include "weather-api.h"
#include "widget.h"
#include "settings.h"
#include <string>
#include <fstream>
#include <sstream>
#include <wchar.h>
#include <commctrl.h>

// Forward declarations
void TreeBranch(HWND, LPWSTR, HTREEITEM);
USHORT ShiftPosition(HWND, int, int);

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

// Message handler for settings box.
INT_PTR CALLBACK Settings(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
	{
		configure("weather.conf", [&hDlg](string line) {
			std::istringstream iss(line);
			std::string command;
			std::string value;

			iss >> command;
			iss >> value;

			std::wstring wvalue(value.begin(), value.end());

			if (command == "box")
				SetDlgItemInt(hDlg, IDC_BOX, 20, false);
			else if (command == "timezone")
				SetDlgItemText(hDlg, IDC_TIMEZONE, wvalue.c_str());
			else if (command == "latitude")
				SetDlgItemText(hDlg, IDC_LATITUDE, wvalue.c_str());
			else if (command == "longitude")
				SetDlgItemText(hDlg, IDC_LONGITUDE, wvalue.c_str());
			else if (command == "elevation")
				SetDlgItemText(hDlg, IDC_ELEVATION, wvalue.c_str());
			else if (command == "temperatureunit")
				CheckRadioButton(hDlg, IDC_UTEMPERATURE_F, IDC_UTEMPERATURE_C, IDC_UTEMPERATURE_F);
			else if (command == "windspeedunit")
				CheckRadioButton(hDlg, IDC_UWINDSPEED_KMH, IDC_UWINDSPEED_KN, IDC_UWINDSPEED_MH);
			else if (command == "precipitationunit")
				CheckRadioButton(hDlg, IDC_UPRECIPITATION_MM, IDC_UPRECIPITATION_INCH, IDC_UPRECIPITATION_INCH);
			else if (command == "cellselection")
				CheckRadioButton(hDlg, IDC_SCELL_LAND, IDC_SCELL_SEA, IDC_SCELL_LAND);
			else;
			});

		return (INT_PTR)TRUE;
	}

	case WM_COMMAND:
		if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDOK)
		{
			//IsDlgButtonChecked(hDlg, buttonID) == BST_CHECKED

			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDC_RESET)
		{
			SendMessage(hDlg, WM_INITDIALOG, NULL, NULL);
			return (INT_PTR)TRUE;
		}
		break;
	}

	return (INT_PTR)FALSE;
}

// Message handler for edit box.
INT_PTR CALLBACK Edit(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HWND hwndTV;
	static Widget* widget;

	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
	{
		hwndTV = GetDlgItem(hDlg, IDC_FIELDS);
		widget = (Widget*)lParam;

		SetDlgItemText(hDlg, IDC_TITLE, std::wstring(widget->title.begin(), widget->title.end()).c_str());
		SetDlgItemText(hDlg, IDC_LEFT, std::to_wstring(widget->rect.left + 1).c_str());
		SetDlgItemText(hDlg, IDC_RIGHT, std::to_wstring(widget->rect.right + 1).c_str());
		SetDlgItemText(hDlg, IDC_TOP, std::to_wstring(widget->rect.top + 1).c_str());
		SetDlgItemText(hDlg, IDC_BOTTOM, std::to_wstring(widget->rect.bottom + 1).c_str());

		HTREEITEM hNext;

		for (std::string field : api::options::setting)
			TreeBranch(hwndTV, const_cast<LPWSTR>(std::wstring(field.begin(), field.end()).c_str()), TVI_ROOT);

		HTREEITEM hti = TreeView_GetNextItem(hwndTV, NULL, TVGN_ROOT);

		TreeBranch(hwndTV, const_cast<LPWSTR>(L"hourly"), TVI_ROOT);

		hNext = TreeView_GetNextItem(hwndTV, hti, TVGN_NEXT);
		while (hNext)
		{
			hti = hNext;
			hNext = TreeView_GetNextItem(hwndTV, hti, TVGN_NEXT);
		}

		for (std::string field : api::options::hourly)
			TreeBranch(hwndTV, const_cast<LPWSTR>(std::wstring(field.begin(), field.end()).c_str()), hti);

		TreeBranch(hwndTV, const_cast<LPWSTR>(L"daily"), TVI_ROOT);

		hNext = TreeView_GetNextItem(hwndTV, hti, TVGN_NEXT);
		TreeView_SetCheckState(hwndTV, hNext, BST_CHECKED);
		while (hNext)
		{
			hti = hNext;
			hNext = TreeView_GetNextItem(hwndTV, hti, TVGN_NEXT);
		}

		for (std::string field : api::options::daily)
			TreeBranch(hwndTV, const_cast<LPWSTR>(std::wstring(field.begin(), field.end()).c_str()), hti);

		return (INT_PTR)TRUE;
	}

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDCANCEL:
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;

		case IDOK:
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;

		case IDC_ADD_L:
			widget->rect.left = ShiftPosition(hDlg, IDC_LEFT, 1) - 1;
			break;

		case IDC_SUBTRACT_L:
			widget->rect.left = ShiftPosition(hDlg, IDC_LEFT, -1) - 1;
			break;

		case IDC_ADD_R:
			widget->rect.right = ShiftPosition(hDlg, IDC_RIGHT, 1) - 1;
			break;

		case IDC_SUBTRACT_R:
			widget->rect.right = ShiftPosition(hDlg, IDC_RIGHT, -1) - 1;
			break;

		case IDC_ADD_T:
			widget->rect.top = ShiftPosition(hDlg, IDC_TOP, 1) - 1;
			break;

		case IDC_SUBTRACT_T:
			widget->rect.top = ShiftPosition(hDlg, IDC_TOP, -1) - 1;
			break;

		case IDC_ADD_B:
			widget->rect.bottom = ShiftPosition(hDlg, IDC_BOTTOM, 1) - 1;
			break;

		case IDC_SUBTRACT_B:
			widget->rect.bottom = ShiftPosition(hDlg, IDC_BOTTOM, -1) - 1;
			break;

		case IDC_RESET:
			TreeView_DeleteAllItems(hwndTV);
			SendMessage(hDlg, WM_INITDIALOG, NULL, (LPARAM)widget);
			return (INT_PTR)TRUE;

		default:
			break;
		}
		break;
	}

	return (INT_PTR)FALSE;
}

void TreeBranch(HWND hwndTV, LPWSTR pszText, HTREEITEM hParent)
{
	TVITEM tvi{};
	TVINSERTSTRUCT tvins{};

	tvi.mask = TVIF_TEXT;
	tvi.pszText = pszText;
	tvins.item = tvi;
	tvins.hInsertAfter = (HTREEITEM)TVI_LAST;
	tvins.hParent = hParent;

	TreeView_InsertItem(hwndTV, &tvins);
}

USHORT ShiftPosition(HWND hDlg, int idc, int shift)
{
	// Get value
	LPWSTR value = new WCHAR[8];
	GetDlgItemText(hDlg, idc, value, sizeof(value));

	// Change value
	USHORT evalue = _wtoi(value) + shift;
	std::wstring v = std::to_wstring(evalue);

	// Set value
	SetDlgItemText(hDlg, idc, v.c_str());
	delete[] value;

	return evalue;
}