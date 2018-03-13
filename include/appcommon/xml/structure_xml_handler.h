// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef APPCOMMON_XML_STRUCTURE_XML_HANDLER_H_
#define APPCOMMON_XML_STRUCTURE_XML_HANDLER_H_

#include <string>

#include "models/base/units.h"
#include "models/transmissionline/structure.h"
#include "wx/xml/xml.h"

#include "appcommon/xml/xml_handler.h"

/// \par OVERVIEW
///
/// This class parses and generates a structure attachment XML node. The data is
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
class StructureAttachmentXmlHandler : public XmlHandler {
 public:
  /// \brief Creates an XML node for a structure attachment.
  /// \param[in] attachment
  ///   The structure attachment.
  /// \param[in] name
  ///   The name of the XML node. This will be an attribute for the created
  ///   node. If empty, no attribute will be created.
  /// \param[in] units
  ///   The unit system, which is used for attributing child XML nodes.
  /// \return An XML node for the cable component.
  static wxXmlNode* CreateNode(const StructureAttachment& attachment,
                               const std::string& name,
                               const units::UnitSystem& units);

  /// \brief Parses an XML node and populates a structure attachment.
  /// \param[in] root
  ///   The XML root node for the structure attachment.
  /// \param[in] filepath
  ///   The filepath that the xml node was loaded from. This is for logging
  ///   purposes only and can be left blank.
  /// \param[out] attachment
  ///   The structure attachment that is populated.
  /// \return The status of the xml node parse. If any errors are encountered
  ///   false is returned.
  /// All errors are logged to the active application log target. Critical
  /// errors cause the parsing to abort. Non-critical errors set the object
  /// property to an invalid state (if applicable).
  static bool ParseNode(const wxXmlNode* root,
                        const std::string& filepath,
                        StructureAttachment& attachment);

 private:
  /// \brief Parses a version 1 XML node and populates a structure attachment.
  /// \param[in] root
  ///   The XML root node for the structure attachment.
  /// \param[in] filepath
  ///   The filepath that the xml node was loaded from. This is for logging
  ///   purposes only and can be left blank.
  /// \param[out] attachment
  ///   The structure attachment that is populated.
  /// \return The status of the xml node parse. If any errors are encountered
  ///   false is returned.
  /// All errors are logged to the active application log target. Critical
  /// errors cause the parsing to abort. Non-critical errors set the object
  /// property to an invalid state (if applicable).
  static bool ParseNodeV1(const wxXmlNode* root,
                          const std::string& filepath,
                          StructureAttachment& attachment);
};

/// \par OVERVIEW
///
/// This class parses and generates a structure XML node. The data is
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
class StructureXmlHandler : public XmlHandler {
 public:
  /// \brief Creates an XML node for a structure struct.
  /// \param[in] structure
  ///   The structure.
  /// \param[in] name
  ///   The name of the XML node. This will be an attribute for the created
  ///   node. If empty, no attribute will be created.
  /// \param[in] units
  ///   The unit system, which is used for attributing child XML nodes.
  /// \return An XML node for the cable.
  static wxXmlNode* CreateNode(const Structure& structure,
                               const std::string& name,
                               const units::UnitSystem& units);

  /// \brief Parses an XML node and populates a structure struct.
  /// \param[in] root
  ///   The XML root node for the structure.
  /// \param[in] filepath
  ///   The filepath that the xml node was loaded from. This is for logging
  ///   purposes only and can be left blank.
  /// \param[out] structure
  ///   The structure that is populated.
  /// \return The status of the xml node parse. If any errors are encountered
  ///   false is returned.
  /// All errors are logged to the active application log target. Critical
  /// errors cause the parsing to abort. Non-critical errors set the object
  /// property to an invalid state (if applicable).
  static bool ParseNode(const wxXmlNode* root,
                        const std::string& filepath,
                        Structure& structure);

 private:
  /// \brief Parses a version 1 XML node and populates a structure struct.
  /// \param[in] root
  ///   The XML root node for the structure.
  /// \param[in] filepath
  ///   The filepath that the xml node was loaded from. This is for logging
  ///   purposes only and can be left blank.
  /// \param[out] structure
  ///   The structure that is populated.
  /// \return The status of the xml node parse. If any errors are encountered
  ///   false is returned.
  /// All errors are logged to the active application log target. Critical
  /// errors cause the parsing to abort. Non-critical errors set the object
  /// property to an invalid state (if applicable).
  static bool ParseNodeV1(const wxXmlNode* root,
                          const std::string& filepath,
                          Structure& structure);
};

#endif  // APPCOMMON_XML_STRUCTURE_XML_HANDLER_H_
