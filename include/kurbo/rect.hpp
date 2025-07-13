#pragma once

// Rect class (2D rectangle)
// Ported from kurbo::Rect (Rust)
// Copyright 2024
//
// This class represents a 2D rectangle with coordinates (x0, y0, x1, y1)
// where (x0, y0) is the minimum point and (x1, y1) is the maximum point.

#include "point.hpp"
#include "size.hpp"
#include "vec2.hpp"
#include <cmath>

namespace kurbo {

class RoundedRect;
class RoundedRectRadii;

class Rect {
public:
    // The minimum x coordinate (left edge)
    double x0;
    // The minimum y coordinate (top edge in y-down spaces)
    double y0;
    // The maximum x coordinate (right edge)
    double x1;
    // The maximum y coordinate (bottom edge in y-down spaces)
    double y1;

    // Static constants
    static const Rect ZERO;

    // Constructors
    constexpr Rect() : x0(0.0), y0(0.0), x1(0.0), y1(0.0) {}
    constexpr Rect(double x0, double y0, double x1, double y1) : x0(x0), y0(y0), x1(x1), y1(y1) {}

    // Static constructors
    static constexpr Rect new_rect(double x0, double y0, double x1, double y1) {
        return Rect(x0, y0, x1, y1);
    }
    static Rect from_points(const Point& p0, const Point& p1);
    static Rect from_origin_size(const Point& origin, const Size& size);
    static Rect from_center_size(const Point& center, const Size& size);

    // Modifiers
    Rect with_origin(const Point& origin) const;
    Rect with_size(const Size& size) const;
    Rect inset(double insets) const;

    // Properties
    double width() const;
    double height() const;
    double min_x() const;
    double max_x() const;
    double min_y() const;
    double max_y() const;
    Point origin() const;
    Size size() const;
    double area() const;
    bool is_zero_area() const;
    Point center() const;

    // Operations
    bool contains(const Point& point) const;
    Rect abs() const;
    Rect union_rect(const Rect& other) const;
    Rect union_pt(const Point& pt) const;
    Rect intersect(const Rect& other) const;
    bool overlaps(const Rect& other) const;
    bool contains_rect(const Rect& other) const;
    Rect inflate(double width, double height) const;

    // Rounding
    Rect round() const;
    Rect ceil() const;
    Rect floor() const;
    Rect expand() const;
    Rect trunc() const;

    // Transformations
    Rect scale_from_origin(double factor) const;
    RoundedRect to_rounded_rect(const RoundedRectRadii& radii) const;
    RoundedRect to_rounded_rect(double radius) const;
    // Ellipse to_ellipse() const; // TODO: implement when Ellipse is ready

    // Utility
    double aspect_ratio() const;
    Rect contained_rect_with_aspect_ratio(double aspect_ratio) const;
    bool is_finite() const;
    bool is_nan() const;

    // Operators
    Rect operator+(const Vec2& v) const;
    Rect& operator+=(const Vec2& v);
    Rect operator-(const Vec2& v) const;
    Rect& operator-=(const Vec2& v);
    // Insets operator-(const Rect& other) const; // TODO: implement when Insets is ready

    // Default
    static Rect zero();
};

} // namespace kurbo 