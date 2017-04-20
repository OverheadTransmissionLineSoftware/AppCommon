// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "appcommon/units/transmission_line_unit_converter.h"

#include "appcommon/units/line_cable_unit_converter.h"
#include "appcommon/units/line_structure_unit_converter.h"
#include "models/base/units.h"

void TransmissionLineUnitConverter::ConvertUnitStyle(
    const units::UnitSystem& system,
    const units::UnitStyle& style_from,
    const units::UnitStyle& style_to,
    TransmissionLine& line) {
  if (style_from == style_to) {
    return;
  }

  // station values don't change, so order of unit conversions isn't important
  int index = -9999;

  // modifies line structures
  index = 0;
  for (auto iter = line.line_structures()->cbegin();
       iter != line.line_structures()->cend(); iter++) {
    LineStructure line_structure = *iter;
    LineStructureUnitConverter::ConvertUnitStyle(system, style_from, style_to,
                                                 line_structure);
    line.ModifyLineStructure(index, line_structure);

    index++;
  }

  // modifies line cables
  index = 0;
  for (auto iter = line.line_cables()->cbegin();
        iter != line.line_cables()->cend(); iter++) {
    LineCable line_cable = *iter;
    LineCableUnitConverter::ConvertUnitStyle(system, style_from, style_to,
                                             line_cable);
    line.ModifyLineCable(index, line_cable);

    index++;
  }
}

/// This function modifies the transmission line in a specific order, depending
/// on whether the maximum station value is increasing or decreasing.
void TransmissionLineUnitConverter::ConvertUnitSystem(
    const units::UnitSystem& system_from,
    const units::UnitSystem& system_to,
    TransmissionLine& line) {
  if (system_from == system_to) {
    return;
  }

  if (system_to == units::UnitSystem::kMetric) {
    // max station value is decreasing
    // the unit conversions ordered such that the line structures remain on the
    // alignment and don't get deleted

    // modifies line cables
    int index = line.line_structures()->size() - 1;
    for (auto iter = line.line_cables()->crbegin();
          iter != line.line_cables()->crend(); iter++) {
      LineCable line_cable = *iter;
      LineCableUnitConverter::ConvertUnitSystem(system_from, system_to,
                                                line_cable);
      line.ModifyLineCable(index, line_cable);

      index++;
    }

    // modifies line structures
    index = line.line_structures()->size() - 1;
    for (auto iter = line.line_structures()->cbegin();
         iter != line.line_structures()->cend(); iter++) {
      LineStructure line_structure = *iter;
      LineStructureUnitConverter::ConvertUnitSystem(system_from, system_to,
                                                    line_structure);
      line.ModifyLineStructure(index, line_structure);

      index++;
    }

    // modifies alignment points
    index = line.alignment()->points()->size() - 1;
    for (auto iter = line.alignment()->points()->crbegin();
         iter != line.alignment()->points()->crend(); iter++) {
      AlignmentPoint point = *iter;
      /// \todo implement alignment point conversions

      index++;
    }

  } else if (system_to == units::UnitSystem::kImperial) {
    // max station value is increasing
    // the unit conversions ordered such that the line structures remain on the
    // alignment and don't get deleted

    // modifies alignment points
    int index = line.alignment()->points()->size() - 1;
    for (auto iter = line.alignment()->points()->crbegin();
         iter != line.alignment()->points()->crend(); iter++) {
      AlignmentPoint point = *iter;
      /// \todo implement alignment point conversions

      index--;
    }

    // modifies line structures
    index = line.line_structures()->size() - 1;
    for (auto iter = line.line_structures()->crbegin();
         iter != line.line_structures()->crend(); iter++) {
      LineStructure line_structure = *iter;
      LineStructureUnitConverter::ConvertUnitSystem(system_from, system_to,
                                                    line_structure);
      line.ModifyLineStructure(index, line_structure);

      index--;
    }

    // modifies line cables
    index = line.line_structures()->size() - 1;
    for (auto iter = line.line_cables()->crbegin();
          iter != line.line_cables()->crend(); iter++) {
      LineCable line_cable = *iter;
      LineCableUnitConverter::ConvertUnitSystem(system_from, system_to,
                                                line_cable);
      line.ModifyLineCable(index, line_cable);

      index--;
    }
  }
}
