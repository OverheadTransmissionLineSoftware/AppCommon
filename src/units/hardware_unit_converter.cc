// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "appcommon/units/hardware_unit_converter.h"

#include "wx/wx.h"

bool HardwareUnitConverter::ConvertUnitStyleToConsistent(
    const int& version,
    const units::UnitSystem& system,
    Hardware& hardware) {
  bool status = true;

  // sends to proper converter function
  if (version == 0) {
    // points to latest converter version
    ConvertUnitStyleToConsistentV1(system, hardware);
  } else if (version == 1) {
    ConvertUnitStyleToConsistentV1(system, hardware);
  } else {
    wxString message = " Invalid version number. Aborting conversion.";
    wxLogError(message);
    status = false;
  }

  return status;
}

void HardwareUnitConverter::ConvertUnitStyleToDifferent(
    const units::UnitSystem& system,
    Hardware& /**hardware**/) {
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

void HardwareUnitConverter::ConvertUnitStyleToConsistentV1(
    const units::UnitSystem& system,
    Hardware& /**hardware**/) {
  if (system == units::UnitSystem::kImperial) {
    // nothing to do - unit styles match
  } else if (system == units::UnitSystem::kMetric) {
    // nothing to do - unit styles match
  }
}
