#include "weather-api.h"

class Weather;
class Hourly;
class Daily;

class Weather
{
public:
	string url(string provider, string apikey);

	float    latitude;                      // Geographical WGS84 coordinate of the location
	float    longitude;                     // Geographical WGS84 coordinate of the location
	float    elevation;                     // The elevation used for statistical downscaling
	bool     current_weather = false;       // Include current weather conditions in the JSON output.
	string   temperature_unit = "celsius";  // If fahrenheit is set, all temperature values are converted to Fahrenheit.
	string   windspeed_unit = "kmh";        // Other wind speed speed units: ms, mph and kn.
	string   precipitation_unit = "mm";     // Other precipitation amount units: inch
	string   timeformat = "iso8601";        // If format unixtime is selected, all time values are returned in UNIX epoch time in seconds.
	string   timezone = "GMT";              // https://en.wikipedia.org/wiki/List_of_tz_database_time_zones
	int      past_days = 0;                 // If past_days is set, yesterday or the day before yesterday data are also returned.
	int      forecast_days = 7;             // Per default, only 7 days are returned. Up to 16 days of forecast are possible.
	string   cell_selection = "land";       // Set a preference how grid-cells are selected.
	Hourly*  hourly;                        // A list of weather variables which should be returned
	Daily*   daily;                         // A list of daily weather variable aggregations which should be returned
};

string Weather::url(string provider, string apikey)
{
	ostringstream url;
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

class Hourly
{
public:
	string url();

	bool  temperature = false;               // Air temperature at 2 meters above ground
	bool  relativehumidity = false;          // Relative humidity at 2 meters above ground
	bool  dewpoint = false;                  // Dew point temperature at 2 meters above ground
	bool  apparenttemperature = false;       // Apparent temperature is the perceived feels-like temperature combining wind chill factor, relative humidity and solar radiation
	bool  precipitationprobability = false;  // Probability of precipitation with more than 0.1 mm of the preceding hour.
	bool  precipitation = false;             // Total precipitation (rain, showers, snow) sum of the preceding hour.
	bool  rain = false;                      // Rain from large scale weather systems of the preceding hour in millimeter.
	bool  showers = false;                   // Showers from convective precipitation in millimeters from the preceding hour.
	bool  snowfall = false;                  // Snowfall amount of the preceding hour in centimeters.
	bool  snowdepth = false;                 // Snow depth on the ground.
	bool  weathercode = false;               // Weather condition as a numeric code. Follow WMO weather interpretation codes.
	bool  pressuremsl = false;               // Atmospheric air pressure reduced to mean sea level.
	bool  surfacepressure = false;           // Atmospheric air pressure at surface.
	bool  cloudcover = false;                // Total cloud cover as an area fraction.
	bool  cloudcoverlow = false;             // Low level clouds and fog up to 3 km altitude.
	bool  cloudcovermid = false;             // Mid level clouds from 3 to 8 km altitude.
	bool  cloudcoverhigh = false;            // High level clouds from 8 km altitude.
	bool  visibility = false;                // Viewing distance in meters. Influenced by low clouds, humidity and aerosols.
	bool  evapotranspiration = false;        // Evapotranspration from land surface and plants that weather models assumes for this location.
	bool  et0faoevapotranspiration = false;  // ET0 Reference Evapotranspiration of a well watered grass field.
	bool  vaporpressuredeficit = false;      // Vapor Pressure Deficit (VPD) in kilopascal (kPa). For high VPD (>1.6), water transpiration of plants increases. 
	bool  windspeed = false;                 // Wind speed at 10, 80, 120 or 180 meters above ground. Wind speed on 10 meters is the standard level.
	bool  winddirection = false;             // Wind direction at 10, 80, 120 or 180 meters above ground.
	bool  windgusts = false;                 // Gusts at 10 meters above ground as a maximum of the preceding hour.
};

string Hourly::url()
{
	string url = "&hourly=";

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

class Daily
{
public:
	string url();

	bool  temperaturemax = false;                // Maximum daily air temperature at 2 meters above ground.
	bool  temperaturemin = false;                // Minimum daily air temperature at 2 meters above ground.
	bool  apparenttemperaturemax = false;        // Maximum daily apparent temperature
	bool  apparenttemperaturemin = false;        // Minimum daily apparent temperature
	bool  precipitationsum = false;              // Sum of daily precipitation (including rain, showers and snowfall).
	bool  rainsum = false;                       // Sum of daily rain.
	bool  showerssum = false;                    // Sum of daily showers.
	bool  snowfallsum = false;                   // Sum of daily snowfall.
	bool  precipitationhours = false;            // The number of hours with rain.
	bool  precipitationprobabilitymax = false;   // Probability of precipitation.
	bool  precipitationprobabilitymin = false;   // Probability of precipitation.
	bool  precipitationprobabilitymean = false;  // Probability of precipitation.
	bool  weathercode = false;                   // The most severe weather condition on a given day.
	bool  sunrise = false;                       // Sun rise times.
	bool  sunset = false;                        // Sun  set times.
	bool  windspeedmax = false;                  // Maximum wind speed on a day.
	bool  windgustsmax = false;                  // Maximum wind gusts on a day.
	bool  winddirectiondominant = false;         // Dominant wind direction.
	bool  shortwaveradiationsum = false;         // The sum of solar radiation on a given day in Megajoules.
	bool  et0faoevapotranspiration = false;    // Daily sum of ET0 Reference Evapotranspiration of a well watered grass field.
	bool  uvindexmax = false;                    // Daily maximum in UV Index starting from 0.
	bool  uvindexclearskymax = false;            // Daily maximum in UV Index starting from 0.
};

string Daily::url()
{
	string url = "&daily=";

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