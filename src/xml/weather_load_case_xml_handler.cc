// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "appcommon/xml/weather_load_case_xml_handler.h"

#include "models/base/helper.h"

#include "appcommon/units/weather_load_case_unit_converter.h"

wxXmlNode* WeatherLoadCaseXmlHandler::CreateNode(
    const WeatherLoadCase& weathercase,
    const std::string& name,
    const units::UnitSystem& system_units,
    const units::UnitStyle& style_units) {
  // variables used to create XML node
  wxXmlNode* node_root = nullptr;
  wxXmlNode* node_element = nullptr;
  std::string title;
  std::string content;
  wxXmlAttribute attribute;
  double value;

  // creates a node for the root
  node_root = new wxXmlNode(wxXML_ELEMENT_NODE, "weather_load_case");
  node_root->AddAttribute("version", "1");

  if (name != "") {
    node_root->AddAttribute("name", name);
  }

  // creates description node
  title = "description";
  content = weathercase.description;
  node_element = CreateElementNodeWithContent(title, content);
  node_root->AddChild(node_element);

  // creates thickness-ice node and adds to root node
  title = "thickness_ice";
  value = weathercase.thickness_ice;
  content = helper::DoubleToString(value, 6);
  if (system_units == units::UnitSystem::kMetric) {
    if (style_units == units::UnitStyle::kConsistent) {
      attribute = wxXmlAttribute("units", "m");
    } else if (style_units == units::UnitStyle::kDifferent) {
      attribute = wxXmlAttribute("units", "cm");
    }
  } else if (system_units == units::UnitSystem::kImperial) {
    if (style_units == units::UnitStyle::kConsistent) {
      attribute = wxXmlAttribute("units", "ft");
    } else if (style_units == units::UnitStyle::kDifferent) {
      attribute = wxXmlAttribute("units", "in");
    }
  }
  node_element = CreateElementNodeWithContent(title, content, &attribute);
  node_root->AddChild(node_element);

  // creates density-ice node and adds to root node
  title = "density_ice";
  value = weathercase.density_ice;
  content = helper::DoubleToString(value, 6);
  if (system_units == units::UnitSystem::kMetric) {
    attribute = wxXmlAttribute("units", "N/m^3");
  } else if (system_units == units::UnitSystem::kImperial) {
    attribute = wxXmlAttribute("units", "lb/ft^3");
  }
  node_element = CreateElementNodeWithContent(title, content, &attribute);
  node_root->AddChild(node_element);

  // creates pressure-wind node and adds to root node
  title = "pressure_wind";
  value = weathercase.pressure_wind;
  content = helper::DoubleToString(value, 6);
  if (system_units == units::UnitSystem::kMetric) {
    attribute = wxXmlAttribute("units", "Pa");
  } else if (system_units == units::UnitSystem::kImperial) {
    attribute = wxXmlAttribute("units", "lb/ft^2");
  }
  node_element = CreateElementNodeWithContent(title, content, &attribute);
  node_root->AddChild(node_element);

  // creates temperature-cable node and adds to root node
  title = "temperature_cable";
  value = weathercase.temperature_cable;
  content = helper::DoubleToString(value, 2, true);
  if (system_units == units::UnitSystem::kMetric) {
    attribute = wxXmlAttribute("units", L"\u00B0C");
  } else if (system_units == units::UnitSystem::kImperial) {
    attribute = wxXmlAttribute("units", L"\u00B0F");
  }
  node_element = CreateElementNodeWithContent(title, content, &attribute);
  node_root->AddChild(node_element);

  // returns root node
  return node_root;
}

bool WeatherLoadCaseXmlHandler::ParseNode(const wxXmlNode* root,
                                          const std::string& filepath,
                                          const units::UnitSystem& units,
                                          const bool& convert,
                                          WeatherLoadCase& weathercase) {
  wxString message;

  // checks for valid root node
  if (root->GetName() != "weather_load_case") {
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
    return ParseNodeV1(root, filepath, units, convert, weathercase);
  } else {
    message = FileAndLineNumber(filepath, root) +
              " Invalid version number. Aborting node parse.";
    wxLogError(message);
    return false;
  }
}

bool WeatherLoadCaseXmlHandler::ParseNodeV1(const wxXmlNode* root,
                                            const std::string& filepath,
                                            const units::UnitSystem& units,
                                            const bool& convert,
                                            WeatherLoadCase& weathercase) {
  bool status = true;
  wxString message;

  // evaluates each child node
  wxXmlNode* node = root->GetChildren();
  while (node != nullptr) {
    const wxString title = node->GetName();
    const wxString content = ParseElementNodeWithContent(node);
    double value = -999999;

    if (title == "description") {
      weathercase.description = content;
    } else if (title == "thickness_ice") {
      if (content.ToDouble(&value) == true) {
        weathercase.thickness_ice = value;
      } else {
        message = FileAndLineNumber(filepath, node)
                  + "Invalid ice thickness.";
        wxLogError(message);
        weathercase.thickness_ice = -999999;
        status = false;
      }
    } else if (title == "density_ice") {
      if (content.ToDouble(&value) == true) {
        weathercase.density_ice = value;
      } else {
        message = FileAndLineNumber(filepath, node)
                  + "Invalid ice density.";
        wxLogError(message);
        weathercase.density_ice = -999999;
        status = false;
      }
    } else if (title == "pressure_wind") {
      if (content.ToDouble(&value) == true) {
        weathercase.pressure_wind = value;
      } else {
        message = FileAndLineNumber(filepath, node)
                  + "Invalid wind pressure.";
        wxLogError(message);
        weathercase.pressure_wind = -999999;
        status = false;
      }
    } else if (title == "temperature_cable") {
      if (content.ToDouble(&value) == true) {
        weathercase.temperature_cable = value;
      } else {
        message = FileAndLineNumber(filepath, node)
                  + "Invalid cable temperature.";
        wxLogError(message);
        weathercase.temperature_cable = -999999;
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

  // converts unit style to 'consistent' if needed
  if (convert == true) {
    WeatherLoadCaseUnitConverter::ConvertUnitStyleToConsistent(
        1,
        units,
        weathercase);
  }

  return status;
}
