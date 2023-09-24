#include "weather-api.h"
#include <sstream>
#include <set>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <array>
#include <iomanip>
#include <limits>

namespace api {
	string Weather::url(string provider, string apikey)
	{
		std::ostringstream url;

		// Set floating point precision to ensure reliability.
		url << std::setprecision(std::numeric_limits<float>::max_digits10);

		url << "https://" << provider << "/v1/forecast?latitude=" << this->latitude << "^&longitude=" << this->longitude;

		if (apikey.size())
			url << "^&apikey=" << apikey;

		if (this->elevation)
			url << "^&elevation=" << this->elevation;

		if (this->current_weather)
			url << "^&current_weather=" << this->current_weather ? "true" : "false";

		if (this->temperature_unit != "celsius")
			url << "^&temperature_unit=" << this->temperature_unit;

		if (this->windspeed_unit != "kmh")
			url << "^&windspeed_unit=" << this->windspeed_unit;

		if (this->precipitation_unit != "mm")
			url << "^&precipitation_unit=" << this->precipitation_unit;

		if (this->timeformat != "iso8601")
			url << "^&timeformat=" << this->timeformat;

		if (this->timezone != "GMT")
			url << "^&timezone=" << this->timezone;

		if (this->past_days)
			url << "^&past_days=" << this->past_days;

		if (this->forecast_days != 7)
			url << "^&forecast_days=" << this->forecast_days;

		if (this->cell_selection != "land")
			url << "^&cell_selection=" << this->cell_selection;

		if (this->hourly != "^&hourly=")
			url << this->hourly;

		if (this->daily != "^&daily=")
			url << this->daily;

		return url.str();
	}

	json Weather::weather(string url)
	{
		string cmd = "curl " + url;
		std::array<char, 128> buffer{};
		string result;
		std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(cmd.c_str(), "r"), _pclose);

		if (!pipe)
			throw std::runtime_error("popen failed");

		while (fgets(buffer.data(), (int)buffer.size(), pipe.get()) != nullptr)
			result += buffer.data();

		return json::parse(result);
	};

	namespace options
	{
		set<string> setting{
			"latitude",
			"longitude",
			"elevation",
			"current_weather",
			"windspeed_unit",
			"precipitation_unit",
			"timeformat",
			"timezone",
			"past_days",
			"forecast_days",
			"cell_selection"
		};

		set<string> hourly{
			"temperature_2m",
			"relativehumidity_2m",
			"dewpoint_2m",
			"apparent_temperature",
			"pressure_msl",
			"surface_pressure",
			"cloudcover",
			"cloudcover_low",
			"cloudcover_mid",
			"cloudcover_high",
			"windspeed_10m",
			"windspeed_80m",
			"windspeed_120m",
			"windspeed_180m",
			"winddirection_10m",
			"winddirection_80m",
			"winddirection_120m",
			"winddirection_180m",
			"windgusts_10m",
			"shortwave_radiation",
			"direct_radiation",
			"direct_normal_irradiance",
			"diffuse_radiation",
			"vapor_pressure_deficit",
			"evapotranspiration",
			"cape",
			"et0_fao_evapotranspiration",
			"precipitation",
			"snowfall",
			"precipitation_probability",
			"showers",
			"weathercode",
			"snow_depth",
			"freezinglevel_height",
			"visibility",
			"soil_temperature_0cm",
			"soil_temperature_6cm",
			"soil_temperature_18cm",
			"soil_temperature_54cm",
			"soil_moisture_0_1cm",
			"soil_moisture_1_3cm",
			"soil_moisture_3_9cm",
			"soil_moisture_9_27cm",
			"soil_moisture_27_81cm",
			"is_day"
		};

		set<string> daily{
			"temperature_2m_max",
			"temperature_2m_min",
			"apparent_temperature_max",
			"apparent_temperature_min",
			"precipitation_sum",
			"rain_sum",
			"showers_sum",
			"snowfall_sum",
			"precipitation_hours",
			"precipitation_probability_max",
			"precipitation_probability_min",
			"precipitation_probability_mean",
			"weathercode",
			"sunrise",
			"sunset",
			"windspeed_10m_max",
			"windgusts_10m_max",
			"winddirection_10m_dominant",
			"shortwave_radiation_sum",
			"et0_fao_evapotranspiration",
			"uv_index_max",
			"uv_index_clear_sky_max"
		};

		string category(string name)
		{
			if (setting.contains(name))
				return "setting";
			else if (hourly.contains(name))
				return "hourly";
			else if (daily.contains(name))
				return "daily";
			else return "";
		}
	}
}