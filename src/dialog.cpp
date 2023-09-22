#include "dialog.h"

// Forward declarations
void TreeBranch(HWND, LPWSTR, HTREEITEM);
void ShiftPosition(HWND, int, int);

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
		std::fstream configFile("weather.conf", std::ios::out | std::ios::in | std::ios::app);

		if (configFile.is_open())
		{
			std::string line; // Current line.
			if (std::getline(configFile, line) && line == "0")
				while (std::getline(configFile, line))
				{
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
				}
			else
			{
				MessageBox(NULL, L"Problem reading configurations.\nPress OK to generate default configuration.", L"Configuration Error", MB_ICONEXCLAMATION | MB_OK);

				configFile.close();
				configFile.open("weather.conf", std::ofstream::out | std::ofstream::trunc);
				configFile.write("0\n", sizeof("0\n"));
			}

			configFile.close();
		}
		else
			MessageBox(NULL, L"Faild to read configuration file.", L"Configuration Error", MB_ICONERROR | MB_OK);


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
		{
			ShiftPosition(hDlg, IDC_LEFT, 1);
			break;
		}

		case IDC_SUBTRACT_L:
		{
			ShiftPosition(hDlg, IDC_LEFT, -1);
			break;
		}

		case IDC_ADD_R:
		{
			ShiftPosition(hDlg, IDC_RIGHT, 1);
			break;
		}

		case IDC_SUBTRACT_R:
		{
			ShiftPosition(hDlg, IDC_RIGHT, -1);
			break;
		}

		case IDC_ADD_T:
		{
			ShiftPosition(hDlg, IDC_TOP, 1);
			break;
		}

		case IDC_SUBTRACT_T:
		{
			ShiftPosition(hDlg, IDC_TOP, -1);
			break;
		}

		case IDC_ADD_B:
		{
			ShiftPosition(hDlg, IDC_BOTTOM, 1);
			break;
		}

		case IDC_SUBTRACT_B:
		{
			ShiftPosition(hDlg, IDC_BOTTOM, -1);
			break;
		}

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

void ShiftPosition(HWND hDlg, int idc, int shift)
{
	LPWSTR value = new WCHAR[16];
	GetDlgItemText(hDlg, idc, value, sizeof(value));
	int evalue = _wtoi(value) + shift;
	memset(value, 0, sizeof(value));
	_itow_s(evalue, value, sizeof(value), 9);
	SetDlgItemText(hDlg, idc, value);
	delete[] value;
}