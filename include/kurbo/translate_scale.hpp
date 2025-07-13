#pragma once

// TranslateScale transform class (2D)
// Ported from kurbo::TranslateScale (Rust)
// Copyright 2024
//
// This class represents a 2D transformation: uniform scaling followed by translation.
// The matrix is stored as scale (double) and translation (Vec2):
// | s 0 x |
// | 0 s y |
// | 0 0 1 |

#include "vec2.hpp"
#include "point.hpp"
#include "affine.hpp"
#include <cmath>

namespace kurbo {

class TranslateScale {
public:
    Vec2 translation;
    double scale;

    // Constructors
    constexpr TranslateScale() : translation{0.0, 0.0}, scale(1.0) {}
    constexpr TranslateScale(const Vec2& translation, double scale) : translation(translation), scale(scale) {}

    // Static constructors
    static constexpr TranslateScale new_transform(const Vec2& translation, double scale) {
        return TranslateScale(translation, scale);
    }
    static constexpr TranslateScale scale_only(double s) {
        return TranslateScale(Vec2{0.0, 0.0}, s);
    }
    static TranslateScale translate_only(const Vec2& translation);
    static TranslateScale from_scale_about(double scale, const Point& focus);

    // Inverse
    TranslateScale inverse() const;

    // Utility
    bool is_finite() const;
    bool is_nan() const;

    // Operators
    Point operator*(const Point& p) const;
    TranslateScale operator*(const TranslateScale& other) const;
    TranslateScale& operator*=(const TranslateScale& other);
    TranslateScale operator+(const Vec2& v) const;
    TranslateScale& operator+=(const Vec2& v);
    TranslateScale operator-(const Vec2& v) const;
    TranslateScale& operator-=(const Vec2& v);

    // Conversion to Affine
    Affine to_affine() const;

    // Default
    static TranslateScale identity();
};

// Free operators
TranslateScale operator*(double scalar, const TranslateScale& ts);
TranslateScale operator+(const Vec2& v, const TranslateScale& ts);

} // namespace kurbo 