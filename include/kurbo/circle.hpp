#pragma once

// Circle class (2D circle)
// Ported from kurbo::Circle (Rust)
// Copyright 2024
//
// This class represents a 2D circle with center and radius.

#include "point.hpp"
#include "vec2.hpp"
#include "rect.hpp"
#include "affine.hpp"
#include "shape.hpp"
#include <optional>
#include <cmath>

namespace kurbo {

class Ellipse; // Forward declaration

class Circle : public Shape {
public:
    // The center
    Point center;
    // The radius
    double radius;

    // Constructors
    constexpr Circle() : center(), radius(0.0) {}
    constexpr Circle(const Point& center, double radius) : center(center), radius(radius) {}

    // Static constructors
    static Circle new_circle(const Point& center, double radius) { return Circle(center, radius); }

    // Basic operations
    // CircleSegment segment(double inner_radius, double start_angle, double sweep_angle) const; // TODO: implement when CircleSegment is ready

    // Utility methods
    bool is_finite() const;
    bool is_nan() const;

    // Default
    static Circle zero();

    // Shape implementation
    std::vector<PathEl> path_elements(double tolerance) const override;
    double area() const override;
    double perimeter(double accuracy) const override;
    int winding(const Point& pt) const override;
    Rect bounding_box() const override;
    std::optional<Circle> as_circle() const override;

    // Operators
    Circle operator+(const Vec2& v) const;
    Circle& operator+=(const Vec2& v);
    Circle operator-(const Vec2& v) const;
    Circle& operator-=(const Vec2& v);
};

// A segment of a circle
class CircleSegment {
public:
    // The center
    Point center;
    // The outer radius
    double outer_radius;
    // The inner radius
    double inner_radius;
    // The angle to start drawing the segment (in radians)
    double start_angle;
    // The arc length of the segment (in radians)
    double sweep_angle;

    // Constructors
    constexpr CircleSegment() : center(), outer_radius(0.0), inner_radius(0.0), start_angle(0.0), sweep_angle(0.0) {}
    constexpr CircleSegment(const Point& center, double outer_radius, double inner_radius, double start_angle, double sweep_angle) : center(center), outer_radius(outer_radius), inner_radius(inner_radius), start_angle(start_angle), sweep_angle(sweep_angle) {}

    // Static constructors
    static constexpr CircleSegment new_segment(const Point& center, double outer_radius, double inner_radius, double start_angle, double sweep_angle) { return CircleSegment(center, outer_radius, inner_radius, start_angle, sweep_angle); }

    // Basic operations
    // Arc outer_arc() const; // TODO: implement when Arc is ready
    // Arc inner_arc() const; // TODO: implement when Arc is ready

    // Utility methods
    bool is_finite() const;
    bool is_nan() const;

    // Default
    static CircleSegment zero();

    // Shape-like methods
    double area() const;
    double perimeter(double accuracy) const;
    int winding(const Point& pt) const;
    Rect bounding_box() const;

    // Operators
    CircleSegment operator+(const Vec2& v) const;
    CircleSegment& operator+=(const Vec2& v);
    CircleSegment operator-(const Vec2& v) const;
    CircleSegment& operator-=(const Vec2& v);
};

bool operator==(const Circle& a, const Circle& b);

// Affine transformation (returns Ellipse since affine transform of circle is ellipse)
Ellipse operator*(const Affine& affine, const Circle& circle);

} // namespace kurbo 