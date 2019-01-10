// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef APPCOMMON_UNITS_CABLE_CONSTRAINT_UNIT_CONVERTER_H_
#define APPCOMMON_UNITS_CABLE_CONSTRAINT_UNIT_CONVERTER_H_

#include "models/base/units.h"
#include "models/transmissionline/cable_constraint.h"

/// \par OVERVIEW
///
/// This class converts a cable constraint between unit systems as well as unit
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
class CableConstraintUnitConverter {
 public:
  /// \brief Converts to 'consistent' unit style.
  /// \param[in] version
  ///   The version. 0 (zero) indicates the latest version.
  /// \param[in] system
  ///   The unit system.
  /// \param[in,out] constraint
  ///   The constraint to be converted.
  /// \return The success status.
  /// The following variables are not owned, and will be skipped:
  ///  - case_weather
  static bool ConvertUnitStyleToConsistent(const int& version,
                                           const units::UnitSystem& system,
                                           CableConstraint& constraint);

  /// \brief Converts to 'different' unit style.
  /// \param[in] system
  ///   The unit system.
  /// \param[in,out] constraint
  ///   The constraint to be converted.
  /// The 'different' style units are as follows:
  ///  - limit (kHorizontalTension) = [N or lb]
  ///  - limit (kCatenaryConstant) = [N/m or lb/ft]
  ///  - limit (kSag) = [m or ft]
  ///  - limit (kSupport Tension) = [N or lb]
  /// The following variables are not owned, and will be skipped:
  ///  - case_weather
  static void ConvertUnitStyleToDifferent(const units::UnitSystem& system,
                                          CableConstraint& constraint);

  /// \brief Changes between unit systems.
  /// \param[in] system_from
  ///   The unit system to convert from. These must be consistent style units.
  /// \param[in] system_to
  ///   The unit system to convert to. These will also be in consistent style
  ///   units.
  /// \param[in,out] constraint
  ///   The constraint to be converted.
  /// The following variables are not owned, and will be skipped:
  ///  - case_weather
  /// This function requires that the constraint be in a 'consistent' unit
  /// style.
  static void ConvertUnitSystem(const units::UnitSystem& system_from,
                                const units::UnitSystem& system_to,
                                CableConstraint& constraint);

 private:
  /// \brief Converts to 'consistent' unit style using version 1.
  /// \param[in] system
  ///   The unit system.
  /// \param[in,out] constraint
  ///   The constraint to be converted.
  /// The following variables are not owned, and will be skipped:
  ///  - case_weather
  static void ConvertUnitStyleToConsistentV1(const units::UnitSystem& system,
                                             CableConstraint& constraint);
};

#endif  // APPCOMMON_UNITS_CABLE_CONSTRAINT_UNIT_CONVERTER_H_
