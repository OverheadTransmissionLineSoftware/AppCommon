// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "appcommon/graphics/renderer_2d.h"

Renderer2d::Renderer2d() {
  always_contrast_background_ = true;
  dataset_ = nullptr;
}

Renderer2d::~Renderer2d() {
}

bool Renderer2d::always_contrast_background() const {
  return always_contrast_background_;
}

const DataSet2d* Renderer2d::dataset() const {
  return dataset_;
}

void Renderer2d::set_always_contrast_background(
    const bool& always_contrast_background) {
  always_contrast_background_ = always_contrast_background;
}

void Renderer2d::set_dataset(const DataSet2d* dataset) {
  dataset_ = dataset;
}

void Renderer2d::ClipHorizontal(const PlotAxis& axis,
                                const float& x_vis, const float& y_vis,
                                float& x, float& y) {
  // determines axis boundary
  float b;
  if (x < axis.Min()) {
    b = axis.Min();
  } else {
    b = axis.Max();
  }

  // solves for coordinates along line that intersect the boundary
  y = (b - x_vis) * (y - y_vis) / (x - x_vis) + y_vis;
  x = b;
}

void Renderer2d::ClipVertical(const PlotAxis& axis,
                              const float& x_vis, const float& y_vis,
                              float& x, float& y) {
  // determines axis boundary
  float b;
  if (y < axis.Min()) {
    b = axis.Min();
  } else {
    b = axis.Max();
  }

  // solves for coordinates along line that intersect the boundary
  x = (b - y_vis) * (x - x_vis) / (y - y_vis) + x_vis;
  y = b;
}

wxCoord Renderer2d::DataToGraphics(const float& value,
                                   const float& value_min,
                                   const float& value_max,
                                   const int& range_graphics,
                                   const bool& is_vertical) {
  // calcs the value range
  const float range_values = value_max - value_min;

  // calculates ratio of point along axis
  float k = 0;
  if (is_vertical == true) {
    k = (value_max - value) / range_values;
  } else {
    k = (value - value_min) / range_values;
  }

  // scales to graphics range
  return k * range_graphics;
}

wxColour Renderer2d::InvertColor(const wxColour& color) {
  // calculates the inverted rgb components
  const int r = 255 - color.Red();
  const int g = 255 - color.Green();
  const int b = 255 - color.Blue();

  // returns
  return wxColour(r, g, b);
}
