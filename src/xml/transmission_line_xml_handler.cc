// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "appcommon/xml/transmission_line_xml_handler.h"

#include "appcommon/units/transmission_line_unit_converter.h"
#include "appcommon/xml/line_cable_xml_handler.h"
#include "appcommon/xml/line_structure_xml_handler.h"
#include "models/base/helper.h"

wxXmlNode* TransmissionLineXmlHandler::CreateNode(
    const TransmissionLine& line,
    const std::string& name,
    const units::UnitSystem& system_units,
    const units::UnitStyle& style_units) {
  // variables used to create XML node
  wxXmlNode* node_root = nullptr;
  wxXmlNode* node_element = nullptr;
  wxXmlAttribute attribute;
  std::string title;
  std::string content;

  // creates a node for the root
  node_root = new wxXmlNode(wxXML_ELEMENT_NODE, "transmission_line");
  node_root->AddAttribute("version", "1");

  if (name != "") {
    node_root->AddAttribute("name", name);
  }

  // creates an alignment node and adds to root node
  title = "alignment";
  node_element = new wxXmlNode(wxXML_ELEMENT_NODE, title);
  node_element->AddAttribute("format", "station,elevation,rotation");

  // creates alignment point nodes and adds to alignment node
  if (system_units == units::UnitSystem::kMetric) {
    attribute = wxXmlAttribute("units", "m,m,deg");
  } else if (system_units == units::UnitSystem::kImperial) {
    attribute = wxXmlAttribute("units", "ft,ft,deg");
  } else {
    attribute = wxXmlAttribute();
  }

  const std::list<AlignmentPoint>* points = line.alignment()->points();
  for (auto iter = points->cbegin(); iter != points->cend();
       iter++) {
    const AlignmentPoint& point = *iter;
    content = helper::DoubleToString(point.station, 3, true)
              + "," + helper::DoubleToString(point.elevation, 3, true)
              + "," + helper::DoubleToString(point.rotation, 3, true);

    wxXmlNode* sub_node = XmlHandler::CreateElementNodeWithContent(
        "point",
        content,
        &attribute);
    node_element->AddChild(sub_node);
  }
  node_root->AddChild(node_element);

  // creates line structures node and adds to root node
  title = "line_structures";
  node_element = new wxXmlNode(wxXML_ELEMENT_NODE, title);

  const std::list<LineStructure>* line_structures = line.line_structures();
  for (auto iter = line_structures->cbegin(); iter != line_structures->cend();
       iter++) {
    const LineStructure& line_structure = *iter;
    wxXmlNode* sub_node = LineStructureXmlHandler::CreateNode(
        line_structure, "", system_units, style_units);
    node_element->AddChild(sub_node);
  }
  node_root->AddChild(node_element);

  // creates line cables node and adds to root node
  title = "line_cables";
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
        line_cable, "", system_units, style_units, &const_line_structures);
    node_element->AddChild(sub_node);
  }
  node_root->AddChild(node_element);

  // returns root node
  return node_root;
}

bool TransmissionLineXmlHandler::ParseNode(
    const wxXmlNode* root,
    const std::string& filepath,
    const units::UnitSystem& units,
    const bool& convert,
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
  const int kVersion = Version(root);
  if (kVersion == -1) {
    message = FileAndLineNumber(filepath, root) +
              " Version attribute is missing or invalid. Aborting node parse.";
    wxLogError(message);
    return false;
  }

  // sends to proper parsing function
  if (kVersion == 1) {
    return ParseNodeV1(root, filepath, units, convert, structures, hardwares,
                       cables, weathercases, line);
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
    const units::UnitSystem& units,
    const bool& convert,
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

    if (title == "alignment") {
      // evaluates each child node
      const wxXmlNode* sub_node = node->GetChildren();
      while (sub_node != nullptr) {
        const wxString sub_title = sub_node->GetName();
        const std::string sub_content = ParseElementNodeWithContent(sub_node);
        double value = -999999;

        if (sub_title == "point") {
          AlignmentPoint point;

          // parses input string as: station,elevation,rotation
          std::list<std::string> content_parsed = helper::Parse(sub_content,
                                                                ',');
          wxString str;

          // reads station
          auto iter = content_parsed.cbegin();
          str = *iter;
          if (str.ToDouble(&value) == true) {
            point.station = value;
          } else {
            message = FileAndLineNumber(filepath, sub_node)
                      + "Invalid station.";
            wxLogError(message);
            point.station = -999999;
            status = false;
          }

          // reads elevation
          iter++;
          str = *iter;
          if (str.ToDouble(&value) == true) {
            point.elevation = value;
          } else {
            message = FileAndLineNumber(filepath, sub_node)
                      + "Invalid elevation.";
            wxLogError(message);
            point.elevation = -999999;
            status = false;
          }

          // reads rotation
          iter++;
          str = *iter;
          if (str.ToDouble(&value) == true) {
            point.rotation = value;
          } else {
            message = FileAndLineNumber(filepath, sub_node)
                      + "Invalid rotation.";
            wxLogError(message);
            point.rotation = -999999;
            status = false;
          }

          // adds to transmission line
          if (-1 == line.AddAlignmentPoint(point)) {
            status = false;
            message = FileAndLineNumber(filepath, sub_node)
                      + "Could not add alignment point to transmission line.";
            wxLogError(message);
          }
        } else {
          message = FileAndLineNumber(filepath, sub_node)
                    + "XML node isn't recognized.";
          wxLogError(message);
          status = false;
        }

        sub_node = sub_node->GetNext();
      }
    } else if (title == "line_structures") {
      // gets attachment sub-nodes
      wxXmlNode* sub_node = node->GetChildren();
      while (sub_node != nullptr) {
        // creates a new line structure
        LineStructure line_structure;
        const bool status_node = LineStructureXmlHandler::ParseNode(
            sub_node, filepath, units, convert, structures, hardwares,
            line_structure);
        if (status_node == false) {
          status = false;
        }

        // adds to transmission line
        if (-1 == line.AddLineStructure(line_structure)) {
          status = false;
          message = FileAndLineNumber(filepath, sub_node)
                    + "Could not add line structure to transmission line.";
          wxLogError(message);
        }

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
            sub_node, filepath, units, convert, cables, &const_line_structures,
            weathercases, line_cable);
        if (status_node == false) {
          status = false;
        }

        // adds to transmission line
        if (-1 == line.AddLineCable(line_cable)) {
          status = false;
          message = FileAndLineNumber(filepath, sub_node)
                    + "Could not add line cable to transmission line.";
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

  // converts unit style to 'consistent' if needed
  if (convert == true) {
    TransmissionLineUnitConverter::ConvertUnitStyleToConsistent(1, units, false,
                                                                line);
  }

  return status;
}
