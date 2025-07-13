#pragma once

#include <cmath>
#include <iostream>
#include <tuple>

namespace kurbo {

// Forward declarations
class Point;
class Size;
class Vec2;


/// A 2D point.
///
/// This type represents a point in 2D space. It has the same layout as Vec2, but
/// its meaning is different: Vec2 represents a change in location (for example velocity).
///
/// In general, kurbo overloads math operators where it makes sense, for example implementing
/// Affine * Point as the point under the affine transformation. However Point + Point and
/// f64 * Point are not implemented, because the operations do not make geometric sense.
class Point {
public:
    /// The x coordinate.
    double x;
    /// The y coordinate.
    double y;

    /// The point (0, 0).
    static const Point ZERO;
    /// The point at the origin; (0, 0).
    static const Point ORIGIN;

    /// Create a new Point with the provided x and y coordinates.
    constexpr Point(double x, double y) : x(x), y(y) {}

    /// Default constructor creates point at origin
    constexpr Point() : x(0.0), y(0.0) {}

    /// Convert this point into a Vec2.
    constexpr Vec2 to_vec2() const;

    /// Linearly interpolate between two points.
    Point lerp(const Point& other, double t) const;

    /// Determine the midpoint of two points.
    Point midpoint(const Point& other) const;

    /// Euclidean distance.
    double distance(const Point& other) const;

    /// Squared Euclidean distance.
    double distance_squared(const Point& other) const;

    /// Returns a new Point, with x and y rounded to the nearest integer.
    Point round() const;

    /// Returns a new Point, with x and y rounded up to the nearest integer.
    Point ceil() const;

    /// Returns a new Point, with x and y rounded down to the nearest integer.
    Point floor() const;

    /// Returns a new Point, with x and y rounded away from zero to the nearest integer.
    Point expand() const;

    /// Returns a new Point, with x and y rounded towards zero to the nearest integer.
    Point trunc() const;

    /// Is this point finite?
    bool is_finite() const;

    /// Is this point NaN?
    bool is_nan() const;

    // Operators
    Point operator+(const Vec2& other) const;
    Point& operator+=(const Vec2& other);
    Point operator-(const Vec2& other) const;
    Point& operator-=(const Vec2& other);
    Vec2 operator-(const Point& other) const;
    Point operator+(const std::tuple<double, double>& other) const;
    Point& operator+=(const std::tuple<double, double>& other);
    Point operator-(const std::tuple<double, double>& other) const;
    Point& operator-=(const std::tuple<double, double>& other);

    // Comparison operators
    bool operator==(const Point& other) const;
    bool operator!=(const Point& other) const;

    static Point from_tuple(const std::tuple<double, double>& v) {
        return Point(std::get<0>(v), std::get<1>(v));
    }
    std::tuple<double, double> to_tuple() const {
        return std::make_tuple(x, y);
    }

    // Default
    static Point zero();
};



// Global conversion functions
Point from_tuple(const std::tuple<double, double>& v);
std::tuple<double, double> to_tuple(const Point& v);

// Stream operators
std::ostream& operator<<(std::ostream& os, const Point& point);

} // namespace kurbo 