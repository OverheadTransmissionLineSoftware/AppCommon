// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef APPCOMMON_UNITS_CABLE_UNIT_CONVERTER_H_
#define APPCOMMON_UNITS_CABLE_UNIT_CONVERTER_H_

#include "models/base/units.h"
#include "models/transmissionline/cable.h"

/// \par OVERVIEW
///
/// This class converts a cable component between unit systems as well as unit
/// styles.
///
/// \par VERSIONS
///
/// Versions are needed when converting to the 'consistent' unit style. The
/// the starter 'different' style units can change over time, and versions
/// allow the converter to align with a specific xml node if needed.
///
/// When converting to the 'different' unit style the latest converter version
/// is used.
///
/// \par STRESS VS. LOAD
///
/// This class uses stress when converted to 'different' unit style, and load
/// when converted to the 'consistent' unit style. Users are accustomed to
/// stress when dealing with CableComponent coefficients and limits, but the
/// Models libraries requires the values be defined in terms of load.
class CableComponentUnitConverter {
 public:
  /// \brief Converts to 'consistent' unit style.
  /// \param[in] version
  ///   The version. 0 (zero) indicates the latest version.
  /// \param[in] system
  ///   The unit system.
  /// \param[in,out] component
  ///   The component to be converted.
  /// \return The success status.
  static bool ConvertUnitStyleToConsistent(const int& version,
                                           const units::UnitSystem& system,
                                           CableComponent& component);

  /// \brief Converts to 'different' unit style.
  /// \param[in] system
  ///   The unit system.
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
  static void ConvertUnitStyleToDifferent(const units::UnitSystem& system,
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

 private:
  /// \brief Converts to 'consistent' unit style using version 1.
  /// \param[in] system
  ///   The unit system.
  /// \param[in,out] component
  ///   The component to be converted.
  static void ConvertUnitStyleToConsistentV1(const units::UnitSystem& system,
                                             CableComponent& component);
};


/// \par OVERVIEW
///
/// This class converts a cable between unit systems as well as unit styles.
///
/// \par RECURSION
///
/// This class supports optionally invoking member variable converters,
/// depending on whether the entire set of data needs converted or just a
/// portion of it.
///
/// \par VERSIONS
///
/// Versions are needed when converting to the 'consistent' unit style. The
/// the starter 'different' style units can change over time, and versions
/// allow the converter to align with a specific xml node if needed. The version
/// will not apply to any member variable converters, as those xml node versions
/// may be different. If the recursive flag is used, the latest version of all
/// member variable converters is used.
///
/// When converting to the 'different' unit style the latest converter version
/// is used.
class CableUnitConverter {
 public:
  /// \brief Converts to 'consistent' unit style.
  /// \param[in] version
  ///   The version. 0 (zero) indicates the latest version.
  /// \param[in] system
  ///   The unit system.
  /// \param[in] is_recursive
  ///   An indicator that determines if member variable converters are invoked.
  /// \param[in,out] cable
  ///   The cable to be converted.
  /// \return The success status.
  static bool ConvertUnitStyleToConsistent(const int& version,
                                           const units::UnitSystem& system,
                                           const bool& is_recursive,
                                           Cable& cable);

  /// \brief Converts to 'different' unit style.
  /// \param[in] system
  ///   The unit system.
  /// \param[in] is_recursive
  ///   An indicator that determines if member variable converters are invoked.
  /// \param[in,out] cable
  ///   The cable to be converted.
  /// The 'different' style units are as follows:
  ///  - area_physical = [mm^2 or in^2]
  ///  - diameter = [mm or in]
  ///  - strength_rated = [N or lbs]
  ///  - temperature_properties_components = [degC or degF]
  ///  - weight_unit = [N/m or lbs/ft]
  static void ConvertUnitStyleToDifferent(const units::UnitSystem& system,
                                          const bool& is_recursive,
                                          Cable& cable);

  /// \brief Changes between unit systems.
  /// \param[in] system_from
  ///   The unit system to convert from.
  /// \param[in] system_to
  ///   The unit system to convert to.
  /// \param[in] is_recursive
  ///   An indicator that determines if member variable converters are invoked.
  /// \param[in,out] cable
  ///   The cable to be converted.
  /// This function requires that the cable be in a 'consistent' unit style.
  static void ConvertUnitSystem(const units::UnitSystem& system_from,
                                const units::UnitSystem& system_to,
                                const bool& is_recursive,
                                Cable& cable);

 private:
  /// \brief Converts to 'consistent' unit style using version 1.
  /// \param[in] system
  ///   The unit system.
  /// \param[in] is_recursive
  ///   An indicator that determines if member variable converters are invoked.
  /// \param[in,out] cable
  ///   The cable to be converted.
  static void ConvertUnitStyleToConsistentV1(const units::UnitSystem& system,
                                             const bool& is_recursive,
                                             Cable& cable);
};

#endif  // APPCOMMON_UNITS_CABLE_UNIT_CONVERTER_H_
