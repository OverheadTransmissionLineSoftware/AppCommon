// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "appcommon/units/line_cable_unit_converter.h"

#include "wx/wx.h"

#include "appcommon/units/cable_constraint_unit_converter.h"

bool LineCableUnitConverter::ConvertUnitStyleToConsistent(
    const int& version,
    const units::UnitSystem& system,
    const bool& is_recursive,
    LineCable& line_cable) {
  bool status = true;

  // sends to proper converter function
  if (version == 0) {
    // points to latest converter version
    ConvertUnitStyleToConsistentV1(system, is_recursive, line_cable);
  } else if (version == 1) {
    ConvertUnitStyleToConsistentV1(system, is_recursive, line_cable);
  } else {
    wxString message = " Invalid version number. Aborting conversion.";
    wxLogError(message);
    status = false;
  }

  return status;
}

void LineCableUnitConverter::ConvertUnitStyleToDifferent(
    const units::UnitSystem& system,
    const bool& is_recursive,
    LineCable& line_cable) {
  if (system == units::UnitSystem::kImperial) {
    // nothing to do
  } else if (system == units::UnitSystem::kMetric) {
    // nothing to do
  }

  // triggers member variable converters
  if (is_recursive == true) {
    // converts constraint
    CableConstraint constraint = line_cable.constraint();
    CableConstraintUnitConverter::ConvertUnitStyleToDifferent(
        system,
        constraint);
    line_cable.set_constraint(constraint);
  }
}

void LineCableUnitConverter::ConvertUnitSystem(
    const units::UnitSystem& system_from,
    const units::UnitSystem& system_to,
    const bool& is_recursive,
    LineCable& line_cable) {
  if (system_from == system_to) {
    return;
  }

  if (system_to == units::UnitSystem::kMetric) {
    // converts ruling span attachment spacing
    Vector3d spacing = line_cable.spacing_attachments_ruling_span();
    spacing.set_x(units::ConvertLength(
        spacing.x(),
        units::LengthConversionType::kFeetToMeters));
    spacing.set_y(units::ConvertLength(
        spacing.y(),
        units::LengthConversionType::kFeetToMeters));
    spacing.set_z(units::ConvertLength(
        spacing.z(),
        units::LengthConversionType::kFeetToMeters));
    line_cable.set_spacing_attachments_ruling_span(spacing);

  } else if (system_to == units::UnitSystem::kImperial) {
    // converts ruling span attachment spacing
    Vector3d spacing = line_cable.spacing_attachments_ruling_span();
    spacing.set_x(units::ConvertLength(
        spacing.x(),
        units::LengthConversionType::kMetersToFeet));
    spacing.set_y(units::ConvertLength(
        spacing.y(),
        units::LengthConversionType::kMetersToFeet));
    spacing.set_z(units::ConvertLength(
        spacing.z(),
        units::LengthConversionType::kMetersToFeet));
    line_cable.set_spacing_attachments_ruling_span(spacing);
  }

  // triggers member variable converters
  if (is_recursive == true) {
    // converts constraint
    CableConstraint constraint = line_cable.constraint();
    CableConstraintUnitConverter::ConvertUnitSystem(system_from,
                                                    system_to,
                                                    constraint);
    line_cable.set_constraint(constraint);
  }
}

void LineCableUnitConverter::ConvertUnitStyleToConsistentV1(
    const units::UnitSystem& system,
    const bool& is_recursive,
    LineCable& line_cable) {
  if (system == units::UnitSystem::kImperial) {
    // nothing to do
  } else if (system == units::UnitSystem::kMetric) {
    // nothing to do
  }

  // triggers member variable converters
  if (is_recursive == true) {
    // converts constraint
    CableConstraint constraint = line_cable.constraint();
    CableConstraintUnitConverter::ConvertUnitStyleToConsistent(
        0,
        system,
        constraint);
    line_cable.set_constraint(constraint);
  }
}
