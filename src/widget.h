#pragma once

#include <Windows.h>
#include <string>
#include <vector>
#include "weather-api.h"

#define WD_NORESIZE     0x00000000L  // No resize action.
#define WD_SOUTHRESIZE  0x00000001L  // Resize down.
#define WD_EASTRESIZE   0x00000010L  // Resize left.
#define WD_WESTRESIZE   0x00000100L  // Resize right.

using std::string;
using std::vector;

class Widget;

struct Data; // Container for json value

class Dashboard
{
public:
	vector<Widget*>  widgets;   // Displayed widgets
	vector<string>   hourtime;  // Time of hour data
	vector<string>   daytime;   // Time of day data

	bool replace(Widget*, RECT);
	bool save();
	void update();

	api::Weather weather;
};

class Widget
{
public:
	Widget(int, RECT);
	~Widget();

	int                    id;      // Index of the widget on the dashboard (vector.size())
	RECT                   rect;    // Widget size and location
	string                 title;   // Header of widget
	vector<string>         fields;  // Fields of data
	vector<vector<Data*>>  data;    // Data pointers
	vector<string>         units;   // The unit of the data of the fields
};

struct Data
{
	~Data();

	vector<double>* v = nullptr;
	string* s = nullptr;
	double* d = nullptr;
	int64_t* i = nullptr;
};

// Change position of widget to mouse position
RECT reposition(int, Widget*, RECT&, POINT, int);

RECT ScaleIn(int, RECT&);
RECT ScaleOut(int, RECT&);
