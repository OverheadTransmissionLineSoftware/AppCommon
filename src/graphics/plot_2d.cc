// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "appcommon/graphics/plot_2d.h"

#include <algorithm>

#include "appcommon/graphics/data_set_2d.h"

Plot2d::Plot2d() {
  offset_.x = -999999;
  offset_.y = -999999;
  ratio_aspect_ = 1;
  scale_ = -999999;
  zoom_factor_fitted_ = 1;

  limits_data_.x_max = -999999;
  limits_data_.x_min = 999999;
  limits_data_.y_max = -999999;
  limits_data_.y_min = 999999;

  is_fitted_ = false;
  is_updated_limits_data_ = false;
}

Plot2d::~Plot2d() {
  ClearRenderers();
}

void Plot2d::AddRenderer(const Renderer2d* renderer) {
  renderers_.push_back(renderer);

  is_updated_limits_data_ = false;
}

void Plot2d::ClearRenderers() {
  for (auto iter = renderers_.begin(); iter != renderers_.end(); iter++) {
    const Renderer2d* renderer = *iter;
    delete renderer;
  }

  renderers_.clear();

  is_updated_limits_data_ = false;
}

bool Plot2d::HasRenderers() const {
  if (renderers_.empty() == true) {
    return false;
  } else {
    return true;
  }
}

Plot2dDataLimits Plot2d::LimitsData() const {
  // updates plot data limits
  if (is_updated_limits_data_ == false) {
    UpdateDataLimits();
  }

  return limits_data_;
}

wxPoint Plot2d::PointDataToGraphics(const Point2d<float>& point_data) const {
  // scales and applies offset
  wxPoint point;
  point.x = (point_data.x / scale_) + offset_.x;
  point.y = (point_data.y / scale_ * ratio_aspect_) - offset_.y;

  return point;
}

Point2d<float> Plot2d::PointGraphicsToData(
    const wxPoint& point_graphics) const {
  // scales and applies offset
  Point2d<float> point;
  point.x = offset_.x + (static_cast<float>(point_graphics.x) * scale_);
  point.y = offset_.y - (static_cast<float>(point_graphics.y)
                         * scale_ / ratio_aspect_);

  return point;
}

void Plot2d::Render(wxDC& dc, wxRect rc) const {
  // sets background color and clears
  dc.SetBackgroundMode(wxSOLID);
  dc.SetBackground(brush_background_);
  dc.Clear();

  // fits plot data to graphics rect
  if (is_fitted_ == true) {
    // updates plot data limits
    if (is_updated_limits_data_ == false) {
      UpdateDataLimits();
    }

    // updates the offset and scale
    UpdateOffsetAndScaleToFitData(rc);

    // applies zoom adjustment at center of graphics rect
    wxPoint point_zoom;
    point_zoom.x = rc.GetPosition().x + (rc.GetWidth() / 2);
    point_zoom.y = rc.GetPosition().y + (rc.GetHeight() / 2);
    DoZoom(zoom_factor_fitted_, point_zoom);
  }

  // generates plot render axes
  const PlotAxis axis_horizontal = Axis(rc.GetPosition().x, rc.GetWidth(),
                                        false);
  const PlotAxis axis_vertical = Axis(rc.GetPosition().y, rc.GetHeight(),
                                      true);

  // triggers all renderers
  for (auto iter = renderers_.cbegin(); iter != renderers_.cend(); iter++) {
    const Renderer2d* renderer = *iter;
    renderer->Draw(dc, rc, axis_horizontal, axis_vertical);
  }
}

void Plot2d::Shift(const float& x, const float& y) {
  // converts to data coordinates
  const float kShiftX = x * scale_;
  const float kShiftY = y * scale_ / ratio_aspect_;

  offset_.x += kShiftX;
  offset_.y += kShiftY;
}

void Plot2d::Zoom(const float& factor, const wxPoint& point) {
  DoZoom(factor, point);

  // removes fitting
  is_fitted_ = false;
}

wxBrush Plot2d::background() const {
  return brush_background_;
}

bool Plot2d::is_fitted() const {
  return is_fitted_;
}

Point2d<float> Plot2d::offset() const {
  return offset_;
}

float Plot2d::ratio_aspect() const {
  return ratio_aspect_;
}

float Plot2d::scale() const {
  return scale_;
}

void Plot2d::set_background(const wxBrush& brush) {
  brush_background_ = brush;
}

void Plot2d::set_is_fitted(const bool& is_fitted) {
  is_fitted_ = is_fitted;
}

void Plot2d::set_offset(const Point2d<float>& offset) {
  offset_ = offset;
}

void Plot2d::set_ratio_aspect(const float& ratio_aspect) {
  ratio_aspect_ = ratio_aspect;
}

void Plot2d::set_scale(const float& scale) {
  scale_ = scale;
}

void Plot2d::set_zoom_factor_fitted(const float& zoom_factor_fitted) {
  zoom_factor_fitted_ = zoom_factor_fitted;
}

float Plot2d::zoom_factor_fitted() const {
  return zoom_factor_fitted_;
}

PlotAxis Plot2d::Axis(const int& position, const int& range,
                      const bool& is_vertical) const {
  // initializes axis
  PlotAxis axis;
  if (is_vertical == true) {
    axis = PlotAxis(PlotAxis::OrientationType::kVertical);

    // solves for center graphics position and converts to data coordinates
    float center = static_cast<float>(position)
                   + (static_cast<float>(range) / 2);
    center = offset_.y - (center * scale_ / ratio_aspect_);
    axis.set_position_center(center);

    // solves for range
    axis.set_range(range * scale_ / ratio_aspect_);
  } else {
    axis = PlotAxis(PlotAxis::OrientationType::kHorizontal);

    // solves for center graphics position and converts to data coordiantes
    float center = static_cast<float>(position)
                   + (static_cast<float>(range) / 2);
    center = offset_.x + (center * scale_);
    axis.set_position_center(center);

    // solves for range
    axis.set_range(range * scale_);
  }

  return axis;
}

void Plot2d::DoZoom(const float& factor, const wxPoint& point) const {
  // caches the data point corresponding to the graphics point
  const Point2d<float> point_old = PointGraphicsToData(point);

  // updates the scale
  scale_ *= factor;

  // calculates a data point corresponding to the graphics point
  Point2d<float> point_new = PointGraphicsToData(point);

  // updates offset
  offset_.x -= (point_new.x - point_old.x);
  offset_.y -= (point_new.y - point_old.y);
}

/// This method compares the aspect ratio (height/width) of the data and the
/// graphics rect to solve for offset and scaling.
void Plot2d::UpdateOffsetAndScaleToFitData(const wxRect& rc) const {
  // checks if rect is valid
  if (rc.GetHeight() == 0 || rc.GetWidth() == 0) {
    return;
  }

  // gets data rect height/width
  const float xg = static_cast<float>(rc.GetWidth());
  const float yg = static_cast<float>(rc.GetHeight());

  // solves for the data rect height/width
  float xd = limits_data_.x_max - limits_data_.x_min;
  float yd = (limits_data_.y_max - limits_data_.y_min) * ratio_aspect_;

  // compares the aspect ratios of the render and data rectangles to determine
  // which axis is most space limited
  const float kRatioAspectGraph = yg / xg;
  const float kRatioAspectData = yd / xd;

  // vertical axis controls
  if (kRatioAspectGraph <= kRatioAspectData) {
    // solves for scale
    scale_ = yd / yg;

    // solves for new data rect width
    xd = xg * (yd / yg);

    // solves for x average
    const float xavg = (limits_data_.x_max + limits_data_.x_min) / 2;

    // solves for upper left corner of graphics rect, but in data coordinates
    offset_.x = xavg - (xd / 2);
    offset_.y = limits_data_.y_max;

  // horizontal axis controls
  } else {
    // solves for scale
    scale_ = xd / xg;

    // solves for new data rect height
    yd = yg * (xd / xg);

    // solves for y average
    const float yavg = (limits_data_.y_max + limits_data_.y_min) / 2;

    // solves for upper left corner of graphics rect, but in data coordinates
    offset_.x = limits_data_.x_min;
    offset_.y = yavg + (yd  / ratio_aspect_ / 2);
  }
}

void Plot2d::UpdateDataLimits() const {
  // searches all the datasets and finds the min/max values for each axis
  float x_min = 999999;
  float x_max = -999999;
  float y_min = 999999;
  float y_max = -999999;
  for (auto iter = renderers_.cbegin(); iter != renderers_.cend(); iter++) {
    const Renderer2d* renderer = *iter;
    const DataSet2d* dataset = renderer->dataset();
    x_min = std::min(x_min, dataset->MinX());
    x_max = std::max(x_max, dataset->MaxX());

    y_min = std::min(y_min, dataset->MinY());
    y_max = std::max(y_max, dataset->MaxY());
  }

  limits_data_.x_min = x_min;
  limits_data_.x_max = x_max;
  limits_data_.y_min = y_min;
  limits_data_.y_max = y_max;

  is_updated_limits_data_ = true;
}
