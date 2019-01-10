// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef APPCOMMON_UNITS_LINE_STRUCTURE_UNIT_CONVERTER_H_
#define APPCOMMON_UNITS_LINE_STRUCTURE_UNIT_CONVERTER_H_

#include "models/base/units.h"
#include "models/transmissionline/line_structure.h"

/// \par OVERVIEW
///
/// This class converts a line structure between unit systems as well as unit
/// styles.
///
/// \par DATA OWNERSHIP
///
/// Only member variables that are owned (i.e. responsibility for allocating or
/// releasing memory) will be converted. Pointers are a typical example where
/// the data is referenced but not owned.
class LineStructureUnitConverter {
 public:
  /// \brief Changes between unit styles.
  /// \param[in] system
  ///   The unit system.
  /// \param[in] style_from
  ///   The unit style to convert from.
  /// \param[in] style_to
  ///   The unit style to convert to.
  /// \param[in,out] line_structure
  ///   The line structure to be converted.
  /// The 'different' style units are as follows:
  ///  - height adjustment = [m or ft]
  ///  - offset = [m or ft]
  ///  - rotation = [deg or deg]
  ///  - station = [m or ft]
  /// The following variables are not owned, and will be skipped:
  ///  - hardwares
  ///  - structure
  static void ConvertUnitStyle(const units::UnitSystem& system,
                               const units::UnitStyle& style_from,
                               const units::UnitStyle& style_to,
                               LineStructure& line_structure);

  /// \brief Changes between unit systems.
  /// \param[in] system_from
  ///   The unit system to convert from. These must be consistent style units.
  /// \param[in] system_to
  ///   The unit system to convert to. These will also be in consistent style
  ///   units.
  /// \param[in,out] line_structure
  ///   The line structure to be converted.
  /// The following variables are not owned, and will be skipped:
  ///  - hardwares
  ///  - structure
  /// This function requires that the line structure be in a 'consistent' unit
  /// style.
  static void ConvertUnitSystem(const units::UnitSystem& system_from,
                                const units::UnitSystem& system_to,
                                LineStructure& line_structure);
};

#endif  // APPCOMMON_UNITS_LINE_STRUCTURE_UNIT_CONVERTER_H_
