// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef APPCOMMON_XML_LINE_STRUCTURE_XML_HANDLER_H_
#define APPCOMMON_XML_LINE_STRUCTURE_XML_HANDLER_H_

#include <list>
#include <string>

#include "models/base/units.h"
#include "models/transmissionline/line_structure.h"
#include "wx/xml/xml.h"

#include "appcommon/xml/xml_handler.h"

/// \par OVERVIEW
///
/// This class parses and generates a line structure XML node. The data is
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
class LineStructureXmlHandler : public XmlHandler {
 public:
  /// \brief Creates an XML node for a line structure.
  /// \param[in] line_structure
  ///   The line structure.
  /// \param[in] name
  ///   The name of the XML node. This will be an attribute for the created
  ///   node. If empty, no attribute will be created.
  /// \param[in] units
  ///   The unit system, which is used for attributing child XML nodes.
  /// \return An XML node for the line structure.
  static wxXmlNode* CreateNode(const LineStructure& line_structure,
                               const std::string& name,
                               const units::UnitSystem& units);

  /// \brief Parses an XML node and populates a line structure.
  /// \param[in] root
  ///   The XML root node for the line structure.
  /// \param[in] filepath
  ///   The filepath that the xml node was loaded from. This is for logging
  ///   purposes only and can be left blank.
  /// \param[in] structures
  ///   A list of structures that is matched against a structure name. If
  ///   found, a pointer will be set to the matching structures.
  /// \param[in] hardwares
  ///   A list of hardwares that is matched against a hardware name. If found,
  ///   a pointer will be set to the matching hardware.
  /// \param[out] line_structure
  ///   The line structure that is populated.
  /// \return The status of the xml node parse. If any errors are encountered
  ///   false is returned.
  /// All errors are logged to the active application log target. Critical
  /// errors cause the parsing to abort. Non-critical errors set the object
  /// property to an invalid state (if applicable).
  static bool ParseNode(const wxXmlNode* root,
                        const std::string& filepath,
                        const std::list<const Structure*>* structures,
                        const std::list<const Hardware*>* hardwares,
                        LineStructure& line_structure);

 private:
  /// \brief Parses an XML node and populates a line structure.
  /// \param[in] root
  ///   The XML root node for the line structure.
  /// \param[in] filepath
  ///   The filepath that the xml node was loaded from. This is for logging
  ///   purposes only and can be left blank.
  /// \param[in] structures
  ///   A list of structures that is matched against a structure name. If
  ///   found, a pointer will be set to the matching structures.
  /// \param[in] hardwares
  ///   A list of hardwares that is matched against a hardware name. If found,
  ///   a pointer will be set to the matching hardware.
  /// \param[out] line_structure
  ///   The line structure that is populated.
  /// \return The status of the xml node parse. If any errors are encountered
  ///   false is returned.
  /// All errors are logged to the active application log target. Critical
  /// errors cause the parsing to abort. Non-critical errors set the object
  /// property to an invalid state (if applicable).
  static bool ParseNodeV1(const wxXmlNode* root,
                          const std::string& filepath,
                          const std::list<const Structure*>* structures,
                          const std::list<const Hardware*>* hardwares,
                          LineStructure& line_structure);
};

#endif  // APPCOMMON_XML_LINE_STRUCTURE_XML_HANDLER_H_
