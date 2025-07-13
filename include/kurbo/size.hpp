#pragma once

#include <cmath>
#include <iostream>
#include <tuple>
#include <limits>

namespace kurbo {

// Forward declarations
class Point;
class Size;
class Vec2;
class Rect;
class RoundedRect;
class RoundedRectRadii;

/// A 2D size.
class Size {
public:
    /// The width.
    double width;
    /// The height.
    double height;

    /// A size with zero width or height.
    static const Size ZERO;
    /// A size with width and height set to infinity.
    static const Size INFINITE;

    /// Create a new Size with the provided width and height.
    constexpr Size(double width, double height) : width(width), height(height) {}

    /// Default constructor creates zero size
    constexpr Size() : width(0.0), height(0.0) {}

    /// Returns the max of width and height.
    double max_side() const;

    /// Returns the min of width and height.
    double min_side() const;

    /// The area covered by this size.
    double area() const;

    /// Whether this size has zero area.
    bool is_zero_area() const;

    /// Whether this size has zero area.
    ///
    /// Note: a size with negative area is not considered empty.
    bool is_empty() const;

    /// Returns the component-wise minimum of self and other.
    Size min(const Size& other) const;

    /// Returns the component-wise maximum of self and other.
    Size max(const Size& other) const;

    /// Returns a new size bounded by min and max.
    Size clamp(const Size& min, const Size& max) const;

    /// Convert this size into a Vec2, with width mapped to x and height
    /// mapped to y.
    constexpr Vec2 to_vec2() const;

    /// Returns a new Size, with width and height rounded to the nearest integer.
    Size round() const;

    /// Returns a new Size, with width and height rounded up to the nearest integer.
    Size ceil() const;

    /// Returns a new Size, with width and height rounded down to the nearest integer.
    Size floor() const;

    /// Returns a new Size, with width and height rounded away from zero to the nearest integer.
    Size expand() const;

    /// Returns a new Size, with width and height rounded towards zero to the nearest integer.
    Size trunc() const;

    /// Returns the aspect ratio (width / height).
    double aspect_ratio() const;

    /// Convert this size to a Rect with origin at (0, 0).
    Rect to_rect() const;

    /// Convert this size to a RoundedRect with origin at (0, 0).
    RoundedRect to_rounded_rect(const RoundedRectRadii& radii) const;

    /// Is this size finite?
    bool is_finite() const;

    /// Is this size NaN?
    bool is_nan() const;

    // Arithmetic operators
    Size& operator*=(double scalar);
    Size operator*(const Size& other) const;
    Size operator*(double scalar) const;
    Size& operator/=(double scalar);
    Size operator/(double scalar) const;
    Size operator+(const Size& other) const;
    Size& operator+=(const Size& other);
    Size operator-(const Size& other) const;
    Size& operator-=(const Size& other);

    // Comparison operators
    bool operator==(const Size& other) const;
    bool operator!=(const Size& other) const;

    static Size from_tuple(const std::tuple<double, double>& v) {
        return Size(std::get<0>(v), std::get<1>(v));
    }
    std::tuple<double, double> to_tuple() const {
        return std::make_tuple(width, height);
    }
};


// Global conversion functions
// Size from_tuple(const std::tuple<double, double>& v); // Deleted
// std::tuple<double, double> to_tuple(const Size& v); // Deleted

// Free functions
Size operator*(double scalar, const Size& size);



// Stream operators
std::ostream& operator<<(std::ostream& os, const Size& size);

} // namespace kurbo 