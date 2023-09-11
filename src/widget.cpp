#include "widget.h"

Widget::Widget(int id, RECT rect) : rect{ rect.left, rect.top, rect.right, rect.bottom } {
	this->id = id;
}

void Dashboard::replace(int id, RECT rect)
{
	this->widgets.at(id)->rect = rect;
}

void Dashboard::update()
{
	this->weather.latitude = 8.765;
	this->weather.longitude = 43.218999;
	this->weather.current_weather = true;

	json::Weather weather = this->weather.weather(this->weather.url("api.open-meteo.com", ""));

	if (weather.error.has_value() && weather.error.value())
	{
		std::string message = weather.reason.value();
		MessageBox(NULL, std::wstring(message.begin(), message.end()).c_str(), L"API Error", MB_ICONERROR | MB_OK);
	}
	else
		for (Widget* widget : this->widgets)
		{
			// Clear old data
			widget->data.clear();

			// Reinitialize 2d vector
			while (widget->data.size() < widget->fields.size())
				widget->data.push_back({});

			for (int i = 0; i < widget->fields.size(); i++)
				if (widget->fields[i] == "latitude")
					widget->data[i].push_back({ .d = &weather.latitude.value() });
				else if (widget->fields[i] == "longitude")
					widget->data[i].push_back({ .d = &weather.longitude.value() });
				else if (widget->fields[i] == "elevation")
					widget->data[i].push_back({ .i = &weather.elevation.value() });
		}
}

void Widget::replace(Dashboard* dashboard, int id, RECT rect)
{
	dashboard->replace(id, rect);
}