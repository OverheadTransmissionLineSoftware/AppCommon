// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef OTLS_APPCOMMON_UNITS_STRUCTUREUNITCONVERTER_H_
#define OTLS_APPCOMMON_UNITS_STRUCTUREUNITCONVERTER_H_

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
class StructureUnitConverter {
 public:
  /// \brief Changes between unit styles.
  /// \param[in] system
  ///   The unit system.
  /// \param[in] style_from
  ///   The unit style to convert from.
  /// \param[in] style_to
  ///   The unit style to convert to.
  /// \param[in,out] structure
  ///   The structure to be converted.
  /// The 'different' style units are as follows:
  ///  - height = [m or ft]
  static void ConvertUnitStyle(const units::UnitSystem& system,
                               const units::UnitStyle& style_from,
                               const units::UnitStyle& style_to,
                               Structure& structure);

  /// \brief Changes between unit systems.
  /// \param[in] system_from
  ///   The unit system to convert from. These must be consistent style units.
  /// \param[in] system_to
  ///   The unit system to convert to. These will also be in consistent style
  ///   units.
  /// \param[in,out] structure
  ///   The structure to be converted.
  /// This function requires that the structure be in a 'consistent' unit style.
  static void ConvertUnitSystem(const units::UnitSystem& system_from,
                                const units::UnitSystem& system_to,
                                Structure& structure);
};

#endif  // OTLS_APPCOMMON_UNITS_STRUCTUREUNITCONVERTER_H_
