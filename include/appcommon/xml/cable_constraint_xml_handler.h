// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef APPCOMMON_XML_CABLE_CONSTRAINT_XML_HANDLER_H_
#define APPCOMMON_XML_CABLE_CONSTRAINT_XML_HANDLER_H_

#include <string>
#include <list>

#include "models/base/units.h"
#include "models/transmissionline/cable_constraint.h"
#include "wx/xml/xml.h"

#include "appcommon/xml/xml_handler.h"

/// \par OVERVIEW
///
/// This class parses and generates a cable constraint XML node. The data is
/// transferred between the XML node and the data object.
///
/// \par VERSION
///
/// This class can parse all versions of the XML node. However, new nodes will
/// only be generated with the most recent version.
///
/// \par UNIT CONVERSIONS
///
/// This class can optionally convert the unit style to 'consistent' when
/// parsing. The conversion will occur after the xml node has been parsed. The
/// respective unit converter class will perform the conversion using the
/// converter version that matches the xml node version.
///
/// \par UNIT ATTRIBUTES
///
/// This class supports attributing the child XML nodes for various unit
/// systems and styles.
class CableConstraintXmlHandler : public XmlHandler {
 public:
  /// \brief Creates an XML node for a cable constraint.
  /// \param[in] constraint
  ///   The cable constraint.
  /// \param[in] name
  ///   The name of the XML node. This will be an attribute for the created
  ///   node. If empty, no attribute will be created.
  /// \param[in] system_units
  ///   The unit system, which is used for attributing child XML nodes.
  /// \param[in] system_style
  ///   The unit style, which is used for attributing child XML nodes.
  /// \return An XML node for the cable constraint.
  static wxXmlNode* CreateNode(const CableConstraint& constraint,
                               const std::string name,
                               const units::UnitSystem& system_units,
                               const units::UnitStyle& style_units);

  /// \brief Parses an XML node and populates a cable constraint.
  /// \param[in] root
  ///   The XML root node for the cable constraint.
  /// \param[in] filepath
  ///   The filepath that the xml node was loaded from. This is for logging
  ///   purposes only and can be left blank.
  /// \param[in] units
  ///   The unit system. If no conversion is being done this will be ignored.
  /// \param[in] convert
  ///   A flag that determines if the unit style is converted to 'consistent'.
  /// \param[in] weathercases
  ///   A list of weathercases that is matched against a weathercase
  ///   description. If found, a pointer will be set to a matching
  ///   weathercase.
  /// \param[out] constraint
  ///   The cable constraint that is populated.
  /// \return The status of the xml node parse. If any errors are encountered
  ///   false is returned.
  /// All errors are logged to the active application log target. Critical
  /// errors cause the parsing to abort. Non-critical errors set the object
  /// property to an invalid state (if applicable).
  static bool ParseNode(const wxXmlNode* root,
                        const std::string& filepath,
                        const units::UnitSystem& units,
                        const bool& convert,
                        const std::list<const WeatherLoadCase*>* weathercases,
                        CableConstraint& constraint);

 private:
  /// \brief Parses a version 1 XML node and populates a cable constraint.
  /// \param[in] root
  ///   The XML root node for the cable constraint.
  /// \param[in] filepath
  ///   The filepath that the xml node was loaded from. This is for logging
  ///   purposes only and can be left blank.
  /// \param[in] units
  ///   The unit system. If no conversion is being done this will be ignored.
  /// \param[in] convert
  ///   A flag that determines if the unit style is converted to 'consistent'.
  /// \param[in] weathercases
  ///   A list of weathercases that is matched against a weathercase
  ///   description. If found, a pointer will be set to a matching
  ///   weathercase.
  /// \param[out] constraint
  ///   The cable constraint that is populated.
  /// \return The status of the xml node parse. If any errors are encountered
  ///   false is returned.
  /// All errors are logged to the active application log target. Critical
  /// errors cause the parsing to abort. Non-critical errors set the object
  /// property to an invalid state (if applicable).
  static bool ParseNodeV1(const wxXmlNode* root,
                          const std::string& filepath,
                          const units::UnitSystem& units,
                          const bool& convert,
                          const std::list<const WeatherLoadCase*>* weathercases,
                          CableConstraint& constraint);

  /// \brief Parses a version 2 XML node and populates a cable constraint.
  /// \param[in] root
  ///   The XML root node for the cable constraint.
  /// \param[in] filepath
  ///   The filepath that the xml node was loaded from. This is for logging
  ///   purposes only and can be left blank.
  /// \param[in] units
  ///   The unit system. If no conversion is being done this will be ignored.
  /// \param[in] convert
  ///   A flag that determines if the unit style is converted to 'consistent'.
  /// \param[in] weathercases
  ///   A list of weathercases that is matched against a weathercase
  ///   description. If found, a pointer will be set to a matching
  ///   weathercase.
  /// \param[out] constraint
  ///   The cable constraint that is populated.
  /// \return The status of the xml node parse. If any errors are encountered
  ///   false is returned.
  /// All errors are logged to the active application log target. Critical
  /// errors cause the parsing to abort. Non-critical errors set the object
  /// property to an invalid state (if applicable).
  static bool ParseNodeV2(const wxXmlNode* root,
                          const std::string& filepath,
                          const units::UnitSystem& units,
                          const bool& convert,
                          const std::list<const WeatherLoadCase*>* weathercases,
                          CableConstraint& constraint);
};

#endif  // APPCOMMON_XML_CABLE_CONSTRAINT_XML_HANDLER_H_
