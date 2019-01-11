// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "appcommon/xml/cable_xml_handler.h"

#include "models/base/helper.h"

wxXmlNode* CableComponentXmlHandler::CreateNode(
    const CableComponent& component,
    const std::string& name,
    const units::UnitSystem& units) {
  // variables used to create XML node
  wxXmlNode* node_root = nullptr;
  wxXmlNode* node_element = nullptr;
  std::string title;
  std::string content;
  wxXmlAttribute attribute;
  double value;
  const std::vector<double>* coefficients = nullptr;

  // creates a node for the root
  node_root = new wxXmlNode(wxXML_ELEMENT_NODE, "cable_component");
  node_root->AddAttribute("version", "1");

  if (name != "") {
    node_root->AddAttribute("name", name);
  }

  // creates capacity-heat node and adds to root node
  title = "capacity_heat";
  value = component.capacity_heat;
  content = helper::DoubleToString(value, 6);
  if (units == units::UnitSystem::kMetric) {
    attribute = wxXmlAttribute("units", "W-s/m-degC");
  } else if (units == units::UnitSystem::kImperial) {
    attribute = wxXmlAttribute("units", "W-s/ft-degF");
  }
  node_element = CreateElementNodeWithContent(title, content, &attribute);
  node_root->AddChild(node_element);

  // creates coefficient-expansion-linear-thermal node and adds to root node
  title = "coefficient_expansion_linear_thermal";
  value = component.coefficient_expansion_linear_thermal;
  content = helper::DoubleToString(value, 6);
  if (units == units::UnitSystem::kMetric) {
    attribute = wxXmlAttribute("units", "deg C");
  } else if (units == units::UnitSystem::kImperial) {
    attribute = wxXmlAttribute("units", "deg F");
  }
  node_element = CreateElementNodeWithContent(title, content, &attribute);
  node_root->AddChild(node_element);

  // creates modulus-compression-elastic-area node and adds to root node
  title = "modulus_compression_elastic";
  value = component.modulus_compression_elastic_area;
  content = helper::DoubleToString(value, 6);
  if (units == units::UnitSystem::kMetric) {
    attribute = wxXmlAttribute("units", "MPa");
  } else if (units == units::UnitSystem::kImperial) {
    attribute = wxXmlAttribute("units", "lbs/in^2");
  }
  node_element = CreateElementNodeWithContent(title, content, &attribute);
  node_root->AddChild(node_element);

  // creates modulus-tension-elastic-area node and adds to root node
  title = "modulus_tension_elastic";
  value = component.modulus_tension_elastic_area;
  content = helper::DoubleToString(value, 6);
  if (units == units::UnitSystem::kMetric) {
    attribute = wxXmlAttribute("units", "MPa");
  } else if (units == units::UnitSystem::kImperial) {
    attribute = wxXmlAttribute("units", "lbs/in^2");
  }
  node_element = CreateElementNodeWithContent(title, content, &attribute);
  node_root->AddChild(node_element);

  // creates coefficients stress-strain node and adds to root node
  title = "coefficients";
  node_element = new wxXmlNode(wxXML_ELEMENT_NODE, title);
  node_element->AddAttribute("name", "stress-strain");

  if (units == units::UnitSystem::kMetric) {
    attribute = wxXmlAttribute("units", "MPa");
  } else if (units == units::UnitSystem::kImperial) {
    attribute = wxXmlAttribute("units", "lbs/in^2");
  } else {
    attribute = wxXmlAttribute();
  }

  coefficients = &component.coefficients_polynomial_loadstrain;
  for (auto iter = coefficients->cbegin(); iter != coefficients->cend();
       iter++) {
    const double& coefficient = *iter;
    content = helper::DoubleToString(coefficient, 6);
    wxXmlNode* sub_node = XmlHandler::CreateElementNodeWithContent(
        "coefficient",
        content,
        &attribute);
    node_element->AddChild(sub_node);
  }
  node_root->AddChild(node_element);

  // creates load-limit-polynomial-loadstrain node and adds to root node
  title = "limit_polynomial_stress-strain";
  value = component.load_limit_polynomial_loadstrain;
  content = helper::DoubleToString(value, 6);
  if (units == units::UnitSystem::kMetric) {
    attribute = wxXmlAttribute("units", "MPa");
  } else if (units == units::UnitSystem::kImperial) {
    attribute = wxXmlAttribute("units", "lbs/in^2");
  }
  node_element = CreateElementNodeWithContent(title, content, &attribute);
  node_root->AddChild(node_element);

  // creates coefficients-creep node and adds to root node
  title = "coefficients";
  node_element = new wxXmlNode(wxXML_ELEMENT_NODE, title);
  node_element->AddAttribute("name", "creep");

  if (units == units::UnitSystem::kMetric) {
    attribute = wxXmlAttribute("units", "MPa");
  } else if (units == units::UnitSystem::kImperial) {
    attribute = wxXmlAttribute("units", "lbs/in^2");
  } else {
    attribute = wxXmlAttribute();
  }

  coefficients = &component.coefficients_polynomial_creep;
  for (auto iter = coefficients->cbegin(); iter != coefficients->cend();
       iter++) {
    const double& coefficient = *iter;
    content = helper::DoubleToString(coefficient, 6);
    wxXmlNode* sub_node = XmlHandler::CreateElementNodeWithContent(
        "coefficient",
        content,
        &attribute);
    node_element->AddChild(sub_node);
  }
  node_root->AddChild(node_element);

  // creates load-limit-polynomial-creep node and adds to root node
  title = "limit_polynomial_creep";
  value = component.load_limit_polynomial_creep;
  content = helper::DoubleToString(value, 6);
  if (units == units::UnitSystem::kMetric) {
    attribute = wxXmlAttribute("units", "MPa");
  } else if (units == units::UnitSystem::kImperial) {
    attribute = wxXmlAttribute("units", "lbs/in^2");
  }
  node_element = CreateElementNodeWithContent(title, content, &attribute);
  node_root->AddChild(node_element);

  // returns root node
  return node_root;
}

bool CableComponentXmlHandler::ParseNode(const wxXmlNode* root,
                                         const std::string& filepath,
                                         CableComponent& component) {
  wxString message;

  // checks for valid root node
  if (root->GetName() != "cable_component") {
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
    return ParseNodeV1(root, filepath, component);
  } else {
    message = FileAndLineNumber(filepath, root) +
              " Invalid version number. Aborting node parse.";
    wxLogError(message);
    return false;
  }
}

bool CableComponentXmlHandler::ParseNodeV1(const wxXmlNode* root,
                                           const std::string& filepath,
                                           CableComponent& component) {
  bool status = true;
  wxString message;

  // evaluates each child node
  const wxXmlNode* node = root->GetChildren();
  while (node != nullptr) {
    const wxString title = node->GetName();
    const wxString content = ParseElementNodeWithContent(node);
    double value = -999999;

    if (title == "capacity_heat") {
      if (content.ToDouble(&value) == true) {
        component.capacity_heat = value;
      } else {
        message = FileAndLineNumber(filepath, node)
                  + "Invalid heat capacity.";
        wxLogError(message);
        component.capacity_heat = -999999;
        status = false;
      }
    } else if (title == "coefficient_expansion_linear_thermal") {
      if (content.ToDouble(&value) == true) {
        component.coefficient_expansion_linear_thermal = value;
      } else {
        message = FileAndLineNumber(filepath, node)
                  + "Invalid coefficient of thermal expansion.";
        wxLogError(message);
        component.coefficient_expansion_linear_thermal = -999999;
        status = false;
      }
    } else if (title == "coefficients") {
      wxString name_coefficients;
      node->GetAttribute("name", &name_coefficients);

      // gets coefficient vector
      std::vector<double>* coefficients = nullptr;
      if (name_coefficients == "stress-strain") {
        coefficients = &component.coefficients_polynomial_loadstrain;
      } else if (name_coefficients == "creep") {
        coefficients = &component.coefficients_polynomial_creep;
      }

      // checks for valid coefficients name
      if (coefficients != nullptr) {
        // gets coefficient sub-nodes
        wxXmlNode* sub_node = node->GetChildren();

        if (sub_node == nullptr) {
          message = FileAndLineNumber(filepath, node)
                    + "Coefficients are undefined.";
          wxLogError(message);
          status = false;
        }

        unsigned int order = 0;
        while (sub_node != nullptr) {
          // creates a new coefficient
          double coefficient = -999999;
          const wxString sub_content = ParseElementNodeWithContent(sub_node);
          if (sub_content.ToDouble(&coefficient) == false) {
            message = FileAndLineNumber(filepath, sub_node)
                      + "Invalid coefficient.";
            wxLogError(message);
            status = false;
          }

          // adds or assigns coefficient to vector
          if (order < coefficients->size()) {
            coefficients->at(order) = coefficient;
          } else {
            coefficients->push_back(coefficient);
          }

          order++;
          sub_node = sub_node->GetNext();
        }
      } else {
        message = FileAndLineNumber(filepath, node)
                  + "XML node isn't recognized.";
        wxLogError(message);
        status = false;
      }
    } else if (title == "limit_polynomial_creep") {
      if (content.ToDouble(&value) == true) {
        component.load_limit_polynomial_creep = value;
      } else {
        message = FileAndLineNumber(filepath, node)
                  + "Invalid creep polynomial limit.";
        wxLogError(message);
        component.load_limit_polynomial_creep = -999999;
        status = false;
      }
    } else if (title == "limit_polynomial_stress-strain") {
      if (content.ToDouble(&value) == true) {
        component.load_limit_polynomial_loadstrain = value;
      } else {
        message = FileAndLineNumber(filepath, node)
                  + "Invalid stress-strain polynomial limit.";
        wxLogError(message);
        component.load_limit_polynomial_loadstrain = -999999;
        status = false;
      }
    } else if (title == "modulus_compression_elastic") {
      if (content.ToDouble(&value) == true) {
        component.modulus_compression_elastic_area = value;
      } else {
        message = FileAndLineNumber(filepath, node)
                  + "Invalid compression elastic modulus.";
        wxLogError(message);
        component.modulus_compression_elastic_area = -999999;
        status = false;
      }
    } else if (title == "modulus_tension_elastic") {
      if (content.ToDouble(&value) == true) {
        component.modulus_tension_elastic_area = value;
      } else {
        message = FileAndLineNumber(filepath, node)
                  + "Invalid tension elastic modulus.";
        wxLogError(message);
        component.modulus_tension_elastic_area = -999999;
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


wxXmlNode* CableXmlHandler::CreateNode(const Cable& cable,
                                       const std::string& name,
                                       const units::UnitSystem& units) {
  // variables used to create XML node
  double value = -999999;
  std::string title;
  std::string content;
  wxXmlAttribute attribute;
  wxXmlNode* node_root = nullptr;
  wxXmlNode* node_element = nullptr;

  // creates a node for the root
  node_root = new wxXmlNode(wxXML_ELEMENT_NODE, "cable");
  node_root->AddAttribute("version", "1");

  if (name != "") {
    node_root->AddAttribute("name", name);
  }

  // creates name node and adds to root node
  title = "name";
  content = cable.name;
  node_element = CreateElementNodeWithContent(title, content);
  node_root->AddChild(node_element);

  // creates area-physical node and adds to root node
  title = "area_physical";
  value = cable.area_physical;
  content = helper::DoubleToString(value, 6);
  if (units == units::UnitSystem::kMetric) {
    attribute = wxXmlAttribute("units", "mm^2");
  } else if (units == units::UnitSystem::kImperial) {
    attribute = wxXmlAttribute("units", "in^2");
  }
  node_element = CreateElementNodeWithContent(title, content, &attribute);
  node_root->AddChild(node_element);

  // creates diameter node and adds to root node
  title = "diameter";
  value = cable.diameter;
  content = helper::DoubleToString(value, 6);
  if (units == units::UnitSystem::kMetric) {
    attribute = wxXmlAttribute("units", "mm");
  } else if (units == units::UnitSystem::kImperial) {
    attribute = wxXmlAttribute("units", "in");
  }
  node_element = CreateElementNodeWithContent(title, content, &attribute);
  node_root->AddChild(node_element);

  // creates weight-unit node and adds to root node
  title = "weight_unit";
  value = cable.weight_unit;
  content = helper::DoubleToString(value, 6);
  if (units == units::UnitSystem::kMetric) {
    attribute = wxXmlAttribute("units", "N/m");
  } else if (units == units::UnitSystem::kImperial) {
    attribute = wxXmlAttribute("units", "lbs/feet");
  }
  node_element = CreateElementNodeWithContent(title, content, &attribute);
  node_root->AddChild(node_element);

  // creates strength-rated node and adds to root node
  title = "strength_rated";
  value = cable.strength_rated;
  content = helper::DoubleToString(value, 6);
  if (units == units::UnitSystem::kMetric) {
    attribute = wxXmlAttribute("units", "N");
  } else if (units == units::UnitSystem::kImperial) {
    attribute = wxXmlAttribute("units", "lbs");
  }
  node_element = CreateElementNodeWithContent(title, content, &attribute);
  node_root->AddChild(node_element);

  // creates temperature-component-properties node and adds to root node
  title = "temperature_properties_components";
  value = cable.temperature_properties_components;
  content = helper::DoubleToString(value, 6);
  if (units == units::UnitSystem::kMetric) {
    attribute = wxXmlAttribute("units", "deg C");
  } else if (units == units::UnitSystem::kImperial) {
    attribute = wxXmlAttribute("units", "deg F");
  }
  node_element = CreateElementNodeWithContent(title, content, &attribute);
  node_root->AddChild(node_element);

  // creates absorptivity node and adds to root node
  title = "absorptivity";
  value = cable.absorptivity;
  content = helper::DoubleToString(value, 6);
  attribute = wxXmlAttribute("units", "");
  node_element = CreateElementNodeWithContent(title, content, &attribute);
  node_root->AddChild(node_element);

  // creates emissivity node and adds to root node
  title = "emissivity";
  value = cable.emissivity;
  content = helper::DoubleToString(value, 6);
  attribute = wxXmlAttribute("units", "");
  node_element = CreateElementNodeWithContent(title, content, &attribute);
  node_root->AddChild(node_element);

  // creates resistances-ac node
  title = "resistances_ac";
  node_element = new wxXmlNode(wxXML_ELEMENT_NODE, title);
  for (auto iter = cable.resistances_ac.cbegin();
       iter != cable.resistances_ac.cend(); iter++) {
    const Cable::ResistancePoint& point = *iter;
    wxXmlNode* sub_node = CreateNodeResistancePoint(point, "", units);
    node_element->AddChild(sub_node);
  }
  node_root->AddChild(node_element);

  // creates component-shell node and adds to root node
  node_root->AddChild(
      CableComponentXmlHandler::CreateNode(cable.component_shell, "shell",
                                           units));

  // creates component-core node and adds to root node
  node_root->AddChild(
      CableComponentXmlHandler::CreateNode(cable.component_core, "core",
                                           units));

  // returns root node
  return node_root;
}

bool CableXmlHandler::ParseNode(const wxXmlNode* root,
                                const std::string& filepath,
                                Cable& cable) {
  wxString message;

  // checks for valid node
  if (root->GetName() != "cable") {
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
    return CableXmlHandler::ParseNodeV1(root, filepath, cable);
  } else {
    message = FileAndLineNumber(filepath, root) +
              " Invalid version number. Aborting node parse.";
    wxLogError(message);
    return false;
  }
}

wxXmlNode* CableXmlHandler::CreateNodeResistancePoint(
    const Cable::ResistancePoint& point,
    const std::string& name,
    const units::UnitSystem& units) {
  // variables used to create XML node
  double value = -999999;
  std::string title;
  std::string content;
  wxXmlAttribute attribute;
  wxXmlNode* node_root = nullptr;
  wxXmlNode* node_element = nullptr;

  // creates a node for the root
  node_root = new wxXmlNode(wxXML_ELEMENT_NODE, "resistance_point");

  if (name != "") {
    node_root->AddAttribute("name", name);
  }

  // creates resistance node and adds to root node
  title = "resistance";
  value = point.resistance;
  content = helper::DoubleToString(value, 6);
  if (units == units::UnitSystem::kMetric) {
    attribute = wxXmlAttribute("units", "ohm/km");
  } else if (units == units::UnitSystem::kImperial) {
    attribute = wxXmlAttribute("units", "ohm/mi");
  }
  node_element = CreateElementNodeWithContent(title, content, &attribute);
  node_root->AddChild(node_element);

  // creates temperature node and adds to root node
  title = "temperature";
  value = point.temperature;
  content = helper::DoubleToString(value, 6);
  if (units == units::UnitSystem::kMetric) {
    attribute = wxXmlAttribute("units", "deg C");
  } else if (units == units::UnitSystem::kImperial) {
    attribute = wxXmlAttribute("units", "deg F");
  }
  node_element = CreateElementNodeWithContent(title, content, &attribute);
  node_root->AddChild(node_element);

  // returns root node
  return node_root;
}

bool CableXmlHandler::ParseNodeResistancePoint(const wxXmlNode* root,
                                               const std::string& filepath,
                                               Cable::ResistancePoint& point) {
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

    if (title == "resistance") {
      if (content.ToDouble(&value) == true) {
        point.resistance = value;
      } else {
        message = FileAndLineNumber(filepath, node)
                  + "Invalid resistance.";
        wxLogError(message);
        point.resistance = -999999;
        status = false;
      }
    } else if (title == "temperature") {
      if (content.ToDouble(&value) == true) {
        point.temperature = value;
      } else {
        message = FileAndLineNumber(filepath, node)
                  + "Invalid temperature.";
        wxLogError(message);
        point.temperature = -999999;
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

bool CableXmlHandler::ParseNodeV1(const wxXmlNode* root,
                                  const std::string& filepath,
                                  Cable& cable) {
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
      cable.name = content;
    } else if (title == "area_physical") {
      if (content.ToDouble(&value) == true) {
        cable.area_physical = value;
      } else {
        message = FileAndLineNumber(filepath, node)
                  + "Invalid physical area.";
        wxLogError(message);
        cable.area_physical = -999999;
        status = false;
      }
    } else if (title == "diameter") {
      if (content.ToDouble(&value) == true) {
        cable.diameter = value;
      } else {
        message = FileAndLineNumber(filepath, node)
                  + "Invalid diameter.";
        wxLogError(message);
        cable.diameter = -999999;
        status = false;
      }
    } else if (title == "strength_rated") {
      if (content.ToDouble(&value) == true) {
        cable.strength_rated = value;
      } else {
        message = FileAndLineNumber(filepath, node)
                  + "Invalid rated strength.";
        wxLogError(message);
        cable.strength_rated = -999999;
        status = false;
      }
    } else if (title == "temperature_properties_components") {
      if (content.ToDouble(&value) == true) {
        cable.temperature_properties_components = value;
      } else {
        message = FileAndLineNumber(filepath, node)
                  + "Invalid component properties temperature.";
        wxLogError(message);
        cable.temperature_properties_components = -999999;
        status = false;
      }
    } else if (title == "weight_unit") {
      if (content.ToDouble(&value) == true) {
        cable.weight_unit = value;
      } else {
        message = FileAndLineNumber(filepath, node)
                  + "Invalid unit weight.";
        wxLogError(message);
        cable.weight_unit = -999999;
        status = false;
      }
    } else if (title == "absorptivity") {
      if (content.ToDouble(&value) == true) {
        cable.absorptivity = value;
      } else {
        message = FileAndLineNumber(filepath, node)
                  + "Invalid absorptivity.";
        wxLogError(message);
        cable.absorptivity = -999999;
        status = false;
      }
    } else if (title == "emissivity") {
      if (content.ToDouble(&value) == true) {
        cable.emissivity = value;
      } else {
        message = FileAndLineNumber(filepath, node)
                  + "Invalid emissivity.";
        wxLogError(message);
        cable.emissivity = -999999;
        status = false;
      }
    } else if (title == "resistances_ac") {
      // gets resistance point sub-nodes
      wxXmlNode* sub_node = node->GetChildren();
      while (sub_node != nullptr) {
        // parses resistance point node
        Cable::ResistancePoint point;
        const bool status_node = ParseNodeResistancePoint(sub_node,
                                                          filepath, point);
        if (status_node == true) {
          // adds to resistance container
          cable.resistances_ac.push_back(point);
        } else {
          status = false;
        }

        sub_node = sub_node->GetNext();
      }
    } else if (title == "cable_component") {
      // selects cable component type and passes off to cable component parser
      wxString name_component = node->GetAttribute("name");
      if (name_component == "shell") {
        const bool status_node = CableComponentXmlHandler::ParseNode(
            node, filepath, cable.component_shell);
        if (status_node == false) {
          status = false;
        }
      } else if (name_component == "core") {
        const bool status_node = CableComponentXmlHandler::ParseNode(
            node, filepath, cable.component_core);
        if (status_node == false) {
          status = false;
        }
      } else {
        message = FileAndLineNumber(filepath, node)
                  + "Invalid component.";
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

  return status;
}
