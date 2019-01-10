// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef APPCOMMON_UNITS_WEATHER_LOAD_CASE_UNIT_CONVERTER_H_
#define APPCOMMON_UNITS_WEATHER_LOAD_CASE_UNIT_CONVERTER_H_

#include "models/base/units.h"
#include "models/transmissionline/weather_load_case.h"

/// \par OVERVIEW
///
/// This class converts a weather case between unit systems as well as unit
/// styles.
///
/// \par VERSIONS
///
/// Versions are needed when converting to the 'consistent' unit style. The
/// the starter 'different' style units can change over time, and versions
/// allow the converter to align with a specific xml node if needed.
///
/// When converting to the 'different' unit style the latest converter version
/// is used.
class WeatherLoadCaseUnitConverter {
 public:
  /// \brief Converts to 'consistent' unit style.
  /// \param[in] version
  ///   The version. 0 (zero) indicates the latest version.
  /// \param[in] system
  ///   The unit system.
  /// \param[in,out] weathercase
  ///   The weather case to be converted.
  /// \return The success status.
  static bool ConvertUnitStyleToConsistent(const int& version,
                                           const units::UnitSystem& system,
                                           WeatherLoadCase& weathercase);

  /// \brief Converts to 'different' unit style.
  /// \param[in] system
  ///   The unit system.
  /// \param[in,out] weathercase
  ///   The weather case to be converted.
  /// The 'different' style units are as follows:
  ///  - density_ice = [N/m^3 or lb/ft^3]
  ///  - pressure_wind = [Pa or lb/ft^2]
  ///  - temperature_cable = [degC or deg F]
  ///  - thickness_ice = [cm or in]
  static void ConvertUnitStyleToDifferent(const units::UnitSystem& system,
                                          WeatherLoadCase& weathercase);

  /// \brief Changes the weather case between unit systems.
  /// \param[in] system_from
  ///   The unit system to convert from. These must be consistent style units.
  /// \param[in] system_to
  ///   The unit system to convert to. These will also be in consistent style
  ///   units.
  /// \param[in,out] weathercase
  ///   The weather case to be converted.
  /// This function requires that the weathercase be in a 'consistent' unit style.
  static void ConvertUnitSystem(const units::UnitSystem& system_from,
                                const units::UnitSystem& system_to,
                                WeatherLoadCase& weathercase);

 private:
  /// \brief Converts to 'consistent' unit style using version 1.
  /// \param[in] system
  ///   The unit system.
  /// \param[in,out] weathercase
  ///   The weather case to be converted.
  static void ConvertUnitStyleToConsistentV1(const units::UnitSystem& system,
                                             WeatherLoadCase& weathercase);
};

#endif  // APPCOMMON_UNITS_WEATHER_LOAD_CASE_UNIT_CONVERTER_H_
