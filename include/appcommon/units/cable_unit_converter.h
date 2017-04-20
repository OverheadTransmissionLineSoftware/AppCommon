// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef OTLS_APPCOMMON_UNITS_CABLEUNITCONVERTER_H_
#define OTLS_APPCOMMON_UNITS_CABLEUNITCONVERTER_H_

#include "models/base/units.h"
#include "models/transmissionline/cable.h"

/// \par OVERVIEW
///
/// This class converts a cable component between unit systems as well as unit
/// styles.
///
/// \par STRESS VS. LOAD
///
/// This class uses stress when converted to 'different' unit style, and load
/// when converted to the 'consistent' unit style. Users are accustomed to
/// stress when dealing with CableComponent coefficients and limits, but the
/// Models libraries requires the values be defined in terms of load.
class CableComponentUnitConverter {
 public:
  /// \brief Changes between unit styles.
  /// \param[in] system
  ///   The unit system.
  /// \param[in] style_from
  ///   The unit style to convert from.
  /// \param[in] style_to
  ///   The unit style to convert to.
  /// \param[in,out] component
  ///   The component to be converted.
  /// The 'different' style units are as follows:
  ///  - coefficient_expansion_linear_thermal = [/degC or /degF]
  ///  - coefficients_polynomial_creep = [MPa or psi]
  ///  - coefficients_polynomial_loadstrain = [MPa or psi]
  ///  - load_limit_polynomial_creep = [MPa or psi]
  ///  - load_limit_polynomial_loadstrain = [MPa or psi]
  ///  - modulus_compression_elastic_area = [MPa or psi]
  ///  - modulus_tension_elastic_area = [MPa of psi]
  static void ConvertUnitStyle(const units::UnitSystem& system,
                               const units::UnitStyle& style_from,
                               const units::UnitStyle& style_to,
                               CableComponent& component);

  /// \brief Changes between unit systems.
  /// \param[in] system_from
  ///   The unit system to convert from.
  /// \param[in] system_to
  ///   The unit system to convert to.
  /// \param[in,out] component
  ///   The component to be converted.
  /// This function requires that the component is in a 'consistent' unit
  /// style.
  static void ConvertUnitSystem(const units::UnitSystem& system_from,
                                const units::UnitSystem& system_to,
                                CableComponent& component);
};

/// \par OVERVIEW
///
/// This class converts a cable between unit systems as well as unit styles.
class CableUnitConverter {
 public:
  /// \brief Changes between unit styles.
  /// \param[in] system
  ///   The unit system.
  /// \param[in] style_from
  ///   The unit style to convert from.
  /// \param[in] style_to
  ///   The unit style to convert to.
  /// \param[in,out] cable
  ///   The cable to be converted.
  /// The 'different' style units are as follows:
  ///  - area_physical = [mm^2 or in^2]
  ///  - diameter = [mm or in]
  ///  - strength_rated = [N or lbs]
  ///  - temperature_properties_components = [degC or degF]
  ///  - weight_unit = [N/m or lbs/ft]
  static void ConvertUnitStyle(const units::UnitSystem& system,
                               const units::UnitStyle& style_from,
                               const units::UnitStyle& style_to,
                               Cable& cable);

  /// \brief Changes between unit systems.
  /// \param[in] system_from
  ///   The unit system to convert from. These must be consistent style units.
  /// \param[in] system_to
  ///   The unit system to convert to. These will also be in consistent style
  ///   units.
  /// \param[in,out] cable
  ///   The cable to be converted.
  /// This function requires that the cable be in a 'consistent' unit style.
  static void ConvertUnitSystem(const units::UnitSystem& system_from,
                                const units::UnitSystem& system_to,
                                Cable& cable);
};

#endif  // OTLS_APPCOMMON_UNITS_CABLEUNITCONVERTER_H_
