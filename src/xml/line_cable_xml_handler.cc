// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "appcommon/xml/line_cable_xml_handler.h"

#include "appcommon/xml/cable_constraint_xml_handler.h"
#include "appcommon/xml/vector_xml_handler.h"

wxXmlNode* LineCableConnectionXmlHandler::CreateNode(
    const LineCableConnection& connection,
    const std::string& name,
    const std::list<const LineStructure*>* line_structures) {
  // variables used to create XML node
  wxXmlNode* node_root = nullptr;
  wxXmlNode* node_element = nullptr;
  std::string title;
  std::string content;
  wxXmlAttribute attribute;

  // creates a node for the cable constraint root
  node_root = new wxXmlNode(wxXML_ELEMENT_NODE, "line_cable_connection");
  node_root->AddAttribute("version", "1");

  if (name != "") {
    node_root->AddAttribute("name", name);
  }

  // creates index_structure node and adds to parent node
  title = "index_structure";
  if (connection.line_structure != nullptr) {
    // searches line structure list for a matching line structure
    auto iter = line_structures->cbegin();
    while (iter != line_structures->cend()) {
      const LineStructure* line_structure = *iter;
      if (line_structure == connection.line_structure) {
        break;
      }

      iter++;
    }

    // gets index, or leaves blank if not found
    if (iter != line_structures->cend()) {
      content = std::distance(line_structures->cbegin(), iter);
    } else {
      content = *wxEmptyString;
    }
  } else {
    content = *wxEmptyString;
  }
  node_element = CreateElementNodeWithContent(title, content);
  node_root->AddChild(node_element);

  // creates index_attachment node and adds to parent node
  title = "index_attachment";
  content = connection.index_attachment;
  node_element = CreateElementNodeWithContent(title, content);
  node_root->AddChild(node_element);

  // returns node
  return node_root;
}

bool LineCableConnectionXmlHandler::ParseNode(
    const wxXmlNode* root,
    const std::string& filepath,
    const std::list<const LineStructure*>* line_structures,
    LineCableConnection& connection) {
  wxString message;

  // checks for valid root node
  if (root->GetName() != "line_cable_connection") {
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
    return ParseNodeV1(root, filepath, line_structures, connection);
  } else {
    message = FileAndLineNumber(filepath, root) +
              " Invalid version number. Aborting node parse.";
    wxLogError(message);
    return false;
  }
}

bool LineCableConnectionXmlHandler::ParseNodeV1(
    const wxXmlNode* root,
    const std::string& filepath,
    const std::list<const LineStructure*>* line_structures,
    LineCableConnection& connection) {
  bool status = true;
  wxString message;
  long value;

  // evaluates each child node
  const wxXmlNode* node = root->GetChildren();
  while (node != nullptr) {
    const wxString title = node->GetName();
    const wxString content = ParseElementNodeWithContent(node);

    if (title == "index_structure") {
      // initializes the line structure reference and assigns to index
      connection.line_structure = nullptr;
      long index = content.ToLong(&index);

      // sets pointer from list
      const int kSize = line_structures->size();
      if ((0 < index) && (index < kSize)) {
        connection.line_structure = *std::next(line_structures->cbegin(),
                                               index);
      } else {
        // invalid index
        message = FileAndLineNumber(filepath, node)
                  + "Invalid line structure index. Couldn't find " + content;
        wxLogError(message);
        status = false;
      }
    } else if (title == "index_attachment") {
      if (content.ToLong(&value) == true) {
        connection.index_attachment = value;
      } else {
        message = FileAndLineNumber(filepath, node)
                  + "Invalid attachment index.";
        wxLogError(message);
        connection.index_attachment = -9999;
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

  // adds child nodes for struct parameters

  // creates cable node and adds to parent node
  title = "cable";
  if (line_cable.cable() != nullptr) {
    content = line_cable.cable()->name;
  } else {
    content = *wxEmptyString;
  }
  node_element = CreateElementNodeWithContent(title, content);
  node_root->AddChild(node_element);

  // creates constraint node and adds to parent node
  node_element = CableConstraintXmlHandler::CreateNode(
      line_cable.constraint(), "", units);
  node_root->AddChild(node_element);

  // creates spacing-attachments-ruling-span node and adds to parent node
  title = "spacing_attachments_ruling_span";
  if (units == units::UnitSystem::kImperial) {
    attribute = wxXmlAttribute("units", "ft");
  } else if (units == units::UnitSystem::kMetric) {
    attribute = wxXmlAttribute("units", "m");
  }
  node_element = Vector3dXmlHandler::CreateNode(
      line_cable.spacing_attachments_ruling_span(), title, attribute, 1);
  node_root->AddChild(node_element);

  // creates weathercase-stretch-creep node and adds to parent node
  title = "weather_load_case";
  attribute = wxXmlAttribute("name", "stretch_creep");
  if (line_cable.weathercase_stretch_creep() != nullptr) {
    content = line_cable.weathercase_stretch_creep()->description;
  } else {
    content = *wxEmptyString;
  }
  node_element = CreateElementNodeWithContent(title, content, &attribute);
  node_root->AddChild(node_element);

  // creates weathercase-stretch-creep node and adds to parent node
  title = "weather_load_case";
  attribute = wxXmlAttribute("name", "stretch_load");
  if (line_cable.weathercase_stretch_load() != nullptr) {
    content = line_cable.weathercase_stretch_load()->description;
  } else {
    content = *wxEmptyString;
  }
  node_element = CreateElementNodeWithContent(title, content, &attribute);
  node_root->AddChild(node_element);

  // creates connections node and adds to parent node
  title = "connections";
  node_element = new wxXmlNode(wxXML_ELEMENT_NODE, title);

  for (auto iter = line_cable.connections()->cbegin();
       iter != line_cable.connections()->cend(); iter++) {
    const LineCableConnection& connection = *iter;
    wxXmlNode* sub_node =
        LineCableConnectionXmlHandler::CreateNode(connection,
                                                  "",
                                                  line_structures);
    node_element->AddChild(sub_node);
  }
  node_root->AddChild(node_element);

  // returns node
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
        LineCableConnection connection;
        const bool status_node = LineCableConnectionXmlHandler::ParseNode(
            node, filepath, line_structures, connection);
        if (status_node == false) {
          status = false;
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
