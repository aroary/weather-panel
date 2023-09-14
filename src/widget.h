#pragma once

#include <Windows.h>
#include <vector>
#include <iomanip>
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
	vector<Widget*> widgets;
	void replace(int, RECT);
	void update();

	api::Weather weather;
};

class Widget
{
public:
	Widget(int, RECT);

	int                    id;      // The index of the widget on the dashboard (vector.size()).
	RECT                   rect;    // The widget size and location.
	vector<string>         fields;  // Fields of data.
	vector<vector<Data*>>  data;    // All data is stored as a string and may be converted to a number later.
	vector<string*>        units;   // The unit of the data of the field.
};

struct Data
{
	~Data();

	vector<double>* v = nullptr;
	string* s = nullptr;
	double* d = nullptr;
	int64_t* i = nullptr;
};