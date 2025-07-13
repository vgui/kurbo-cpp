#pragma once

// Affine transform class (2D)
// Ported from kurbo::Affine (Rust)
// Copyright 2024
//
// This class represents a 2D affine transformation matrix.
// The matrix is stored as 6 coefficients: [a, b, c, d, e, f]
// representing the matrix:
// | a c e |
// | b d f |
// | 0 0 1 |

#include <array>
#include <cmath>
#include "point.hpp"
#include "vec2.hpp"
#include "size.hpp"

namespace kurbo {

class Affine {
public:
    // The coefficients of the affine transform
    std::array<double, 6> coeffs;

    // Constructors
    constexpr Affine() : coeffs{1.0, 0.0, 0.0, 1.0, 0.0, 0.0} {}
    constexpr Affine(const std::array<double, 6>& c) : coeffs(c) {}

    // Static constants
    static const Affine IDENTITY;
    static const Affine FLIP_Y;
    static const Affine FLIP_X;

    // Static constructors
    static constexpr Affine scale(double s) {
        return Affine({s, 0.0, 0.0, s, 0.0, 0.0});
    }
    static constexpr Affine scale_non_uniform(double sx, double sy) {
        return Affine({sx, 0.0, 0.0, sy, 0.0, 0.0});
    }
    static Affine scale_about(double s, const Point& center);
    static Affine rotate(double th);
    static Affine rotate_about(double th, const Point& center);
    static Affine translate(const Vec2& v);
    static Affine skew(double skew_x, double skew_y);
    static Affine reflect(const Point& point, const Vec2& direction);

    // Chaining
    Affine pre_rotate(double th) const;
    Affine pre_rotate_about(double th, const Point& center) const;
    Affine pre_scale(double s) const;
    Affine pre_scale_non_uniform(double sx, double sy) const;
    Affine pre_translate(const Vec2& v) const;
    Affine then_rotate(double th) const;
    Affine then_rotate_about(double th, const Point& center) const;
    Affine then_scale(double s) const;
    Affine then_scale_non_uniform(double sx, double sy) const;
    Affine then_scale_about(double s, const Point& center) const;
    Affine then_translate(const Vec2& v) const;

    // Matrix operations
    std::array<double, 6> as_coeffs() const;
    double determinant() const;
    Affine inverse() const;

    // Transformations
    Point operator*(const Point& p) const;
    Vec2 operator*(const Vec2& v) const;
    Size operator*(const Size& s) const;

    // Utility
    bool is_finite() const;
    bool is_nan() const;
    Vec2 translation() const;
    Affine with_translation(const Vec2& v) const;
    std::pair<Vec2, double> svd() const;

    // Operators
    Affine operator*(const Affine& other) const;
    Affine& operator*=(const Affine& other);

    // Default
    static Affine identity();
};

// Stream operator
std::ostream& operator<<(std::ostream& os, const Affine& affine);

} // namespace kurbo 