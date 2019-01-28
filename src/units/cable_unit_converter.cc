// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "appcommon/units/cable_unit_converter.h"

#include "wx/wx.h"

namespace {

/// \brief Converts the component load coefficients.
/// \param[in] type
///   The type of force conversion.
/// \param[in,out] coefficients
///   The coefficients to be converted.
void ConvertComponentCoefficientsLoad(
    const units::ForceConversionType& type,
    std::vector<double>& coefficients) {
  // converts stress coefficients
  for (auto iter = coefficients.begin(); iter != coefficients.end(); iter ++) {
    double& coefficient = *iter;
    coefficient = units::ConvertForce(coefficient, type);
  }
}

/// \brief Converts the component stress coefficients.
/// \param[in] type
///   The type of stress conversion.
/// \param[in,out] coefficients
///   The coefficients to be converted.
void ConvertComponentCoefficientsStress(
    const units::StressConversionType& type,
    std::vector<double>& coefficients) {
  // converts stress coefficients
  for (auto iter = coefficients.begin(); iter != coefficients.end(); iter ++) {
    double& coefficient = *iter;
    coefficient = units::ConvertStress(coefficient, type);
  }
}

/// \brief Converts the component loads to virtual stresses. This is needed for
///   compatibility with industry-standard polynomials.
/// \param[in] area_virtual
///   The virtual area of the component when converting to stress.
/// \param[in,out] component
///   The component to be converted.
void ConvertComponentLoadToStress(
    const double& area_virtual,
    CableComponent& component) {
  for (auto iter = component.coefficients_polynomial_creep.begin();
       iter != component.coefficients_polynomial_creep.end(); iter++) {
    double& coefficient = *iter;
    coefficient = coefficient / area_virtual;
  }

  for (auto iter = component.coefficients_polynomial_loadstrain.begin();
        iter != component.coefficients_polynomial_loadstrain.end(); iter++) {
    double& coefficient = *iter;
    coefficient = coefficient / area_virtual;
  }

  component.load_limit_polynomial_creep =
      component.load_limit_polynomial_creep / area_virtual;
  component.load_limit_polynomial_loadstrain =
      component.load_limit_polynomial_loadstrain / area_virtual;
  component.modulus_compression_elastic_area =
      component.modulus_compression_elastic_area / area_virtual;
  component.modulus_tension_elastic_area =
      component.modulus_tension_elastic_area / area_virtual;
}

/// \brief Converts the component virtual stresses to loads. This is needed
///   for compatibility with industry-standard polynomials.
/// \param[in] area_virtual
///   The virtual area of the component when converting to stress.
/// \param[in,out] component
///   The component to be converted.
void ConvertComponentStressToLoad(
    const double& area_virtual,
    CableComponent& component) {
  for (auto iter = component.coefficients_polynomial_creep.begin();
       iter != component.coefficients_polynomial_creep.end(); iter++) {
    double& coefficient = *iter;
    coefficient = coefficient * area_virtual;
  }

  for (auto iter = component.coefficients_polynomial_loadstrain.begin();
        iter != component.coefficients_polynomial_loadstrain.end(); iter++) {
    double& coefficient = *iter;
    coefficient = coefficient * area_virtual;
  }

  component.load_limit_polynomial_creep =
      component.load_limit_polynomial_creep * area_virtual;
  component.load_limit_polynomial_loadstrain =
      component.load_limit_polynomial_loadstrain * area_virtual;
  component.modulus_compression_elastic_area =
      component.modulus_compression_elastic_area * area_virtual;
  component.modulus_tension_elastic_area =
      component.modulus_tension_elastic_area * area_virtual;
}

}  // namespace


bool CableComponentUnitConverter::ConvertUnitStyleToConsistent(
    const int& version,
    const units::UnitSystem& system,
    CableComponent& component) {
  bool status = true;

  // sends to proper converter function
  if (version == 0) {
    // points to latest converter version
    ConvertUnitStyleToConsistentV2(system, component);
  } else if (version == 1) {
    ConvertUnitStyleToConsistentV1(system, component);
  } else if (version == 2) {
    ConvertUnitStyleToConsistentV2(system, component);
  } else {
    wxString message = " Invalid version number. Aborting conversion.";
    wxLogError(message);
    status = false;
  }

  return status;
}

void CableComponentUnitConverter::ConvertUnitStyleToDifferent(
    const units::UnitSystem& system,
    CableComponent& component) {
  if (system == units::UnitSystem::kMetric) {
    component.coefficient_expansion_linear_thermal *= 100;

    ConvertComponentCoefficientsStress(
        units::StressConversionType::kPascalToMegaPascal,
        component.coefficients_polynomial_creep);

    ConvertComponentCoefficientsStress(
        units::StressConversionType::kPascalToMegaPascal,
        component.coefficients_polynomial_loadstrain);

    component.load_limit_polynomial_creep = units::ConvertStress(
        component.load_limit_polynomial_creep,
        units::StressConversionType::kPascalToMegaPascal);
    component.load_limit_polynomial_creep /= 100;

    component.load_limit_polynomial_loadstrain = units::ConvertStress(
        component.load_limit_polynomial_loadstrain,
        units::StressConversionType::kPascalToMegaPascal);
    component.load_limit_polynomial_loadstrain /= 100;

    component.modulus_compression_elastic_area = units::ConvertStress(
        component.modulus_compression_elastic_area,
        units::StressConversionType::kPascalToMegaPascal);
    component.modulus_compression_elastic_area /= 100;

    component.modulus_tension_elastic_area = units::ConvertStress(
        component.modulus_tension_elastic_area,
        units::StressConversionType::kPascalToMegaPascal);
    component.modulus_tension_elastic_area /= 100;
  } else if (system == units::UnitSystem::kImperial) {
    component.coefficient_expansion_linear_thermal *= 100;

    ConvertComponentCoefficientsStress(
        units::StressConversionType::kPsfToPsi,
        component.coefficients_polynomial_creep);

    ConvertComponentCoefficientsStress(
        units::StressConversionType::kPsfToPsi,
        component.coefficients_polynomial_loadstrain);

    component.load_limit_polynomial_creep = units::ConvertStress(
        component.load_limit_polynomial_creep,
        units::StressConversionType::kPsfToPsi);
    component.load_limit_polynomial_creep /= 100;

    component.load_limit_polynomial_loadstrain = units::ConvertStress(
        component.load_limit_polynomial_loadstrain,
        units::StressConversionType::kPsfToPsi);
    component.load_limit_polynomial_loadstrain /= 100;

    component.modulus_compression_elastic_area = units::ConvertStress(
        component.modulus_compression_elastic_area,
        units::StressConversionType::kPsfToPsi);
    component.modulus_compression_elastic_area /= 100;

    component.modulus_tension_elastic_area = units::ConvertStress(
        component.modulus_tension_elastic_area,
        units::StressConversionType::kPsfToPsi);
    component.modulus_tension_elastic_area /= 100;
  }
}

void CableComponentUnitConverter::ConvertUnitSystem(
    const units::UnitSystem& system_from,
    const units::UnitSystem& system_to,
    CableComponent& component) {
  if (system_from == system_to) {
    return;
  }

  // converts unit system for cable components
  if (system_to == units::UnitSystem::kMetric) {
    component.capacity_heat = units::ConvertLength(
      component.capacity_heat,
      units::LengthConversionType::kFeetToMeters, 1, false);
    component.capacity_heat = units::ConvertTemperature(
      component.capacity_heat,
      units::TemperatureConversionType::kRankineToKelvin, 1, false);

    component.coefficient_expansion_linear_thermal = units::ConvertTemperature(
      component.coefficient_expansion_linear_thermal,
      units::TemperatureConversionType::kRankineToKelvin, 1, false);

    ConvertComponentCoefficientsLoad(
        units::ForceConversionType::kPoundsToNewtons,
        component.coefficients_polynomial_creep);

    ConvertComponentCoefficientsLoad(
        units::ForceConversionType::kPoundsToNewtons,
        component.coefficients_polynomial_loadstrain);

    component.load_limit_polynomial_creep = units::ConvertForce(
        component.load_limit_polynomial_creep,
        units::ForceConversionType::kPoundsToNewtons);

    component.load_limit_polynomial_loadstrain = units::ConvertForce(
        component.load_limit_polynomial_loadstrain,
        units::ForceConversionType::kPoundsToNewtons);

    component.modulus_compression_elastic_area = units::ConvertForce(
        component.modulus_compression_elastic_area,
        units::ForceConversionType::kPoundsToNewtons);

    component.modulus_tension_elastic_area = units::ConvertForce(
        component.modulus_tension_elastic_area,
        units::ForceConversionType::kPoundsToNewtons);
  } else if (system_to == units::UnitSystem::kImperial) {
    component.capacity_heat = units::ConvertLength(
      component.capacity_heat,
      units::LengthConversionType::kMetersToFeet, 1, false);
    component.capacity_heat = units::ConvertTemperature(
      component.capacity_heat,
      units::TemperatureConversionType::kKelvinToRankine, 1, false);

    component.coefficient_expansion_linear_thermal = units::ConvertTemperature(
      component.coefficient_expansion_linear_thermal,
      units::TemperatureConversionType::kKelvinToRankine, 1, false);

    ConvertComponentCoefficientsLoad(
        units::ForceConversionType::kNewtonsToPounds,
        component.coefficients_polynomial_creep);

    ConvertComponentCoefficientsLoad(
        units::ForceConversionType::kNewtonsToPounds,
        component.coefficients_polynomial_loadstrain);

    component.load_limit_polynomial_creep = units::ConvertForce(
        component.load_limit_polynomial_creep,
        units::ForceConversionType::kNewtonsToPounds);

    component.load_limit_polynomial_loadstrain = units::ConvertForce(
        component.load_limit_polynomial_loadstrain,
        units::ForceConversionType::kNewtonsToPounds);

    component.modulus_compression_elastic_area = units::ConvertForce(
        component.modulus_compression_elastic_area,
        units::ForceConversionType::kNewtonsToPounds);

    component.modulus_tension_elastic_area = units::ConvertForce(
        component.modulus_tension_elastic_area,
        units::ForceConversionType::kNewtonsToPounds);
  }
}

void CableComponentUnitConverter::ConvertUnitStyleToConsistentV1(
    const units::UnitSystem& system,
    CableComponent& component) {
  if (system == units::UnitSystem::kMetric) {
    ConvertComponentCoefficientsStress(
        units::StressConversionType::kMegaPascalToPascal,
        component.coefficients_polynomial_creep);

    ConvertComponentCoefficientsStress(
        units::StressConversionType::kMegaPascalToPascal,
        component.coefficients_polynomial_loadstrain);

    component.load_limit_polynomial_creep = units::ConvertStress(
        component.load_limit_polynomial_creep,
        units::StressConversionType::kMegaPascalToPascal);

    component.load_limit_polynomial_loadstrain = units::ConvertStress(
        component.load_limit_polynomial_loadstrain,
        units::StressConversionType::kMegaPascalToPascal);

    component.modulus_compression_elastic_area = units::ConvertStress(
        component.modulus_compression_elastic_area,
        units::StressConversionType::kMegaPascalToPascal);

    component.modulus_tension_elastic_area = units::ConvertStress(
        component.modulus_tension_elastic_area,
        units::StressConversionType::kMegaPascalToPascal);
  } else if (system == units::UnitSystem::kImperial) {
    ConvertComponentCoefficientsStress(
        units::StressConversionType::kPsiToPsf,
        component.coefficients_polynomial_creep);

    ConvertComponentCoefficientsStress(
        units::StressConversionType::kPsiToPsf,
        component.coefficients_polynomial_loadstrain);

    component.load_limit_polynomial_creep = units::ConvertStress(
        component.load_limit_polynomial_creep,
        units::StressConversionType::kPsiToPsf);

    component.load_limit_polynomial_loadstrain = units::ConvertStress(
        component.load_limit_polynomial_loadstrain,
        units::StressConversionType::kPsiToPsf);

    component.modulus_compression_elastic_area = units::ConvertStress(
        component.modulus_compression_elastic_area,
        units::StressConversionType::kPsiToPsf);

    component.modulus_tension_elastic_area = units::ConvertStress(
        component.modulus_tension_elastic_area,
        units::StressConversionType::kPsiToPsf);
  }
}

void CableComponentUnitConverter::ConvertUnitStyleToConsistentV2(
    const units::UnitSystem& system,
    CableComponent& component) {
  // converts based on unit system
  if (system == units::UnitSystem::kMetric) {
    component.coefficient_expansion_linear_thermal /= 100;

    ConvertComponentCoefficientsStress(
        units::StressConversionType::kMegaPascalToPascal,
        component.coefficients_polynomial_creep);

    ConvertComponentCoefficientsStress(
        units::StressConversionType::kMegaPascalToPascal,
        component.coefficients_polynomial_loadstrain);

    component.load_limit_polynomial_creep *= 100;
    component.load_limit_polynomial_creep = units::ConvertStress(
        component.load_limit_polynomial_creep,
        units::StressConversionType::kMegaPascalToPascal);

    component.load_limit_polynomial_loadstrain *= 100;
    component.load_limit_polynomial_loadstrain = units::ConvertStress(
        component.load_limit_polynomial_loadstrain,
        units::StressConversionType::kMegaPascalToPascal);

    component.modulus_compression_elastic_area *= 100;
    component.modulus_compression_elastic_area = units::ConvertStress(
        component.modulus_compression_elastic_area,
        units::StressConversionType::kMegaPascalToPascal);

    component.modulus_tension_elastic_area *= 100;
    component.modulus_tension_elastic_area = units::ConvertStress(
        component.modulus_tension_elastic_area,
        units::StressConversionType::kMegaPascalToPascal);
  } else if (system == units::UnitSystem::kImperial) {
    component.coefficient_expansion_linear_thermal /= 100;

    ConvertComponentCoefficientsStress(
        units::StressConversionType::kPsiToPsf,
        component.coefficients_polynomial_creep);

    ConvertComponentCoefficientsStress(
        units::StressConversionType::kPsiToPsf,
        component.coefficients_polynomial_loadstrain);

    component.load_limit_polynomial_creep *= 100;
    component.load_limit_polynomial_creep = units::ConvertStress(
        component.load_limit_polynomial_creep,
        units::StressConversionType::kPsiToPsf);

    component.load_limit_polynomial_loadstrain *= 100;
    component.load_limit_polynomial_loadstrain = units::ConvertStress(
        component.load_limit_polynomial_loadstrain,
        units::StressConversionType::kPsiToPsf);

    component.modulus_compression_elastic_area *= 100;
    component.modulus_compression_elastic_area = units::ConvertStress(
        component.modulus_compression_elastic_area,
        units::StressConversionType::kPsiToPsf);

    component.modulus_tension_elastic_area *= 100;
    component.modulus_tension_elastic_area = units::ConvertStress(
        component.modulus_tension_elastic_area,
        units::StressConversionType::kPsiToPsf);
  }
}


bool CableUnitConverter::ConvertUnitStyleToConsistent(
    const int& version,
    const units::UnitSystem& system,
    const bool& is_recursive,
    Cable& cable) {
  bool status = true;

  // sends to proper converter function
  if (version == 0) {
    // points to latest converter version
    ConvertUnitStyleToConsistentV1(system, is_recursive, cable);
  } else if (version == 1) {
    ConvertUnitStyleToConsistentV1(system, is_recursive, cable);
  } else {
    wxString message = " Invalid version number. Aborting conversion.";
    wxLogError(message);
    status = false;
  }

  return status;
}

void CableUnitConverter::ConvertUnitStyleToDifferent(
    const units::UnitSystem& system,
    const bool& is_recursive,
    Cable& cable) {
  // converts component parameters from load to stress if applicable
  // this is unique to cables, and due to industry standard polynomials
  ConvertComponentLoadToStress(cable.area_physical, cable.component_core);
  ConvertComponentLoadToStress(cable.area_physical, cable.component_shell);

  // converts unit style for cable
  if (system == units::UnitSystem::kMetric) {
    cable.area_physical = units::ConvertLength(
        cable.area_physical,
        units::LengthConversionType::kMetersToMillimeters, 2);

    cable.diameter = units::ConvertLength(
        cable.diameter,
        units::LengthConversionType::kMetersToMillimeters);

    for (auto iter = cable.resistances_ac.begin();
          iter != cable.resistances_ac.end(); iter++) {
      Cable::ResistancePoint& point = *iter;

      point.resistance = units::ConvertLength(
        point.resistance,
        units::LengthConversionType::kMetersToKilometers, 1, false);
    }
  } else if (system == units::UnitSystem::kImperial) {
    cable.area_physical = units::ConvertLength(
        cable.area_physical,
        units::LengthConversionType::kFeetToInches, 2);

    cable.diameter = units::ConvertLength(
        cable.diameter,
        units::LengthConversionType::kFeetToInches);

    for (auto iter = cable.resistances_ac.begin();
          iter != cable.resistances_ac.end(); iter++) {
      Cable::ResistancePoint& point = *iter;

      point.resistance = units::ConvertLength(
        point.resistance,
        units::LengthConversionType::kFeetToMiles, 1, false);
    }
  }

  // triggers member variable converters
  if (is_recursive == true) {
    // converts unit style for cable components
    CableComponentUnitConverter::ConvertUnitStyleToDifferent(
        system,
        cable.component_core);

    CableComponentUnitConverter::ConvertUnitStyleToDifferent(
        system,
        cable.component_shell);
  }
}

void CableUnitConverter::ConvertUnitSystem(const units::UnitSystem& system_from,
                                           const units::UnitSystem& system_to,
                                           const bool& is_recursive,
                                           Cable& cable) {
  if (system_from == system_to) {
    return;
  }

  // converts unit system for cable
  if (system_to == units::UnitSystem::kMetric) {
    cable.area_physical = units::ConvertLength(
        cable.area_physical,
        units::LengthConversionType::kFeetToMeters, 2);

    cable.diameter = units::ConvertLength(
        cable.diameter,
        units::LengthConversionType::kFeetToMeters);

    for (auto iter = cable.resistances_ac.begin();
         iter != cable.resistances_ac.end(); iter++) {
      Cable::ResistancePoint& point = *iter;

      point.resistance = units::ConvertLength(
        point.resistance,
        units::LengthConversionType::kFeetToMeters, 1, false);

      point.temperature = units::ConvertTemperature(
        point.temperature,
        units::TemperatureConversionType::kFahrenheitToCelsius);
    }

    cable.strength_rated = units::ConvertForce(
        cable.strength_rated,
        units::ForceConversionType::kPoundsToNewtons);

    cable.temperature_properties_components = units::ConvertTemperature(
        cable.temperature_properties_components,
        units::TemperatureConversionType::kFahrenheitToCelsius);

    cable.weight_unit = units::ConvertForce(
        cable.weight_unit,
        units::ForceConversionType::kPoundsToNewtons);
    cable.weight_unit = units::ConvertLength(
        cable.weight_unit,
        units::LengthConversionType::kFeetToMeters, 1, false);
  } else if (system_to == units::UnitSystem::kImperial) {
    cable.area_physical = units::ConvertLength(
        cable.area_physical,
        units::LengthConversionType::kMetersToFeet, 2);

    cable.diameter = units::ConvertLength(
        cable.diameter,
        units::LengthConversionType::kMetersToFeet);

    for (auto iter = cable.resistances_ac.begin();
         iter != cable.resistances_ac.end(); iter++) {
      Cable::ResistancePoint& point = *iter;

      point.resistance = units::ConvertLength(
        point.resistance,
        units::LengthConversionType::kMetersToFeet, 1, false);

      point.temperature = units::ConvertTemperature(
        point.temperature,
        units::TemperatureConversionType::kCelsiusToFahrenheit);
    }

    cable.strength_rated = units::ConvertForce(
        cable.strength_rated,
        units::ForceConversionType::kNewtonsToPounds);

    cable.temperature_properties_components = units::ConvertTemperature(
        cable.temperature_properties_components,
        units::TemperatureConversionType::kCelsiusToFahrenheit);

    cable.weight_unit = units::ConvertForce(
        cable.weight_unit,
        units::ForceConversionType::kNewtonsToPounds);
    cable.weight_unit = units::ConvertLength(
        cable.weight_unit,
        units::LengthConversionType::kMetersToFeet, 1, false);
  }

  // triggers member variable converters
  if (is_recursive == true) {
    // converts unit system for cable components
    CableComponentUnitConverter::ConvertUnitSystem(system_from, system_to,
                                                   cable.component_core);
    CableComponentUnitConverter::ConvertUnitSystem(system_from, system_to,
                                                   cable.component_shell);
  }
}

void CableUnitConverter::ConvertUnitStyleToConsistentV1(
    const units::UnitSystem& system,
    const bool& is_recursive,
    Cable& cable) {
  // converts unit style for cable
  if (system == units::UnitSystem::kMetric) {
    cable.area_physical = units::ConvertLength(
        cable.area_physical,
        units::LengthConversionType::kMillimetersToMeters, 2);

    cable.diameter = units::ConvertLength(
        cable.diameter,
        units::LengthConversionType::kMillimetersToMeters);

    for (auto iter = cable.resistances_ac.begin();
          iter != cable.resistances_ac.end(); iter++) {
      Cable::ResistancePoint& point = *iter;

      point.resistance = units::ConvertLength(
        point.resistance,
        units::LengthConversionType::kKilometersToMeters, 1, false);
    }
  } else if (system == units::UnitSystem::kImperial) {
    cable.area_physical = units::ConvertLength(
        cable.area_physical,
        units::LengthConversionType::kInchesToFeet,
        2);

    cable.diameter = units::ConvertLength(
        cable.diameter,
        units::LengthConversionType::kInchesToFeet);

    for (auto iter = cable.resistances_ac.begin();
          iter != cable.resistances_ac.end(); iter++) {
      Cable::ResistancePoint& point = *iter;

      point.resistance = units::ConvertLength(
        point.resistance,
        units::LengthConversionType::kMilesToFeet, 1, false);
    }
  }

  // triggers member variable converters
  if (is_recursive == true) {
    CableComponentUnitConverter::ConvertUnitStyleToConsistent(
        0,
        system,
        cable.component_core);

    CableComponentUnitConverter::ConvertUnitStyleToConsistent(
        0,
        system,
        cable.component_shell);
  }

  // converts component parameters from stress to load if applicable
  // this is unique to cables, and due to industry standard polynomials
  ConvertComponentStressToLoad(cable.area_physical, cable.component_core);
  ConvertComponentStressToLoad(cable.area_physical, cable.component_shell);
}
