// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef APPCOMMON_UNITS_LINE_CABLE_UNIT_CONVERTER_H_
#define APPCOMMON_UNITS_LINE_CABLE_UNIT_CONVERTER_H_

#include "models/base/units.h"
#include "models/transmissionline/line_cable.h"

/// \par OVERVIEW
///
/// This class converts a line cable between unit systems as well as unit
/// styles.
///
/// \par DATA OWNERSHIP
///
/// Only member variables that are owned (i.e. responsibility for allocating or
/// releasing memory) will be converted. Pointers are a typical example where
/// the data is referenced but not owned.
///
/// \par RECURSION
///
/// This class supports optionally invoking member variable converters,
/// depending on whether the entire set of data needs converted or just a
/// portion of it.
class LineCableUnitConverter {
 public:
  /// \brief Changes between unit styles.
  /// \param[in] system
  ///   The unit system.
  /// \param[in] style_from
  ///   The unit style to convert from.
  /// \param[in] style_to
  ///   The unit style to convert to.
  /// \param[in] is_recursive
  ///   An indicator that determines if member variable converters are invoked.
  /// \param[in,out] line_cable
  ///   The line cable to be converted.
  /// The 'different' style units are as follows:
  ///  - spacing_attachments_ruling_span = [m or ft]
  /// The following variables are not owned, and will be skipped:
  ///  - cable
  ///  - weathercase_stretch_creep
  ///  - weathercase_stretch_load
  static void ConvertUnitStyle(const units::UnitSystem& system,
                               const units::UnitStyle& style_from,
                               const units::UnitStyle& style_to,
                               const bool& is_recursive,
                               LineCable& line_cable);

  /// \brief Changes between unit systems.
  /// \param[in] system_from
  ///   The unit system to convert from. These must be consistent style units.
  /// \param[in] system_to
  ///   The unit system to convert to. These will also be in consistent style
  ///   units.
  /// \param[in] is_recursive
  ///   An indicator that determines if member variable converters are invoked.
  /// \param[in,out] line_cable
  ///   The line_cable to be converted.
  /// The following variables are not owned, and will be skipped:
  ///  - cable
  ///  - weathercase_stretch_creep
  ///  - weathercase_stretch_load
  /// This function requires that the line cable be in a 'consistent' unit
  /// style.
  static void ConvertUnitSystem(const units::UnitSystem& system_from,
                                const units::UnitSystem& system_to,
                                const bool& is_recursive,
                                LineCable& line_cable);
};

#endif  // APPCOMMON_UNITS_LINE_CABLE_UNIT_CONVERTER_H_
