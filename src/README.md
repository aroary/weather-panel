# Documentation
### Data Access
|Name|Type|Description|
|---|:-:|---|
|**latitude**|*Number*|Geographical WGS84 coordinate of the location.|
|**longitude**|*Number*|Geographical WGS84 coordinate of the location.|
|**elevation**|*Number*|The elevation used for statistical downscaling.|
|**temperature_unit**|*String*| |
|**windspeed_unit**|*String*| |
|**precipitation_unit**|*String*| |
|**timeformat**|*String*| |
|**timezone**|*String*|Any time zone name from the time zone database is supported.|
|**cell_selection**|*String*|Set a preference how grid-cells are selected.|
|**hourly**|*Object*|Hourly values.|
|**temperature_2m**|*Number*|Air temperature above ground.|
|**relativehumidity_2m**|*Number*|Relative humidity above ground.|
|**dewpoint_2m**|*Number*|Dew point temperature above ground.|
|**apparent_temperature**|*Number*|Apparent temperature is the perceived feels-like temperature combining wind chill factor, relative humidity and solar radiation.|
|**pressure_msl**|*Number*|Atmospheric air pressure reduced to mean sea level.|
|**surface_pressure**|*Number*|Atmospheric air pressure at surface.|
|**cloudcover**|*Percent*|Total cloud cover as an area fraction.|
|**cloudcover_low**|*Percent*|Low level clouds and fog up to 3 km altitude.|
|**cloudcover_mid**|*Percent*|Mid level clouds from 3 to 8 km altitude.|
|**cloudcover_high**|*Percent*|High level clouds from 8 km altitude.|
|**windspeed_10m**|*Number*|Wind speed at 10 meters above ground.|
|**windspeed_80m**|*Number*|Wind speed at 80 meters above ground.|
|**windspeed_120m**|*Number*|Wind speed at 120 meters above ground.|
|**windspeed_180m**|*Number*|Wind speed at 180 meters above ground.|
|**winddirection_10m**|*Number*|Wind direction at 10 meters above ground.|
|**winddirection_80m**|*Number*|Wind direction at 80 meters above ground.|
|**winddirection_120m**|*Number*|Wind direction at 120 meters above ground.|
|**winddirection_180m**|*Number*|Wind direction at 180 meters above ground.|
|**windgusts_10m**|*Number*|Gusts at 10 meters above ground as a maximum of the preceding hour.|
|**shortwave_radiation**|*Number*|Shortwave solar radiation as average of the preceding hour.|
|**direct_radiation**|*Number*|Direct solar radiation as average of the preceding hour on the horizontal plane.|
|**direct_normal_irradiance**|*Number*|Direct solar radiation as average of the preceding hour on normal plane (perpendicular to the sun).|
|**diffuse_radiation**|*Number*|Diffuse solar radiation as average of the preceding hour.|  |
|**vapor_pressure_deficit**|*Number*|Vapor Pressure Deficit (VPD) in kilopascal (kPa).|
|**evapotranspiration**|*Number*|Evapotranspration from land surface and plants that weather models assumes for this location.|
|**cape**|*Number*|Convective available potential energy.|
|**et0_fao_evapotranspiration**|*Number*|ET₀ Reference Evapotranspiration of a well watered grass field.|
|**precipitation**|*Number*|Total precipitation (rain, showers, snow) sum of the preceding hour.|
|**snowfall**|*Number*|Snowfall amount of the preceding hour in centimeters.|
|**precipitation_probability**|*Percent*|Probability of precipitation with more than 0.1 mm of the preceding hour.|
|**showers**|*Number*|Showers from convective precipitation in millimeters from the preceding hour.|
|**weathercode**|*Number*|Weather condition as a numeric code.|
|**snow_depth**|*Number*|Snow depth on the ground.|
|**freezinglevel_height**|*Number*|Altitude above sea level of the 0°C level.|
|**visibility**|*Number*|Viewing distance in meters.|
|**soil_temperature_0cm**|*Number*|Temperature in the soil at 0 cm depths.|
|**soil_temperature_6cm**|*Number*|Temperature in the soil at 6 cm depths.|
|**soil_temperature_18cm**|*Number*|Temperature in the soil at 18 cm depths.|
|**soil_temperature_54cm**|*Number*|Temperature in the soil at 54 cm depths.|
|**soil_moisture_0_1cm**|*Number*|Average soil water content as volumetric mixing ratio at 0 to 1 cm depths.|
|**soil_moisture_1_3cm**|*Number*|Average soil water content as volumetric mixing ratio at 1 to 3 cm depths.|
|**soil_moisture_3_9cm**|*Number*|Average soil water content as volumetric mixing ratio at 3 to 9 cm depths.|
|**soil_moisture_9_27cm**|*Number*|Average soil water content as volumetric mixing ratio at 9 to 27 cm depths.|
|**soil_moisture_27_81cm**|*Number*|Average soil water content as volumetric mixing ratio at 27 to 81 cm depths.|
|**is_day**|*Boolean*|if the current time step has daylight.|
|**daily**|*Object*|Daily values.|
|**temperature_2m_max**|*Number*|Maximum daily air temperature at 2 meters above ground.|
|**temperature_2m_min**|*Number*|Minimum daily air temperature at 2 meters above ground.|
|**apparent_temperature_max**|*Number*|Maximum daily apparent temperature.|
|**apparent_temperature_min**|*Number*|Minimum daily apparent temperature.|
|**precipitation_sum**|*Number*|Sum of daily precipitation (including rain, showers and snowfall).|
|**rain_sum**|*Number*|Sum of daily rain.|
|**showers_sum**|*Number*|Sum of daily showers.|
|**snowfall_sum**|*Number*|Sum of daily snowfall.|
|**precipitation_hours**|*Number*|The number of hours with rain.|
|**precipitation_probability_max**|*Percent*|Max probability of precipitation.|
|**precipitation_probability_min**|*Percent*|Min probability of precipitation.|
|**precipitation_probability_mean**|*Percent*|Mean probability of precipitation.|
|**weathercode**|*Number*|The most severe weather condition on a given day.|
|**sunrise**|*String*|Sun rise time.|
|**sunset**|*String*|Sun set time.|
|**windspeed_10m_max**|*Number*|Maximum wind speed and gusts on a day.|
|**windgusts_10m_max**|*Number*|Maximum wind speed and gusts on a day.|
|**winddirection_10m_dominant**|*Number*|Dominant wind direction.|
|**shortwave_radiation_sum**|*Number*|The sum of solar radiation on a given day in Megajoules.|
|**et0_fao_evapotranspiration**|*Number*|Daily sum of ET₀ Reference Evapotranspiration of a well watered grass field.|
|**uv_index_max**|*Number*|Daily maximum in UV Index starting from 0.|
|**uv_index_clear_sky_max**|*Number*|Daily maximum in UV Index starting from 0.|