#include "dialog.h"

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
