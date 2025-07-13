#pragma once

// RoundedRect class (2D rectangle with rounded corners)
// Ported from kurbo::RoundedRect (Rust)
// Copyright 2024
//
// This class represents a 2D rectangle with rounded corners.
// It contains a Rect and RoundedRectRadii for the corner radii.

#include "rect.hpp"
#include "rounded_rect_radii.hpp"
#include "point.hpp"
#include "size.hpp"
#include "vec2.hpp"
#include <cmath>

namespace kurbo {

class RoundedRect {
public:
    // Coordinates of the rectangle
    Rect rect;
    // Radius of all four corners
    RoundedRectRadii radii;

    // Constructors
    RoundedRect();
    RoundedRect(const Rect& rect, const RoundedRectRadii& radii);

    // Static constructors
    static RoundedRect new_rounded_rect(double x0, double y0, double x1, double y1, double radius);
    static RoundedRect new_rounded_rect(double x0, double y0, double x1, double y1, const RoundedRectRadii& radii);
    static RoundedRect from_rect(const Rect& rect, const RoundedRectRadii& radii);
    static RoundedRect from_rect(const Rect& rect, double radius);
    static RoundedRect from_points(const Point& p0, const Point& p1, const RoundedRectRadii& radii);
    static RoundedRect from_points(const Point& p0, const Point& p1, double radius);
    static RoundedRect from_origin_size(const Point& origin, const Size& size, const RoundedRectRadii& radii);
    static RoundedRect from_origin_size(const Point& origin, const Size& size, double radius);

    // Properties
    double width() const;
    double height() const;
    RoundedRectRadii get_radii() const;
    Rect get_rect() const;
    Point origin() const;
    Point center() const;

    // Utility
    bool is_finite() const;
    bool is_nan() const;

    // Operators
    RoundedRect operator+(const Vec2& v) const;
    RoundedRect& operator+=(const Vec2& v);
    RoundedRect operator-(const Vec2& v) const;
    RoundedRect& operator-=(const Vec2& v);

    // Default
    static RoundedRect zero();
};

// Stream operator
std::ostream& operator<<(std::ostream& os, const RoundedRect& rr);

} // namespace kurbo 