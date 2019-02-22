// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "appcommon/editors/weather_load_case_editor_dialog.h"

#include "wx/valnum.h"
#include "wx/xrc/xmlres.h"

#include "appcommon/widgets/error_message_dialog.h"

BEGIN_EVENT_TABLE(WeatherLoadCaseEditorDialog, wxDialog)
  EVT_BUTTON(wxID_CANCEL, WeatherLoadCaseEditorDialog::OnCancel)
  EVT_BUTTON(wxID_OK, WeatherLoadCaseEditorDialog::OnOk)
  EVT_CLOSE(WeatherLoadCaseEditorDialog::OnClose)
END_EVENT_TABLE()

WeatherLoadCaseEditorDialog::WeatherLoadCaseEditorDialog(
    wxWindow* parent,
    WeatherLoadCase* weathercase,
    const units::UnitSystem units) {
  // loads dialog from virtual xrc file system
  wxXmlResource::Get()->LoadDialog(this, parent,
                                   "weather_load_case_editor_dialog");

  // sets static text to display selected units
  SetUnitsStaticText(units);

  // saves unmodified cable reference, and copies to modified cable
  weathercase_ = weathercase;
  weathercase_modified_ = WeatherLoadCase(*weathercase);

  // sets form validators to transfer between controls data and controls
  SetValidators();

  // transfers non-validator data to the window
  TransferCustomDataToWindow();

  // fits the dialog around the sizers
  this->Fit();
}

WeatherLoadCaseEditorDialog::~WeatherLoadCaseEditorDialog() {
}

void WeatherLoadCaseEditorDialog::OnCancel(wxCommandEvent &event) {
  EndModal(wxID_CANCEL);
}

void WeatherLoadCaseEditorDialog::OnClose(wxCloseEvent &event) {
  EndModal(wxID_CLOSE);
}

void WeatherLoadCaseEditorDialog::OnOk(wxCommandEvent &event) {
  // validates data from form
  if (this->Validate() == false) {
    wxMessageBox("Errors on form");
    return;
  }

  wxBusyCursor cursor;

  // transfers data from dialog controls
  TransferDataFromWindow();
  TransferCustomDataFromWindow();

  // validates weathercase data
  std::list<ErrorMessage> messages;
  if (weathercase_modified_.Validate(true, &messages) == true) {
    // updates original cable reference based on user form edits
    *weathercase_ = WeatherLoadCase(weathercase_modified_);

    // ends modal by returning ok indicator
    EndModal(wxID_OK);
  } else {
    // displays errors to user
    ErrorMessageDialog message(this, &messages);
    message.ShowModal();
    return;
  }
}

void WeatherLoadCaseEditorDialog::SetUnitsStaticText(
    const units::UnitSystem& units) {
  if (units == units::UnitSystem::kMetric) {
    XRCCTRL(*this, "statictext_thickness_ice_units", wxStaticText)->
        SetLabel("[cm]");
    XRCCTRL(*this, "statictext_density_ice_units", wxStaticText)->
        SetLabel("[N/m^3]");
    XRCCTRL(*this, "statictext_pressure_wind_units", wxStaticText)->
        SetLabel("[Pa]");
    XRCCTRL(*this, "statictext_temperature_cable_units", wxStaticText)->
        SetLabel(L"[\u00B0C]");
  } else if (units == units::UnitSystem::kImperial) {
    XRCCTRL(*this, "statictext_thickness_ice_units", wxStaticText)->
        SetLabel("[in]");
    XRCCTRL(*this, "statictext_density_ice_units", wxStaticText)->
        SetLabel("[pcf]");
    XRCCTRL(*this, "statictext_pressure_wind_units", wxStaticText)->
        SetLabel("[psf]");
    XRCCTRL(*this, "statictext_temperature_cable_units", wxStaticText)->
        SetLabel(L"[\u00B0F]");
  }
}

void WeatherLoadCaseEditorDialog::SetValidators() {
  // variables used for creating validators
  int style = 0;
  double* value_num = nullptr;
  int precision = 0;
  wxTextCtrl* textctrl = nullptr;

  // description
  style = wxFILTER_NONE;
  textctrl = XRCCTRL(*this, "textctrl_description", wxTextCtrl);
  textctrl->SetValidator(wxTextValidator(style, nullptr));

  // thickness-ice
  precision = 3;
  value_num = &weathercase_modified_.thickness_ice;
  style = wxNUM_VAL_NO_TRAILING_ZEROES;
  textctrl = XRCCTRL(*this, "textctrl_thickness_ice", wxTextCtrl);
  textctrl->SetValidator(
      wxFloatingPointValidator<double>(precision, value_num, style));

  // density-ice
  precision = 2;
  value_num = &weathercase_modified_.density_ice;
  style = wxNUM_VAL_NO_TRAILING_ZEROES;
  textctrl = XRCCTRL(*this, "textctrl_density_ice", wxTextCtrl);
  textctrl->SetValidator(
      wxFloatingPointValidator<double>(precision, value_num, style));

  // pressure-wind
  precision = 2;
  value_num = &weathercase_modified_.pressure_wind;
  style = wxNUM_VAL_NO_TRAILING_ZEROES;
  textctrl = XRCCTRL(*this, "textctrl_pressure_wind", wxTextCtrl);
  textctrl->SetValidator(
      wxFloatingPointValidator<double>(precision, value_num, style));

  // temperature-cable
  precision = 1;
  value_num = &weathercase_modified_.temperature_cable;
  style = wxNUM_VAL_NO_TRAILING_ZEROES;
  textctrl = XRCCTRL(*this, "textctrl_temperature_cable", wxTextCtrl);
  textctrl->SetValidator(
      wxFloatingPointValidator<double>(precision, value_num, style));
}

void WeatherLoadCaseEditorDialog::TransferCustomDataFromWindow() {
  // transfers name
  wxTextCtrl* textctrl = XRCCTRL(*this, "textctrl_description", wxTextCtrl);
  weathercase_modified_.description = textctrl->GetValue();
}

void WeatherLoadCaseEditorDialog::TransferCustomDataToWindow() {
  // transfers name
  wxTextCtrl* textctrl = XRCCTRL(*this, "textctrl_description", wxTextCtrl);
  textctrl->SetValue(weathercase_modified_.description);
}
