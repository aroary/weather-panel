#pragma once

#include "settings.h"
#include "framework.h"
#include "weather-panel.h"
#include <fstream>
#include <sstream>

using std::function;
using std::string;
using std::fstream;
using std::getline;
using std::ios;

bool configure(string file, function<void(string)> cb)
{
	fstream configFile(file, ios::out | ios::in | ios::app);

	// Init config
	if (configFile.is_open())
	{
		string line; // Current line.
		if (getline(configFile, line) && line == "0")
			while (getline(configFile, line))
				cb(line);
		else
		{
			MessageBox(NULL, L"Problem reading configurations.\nPress OK to generate default configuration.", L"Configuration Error", MB_ICONEXCLAMATION | MB_OK);

			configFile.close();
			configFile.open(file, ios::out | ios::trunc);
			configFile.write("0\n", sizeof("0\n"));

			return false;
		}

		configFile.close();
	}
	else
	{
		MessageBox(NULL, L"Faild to read configuration file.", L"Configuration Error", MB_ICONERROR | MB_OK);
		return false;
	}

	return true;
}