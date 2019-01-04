// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "appcommon/xml/line_cable_xml_handler.h"

#include "appcommon/xml/cable_constraint_xml_handler.h"
#include "appcommon/xml/vector_xml_handler.h"
#include "models/base/helper.h"

wxXmlNode* LineCableXmlHandler::CreateNode(
    const LineCable& line_cable,
    const std::string& name,
    const units::UnitSystem& units,
    const std::list<const LineStructure*>* line_structures) {
  // variables used to create XML node
  wxXmlNode* node_root = nullptr;
  wxXmlNode* node_element = nullptr;
  std::string title;
  std::string content;
  wxXmlAttribute attribute;

  // creates a node for the root
  node_root = new wxXmlNode(wxXML_ELEMENT_NODE, "line_cable");
  node_root->AddAttribute("version", "1");

  if (name != "") {
    node_root->AddAttribute("name", name);
  }

  // creates cable node and adds to root node
  title = "cable";
  if (line_cable.cable() != nullptr) {
    content = line_cable.cable()->name;
  } else {
    content = *wxEmptyString;
  }
  node_element = CreateElementNodeWithContent(title, content);
  node_root->AddChild(node_element);

  // creates constraint node and adds to root node
  node_element = CableConstraintXmlHandler::CreateNode(
      line_cable.constraint(), "", units);
  node_root->AddChild(node_element);

  // creates spacing-attachments-ruling-span node and adds to root node
  title = "spacing_attachments_ruling_span";
  if (units == units::UnitSystem::kImperial) {
    attribute = wxXmlAttribute("units", "ft");
  } else if (units == units::UnitSystem::kMetric) {
    attribute = wxXmlAttribute("units", "m");
  }
  node_element = Vector3dXmlHandler::CreateNode(
      line_cable.spacing_attachments_ruling_span(), title, attribute, 6);
  node_root->AddChild(node_element);

  // creates weathercase-stretch-creep node and adds to root node
  title = "weather_load_case";
  attribute = wxXmlAttribute("name", "stretch_creep");
  if (line_cable.weathercase_stretch_creep() != nullptr) {
    content = line_cable.weathercase_stretch_creep()->description;
  } else {
    content = *wxEmptyString;
  }
  node_element = CreateElementNodeWithContent(title, content, &attribute);
  node_root->AddChild(node_element);

  // creates weathercase-stretch-creep node and adds to root node
  title = "weather_load_case";
  attribute = wxXmlAttribute("name", "stretch_load");
  if (line_cable.weathercase_stretch_load() != nullptr) {
    content = line_cable.weathercase_stretch_load()->description;
  } else {
    content = *wxEmptyString;
  }
  node_element = CreateElementNodeWithContent(title, content, &attribute);
  node_root->AddChild(node_element);

  // creates connections node and adds to root node
  title = "connections";
  node_element = new wxXmlNode(wxXML_ELEMENT_NODE, title);
  node_element->AddAttribute("format", "structure_index,attach_index");

  for (auto iter = line_cable.connections()->cbegin();
       iter != line_cable.connections()->cend(); iter++) {
    const LineCableConnection& connection = *iter;

    // searches line structure list for a matching line structure
    auto it = line_structures->cbegin();
    while (it != line_structures->cend()) {
      const LineStructure* line_structure = *it;
      if (line_structure == connection.line_structure) {
        break;
      }

      it++;
    }

    // gets structure index
    const int index_structure = std::distance(line_structures->cbegin(), it);
    content = std::to_string(index_structure)
              + "," + std::to_string(connection.index_attachment);

    // creates connection node and adds to connections node
    wxXmlNode* sub_node = XmlHandler::CreateElementNodeWithContent(
        "connection",
        content);
    node_element->AddChild(sub_node);
  }
  node_root->AddChild(node_element);

  // returns root node
  return node_root;
}

bool LineCableXmlHandler::ParseNode(
    const wxXmlNode* root,
    const std::string& filepath,
    const std::list<const Cable*>* cables,
    const std::list<const LineStructure*>* line_structures,
    const std::list<const WeatherLoadCase*>* weathercases,
    LineCable& line_cable) {
  wxString message;

  // checks for valid root node
  if (root->GetName() != "line_cable") {
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
    return ParseNodeV1(root, filepath, cables, line_structures, weathercases,
                       line_cable);
  } else {
    message = FileAndLineNumber(filepath, root) +
              " Invalid version number. Aborting node parse.";
    wxLogError(message);
    return false;
  }
}

bool LineCableXmlHandler::ParseNodeV1(
    const wxXmlNode* root,
    const std::string& filepath,
    const std::list<const Cable*>* cables,
    const std::list<const LineStructure*>* line_structures,
    const std::list<const WeatherLoadCase*>* weathercases,
    LineCable& line_cable) {
  bool status = true;
  wxString message;

  // evaluates each child node
  const wxXmlNode* node = root->GetChildren();
  while (node != nullptr) {
    const wxString title = node->GetName();
    const wxString content = ParseElementNodeWithContent(node);

    if (title == "cable") {
      // initializes the weathercase and attempts to find a match
      line_cable.set_cable(nullptr);
      for (auto iter = cables->cbegin(); iter != cables->cend();
           iter++) {
        const Cable* cable = *iter;
        if (content == cable->name) {
          line_cable.set_cable(cable);
          break;
        }
      }

      // checks if match was found
      if (line_cable.cable() == nullptr) {
        message = FileAndLineNumber(filepath, node)
                  + "Invalid cable. Couldn't find " + content;
        wxLogError(message);
        status = false;
      }
    } else if (title == "cable_constraint") {
      CableConstraint constraint;
      const bool status_node = CableConstraintXmlHandler::ParseNode(
          node, filepath, weathercases, constraint);
      line_cable.set_constraint(constraint);
      if (status_node == false) {
        status = false;
      }
    } else if (title == "vector_3d") {
      Vector3d spacing;
      const bool status_node = Vector3dXmlHandler::ParseNode(
          node, filepath, spacing);
      line_cable.set_spacing_attachments_ruling_span(spacing);
      if (status_node == false) {
        status = false;
      }
    } else if (title == "weather_load_case") {
      const wxString name = node->GetAttribute("name");
      if (name == "stretch_creep") {
        // initializes the weathercase and attempts to find a match
        line_cable.set_weathercase_stretch_creep(nullptr);
        for (auto iter = weathercases->cbegin(); iter != weathercases->cend();
             iter++) {
          const WeatherLoadCase* weathercase = *iter;
          if (content == weathercase->description) {
            line_cable.set_weathercase_stretch_creep(weathercase);
            break;
          }
        }

        // checks if match was found
        if (line_cable.weathercase_stretch_creep() == nullptr) {
          message = FileAndLineNumber(filepath, node)
                    + "Invalid creep stretch weathercase. Couldn't find "
                    + content;
          wxLogError(message);
          status = false;
        }
      } else if (name == "stretch_load") {
        // initializes the weathercase and attempts to find a match
        line_cable.set_weathercase_stretch_load(nullptr);
        for (auto iter = weathercases->cbegin(); iter != weathercases->cend();
             iter++) {
          const WeatherLoadCase* weathercase = *iter;
          if (content == weathercase->description) {
            line_cable.set_weathercase_stretch_load(weathercase);
            break;
          }
        }

        // checks if match was found
        if (line_cable.weathercase_stretch_creep() == nullptr) {
          message = FileAndLineNumber(filepath, node)
                    + "Invalid load stretch weathercase. Couldn't find "
                    + content;
          wxLogError(message);
          status = false;
        }
      }
    } else if (title == "connections") {
      // gets connection sub-nodes
      wxXmlNode* sub_node = node->GetChildren();
      while (sub_node != nullptr) {
        const wxString sub_title = sub_node->GetName();
        const std::string sub_content = ParseElementNodeWithContent(sub_node);
        long value = -9999;

        LineCableConnection connection;

        // parses input string as: structure_index,attach_index
        std::list<std::string> content_parsed = helper::Parse(sub_content, ',');
        wxString str;

        // initializes the line structure reference and assigns to index
        connection.line_structure = nullptr;
        str = *std::next(content_parsed.cbegin(), 0);
        if (str.ToLong(&value) == true) {
          connection.line_structure = *std::next(line_structures->cbegin(), value);
        } else {
          message = FileAndLineNumber(filepath, node)
                    + "Invalid line structure index.";
          wxLogError(message);
          connection.line_structure = nullptr;
          status = false;
          continue;
        }

        str = *std::next(content_parsed.cbegin(), 1);
        if (str.ToLong(&value) == true) {
          connection.index_attachment = value;
        } else {
          message = FileAndLineNumber(filepath, node)
                    + "Invalid attachment index.";
          wxLogError(message);
          connection.index_attachment = -9999;
          status = false;
        }

        // adds to line cable
        if (-1 == line_cable.AddConnection(connection)) {
          status = false;
          message = FileAndLineNumber(filepath, sub_node)
                    + "Could not add connection to line cable.";
          wxLogError(message);
        }

        sub_node = sub_node->GetNext();
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
