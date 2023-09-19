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
	static HWND boxControl = GetDlgItem(hDlg, IDC_BOX);
	static HWND latidudeControl = GetDlgItem(hDlg, IDC_LATITUDE);
	static HWND longitudeControl = GetDlgItem(hDlg, IDC_LOGITUDE);
	static HWND elevationControl = GetDlgItem(hDlg, IDC_ELEVATION);
	static HWND timezoneControl = GetDlgItem(hDlg, IDC_TIMEZONE);
	static HWND windspeedKMHControl = GetDlgItem(hDlg, IDC_UWINDSPEED_KMH);
	static HWND windspeedMSControl = GetDlgItem(hDlg, IDC_UWINDSPEED_MS);
	static HWND windspeedMHControl = GetDlgItem(hDlg, IDC_UWINDSPEED_MH);
	static HWND windspeedKNControl = GetDlgItem(hDlg, IDC_UWINDSPEED_KN);
	static HWND temperatureFControl = GetDlgItem(hDlg, IDC_UTEMPERATURE_F);
	static HWND temperatureCControl = GetDlgItem(hDlg, IDC_UTEMPERATURE_C);
	static HWND precipitationMMControl = GetDlgItem(hDlg, IDC_UPRECIPITATION_MM);
	static HWND precipitationINCHControl = GetDlgItem(hDlg, IDC_UPRECIPITATION_INCH);
	static HWND cellselectionLANDControl = GetDlgItem(hDlg, IDC_SCELL_LAND);
	static HWND cellselectionSEAControl = GetDlgItem(hDlg, IDC_SCELL_SEA);

	//IsDlgButtonChecked(hDlg, buttonID) == BST_CHECKED
	

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
					{}
					else if (command == "timezone")
						SetWindowText(timezoneControl, wvalue.c_str());
					else if (command == "latitude")
						SetWindowText(latidudeControl, wvalue.c_str());
					else if (command == "longitude")
						SetWindowText(longitudeControl, wvalue.c_str());
					else if (command == "elevation")
						SetWindowText(elevationControl, wvalue.c_str());
					else if (command == "temperatureunit")
					{}
					else if (command == "windspeedunit")
					{}
					else if (command == "precipitationunit")
					{}
					else if (command == "timeformat")
					{}
					else if (command == "cellselection")
					{}
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
