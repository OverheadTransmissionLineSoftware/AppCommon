// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef APPCOMMON_GRAPHICS_LINE_DATA_SET_2D_H_
#define APPCOMMON_GRAPHICS_LINE_DATA_SET_2D_H_

#include <list>

#include "models/base/point.h"

#include "appcommon/graphics/data_set_2d.h"

/// \par OVERVIEW
///
/// This struct represents a 2D line.
struct Line2d {
  /// \var p0
  ///   The start point.
  Point2d<float> p0;

  /// \var p1
  ///   The end point.
  Point2d<float> p1;
};

/// \par OVERVIEW
///
/// This class represents a set of 2D lines.
class LineDataSet2d : public DataSet2d {
 public:
  /// \brief Constructor.
  LineDataSet2d();

  /// \brief Destructor.
  virtual ~LineDataSet2d();

  /// \brief Adds a line to the dataset.
  /// \param[in] line
  ///   The line.
  /// This class will take ownership of the pointer.
  void Add(const Line2d* line);

  /// \brief Clears all of the stored lines.
  void Clear();

  /// \brief Gets the maximum x value.
  /// \return The maximum x value.
  float MaxX() const override;

  /// \brief Gets the maximum y value.
  /// \return The maximum y value.
  float MaxY() const override;

  /// \brief Gets the minimum x value.
  /// \return The minimum x value.
  float MinX() const override;

  /// \brief Gets the minimum y value.
  /// \return The minimum y value.
  float MinY() const override;

  /// \brief Gets the data.
  /// \return The data.
  const std::list<const Line2d*>* data() const;

 private:
  /// \brief Updates the cached values.
  void Update() const;

  /// \var data_
  ///   The line data.
  std::list<const Line2d*> data_;

  /// \var is_updated_
  ///   An indicator that tells if the cached values have been updated.
  mutable bool is_updated_;
};

#endif  // APPCOMMON_GRAPHICS_LINE_DATA_SET_2D_H_
