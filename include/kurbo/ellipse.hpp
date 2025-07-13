#pragma once

// Ellipse class (2D ellipse)
// Ported from kurbo::Ellipse (Rust)
// Copyright 2024
//
// This class represents a 2D ellipse with center, radii, and rotation.

#include "point.hpp"
#include "vec2.hpp"
#include "rect.hpp"
#include "affine.hpp"
#include "circle.hpp"
#include <optional>
#include <cmath>

namespace kurbo {

class Ellipse {
public:
    // All ellipses can be represented as an affine map of the unit circle
    // centered at (0, 0). Therefore we can store the ellipse as an affine map.
    Affine inner;

    // Constructors
    Ellipse();
    Ellipse(const Point& center, const Vec2& radii, double rotation);
    constexpr Ellipse(const Affine& inner);

    // Static constructors
    static Ellipse new_ellipse(const Point& center, const Vec2& radii, double x_rotation);
    static Ellipse from_rect(const Rect& rect);
    static constexpr Ellipse from_affine(const Affine& affine);
    static Ellipse from_circle(const Circle& circle);

    // Modifiers
    Ellipse with_center(const Point& new_center) const;
    Ellipse with_radii(const Vec2& new_radii) const;
    Ellipse with_rotation(double rotation) const;

    // Getters
    Point center() const;
    Vec2 radii() const;
    double rotation() const;
    std::pair<Vec2, double> radii_and_rotation() const;

    // Utility methods
    bool is_finite() const;
    bool is_nan() const;

    // Shape methods
    double area() const;
    double perimeter(double accuracy) const;
    int winding(const Point& pt) const;
    Rect bounding_box() const;
    std::optional<Ellipse> as_ellipse() const;

    // Operators
    Ellipse operator+(const Vec2& v) const;
    Ellipse& operator+=(const Vec2& v);
    Ellipse operator-(const Vec2& v) const;
    Ellipse& operator-=(const Vec2& v);
    Ellipse operator*(const Affine& affine) const;
    Ellipse& operator*=(const Affine& affine);
    friend Ellipse operator*(const Affine& affine, const Ellipse& ellipse);

    // Default
    static Ellipse zero();

private:
    // Internal constructor
    static Ellipse private_new(const Vec2& center, double scale_x, double scale_y, double x_rotation);
};

} // namespace kurbo 