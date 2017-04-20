// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "appcommon/xml/alignment_xml_handler.h"

#include "models/base/helper.h"

wxXmlNode* AlignmentXmlHandler::CreateNode(
    const Alignment& alignment,
    const std::string& name,
    const units::UnitSystem& units) {
  // variables used to create XML node
  wxXmlNode* node_root = nullptr;
  wxXmlNode* node_element = nullptr;
  std::string title;
  std::string content;
  wxXmlAttribute attribute;
  double value = -999999;

  // creates a node for the alignment point root
  node_root = new wxXmlNode(wxXML_ELEMENT_NODE, "alignment");
  node_root->AddAttribute("version", "1");

  if (name != "") {
    node_root->AddAttribute("name", name);
  }

  // creates alignment point nodes and adds to root node
  if (units == units::UnitSystem::kMetric) {
    attribute = wxXmlAttribute("units", "ft,ft,deg");
  } else if (units == units::UnitSystem::kImperial) {
    attribute = wxXmlAttribute("units", "m,m,deg");
  } else {
    attribute = wxXmlAttribute();
  }

  const std::list<AlignmentPoint>* points = alignment.points();
  for (auto iter = points->cbegin(); iter != points->cend();
       iter++) {
    const AlignmentPoint& point = *iter;
    content = helper::DoubleToFormattedString(point.station, 2)
              + "," + helper::DoubleToFormattedString(point.elevation, 2)
              + ";" + helper::DoubleToFormattedString(point.rotation, 2);
    wxXmlNode* node_element = XmlHandler::CreateElementNodeWithContent(
        "point",
        content,
        &attribute);
    node_root->AddChild(node_element);
  }

  // returns node
  return node_root;
}

bool AlignmentXmlHandler::ParseNode(
    const wxXmlNode* root,
    const std::string& filepath,
    Alignment& alignment) {
  wxString message;

  // checks for valid root node
  if (root->GetName() != "alignment") {
    message = FileAndLineNumber(filepath, root) +
              " Invalid root node. Aborting node parse.";
    wxLogError(message);
    return false;
  }

  // gets version attribute
  wxString version;
  if (root->GetAttribute("version", &version) == false) {
    message = FileAndLineNumber(filepath, root) +
              " Version attribute is missing. Aborting node parse.";
    wxLogError(message);
    return false;
  }

  // sends to proper parsing function
  if (version == "1") {
    return ParseNodeV1(root, filepath, alignment);
  } else {
    message = FileAndLineNumber(filepath, root) +
              " Invalid version number. Aborting node parse.";
    wxLogError(message);
    return false;
  }
}

bool AlignmentXmlHandler::ParseNodeV1(
    const wxXmlNode* root,
    const std::string& filepath,
    Alignment& alignment) {
  bool status = true;
  wxString message;

  // evaluates each child node
  const wxXmlNode* node = root->GetChildren();
  while (node != nullptr) {
    const wxString title = node->GetName();
    const std::string content = ParseElementNodeWithContent(node);
    double value = -999999;

    if (title == "point") {
      AlignmentPoint point;

      std::list<std::string> content_parsed = helper::Parse(content, ',');
      wxString str;

      auto iter = content_parsed.cbegin();
      str = *iter;
      if (str.ToDouble(&value) == true) {
        point.station = value;
      } else {
        message = FileAndLineNumber(filepath, node)
                  + "Invalid station.";
        wxLogError(message);
        point.station = -999999;
        status = false;
      }

      iter++;
      str = *iter;
      if (str.ToDouble(&value) == true) {
        point.elevation = value;
      } else {
        message = FileAndLineNumber(filepath, node)
                  + "Invalid elevation.";
        wxLogError(message);
        point.elevation = -999999;
        status = false;
      }

      iter++;
      str = *iter;
      if (str.ToDouble(&value) == true) {
        point.rotation = value;
      } else {
        message = FileAndLineNumber(filepath, node)
                  + "Invalid rotation.";
        wxLogError(message);
        point.rotation = -999999;
        status = false;
      }
    } else {
      message = FileAndLineNumber(filepath, node)
                + "XML node isn't recognized.";
      wxLogError(message);
      status = false;
    }

    node = node->GetNext();
  }

  return status;
}
