#pragma once

#include <cmath>
#include <iostream>
#include <tuple>
#include <numeric>

namespace kurbo {

// Forward declarations
class Point;
class Size;
class Vec2;

/// A 2D vector.
///
/// This is intended primarily for a vector in the mathematical sense,
/// but it can be interpreted as a translation, and converted to and
/// from a Point (vector relative to the origin) and Size.
class Vec2 {
public:
    /// The x-coordinate.
    double x;
    /// The y-coordinate.
    double y;

    /// The vector (0, 0).
    static const Vec2 ZERO;

    /// Create a new vector.
    constexpr Vec2(double x, double y) : x(x), y(y) {}

    /// Default constructor creates zero vector
    constexpr Vec2() : x(0.0), y(0.0) {}

    /// Convert this vector into a Point.
    constexpr Point to_point() const;

    /// Convert this vector into a Size.
    constexpr Size to_size() const;

    /// Create a Vec2 with the same value for x and y
    static constexpr Vec2 splat(double v) {
        return Vec2(v, v);
    }

    /// Dot product of two vectors.
    double dot(const Vec2& other) const;

    /// Cross product of two vectors.
    ///
    /// This is signed so that (1, 0) × (0, 1) = 1.
    ///
    /// The following relations hold:
    ///
    /// u.cross(v) = -v.cross(u)
    ///
    /// v.cross(v) = 0.0
    double cross(const Vec2& other) const;

    /// Magnitude of vector.
    double hypot() const;

    /// Magnitude of vector.
    ///
    /// This is an alias for Vec2::hypot.
    double length() const;

    /// Magnitude squared of vector.
    double hypot2() const;

    /// Magnitude squared of vector.
    ///
    /// This is an alias for Vec2::hypot2.
    double length_squared() const;

    /// Find the angle in radians between this vector and the vector Vec2 { x: 1.0, y: 0.0 }
    /// in the positive y direction.
    ///
    /// If the vector is interpreted as a complex number, this is the argument.
    /// The angle is expressed in radians.
    double atan2() const;

    /// Find the angle in radians between this vector and the vector Vec2 { x: 1.0, y: 0.0 }
    /// in the positive y direction.
    ///
    /// This is an alias for Vec2::atan2.
    double angle() const;

    /// A unit vector of the given angle.
    ///
    /// With th at zero, the result is the positive X unit vector, and
    /// at π/2, it is the positive Y unit vector. The angle is expressed
    /// in radians.
    ///
    /// Thus, in a Y-down coordinate system (as is common for graphics),
    /// it is a clockwise rotation, and in Y-up (traditional for math), it
    /// is anti-clockwise. This convention is consistent with Affine::rotate.
    static Vec2 from_angle(double th);

    /// Linearly interpolate between two vectors.
    Vec2 lerp(const Vec2& other, double t) const;

    /// Returns a vector of magnitude 1.0 with the same angle as self; i.e.
    /// a unit/direction vector.
    ///
    /// This produces NaN values when the magnitude is 0.
    Vec2 normalize() const;

    /// Returns a new Vec2, with x and y rounded to the nearest integer.
    Vec2 round() const;

    /// Returns a new Vec2, with x and y rounded up to the nearest integer.
    Vec2 ceil() const;

    /// Returns a new Vec2, with x and y rounded down to the nearest integer.
    Vec2 floor() const;

    /// Returns a new Vec2, with x and y rounded away from zero to the nearest integer.
    Vec2 expand() const;

    /// Returns a new Vec2, with x and y rounded towards zero to the nearest integer.
    Vec2 trunc() const;

    /// Is this vector finite?
    bool is_finite() const;

    /// Is this vector NaN?
    bool is_nan() const;

    /// Rotate this vector by 90 degrees counterclockwise.
    Vec2 turn_90() const;

    /// Rotate and scale this vector by another vector.
    Vec2 rotate_scale(const Vec2& rhs) const;

    // Arithmetic operators
    Vec2 operator+(const Vec2& other) const;
    Vec2& operator+=(const Vec2& other);
    Vec2 operator-(const Vec2& other) const;
    Vec2& operator-=(const Vec2& other);
    Vec2 operator*(double scalar) const;
    Vec2& operator*=(double scalar);
    Vec2 operator*(const Vec2& other) const;
    Vec2 operator/(double scalar) const;
    Vec2& operator/=(double scalar);
    Vec2 operator-() const;

    // Comparison operators
    bool operator==(const Vec2& other) const;
    bool operator!=(const Vec2& other) const;

    static Vec2 from_tuple(const std::tuple<double, double>& v) {
        return Vec2(std::get<0>(v), std::get<1>(v));
    }
    std::tuple<double, double> to_tuple() const {
        return std::make_tuple(x, y);
    }
};


// Global conversion functions
// Free functions
Vec2 operator*(double scalar, const Vec2& vec);


// Stream operators
std::ostream& operator<<(std::ostream& os, const Vec2& vec);

} // namespace kurbo 