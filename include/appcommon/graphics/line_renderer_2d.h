// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef APPCOMMON_GRAPHICS_LINE_RENDERER_2D_H_
#define APPCOMMON_GRAPHICS_LINE_RENDERER_2D_H_

#include "wx/wx.h"

#include "appcommon/graphics/line_data_set_2d.h"
#include "appcommon/graphics/renderer_2d.h"

/// \par OVERVIEW
///
/// This class renders a line dataset onto a plot.
class LineRenderer2d : public Renderer2d {
 public:
  /// \brief Constructor.
  LineRenderer2d();

  /// \brief Draws the data onto the device context.
  /// \param[in] dc
  ///   The device context.
  /// \param[in] rc
  ///   The dc region to draw onto.
  /// \param[in] axis_horizontal
  ///   The horizontal plot axis.
  /// \param[in] axis_vertical
  ///   The vertical plot axis.
  void Draw(wxDC& dc, wxRect rc, const PlotAxis& axis_horizontal,
            const PlotAxis& axis_vertical) const override;

  /// \brief Gets the pen.
  /// \return The pen.
  const wxPen* pen() const;

  /// \brief Sets the pen.
  /// \param[in] pen
  ///   The pen.
  void set_pen(const wxPen* pen);

 private:
  /// \var pen_
  ///   The pen.
  const wxPen* pen_;
};

#endif  // APPCOMMON_GRAPHICS_LINE_RENDERER_2D_H_
