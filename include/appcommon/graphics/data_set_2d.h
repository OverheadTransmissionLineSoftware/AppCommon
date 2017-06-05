// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef OTLS_APPCOMMON_GRAPHICS_DATASET2D_H_
#define OTLS_APPCOMMON_GRAPHICS_DATASET2D_H_

/// \par OVERVIEW
///
/// This class a set of data to be plotted.
class DataSet2d {
 public:
  /// \brief Constructor.
  DataSet2d();

  /// \brief Destructor.
  virtual ~DataSet2d();

  /// \brief Gets the maximum x value.
  /// \return The maximum x value.
  virtual float MaxX() const = 0;

  /// \brief Gets the maximum y value.
  /// \return The maximum y value.
  virtual float MaxY() const = 0;

  /// \brief Gets the minimum x value.
  /// \return The minimum x value.
  virtual float MinX() const = 0;

  /// \brief Gets the minimum y value.
  /// \return The minimum y value.
  virtual float MinY() const = 0;

 protected:
  /// \var x_max_
  ///   The maximum x value.
  mutable float x_max_;

  /// \var x_min_
  ///   The minimum x value.
  mutable float x_min_;

  /// \var y_max_
  ///   The maximum y value.
  mutable float y_max_;

  /// \var y_min_
  ///   The minimum value.
  mutable float y_min_;
};

#endif  // OTLS_APPCOMMON_GRAPHICS_DATASET2D_H_
