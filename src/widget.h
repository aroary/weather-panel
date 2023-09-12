#pragma once

#include <Windows.h>
#include <vector>
#include <iomanip>
#include "weather-api.h"

class Widget;

union Data; // Container for json value

class Dashboard
{
public:
	std::vector<Widget*> widgets;
	void replace(int, RECT);
	void update();

	api::Weather weather;
};

class Widget
{
public:
	Widget(int, RECT);
	void replace(Dashboard*, int, RECT);

	int                              id;      // The index of the widget on the dashboard (vector.size()).
	RECT                             rect;    // The widget size and location.
	std::vector<std::string>         fields;  // Fields of data.
	std::vector<std::vector<Data*>>  data;    // All data is stored as a string and may be converted to a number later.
};

union Data
{
	std::string* s;
	double* d;
	int64_t* i;
};