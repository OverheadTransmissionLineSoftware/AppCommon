// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "appcommon/xml/line_structure_xml_handler.h"

#include "models/base/helper.h"

wxXmlNode* LineStructureXmlHandler::CreateNode(
    const LineStructure& line_structure,
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
  node_root = new wxXmlNode(wxXML_ELEMENT_NODE, "line_structure");
  node_root->AddAttribute("version", "1");

  if (name != "") {
    node_root->AddAttribute("name", name);
  }

  // creates structure node and adds to root node
  title = "structure";
  if (line_structure.structure() != nullptr) {
    content = line_structure.structure()->name;
  } else {
    content = *wxEmptyString;
  }
  node_element = CreateElementNodeWithContent(title, content);
  node_root->AddChild(node_element);

  // creates station node and adds to root node
  title = "station";
  value = line_structure.station();
  content = helper::DoubleToString(value, 6);
  if (units == units::UnitSystem::kImperial) {
    attribute = wxXmlAttribute("units", "ft");
  } else if (units == units::UnitSystem::kMetric) {
    attribute = wxXmlAttribute("units", "m");
  }
  node_element = CreateElementNodeWithContent(title, content, &attribute);
  node_root->AddChild(node_element);

  // creates offset node and adds to root node
  title = "offset";
  value = line_structure.offset();
  content = helper::DoubleToString(value, 6);
  if (units == units::UnitSystem::kImperial) {
    attribute = wxXmlAttribute("units", "ft");
  } else if (units == units::UnitSystem::kMetric) {
    attribute = wxXmlAttribute("units", "m");
  }
  node_element = CreateElementNodeWithContent(title, content, &attribute);
  node_root->AddChild(node_element);

  // creates rotation node and adds to root node
  title = "rotation";
  value = line_structure.rotation();
  content = helper::DoubleToString(value, 6);
  if (units == units::UnitSystem::kImperial) {
    attribute = wxXmlAttribute("units", "ft");
  } else if (units == units::UnitSystem::kMetric) {
    attribute = wxXmlAttribute("units", "m");
  }
  node_element = CreateElementNodeWithContent(title, content, &attribute);
  node_root->AddChild(node_element);

  // creates height adjustment node and adds to root node
  title = "height_adjustment";
  value = line_structure.height_adjustment();
  content = helper::DoubleToString(value, 6);
  if (units == units::UnitSystem::kImperial) {
    attribute = wxXmlAttribute("units", "ft");
  } else if (units == units::UnitSystem::kMetric) {
    attribute = wxXmlAttribute("units", "m");
  }
  node_element = CreateElementNodeWithContent(title, content, &attribute);
  node_root->AddChild(node_element);

  // creates hardwares node and adds to root node
  title = "hardwares";
  node_element = new wxXmlNode(wxXML_ELEMENT_NODE, title);

  const std::vector<const Hardware*>* hardwares = line_structure.hardwares();
  for (auto iter = hardwares->cbegin(); iter != hardwares->cend(); iter++) {
    const Hardware* hardware = *iter;
    wxXmlNode* sub_node;
    if (hardware == nullptr) {
      sub_node = CreateElementNodeWithContent("hardware", "");
    } else {
      sub_node = CreateElementNodeWithContent("hardware", hardware->name);
    }
    node_element->AddChild(sub_node);
  }
  node_root->AddChild(node_element);

  // returns root node
  return node_root;
}

bool LineStructureXmlHandler::ParseNode(
    const wxXmlNode* root,
    const std::string& filepath,
    const std::list<const Structure*>* structures,
    const std::list<const Hardware*>* hardwares,
    LineStructure& line_structure) {
  wxString message;

  // checks for valid root node
  if (root->GetName() != "line_structure") {
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
    return ParseNodeV1(root, filepath, structures, hardwares, line_structure);
  } else {
    message = FileAndLineNumber(filepath, root) +
              " Invalid version number. Aborting node parse.";
    wxLogError(message);
    return false;
  }
}

bool LineStructureXmlHandler::ParseNodeV1(
    const wxXmlNode* root,
    const std::string& filepath,
    const std::list<const Structure*>* structures,
    const std::list<const Hardware*>* hardwares,
    LineStructure& line_structure) {
  bool status = true;
  wxString message;

  // evaluates each child node
  const wxXmlNode* node = root->GetChildren();
  while (node != nullptr) {
    const wxString title = node->GetName();
    const wxString content = ParseElementNodeWithContent(node);
    double value = -999999;

    if (title == "structure") {
      // initializes the structure and attempts to find a match
      line_structure.set_structure(nullptr);
      for (auto iter = structures->cbegin(); iter != structures->cend();
           iter++) {
        const Structure* structure = *iter;
        if (content == structure->name) {
          line_structure.set_structure(structure);
          break;
        }
      }

      // checks if match was found
      if (line_structure.structure() == nullptr) {
        message = FileAndLineNumber(filepath, node)
                  + "Invalid structure. Couldn't find " + content;
        wxLogError(message);
        status = false;
      }
    } else if (title == "station") {
      if (content.ToDouble(&value) == true) {
        line_structure.set_station(value);
      } else {
        message = FileAndLineNumber(filepath, node)
                  + "Invalid station.";
        wxLogError(message);
        line_structure.set_station(-999999);
        status = false;
      }
    } else if (title == "offset") {
      if (content.ToDouble(&value) == true) {
        line_structure.set_offset(value);
      } else {
        message = FileAndLineNumber(filepath, node)
                  + "Invalid offset.";
        wxLogError(message);
        line_structure.set_offset(-999999);
        status = false;
      }
    } else if (title == "rotation") {
      if (content.ToDouble(&value) == true) {
        line_structure.set_rotation(value);
      } else {
        message = FileAndLineNumber(filepath, node)
                  + "Invalid rotation.";
        wxLogError(message);
        line_structure.set_rotation(-999999);
        status = false;
      }
    } else if (title == "height_adjustment") {
      if (content.ToDouble(&value) == true) {
        line_structure.set_height_adjustment(value);
      } else {
        message = FileAndLineNumber(filepath, node)
                  + "Invalid height adjustment.";
        wxLogError(message);
        line_structure.set_height_adjustment(-999999);
        status = false;
      }
    } else if (title == "hardwares") {
      // gets hardware sub-nodes
      wxXmlNode* sub_node = node->GetChildren();
      int index = 0;
      while (sub_node != nullptr) {
        // gets content of a hardware node
        const wxString sub_content = ParseElementNodeWithContent(sub_node);
        if (sub_content != wxEmptyString) {
          // searches hardware list for a match
          for (auto iter = hardwares->cbegin(); iter != hardwares->cend();
               iter++) {
            const Hardware* hardware = *iter;
            if (sub_content == hardware->name) {
              // match is found, attaches to line structure
              line_structure.AttachHardware(index, hardware);
              break;
            }
          }
        }

        sub_node = sub_node->GetNext();
        index++;
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
