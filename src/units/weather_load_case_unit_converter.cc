// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "appcommon/units/weather_load_case_unit_converter.h"

#include "wx/wx.h"

bool WeatherLoadCaseUnitConverter::ConvertUnitStyleToConsistent(
    const int& version,
    const units::UnitSystem& system,
    WeatherLoadCase& weathercase) {
  bool status = true;

  // sends to proper converter function
  if (version == 0) {
    // points to latest converter version
    ConvertUnitStyleToConsistentV1(system, weathercase);
  } else if (version == 1) {
    ConvertUnitStyleToConsistentV1(system, weathercase);
  } else {
    wxString message = " Invalid version number. Aborting conversion.";
    wxLogError(message);
    status = false;
  }

  return status;
}

void WeatherLoadCaseUnitConverter::ConvertUnitStyleToDifferent(
    const units::UnitSystem& system,
    WeatherLoadCase& weathercase) {
  if (system == units::UnitSystem::kMetric) {
    weathercase.thickness_ice = units::ConvertLength(
        weathercase.thickness_ice,
        units::LengthConversionType::kMetersToCentimeters);
  } else if (system == units::UnitSystem::kImperial) {
    weathercase.thickness_ice = units::ConvertLength(
        weathercase.thickness_ice,
        units::LengthConversionType::kFeetToInches);
  }
}

void WeatherLoadCaseUnitConverter::ConvertUnitSystem(
    const units::UnitSystem& system_from,
    const units::UnitSystem& system_to,
    WeatherLoadCase& weathercase) {
  if (system_from == system_to) {
    return;
  }

  if (system_to == units::UnitSystem::kMetric) {
    weathercase.density_ice = units::ConvertForce(
        weathercase.density_ice,
        units::ForceConversionType::kPoundsToNewtons);
    weathercase.density_ice = units::ConvertLength(
        weathercase.density_ice,
        units::LengthConversionType::kFeetToMeters,
        3,
        false);

    weathercase.pressure_wind = units::ConvertStress(
        weathercase.pressure_wind,
        units::StressConversionType::kPsfToPascal);
    weathercase.temperature_cable = units::ConvertTemperature(
        weathercase.temperature_cable,
        units::TemperatureConversionType::kFahrenheitToCelsius);
    weathercase.thickness_ice = units::ConvertLength(
        weathercase.thickness_ice,
        units::LengthConversionType::kFeetToMeters);
  } else if (system_to == units::UnitSystem::kImperial) {
    weathercase.density_ice = units::ConvertForce(
        weathercase.density_ice,
        units::ForceConversionType::kNewtonsToPounds);
    weathercase.density_ice = units::ConvertLength(
        weathercase.density_ice,
        units::LengthConversionType::kMetersToFeet,
        3,
        false);

    weathercase.pressure_wind = units::ConvertStress(
        weathercase.pressure_wind,
        units::StressConversionType::kPascalToPsf);
    weathercase.temperature_cable = units::ConvertTemperature(
        weathercase.temperature_cable,
        units::TemperatureConversionType::kCelsiusToFahrenheit);
    weathercase.thickness_ice = units::ConvertLength(
        weathercase.thickness_ice,
        units::LengthConversionType::kMetersToFeet);
  }
}

void WeatherLoadCaseUnitConverter::ConvertUnitStyleToConsistentV1(
    const units::UnitSystem& system,
    WeatherLoadCase& weathercase) {
  if (system == units::UnitSystem::kMetric) {
    weathercase.thickness_ice = units::ConvertLength(
        weathercase.thickness_ice,
        units::LengthConversionType::kCentimetersToMeters);
  } else if (system == units::UnitSystem::kImperial) {
    weathercase.thickness_ice = units::ConvertLength(
        weathercase.thickness_ice,
        units::LengthConversionType::kInchesToFeet);
  }
}
