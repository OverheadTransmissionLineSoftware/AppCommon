// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "appcommon/xml/hardware_xml_handler.h"

#include "models/base/helper.h"

wxXmlNode* HardwareXmlHandler::CreateNode(
    const Hardware& hardware,
    const std::string& name,
    const units::UnitSystem& units) {
  // variables used to create XML node
  wxXmlNode* node_root = nullptr;
  wxXmlNode* node_element = nullptr;
  std::string title;
  std::string content;
  wxXmlAttribute attribute;
  double value = -999999;

  // creates a node for the root
  node_root = new wxXmlNode(wxXML_ELEMENT_NODE, "hardware");
  node_root->AddAttribute("version", "1");

  if (name != "") {
    node_root->AddAttribute("name", name);
  }

  // creates name node and adds to root node
  title = "name";
  content = hardware.name;
  node_element = CreateElementNodeWithContent(title, content);
  node_root->AddChild(node_element);

  // creates type node and adds to root node
  title = "type";
  if (hardware.type == Hardware::HardwareType::kDeadEnd) {
    content = "DeadEnd";
  } else if (hardware.type == Hardware::HardwareType::kSuspension) {
    content = "Suspension";
  }
  node_element = CreateElementNodeWithContent(title, content);
  node_root->AddChild(node_element);

  // creates area-cross-section node and adds to root node
  title = "area_cross_section";
  value = hardware.area_cross_section;
  content = helper::DoubleToString(value, 6);
  if (units == units::UnitSystem::kImperial) {
    attribute = wxXmlAttribute("units", "ft^2");
  } else if (units == units::UnitSystem::kMetric) {
    attribute = wxXmlAttribute("units", "m^2");
  }
  node_element = CreateElementNodeWithContent(title, content, &attribute);
  node_root->AddChild(node_element);

  // creates length node and adds to root node
  title = "length";
  value = hardware.length;
  content = helper::DoubleToString(value, 6);
  if (units == units::UnitSystem::kImperial) {
    attribute = wxXmlAttribute("units", "ft");
  } else if (units == units::UnitSystem::kMetric) {
    attribute = wxXmlAttribute("units", "m");
  }
  node_element = CreateElementNodeWithContent(title, content, &attribute);
  node_root->AddChild(node_element);

  // creates weight node and adds to root node
  title = "weight";
  value = hardware.weight;
  content = helper::DoubleToString(value, 6);
  if (units == units::UnitSystem::kImperial) {
    attribute = wxXmlAttribute("units", "lbs");
  } else if (units == units::UnitSystem::kMetric) {
    attribute = wxXmlAttribute("units", "N");
  }
  node_element = CreateElementNodeWithContent(title, content, &attribute);
  node_root->AddChild(node_element);

  // returns root node
  return node_root;
}

bool HardwareXmlHandler::ParseNode(
    const wxXmlNode* root,
    const std::string& filepath,
    Hardware& hardware) {
  wxString message;

  // checks for valid root node
  if (root->GetName() != "hardware") {
    message = FileAndLineNumber(filepath, root) +
              " Invalid root node. Aborting node parse.";
    wxLogError(message);
    return false;
  }

  // gets version attribute
  int version = Version(root);
  if (version == -1) {
    message = FileAndLineNumber(filepath, root) +
              " Version attribute is missing or invalid. Aborting node parse.";
    wxLogError(message);
    return false;
  }

  // sends to proper parsing function
  if (version == 1) {
    return ParseNodeV1(root, filepath, hardware);
  } else {
    message = FileAndLineNumber(filepath, root) +
              " Invalid version number. Aborting node parse.";
    wxLogError(message);
    return false;
  }
}

bool HardwareXmlHandler::ParseNodeV1(
    const wxXmlNode* root,
    const std::string& filepath,
    Hardware& hardware) {
  bool status = true;
  wxString message;

  // evaluates each child node
  const wxXmlNode* node = root->GetChildren();
  while (node != nullptr) {
    const wxString title = node->GetName();
    const wxString content = ParseElementNodeWithContent(node);
    double value = -999999;

    if (title == "name") {
      hardware.name = content;
    } else if (title == "type") {
      if (content == "DeadEnd") {
        hardware.type = Hardware::HardwareType::kDeadEnd;
      } else if (content == "Suspension") {
        hardware.type = Hardware::HardwareType::kSuspension;
      } else {
        message = FileAndLineNumber(filepath, node)
                  + "Invalid type.";
        wxLogError(message);
        status = false;
      }
    } else if (title == "area_cross_section") {
      if (content.ToDouble(&value) == true) {
        hardware.area_cross_section = value;
      } else {
        message = FileAndLineNumber(filepath, node)
                  + "Invalid cross sectional area.";
        wxLogError(message);
        hardware.area_cross_section = -999999;
        status = false;
      }
    } else if (title == "length") {
      if (content.ToDouble(&value) == true) {
        hardware.length = value;
      } else {
        message = FileAndLineNumber(filepath, node)
                  + "Invalid length.";
        wxLogError(message);
        hardware.length = -999999;
        status = false;
      }
    } else if (title == "weight") {
      if (content.ToDouble(&value) == true) {
        hardware.weight = value;
      } else {
        message = FileAndLineNumber(filepath, node)
                  + "Invalid weight.";
        wxLogError(message);
        hardware.weight = -999999;
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
