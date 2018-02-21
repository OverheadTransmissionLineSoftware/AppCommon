// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef OTLS_APPCOMMON_XML_COLORXMLHANDLER_H_
#define OTLS_APPCOMMON_XML_COLORXMLHANDLER_H_

#include <string>

#include "wx/xml/xml.h"

#include "appcommon/xml/xml_handler.h"

/// \par OVERVIEW
///
/// This class parses and generates a color XML node. The data is transferred
/// between the XML node and the data object.
///
/// \par VERSION
///
/// This class can parse all versions of the XML node. However, new nodes will
/// only be generated with the most recent version.
class ColorXmlHandler : public XmlHandler {
 public:
  /// \brief Creates an XML node for a color.
  /// \param[in] color
  ///   The color.
  /// \param[in] name
  ///   The name of the XML node. This will be an attribute for the created
  ///   node. If empty, no attribute will be created.
  /// \return An XML node for the color.
  static wxXmlNode* CreateNode(const wxColour& color,
                               const std::string& name);

  /// \brief Parses an XML node and populates a color.
  /// \param[in] root
  ///   The XML root node for the color.
  /// \param[in] filepath
  ///   The filepath that the xml node was loaded from. This is for logging
  ///   purposes only and can be left blank.
  /// \param[out] color
  ///   The color that is populated.
  /// \return The status of the xml node parse. If any errors are encountered
  ///   false is returned.
  /// All errors are logged to the active application log target. Critical
  /// errors cause the parsing to abort. Non-critical errors set the object
  /// property to an invalid state (if applicable).
  static bool ParseNode(const wxXmlNode* root,
                        const std::string& filepath,
                        wxColour& color);

 private:
  /// \brief Parses a version 1 XML node and populates a color.
  /// \param[in] root
  ///   The XML root node for the color.
  /// \param[in] filepath
  ///   The filepath that the xml node was loaded from. This is for logging
  ///   purposes only and can be left blank.
  /// \param[out] color
  ///   The color that is populated.
  /// \return The status of the xml node parse. If any errors are encountered
  ///   false is returned.
  /// All errors are logged to the active application log target. Critical
  /// errors cause the parsing to abort. Non-critical errors set the object
  /// property to an invalid state (if applicable).
  static bool ParseNodeV1(const wxXmlNode* root,
                          const std::string& filepath,
                          wxColour& color);
};

#endif  // OTLS_APPCOMMON_XML_COLORXMLHANDLER_H_
