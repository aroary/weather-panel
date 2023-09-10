#include "weather-api.h"

std::string api::Weather::url(std::string provider, std::string apikey)
{
	std::ostringstream url;
	url << "https://" << provider << "/v1/forecast?latitude=" << this->latitude << "&longitude=" << this->longitude;
	
	if (apikey.size())
		url << "&apikey=" << apikey;

	if (this->elevation)
		url << "&elevation=" << this->elevation;

	if (this->current_weather)
		url << "&current_weather=" << this->current_weather;

	if (this->temperature_unit.size())
		url << "&temperature_unit=" << this->temperature_unit;

	if (this->windspeed_unit.size())
		url << "&windspeed_unit=" << this->windspeed_unit;

	if (this->precipitation_unit.size())
		url << "&precipitation_unit=" << this->precipitation_unit;

	if (this->timeformat.size())
		url << "&timeformat=" << this->timeformat;

	if (this->timezone.size())
		url << "&timezone=" << this->timezone;

	if (this->past_days)
		url << "&past_days=" << this->past_days;

	if (this->forecast_days != 7)
		url << "&forecast_days=" << this->forecast_days;

	if (this->cell_selection.size())
		url << "&cell_selection=" << this->cell_selection;

	return url.str();
}

json::Weather api::Weather::weather(std::string url)
{
	const char* cmd[2048];

	strcpy_s((char*)cmd, sizeof cmd, "curl ");
	strcat_s((char*)cmd, sizeof cmd, url.c_str());

	std::array<char, 128> buffer{};
	std::string result;
	std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(*cmd, "r"), _pclose);
	
	if (!pipe) 
		throw std::runtime_error("popen failed");
	
	while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
		result += buffer.data();

	return nlohmann::json::parse("{}");
};

std::string api::Hourly::url()
{
	std::string url = "&hourly=";

	if (this->temperature)
		url += "temperature_2m,";

	if (this->relativehumidity)
		url += "relativehumidity_2m,";

	if (this->dewpoint)
		url += "dewpoint_2m,";

	if (this->apparenttemperature)
		url += "apparent_temperature,";

	if (this->precipitationprobability)
		url += "precipitation_probability,";

	if (this->precipitation)
		url += "precipitation,";
	
	if (this->rain)
		url += "rain,";

	if (this->showers)
		url += "showers,";
		
	if (this->snowfall)
		url += "snowfall,";
	
	if (this->snowdepth)
		url += "snow_depth";
	
	if (this->weathercode)
		url += "weathercode,";

	 if (this->pressuremsl)
		url += "pressure_msl,";
	
	if (this->surfacepressure)
		url += "surface_pressure,";

	if (this->cloudcover)
		url += "cloudcover,";

	if (this->cloudcoverlow)
		url += "cloudcover_low,";

	if (this->cloudcovermid)
		url += "cloudcover_mid,";
	
	if (this->cloudcoverhigh)
		url += "cloudcover_high";

	if (this->visibility)
		url += "visibility,";

	if (this->evapotranspiration)
		url += "evapotranspiration";

	if (this->et0faoevapotranspiration)
		url += "et0_fao_evapotranspiration,";

	if (this->vaporpressuredeficit)
		url += "vapor_pressure_deficit,";

	if (this->windspeed)
		url += "windspeed_10m,windspeed_80m,windspeed_120m,windspeed_180m,";

	if (this->winddirection)
		url += "winddirection_10m,winddirection_80m,winddirection_120m,winddirection_180m";

	if (this->windgusts)
		url += "windgusts_10m";

	return url;
}

std::string api::Daily::url()
{
	std::string url = "&daily=";

	if (this->temperaturemax)
		url += "temperature_2m_max,";

	if (this->temperaturemin)
		url += "temperature_2m_min,";

	if (this->apparenttemperaturemax)
		url += "apparent_temperature_max,";

	if (this->apparenttemperaturemin)
		url += "apparent_temperature_min,";

	if (this->precipitationsum)
		url += "precipitation_sum,";

	if (this->rainsum)
		url += "rain_sum,";

	if (this->showerssum)
		url += "showers_sum,";

	if (this->snowfallsum)
		url += "snowfall_sum,";

	if (this->precipitationhours)
		url += "precipitation_hours,";

	if (this->precipitationprobabilitymax)
		url += "precipitation_probability_max,";

	if (this->precipitationprobabilitymin)
		url += "precipitation_probability_min,";

	if (this->precipitationprobabilitymean)
		url += "precipitation_probability_mean,";

	if (this->weathercode)
		url += "weathercode,";

	if (this->sunrise)
		url += "sunrise,";

	if (this->sunset)
		url += "sunset,";

	if (this->windspeedmax)
		url += "windspeed_10m_max,";

	if (this->windgustsmax)
		url += "windgusts_10m_max,";

	if (this->winddirectiondominant)
		url += "winddirection_10m_dominant,";

	if (this->shortwaveradiationsum)
		url += "shortwave_radiation_sum,";

	if (this->et0faoevapotranspiration)
		url += "et0_fao_evapotranspiration,";

	if (this->uvindexmax)
		url += "uv_index_max,";

	if (this->uvindexclearskymax)
		url += "uv_index_clear_sky_max,";

	return url;
}
