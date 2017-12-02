// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef OTLS_APPCOMMON_GRAPHICS_RENDERER2D_H_
#define OTLS_APPCOMMON_GRAPHICS_RENDERER2D_H_

#include "wx/wx.h"

#include "appcommon/graphics/data_set_2d.h"
#include "appcommon/graphics/plot_axis.h"

/// \par OVERVIEW
///
/// This class is an abstract renderer which must be derived from.
class Renderer2d {
 public:
  /// \brief Constructor.
  Renderer2d();

  /// \brief Destructor.
  virtual ~Renderer2d();

  /// \brief Draws the data onto the device context.
  /// \param[in] dc
  ///   The device context.
  /// \param[in] rc
  ///   The dc region to draw onto.
  /// \param[in] axis_horizontal
  ///   The horizontal plot axis.
  /// \param[in] axis_vertical
  ///   The vertical plot axis.
  virtual void Draw(wxDC& dc, wxRect rc, const PlotAxis& axis_horizontal,
                    const PlotAxis& axis_vertical) const = 0;

  /// \brief Gets if the rendered item must contrast the background color.
  /// \return If the rendered item must always contrast the background color.
  bool always_contrast_background() const;

  /// \brief Gets the dataset.
  /// \return The dataset.
  const DataSet2d* dataset() const;

  /// \brief Sets if the rendered item must contrast the background color.
  /// \param[in] always_contrast_background
  ///   The indicator.
  void set_always_contrast_background(const bool& always_contrast_background);

  /// \brief Sets the dataset.
  /// \param[in] dataset
  ///   The dataset.
  void set_dataset(const DataSet2d* dataset);

 protected:
  /// \brief Clips the points to fit the horizontal axis.
  /// \param[in] axis
  ///   The axis, which contains the bounds/limits.
  /// \param[in] x_vis
  ///   The visible x coordinate.
  /// \param[in] y_vis
  ///   The visible y coordinate.
  /// \param[in,out] x
  ///   The x coordinate that needs clipped. At the start of the function
  ///   this coordinate is not visible.
  /// \param[in,out] y
  ///   The x coordinate that needs clipped. At the start of the function
  ///   this coordinate is not visible.
  static void ClipHorizontal(const PlotAxis& axis,
                             const float& x_vis, const float& y_vis,
                             float& x, float& y);

  /// \brief Clips the points to fit the horizontal axis.
  /// \param[in] axis
  ///   The axis, which contains the bounds/limits.
  /// \param[in] x_vis
  ///   The visible x coordinate.
  /// \param[in] y_vis
  ///   The visible y coordinate.
  /// \param[in,out] x
  ///   The x coordinate that needs clipped. At the start of the function
  ///   this coordinate is not visible.
  /// \param[in,out] y
  ///   The x coordinate that needs clipped. At the start of the function
  ///   this coordinate is not visible.
  static void ClipVertical(const PlotAxis& axis,
                           const float& x_vis, const float& y_vis,
                           float& x, float& y);

  /// \brief Translates from data to graphics coordinates.
  /// \param[in] value
  ///   The data value to convert.
  /// \param[in] value_min
  ///   The minimum data value (at the edge of the graphics drawing region).
  /// \param[in] value_max
  ///   The maximum data value (at the edge of the graphics drawing region).
  /// \param[in] range_graphics
  ///   The range (width/height) of the graphics coordinates.
  /// \param[in] is_vertical
  ///   An indicator that tells which type of conversion to do. Data with a
  ///   vertical orientations has to be inverted to match the graphics
  ///   coordinate system.
  /// \return The graphics coordinate.
  static wxCoord DataToGraphics(const float& value,
                                const float& value_min,
                                const float& value_max,
                                const int& range_graphics,
                                const bool& is_vertical);

  /// \brief Inverts a color.
  /// \param[in] color
  ///   The color to invert.
  /// \return The inverted color.
  static wxColour InvertColor(const wxColour& color);

  /// \var always_contrast_background_
  ///   An indicator that determines if the rendered item color must always
  ///   be different than the background color.
  bool always_contrast_background_;

  /// \var dataset_
  ///   The dataset.
  const DataSet2d* dataset_;
};

#endif  // OTLS_APPCOMMON_GRAPHICS_RENDERER2D_H_
