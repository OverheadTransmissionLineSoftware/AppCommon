// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef APPCOMMON_XML_LINE_CABLE_XML_HANDLER_H_
#define APPCOMMON_XML_LINE_CABLE_XML_HANDLER_H_

#include <list>
#include <string>

#include "models/base/units.h"
#include "models/transmissionline/line_cable.h"
#include "wx/xml/xml.h"

#include "appcommon/xml/xml_handler.h"

/// \par OVERVIEW
///
/// This class parses and generates a linecable XML node. The data is
/// transferred between the XML node and the data object.
///
/// \par VERSION
///
/// This class can parse all versions of the XML node. However, new nodes will
/// only be generated with the most recent version.
///
/// \par UNIT ATTRIBUTES
///
/// This class supports attributing the child XML nodes for various unit
/// systems.
class LineCableXmlHandler : public XmlHandler {
 public:
  /// \brief Creates an XML node for a linecable.
  /// \param[in] line_cable
  ///   The line cable.
  /// \param[in] name
  ///   The name of the XML node. This will be an attribute for the created
  ///   node. If empty, no attribute will be created.
  /// \param[in] units
  ///   The unit system, which is used for attributing child XML nodes.
  /// \param[in] line_structures
  ///   A list of line structures that is matched against for connections.
  /// \return An XML node for the line cable.
  static wxXmlNode* CreateNode(
      const LineCable& line_cable,
      const std::string& name,
      const units::UnitSystem& units,
      const std::list<const LineStructure*>* line_structures);

  /// \brief Parses an XML node and populates a line cable.
  /// \param[in] root
  ///   The XML root node for the linecable.
  /// \param[in] filepath
  ///   The filepath that the xml node was loaded from. This is for logging
  ///   purposes only and can be left blank.
  /// \param[in] cables
  ///   A list of cables that is matched against a cable description. If
  ///   found, a pointer will be set to the matching cable.
  /// \param[in] line_structures
  ///   A list of line structures that is matched against for connections. If
  ///   found, a pointer will be set to the matching line structure.
  /// \param[in] weathercases
  ///   A list of weathercases that is matched against a weathercase
  ///   description. If found, a pointer will be set to the matching
  ///   weathercase.
  /// \param[out] line_cable
  ///   The line cable that is populated.
  /// \return The status of the xml node parse. If any errors are encountered
  ///   false is returned.
  /// All errors are logged to the active application log target. Critical
  /// errors cause the parsing to abort. Non-critical errors set the object
  /// property to an invalid state (if applicable).
  static bool ParseNode(const wxXmlNode* root,
                        const std::string& filepath,
                        const std::list<const Cable*>* cables,
                        const std::list<const LineStructure*>* line_structures,
                        const std::list<const WeatherLoadCase*>* weathercases,
                        LineCable& line_cable);

 private:
  /// \brief Parses a version 1 XML node and populates a linecable.
  /// \param[in] root
  ///   The XML root node for the linecable.
  /// \param[in] filepath
  ///   The filepath that the xml node was loaded from. This is for logging
  ///   purposes only and can be left blank.
  /// \param[in] cables
  ///   A list of cables that is matched against a cable description. If
  ///   found, a pointer will be set to the matching cable.
  /// \param[in] line_structures
  ///   A list of line structures that is matched against for connections. If
  ///   found, a pointer will be set to the matching line structure.
  /// \param[in] weathercases
  ///   A list of weathercases that is matched against a weathercase
  ///   description. If found, a pointer will be set to the matching
  ///   weathercase.
  /// \param[out] line_cable
  ///   The line cable that is populated.
  /// \return The status of the xml node parse. If any errors are encountered
  ///   false is returned.
  /// All errors are logged to the active application log target. Critical
  /// errors cause the parsing to abort. Non-critical errors set the object
  /// property to an invalid state (if applicable).
  static bool ParseNodeV1(const wxXmlNode* root,
                          const std::string& filepath,
                          const std::list<const Cable*>* cables,
                          const std::list<const LineStructure*>* line_structures,
                          const std::list<const WeatherLoadCase*>* weathercases,
                          LineCable& line_cable);
};

#endif  // APPCOMMON_XML_LINE_CABLE_XML_HANDLER_H_
