#include "dialog.h"
#include "framework.h"
#include "weather-panel.h"
#include "weather-api.h"
#include "widget.h"
#include <string>
#include <fstream>
#include <sstream>
#include <wchar.h>
#include <commctrl.h>

// Forward declarations
void TreeBranch(HWND, LPWSTR, UINT, HTREEITEM);
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
	static Dashboard* dashboard;

	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
	{
		dashboard = (Dashboard*)lParam;

		SetDlgItemText(hDlg, IDC_TIMEZONE, std::wstring(dashboard->weather.timezone.begin(), dashboard->weather.timezone.end()).c_str());
		SetDlgItemText(hDlg, IDC_LATITUDE, std::to_wstring(dashboard->weather.latitude).c_str());
		SetDlgItemText(hDlg, IDC_LONGITUDE, std::to_wstring(dashboard->weather.longitude).c_str());
		SetDlgItemText(hDlg, IDC_ELEVATION, std::to_wstring(dashboard->weather.elevation).c_str());

		// Set Windspeed Unit
		if (dashboard->weather.windspeed_unit == "kmh")
			CheckRadioButton(hDlg, IDC_UWINDSPEED_KMH, IDC_UWINDSPEED_KN, IDC_UWINDSPEED_KMH);
		else if (dashboard->weather.windspeed_unit == "ms")
			CheckRadioButton(hDlg, IDC_UWINDSPEED_KMH, IDC_UWINDSPEED_KN, IDC_UWINDSPEED_MS);
		else if (dashboard->weather.windspeed_unit == "kn")
			CheckRadioButton(hDlg, IDC_UWINDSPEED_KMH, IDC_UWINDSPEED_KN, IDC_UWINDSPEED_KN);
		else
			CheckRadioButton(hDlg, IDC_UWINDSPEED_KMH, IDC_UWINDSPEED_KN, IDC_UWINDSPEED_MH);

		// Set Cell Selection
		if (dashboard->weather.cell_selection == "land")
			CheckRadioButton(hDlg, IDC_SCELL_LAND, IDC_SCELL_NEAR, IDC_SCELL_LAND);
		else if (dashboard->weather.cell_selection == "sea")
			CheckRadioButton(hDlg, IDC_SCELL_LAND, IDC_SCELL_NEAR, IDC_SCELL_SEA);
		else
			CheckRadioButton(hDlg, IDC_SCELL_LAND, IDC_SCELL_NEAR, IDC_SCELL_NEAR);

		// Set Precipitation Unit
		if (dashboard->weather.precipitation_unit == "mm")
			CheckRadioButton(hDlg, IDC_UPRECIPITATION_MM, IDC_UPRECIPITATION_INCH, IDC_UPRECIPITATION_MM);
		else
			CheckRadioButton(hDlg, IDC_UPRECIPITATION_MM, IDC_UPRECIPITATION_INCH, IDC_UPRECIPITATION_INCH);

		// Set Temperature Unit
		if (dashboard->weather.temperature_unit == "celsius")
			CheckRadioButton(hDlg, IDC_UTEMPERATURE_F, IDC_UTEMPERATURE_C, IDC_UTEMPERATURE_C);
		else
			CheckRadioButton(hDlg, IDC_UTEMPERATURE_F, IDC_UTEMPERATURE_C, IDC_UTEMPERATURE_F);

		return (INT_PTR)TRUE;
	}

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_TIMEZONE_AUTO:
			SetDlgItemText(hDlg, IDC_TIMEZONE, L"auto");
			return (INT_PTR)TRUE;

		case IDCANCEL:
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;

		case IDOK:
			//IsDlgButtonChecked(hDlg, buttonID) == BST_CHECKED

			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;

		case IDC_RESET:
			SendMessage(hDlg, WM_INITDIALOG, NULL, NULL);
			return (INT_PTR)TRUE;

		default:
			break;
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

		// Current treeview item
		HTREEITEM hti = TreeView_GetNextItem(hwndTV, NULL, TVGN_ROOT);

		// Append items to last branch
		for (std::string field : api::options::setting)
			TreeBranch(hwndTV, const_cast<LPWSTR>(std::wstring(field.begin(), field.end()).c_str()), widget->fields.contains(field), TVI_ROOT);

		TreeBranch(hwndTV, const_cast<LPWSTR>(L"hourly"), FALSE, TVI_ROOT);

		// Get last treeview item
		hti = TreeView_GetNextItem(hwndTV, hti, TVGN_LASTVISIBLE);

		// Append items to last branch
		for (std::string field : api::options::hourly)
			TreeBranch(hwndTV, const_cast<LPWSTR>(std::wstring(field.begin(), field.end()).c_str()), widget->fields.contains(field), hti);

		TreeBranch(hwndTV, const_cast<LPWSTR>(L"daily"), FALSE, TVI_ROOT);

		// Get last treeview item
		hti = TreeView_GetNextItem(hwndTV, hti, TVGN_LASTVISIBLE);

		// Append items to last branch
		for (std::string field : api::options::daily)
			TreeBranch(hwndTV, const_cast<LPWSTR>(std::wstring(field.begin(), field.end()).c_str()), widget->fields.contains(field), hti);

		return (INT_PTR)TRUE;
	}

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDCANCEL:
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;

		case IDOK:
		{
			HTREEITEM hti;

			// Clear old field settings
			widget->fields.clear();

			auto check = [](HTREEITEM hti, auto& check) {
				while (hti)
				{
					WCHAR text[MAX_PATH]{};
					TVITEM tvi{};

					tvi.hItem = hti;
					tvi.mask = TVIF_TEXT | TVIF_STATE;
					tvi.pszText = text;
					tvi.cchTextMax = MAX_PATH;
					tvi.stateMask = TVIS_STATEIMAGEMASK;

					// Get item
					TreeView_GetItem(hwndTV, &tvi);

					// Add field
					if (STATEIMAGEMASKTOINDEX(tvi.state) - 1)
					{
						std::wstring wtext(tvi.pszText);
						widget->fields.insert(string(wtext.begin(), wtext.end()));
					}

					// Get next item
					check(TreeView_GetNextItem(hwndTV, hti, TVGN_CHILD), check);
					hti = TreeView_GetNextItem(hwndTV, hti, TVGN_NEXT);
				}
			};

			check(hti, check);


			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}

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

void TreeBranch(HWND hwndTV, LPWSTR pszText, UINT fCheck, HTREEITEM hParent)
{
	TVITEM tvi{};
	TVINSERTSTRUCT tvins{};

	tvi.mask = TVIF_TEXT | TVIF_STATE;
	tvi.pszText = pszText;
	tvi.stateMask = TVIS_STATEIMAGEMASK;
	tvi.state = INDEXTOSTATEIMAGEMASK(fCheck ? 2 : 1);
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