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
	this->weather.latitude = 98.765;
	this->weather.longitude = 43.21;
	this->weather.current_weather = true;

	json::Weather weather = this->weather.weather(this->weather.url("api.open-meteo.com", ""));
}

void Widget::replace(Dashboard* dashboard, int id, RECT rect)
{
	dashboard->replace(id, rect);
}