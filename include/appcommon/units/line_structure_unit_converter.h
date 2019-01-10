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
///
/// \par VERSIONS
///
/// Versions are needed when converting to the 'consistent' unit style. The
/// the starter 'different' style units can change over time, and versions
/// allow the converter to align with a specific xml node if needed.
///
/// When converting to the 'different' unit style the latest converter version
/// is used.
class LineStructureUnitConverter {
 public:
  /// \brief Converts to 'consistent' unit style.
  /// \param[in] version
  ///   The version. 0 (zero) indicates the latest version.
  /// \param[in] system
  ///   The unit system.
  /// \param[in,out] line_structure
  ///   The line structure to be converted.
  /// \return The success status.
  /// The following variables are not owned, and will be skipped:
  ///  - hardwares
  ///  - structure
  static bool ConvertUnitStyleToConsistent(const int& version,
                                           const units::UnitSystem& system,
                                           LineStructure& line_structure);

  /// \brief Converts to 'different' unit style.
  /// \param[in] system
  ///   The unit system.
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
  static void ConvertUnitStyleToDifferent(const units::UnitSystem& system,
                                          LineStructure& line_structure);

  /// \brief Changes between unit systems.
  /// \param[in] system_from
  ///   The unit system to convert from.
  /// \param[in] system_to
  ///   The unit system to convert to.
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

 private:
  /// \brief Converts to 'consistent' unit style.
  /// \param[in] system
  ///   The unit system.
  /// \param[in,out] line_structure
  ///   The line structure to be converted.
  /// The following variables are not owned, and will be skipped:
  ///  - hardwares
  ///  - structure
  static void ConvertUnitStyleToConsistentV1(const units::UnitSystem& system,
                                             LineStructure& line_structure);
};

#endif  // APPCOMMON_UNITS_LINE_STRUCTURE_UNIT_CONVERTER_H_
