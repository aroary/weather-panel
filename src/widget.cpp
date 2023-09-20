#include "widget.h"

Widget::Widget(int id, RECT rect) : rect{ rect.left, rect.top, rect.right, rect.bottom } {
	this->id = id;
}

Widget::~Widget()
{
	for (vector<Data*> datas : this->data)
	{
		for (Data* data : datas)
			delete data;

		datas.clear();
	}

	this->data.clear();

	for (string* unit : this->units)
		delete unit;

	this->units.clear();
}

bool Dashboard::replace(int id, RECT rect)
{
	// Validate new widget size.
	if (rect.right - rect.left < 2 || rect.bottom - rect.top < 2)
		return false;

	// Check for intersection with existing widgets.
	for (Widget* widget : this->widgets)
		if (widget->id != id && rect.left < widget->rect.right && rect.right > widget->rect.left && rect.top < widget->rect.bottom && rect.bottom > widget->rect.top)
			return false;

	// Relocate to new position.
	this->widgets.at(id)->rect = { rect.left, rect.top, rect.right, rect.bottom };

	// Save the changes
	this->save();

	return true;
}

bool Dashboard::save()
{
	using std::endl;

	std::fstream file("weather.conf", std::ofstream::out | std::ofstream::trunc);

	if (file.is_open())
	{
		// 0
		file << "0" << endl;

		// Save data
		file << "latitude " << this->weather.latitude << endl;
		file << "longitude " << this->weather.longitude << endl;
		file << "elevation " << this->weather.elevation << endl;
		file << "temperatureunit " << this->weather.temperature_unit << endl;
		file << "windspeedunit " << this->weather.windspeed_unit << endl;
		file << "precipitationunit " << this->weather.precipitation_unit << endl;
		file << "timeformat " << this->weather.timeformat << endl;
		file << "timezone " << this->weather.timezone << endl;
		file << "cellselection " << this->weather.cell_selection << endl;

		// Save widgets
		for (Widget* widget : this->widgets)
		{
			file << "widget " << widget->rect.left << " " << widget->rect.top << " " << widget->rect.right << " " << widget->rect.bottom;

			for (std::string field : widget->fields)
				file << " " << field;

			file << endl;
		}
	}
	else
		return false;

	return true;
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
		for (string field : widget->fields)
			if (field == "hourly.temperature2m")
				this->weather.hourly->temperature = true;
			else if (field == "hourly.relativehumidity2m")
				this->weather.hourly->relativehumidity = true;
			else if (field == "hourly.dewpoint2m")
				this->weather.hourly->dewpoint = true;
			else if (field == "hourly.apparenttemperature")
				this->weather.hourly->apparenttemperature = true;
			else if (field == "hourly.precipitationprobability")
				this->weather.hourly->precipitationprobability = true;
			else if (field == "hourly.precipitation")
				this->weather.hourly->precipitation = true;
			else if (field == "hourly.rain")
				this->weather.hourly->rain = true;
			else if (field == "hourly.showers")
				this->weather.hourly->showers = true;
			else if (field == "hourly.snowfall")
				this->weather.hourly->snowfall = true;
			else if (field == "hourly.snowdepth")
				this->weather.hourly->snowdepth = true;
			else if (field == "hourly.weathercode")
				this->weather.hourly->weathercode = true;
			else if (field == "hourly.pressuremsl")
				this->weather.hourly->pressuremsl = true;
			else if (field == "hourly.surfacepressure")
				this->weather.hourly->surfacepressure = true;
			else if (field == "hourly.cloudcover")
				this->weather.hourly->cloudcover = true;
			else if (field == "hourly.cloudcoverlow")
				this->weather.hourly->cloudcoverlow = true;
			else if (field == "hourly.cloudcovermid")
				this->weather.hourly->cloudcovermid = true;
			else if (field == "hourly.cloudcoverhigh")
				this->weather.hourly->cloudcoverhigh = true;
			else if (field == "hourly.visibility")
				this->weather.hourly->visibility = true;
			else if (field == "hourly.evapotranspiration")
				this->weather.hourly->evapotranspiration = true;
			else if (field == "hourly.et0faoevapotranspiration")
				this->weather.hourly->et0faoevapotranspiration = true;
			else if (field == "hourly.vaporpressuredeficit")
				this->weather.hourly->vaporpressuredeficit = true;
			else if (field == "hourly.windspeed10m")
				this->weather.hourly->windspeed = true;
			else if (field == "hourly.windspeed80m")
				this->weather.hourly->windspeed = true;
			else if (field == "hourly.windspeed120m")
				this->weather.hourly->windspeed = true;
			else if (field == "hourly.windspeed180m")
				this->weather.hourly->windspeed = true;
			else if (field == "hourly.winddirection10m")
				this->weather.hourly->winddirection = true;
			else if (field == "hourly.winddirection80m")
				this->weather.hourly->winddirection = true;
			else if (field == "hourly.winddirection120m")
				this->weather.hourly->winddirection = true;
			else if (field == "hourly.winddirection180m")
				this->weather.hourly->winddirection = true;
			else if (field == "hourly.windgusts10m")
				this->weather.hourly->windgusts = true;
			else if (field == "hourly.temperature80m")
				this->weather.hourly->temperature = true;
			else if (field == "hourly.temperature120m")
				this->weather.hourly->temperature = true;
			else if (field == "hourly.temperature180m")
				this->weather.hourly->temperature = true;
			else if (field == "daily.apparenttemperaturemax")
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
		string message = weather.reason.value();
		MessageBox(NULL, std::wstring(message.begin(), message.end()).c_str(), L"API Error", MB_ICONERROR | MB_OK);
	}
	else
		for (Widget* widget : this->widgets)
		{
			// Clear old data
			for (vector<Data*> field : widget->data)
				for (Data* data : field)
					delete data;

			for (string* unit : widget->units)
				delete unit;

			widget->data.clear();
			widget->units.clear();

			// Reinitialize 2d vector
			while (widget->data.size() < widget->fields.size())
				widget->data.push_back({});

			// Add data.
			for (int i = 0; i < widget->fields.size(); i++)
			{
				Data* data = new Data;

				if (widget->fields[i] == "latitude" && weather.latitude.has_value())
				{
					data->d = new double(*weather.latitude);
					widget->units.push_back(new string(""));
				}
				else if (widget->fields[i] == "longitude" && weather.longitude.has_value())
				{
					data->d = new double(*weather.longitude);
					widget->units.push_back(new string(""));
				}
				else if (widget->fields[i] == "generationtime" && weather.generationtime_ms.has_value())
				{
					data->d = new double(*weather.generationtime_ms);
					widget->units.push_back(new string("ms"));
				}
				else if (widget->fields[i] == "timezone" && weather.timezone.has_value())
				{
					data->s = new string(*weather.timezone);
					widget->units.push_back(new string(""));
				}
				else if (widget->fields[i] == "utcoffsetseconds" && weather.utc_offset_seconds.has_value())
				{
					data->i = new int64_t(*weather.utc_offset_seconds);
					widget->units.push_back(new string("s"));
				}
				else if (widget->fields[i] == "timezoneabbreviation")
				{
					data->s = new string(*weather.timezone_abbreviation);
					widget->units.push_back(new string(""));
				}
				else if (widget->fields[i] == "elevation" && weather.elevation.has_value())
				{
					data->i = new int64_t(*weather.elevation);
					widget->units.push_back(new string("m"));
				}
				else if (widget->fields[i] == "hourly.temperature2m" && (*weather.hourly).temperature_2m.has_value())
				{
					data->v = new vector<double>(*(*weather.hourly).temperature_2m);
					widget->units.push_back(new string(*(*weather.hourly_units).temperature_2m));
				}
				else if (widget->fields[i] == "hourly.relativehumidity2m" && (*weather.hourly).relativehumidity_2m.has_value())
				{
					data->v = new vector<double>(*(*weather.hourly).relativehumidity_2m);
					widget->units.push_back(new string(*(*weather.hourly_units).relativehumidity_2m));
				}
				else if (widget->fields[i] == "hourly.dewpoint2m" && (*weather.hourly).dewpoint_2m.has_value())
				{
					data->v = new vector<double>(*(*weather.hourly).dewpoint_2m);
					widget->units.push_back(new string(*(*weather.hourly_units).dewpoint_2m));
				}
				else if (widget->fields[i] == "hourly.apparenttemperature" && (*weather.hourly).apparent_temperature.has_value())
				{
					data->v = new vector<double>(*(*weather.hourly).apparent_temperature);
					widget->units.push_back(new string(*(*weather.hourly_units).apparent_temperature));
				}
				else if (widget->fields[i] == "hourly.precipitationprobability" && (*weather.hourly).precipitation_probability.has_value())
				{
					data->v = new vector<double>(*(*weather.hourly).precipitation_probability);
					widget->units.push_back(new string(*(*weather.hourly_units).precipitation_probability));
				}
				else if (widget->fields[i] == "hourly.precipitation" && (*weather.hourly).precipitation.has_value())
				{
					data->v = new vector<double>(*(*weather.hourly).precipitation);
					widget->units.push_back(new string(*(*weather.hourly_units).precipitation));

				}
				else if (widget->fields[i] == "hourly.rain" && (*weather.hourly).rain.has_value())
				{
					data->v = new vector<double>(*(*weather.hourly).rain);
					widget->units.push_back(new string(*(*weather.hourly_units).rain));
				}
				else if (widget->fields[i] == "hourly.showers" && (*weather.hourly).showers.has_value())
				{
					data->v = new vector<double>(*(*weather.hourly).showers);
					widget->units.push_back(new string(*(*weather.hourly_units).showers));

				}
				else if (widget->fields[i] == "hourly.snowfall" && (*weather.hourly).snowfall.has_value())
				{
					data->v = new vector<double>(*(*weather.hourly).snowfall);
					widget->units.push_back(new string(*(*weather.hourly_units).snowfall));

				}
				else if (widget->fields[i] == "hourly.snowdepth" && (*weather.hourly).snow_depth.has_value())
				{
					data->v = new vector<double>(*(*weather.hourly).snow_depth);
					widget->units.push_back(new string(*(*weather.hourly_units).snow_depth));

				}
				else if (widget->fields[i] == "hourly.weathercode" && (*weather.hourly).weathercode.has_value())
				{
					data->v = new vector<double>(*(*weather.hourly).weathercode);
					widget->units.push_back(new string(*(*weather.hourly_units).weathercode));
				}
				else if (widget->fields[i] == "hourly.pressuremsl" && (*weather.hourly).pressure_msl.has_value())
				{
					data->v = new vector<double>(*(*weather.hourly).pressure_msl);
					widget->units.push_back(new string(*(*weather.hourly_units).pressure_msl));

				}
				else if (widget->fields[i] == "hourly.surfacepressure" && (*weather.hourly).surface_pressure.has_value())
				{
					data->v = new vector<double>(*(*weather.hourly).surface_pressure);
					widget->units.push_back(new string(*(*weather.hourly_units).surface_pressure));

				}
				else if (widget->fields[i] == "hourly.cloudcover" && (*weather.hourly).cloudcover.has_value())
				{
					data->v = new vector<double>(*(*weather.hourly).cloudcover);
					widget->units.push_back(new string(*(*weather.hourly_units).cloudcover));

				}
				else if (widget->fields[i] == "hourly.cloudcoverlow" && (*weather.hourly).cloudcover_low.has_value())
				{
					data->v = new vector<double>(*(*weather.hourly).cloudcover_low);
					widget->units.push_back(new string(*(*weather.hourly_units).cloudcover_low));

				}
				else if (widget->fields[i] == "hourly.cloudcovermid" && (*weather.hourly).cloudcover_mid.has_value())
				{
					data->v = new vector<double>(*(*weather.hourly).cloudcover_mid);
					widget->units.push_back(new string(*(*weather.hourly_units).cloudcover_mid));

				}
				else if (widget->fields[i] == "hourly.cloudcoverhigh" && (*weather.hourly).cloudcover_high.has_value())
				{
					data->v = new vector<double>(*(*weather.hourly).cloudcover_high);
					widget->units.push_back(new string(*(*weather.hourly_units).cloudcover_high));
				}
				else if (widget->fields[i] == "hourly.visibility" && (*weather.hourly).visibility.has_value())
				{
					data->v = new vector<double>(*(*weather.hourly).visibility);
					widget->units.push_back(new string(*(*weather.hourly_units).visibility));
				}
				else if (widget->fields[i] == "hourly.evapotranspiration" && (*weather.hourly).evapotranspiration.has_value())
				{
					data->v = new vector<double>(*(*weather.hourly).evapotranspiration);
					widget->units.push_back(new string(*(*weather.hourly_units).evapotranspiration));
				}
				else if (widget->fields[i] == "hourly.et0faoevapotranspiration" && (*weather.hourly).et0_fao_evapotranspiration.has_value())
				{
					data->v = new vector<double>(*(*weather.hourly).et0_fao_evapotranspiration);
					widget->units.push_back(new string(*(*weather.hourly_units).et0_fao_evapotranspiration));
				}
				else if (widget->fields[i] == "hourly.vaporpressuredeficit" && (*weather.hourly).vapor_pressure_deficit.has_value())
				{
					data->v = new vector<double>(*(*weather.hourly).vapor_pressure_deficit);
					widget->units.push_back(new string(*(*weather.hourly_units).vapor_pressure_deficit));
				}
				else if (widget->fields[i] == "hourly.windspeed10m" && (*weather.hourly).windspeed_10m.has_value())
				{
					data->v = new vector<double>(*(*weather.hourly).windspeed_10m);
					widget->units.push_back(new string(*(*weather.hourly_units).windspeed_10m));
				}
				else if (widget->fields[i] == "hourly.windspeed80m" && (*weather.hourly).windspeed_80m.has_value())
				{
					data->v = new vector<double>(*(*weather.hourly).windspeed_80m);
					widget->units.push_back(new string(*(*weather.hourly_units).windspeed_80m));
				}
				else if (widget->fields[i] == "hourly.windspeed120m" && (*weather.hourly).windspeed_120m.has_value())
				{
					data->v = new vector<double>(*(*weather.hourly).windspeed_120m);
					widget->units.push_back(new string(*(*weather.hourly_units).windspeed_120m));
				}
				else if (widget->fields[i] == "hourly.windspeed180m" && (*weather.hourly).windspeed_180m.has_value())
				{
					data->v = new vector<double>(*(*weather.hourly).windspeed_180m);
					widget->units.push_back(new string(*(*weather.hourly_units).windspeed_180m));
				}
				else if (widget->fields[i] == "hourly.winddirection10m" && (*weather.hourly).winddirection_10m.has_value())
				{
					data->v = new vector<double>(*(*weather.hourly).winddirection_10m);
					widget->units.push_back(new string(*(*weather.hourly_units).winddirection_10m));
				}
				else if (widget->fields[i] == "hourly.winddirection80m" && (*weather.hourly).winddirection_80m.has_value())
				{
					data->v = new vector<double>(*(*weather.hourly).winddirection_80m);
					widget->units.push_back(new string(*(*weather.hourly_units).winddirection_80m));
				}
				else if (widget->fields[i] == "hourly.winddirection120m" && (*weather.hourly).winddirection_120m.has_value())
				{
					data->v = new vector<double>(*(*weather.hourly).winddirection_120m);
					widget->units.push_back(new string(*(*weather.hourly_units).winddirection_120m));
				}
				else if (widget->fields[i] == "hourly.winddirection180m" && (*weather.hourly).winddirection_180m.has_value())
				{
					data->v = new vector<double>(*(*weather.hourly).winddirection_180m);
					widget->units.push_back(new string(*(*weather.hourly_units).winddirection_180m));
				}
				else if (widget->fields[i] == "hourly.windgusts10m" && (*weather.hourly).windgusts_10m.has_value())
				{
					data->v = new vector<double>(*(*weather.hourly).windgusts_10m);
					widget->units.push_back(new string(*(*weather.hourly_units).windgusts_10m));
				}
				else if (widget->fields[i] == "hourly.temperature80m" && (*weather.hourly).temperature_80m.has_value())
				{
					data->v = new vector<double>(*(*weather.hourly).temperature_80m);
					widget->units.push_back(new string(*(*weather.hourly_units).temperature_80m));
				}
				else if (widget->fields[i] == "hourly.temperature120m" && (*weather.hourly).temperature_120m.has_value())
				{
					data->v = new vector<double>(*(*weather.hourly).temperature_120m);
					widget->units.push_back(new string(*(*weather.hourly_units).temperature_120m));
				}
				else if (widget->fields[i] == "hourly.temperature180m" && (*weather.hourly).temperature_180m.has_value())
				{
					data->v = new vector<double>(*(*weather.hourly).temperature_180m);
					widget->units.push_back(new string(*(*weather.hourly_units).temperature_180m));
				}
				else if (widget->fields[i] == "hourly.soiltemperature0cm" && (*weather.hourly).soil_temperature_0cm.has_value())
				{
					data->v = new vector<double>(*(*weather.hourly).soil_temperature_0cm);
					widget->units.push_back(new string(*(*weather.hourly_units).soil_temperature_0cm));
				}
				else if (widget->fields[i] == "hourly.soiltemperature6cm" && (*weather.hourly).soil_temperature_6cm.has_value())
				{
					data->v = new vector<double>(*(*weather.hourly).soil_temperature_6cm);
					widget->units.push_back(new string(*(*weather.hourly_units).soil_temperature_6cm));
				}
				else if (widget->fields[i] == "hourly.soiltemperature18cm" && (*weather.hourly).soil_temperature_18cm.has_value())
				{
					data->v = new vector<double>(*(*weather.hourly).soil_temperature_18cm);
					widget->units.push_back(new string(*(*weather.hourly_units).soil_temperature_18cm));
				}
				else if (widget->fields[i] == "hourly.soiltemperature54cm" && (*weather.hourly).soil_temperature_54cm.has_value())
				{
					data->v = new vector<double>(*(*weather.hourly).soil_temperature_54cm);
					widget->units.push_back(new string(*(*weather.hourly_units).soil_temperature_54cm));
				}
				else if (widget->fields[i] == "hourly.soilmoisture01cm" && (*weather.hourly).soil_moisture_0_1cm.has_value())
				{
					data->v = new vector<double>(*(*weather.hourly).soil_moisture_0_1cm);
					widget->units.push_back(new string(*(*weather.hourly_units).soil_moisture_0_1cm));
				}
				else if (widget->fields[i] == "hourly.soilmoisture13cm" && (*weather.hourly).soil_moisture_1_3cm.has_value())
				{
					data->v = new vector<double>(*(*weather.hourly).soil_moisture_1_3cm);
					widget->units.push_back(new string(*(*weather.hourly_units).soil_moisture_1_3cm));
				}
				else if (widget->fields[i] == "hourly.soilmoisture39cm" && (*weather.hourly).soil_moisture_3_9cm.has_value())
				{
					data->v = new vector<double>(*(*weather.hourly).soil_moisture_3_9cm);
					widget->units.push_back(new string(*(*weather.hourly_units).soil_moisture_3_9cm));
				}
				else if (widget->fields[i] == "hourly.soil_moisture927cm" && (*weather.hourly).soil_moisture_9_27cm.has_value())
				{
					data->v = new vector<double>(*(*weather.hourly).soil_moisture_9_27cm);
					widget->units.push_back(new string(*(*weather.hourly_units).soil_moisture_9_27cm));
				}
				else if (widget->fields[i] == "hourly.soilmoisture2781cm" && (*weather.hourly).soil_moisture_27_81cm.has_value())
				{
					data->v = new vector<double>(*(*weather.hourly).soil_moisture_27_81cm);
					widget->units.push_back(new string(*(*weather.hourly_units).soil_moisture_27_81cm));
				}
				else widget->units.push_back(new string(""));

				widget->data[i].push_back(data);
			}
		}
}

Data::~Data()
{
	delete this->s;
	delete this->d;
	delete this->i;
	delete this->v;
}