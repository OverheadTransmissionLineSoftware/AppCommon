// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef APPCOMMON_UNITS_HARDWARE_UNIT_CONVERTER_H_
#define APPCOMMON_UNITS_HARDWARE_UNIT_CONVERTER_H_

#include "models/base/units.h"
#include "models/transmissionline/hardware.h"

/// \par OVERVIEW
///
/// This class converts hardware between unit systems as well as unit styles.
///
/// \par VERSIONS
///
/// Versions are needed when converting to the 'consistent' unit style. The
/// the starter 'different' style units can change over time, and versions
/// allow the converter to align with a specific xml node if needed.
///
/// When converting to the 'different' unit style the latest converter version
/// is used.
class HardwareUnitConverter {
 public:
  /// \brief Converts to 'consistent' unit style.
  /// \param[in] version
  ///   The version. 0 (zero) indicates the latest version.
  /// \param[in] system
  ///   The unit system.
  /// \param[in,out] hardware
  ///   The hardware to be converted.
  /// \return The success status.
  static bool ConvertUnitStyleToConsistent(const int& version,
                                           const units::UnitSystem& system,
                                           Hardware& hardware);

  /// \brief Converts to 'different' unit style.
  /// \param[in] system
  ///   The unit system.
  /// \param[in,out] hardware
  ///   The hardware to be converted.
  /// The 'different' style units are as follows:
  ///  - area_cross_section = [m^2 or ft^2]
  ///  - length = [m or ft]
  ///  - weight = [N or lb]
  static void ConvertUnitStyleToDifferent(const units::UnitSystem& system,
                                          Hardware& hardware);

  /// \brief Converts between unit systems.
  /// \param[in] system_from
  ///   The unit system to convert from.
  /// \param[in] system_to
  ///   The unit system to convert to.
  /// \param[in,out] hardware
  ///   The hardware to be converted.
  /// This function requires that the hardware be in a 'consistent' unit style.
  static void ConvertUnitSystem(const units::UnitSystem& system_from,
                                const units::UnitSystem& system_to,
                                Hardware& hardware);

 private:
  /// \brief Converts to 'consistent' unit style using version 1.
  /// \param[in] system
  ///   The unit system.
  /// \param[in,out] hardware
  ///   The hardware to be converted.
  static void ConvertUnitStyleToConsistentV1(const units::UnitSystem& system,
                                             Hardware& hardware);
};

#endif  // APPCOMMON_UNITS_HARDWARE_UNIT_CONVERTER_H_
