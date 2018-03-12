// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "appcommon/units/structure_unit_converter.h"

void StructureAttachmentUnitConverter::ConvertUnitStyle(
    const units::UnitSystem& system,
    const units::UnitStyle& style_from,
    const units::UnitStyle& style_to,
    StructureAttachment& /**attachment**/) {
  if (style_from == style_to) {
    return;
  }

  if (system == units::UnitSystem::kImperial) {
    // nothing to do - unit styles match
  } else if (system == units::UnitSystem::kMetric) {
    // nothing to do - unit styles match
  }
}

void StructureAttachmentUnitConverter::ConvertUnitSystem(
    const units::UnitSystem& system_from,
    const units::UnitSystem& system_to,
    StructureAttachment& attachment) {
  if (system_from == system_to) {
    return;
  }

  if (system_to == units::UnitSystem::kMetric) {
    attachment.offset_longitudinal = units::ConvertLength(
        attachment.offset_longitudinal,
        units::LengthConversionType::kFeetToMeters);

    attachment.offset_transverse = units::ConvertLength(
        attachment.offset_transverse,
        units::LengthConversionType::kFeetToMeters);

    attachment.offset_vertical_top = units::ConvertLength(
        attachment.offset_vertical_top,
        units::LengthConversionType::kFeetToMeters);
  } else if (system_to == units::UnitSystem::kImperial) {
    attachment.offset_longitudinal = units::ConvertLength(
        attachment.offset_longitudinal,
        units::LengthConversionType::kMetersToFeet);

    attachment.offset_transverse = units::ConvertLength(
        attachment.offset_transverse,
        units::LengthConversionType::kMetersToFeet);

    attachment.offset_vertical_top = units::ConvertLength(
        attachment.offset_vertical_top,
        units::LengthConversionType::kMetersToFeet);
  }
}


void StructureUnitConverter::ConvertUnitStyle(
    const units::UnitSystem& system,
    const units::UnitStyle& style_from,
    const units::UnitStyle& style_to,
    Structure& /**structure**/) {
  if (style_from == style_to) {
    return;
  }

  if (system == units::UnitSystem::kImperial) {
    // nothing to do - unit styles match
  } else if (system == units::UnitSystem::kMetric) {
    // nothing to do - unit styles match
  }
}

void StructureUnitConverter::ConvertUnitSystem(
    const units::UnitSystem& system_from,
    const units::UnitSystem& system_to,
    Structure& structure) {
  if (system_from == system_to) {
    return;
  }

  // converts unit system for structure
  if (system_to == units::UnitSystem::kMetric) {
    structure.height = units::ConvertLength(
        structure.height,
        units::LengthConversionType::kFeetToMeters);
  } else if (system_to == units::UnitSystem::kImperial) {
    structure.height = units::ConvertLength(
        structure.height,
        units::LengthConversionType::kMetersToFeet);
  }

  // converts unit system for attachments
  for (auto iter = structure.attachments.begin();
       iter != structure.attachments.end(); iter++) {
    StructureAttachment& attachment = *iter;
    StructureAttachmentUnitConverter::ConvertUnitSystem(system_from, system_to,
                                                        attachment);
  }
}
