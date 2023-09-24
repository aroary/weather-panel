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
	this->units.clear();
}

bool Dashboard::replace(Widget* select, RECT rect)
{
	// Validate new widget size.
	if (rect.right - rect.left < 2 || rect.bottom - rect.top < 2)
		return false;

	// Check for intersection with existing widgets.
	for (Widget* widget : this->widgets)
		if (widget->id != select->id && rect.left < widget->rect.right && rect.right > widget->rect.left && rect.top < widget->rect.bottom && rect.bottom > widget->rect.top)
			return false;

	// Relocate to new position.
	select->rect = { rect.left, rect.top, rect.right, rect.bottom };

	// Save the changes
	this->save();

	// Return success
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
		file << "timezone " << this->weather.timezone << endl;
		file << "cellselection " << this->weather.cell_selection << endl;

		// Save widgets
		for (Widget* widget : this->widgets)
		{
			file << "widget " << widget->rect.left << " " << widget->rect.top << " " << widget->rect.right << " " << widget->rect.bottom;
			file << " \"" << widget->title << '"';

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
	// Set api parameters from widget data.
	for (Widget* widget : this->widgets)
		for (string field : widget->fields)
		{
			string category = api::options::category(field);
			if (category == "setting")
				continue;
			else if (category == "hourly")
				this->weather.hourly += field + ",";
			else if (category == "daily")
				this->weather.daily += field + ",";
			else
				continue;
		}

	this->hourtime.clear();
	this->daytime.clear();

	// Call api
	api::json weather = this->weather.weather(this->weather.url("api.open-meteo.com", ""));

	if (weather.count("error"))
	{
		this->weather.success = false;
		string reason = weather["reason"].get<std::string>();
		MessageBox(NULL, std::wstring(reason.begin(), reason.end()).c_str(), L"API Error", MB_ICONERROR | MB_OK);
	}
	else
	{
		this->weather.success = true;
		
		if (weather.count("hourly"))
			this->hourtime = weather["hourly"]["time"].get<vector<string>>();
		
		if (weather.count("daily"))
			this->daytime = weather["daily"]["time"].get<vector<string>>();

		for (Widget* widget : this->widgets)
		{
			// Clear old data
			for (vector<Data*> field : widget->data)
				for (Data* data : field)
					delete data;

			widget->data.clear();
			widget->units.clear();

			// Reinitialize 2d vector
			while (widget->data.size() < widget->fields.size())
				widget->data.push_back({});

			// Add data.
			for (int i = 0; i < widget->fields.size(); i++)
			{
				Data* data = new Data;

				if (weather.count(widget->fields[i]))
				{
					if (weather[widget->fields[i]].is_string())
						data->s = new string(weather[widget->fields[i]].get<string>());
					else if (weather[widget->fields[i]].is_number())
						data->d = new double(weather[widget->fields[i]].get<double>());

					widget->units.push_back("");
				}
				else if (weather.count("hourly") && weather["hourly"].count(widget->fields[i]))
				{
					data->v = new vector<double>(weather["hourly"][widget->fields[i]].get<vector<double>>());
					widget->units.push_back(weather["hourly_units"][widget->fields[i]].get<string>());
				}
				else if (weather.count("daily") && weather["daily"].count(widget->fields[i]))
				{
					data->v = new vector<double>(weather["daily"][widget->fields[i]].get<vector<double>>());
					widget->units.push_back(weather["hourly_units"][widget->fields[i]].get<string>());
				}

				widget->data[i].push_back(data);
			}
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