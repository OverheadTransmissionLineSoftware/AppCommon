// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef APPCOMMON_XML_POINT_XML_HANDLER_H_
#define APPCOMMON_XML_POINT_XML_HANDLER_H_

#include <string>

#include "models/base/point.h"
#include "wx/xml/xml.h"

#include "appcommon/xml/xml_handler.h"

/// \par OVERVIEW
///
/// This class parses and generates a Point2d<double> XML node. The data is
/// transferred between the XML node and the data object.
///
/// \par VERSION
///
/// This class can parse all versions of the XML node. However, new nodes will
/// only be generated with the most recent version.
class Point2dDoubleXmlHandler : public XmlHandler {
 public:
  /// \brief Creates an XML node for the point.
  /// \param[in] point
  ///   The point.
  /// \param[in] name
  ///   The name of the XML node. This will be an attribute for the created
  ///   node. If empty, no attribute will be created.
  /// \param[in] attribute_components
  ///   The attribute that will be created for the node.
  /// \param[in] precision
  ///   The rounding precision for the components.
  /// \param[in] is_fixed_decimal
  ///   A flag that indicates if the precision is to apply to decimal digits
  ///   only. Enabling this will set the number of decimal digits equal to the
  ///   precision.
  /// \return An XML node for the point.
  static wxXmlNode* CreateNode(const Point2d<double>& point,
                               const std::string& name,
                               const wxXmlAttribute& attribute_components,
                               const int& precision,
                               const bool& is_fixed_decimal = false);

  /// \brief Parses an XML node and populates a point.
  /// \param[in] root
  ///   The XML root node for the point.
  /// \param[in] filepath
  ///   The filepath that the xml node was loaded from. This is for logging
  ///   purposes only and can be left blank.
  /// \param[out] point
  ///   The point that is populated.
  /// \return The status of the xml node parse. If any errors are encountered
  ///   false is returned.
  /// All errors are logged to the active application log target. Critical
  /// errors cause the parsing to abort. Non-critical errors set the object
  /// property to an invalid state (if applicable).
  static bool ParseNode(const wxXmlNode* root,
                        const std::string& filepath,
                        Point2d<double>& point);

 private:
  /// \brief Parses a version 1 XML node and populates a point.
  /// \param[in] root
  ///   The XML root node for the point.
  /// \param[in] filepath
  ///   The filepath that the xml node was loaded from. This is for logging
  ///   purposes only and can be left blank.
  /// \param[out] point
  ///   The point that is populated.
  /// \return The status of the xml node parse. If any errors are encountered
  ///   false is returned.
  /// All errors are logged to the active application log target. Critical
  /// errors cause the parsing to abort. Non-critical errors set the object
  /// property to an invalid state (if applicable).
  static bool ParseNodeV1(const wxXmlNode* root,
                          const std::string& filepath,
                          Point2d<double>& point);
};


/// \par OVERVIEW
///
/// This class parses and generates a Point3d<double> XML node. The data is
/// transferred between the XML node and the data object.
///
/// \par VERSION
///
/// This class can parse all versions of the XML node. However, new nodes will
/// only be generated with the most recent version.
class Point3dDoubleXmlHandler : public XmlHandler {
 public:
  /// \brief Creates an XML node for the point.
  /// \param[in] point
  ///   The point.
  /// \param[in] name
  ///   The name of the XML node. This will be an attribute for the created
  ///   node. If empty, no attribute will be created.
  /// \param[in] attribute_components
  ///   The attribute that will be created for the node.
  /// \param[in] precision
  ///   The rounding precision for the components.
  /// \param[in] is_fixed_decimal
  ///   A flag that indicates if the precision is to apply to decimal digits
  ///   only. Enabling this will set the number of decimal digits equal to the
  ///   precision.
  /// \return An XML node for the point.
  static wxXmlNode* CreateNode(const Point3d<double>& point,
                               const std::string& name,
                               const wxXmlAttribute& attribute_components,
                               const int& precision,
                               const bool& is_fixed_decimal = false);

  /// \brief Parses an XML node and populates a point.
  /// \param[in] root
  ///   The XML root node for the point.
  /// \param[in] filepath
  ///   The filepath that the xml node was loaded from. This is for logging
  ///   purposes only and can be left blank.
  /// \param[out] point
  ///   The point that is populated.
  /// \return The status of the xml node parse. If any errors are encountered
  ///   false is returned.
  /// All errors are logged to the active application log target. Critical
  /// errors cause the parsing to abort. Non-critical errors set the object
  /// property to an invalid state (if applicable).
  static bool ParseNode(const wxXmlNode* root,
                        const std::string& filepath,
                        Point3d<double>& point);

 private:
  /// \brief Parses a version 1 XML node and populates a point.
  /// \param[in] root
  ///   The XML root node for the point.
  /// \param[in] filepath
  ///   The filepath that the xml node was loaded from. This is for logging
  ///   purposes only and can be left blank.
  /// \param[out] point
  ///   The point that is populated.
  /// \return The status of the xml node parse. If any errors are encountered
  ///   false is returned.
  /// All errors are logged to the active application log target. Critical
  /// errors cause the parsing to abort. Non-critical errors set the object
  /// property to an invalid state (if applicable).
  static bool ParseNodeV1(const wxXmlNode* root,
                          const std::string& filepath,
                          Point3d<double>& point);
};

#endif  // APPCOMMON_XML_POINT_XML_HANDLER_H_
