// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef OTLS_APPCOMMON_UNITS_TRANSMISSIONLINEUNITCONVERTER_H_
#define OTLS_APPCOMMON_UNITS_TRANSMISSIONLINEUNITCONVERTER_H_

#include "models/base/units.h"
#include "models/transmissionline/transmission_line.h"

/// \par OVERVIEW
///
/// This class converts a transmission line between unit systems as well as unit
/// styles.
class TransmissionLineUnitConverter {
 public:
  /// \brief Changes between unit styles.
  /// \param[in] system
  ///   The unit system.
  /// \param[in] style_from
  ///   The unit style to convert from.
  /// \param[in] style_to
  ///   The unit style to convert to.
  /// \param[in,out] line
  ///   The transmission line to be converted.
  /// The 'different' style units are as follows:
  ///  - origin = [m or ft]
  static void ConvertUnitStyle(const units::UnitSystem& system,
                               const units::UnitStyle& style_from,
                               const units::UnitStyle& style_to,
                               TransmissionLine& line);

  /// \brief Changes between unit systems.
  /// \param[in] system_from
  ///   The unit system to convert from. These must be consistent style units.
  /// \param[in] system_to
  ///   The unit system to convert to. These will also be in consistent style
  ///   units.
  /// \param[in,out] line
  ///   The transmission line to be converted.
  /// This function requires that the line cable be in a 'consistent' unit
  /// style.
  static void ConvertUnitSystem(const units::UnitSystem& system_from,
                                const units::UnitSystem& system_to,
                                TransmissionLine& line);
};

#endif  // OTLS_APPCOMMON_UNITS_TRANSMISSIONLINEUNITCONVERTER_H_