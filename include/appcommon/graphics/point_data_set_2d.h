// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef APPCOMMON_GRAPHICS_POINT_DATA_SET_2D_H_
#define APPCOMMON_GRAPHICS_POINT_DATA_SET_2D_H_

#include <list>

#include "models/base/point.h"

#include "appcommon/graphics/data_set_2d.h"

/// \par OVERVIEW
///
/// This class represents a set of 2D lines.
class PointDataSet2d : public DataSet2d {
 public:
  /// \brief Constructor.
  PointDataSet2d();

  /// \brief Destructor.
  virtual ~PointDataSet2d();

  /// \brief Adds a point to the dataset.
  /// \param[in] point
  ///   The point.
  /// This class will take ownership of the pointer.
  void Add(const Point2d<float>* point);

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
  const std::list<const Point2d<float>*>* data() const;

 private:
  /// \brief Updates the cached values.
  void Update() const;

  /// \var data_
  ///   The point data.
  std::list<const Point2d<float>*> data_;

  /// \var is_updated_
  ///   An indicator that tells if the cached values have been updated.
  mutable bool is_updated_;
};

#endif  // APPCOMMON_GRAPHICS_POINT_DATA_SET_2D_H_
