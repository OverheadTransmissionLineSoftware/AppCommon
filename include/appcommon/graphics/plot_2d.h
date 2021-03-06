// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef APPCOMMON_GRAPHICS_PLOT_2D_H_
#define APPCOMMON_GRAPHICS_PLOT_2D_H_

#include <list>

#include "models/base/point.h"
#include "wx/wx.h"

#include "appcommon/graphics/plot_axis.h"
#include "appcommon/graphics/renderer_2d.h"

/// \par OVERVIEW
///
/// This struct contains mix/max values for the x and y axis.
struct Plot2dDataLimits {
  /// \var x_max
  ///   The maximum x value.
  float x_max;

  /// \var x_min
  ///   The minimum x value.
  float x_min;

  /// \var y_max
  ///   The maximum y value.
  float y_max;

  /// \var y_min
  ///   The minimum y value.
  float y_min;
};

/// \par OVERVIEW
///
/// This class is a 2D plot. It accepts data/renderers and renders them onto a
/// device context.
///
/// \par COORDINATE SYSTEMS
///
/// There are two coordinate systems that are used in this class:
/// - data
/// - graphics
///
/// The data coordinate system aligns with the data/renderers. The horizontal
/// axis increases to the right, while the vertical axis increases upward.
///
/// The graphics coordinate system origin aligns with the upper left corner of
/// the rendering window. The horizontal axis increases to the right, while the
/// vertical axis increases downward. The vertical axis is inverted compared to
/// the data coordinate system.
///
/// To translate between these coordinate systems, this class uses an offset for
/// each axis and a scaling factor.
///
/// \par RENDERERS
///
/// This plot uses individual renderers of varying types to render the data onto
/// the device context. Multiple renderers may be added to create successive
/// layers to be drawn.
///
/// This plot copies the renderers as they are added to the class, but does NOT
/// own the datasets that are referenced by the renderers. This is done by
/// design, as this class is purely a data visualizer.
///
/// \par AXIS SCALING
///
/// This plot allows the vertical and horizontal axes to have different scales.
/// The data will stay in the native units, but the graphics coordinates will be
/// scaled when rendered.
///
/// \par FIT
///
/// The plot can fit the data to a specific device context size. It can center
/// the data on the graphics rect, adjust the general scale to fit exactly, and
/// then apply the zoom modifier.
///
/// \par SHIFTING
///
/// The plot can be shifted on both axes.
///
/// \par ZOOM
///
/// This plot supports zooming in/out.
class Plot2d {
 public:
  /// \brief Constructor.
  Plot2d();

  /// \brief Destructor.
  ~Plot2d();

  /// \brief Adds a renderer.
  /// \param[in] renderer
  ///   The renderer. The plot will take ownership of the pointer.
  void AddRenderer(const Renderer2d* renderer);

  /// \brief Clears the renderers.
  void ClearRenderers();

  /// \brief Gets the plot data limits.
  /// \return The plot data limits.
  Plot2dDataLimits LimitsData() const;

  /// \brief Gets if the plot has any renderers.
  /// \return If the plot has any renderers.
  bool HasRenderers() const;

  /// \brief Translates a data coordinate to a graphics coordinate.
  /// \param[in] point_data
  ///   The data coordinate.
  /// \return The graphics coordinate.
  wxPoint PointDataToGraphics(const Point2d<float>& point_data) const;

  /// \brief Translates a graphics coordinate to a data coordinate.
  /// \param[in] point_graphics
  ///   The graphics coordinate.
  /// \return The data coordinate.
  Point2d<float> PointGraphicsToData(const wxPoint& point_graphics) const;

  /// \brief Renders the plot.
  /// \param[in] dc
  ///   The device context.
  /// \param[in] rc
  ///   The rectangle of the rendering region.
  void Render(wxDC& dc, wxRect rc) const;

  /// \brief Shifts the plot.
  /// \param[in] x
  ///   The amount (in graphics units) to shift the x axis.
  /// \param[in] y
  ///   The amount (in graphics units) to shift the y axis.
  /// The shift is converted from graphics units to data units and applied to
  /// the offset.
  void Shift(const int& x, const int& y);

  /// \brief Zooms the plot.
  /// \param[in] factor
  ///   The zoom factor, which is used to adjust the current plot scale.
  /// \param[in] point
  ///   The point (in graphics units) to zoom to.
  /// This function does not render the graph. The function parameters are used
  /// to update the internal class members only.
  void Zoom(const float& factor, const wxPoint& point);

  /// \brief Gets the background brush.
  /// \return The background brush.
  wxBrush background() const;

  /// \brief Gets if the plot data is fitted when rendered.
  /// \return If the plot data is fitted when rendered.
  bool is_fitted() const;

  /// \brief Gets the offset.
  /// \return The offset.
  Point2d<float> offset() const;

  /// \brief Gets the scaling factor.
  /// \return The scaling factor.
  float scale() const;

  /// \brief Gets the x-axis scaling factor.
  /// \return The x-axis scaling factor.
  float scale_x() const;

  /// \brief Gets the y-axis scaling factor.
  /// \return The y-axis scaling factor.
  float scale_y() const;

  /// \brief Sets the background brush.
  /// \param[in] brush
  ///   The background brush.
  void set_background(const wxBrush& brush);

  /// \brief Sets if the plot data is fitted when rendered.
  /// \param[in] is_fitted
  ///   An indicator that determines if the plot data is fitted to the window
  ///   when rendered.
  void set_is_fitted(const bool& is_fitted);

  /// \brief Sets the offset.
  /// \param[in] offset
  ///   The offset.
  void set_offset(const Point2d<float>& offset);

  /// \brief Sets the scale.
  /// \param[in] scale
  ///   The scaling factor.
  void set_scale(const float& scale);

  /// \brief Sets the x-axis scale.
  /// \param[in] scale_x
  ///   The x-axis scaling factor.
  void set_scale_x(const float& scale_x);

  /// \brief Sets the y-axis scale.
  /// \param[in] scale_y
  ///   The y-axis scaling factor.
  void set_scale_y(const float& scale_y);

  /// \brief Sets the zoom factor to apply after fitting the plot.
  /// \param[in] zoom_factor_fitted
  ///   The zoom factor to apply after fitting the plot.
  void set_zoom_factor_fitted(const float& zoom_factor_fitted);

  /// \brief Gets the zoom factor to apply after fitting the plot.
  /// \return The zoom factor to apply after fitting the plot.
  float zoom_factor_fitted() const;

 private:
  /// \brief Gets a plot axis used for rendering.
  /// \param[in] position
  ///   The upper/left coordinate component of the graphics rectangle.
  /// \param[in] range
  ///   The height/width of the graphics rectangle.
  /// \param[in] is_vertical
  ///   An indicator that adjusts the axis to create.
  /// \return A plot axis (in data coordinates) of the area being rendered.
  ///   These axes are needed by the renderers to draw onto the graphics rect.
  PlotAxis Axis(const int& position, const int& range,
                const bool& is_vertical) const;

  /// \brief Does a zoom.
  /// \param[in] factor
  ///   The zoom factor, which is used to adjust the current plot scale.
  /// \param[in] point
  ///   The point (in graphics units) to zoom to.
  void DoZoom(const float& factor, const wxPoint& point) const;

  /// \brief Updates the plot offset and scale to encapsulate the plot data.
  /// \param[in] rc
  ///   The graphics rectangle.
  /// This is typically done when fitting the data to the graphics rect.
  void UpdateOffsetAndScaleToFitData(const wxRect& rc) const;

  /// \brief Updates the data boundaries.
  void UpdateDataLimits() const;

  /// \var brush_background_
  ///   The background brush.
  wxBrush brush_background_;

  /// \var limits_data_
  ///   The plot data min/max values.
  mutable Plot2dDataLimits limits_data_;

  /// \var is_fitted_
  ///   An indicator that tells if the plot is fitted to the graphics rect on a
  ///   redraw.
  bool is_fitted_;

  /// \var is_updated_limits_data_
  ///   An indicator that tells if the data limits are updated.
  mutable bool is_updated_limits_data_;

  /// \var offset_
  ///   The horizontal and vertical offset from the data coordinate system origin
  ///   to the graphics coordinate system origin. This is effectively the graphics
  ///   origin defined in the data coordinate system.
  mutable Point2d<float> offset_;

  /// \var renderers_
  ///   The list of renderers.
  std::list<const Renderer2d*> renderers_;

  /// \var scale_
  ///   The factor that is used to scale the plot (x+y axes) data coordinates
  ///   to graphics coordinates. This factor and the individual axis factor are
  ///   combined when scaling.
  mutable float scale_;

  /// \var scale_x_
  ///   A factor that is used to scale the x-axis data coordinates to graphics
  ///   coordinates. This factor and the general scale factor are combined when
  ///   scaling.
  float scale_x_;

  /// \var scale_y_
  ///   A factor that is used to scale the y-axis data coordinates to graphics
  ///   coordinates. This factor and the general scale factor are combined when
  ///   scaling.
  float scale_y_;

  /// \var zoom_factor_fitted_
  ///   The zoom factor to apply after the plot is fitted. Setting this to 1
  ///   will not modify the scale or offset.
  float zoom_factor_fitted_;
};

#endif  // APPCOMMON_GRAPHICS_PLOT_2D_H_
