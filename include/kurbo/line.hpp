#pragma once

// Line class (2D line segment)
// Ported from kurbo::Line (Rust)
// Copyright 2024
//
// This class represents a 2D line segment with start and end points.

#include "point.hpp"
#include "vec2.hpp"
#include "rect.hpp"
#include "affine.hpp"
#include <optional>
#include <cmath>

namespace kurbo {

class Line {
public:
    // The line's start point
    Point p0;
    // The line's end point
    Point p1;

    // Constructors
    constexpr Line() : p0(), p1() {}
    constexpr Line(const Point& p0, const Point& p1) : p0(p0), p1(p1) {}

    // Static constructors
    static constexpr Line new_line(const Point& p0, const Point& p1) { return Line(p0, p1); }

    // Basic operations
    Line reversed() const;
    double length() const;
    Point midpoint() const;
    std::optional<Point> crossing_point(const Line& other) const;

    // Utility methods
    bool is_finite() const;
    bool is_nan() const;

    // Parametric curve methods
    Point eval(double t) const;
    Line subsegment(double start_t, double end_t) const;
    Point start() const;
    Point end() const;
    double arclen(double accuracy) const;
    double inv_arclen(double arclen, double accuracy) const;
    double signed_area() const;
    double curvature(double t) const;

    // Operators
    Line operator+(const Vec2& v) const;
    Line& operator+=(const Vec2& v);
    Line operator-(const Vec2& v) const;
    Line& operator-=(const Vec2& v);
    Line operator*(const Affine& affine) const;
    Line& operator*=(const Affine& affine);

    // Default
    static Line zero();
};

// A trivial "curve" that is just a constant point
class ConstPoint {
public:
    Point point;

    constexpr ConstPoint(const Point& point) : point(point) {}
    constexpr ConstPoint(double x, double y) : point(x, y) {}

    // Utility methods
    bool is_finite() const;
    bool is_nan() const;

    // Parametric curve methods
    Point eval(double t) const;
    ConstPoint subsegment(double start_t, double end_t) const;
    Point start() const;
    Point end() const;
    double arclen(double accuracy) const;
    double inv_arclen(double arclen, double accuracy) const;
    double signed_area() const;
    double curvature(double t) const;

    // Default
    static ConstPoint zero();
};

} // namespace kurbo 