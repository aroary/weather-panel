#pragma once

#include <Windows.h>
#include <vector>
#include "weather-api.h"

class Widget;

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

	int                      id;      // The index of the widget on the dashboard (vector.size()).
	RECT                     rect;    // The widget size and location.
	std::vector<std::string> fields;  //
};