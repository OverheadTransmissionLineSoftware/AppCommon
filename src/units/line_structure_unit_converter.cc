// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "appcommon/units/line_structure_unit_converter.h"

#include "wx/wx.h"

bool LineStructureUnitConverter::ConvertUnitStyleToConsistent(
    const int& version,
    const units::UnitSystem& system,
    LineStructure& line_structure) {
  bool status = true;

  // sends to proper converter function
  if (version == 0) {
    // points to latest converter version
    ConvertUnitStyleToConsistentV1(system, line_structure);
  } else if (version == 1) {
    ConvertUnitStyleToConsistentV1(system, line_structure);
  } else {
    wxString message = " Invalid version number. Aborting conversion.";
    wxLogError(message);
    status = false;
  }

  return status;
}

void LineStructureUnitConverter::ConvertUnitStyleToDifferent(
    const units::UnitSystem& system,
    LineStructure& /**line_structure**/) {
  if (system == units::UnitSystem::kImperial) {
    // nothing to do - unit styles match
  } else if (system == units::UnitSystem::kMetric) {
    // nothing to do - unit styles match
  }
}

void LineStructureUnitConverter::ConvertUnitSystem(
    const units::UnitSystem& system_from,
    const units::UnitSystem& system_to,
    LineStructure& line_structure) {
  if (system_from == system_to) {
    return;
  }

  if (system_to == units::UnitSystem::kMetric) {
    line_structure.set_height_adjustment(units::ConvertLength(
        line_structure.height_adjustment(),
        units::LengthConversionType::kFeetToMeters));

    line_structure.set_offset(units::ConvertLength(
        line_structure.offset(),
        units::LengthConversionType::kFeetToMeters));

    line_structure.set_station(units::ConvertLength(
        line_structure.station(),
        units::LengthConversionType::kFeetToMeters));

  } else if (system_to == units::UnitSystem::kImperial) {
    line_structure.set_height_adjustment(units::ConvertLength(
        line_structure.height_adjustment(),
        units::LengthConversionType::kMetersToFeet));

    line_structure.set_offset(units::ConvertLength(
        line_structure.offset(),
        units::LengthConversionType::kMetersToFeet));

    line_structure.set_station(units::ConvertLength(
        line_structure.station(),
        units::LengthConversionType::kMetersToFeet));
  }
}

void LineStructureUnitConverter::ConvertUnitStyleToConsistentV1(
    const units::UnitSystem& system,
    LineStructure& /**line_structure**/) {
  if (system == units::UnitSystem::kImperial) {
    // nothing to do - unit styles match
  } else if (system == units::UnitSystem::kMetric) {
    // nothing to do - unit styles match
  }
}
