#pragma once

// Triangle class (2D triangle)
// Ported from kurbo::Triangle (Rust)
// Copyright 2024
//
// This class represents a 2D triangle with three vertices.

#include "point.hpp"
#include "vec2.hpp"
#include "rect.hpp"
#include "circle.hpp"
#include <optional>
#include <cmath>

namespace kurbo {

class Triangle {
public:
    // Vertex a
    Point a;
    // Vertex b
    Point b;
    // Vertex c
    Point c;

    // Constructors
    constexpr Triangle() : a(), b(), c() {}
    constexpr Triangle(const Point& a, const Point& b, const Point& c) : a(a), b(b), c(c) {}

    // Static constructors
    static constexpr Triangle new_triangle(const Point& a, const Point& b, const Point& c) { return Triangle(a, b, c); }
    static constexpr Triangle from_coords(double ax, double ay, double bx, double by, double cx, double cy) { return Triangle(Point(ax, ay), Point(bx, by), Point(cx, cy)); }

    // Static constants
    static const Triangle ZERO;
    static const Triangle EQUILATERAL;

    // Basic operations
    Point centroid() const;
    std::array<Vec2, 3> offsets() const;
    double area() const;
    bool is_zero_area() const;

    // Circle operations
    Circle inscribed_circle() const;
    Circle circumscribed_circle() const;

    // Transformations
    Triangle inflate(double scalar) const;

    // Utility methods
    bool is_finite() const;
    bool is_nan() const;

    // Shape methods
    double perimeter(double accuracy) const;
    int winding(const Point& pt) const;
    Rect bounding_box() const;
    std::optional<Triangle> as_triangle() const;

    // Operators
    Triangle operator+(const Vec2& v) const;
    Triangle& operator+=(const Vec2& v);
    Triangle operator-(const Vec2& v) const;
    Triangle& operator-=(const Vec2& v);

    // Default
    static Triangle zero();
};

} // namespace kurbo 