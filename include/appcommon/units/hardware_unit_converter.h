// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef OTLS_APPCOMMON_UNITS_HARDWAREUNITCONVERTER_H_
#define OTLS_APPCOMMON_UNITS_HARDWAREUNITCONVERTER_H_

#include "models/base/units.h"
#include "models/transmissionline/hardware.h"

/// \par OVERVIEW
///
/// This class converts hardware between unit systems as well as unit styles.
class HardwareUnitConverter {
 public:
  /// \brief Changes between unit styles.
  /// \param[in] system
  ///   The unit system.
  /// \param[in] style_from
  ///   The unit style to convert from.
  /// \param[in] style_to
  ///   The unit style to convert to.
  /// \param[in,out] hardware
  ///   The hardware to be converted.
  /// The 'different' style units are as follows:
  ///  - area_cross_section = [m^2 or ft^2]
  ///  - length = [m or ft]
  ///  - weight = [N or lb]
  static void ConvertUnitStyle(const units::UnitSystem& system,
                               const units::UnitStyle& style_from,
                               const units::UnitStyle& style_to,
                               Hardware& hardware);

  /// \brief Changes between unit systems.
  /// \param[in] system_from
  ///   The unit system to convert from. These must be consistent style units.
  /// \param[in] system_to
  ///   The unit system to convert to. These will also be in consistent style
  ///   units.
  /// \param[in,out] hardware
  ///   The hardware to be converted.
  /// This function requires that the hardware be in a 'consistent' unit style.
  static void ConvertUnitSystem(const units::UnitSystem& system_from,
                                const units::UnitSystem& system_to,
                                Hardware& hardware);
};

#endif  // OTLS_APPCOMMON_UNITS_HARDWAREUNITCONVERTER_H_
