// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "appcommon/units/transmission_line_unit_converter.h"

#include "appcommon/units/line_cable_unit_converter.h"
#include "appcommon/units/line_structure_unit_converter.h"
#include "models/base/units.h"
#include "wx/wx.h"

bool TransmissionLineUnitConverter::ConvertUnitStyleToConsistent(
    const int& version,
    const units::UnitSystem& system,
    const bool& is_recursive,
    TransmissionLine& line) {
  bool status = true;

  // sends to proper converter function
  if (version == 0) {
    // points to latest converter version
    ConvertUnitStyleToConsistentV1(system, is_recursive, line);
  } else if (version == 1) {
    ConvertUnitStyleToConsistentV1(system, is_recursive, line);
  } else {
    wxString message = " Invalid version number. Aborting conversion.";
    wxLogError(message);
    status = false;
  }

  return status;
}

void TransmissionLineUnitConverter::ConvertUnitStyleToDifferent(
    const units::UnitSystem& system,
    const bool& is_recursive,
    TransmissionLine& line) {
  // station values don't change, so order of unit conversions isn't important
  int index = -9999;

  // alignment always uses consistent units - no unit style change required

  // triggers member variable converters
  if (is_recursive == true) {
    // converts line structures
    index = 0;
    const int kSizeLineStructures = line.line_structures()->size();
    while (index < kSizeLineStructures) {
      LineStructure line_structure =
          *std::next(line.line_structures()->cbegin(), index);
      LineStructureUnitConverter::ConvertUnitStyleToDifferent(
          system,
          line_structure);
      line.ModifyLineStructure(index, line_structure);

      index++;
    }

    // converts line cables
    index = 0;
    const int kSizeLineCables = line.line_cables()->size();
    while (index < kSizeLineCables) {
      LineCable line_cable = *std::next(line.line_cables()->cbegin(), index);
      LineCableUnitConverter::ConvertUnitStyleToDifferent(
          system,
          is_recursive,
          line_cable);
      line.ModifyLineCable(index, line_cable);

      index++;
    }
  }
}

/// This function modifies the transmission line in a specific order, depending
/// on whether the maximum station value is increasing or decreasing.
void TransmissionLineUnitConverter::ConvertUnitSystem(
    const units::UnitSystem& system_from,
    const units::UnitSystem& system_to,
    const bool& is_recursive,
    TransmissionLine& line) {
  if (system_from == system_to) {
    return;
  }

  int index = -9999;

  if (system_to == units::UnitSystem::kMetric) {
    // max station value is decreasing
    // the unit conversions ordered such that the line structures remain on the
    // alignment and don't get deleted

    // triggers member variable converters
    if (is_recursive == true) {
      // converts line cables
      index = 0;
      const int kSizeLineCables = line.line_cables()->size();
      while (index < kSizeLineCables) {
        LineCable line_cable = *std::next(line.line_cables()->cbegin(), index);
        LineCableUnitConverter::ConvertUnitSystem(
            system_from,
            system_to,
            is_recursive,
            line_cable);
        line.ModifyLineCable(index, line_cable);

        index++;
      }

      // converts line structures
      index = 0;
      const int kSizeLineStructures = line.line_structures()->size();
      while (index < kSizeLineStructures) {
        LineStructure line_structure =
            *std::next(line.line_structures()->cbegin(), index);
        LineStructureUnitConverter::ConvertUnitSystem(
            system_from,
            system_to,
            line_structure);
        line.ModifyLineStructure(index, line_structure);

        index++;
      }
    }

    // converts alignment points
    index = 0;
    const int kSizeAlignment = line.alignment()->points()->size();
    while (index < kSizeAlignment) {
      AlignmentPoint point = *std::next(line.alignment()->points()->cbegin(),
                                        index);
      point.elevation = units::ConvertLength(
          point.elevation,
          units::LengthConversionType::kFeetToMeters);

      point.station = units::ConvertLength(
          point.station,
          units::LengthConversionType::kFeetToMeters);

      line.ModifyAlignmentPoint(index, point);

      index++;
    }

  } else if (system_to == units::UnitSystem::kImperial) {
    // max station value is increasing
    // the unit conversions ordered such that the line structures remain on the
    // alignment and don't get deleted

    // converts alignment points
    index = line.alignment()->points()->size() - 1;
    while (0 <= index) {
      AlignmentPoint point = *std::next(line.alignment()->points()->cbegin(),
                                        index);
      point.elevation = units::ConvertLength(
          point.elevation,
          units::LengthConversionType::kMetersToFeet);

      point.station = units::ConvertLength(
          point.station,
          units::LengthConversionType::kMetersToFeet);

      line.ModifyAlignmentPoint(index, point);

      index--;
    }

    // triggers member variable converters
    if (is_recursive == true) {
      // converts line structures
      index = line.line_structures()->size() - 1;
      while (0 <= index) {
        LineStructure line_structure = *std::next(
            line.line_structures()->cbegin(),
            index);
        LineStructureUnitConverter::ConvertUnitSystem(system_from, system_to,
                                                      line_structure);
        line.ModifyLineStructure(index, line_structure);

        index--;
      }

      // converts line cables
      index = line.line_cables()->size() - 1;
      while (0 <= index) {
        LineCable line_cable = *std::next(line.line_cables()->cbegin(), index);
        LineCableUnitConverter::ConvertUnitSystem(system_from, system_to,
                                                  is_recursive, line_cable);
        line.ModifyLineCable(index, line_cable);

        index--;
      }
    }
  }
}

void TransmissionLineUnitConverter::ConvertUnitStyleToConsistentV1(
    const units::UnitSystem& system,
    const bool& is_recursive,
    TransmissionLine& line) {
  // station values don't change, so order of unit conversions isn't important
  int index = -9999;

  // alignment always uses consistent units - no unit style change required

  // triggers member variable converters
  if (is_recursive == true) {
    // converts line structures
    index = 0;
    const int kSizeLineStructures = line.line_structures()->size();
    while (index < kSizeLineStructures) {
      LineStructure line_structure = *std::next(line.line_structures()->cbegin(),
                                                index);
      LineStructureUnitConverter::ConvertUnitStyleToConsistent(
          0,
          system,
          line_structure);
      line.ModifyLineStructure(index, line_structure);

      index++;
    }

    // converts line cables
    index = 0;
    const int kSizeLineCables = line.line_cables()->size();
    while (index < kSizeLineCables) {
      LineCable line_cable = *std::next(line.line_cables()->cbegin(), index);
      LineCableUnitConverter::ConvertUnitStyleToConsistent(
          0,
          system,
          is_recursive,
          line_cable);
      line.ModifyLineCable(index, line_cable);

      index++;
    }
  }
}
