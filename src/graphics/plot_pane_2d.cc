// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "appcommon/graphics/plot_pane_2d.h"

#include "models/transmissionline/catenary.h"
#include "wx/dcbuffer.h"

BEGIN_EVENT_TABLE(PlotPane2d, wxPanel)
  EVT_ENTER_WINDOW(PlotPane2d::OnMouse)
  EVT_ERASE_BACKGROUND(PlotPane2d::OnEraseBackground)
  EVT_LEAVE_WINDOW(PlotPane2d::OnMouse)
  EVT_LEFT_DOWN(PlotPane2d::OnMouse)
  EVT_LEFT_UP(PlotPane2d::OnMouse)
  EVT_MIDDLE_DOWN(PlotPane2d::OnMouse)
  EVT_MOUSEWHEEL(PlotPane2d::OnMouseWheel)
  EVT_PAINT(PlotPane2d::OnPaint)
  EVT_RIGHT_DOWN(PlotPane2d::OnMouse)
END_EVENT_TABLE()

PlotPane2d::PlotPane2d(wxWindow* parent)
    : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize,
              wxTAB_TRAVERSAL) {
  // initializes mouse coordinate
  coord_mouse_.x = -9999;
  coord_mouse_.y = -9999;

  // initializes plot
  plot_.set_background(*wxBLACK_BRUSH);
  plot_.set_is_fitted(true);
  plot_.set_offset(Point2d<float>(0, 0));
  plot_.set_scale(1);
  plot_.set_scale_x(1);
  plot_.set_scale_y(1);
  plot_.set_zoom_factor_fitted(1);

  // setting to avoid flickering
  this->SetBackgroundStyle(wxBG_STYLE_PAINT);
}

PlotPane2d::~PlotPane2d() {
}

void PlotPane2d::RenderPlot(wxDC& dc) {
  RenderBefore(dc);
  plot_.Render(dc, GetClientRect());
  RenderAfter(dc);
}

wxBrush PlotPane2d::background() const {
  return plot_.background();
}

const Plot2d* PlotPane2d::plot() const {
  return &plot_;
}

void PlotPane2d::set_background(const wxBrush& brush) {
  plot_.set_background(brush);
}

/// This function overrides the typical window erase background event handling.
/// When used in conjuction with double-buffered device contexts, it will
/// prevent flickering.
void PlotPane2d::OnEraseBackground(wxEraseEvent& event) {
  // do nothing
}

void PlotPane2d::OnMouse(wxMouseEvent& event) {
  if (event.Dragging() == true) {
    // checks if left button is pressed
    if (event.LeftIsDown() == false) {
      return;
    }

    // checks if cached mouse coordinate is valid
    // this guards against dragging events that start outside of window
    if ((coord_mouse_.x < 0) || (coord_mouse_.y < 0)) {
      return;
    }

    // disables plot fitting if active
    if (plot_.is_fitted() == true) {
      plot_.set_is_fitted(false);
    }

    // gets updated mouse point from event
    wxPoint coord_new;
    coord_new.x = event.GetX();
    coord_new.y = event.GetY();

    // finds difference between cached and new mouse points
    // applies inversion to make plot track mouse position
    const int kShiftX = (coord_new.x - coord_mouse_.x) * -1;
    const int kShiftY = (coord_new.y - coord_mouse_.y);
    plot_.Shift(kShiftX, kShiftY);

    // updates cached mouse point
    coord_mouse_ = coord_new;

    // refreshes window
    this->Refresh();
  } else if (event.Leaving() == true) {
    // resets mouse coordinates
    coord_mouse_.x = -9999;
    coord_mouse_.y = -9999;
  } else if (event.LeftDown() == true) {
    // forces the pane to get focus, which helps catch mouse events
    this->SetFocus();

    // caches the mouse coordinates
    coord_mouse_.x = event.GetX();
    coord_mouse_.y = event.GetY();
  } else if (event.LeftUp() == true) {
    // resets mouse coordinates
    coord_mouse_.x = -9999;
    coord_mouse_.y = -9999;
  } else if (event.MiddleDown() == true) {
    // forces the pane to get focus, which helps catch mouse events
    this->SetFocus();
  } else if (event.RightDown() == true) {
    // forces the pane to get focus, which helps catch mouse events
    this->SetFocus();
  }
}

void PlotPane2d::OnMouseWheel(wxMouseEvent& event) {
  // skips if no plot renderers are active
  if (plot_.HasRenderers() == false) {
    return;
  }

  // skips if point is outside of graphics rect
  wxRect rect = GetClientRect();
  if ((event.GetX() < 0) || (rect.GetWidth() < event.GetX())) {
    return;
  }

  if ((event.GetY() < 0) || (rect.GetHeight() < event.GetY())) {
    return;
  }

  // zoom factor
  const float kZoomFactor = 1.2;

  // zoom point
  wxPoint coord_zoom = event.GetPosition();

  if (event.GetWheelRotation() < 0) {
    // zooms out
    plot_.Zoom(1.0 / kZoomFactor, coord_zoom);
  } else if (0 < event.GetWheelRotation()) {
    // zooms in
    plot_.Zoom(kZoomFactor, coord_zoom);
  }

  // refreshes window
  this->Refresh();
}

void PlotPane2d::OnPaint(wxPaintEvent& event) {
  // gets a device context
  // a buffered device context helps prevent flickering
  wxBufferedPaintDC dc(this, bitmap_buffer_);

  // renders
  RenderPlot(dc);
}

void PlotPane2d::RenderAfter(wxDC& dc) {
  // placeholder for optional override
}

void PlotPane2d::RenderBefore(wxDC& dc) {
  // placeholder for optional override
}
