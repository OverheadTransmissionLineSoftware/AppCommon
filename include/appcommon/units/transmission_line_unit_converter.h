// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef APPCOMMON_UNITS_TRANSMISSION_LINE_UNIT_CONVERTER_H_
#define APPCOMMON_UNITS_TRANSMISSION_LINE_UNIT_CONVERTER_H_

#include "models/base/units.h"
#include "models/transmissionline/transmission_line.h"

/// \par OVERVIEW
///
/// This class converts a transmission line between unit systems as well as unit
/// styles.
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
class TransmissionLineUnitConverter {
 public:
  /// \brief Converts to 'consistent' unit style.
  /// \param[in] version
  ///   The version. 0 (zero) indicates the latest version.
  /// \param[in] system
  ///   The unit system.
  /// \param[in] is_recursive
  ///   An indicator that determines if member variable converters are invoked.
  /// \param[in,out] line
  ///   The transmission line to be converted.
  /// \return The success status.
  static bool ConvertUnitStyleToConsistent(const int& version,
                                           const units::UnitSystem& system,
                                           const bool& is_recursive,
                                           TransmissionLine& line);

  /// \brief Converts to 'different' unit style.
  /// \param[in] system
  ///   The unit system.
  /// \param[in] is_recursive
  ///   An indicator that determines if member variable converters are invoked.
  /// \param[in,out] line
  ///   The transmission line to be converted.
  /// The 'different' style units are as follows:
  ///  - origin = [m or ft]
  static void ConvertUnitStyleToDifferent(const units::UnitSystem& system,
                                          const bool& is_recursive,
                                          TransmissionLine& line);

  /// \brief Converts between unit systems.
  /// \param[in] system_from
  ///   The unit system to convert from.
  /// \param[in] system_to
  ///   The unit system to convert to.
  /// \param[in] is_recursive
  ///   An indicator that determines if member variable converters are invoked.
  /// \param[in,out] line
  ///   The transmission line to be converted.
  /// This function requires that the line cable be in a 'consistent' unit
  /// style.
  static void ConvertUnitSystem(const units::UnitSystem& system_from,
                                const units::UnitSystem& system_to,
                                const bool& is_recursive,
                                TransmissionLine& line);

 private:
  /// \brief Converts to 'consistent' unit style using version 1.
  /// \param[in] system
  ///   The unit system.
  /// \param[in] is_recursive
  ///   An indicator that determines if member variable converters are invoked.
  /// \param[in,out] line
  ///   The transmission line to be converted.
  static void ConvertUnitStyleToConsistentV1(const units::UnitSystem& system,
                                             const bool& is_recursive,
                                             TransmissionLine& line);
};

#endif  // APPCOMMON_UNITS_TRANSMISSION_LINE_UNIT_CONVERTER_H_
