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
	// Clear api parameters.
	delete this->weather.hourly;
	delete this->weather.daily;
	this->weather.hourly = new api::Hourly;
	this->weather.daily = new api::Daily;

	// Set api parameters from widget data.
	for (Widget* widget : this->widgets)
		for (std::string field : widget->fields)
			if (field == "daily.apparenttemperaturemax")
				this->weather.daily->apparenttemperaturemax = true;
			else if (field == "daily.apparenttemperaturemin")
				this->weather.daily->apparenttemperaturemin = true;
			else if (field == "daily.et0faoevapotranspiration")
				this->weather.daily->et0faoevapotranspiration = true;
			else if (field == "daily.precipitationhours")
				this->weather.daily->precipitationhours = true;
			else if (field == "daily.precipitationprobabilitymax")
				this->weather.daily->precipitationprobabilitymax = true;
			else if (field == "daily.precipitationprobabilitymean")
				this->weather.daily->precipitationprobabilitymean = true;
			else if (field == "daily.precipitationprobabilitymin")
				this->weather.daily->precipitationprobabilitymin = true;
			else if (field == "daily.precipitationsum")
				this->weather.daily->precipitationsum = true;
			else if (field == "daily.rainsum")
				this->weather.daily->rainsum = true;
			else if (field == "daily.shortwaveradiationsum")
				this->weather.daily->shortwaveradiationsum = true;
			else if (field == "daily.showerssum")
				this->weather.daily->showerssum = true;
			else if (field == "daily.snowfallsum")
				this->weather.daily->snowfallsum = true;
			else if (field == "daily.sunrise")
				this->weather.daily->sunrise = true;
			else if (field == "daily.sunset")
				this->weather.daily->sunset = true;
			else if (field == "daily.temperaturemax")
				this->weather.daily->temperaturemax = true;
			else if (field == "daily.temperaturemin")
				this->weather.daily->temperaturemin = true;
			else if (field == "daily.uvindexclearskymax")
				this->weather.daily->uvindexclearskymax = true;
			else if (field == "daily.uvindexmax")
				this->weather.daily->uvindexmax = true;
			else if (field == "daily.weathercode")
				this->weather.daily->weathercode = true;
			else if (field == "daily.winddirectiondominant")
				this->weather.daily->winddirectiondominant = true;
			else if (field == "daily.windgustsmax")
				this->weather.daily->windgustsmax = true;
			else if (field == "daily.windspeedmax")
				this->weather.daily->windspeedmax = true;

	// Call api
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
					delete data;

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
				else if (widget->fields[i] == "generationtime" && weather.generationtime_ms.has_value())
					data->d = new double(*weather.generationtime_ms);
				else if (widget->fields[i] == "utc_offset_seconds" && weather.utc_offset_seconds.has_value())
					data->i = new int64_t(*weather.utc_offset_seconds);
				else;

				widget->data[i].push_back(data);
			}
		}
}

void Widget::replace(Dashboard* dashboard, int id, RECT rect)
{
	dashboard->replace(id, rect);
}

Data::~Data()
{
	delete this->s;
	delete this->d;
	delete this->i;
}