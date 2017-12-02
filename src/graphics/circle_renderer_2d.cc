// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "appcommon/graphics/circle_renderer_2d.h"

CircleRenderer2d::CircleRenderer2d() {
  brush_ = nullptr;
  pen_ = nullptr;
}

void CircleRenderer2d::Draw(wxDC& dc, wxRect rc,
                            const PlotAxis& axis_horizontal,
                            const PlotAxis& axis_vertical) const {
  // sets drawing brush
  if ((always_contrast_background_ == true)
       && (brush_->GetColour() == dc.GetBackground().GetColour())) {
    // gets the inverse color
    wxColour color_inverse = InvertColor(pen_->GetColour());

    // creates a new brush with the inverted color
    wxBrush brush = *brush_;
    brush.SetColour(color_inverse);

    // updates dc with inverted brush
    dc.SetBrush(brush);
  } else {
    // updates dc with typical brush
    dc.SetBrush(*brush_);
  }

  // sets drawing pen
  if ((always_contrast_background_ == true)
       && (pen_->GetColour() == dc.GetBackground().GetColour())) {
    // gets the inverse color
    wxColour color_inverse = InvertColor(pen_->GetColour());

    // creates a new pen with the inverted color
    wxPen pen = *pen_;
    pen.SetColour(color_inverse);

    // updates dc with inverted pen
    dc.SetPen(pen);
  } else {
    // updates dc with typical pen
    dc.SetPen(*pen_);
  }

  // casts to circle dataset
  const CircleDataSet2d* dataset =
      dynamic_cast<const CircleDataSet2d*>(dataset_);

  // draws each circle in dataset
  const std::list<const Circle2d*>* data = dataset->data();
  for (auto iter = data->cbegin(); iter != data->cend(); iter++) {
    // gets circle
    const Circle2d* circle = *iter;

    // gets circle center
    const float& x = circle->center.x;
    const float& y = circle->center.y;

    // skips lines that are out of range
    if ((x + circle->radius) < axis_horizontal.Min()) {
      continue;
    } else if (axis_horizontal.Max() < (x - circle->radius)) {
      continue;
    } else if ((y + circle->radius) < axis_vertical.Min()) {
      continue;
    } else if (axis_vertical.Max() < (y - circle->radius)) {
      continue;
    }

    // translates center to graphics coordinates
    wxCoord xg, yg;

    xg = DataToGraphics(x, axis_horizontal.Min(), axis_horizontal.Max(),
                        rc.GetWidth(), false);
    yg = DataToGraphics(y, axis_vertical.Min(), axis_vertical.Max(),
                        rc.GetHeight(), true);

    // translates radius to graphics scale
    wxCoord rg = static_cast<wxCoord>(circle->radius);

    // draws onto DC
    dc.DrawCircle(xg, yg, rg);
  }
}

const wxBrush* CircleRenderer2d::brush() const {
  return brush_;
}

const wxPen* CircleRenderer2d::pen() const {
  return pen_;
}

void CircleRenderer2d::set_brush(const wxBrush* brush) {
  brush_ = brush;
}

void CircleRenderer2d::set_pen(const wxPen* pen) {
  pen_ = pen;
}
