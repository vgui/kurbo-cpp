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
#include "param_curve.hpp"
#include "shape.hpp"
#include <optional>
#include <cmath>

namespace kurbo {

class Line : public ParamCurve, public ParamCurveDeriv, public ParamCurveArclen, 
            public ParamCurveArea, public ParamCurveNearest, public ParamCurveExtrema, public Shape {
public:
    // The line's start point
    Point p0;
    // The line's end point
    Point p1;

    // Constructors
    constexpr Line() : p0(), p1() {}
    constexpr Line(const Point& p0, const Point& p1) : p0(p0), p1(p1) {}

    // Static constructors
    static Line new_line(const Point& p0, const Point& p1) { return Line(p0, p1); }

    // Basic operations
    Line reversed() const;
    double length() const;
    Point midpoint() const;
    std::optional<Point> crossing_point(const Line& other) const;

    // Utility methods
    bool is_finite() const;
    bool is_nan() const;

    // ParamCurve implementation
    Point eval(double t) const override;
    ParamCurve* subsegment(double start, double end) const override;
    std::pair<ParamCurve*, ParamCurve*> subdivide() const override;
    Point start() const override;
    Point end() const override;

    // ParamCurveDeriv implementation
    ParamCurveDeriv* deriv() const override;

    // ParamCurveArclen implementation
    double arclen(double accuracy) const override;

    // ParamCurveArea implementation
    double signed_area() const override;

    // ParamCurveNearest implementation
    Nearest nearest(const Point& p, double accuracy) const override;

    // ParamCurveExtrema implementation
    std::vector<double> extrema() const override;

    // Transformations
    Line transform(const Affine& affine) const;
    bool is_linear() const;

    // Operators
    Line operator+(const Vec2& v) const;
    Line& operator+=(const Vec2& v);
    Line operator-(const Vec2& v) const;
    Line& operator-=(const Vec2& v);
    Line operator*(const Affine& affine) const;
    Line& operator*=(const Affine& affine);

    // Default
    static Line zero();

    // Shape implementation
    std::vector<PathEl> path_elements(double tolerance) const override;
    double area() const override;
    double perimeter(double accuracy) const override;
    int winding(const Point& pt) const override;
    Rect bounding_box() const override;
    std::optional<Line> as_line() const override;
    bool contains(const Point& pt) const override;
};

// Stream operator
std::ostream& operator<<(std::ostream& os, const Line& line);

bool operator==(const Line& a, const Line& b);

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