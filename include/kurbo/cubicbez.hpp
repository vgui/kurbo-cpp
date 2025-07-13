#pragma once

// CubicBez class (cubic Bézier curves)
// Ported from kurbo::CubicBez (Rust)
// Copyright 2024
//
// This class represents a cubic Bézier curve segment with four control points.

#include "point.hpp"
#include "vec2.hpp"
#include "quadbez.hpp"
#include "param_curve.hpp"
#include "affine.hpp"
#include <vector>
#include <cmath>

namespace kurbo {

class QuadSpline;

/// A single cubic Bézier segment.
class CubicBez {
public:
    // Control points
    Point p0;
    Point p1;
    Point p2;
    Point p3;

    // Constructors
    CubicBez();
    CubicBez(const Point& p0, const Point& p1, const Point& p2, const Point& p3);

    // Static constructors
    static CubicBez new_curve(const Point& p0, const Point& p1, const Point& p2, const Point& p3);

    // Conversion to quadratic Béziers
    std::vector<std::pair<double, double>> to_quads(double accuracy) const;

    // Approximation (temporarily disabled - requires QuadSpline implementation)
    // std::optional<QuadSpline> approx_spline(double accuracy) const;

    // Subdivision
    std::pair<CubicBez, CubicBez> subdivide() const;
    std::tuple<CubicBez, CubicBez, CubicBez> subdivide_3() const;

    // Utility methods
    bool is_finite() const;
    bool is_nan() const;
    std::vector<double> inflections() const;
    std::vector<double> tangents_to_point(const Point& p) const;

    // Operators
    CubicBez operator+(const Vec2& v) const;
    CubicBez& operator+=(const Vec2& v);
    CubicBez operator-(const Vec2& v) const;
    CubicBez& operator-=(const Vec2& v);

    // Default
    static CubicBez zero();
};

// Free operators
CubicBez operator*(const Affine& affine, const CubicBez& cubic);

// Stream operator
std::ostream& operator<<(std::ostream& os, const CubicBez& cubic);

} // namespace kurbo 