// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "appcommon/xml/structure_xml_handler.h"

#include "models/base/helper.h"

wxXmlNode* StructureAttachmentXmlHandler::CreateNode(
    const StructureAttachment& attachment,
    const std::string& name,
    const units::UnitSystem& units) {
  // variables used to create XML node
  wxXmlNode* node_root = nullptr;
  wxXmlNode* node_element = nullptr;
  std::string title;
  std::string content;
  wxXmlAttribute attribute;
  double value;

  // creates a node for the root
  node_root = new wxXmlNode(wxXML_ELEMENT_NODE, "structure_attachment");
  node_root->AddAttribute("version", "1");

  if (name != "") {
    node_root->AddAttribute("name", name);
  }

  // creates offset-longitudinal node and adds to root node
  title = "offset_longitudinal";
  value = attachment.offset_longitudinal;
  content = helper::DoubleToString(value, 6);
  if (units == units::UnitSystem::kImperial) {
    attribute = wxXmlAttribute("units", "ft");
  } else if (units == units::UnitSystem::kMetric) {
    attribute = wxXmlAttribute("units", "m");
  }
  node_element = CreateElementNodeWithContent(title, content, &attribute);
  node_root->AddChild(node_element);

  // creates offset-transverse node and adds to root node
  title = "offset_transverse";
  value = attachment.offset_transverse;
  content = helper::DoubleToString(value, 6);
  if (units == units::UnitSystem::kImperial) {
    attribute = wxXmlAttribute("units", "ft");
  } else if (units == units::UnitSystem::kMetric) {
    attribute = wxXmlAttribute("units", "m");
  }
  node_element = CreateElementNodeWithContent(title, content, &attribute);
  node_root->AddChild(node_element);

  // creates offset-vertical-top node and adds to root node
  title = "offset_vertical_top";
  value = attachment.offset_vertical_top;
  content = helper::DoubleToString(value, 6);
  if (units == units::UnitSystem::kImperial) {
    attribute = wxXmlAttribute("units", "ft");
  } else if (units == units::UnitSystem::kMetric) {
    attribute = wxXmlAttribute("units", "m");
  }
  node_element = CreateElementNodeWithContent(title, content, &attribute);
  node_root->AddChild(node_element);

  // returns root node
  return node_root;
}

bool StructureAttachmentXmlHandler::ParseNode(const wxXmlNode* root,
                                              const std::string& filepath,
                                              StructureAttachment& attachment) {
  wxString message;

  // checks for valid root node
  if (root->GetName() != "structure_attachment") {
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
    return ParseNodeV1(root, filepath, attachment);
  } else {
    message = FileAndLineNumber(filepath, root) +
              " Invalid version number. Aborting node parse.";
    wxLogError(message);
    return false;
  }
}

bool StructureAttachmentXmlHandler::ParseNodeV1(
    const wxXmlNode* root,
    const std::string& filepath,
    StructureAttachment& attachment) {
  bool status = true;
  wxString message;

  // evaluates each child node
  const wxXmlNode* node = root->GetChildren();
  while (node != nullptr) {
    const wxString title = node->GetName();
    const wxString content = ParseElementNodeWithContent(node);
    double value = -999999;

    if (title == "offset_longitudinal") {
      if (content.ToDouble(&value) == true) {
        attachment.offset_longitudinal = value;
      } else {
        message = FileAndLineNumber(filepath, node)
                  + "Invalid longitudinal offset.";
        wxLogError(message);
        attachment.offset_longitudinal = -999999;
        status = false;
      }
    } else if (title == "offset_transverse") {
      if (content.ToDouble(&value) == true) {
        attachment.offset_transverse = value;
      } else {
        message = FileAndLineNumber(filepath, node)
                  + "Invalid transverse offset.";
        wxLogError(message);
        attachment.offset_transverse = -999999;
        status = false;
      }
    } else if (title == "offset_vertical_top") {
      if (content.ToDouble(&value) == true) {
        attachment.offset_vertical_top = value;
      } else {
        message = FileAndLineNumber(filepath, node)
                  + "Invalid vertical top offset.";
        wxLogError(message);
        attachment.offset_vertical_top = -999999;
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


wxXmlNode* StructureXmlHandler::CreateNode(const Structure& structure,
                                           const std::string& name,
                                           const units::UnitSystem& units) {
  // variables used to create XML node
  double value = -999999;
  std::string title;
  std::string content;
  wxXmlAttribute attribute;
  wxXmlNode* node_root = nullptr;
  wxXmlNode* node_element = nullptr;

  // creates a root node for the cable
  node_root = new wxXmlNode(wxXML_ELEMENT_NODE, "structure");
  node_root->AddAttribute("version", "1");

  if (name != "") {
    node_root->AddAttribute("name", name);
  }

  // creates name node and adds to root node
  title = "name";
  content = structure.name;
  node_element = CreateElementNodeWithContent(title, content);
  node_root->AddChild(node_element);

  // creates height node and adds to root node
  title = "height";
  value = structure.height;
  content = helper::DoubleToString(value, 6);
  if (units == units::UnitSystem::kMetric) {
    attribute = wxXmlAttribute("units", "mm^2");
  } else if (units == units::UnitSystem::kImperial) {
    attribute = wxXmlAttribute("units", "in^2");
  }
  node_element = CreateElementNodeWithContent(title, content, &attribute);
  node_root->AddChild(node_element);

  // creates attachments node and adds to root node
  title = "attachments";
  node_element = new wxXmlNode(wxXML_ELEMENT_NODE, title);

  for (auto iter = structure.attachments.cbegin();
       iter != structure.attachments.cend(); iter++) {
    const StructureAttachment& attachment = *iter;
    wxXmlNode* sub_node = StructureAttachmentXmlHandler::CreateNode(attachment,
                                                                    "",
                                                                    units);
    node_element->AddChild(sub_node);
  }
  node_root->AddChild(node_element);

  return node_root;
}

bool StructureXmlHandler::ParseNode(const wxXmlNode* root,
                                    const std::string& filepath,
                                    Structure& structure) {
  wxString message;

  // checks for valid node
  if (root->GetName() != "structure") {
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
    return StructureXmlHandler::ParseNodeV1(root, filepath, structure);
  } else {
    message = FileAndLineNumber(filepath, root) +
              " Invalid version number. Aborting node parse.";
    wxLogError(message);
    return false;
  }
}

bool StructureXmlHandler::ParseNodeV1(const wxXmlNode* root,
                                      const std::string& filepath,
                                      Structure& structure) {
  // variables used to parse XML node
  bool status = true;
  wxString title;
  wxString content;
  double value = -999999;

  wxString message;

  // evaluates each child node
  const wxXmlNode* node = root->GetChildren();
  while (node != nullptr) {
    title = node->GetName();
    content = ParseElementNodeWithContent(node);

    if (title == "name") {
      structure.name = content;
    } else if (title == "height") {
      if (content.ToDouble(&value) == true) {
        structure.height = value;
      } else {
        message = FileAndLineNumber(filepath, node)
                  + "Invalid height.";
        wxLogError(message);
        structure.height = -999999;
        status = false;
      }
    } else if (title == "attachments") {
      std::vector<StructureAttachment>& attachments = structure.attachments;

      // gets attachment sub-nodes
      wxXmlNode* sub_node = node->GetChildren();
      while (sub_node != nullptr) {
        // creates a new attachment
        StructureAttachment attachment;
        const bool status_node = StructureAttachmentXmlHandler::ParseNode(
            sub_node, filepath, attachment);
        if (status_node == false) {
          status = false;
        }

        // adds to attachment vector
        attachments.push_back(attachment);

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
