// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef APPCOMMON_UNITS_STRUCTURE_UNIT_CONVERTER_H_
#define APPCOMMON_UNITS_STRUCTURE_UNIT_CONVERTER_H_

#include "models/base/units.h"
#include "models/transmissionline/structure.h"

/// \par OVERVIEW
///
/// This class converts a structure attachment between unit systems as well as
/// unit styles.
class StructureAttachmentUnitConverter {
 public:
  /// \brief Changes between unit styles.
  /// \param[in] system
  ///   The unit system.
  /// \param[in] style_from
  ///   The unit style to convert from.
  /// \param[in] style_to
  ///   The unit style to convert to.
  /// \param[in,out] attachment
  ///   The attachment to be converted.
  /// The 'different' style units are as follows:
  ///  - offset_longitudinal = [m or ft]
  ///  - offset_transverse = [m or ft]
  ///  - offset_vertical_top = [m or ft]
  static void ConvertUnitStyle(const units::UnitSystem& system,
                               const units::UnitStyle& style_from,
                               const units::UnitStyle& style_to,
                               StructureAttachment& attachment);

  /// \brief Changes between unit systems.
  /// \param[in] system_from
  ///   The unit system to convert from.
  /// \param[in] system_to
  ///   The unit system to convert to.
  /// \param[in,out] attachment
  ///   The attachment to be converted.
  /// This function requires that the attachment is in a 'consistent' unit
  /// style.
  static void ConvertUnitSystem(const units::UnitSystem& system_from,
                                const units::UnitSystem& system_to,
                                StructureAttachment& attachment);
};

/// \par OVERVIEW
///
/// This class converts a structure between unit systems as well as unit styles.
///
/// \par RECURSION
///
/// This class supports optionally invoking member variable converters,
/// depending on whether the entire set of data needs converted or just a
/// portion of it.
class StructureUnitConverter {
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
  /// \param[in,out] structure
  ///   The structure to be converted.
  /// The 'different' style units are as follows:
  ///  - height = [m or ft]
  static void ConvertUnitStyle(const units::UnitSystem& system,
                               const units::UnitStyle& style_from,
                               const units::UnitStyle& style_to,
                               const bool& is_recursive,
                               Structure& structure);

  /// \brief Changes between unit systems.
  /// \param[in] system_from
  ///   The unit system to convert from. These must be consistent style units.
  /// \param[in] system_to
  ///   The unit system to convert to. These will also be in consistent style
  ///   units.
  /// \param[in] is_recursive
  ///   An indicator that determines if member variable converters are invoked.
  /// \param[in,out] structure
  ///   The structure to be converted.
  /// This function requires that the structure be in a 'consistent' unit style.
  static void ConvertUnitSystem(const units::UnitSystem& system_from,
                                const units::UnitSystem& system_to,
                                const bool& is_recursive,
                                Structure& structure);
};

#endif  // APPCOMMON_UNITS_STRUCTURE_UNIT_CONVERTER_H_
