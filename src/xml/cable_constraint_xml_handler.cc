// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "appcommon/xml/cable_constraint_xml_handler.h"

#include "models/base/helper.h"

#include "appcommon/units/cable_constraint_unit_converter.h"
#include "appcommon/xml/weather_load_case_xml_handler.h"

wxXmlNode* CableConstraintXmlHandler::CreateNode(
    const CableConstraint& constraint,
    const std::string name,
    const units::UnitSystem& system_units,
    const units::UnitStyle& /**style_units**/) {
  // variables used to create XML node
  wxXmlNode* node_root = nullptr;
  wxXmlNode* node_element = nullptr;
  std::string title;
  std::string content;
  wxXmlAttribute attribute;
  double value;

  // creates a node for the root
  node_root = new wxXmlNode(wxXML_ELEMENT_NODE, "cable_constraint");
  node_root->AddAttribute("version", "2");

  if (name != "") {
    node_root->AddAttribute("name", name);
  }

  // creates limit node and adds to root node
  title = "limit";
  value = constraint.limit;
  content = helper::DoubleToString(value, 3, true);
  node_element = CreateElementNodeWithContent(title, content);
  node_root->AddChild(node_element);

  if (constraint.type_limit ==
      CableConstraint::LimitType::kCatenaryConstant) {
    attribute = wxXmlAttribute("type", "CatenaryConstant");
      node_element->AddAttribute(attribute.GetName(), attribute.GetValue());

    if (system_units == units::UnitSystem::kMetric) {
      attribute = wxXmlAttribute("units", "N/m");
      node_element->AddAttribute(&attribute);
    } else if (system_units == units::UnitSystem::kImperial) {
      attribute = wxXmlAttribute("units", "lbs/ft");
      node_element->AddAttribute(attribute.GetName(), attribute.GetValue());
    }
  } else if (constraint.type_limit ==
      CableConstraint::LimitType::kHorizontalTension) {
    attribute = wxXmlAttribute("type", "HorizontalTension");
      node_element->AddAttribute(attribute.GetName(), attribute.GetValue());

    if (system_units == units::UnitSystem::kMetric) {
      attribute = wxXmlAttribute("units", "N");
      node_element->AddAttribute(&attribute);
    } else if (system_units == units::UnitSystem::kImperial) {
      attribute = wxXmlAttribute("units", "lbs");
      node_element->AddAttribute(attribute.GetName(), attribute.GetValue());
    }
  } else if (constraint.type_limit == CableConstraint::LimitType::kLength) {
    attribute = wxXmlAttribute("type", "Length");
      node_element->AddAttribute(attribute.GetName(), attribute.GetValue());

    if (system_units == units::UnitSystem::kMetric) {
      attribute = wxXmlAttribute("units", "m");
      node_element->AddAttribute(attribute.GetName(), attribute.GetValue());
    } else if (system_units == units::UnitSystem::kImperial) {
      attribute = wxXmlAttribute("units", "ft");
      node_element->AddAttribute(attribute.GetName(), attribute.GetValue());
    }
  } else if (constraint.type_limit == CableConstraint::LimitType::kSag) {
    attribute = wxXmlAttribute("type", "Sag");
      node_element->AddAttribute(attribute.GetName(), attribute.GetValue());

    if (system_units == units::UnitSystem::kMetric) {
      attribute = wxXmlAttribute("units", "m");
      node_element->AddAttribute(attribute.GetName(), attribute.GetValue());
    } else if (system_units == units::UnitSystem::kImperial) {
      attribute = wxXmlAttribute("units", "ft");
      node_element->AddAttribute(attribute.GetName(), attribute.GetValue());
    }
  } else if (constraint.type_limit ==
      CableConstraint::LimitType::kSupportTension) {
    attribute = wxXmlAttribute("type", "SupportTension");
      node_element->AddAttribute(attribute.GetName(), attribute.GetValue());

    if (system_units == units::UnitSystem::kMetric) {
      attribute = wxXmlAttribute("units", "N");
      node_element->AddAttribute(attribute.GetName(), attribute.GetValue());
    } else if (system_units == units::UnitSystem::kImperial) {
      attribute = wxXmlAttribute("units", "lbs");
      node_element->AddAttribute(attribute.GetName(), attribute.GetValue());
    }
  }

  // creates weathercase node and adds to parent node
  title = "weather_load_case";
  if (constraint.case_weather != nullptr) {
    content = constraint.case_weather->description;
  } else {
    content = *wxEmptyString;
  }
  node_element = CreateElementNodeWithContent(title, content);
  node_root->AddChild(node_element);

  // creates condition node and adds to parent node
  title = "condition";
  if (constraint.condition == CableConditionType::kCreep) {
    content = "Creep";
  } else if (constraint.condition == CableConditionType::kInitial) {
    content = "Initial";
  } else if (constraint.condition == CableConditionType::kLoad) {
    content = "Load";
  } else {
    content = *wxEmptyString;
  }
  node_element = CreateElementNodeWithContent(title, content);
  node_root->AddChild(node_element);

  // creates note node and adds to parent node
  title = "note";
  content = constraint.note;
  node_element = CreateElementNodeWithContent(title, content);
  node_root->AddChild(node_element);

  // returns root node
  return node_root;
}

bool CableConstraintXmlHandler::ParseNode(
    const wxXmlNode* root,
    const std::string& filepath,
    const units::UnitSystem& units,
    const bool& convert,
    const std::list<const WeatherLoadCase*>* weathercases,
    CableConstraint& constraint) {
  wxString message;

  // checks for valid root node
  if (root->GetName() != "cable_constraint") {
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
    return ParseNodeV1(root, filepath, units, convert, weathercases,
                       constraint);
  } else if (kVersion == 2) {
    return ParseNodeV2(root, filepath, units, convert, weathercases,
                       constraint);
  } else {
    message = FileAndLineNumber(filepath, root) +
              " Invalid version number. Aborting node parse.";
    wxLogError(message);
    return false;
  }
}

bool CableConstraintXmlHandler::ParseNodeV1(
    const wxXmlNode* root,
    const std::string& filepath,
    const units::UnitSystem& units,
    const bool& convert,
    const std::list<const WeatherLoadCase*>* weathercases,
    CableConstraint& constraint) {
  bool status = true;
  wxString message;

  // evaluates each child node
  const wxXmlNode* node = root->GetChildren();
  while (node != nullptr) {
    const wxString title = node->GetName();
    const wxString content = ParseElementNodeWithContent(node);
    double value = -999999;

    if (title == "limit") {
      if (content.ToDouble(&value) == true) {
        constraint.limit = value;
      } else {
        message = FileAndLineNumber(filepath, node)
                  + "Invalid limit.";
        wxLogError(message);
        constraint.limit = -999999;
        status = false;
      }

      const wxString content_attribute = node->GetAttribute("type");
      if (content_attribute == "CatenaryConstant") {
        constraint.type_limit = CableConstraint::LimitType::kCatenaryConstant;
      } else if (content_attribute == "HorizontalTension") {
        constraint.type_limit = CableConstraint::LimitType::kHorizontalTension;
      } else if (content_attribute == "Length") {
        constraint.type_limit = CableConstraint::LimitType::kLength;
      } else if (content_attribute == "Sag") {
        constraint.type_limit = CableConstraint::LimitType::kSag;
      } else if (content_attribute == "SupportTension") {
        constraint.type_limit = CableConstraint::LimitType::kSupportTension;
      } else {
        message = FileAndLineNumber(filepath, node)
                  + "Invalid limit type.";
        wxLogError(message);
        status = false;
      }
    } else if (title == "weather_load_case") {
      // initializes the weathercase and attempts to find a match
      constraint.case_weather = nullptr;
      for (auto iter = weathercases->cbegin(); iter != weathercases->cend();
           iter++) {
        const WeatherLoadCase* weathercase = *iter;
        if (content == weathercase->description) {
          constraint.case_weather = weathercase;
          break;
        }
      }

      // checks if match was found
      if (constraint.case_weather == nullptr) {
        message = FileAndLineNumber(filepath, node)
                  + "Invalid weathercase. Couldn't find " + content;
        wxLogError(message);
        status = false;
      }
    } else if (title == "condition") {
      if (content == "Creep") {
        constraint.condition = CableConditionType::kCreep;
      } else if (content == "Initial") {
        constraint.condition = CableConditionType::kInitial;
      } else if (content == "Load") {
        constraint.condition = CableConditionType::kLoad;
      } else {
        message = FileAndLineNumber(filepath, node)
                  + "Invalid condition.";
        wxLogError(message);
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
    CableConstraintUnitConverter::ConvertUnitStyleToConsistent(1, units,
                                                               constraint);
  }

  return status;
}

bool CableConstraintXmlHandler::ParseNodeV2(
    const wxXmlNode* root,
    const std::string& filepath,
    const units::UnitSystem& units,
    const bool& convert,
    const std::list<const WeatherLoadCase*>* weathercases,
    CableConstraint& constraint) {
  bool status = true;
  wxString message;

  // evaluates each child node
  const wxXmlNode* node = root->GetChildren();
  while (node != nullptr) {
    const wxString title = node->GetName();
    const wxString content = ParseElementNodeWithContent(node);
    double value = -999999;

    if (title == "limit") {
      if (content.ToDouble(&value) == true) {
        constraint.limit = value;
      } else {
        message = FileAndLineNumber(filepath, node)
                  + "Invalid limit.";
        wxLogError(message);
        constraint.limit = -999999;
        status = false;
      }

      const wxString content_attribute = node->GetAttribute("type");
      if (content_attribute == "CatenaryConstant") {
        constraint.type_limit = CableConstraint::LimitType::kCatenaryConstant;
      } else if (content_attribute == "HorizontalTension") {
        constraint.type_limit = CableConstraint::LimitType::kHorizontalTension;
      } else if (content_attribute == "Length") {
        constraint.type_limit = CableConstraint::LimitType::kLength;
      } else if (content_attribute == "Sag") {
        constraint.type_limit = CableConstraint::LimitType::kSag;
      } else if (content_attribute == "SupportTension") {
        constraint.type_limit = CableConstraint::LimitType::kSupportTension;
      } else {
        message = FileAndLineNumber(filepath, node)
                  + "Invalid limit type.";
        wxLogError(message);
        status = false;
      }
    } else if (title == "weather_load_case") {
      // initializes the weathercase and attempts to find a match
      constraint.case_weather = nullptr;
      for (auto iter = weathercases->cbegin(); iter != weathercases->cend();
           iter++) {
        const WeatherLoadCase* weathercase = *iter;
        if (content == weathercase->description) {
          constraint.case_weather = weathercase;
          break;
        }
      }

      // checks if match was found
      if (constraint.case_weather == nullptr) {
        message = FileAndLineNumber(filepath, node)
                  + "Invalid weathercase. Couldn't find " + content;
        wxLogError(message);
        status = false;
      }
    } else if (title == "condition") {
      if (content == "Creep") {
        constraint.condition = CableConditionType::kCreep;
      } else if (content == "Initial") {
        constraint.condition = CableConditionType::kInitial;
      } else if (content == "Load") {
        constraint.condition = CableConditionType::kLoad;
      } else {
        message = FileAndLineNumber(filepath, node)
                  + "Invalid condition.";
        wxLogError(message);
        status = false;
      }
    } else if (title == "note") {
      constraint.note = content;
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
    CableConstraintUnitConverter::ConvertUnitStyleToConsistent(1, units,
                                                               constraint);
  }

  return status;
}
