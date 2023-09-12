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
			for (std::vector<Data*> field : widget->data)
				for (Data* data : field)
				{
					delete data;
				}

			widget->data.clear();

			// Reinitialize 2d vector
			while (widget->data.size() < widget->fields.size())
				widget->data.push_back({});

			// Add data.
			for (int i = 0; i < widget->fields.size(); i++)
			{
				Data* data = new Data;

				if (widget->fields[i] == "latitude" && weather.latitude.has_value())
					data->d = new double(*weather.latitude);
				else if (widget->fields[i] == "longitude" && weather.longitude.has_value())
					data->d = new double(*weather.longitude);
				else if (widget->fields[i] == "elevation" && weather.elevation.has_value())
					data->i = new int64_t(*weather.elevation);
				else if (widget->fields[i] == "timezone" && weather.timezone.has_value())
					data->s = new std::string(*weather.timezone);

				widget->data[i].push_back(data);
			}
		}
}

void Widget::replace(Dashboard* dashboard, int id, RECT rect)
{
	dashboard->replace(id, rect);
}