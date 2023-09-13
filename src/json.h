#pragma once

#include <optional>
#include "nlohmann/json.hpp"

//  To parse this JSON data, first install
//
//      json.hpp  https://github.com/nlohmann/json
//
//  Then include this file, and then do
//
//     Weather data = nlohmann::json::parse(jsonString);

#ifndef NLOHMANN_OPT_HELPER
#define NLOHMANN_OPT_HELPER
namespace nlohmann {
	template <typename T>
	struct adl_serializer<std::shared_ptr<T>> {
		static void to_json(json& j, const std::shared_ptr<T>& opt) {
			if (!opt)
				j = nullptr;
			else
				j = *opt;
		}

		static std::shared_ptr<T> from_json(const json& j) {
			if (j.is_null())
				return std::make_shared<T>();
			else
				return std::make_shared<T>(j.get<T>());
		}
	};
	template <typename T>
	struct adl_serializer<std::optional<T>> {
		static void to_json(json& j, const std::optional<T>& opt) {
			if (!opt) j = nullptr;
			else j = *opt;
		}

		static std::optional<T> from_json(const json& j) {
			if (j.is_null())
				return std::make_optional<T>();
			else
				return std::make_optional<T>(j.get<T>());
		}
	};
}
#endif

namespace json {
	using nlohmann::json;

#ifndef NLOHMANN_UNTYPED_json_HELPER
#define NLOHMANN_UNTYPED_json_HELPER
	inline json get_untyped(const json& j, const char* property) {
		if (j.find(property) != j.end())
			return j.at(property).get<json>();
		return json();
	}

	inline json get_untyped(const json& j, std::string property) {
		return get_untyped(j, property.data());
	}
#endif

#ifndef NLOHMANN_OPTIONAL_json_HELPER
#define NLOHMANN_OPTIONAL_json_HELPER
	template <typename T>
	inline std::shared_ptr<T> get_heap_optional(const json& j, const char* property) {
		auto it = j.find(property);
		if (it != j.end() && !it->is_null())
			return j.at(property).get<std::shared_ptr<T>>();
		return std::shared_ptr<T>();
	}

	template <typename T>
	inline std::shared_ptr<T> get_heap_optional(const json& j, std::string property) {
		return get_heap_optional<T>(j, property.data());
	}
	template <typename T>
	inline std::optional<T> get_stack_optional(const json& j, const char* property) {
		auto it = j.find(property);
		if (it != j.end() && !it->is_null())
			return j.at(property).get<std::optional<T>>();
		return std::optional<T>();
	}

	template <typename T>
	inline std::optional<T> get_stack_optional(const json& j, std::string property) {
		return get_stack_optional<T>(j, property.data());
	}
#endif

	struct DailyUnits {
		std::optional<std::string> time;
		std::optional<std::string> weathercode;
		std::optional<std::string> temperature_2m_max;
		std::optional<std::string> temperature_2m_min;
		std::optional<std::string> apparent_temperature_max;
		std::optional<std::string> apparent_temperature_min;
		std::optional<std::string> sunrise;
		std::optional<std::string> sunset;
		std::optional<std::string> uv_index_max;
		std::optional<std::string> uv_index_clear_sky_max;
		std::optional<std::string> precipitation_sum;
		std::optional<std::string> rain_sum;
		std::optional<std::string> showers_sum;
		std::optional<std::string> snowfall_sum;
		std::optional<std::string> precipitation_hours;
		std::optional<std::string> precipitation_probability_max;
		std::optional<std::string> windspeed_10m_max;
		std::optional<std::string> windgusts_10m_max;
		std::optional<std::string> winddirection_10m_dominant;
		std::optional<std::string> shortwave_radiation_sum;
		std::optional<std::string> et0_fao_evapotranspiration;
	};

	struct HourlyUnits {
		std::optional<std::string> time;
		std::optional<std::string> temperature_2m;
		std::optional<std::string> relativehumidity_2m;
		std::optional<std::string> dewpoint_2m;
		std::optional<std::string> apparent_temperature;
		std::optional<std::string> precipitation_probability;
		std::optional<std::string> precipitation;
		std::optional<std::string> rain;
		std::optional<std::string> showers;
		std::optional<std::string> snowfall;
		std::optional<std::string> snow_depth;
		std::optional<std::string> weathercode;
		std::optional<std::string> pressure_msl;
		std::optional<std::string> surface_pressure;
		std::optional<std::string> cloudcover;
		std::optional<std::string> cloudcover_low;
		std::optional<std::string> cloudcover_mid;
		std::optional<std::string> cloudcover_high;
		std::optional<std::string> visibility;
		std::optional<std::string> evapotranspiration;
		std::optional<std::string> et0_fao_evapotranspiration;
		std::optional<std::string> vapor_pressure_deficit;
		std::optional<std::string> windspeed_10m;
		std::optional<std::string> windspeed_80m;
		std::optional<std::string> windspeed_120m;
		std::optional<std::string> windspeed_180m;
		std::optional<std::string> winddirection_10m;
		std::optional<std::string> winddirection_80m;
		std::optional<std::string> winddirection_120m;
		std::optional<std::string> winddirection_180m;
		std::optional<std::string> windgusts_10m;
		std::optional<std::string> temperature_80m;
		std::optional<std::string> temperature_120m;
		std::optional<std::string> temperature_180m;
		std::optional<std::string> soil_temperature_0cm;
		std::optional<std::string> soil_temperature_6cm;
		std::optional<std::string> soil_temperature_18cm;
		std::optional<std::string> soil_temperature_54cm;
		std::optional<std::string> soil_moisture_0_1cm;
		std::optional<std::string> soil_moisture_1_3cm;
		std::optional<std::string> soil_moisture_3_9cm;
		std::optional<std::string> soil_moisture_9_27cm;
		std::optional<std::string> soil_moisture_27_81cm;
	};

	struct Hourly {
		std::optional<std::vector<double>> time;
		std::optional<std::vector<double>> temperature_2m;
		std::optional<std::vector<double>> relativehumidity_2m;
		std::optional<std::vector<double>> dewpoint_2m;
		std::optional<std::vector<double>> apparent_temperature;
		std::optional<std::vector<double>> precipitation_probability;
		std::optional<std::vector<double>> precipitation;
		std::optional<std::vector<double>> rain;
		std::optional<std::vector<double>> showers;
		std::optional<std::vector<double>> snowfall;
		std::optional<std::vector<double>> snow_depth;
		std::optional<std::vector<double>> weathercode;
		std::optional<std::vector<double>> pressure_msl;
		std::optional<std::vector<double>> surface_pressure;
		std::optional<std::vector<double>> cloudcover;
		std::optional<std::vector<double>> cloudcover_low;
		std::optional<std::vector<double>> cloudcover_mid;
		std::optional<std::vector<double>> cloudcover_high;
		std::optional<std::vector<double>> visibility;
		std::optional<std::vector<double>> evapotranspiration;
		std::optional<std::vector<double>> et0_fao_evapotranspiration;
		std::optional<std::vector<double>> vapor_pressure_deficit;
		std::optional<std::vector<double>> windspeed_10m;
		std::optional<std::vector<double>> windspeed_80m;
		std::optional<std::vector<double>> windspeed_120m;
		std::optional<std::vector<double>> windspeed_180m;
		std::optional<std::vector<double>> winddirection_10m;
		std::optional<std::vector<double>> winddirection_80m;
		std::optional<std::vector<double>> winddirection_120m;
		std::optional<std::vector<double>> winddirection_180m;
		std::optional<std::vector<double>> windgusts_10m;
		std::optional<std::vector<double>> temperature_80m;
		std::optional<std::vector<double>> temperature_120m;
		std::optional<std::vector<double>> temperature_180m;
		std::optional<std::vector<double>> soil_temperature_0cm;
		std::optional<std::vector<double>> soil_temperature_6cm;
		std::optional<std::vector<double>> soil_temperature_18cm;
		std::optional<std::vector<double>> soil_temperature_54cm;
		std::optional<std::vector<double>> soil_moisture_0_1cm;
		std::optional<std::vector<double>> soil_moisture_1_3cm;
		std::optional<std::vector<double>> soil_moisture_3_9cm;
		std::optional<std::vector<double>> soil_moisture_9_27cm;
		std::optional<std::vector<double>> soil_moisture_27_81cm;
	};

	struct Daily {
		std::optional<std::vector<double>> time;
		std::optional<std::vector<double>> weathercode;
		std::optional<std::vector<double>> temperature_2m_max;
		std::optional<std::vector<double>> temperature_2m_min;
		std::optional<std::vector<double>> apparent_temperature_max;
		std::optional<std::vector<double>> apparent_temperature_min;
		std::optional<std::vector<double>> sunrise;
		std::optional<std::vector<double>> sunset;
		std::optional<std::vector<double>> uv_index_max;
		std::optional<std::vector<double>> uv_index_clear_sky_max;
		std::optional<std::vector<double>> precipitation_sum;
		std::optional<std::vector<double>> rain_sum;
		std::optional<std::vector<double>> showers_sum;
		std::optional<std::vector<double>> snowfall_sum;
		std::optional<std::vector<double>> precipitation_hours;
		std::optional<std::vector<double>> precipitation_probability_max;
		std::optional<std::vector<double>> windspeed_10m_max;
		std::optional<std::vector<double>> windgusts_10m_max;
		std::optional<std::vector<double>> winddirection_10m_dominant;
		std::optional<std::vector<double>> shortwave_radiation_sum;
		std::optional<std::vector<double>> et0_fao_evapotranspiration;
	};

	struct Weather {
		std::optional<bool> error;
		std::optional<std::string> reason;
		std::optional<double> latitude;
		std::optional<double> longitude;
		std::optional<double> generationtime_ms;
		std::optional<int64_t> utc_offset_seconds;
		std::optional<std::string> timezone;
		std::optional<std::string> timezone_abbreviation;
		std::optional<int64_t> elevation;
		std::optional<HourlyUnits> hourly_units;
		std::optional<Hourly> hourly;
		std::optional<DailyUnits> daily_units;
		std::optional<Daily> daily;
	};
}

namespace json {
	void from_json(const json& j, DailyUnits& x);
	void to_json(json& j, const DailyUnits& x);

	void from_json(const json& j, HourlyUnits& x);
	void to_json(json& j, const HourlyUnits& x);

	void from_json(const json& j, Weather& x);
	void to_json(json& j, const Weather& x);

	inline void from_json(const json& j, DailyUnits& x) {
		x.time = get_stack_optional<std::string>(j, "time");
		x.weathercode = get_stack_optional<std::string>(j, "weathercode");
		x.temperature_2m_max = get_stack_optional<std::string>(j, "temperature_2m_max");
		x.temperature_2m_min = get_stack_optional<std::string>(j, "temperature_2m_min");
		x.apparent_temperature_max = get_stack_optional<std::string>(j, "apparent_temperature_max");
		x.apparent_temperature_min = get_stack_optional<std::string>(j, "apparent_temperature_min");
		x.sunrise = get_stack_optional<std::string>(j, "sunrise");
		x.sunset = get_stack_optional<std::string>(j, "sunset");
		x.uv_index_max = get_stack_optional<std::string>(j, "uv_index_max");
		x.uv_index_clear_sky_max = get_stack_optional<std::string>(j, "uv_index_clear_sky_max");
		x.precipitation_sum = get_stack_optional<std::string>(j, "precipitation_sum");
		x.rain_sum = get_stack_optional<std::string>(j, "rain_sum");
		x.showers_sum = get_stack_optional<std::string>(j, "showers_sum");
		x.snowfall_sum = get_stack_optional<std::string>(j, "snowfall_sum");
		x.precipitation_hours = get_stack_optional<std::string>(j, "precipitation_hours");
		x.precipitation_probability_max = get_stack_optional<std::string>(j, "precipitation_probability_max");
		x.windspeed_10m_max = get_stack_optional<std::string>(j, "windspeed_10m_max");
		x.windgusts_10m_max = get_stack_optional<std::string>(j, "windgusts_10m_max");
		x.winddirection_10m_dominant = get_stack_optional<std::string>(j, "winddirection_10m_dominant");
		x.shortwave_radiation_sum = get_stack_optional<std::string>(j, "shortwave_radiation_sum");
		x.et0_fao_evapotranspiration = get_stack_optional<std::string>(j, "et0_fao_evapotranspiration");
	}

	inline void to_json(json& j, const DailyUnits& x) {
		j = json::object();
		j["time"] = x.time;
		j["weathercode"] = x.weathercode;
		j["temperature_2m_max"] = x.temperature_2m_max;
		j["temperature_2m_min"] = x.temperature_2m_min;
		j["apparent_temperature_max"] = x.apparent_temperature_max;
		j["apparent_temperature_min"] = x.apparent_temperature_min;
		j["sunrise"] = x.sunrise;
		j["sunset"] = x.sunset;
		j["uv_index_max"] = x.uv_index_max;
		j["uv_index_clear_sky_max"] = x.uv_index_clear_sky_max;
		j["precipitation_sum"] = x.precipitation_sum;
		j["rain_sum"] = x.rain_sum;
		j["showers_sum"] = x.showers_sum;
		j["snowfall_sum"] = x.snowfall_sum;
		j["precipitation_hours"] = x.precipitation_hours;
		j["precipitation_probability_max"] = x.precipitation_probability_max;
		j["windspeed_10m_max"] = x.windspeed_10m_max;
		j["windgusts_10m_max"] = x.windgusts_10m_max;
		j["winddirection_10m_dominant"] = x.winddirection_10m_dominant;
		j["shortwave_radiation_sum"] = x.shortwave_radiation_sum;
		j["et0_fao_evapotranspiration"] = x.et0_fao_evapotranspiration;
	}

	inline void from_json(const json& j, HourlyUnits& x) {
		x.time = get_stack_optional<std::string>(j, "time");
		x.temperature_2m = get_stack_optional<std::string>(j, "temperature_2m");
		x.relativehumidity_2m = get_stack_optional<std::string>(j, "relativehumidity_2m");
		x.dewpoint_2m = get_stack_optional<std::string>(j, "dewpoint_2m");
		x.apparent_temperature = get_stack_optional<std::string>(j, "apparent_temperature");
		x.precipitation_probability = get_stack_optional<std::string>(j, "precipitation_probability");
		x.precipitation = get_stack_optional<std::string>(j, "precipitation");
		x.rain = get_stack_optional<std::string>(j, "rain");
		x.showers = get_stack_optional<std::string>(j, "showers");
		x.snowfall = get_stack_optional<std::string>(j, "snowfall");
		x.snow_depth = get_stack_optional<std::string>(j, "snow_depth");
		x.weathercode = get_stack_optional<std::string>(j, "weathercode");
		x.pressure_msl = get_stack_optional<std::string>(j, "pressure_msl");
		x.surface_pressure = get_stack_optional<std::string>(j, "surface_pressure");
		x.cloudcover = get_stack_optional<std::string>(j, "cloudcover");
		x.cloudcover_low = get_stack_optional<std::string>(j, "cloudcover_low");
		x.cloudcover_mid = get_stack_optional<std::string>(j, "cloudcover_mid");
		x.cloudcover_high = get_stack_optional<std::string>(j, "cloudcover_high");
		x.visibility = get_stack_optional<std::string>(j, "visibility");
		x.evapotranspiration = get_stack_optional<std::string>(j, "evapotranspiration");
		x.et0_fao_evapotranspiration = get_stack_optional<std::string>(j, "et0_fao_evapotranspiration");
		x.vapor_pressure_deficit = get_stack_optional<std::string>(j, "vapor_pressure_deficit");
		x.windspeed_10m = get_stack_optional<std::string>(j, "windspeed_10m");
		x.windspeed_80m = get_stack_optional<std::string>(j, "windspeed_80m");
		x.windspeed_120m = get_stack_optional<std::string>(j, "windspeed_120m");
		x.windspeed_180m = get_stack_optional<std::string>(j, "windspeed_180m");
		x.winddirection_10m = get_stack_optional<std::string>(j, "winddirection_10m");
		x.winddirection_80m = get_stack_optional<std::string>(j, "winddirection_80m");
		x.winddirection_120m = get_stack_optional<std::string>(j, "winddirection_120m");
		x.winddirection_180m = get_stack_optional<std::string>(j, "winddirection_180m");
		x.windgusts_10m = get_stack_optional<std::string>(j, "windgusts_10m");
		x.temperature_80m = get_stack_optional<std::string>(j, "temperature_80m");
		x.temperature_120m = get_stack_optional<std::string>(j, "temperature_120m");
		x.temperature_180m = get_stack_optional<std::string>(j, "temperature_180m");
		x.soil_temperature_0cm = get_stack_optional<std::string>(j, "soil_temperature_0cm");
		x.soil_temperature_6cm = get_stack_optional<std::string>(j, "soil_temperature_6cm");
		x.soil_temperature_18cm = get_stack_optional<std::string>(j, "soil_temperature_18cm");
		x.soil_temperature_54cm = get_stack_optional<std::string>(j, "soil_temperature_54cm");
		x.soil_moisture_0_1cm = get_stack_optional<std::string>(j, "soil_moisture_0_1cm");
		x.soil_moisture_1_3cm = get_stack_optional<std::string>(j, "soil_moisture_1_3cm");
		x.soil_moisture_3_9cm = get_stack_optional<std::string>(j, "soil_moisture_3_9cm");
		x.soil_moisture_9_27cm = get_stack_optional<std::string>(j, "soil_moisture_9_27cm");
		x.soil_moisture_27_81cm = get_stack_optional<std::string>(j, "soil_moisture_27_81cm");
	}

	inline void to_json(json& j, const HourlyUnits& x) {
		j = json::object();
		j["time"] = x.time;
		j["temperature_2m"] = x.temperature_2m;
		j["relativehumidity_2m"] = x.relativehumidity_2m;
		j["dewpoint_2m"] = x.dewpoint_2m;
		j["apparent_temperature"] = x.apparent_temperature;
		j["precipitation_probability"] = x.precipitation_probability;
		j["precipitation"] = x.precipitation;
		j["rain"] = x.rain;
		j["showers"] = x.showers;
		j["snowfall"] = x.snowfall;
		j["snow_depth"] = x.snow_depth;
		j["weathercode"] = x.weathercode;
		j["pressure_msl"] = x.pressure_msl;
		j["surface_pressure"] = x.surface_pressure;
		j["cloudcover"] = x.cloudcover;
		j["cloudcover_low"] = x.cloudcover_low;
		j["cloudcover_mid"] = x.cloudcover_mid;
		j["cloudcover_high"] = x.cloudcover_high;
		j["visibility"] = x.visibility;
		j["evapotranspiration"] = x.evapotranspiration;
		j["et0_fao_evapotranspiration"] = x.et0_fao_evapotranspiration;
		j["vapor_pressure_deficit"] = x.vapor_pressure_deficit;
		j["windspeed_10m"] = x.windspeed_10m;
		j["windspeed_80m"] = x.windspeed_80m;
		j["windspeed_120m"] = x.windspeed_120m;
		j["windspeed_180m"] = x.windspeed_180m;
		j["winddirection_10m"] = x.winddirection_10m;
		j["winddirection_80m"] = x.winddirection_80m;
		j["winddirection_120m"] = x.winddirection_120m;
		j["winddirection_180m"] = x.winddirection_180m;
		j["windgusts_10m"] = x.windgusts_10m;
		j["temperature_80m"] = x.temperature_80m;
		j["temperature_120m"] = x.temperature_120m;
		j["temperature_180m"] = x.temperature_180m;
		j["soil_temperature_0cm"] = x.soil_temperature_0cm;
		j["soil_temperature_6cm"] = x.soil_temperature_6cm;
		j["soil_temperature_18cm"] = x.soil_temperature_18cm;
		j["soil_temperature_54cm"] = x.soil_temperature_54cm;
		j["soil_moisture_0_1cm"] = x.soil_moisture_0_1cm;
		j["soil_moisture_1_3cm"] = x.soil_moisture_1_3cm;
		j["soil_moisture_3_9cm"] = x.soil_moisture_3_9cm;
		j["soil_moisture_9_27cm"] = x.soil_moisture_9_27cm;
		j["soil_moisture_27_81cm"] = x.soil_moisture_27_81cm;
	}

	inline void from_json(const json& j, Hourly& x) {
		x.time = get_stack_optional<std::vector<double>>(j, "time");
		x.temperature_2m = get_stack_optional<std::vector<double>>(j, "temperature_2m");
		x.relativehumidity_2m = get_stack_optional<std::vector<double>>(j, "relativehumidity_2m");
		x.dewpoint_2m = get_stack_optional<std::vector<double>>(j, "dewpoint_2m");
		x.apparent_temperature = get_stack_optional<std::vector<double>>(j, "apparent_temperature");
		x.precipitation_probability = get_stack_optional<std::vector<double>>(j, "precipitation_probability");
		x.precipitation = get_stack_optional<std::vector<double>>(j, "precipitation");
		x.rain = get_stack_optional<std::vector<double>>(j, "rain");
		x.showers = get_stack_optional<std::vector<double>>(j, "showers");
		x.snowfall = get_stack_optional<std::vector<double>>(j, "snowfall");
		x.snow_depth = get_stack_optional<std::vector<double>>(j, "snow_depth");
		x.weathercode = get_stack_optional<std::vector<double>>(j, "weathercode");
		x.pressure_msl = get_stack_optional<std::vector<double>>(j, "pressure_msl");
		x.surface_pressure = get_stack_optional<std::vector<double>>(j, "surface_pressure");
		x.cloudcover = get_stack_optional<std::vector<double>>(j, "cloudcover");
		x.cloudcover_low = get_stack_optional<std::vector<double>>(j, "cloudcover_low");
		x.cloudcover_mid = get_stack_optional<std::vector<double>>(j, "cloudcover_mid");
		x.cloudcover_high = get_stack_optional<std::vector<double>>(j, "cloudcover_high");
		x.visibility = get_stack_optional<std::vector<double>>(j, "visibility");
		x.evapotranspiration = get_stack_optional<std::vector<double>>(j, "evapotranspiration");
		x.et0_fao_evapotranspiration = get_stack_optional<std::vector<double>>(j, "et0_fao_evapotranspiration");
		x.vapor_pressure_deficit = get_stack_optional<std::vector<double>>(j, "vapor_pressure_deficit");
		x.windspeed_10m = get_stack_optional<std::vector<double>>(j, "windspeed_10m");
		x.windspeed_80m = get_stack_optional<std::vector<double>>(j, "windspeed_80m");
		x.windspeed_120m = get_stack_optional<std::vector<double>>(j, "windspeed_120m");
		x.windspeed_180m = get_stack_optional<std::vector<double>>(j, "windspeed_180m");
		x.winddirection_10m = get_stack_optional<std::vector<double>>(j, "winddirection_10m");
		x.winddirection_80m = get_stack_optional<std::vector<double>>(j, "winddirection_80m");
		x.winddirection_120m = get_stack_optional<std::vector<double>>(j, "winddirection_120m");
		x.winddirection_180m = get_stack_optional<std::vector<double>>(j, "winddirection_180m");
		x.windgusts_10m = get_stack_optional<std::vector<double>>(j, "windgusts_10m");
		x.temperature_80m = get_stack_optional<std::vector<double>>(j, "temperature_80m");
		x.temperature_120m = get_stack_optional<std::vector<double>>(j, "temperature_120m");
		x.temperature_180m = get_stack_optional<std::vector<double>>(j, "temperature_180m");
		x.soil_temperature_0cm = get_stack_optional<std::vector<double>>(j, "soil_temperature_0cm");
		x.soil_temperature_6cm = get_stack_optional<std::vector<double>>(j, "soil_temperature_6cm");
		x.soil_temperature_18cm = get_stack_optional<std::vector<double>>(j, "soil_temperature_18cm");
		x.soil_temperature_54cm = get_stack_optional<std::vector<double>>(j, "soil_temperature_54cm");
		x.soil_moisture_0_1cm = get_stack_optional<std::vector<double>>(j, "soil_moisture_0_1cm");
		x.soil_moisture_1_3cm = get_stack_optional<std::vector<double>>(j, "soil_moisture_1_3cm");
		x.soil_moisture_3_9cm = get_stack_optional<std::vector<double>>(j, "soil_moisture_3_9cm");
		x.soil_moisture_9_27cm = get_stack_optional<std::vector<double>>(j, "soil_moisture_9_27cm");
		x.soil_moisture_27_81cm = get_stack_optional<std::vector<double>>(j, "soil_moisture_27_81cm");
	}

	inline void to_json(json& j, const Hourly& x) {
		j = json::object();
		j["time"] = x.time;
		j["temperature_2m"] = x.temperature_2m;
		j["relativehumidity_2m"] = x.relativehumidity_2m;
		j["dewpoint_2m"] = x.dewpoint_2m;
		j["apparent_temperature"] = x.apparent_temperature;
		j["precipitation_probability"] = x.precipitation_probability;
		j["precipitation"] = x.precipitation;
		j["rain"] = x.rain;
		j["showers"] = x.showers;
		j["snowfall"] = x.snowfall;
		j["snow_depth"] = x.snow_depth;
		j["weathercode"] = x.weathercode;
		j["pressure_msl"] = x.pressure_msl;
		j["surface_pressure"] = x.surface_pressure;
		j["cloudcover"] = x.cloudcover;
		j["cloudcover_low"] = x.cloudcover_low;
		j["cloudcover_mid"] = x.cloudcover_mid;
		j["cloudcover_high"] = x.cloudcover_high;
		j["visibility"] = x.visibility;
		j["evapotranspiration"] = x.evapotranspiration;
		j["et0_fao_evapotranspiration"] = x.et0_fao_evapotranspiration;
		j["vapor_pressure_deficit"] = x.vapor_pressure_deficit;
		j["windspeed_10m"] = x.windspeed_10m;
		j["windspeed_80m"] = x.windspeed_80m;
		j["windspeed_120m"] = x.windspeed_120m;
		j["windspeed_180m"] = x.windspeed_180m;
		j["winddirection_10m"] = x.winddirection_10m;
		j["winddirection_80m"] = x.winddirection_80m;
		j["winddirection_120m"] = x.winddirection_120m;
		j["winddirection_180m"] = x.winddirection_180m;
		j["windgusts_10m"] = x.windgusts_10m;
		j["temperature_80m"] = x.temperature_80m;
		j["temperature_120m"] = x.temperature_120m;
		j["temperature_180m"] = x.temperature_180m;
		j["soil_temperature_0cm"] = x.soil_temperature_0cm;
		j["soil_temperature_6cm"] = x.soil_temperature_6cm;
		j["soil_temperature_18cm"] = x.soil_temperature_18cm;
		j["soil_temperature_54cm"] = x.soil_temperature_54cm;
		j["soil_moisture_0_1cm"] = x.soil_moisture_0_1cm;
		j["soil_moisture_1_3cm"] = x.soil_moisture_1_3cm;
		j["soil_moisture_3_9cm"] = x.soil_moisture_3_9cm;
		j["soil_moisture_9_27cm"] = x.soil_moisture_9_27cm;
		j["soil_moisture_27_81cm"] = x.soil_moisture_27_81cm;
	}

	inline void from_json(const json& j, Daily& x) {
		x.time = get_stack_optional<std::vector<double>>(j, "time");
		x.weathercode = get_stack_optional<std::vector<double>>(j, "weathercode");
		x.temperature_2m_max = get_stack_optional<std::vector<double>>(j, "temperature_2m_max");
		x.temperature_2m_min = get_stack_optional<std::vector<double>>(j, "temperature_2m_min");
		x.apparent_temperature_max = get_stack_optional<std::vector<double>>(j, "apparent_temperature_max");
		x.apparent_temperature_min = get_stack_optional<std::vector<double>>(j, "apparent_temperature_min");
		x.sunrise = get_stack_optional<std::vector<double>>(j, "sunrise");
		x.sunset = get_stack_optional<std::vector<double>>(j, "sunset");
		x.uv_index_max = get_stack_optional<std::vector<double>>(j, "uv_index_max");
		x.uv_index_clear_sky_max = get_stack_optional<std::vector<double>>(j, "uv_index_clear_sky_max");
		x.precipitation_sum = get_stack_optional<std::vector<double>>(j, "precipitation_sum");
		x.rain_sum = get_stack_optional<std::vector<double>>(j, "rain_sum");
		x.showers_sum = get_stack_optional<std::vector<double>>(j, "showers_sum");
		x.snowfall_sum = get_stack_optional<std::vector<double>>(j, "snowfall_sum");
		x.precipitation_hours = get_stack_optional<std::vector<double>>(j, "precipitation_hours");
		x.precipitation_probability_max = get_stack_optional<std::vector<double>>(j, "precipitation_probability_max");
		x.windspeed_10m_max = get_stack_optional<std::vector<double>>(j, "windspeed_10m_max");
		x.windgusts_10m_max = get_stack_optional<std::vector<double>>(j, "windgusts_10m_max");
		x.winddirection_10m_dominant = get_stack_optional<std::vector<double>>(j, "winddirection_10m_dominant");
		x.shortwave_radiation_sum = get_stack_optional<std::vector<double>>(j, "shortwave_radiation_sum");
		x.et0_fao_evapotranspiration = get_stack_optional<std::vector<double>>(j, "et0_fao_evapotranspiration");
	}

	inline void to_json(json& j, const Daily& x) {
		j = json::object();
		j["time"] = x.time;
		j["weathercode"] = x.weathercode;
		j["temperature_2m_max"] = x.temperature_2m_max;
		j["temperature_2m_min"] = x.temperature_2m_min;
		j["apparent_temperature_max"] = x.apparent_temperature_max;
		j["apparent_temperature_min"] = x.apparent_temperature_min;
		j["sunrise"] = x.sunrise;
		j["sunset"] = x.sunset;
		j["uv_index_max"] = x.uv_index_max;
		j["uv_index_clear_sky_max"] = x.uv_index_clear_sky_max;
		j["precipitation_sum"] = x.precipitation_sum;
		j["rain_sum"] = x.rain_sum;
		j["showers_sum"] = x.showers_sum;
		j["snowfall_sum"] = x.snowfall_sum;
		j["precipitation_hours"] = x.precipitation_hours;
		j["precipitation_probability_max"] = x.precipitation_probability_max;
		j["windspeed_10m_max"] = x.windspeed_10m_max;
		j["windgusts_10m_max"] = x.windgusts_10m_max;
		j["winddirection_10m_dominant"] = x.winddirection_10m_dominant;
		j["shortwave_radiation_sum"] = x.shortwave_radiation_sum;
		j["et0_fao_evapotranspiration"] = x.et0_fao_evapotranspiration;
	}

	inline void from_json(const json& j, Weather& x) {
		x.error = get_stack_optional<bool>(j, "error");
		x.reason = get_stack_optional<std::string>(j, "reason");
		x.latitude = get_stack_optional<double>(j, "latitude");
		x.longitude = get_stack_optional<double>(j, "longitude");
		x.generationtime_ms = get_stack_optional<double>(j, "generationtime_ms");
		x.utc_offset_seconds = get_stack_optional<int64_t>(j, "utc_offset_seconds");
		x.timezone = get_stack_optional<std::string>(j, "timezone");
		x.timezone_abbreviation = get_stack_optional<std::string>(j, "timezone_abbreviation");
		x.elevation = get_stack_optional<int64_t>(j, "elevation");
		x.hourly_units = get_stack_optional<HourlyUnits>(j, "hourly_units");
		x.hourly = get_stack_optional<Hourly>(j, "hourly");
		x.daily_units = get_stack_optional<DailyUnits>(j, "daily_units");
		x.daily = get_stack_optional<Daily>(j, "daily");
	}

	inline void to_json(json& j, const Weather& x) {
		j = json::object();
		j["error"] = x.error;
		j["reason"] = x.reason;
		j["latitude"] = x.latitude;
		j["longitude"] = x.longitude;
		j["generationtime_ms"] = x.generationtime_ms;
		j["utc_offset_seconds"] = x.utc_offset_seconds;
		j["timezone"] = x.timezone;
		j["timezone_abbreviation"] = x.timezone_abbreviation;
		j["elevation"] = x.elevation;
		j["hourly_units"] = x.hourly_units;
		j["hourly"] = x.hourly;
		j["daily_units"] = x.daily_units;
		j["daily"] = x.daily;
	}
}