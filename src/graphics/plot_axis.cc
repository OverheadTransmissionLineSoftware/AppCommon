// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "appcommon/graphics/plot_axis.h"

PlotAxis::PlotAxis() {
  max_ = -999999;
  min_ = 999999;
  orientation_ = OrientationType::kNull;
  position_center_ = -999999;
  range_ = -999999;
}

PlotAxis::PlotAxis(PlotAxis::OrientationType orientation) {
  orientation_ = orientation;

  max_ = -999999;
  min_ = 999999;
  position_center_ = -999999;
  range_ = -999999;
}

PlotAxis::~PlotAxis() {
}

bool PlotAxis::IsVisible(const float& v) const {
  return (v >= min_) && (v <= max_);
}

float PlotAxis::Max() const {
  return max_;
}

float PlotAxis::Min() const {
  return min_;
}

float PlotAxis::position_center() const {
  return position_center_;
}

float PlotAxis::range() const {
  return range_;
}

void PlotAxis::set_position_center(const float& position_center) {
  position_center_ = position_center;

  min_ = position_center_ - (range_ / 2);
  max_ = position_center_ + (range_ / 2);
}

void PlotAxis::set_range(const float& range) {
  range_ = range;

  min_ = position_center_ - (range_ / 2);
  max_ = position_center_ + (range_ / 2);
}
