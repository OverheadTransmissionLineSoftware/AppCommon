// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "appcommon/units/cable_constraint_unit_converter.h"

#include "wx/wx.h"

bool CableConstraintUnitConverter::ConvertUnitStyleToConsistent(
    const int& version,
    const units::UnitSystem& system,
    CableConstraint& constraint) {
  bool status = true;

  // sends to proper converter function
  if (version == 0) {
    // points to latest converter version
    ConvertUnitStyleToConsistentV1(system, constraint);
  } else if (version == 1) {
    ConvertUnitStyleToConsistentV1(system, constraint);
  } else {
    wxString message = " Invalid version number. Aborting conversion.";
    wxLogError(message);
    status = false;
  }

  return status;
}

void CableConstraintUnitConverter::ConvertUnitStyleToDifferent(
    const units::UnitSystem& system,
    CableConstraint& /**constraint**/) {
  if (system == units::UnitSystem::kImperial) {
    // nothing to do - unit styles match
  } else if (system == units::UnitSystem::kMetric) {
    // nothing to do - unit styles match
  }
}

void CableConstraintUnitConverter::ConvertUnitSystem(
    const units::UnitSystem& system_from,
    const units::UnitSystem& system_to,
    CableConstraint& constraint) {
  if (system_from == system_to) {
    return;
  }

  if (system_to == units::UnitSystem::kMetric) {
    if (constraint.type_limit ==
          CableConstraint::LimitType::kCatenaryConstant) {
      constraint.limit = units::ConvertForce(
          constraint.limit,
          units::ForceConversionType::kPoundsToNewtons);
      constraint.limit = units::ConvertLength(
          constraint.limit,
          units::LengthConversionType::kFeetToMeters,
          1,
          false);
    } else if (constraint.type_limit ==
          CableConstraint::LimitType::kHorizontalTension) {
      constraint.limit = units::ConvertForce(
          constraint.limit,
          units::ForceConversionType::kPoundsToNewtons);
    } else if (constraint.type_limit == CableConstraint::LimitType::kLength) {
      constraint.limit = units::ConvertLength(
          constraint.limit,
          units::LengthConversionType::kFeetToMeters,
          1,
          false);
    } else if (constraint.type_limit == CableConstraint::LimitType::kSag) {
      constraint.limit = units::ConvertLength(
          constraint.limit,
          units::LengthConversionType::kFeetToMeters,
          1,
          false);
    } else if (constraint.type_limit ==
          CableConstraint::LimitType::kSupportTension) {
      constraint.limit = units::ConvertForce(
          constraint.limit,
          units::ForceConversionType::kPoundsToNewtons);
    }

  } else if (system_to == units::UnitSystem::kImperial) {
    if (constraint.type_limit ==
          CableConstraint::LimitType::kCatenaryConstant) {
      constraint.limit = units::ConvertForce(
          constraint.limit,
          units::ForceConversionType::kNewtonsToPounds);
      constraint.limit = units::ConvertLength(
          constraint.limit,
          units::LengthConversionType::kMetersToFeet,
          1,
          false);
    } else if (constraint.type_limit ==
          CableConstraint::LimitType::kHorizontalTension) {
      constraint.limit = units::ConvertForce(
          constraint.limit,
          units::ForceConversionType::kNewtonsToPounds);
    } else if (constraint.type_limit == CableConstraint::LimitType::kLength) {
      constraint.limit = units::ConvertLength(
          constraint.limit,
          units::LengthConversionType::kMetersToFeet,
          1,
          false);
    } else if (constraint.type_limit == CableConstraint::LimitType::kSag) {
      constraint.limit = units::ConvertLength(
          constraint.limit,
          units::LengthConversionType::kMetersToFeet,
          1,
          false);
    } else if (constraint.type_limit ==
          CableConstraint::LimitType::kSupportTension) {
      constraint.limit = units::ConvertForce(
          constraint.limit,
          units::ForceConversionType::kNewtonsToPounds);
    }
  }
}

void CableConstraintUnitConverter::ConvertUnitStyleToConsistentV1(
    const units::UnitSystem& system,
    CableConstraint& /**constraint**/) {
  if (system == units::UnitSystem::kImperial) {
    // nothing to do - unit styles match
  } else if (system == units::UnitSystem::kMetric) {
    // nothing to do - unit styles match
  }
}
