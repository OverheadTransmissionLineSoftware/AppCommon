// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "appcommon/xml/transmission_line_xml_handler.h"

#include "appcommon/xml/alignment_xml_handler.h"
#include "appcommon/xml/line_cable_xml_handler.h"
#include "appcommon/xml/line_structure_xml_handler.h"
#include "models/base/helper.h"

wxXmlNode* TransmissionLineXmlHandler::CreateNode(
    const TransmissionLine& line,
    const std::string& name,
    const units::UnitSystem& units) {
  // variables used to create XML node
  wxXmlNode* node_root = nullptr;
  wxXmlNode* node_element = nullptr;
  std::string title;
  std::string content;
  wxXmlAttribute attribute;

  // creates a node for the root
  node_root = new wxXmlNode(wxXML_ELEMENT_NODE, "transmission_line");
  node_root->AddAttribute("version", "1");

  if (name != "") {
    node_root->AddAttribute("name", name);
  }

  // creates an alignment node and adds to root node
  title = "alignment";
  node_element = AlignmentXmlHandler::CreateNode(*line.alignment(), "", units);
  node_root->AddChild(node_element);

  // creates line structures node and adds to root node
  title = "line structures";
  node_element = new wxXmlNode(wxXML_ELEMENT_NODE, title);

  const std::list<LineStructure>* line_structures = line.line_structures();
  for (auto iter = line_structures->cbegin(); iter != line_structures->cend();
       iter++) {
    const LineStructure& line_structure = *iter;
    wxXmlNode* sub_node = LineStructureXmlHandler::CreateNode(line_structure,
                                                              "", units);
    node_element->AddChild(sub_node);
  }
  node_root->AddChild(node_element);

  // creates line cables node and adds to root node
  title = "line cables";
  node_element = new wxXmlNode(wxXML_ELEMENT_NODE, title);

  std::list<const LineStructure*> const_line_structures;
  for (auto iter = line.line_structures()->cbegin();
       iter != line.line_structures()->cend(); iter++) {
    const LineStructure* line_structure = &(*iter);
    const_line_structures.push_back(line_structure);
  }

  const std::list<LineCable>* line_cables = line.line_cables();
  for (auto iter = line_cables->cbegin(); iter != line_cables->cend();
       iter++) {
    const LineCable& line_cable = *iter;
    wxXmlNode* sub_node = LineCableXmlHandler::CreateNode(
        line_cable, "", units, &const_line_structures);
    node_element->AddChild(sub_node);
  }
  node_root->AddChild(node_element);

  // returns node
  return node_root;
}

bool TransmissionLineXmlHandler::ParseNode(
    const wxXmlNode* root,
    const std::string& filepath,
    const std::list<const Structure*>* structures,
    const std::list<const Hardware*>* hardwares,
    const std::list<const Cable*>* cables,
    const std::list<const WeatherLoadCase*>* weathercases,
    TransmissionLine& line) {
  wxString message;

  // checks for valid root node
  if (root->GetName() != "transmission_line") {
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
    return ParseNodeV1(root, filepath, structures, hardwares, cables,
                       weathercases, line);
  } else {
    message = FileAndLineNumber(filepath, root) +
              " Invalid version number. Aborting node parse.";
    wxLogError(message);
    return false;
  }
}

bool TransmissionLineXmlHandler::ParseNodeV1(
    const wxXmlNode* root,
    const std::string& filepath,
    const std::list<const Structure*>* structures,
    const std::list<const Hardware*>* hardwares,
    const std::list<const Cable*>* cables,
    const std::list<const WeatherLoadCase*>* weathercases,
    TransmissionLine& line) {
  bool status = true;
  wxString message;

  // evaluates each child node
  const wxXmlNode* node = root->GetChildren();
  while (node != nullptr) {
    const wxString title = node->GetName();
    const wxString content = ParseElementNodeWithContent(node);
    double value = -999999;

    if (title == "alignment") {
      Alignment alignment;
      const bool status_node = AlignmentXmlHandler::ParseNode(
          node, filepath, alignment);
      if (status_node == false) {
        status = false;
      }

      // adds points to transmission line
      for (auto iter = alignment.points()->cbegin();
           iter != alignment.points()->end(); iter++) {
        const AlignmentPoint& point = *iter;
        line.AddAlignmentPoint(point);
      }
    } else if (title == "line_structures") {
      // gets attachment sub-nodes
      wxXmlNode* sub_node = node->GetChildren();
      while (sub_node != nullptr) {
        // creates a new line structure
        LineStructure line_structure;
        const bool status_node = LineStructureXmlHandler::ParseNode(
            sub_node, filepath, structures, hardwares, line_structure);
        if (status_node == false) {
          status = false;
        }

        // adds to transmission line
        line.AddLineStructure(line_structure);

        sub_node = sub_node->GetNext();
      }
    } else if (title == "line_cables") {
      // creates a temporary list of line structure references
      std::list<const LineStructure*> const_line_structures;
      for (auto iter = line.line_structures()->cbegin();
           iter != line.line_structures()->cend(); iter++) {
        const LineStructure* line_structure = &(*iter);
        const_line_structures.push_back(line_structure);
      }

      // gets attachment sub-nodes
      wxXmlNode* sub_node = node->GetChildren();
      while (sub_node != nullptr) {
        // creates a new line structure
        LineCable line_cable;
        const bool status_node = LineCableXmlHandler::ParseNode(
            sub_node, filepath, cables, &const_line_structures, weathercases,
            line_cable);
        if (status_node == false) {
          status = false;
        }

        // adds to transmission line
        line.AddLineCable(line_cable);

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
