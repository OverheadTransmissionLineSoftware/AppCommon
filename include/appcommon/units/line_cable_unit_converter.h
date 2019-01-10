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
///
/// \par VERSIONS
///
/// Versions are needed when converting to the 'consistent' unit style. The
/// the starter 'different' style units can change over time, and versions
/// allow the converter to align with a specific xml node if needed. The version
/// will not apply to any member variable converters, as those xml node versions
/// may be different. If the recursive flag is used, the latest version of all
/// member variable converters is used.
///
/// When converting to the 'different' unit style the latest converter version
/// is used.
class LineCableUnitConverter {
 public:
  /// \brief Converts to 'consistent' unit style.
  /// \param[in] version
  ///   The version. 0 (zero) indicates the latest version.
  /// \param[in] system
  ///   The unit system.
  /// \param[in] is_recursive
  ///   An indicator that determines if member variable converters are invoked.
  /// \param[in,out] line_cable
  ///   The line cable to be converted.
  /// \return The success status.
  /// The following variables are not owned, and will be skipped:
  ///  - cable
  ///  - weathercase_stretch_creep
  ///  - weathercase_stretch_load
  static bool ConvertUnitStyleToConsistent(const int& version,
                                           const units::UnitSystem& system,
                                           const bool& is_recursive,
                                           LineCable& line_cable);

  /// \brief Converts to 'different' unit style.
  /// \param[in] system
  ///   The unit system.
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
  static void ConvertUnitStyleToDifferent(const units::UnitSystem& system,
                                          const bool& is_recursive,
                                          LineCable& line_cable);

  /// \brief Converts between unit systems.
  /// \param[in] system_from
  ///   The unit system to convert from.
  /// \param[in] system_to
  ///   The unit system to convert to.
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

 private:
  /// \brief Converts to 'consistent' unit style using version 1.
  /// \param[in] system
  ///   The unit system.
  /// \param[in] is_recursive
  ///   An indicator that determines if member variable converters are invoked.
  /// \param[in,out] line_cable
  ///   The line_cable to be converted.
  static void ConvertUnitStyleToConsistentV1(const units::UnitSystem& system,
                                             const bool& is_recursive,
                                             LineCable& line_cable);
};

#endif  // APPCOMMON_UNITS_LINE_CABLE_UNIT_CONVERTER_H_
