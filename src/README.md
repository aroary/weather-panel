# Documentation
### Data Access
|Name|Value|ID|Type|Description|
|---|---|---|:-:|---|
|**latitude**|latitude| `latitude`|*Number*|Geographical WGS84 coordinate of the location |
|**longitude**|longitude|`longitude`|*Number*|Geographical WGS84 coordinate of the location |
|**elevation**|elevation|`elevation`|*Number*|The elevation used for statistical downscaling. |
|**temperature_unit**|temperatureunit|`temperatureunit`|*String*| |
|**windspeed_unit**|windspeedunit|`windspeedunit`|*String*| |
|**precipitation_unit**|precipitationunit|`precipitationunit`|*String*| |
|**timeformat**|timeformat|`timeformat`|*String*| |
|**timezone**|timezone|`timezone`|*String*|Any time zone name from the time zone database is supported.|
|**cell_selection**|cellselection|`cellselection`|*String*|Set a preference how grid-cells are selected.|
|**hourly**|hourly| |*Object*|Hourly values.|
|**temperature_2m**|temperature|`hourly.temperature`|*Number*|Air temperature above ground.|
|**relativehumidity_2m**|relativehumidity|`hourly.relativehumidity`|*Number*|Relative humidity above ground.|
|**dewpoint_2m**|dewpoint|`hourly.dewpoint`|*Number*|Dew point temperature above ground.|
|**apparent_temperature**|apparenttemperature|`hourly.apparenttemperature`|*Number*|Apparent temperature is the perceived feels-like temperature combining wind chill factor, relative humidity and solar radiation.|
|**pressure_msl**|pressuremsl|`hourly.pressuremsl`|*Number*|Atmospheric air pressure reduced to mean sea level.|
|**surface_pressure**|surfacepressure|`hourly.surfacepressure`|*Number*|Atmospheric air pressure at surface.|
|**cloudcover**|cloudcover|`hourly.cloudcover`|*Percent*|Total cloud cover as an area fraction.|
|**cloudcover_low**|cloudcoverlow|`hourly.cloudcoverlow`|*Percent*|Low level clouds and fog up to 3 km altitude.|
|**cloudcover_mid**|cloudcovermid|`hourly.cloudcovermid`|*Percent*|Mid level clouds from 3 to 8 km altitude.|
|**cloudcover_high**|cloudcoverhigh|`hourly.cloudcoverhigh`|*Percent*|High level clouds from 8 km altitude.|
|**windspeed_10m**|windspeed|`hourly.windspeed`|*Number*|Wind speed at 10 meters above ground.|
|**windspeed_80m**|windspeed|`hourly.windspeed`|*Number*|Wind speed at 80 meters above ground.|
|**windspeed_120m**|windspeed|`hourly.windspeed`|*Number*|Wind speed at 120 meters above ground.|
|**windspeed_180m**|windspeed|`hourly.windspeed`|*Number*|Wind speed at 180 meters above ground.|
|**winddirection_10m**|winddirection|`hourly.winddirection`|*Number*|Wind direction at 10 meters above ground.|
|**winddirection_80m**|winddirection|`hourly.winddirection`|*Number*|Wind direction at 80 meters above ground.|
|**winddirection_120m**|winddirection|`hourly.winddirection`|*Number*|Wind direction at 120 meters above ground.|
|**winddirection_180m**|winddirection|`hourly.winddirection`|*Number*|Wind direction at 180 meters above ground.|
|**windgusts_10m**|windgusts|`hourly.windgusts`|*Number*|Gusts at 10 meters above ground as a maximum of the preceding hour.|
|**shortwave_radiation**|shortwaveradiation|`hourly.shortwaveradiation`|*Number*|Shortwave solar radiation as average of the preceding hour.|
|**direct_radiation**|directradiation|`hourly.directradiation`|*Number*|Direct solar radiation as average of the preceding hour on the horizontal plane.|
|**direct_normal_irradiance**|directnormalirradiance|`hourly.directnormalirradiance`|*Number*|Direct solar radiation as average of the preceding hour on normal plane (perpendicular to the sun).|
|**diffuse_radiation**|diffuseradiation|`hourly.diffuseradiation`|*Number*|Diffuse solar radiation as average of the preceding hour.|  |
|**vapor_pressure_deficit**|vaporpressuredeficit|`hourly.vaporpressuredeficit`|*Number*|Vapor Pressure Deficit (VPD) in kilopascal (kPa).|
|**evapotranspiration**|evapotranspiration|`hourly.evapotranspiration`|*Number*|Evapotranspration from land surface and plants that weather models assumes for this location.|
|**cape**|cape|`hourly.cape`|*Number*|Convective available potential energy.|
|**et0_fao_evapotranspiration**|et0faoevapotranspiration|`hourly.et0faoevapotranspiration`|*Number*|ET₀ Reference Evapotranspiration of a well watered grass field.|
|**precipitation**|precipitation|`hourly.precipitation`|*Number*|Total precipitation (rain, showers, snow) sum of the preceding hour.|
|**snowfall**|snowfall|`hourly.snowfall`|*Number*|Snowfall amount of the preceding hour in centimeters.|
|**precipitation_probability**|precipitationprobability|`hourly.precipitationprobability`|*Percent*|Probability of precipitation with more than 0.1 mm of the preceding hour.|
|**showers**|showers|`hourly.showers`|*Number*|Showers from convective precipitation in millimeters from the preceding hour.|
|**weathercode**|weathercode|`hourly.weathercode`|*Number*|Weather condition as a numeric code.|
|**snow_depth**|snowdepth|`hourly.snowdepth`|*Number*|Snow depth on the ground.|
|**freezinglevel_height**|freezinglevelheight|`hourly.freezinglevelheight`|*Number*|Altitude above sea level of the 0°C level.|
|**visibility**|visibility|`hourly.visibility`|*Number*|Viewing distance in meters.|
|**soil_temperature_0cm**|soiltemperature|`hourly.soiltemperature`|*Number*|Temperature in the soil at 0 cm depths.|
|**soil_temperature_6cm**|soiltemperature|`hourly.soiltemperature`|*Number*|Temperature in the soil at 6 cm depths.|
|**soil_temperature_18cm**|soiltemperature|`hourly.soiltemperature`|*Number*|Temperature in the soil at 18 cm depths.|
|**soil_temperature_54cm**|soiltemperature|`hourly.soiltemperature`|*Number*|Temperature in the soil at 54 cm depths.|
|**soil_moisture_0_1cm**|soilmoisture|`hourly.soilmoisture`|*Number*|Average soil water content as volumetric mixing ratio at 0 to 1 cm depths.|
|**soil_moisture_1_3cm**|soilmoisture|`hourly.soilmoisture`|*Number*|Average soil water content as volumetric mixing ratio at 1 to 3 cm depths.|
|**soil_moisture_3_9cm**|soilmoisture|`hourly.soilmoisture`|*Number*|Average soil water content as volumetric mixing ratio at 3 to 9 cm depths.|
|**soil_moisture_9_27cm**|soilmoisture|`hourly.soilmoisture`|*Number*|Average soil water content as volumetric mixing ratio at 9 to 27 cm depths.|
|**soil_moisture_27_81cm**|soilmoisture|`hourly.soilmoisture`|*Number*|Average soil water content as volumetric mixing ratio at 27 to 81 cm depths.|
|**is_day**|isday|`hourly.isday`|*Boolean*|if the current time step has daylight.|
|**daily**|daily| |*Object*|Daily values.|
|**temperature_2m_max**|temperaturemax|`daily.temperaturemax`|*Number*|Maximum daily air temperature at 2 meters above ground.|
|**temperature_2m_min**|temperaturemin|`daily.temperaturemin`|*Number*|Minimum daily air temperature at 2 meters above ground.|
|**apparent_temperature_max**|apparenttemperaturemax|`daily.apparenttemperaturemax`|*Number*|Maximum daily apparent temperature.|
|**apparent_temperature_min**|apparenttemperaturemin|`daily.apparenttemperaturemin`|*Number*|Minimum daily apparent temperature.|
|**precipitation_sum**|precipitationsum|`daily.precipitationsum`|*Number*|Sum of daily precipitation (including rain, showers and snowfall).|
|**rain_sum**|rain_sum|`daily.rainsum`|*Number*|Sum of daily rain.|
|**showers_sum**|showerssum|`daily.showerssum`|*Number*|Sum of daily showers.|
|**snowfall_sum**|snowfallsum|`daily.snowfallsum`|*Number*|Sum of daily snowfall.|
|**precipitation_hours**|precipitationhours|`daily.precipitationhours`|*Number*|The number of hours with rain.|
|**precipitation_probability_max**|precipitationprobabilitymax|`daily.precipitationprobabilitymax`|*Percent*|Max probability of precipitation.|
|**precipitation_probability_min**|precipitationprobabilitymin|`daily.precipitationprobabilitymin`|*Percent*|Min probability of precipitation.|
|**precipitation_probability_mean**|precipitationprobabilitymean|`daily.precipitationprobabilitymean`|*Percent*|Mean probability of precipitation.|
|**weathercode**|weathercode|`daily.weathercode`|*Number*|The most severe weather condition on a given day.|
|**sunrise**|sunrise|`daily.sunrise`|*String*|Sun rise time.|
|**sunset**|sunset|`daily.sunset`|*String*|Sun set time.|
|**windspeed_10m_max**|windspeedmax|`daily.windspeedmax`|*Number*|Maximum wind speed and gusts on a day.|
|**windgusts_10m_max**|windgustsmax|`daily.windgustsmax`|*Number*|Maximum wind speed and gusts on a day.|
|**winddirection_10m_dominant**|winddirectiondominant|`daily.winddirectiondominant`|*Number*|Dominant wind direction.|
|**shortwave_radiation_sum**|shortwaveradiationsum|`daily.shortwaveradiationsum`|*Number*|The sum of solar radiation on a given day in Megajoules.|
|**et0_fao_evapotranspiration**|et0faoevapotranspiration|`daily.et0faoevapotranspiration`|*Number*|Daily sum of ET₀ Reference Evapotranspiration of a well watered grass field.|
|**uv_index_max**|uv_index_max|`daily.uvindexmax`|*Number*|Daily maximum in UV Index starting from 0.|
|**uv_index_clear_sky_max**|uvindexclearskymax|`daily.uvindexclearskymax`|*Number*|Daily maximum in UV Index starting from 0.|