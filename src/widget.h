#pragma once

#include <Windows.h>
#include <vector>

class Widget;

class Dashboard
{
public:
	std::vector<Widget*> widgets;
	void replace(int, RECT);
};

class Widget
{
public:
	Widget(int, RECT);
	void replace(Dashboard*, int, RECT);

	int   id;    // The index of the widget on the dashboard (vector.size()).
	RECT  rect;  // The widget size and location.
};