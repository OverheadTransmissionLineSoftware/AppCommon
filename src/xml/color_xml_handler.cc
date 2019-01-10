// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "appcommon/xml/color_xml_handler.h"

#include "models/base/helper.h"

wxXmlNode* ColorXmlHandler::CreateNode(
    const wxColour& color,
    const std::string& name) {
  // variables used to create XML node
  wxXmlNode* node_root = nullptr;
  std::string title;
  std::string content;

  // creates a node for the root
  title = "color";
  content = std::to_string(color.Red())
      + "," + std::to_string(color.Green())
      + "," + std::to_string(color.Blue());

  node_root = CreateElementNodeWithContent(title, content);

  // adds attributes
  node_root->AddAttribute("version", "1");

  if (name != "") {
    node_root->AddAttribute("name", name);
  }

  // returns root node
  return node_root;
}

bool ColorXmlHandler::ParseNode(
    const wxXmlNode* root,
    const std::string& filepath,
    wxColour& color) {
  wxString message;

  // checks for valid root node
  if (root->GetName() != "color") {
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
    return ParseNodeV1(root, filepath, color);
  } else {
    message = FileAndLineNumber(filepath, root) +
              " Invalid version number. Aborting node parse.";
    wxLogError(message);
    return false;
  }
}

bool ColorXmlHandler::ParseNodeV1(
    const wxXmlNode* root,
    const std::string& filepath,
    wxColour& color) {
  bool status = true;
  wxString message;

  // parses content into separate strings
  const std::string content = ParseElementNodeWithContent(root);
  std::list<std::string> content_parsed = helper::Parse(content, ',');
  if (content_parsed.size() != 3) {
    message = FileAndLineNumber(filepath, root)
              + "Invalid color.";
    wxLogError(message);
    status = false;

    return false;
  }

  // converts strings to color component values
  wxString str;
  long value = -9999;
  int r = 0;
  int g = 0;
  int b = 0;

  str = *std::next(content_parsed.cbegin(), 0);
  if (str.ToLong(&value) == true) {
    r = value;
  } else {
    message = FileAndLineNumber(filepath, root)
              + "Invalid red color component.";
    wxLogError(message);
    status = false;
  }

  str = *std::next(content_parsed.cbegin(), 1);
  if (str.ToLong(&value) == true) {
    g = value;
  } else {
    message = FileAndLineNumber(filepath, root)
              + "Invalid green color component.";
    wxLogError(message);
    status = false;
  }

  str = *std::next(content_parsed.cbegin(), 2);
  if (str.ToLong(&value) == true) {
    b = value;
  } else {
    message = FileAndLineNumber(filepath, root)
              + "Invalid blue color component.";
    wxLogError(message);
    status = false;
  }

  // modifies color with parsed components
  color = wxColour(r, g, b);

  return status;
}
