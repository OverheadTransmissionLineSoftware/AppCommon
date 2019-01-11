// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "appcommon/xml/point_xml_handler.h"

#include "models/base/helper.h"

wxXmlNode* Point2dDoubleXmlHandler::CreateNode(
    const Point2d<double>& point,
    const std::string& name,
    const wxXmlAttribute& attribute_components,
    const int& precision) {
  // variables used to create XML node
  wxXmlNode* node_root = nullptr;
  std::string title;
  std::string content;

  // creates a node for the root
  title = "point_2d";
  content = helper::DoubleToString(point.x, precision)
      + "," + helper::DoubleToString(point.y, precision);

  node_root = CreateElementNodeWithContent(title, content);

  // adds attributes
  node_root->AddAttribute("version", "1");

  if (name != "") {
    node_root->AddAttribute("name", name);
  }

  // returns node
  return node_root;
}

bool Point2dDoubleXmlHandler::ParseNode(const wxXmlNode* root,
                                        const std::string& filepath,
                                        Point2d<double>& point) {
  wxString message;

  // checks for valid root node
  if (root->GetName() != "point_2d") {
    message = FileAndLineNumber(filepath, root) +
              " Invalid root node. Aborting node parse.";
    wxLogError(message);
    return false;
  }

  // gets version attribute
  const int kVersion = Version(root);
  if (kVersion == -1) {
    message = FileAndLineNumber(filepath, root) +
              " Version attribute is missing or invalid. Aborting node parse.";
    wxLogError(message);
    return false;
  }

  // sends to proper parsing function
  if (kVersion == 1) {
    return ParseNodeV1(root, filepath, point);
  } else {
    message = FileAndLineNumber(filepath, root) +
              " Invalid version number. Aborting node parse.";
    wxLogError(message);
    return false;
  }
}

bool Point2dDoubleXmlHandler::ParseNodeV1(const wxXmlNode* root,
                                          const std::string& filepath,
                                          Point2d<double>& point) {
  bool status = true;
  wxString message;

  // parses content into separate strings
  const std::string content = ParseElementNodeWithContent(root);
  std::list<std::string> content_parsed = helper::Parse(content, ',');
  if (content_parsed.size() != 2) {
    message = FileAndLineNumber(filepath, root)
              + "Invalid point.";
    wxLogError(message);
    status = false;

    return false;
  }

  // converts strings to double values
  wxString str;
  double value = -999999;

  str = *std::next(content_parsed.cbegin(), 0);
  if (str.ToDouble(&value) == true) {
    point.x = value;
  } else {
    message = FileAndLineNumber(filepath, root)
              + "Invalid x value.";
    wxLogError(message);
    status = false;
  }

  str = *std::next(content_parsed.cbegin(), 1);
  if (str.ToDouble(&value) == true) {
    point.y = value;
  } else {
    message = FileAndLineNumber(filepath, root)
              + "Invalid y value.";
    wxLogError(message);
    status = false;
  }

  return status;
}


wxXmlNode* Point3dDoubleXmlHandler::CreateNode(
    const Point3d<double>& point,
    const std::string& name,
    const wxXmlAttribute& attribute_components,
    const int& precision) {
  // variables used to create XML node
  wxXmlNode* node_root = nullptr;
  std::string title;
  std::string content;

  // creates a node for the root
  title = "point_3d";
  content = helper::DoubleToString(point.x, precision)
      + "," + helper::DoubleToString(point.y, precision)
      + "," + helper::DoubleToString(point.z, precision);

  node_root = CreateElementNodeWithContent(title, content);

  // adds attributes
  node_root->AddAttribute("version", "1");

  if (name != "") {
    node_root->AddAttribute("name", name);
  }

  // returns node
  return node_root;
}

bool Point3dDoubleXmlHandler::ParseNode(const wxXmlNode* root,
                                        const std::string& filepath,
                                        Point3d<double>& point) {
  wxString message;

  // checks for valid root node
  if (root->GetName() != "point_3d") {
    message = FileAndLineNumber(filepath, root) +
              " Invalid root node. Aborting node parse.";
    wxLogError(message);
    return false;
  }

  // gets version attribute
  const int kVersion = Version(root);
  if (kVersion == -1) {
    message = FileAndLineNumber(filepath, root) +
              " Version attribute is missing or invalid. Aborting node parse.";
    wxLogError(message);
    return false;
  }

  // sends to proper parsing function
  if (kVersion == 1) {
    return ParseNodeV1(root, filepath, point);
  } else {
    message = FileAndLineNumber(filepath, root) +
              " Invalid version number. Aborting node parse.";
    wxLogError(message);
    return false;
  }
}

bool Point3dDoubleXmlHandler::ParseNodeV1(const wxXmlNode* root,
                                          const std::string& filepath,
                                          Point3d<double>& point) {
  bool status = true;
  wxString message;

  // parses content into separate strings
  const std::string content = ParseElementNodeWithContent(root);
  std::list<std::string> content_parsed = helper::Parse(content, ',');
  if (content_parsed.size() != 3) {
    message = FileAndLineNumber(filepath, root)
              + "Invalid point.";
    wxLogError(message);
    status = false;

    return false;
  }

  // converts strings to double values
  wxString str;
  double value = -999999;

  str = *std::next(content_parsed.cbegin(), 0);
  if (str.ToDouble(&value) == true) {
    point.x = value;
  } else {
    message = FileAndLineNumber(filepath, root)
              + "Invalid x value.";
    wxLogError(message);
    status = false;
  }

  str = *std::next(content_parsed.cbegin(), 1);
  if (str.ToDouble(&value) == true) {
    point.y = value;
  } else {
    message = FileAndLineNumber(filepath, root)
              + "Invalid y value.";
    wxLogError(message);
    status = false;
  }

  str = *std::next(content_parsed.cbegin(), 1);
  if (str.ToDouble(&value) == true) {
    point.z = value;
  } else {
    message = FileAndLineNumber(filepath, root)
              + "Invalid z value.";
    wxLogError(message);
    status = false;
  }

  return status;
}
