#pragma once

#include <sstream>
#include <string>
#include <vector>
#include <set>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <array>
#include <iomanip>
#include <limits>

#pragma warning(push, 0)
#include "nlohmann/json.hpp"
#pragma warning(pop)

namespace api
{
	using std::string;
	using std::vector;
	using nlohmann::json;

	class Weather
	{
	public:
		string url(string, string);
		json weather(string url);

		float   latitude = 0;                  // Geographical WGS84 coordinate of the location
		float   longitude = 0;                 // Geographical WGS84 coordinate of the location
		float   elevation = NAN;               // The elevation used for statistical downscaling
		bool    current_weather = false;       // Include current weather conditions in the JSON output.
		string  temperature_unit = "celsius";  // If fahrenheit is set, all temperature values are converted to Fahrenheit.
		string  windspeed_unit = "kmh";        // Other wind speed speed units: ms, mph and kn.
		string  precipitation_unit = "mm";     // Other precipitation amount units: inch
		string  timeformat = "iso8601";        // If format unixtime is selected, all time values are returned in UNIX epoch time in seconds.
		string  timezone = "GMT";              // https://en.wikipedia.org/wiki/List_of_tz_database_time_zones
		int     past_days = 0;                 // If past_days is set, yesterday or the day before yesterday data are also returned.
		int     forecast_days = 7;             // Per default, only 7 days are returned. Up to 16 days of forecast are possible.
		string  cell_selection = "land";       // Set a preference how grid-cells are selected.
		string  hourly = "^&hourly=";          // A list of weather variables which should be returned
		string  daily = "^&daily=";            // A list of daily weather variable aggregations which should be returned
		bool    success = false;               // Whether request succeeded
	};

	namespace options
	{
		using std::set;

		extern set<string> setting;
		extern set<string> hourly;
		extern set<string> daily;

		string category(string option);
	}
}