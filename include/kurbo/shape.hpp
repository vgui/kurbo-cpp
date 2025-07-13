#pragma once

#include <iterator>
#include <optional>
#include <vector>
#include "kurbo/path_el.hpp"

namespace kurbo {

// Forward declarations
class Point;
class Rect;
class Line;
class Circle;
class RoundedRect;
class BezPath;
class PathSeg;

// Forward declaration for Segments iterator
template<typename Iter>
class Segments;

/// A generic trait for open and closed shapes.
///
/// This trait provides conversion from shapes to BezPath, as well as
/// general geometry functionality like computing area, bounding boxes,
/// and winding number.
class Shape {
public:
    /// Virtual destructor for proper cleanup
    virtual ~Shape() = default;

    /// Returns an iterator over this shape expressed as PathEls;
    /// that is, as Bézier path elements.
    ///
    /// All shapes can be represented as Béziers, but in many situations
    /// (such as when interfacing with a platform drawing API) there are more
    /// efficient native types for specific concrete shapes. In this case,
    /// the user should exhaust the as_ methods (as_rect, as_line, etc)
    /// before converting to a BezPath, as those are likely to be more
    /// efficient.
    ///
    /// In many cases, shapes are able to iterate their elements without
    /// allocating; however creating a BezPath object always allocates.
    /// If you need an owned BezPath you can use to_path instead.
    ///
    /// The tolerance parameter controls the accuracy of
    /// conversion of geometric primitives to Bézier curves, as
    /// curves such as circles cannot be represented exactly but
    /// only approximated. For drawing as in UI elements, a value
    /// of 0.1 is appropriate, as it is unlikely to be visible to
    /// the eye. For scientific applications, a smaller value
    /// might be appropriate. Note that in general the number of
    /// cubic Bézier segments scales as tolerance ^ (-1/6).
    virtual std::vector<PathEl> path_elements(double tolerance) const = 0;

    /// Convert to a Bézier path.
    ///
    /// This always allocates. It is appropriate when both the source
    /// shape and the resulting path are to be retained.
    ///
    /// If you only need to iterate the elements (such as to convert them to
    /// drawing commands for a given 2D graphics API) you should prefer
    /// path_elements, which can avoid allocating where possible.
    ///
    /// The tolerance parameter is the same as for path_elements.
    virtual BezPath to_path(double tolerance) const;

    /// Convert into a Bézier path.
    ///
    /// This allocates in the general case, but is zero-cost if the
    /// shape is already a BezPath.
    ///
    /// The tolerance parameter is the same as for path_elements().
    virtual BezPath into_path(double tolerance) const;

    /// Returns an iterator over this shape expressed as Bézier path
    /// segments (PathSegs).
    ///
    /// The allocation behaviour and tolerance parameter are the
    /// same as for path_elements()
    // virtual Segments<std::vector<PathEl>::const_iterator> path_segments(double tolerance) const;

    /// Signed area.
    ///
    /// This method only produces meaningful results with closed shapes.
    ///
    /// The convention for positive area is that y increases when x is
    /// positive. Thus, it is clockwise when down is increasing y (the
    /// usual convention for graphics), and anticlockwise when
    /// up is increasing y (the usual convention for math).
    virtual double area() const = 0;

    /// Total length of perimeter.
    virtual double perimeter(double accuracy) const = 0;

    /// The winding number of a point.
    ///
    /// This method only produces meaningful results with closed shapes.
    ///
    /// The sign of the winding number is consistent with that of area,
    /// meaning it is +1 when the point is inside a positive area shape
    /// and -1 when it is inside a negative area shape. Of course, greater
    /// magnitude values are also possible when the shape is more complex.
    virtual int winding(const Point& pt) const = 0;

    /// Returns true if the Point is inside this shape.
    ///
    /// This is only meaningful for closed shapes. Some shapes may have specialized
    /// implementations of this function or of winding determination.
    ///
    /// The default implementation uses the non-zero winding rule.
    ///
    /// To determine containment using the even-odd winding rule, check the
    /// winding directly.
    virtual bool contains(const Point& pt) const;

    /// The smallest rectangle that encloses the shape.
    virtual Rect bounding_box() const = 0;

    /// If the shape is a line, make it available.
    virtual std::optional<Line> as_line() const;

    /// If the shape is a rectangle, make it available.
    virtual std::optional<Rect> as_rect() const;

    /// If the shape is a rounded rectangle, make it available.
    virtual std::optional<RoundedRect> as_rounded_rect() const;

    /// If the shape is a circle, make it available.
    virtual std::optional<Circle> as_circle() const;

    /// If the shape is stored as a slice of path elements, make
    /// that available.
    ///
    /// Note: when GAT's land, a method like path_elements would be
    /// able to iterate through the slice with no extra allocation,
    /// without making any assumption that storage is contiguous.
    virtual std::optional<std::vector<PathEl>> as_path_slice() const;
};

} // namespace kurbo 