// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef OTLS_APPCOMMON_UNITS_CABLECONSTRAINTUNITCONVERTER_H_
#define OTLS_APPCOMMON_UNITS_CABLECONSTRAINTUNITCONVERTER_H_

#include "models/base/units.h"
#include "models/transmissionline/cable_constraint.h"

/// \par OVERVIEW
///
/// This class converts a cable constraint between unit systems as well as unit
/// styles.
class CableConstraintUnitConverter {
 public:
  /// \brief Changes between unit styles.
  /// \param[in] system
  ///   The unit system.
  /// \param[in] style_from
  ///   The unit style to convert from.
  /// \param[in] style_to
  ///   The unit style to convert to.
  /// \param[in,out] constraint
  ///   The constraint to be converted.
  /// The 'different' style units are as follows:
  ///  - limit (kHorizontalTension) = [N or lb]
  ///  - limit (kCatenaryConstant) = [N/m or lb/ft]
  ///  - limit (kSag) = [m or ft]
  ///  - limit (kSupport Tension) = [N or lb]
  /// The following variables are not owned, and will be skipped:
  ///  - case_weather
  static void ConvertUnitStyle(const units::UnitSystem& system,
                               const units::UnitStyle& style_from,
                               const units::UnitStyle& style_to,
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
};

#endif  // OTLS_APPCOMMON_UNITS_CABLECONSTRAINTUNITCONVERTER_H_
