// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "appcommon/units/line_cable_unit_converter.h"

#include "appcommon/units/cable_constraint_unit_converter.h"

void LineCableUnitConverter::ConvertUnitStyle(
    const units::UnitSystem& system,
    const units::UnitStyle& style_from,
    const units::UnitStyle& style_to,
    const bool& is_recursive,
    LineCable& line_cable) {
  if (style_from == style_to) {
    return;
  }

  if (system == units::UnitSystem::kImperial) {
    // nothing to do
  } else if (system == units::UnitSystem::kMetric) {
    // nothing to do
  }

  // triggers member variable converters
  if (is_recursive == true) {
    // converts constraint
    CableConstraint constraint = line_cable.constraint();
    CableConstraintUnitConverter::ConvertUnitStyle(system, style_from, style_to,
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
