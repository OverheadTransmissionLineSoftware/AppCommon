// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "appcommon/units/line_structure_unit_converter.h"

void LineStructureUnitConverter::ConvertUnitStyle(
    const units::UnitSystem& system,
    const units::UnitStyle& style_from,
    const units::UnitStyle& style_to,
    const bool& /**is_recursive**/,
    LineStructure& /**line_structure**/) {
  if (style_from == style_to) {
    return;
  }

  if (system == units::UnitSystem::kImperial) {
    // nothing to do - unit styles match
  } else if (system == units::UnitSystem::kMetric) {
    // nothing to do - unit styles match
  }
}

void LineStructureUnitConverter::ConvertUnitSystem(
    const units::UnitSystem& system_from,
    const units::UnitSystem& system_to,
    const bool& /**is_recursive**/,
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
