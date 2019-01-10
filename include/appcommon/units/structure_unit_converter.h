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
///
/// \par VERSIONS
///
/// Versions are needed when converting to the 'consistent' unit style. The
/// the starter 'different' style units can change over time, and versions
/// allow the converter to align with a specific xml node if needed.
///
/// When converting to the 'different' unit style the latest converter version
/// is used.
class StructureAttachmentUnitConverter {
 public:
  /// \brief Converts to 'consistent' unit style.
  /// \param[in] version
  ///   The version. 0 (zero) indicates the latest version.
  /// \param[in] system
  ///   The unit system.
  /// \param[in,out] attachment
  ///   The attachment to be converted.
  /// \return The success status.
  static bool ConvertUnitStyleToConsistent(const int& version,
                                           const units::UnitSystem& system,
                                           StructureAttachment& attachment);

  /// \brief Converts to 'different' unit style.
  /// \param[in] system
  ///   The unit system.
  /// \param[in,out] attachment
  ///   The attachment to be converted.
  /// The 'different' style units are as follows:
  ///  - offset_longitudinal = [m or ft]
  ///  - offset_transverse = [m or ft]
  ///  - offset_vertical_top = [m or ft]
  static void ConvertUnitStyleToDifferent(const units::UnitSystem& system,
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

 private:
  /// \brief Converts to 'consistent' unit style using version 1.
  /// \param[in] system
  ///   The unit system.
  /// \param[in,out] attachment
  ///   The attachment to be converted.
  static void ConvertUnitStyleToConsistentV1(const units::UnitSystem& system,
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
class StructureUnitConverter {
 public:
  /// \brief Converts to 'consistent' unit style.
  /// \param[in] version
  ///   The version. 0 (zero) indicates the latest version.
  /// \param[in] system
  ///   The unit system.
  /// \param[in] is_recursive
  ///   An indicator that determines if member variable converters are invoked.
  /// \param[in,out] structure
  ///   The structure to be converted.
  /// \return The success status.
  static bool ConvertUnitStyleToConsistent(const int& version,
                                           const units::UnitSystem& system,
                                           const bool& is_recursive,
                                           Structure& structure);

  /// \brief Converts to 'different' unit style.
  /// \param[in] system
  ///   The unit system.
  /// \param[in] is_recursive
  ///   An indicator that determines if member variable converters are invoked.
  /// \param[in,out] structure
  ///   The structure to be converted.
  /// The 'different' style units are as follows:
  ///  - height = [m or ft]
  static void ConvertUnitStyleToDifferent(const units::UnitSystem& system,
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

 private:
  /// \brief Converts to 'consistent' unit style using version 1.
  /// \param[in] system
  ///   The unit system.
  /// \param[in] is_recursive
  ///   An indicator that determines if member variable converters are invoked.
  /// \param[in,out] structure
  ///   The structure to be converted.
  static void ConvertUnitStyleToConsistentV1(const units::UnitSystem& system,
                                             const bool& is_recursive,
                                             Structure& structure);
};

#endif  // APPCOMMON_UNITS_STRUCTURE_UNIT_CONVERTER_H_
