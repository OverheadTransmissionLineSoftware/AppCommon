// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "appcommon/units/structure_unit_converter.h"

#include "wx/wx.h"

bool StructureAttachmentUnitConverter::ConvertUnitStyleToConsistent(
    const int& version,
    const units::UnitSystem& system,
    StructureAttachment& attachment) {
  bool status = true;

  // sends to proper converter function
  if (version == 0) {
    // points to latest converter version
    ConvertUnitStyleToConsistentV1(system, attachment);
  } else if (version == 1) {
    ConvertUnitStyleToConsistentV1(system, attachment);
  } else {
    wxString message = " Invalid version number. Aborting conversion.";
    wxLogError(message);
    status = false;
  }

  return status;
}

void StructureAttachmentUnitConverter::ConvertUnitStyleToDifferent(
    const units::UnitSystem& system,
    StructureAttachment& /**attachment**/) {
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

void StructureAttachmentUnitConverter::ConvertUnitStyleToConsistentV1(
    const units::UnitSystem& system,
    StructureAttachment& /**attachment**/) {
  if (system == units::UnitSystem::kImperial) {
    // nothing to do - unit styles match
  } else if (system == units::UnitSystem::kMetric) {
    // nothing to do - unit styles match
  }
}


bool StructureUnitConverter::ConvertUnitStyleToConsistent(
    const int& version,
    const units::UnitSystem& system,
    const bool& is_recursive,
    Structure& structure) {
  bool status = true;

  // sends to proper converter function
  if (version == 0) {
    // points to latest converter version
    ConvertUnitStyleToConsistentV1(system, is_recursive, structure);
  } else if (version == 1) {
    ConvertUnitStyleToConsistentV1(system, is_recursive, structure);
  } else {
    wxString message = " Invalid version number. Aborting conversion.";
    wxLogError(message);
    status = false;
  }

  return status;
}

void StructureUnitConverter::ConvertUnitStyleToDifferent(
    const units::UnitSystem& system,
    const bool& is_recursive,
    Structure& structure) {
  if (system == units::UnitSystem::kImperial) {
    // nothing to do - unit styles match
  } else if (system == units::UnitSystem::kMetric) {
    // nothing to do - unit styles match
  }

  // triggers member variable converters
  if (is_recursive == true) {
    // converts attachments
    for (auto iter = structure.attachments.begin();
         iter != structure.attachments.end(); iter++) {
      StructureAttachment& attachment = *iter;
      StructureAttachmentUnitConverter::ConvertUnitStyleToDifferent(
          system,
          attachment);
    }
  }
}

void StructureUnitConverter::ConvertUnitSystem(
    const units::UnitSystem& system_from,
    const units::UnitSystem& system_to,
    const bool& is_recursive,
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

  // triggers member variable converters
  if (is_recursive == true) {
    // converts unit system for attachments
    for (auto iter = structure.attachments.begin();
         iter != structure.attachments.end(); iter++) {
      StructureAttachment& attachment = *iter;
      StructureAttachmentUnitConverter::ConvertUnitSystem(system_from,
                                                          system_to,
                                                          attachment);
    }
  }
}

void StructureUnitConverter::ConvertUnitStyleToConsistentV1(
    const units::UnitSystem& system,
    const bool& is_recursive,
    Structure& structure) {
  if (system == units::UnitSystem::kImperial) {
    // nothing to do - unit styles match
  } else if (system == units::UnitSystem::kMetric) {
    // nothing to do - unit styles match
  }

  // triggers member variable converters
  if (is_recursive == true) {
    // converts attachments
    for (auto iter = structure.attachments.begin();
         iter != structure.attachments.end(); iter++) {
      StructureAttachment& attachment = *iter;
      StructureAttachmentUnitConverter::ConvertUnitStyleToConsistent(
          0,
          system,
          attachment);
    }
  }
}
