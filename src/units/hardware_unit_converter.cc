// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "appcommon/units/hardware_unit_converter.h"

void HardwareUnitConverter::ConvertUnitStyle(
    const units::UnitSystem& system,
    const units::UnitStyle& style_from,
    const units::UnitStyle& style_to,
    Hardware& /**hardware**/) {
  if (style_from == style_to) {
    return;
  }

  if (system == units::UnitSystem::kImperial) {
    // nothing to do - unit styles match
  } else if (system == units::UnitSystem::kMetric) {
    // nothing to do - unit styles match
  }
}

void HardwareUnitConverter::ConvertUnitSystem(
    const units::UnitSystem& system_from,
    const units::UnitSystem& system_to,
    Hardware& hardware) {
  if (system_from == system_to) {
    return;
  }

  if (system_to == units::UnitSystem::kMetric) {
    hardware.area_cross_section = units::ConvertLength(
        hardware.area_cross_section,
        units::LengthConversionType::kFeetToMeters,
        2);

    hardware.length = units::ConvertLength(
        hardware.length,
        units::LengthConversionType::kFeetToMeters);

    hardware.weight = units::ConvertForce(
        hardware.weight,
        units::ForceConversionType::kPoundsToNewtons);
  } else if (system_to == units::UnitSystem::kImperial) {
    hardware.area_cross_section = units::ConvertLength(
        hardware.area_cross_section,
        units::LengthConversionType::kMetersToFeet,
        2);

    hardware.length = units::ConvertLength(
        hardware.length,
        units::LengthConversionType::kMetersToFeet);

    hardware.weight = units::ConvertForce(
        hardware.weight,
        units::ForceConversionType::kNewtonsToPounds);
  }
}
