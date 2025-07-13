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
class CubicBez : public ParamCurve, public ParamCurveDeriv, public ParamCurveArclen, 
                public ParamCurveArea, public ParamCurveNearest, public ParamCurveExtrema {
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

    // ParamCurve implementation
    Point eval(double t) const override;
    ParamCurve* subsegment(double start, double end) const override;
    std::pair<ParamCurve*, ParamCurve*> subdivide() const override;
    Point start() const override;
    Point end() const override;

    // ParamCurveDeriv implementation
    ParamCurveDeriv* deriv() const override;

    // ParamCurveArclen implementation
    double arclen(double accuracy = 1e-9) const override;

    // ParamCurveArea implementation
    double signed_area() const override;

    // ParamCurveNearest implementation
    Nearest nearest(const Point& p, double accuracy) const override;

    // ParamCurveExtrema implementation
    std::vector<double> extrema() const override;

    // Concrete subdivision methods
    std::pair<CubicBez, CubicBez> subdivide_concrete() const;
    std::tuple<CubicBez, CubicBez, CubicBez> subdivide_3() const;

    // Utility methods
    bool is_finite() const;
    bool is_nan() const;
    std::vector<double> inflections() const;
    std::vector<double> tangents_to_point(const Point& p) const;
    int winding(const Point& pt) const;
    Rect bounding_box() const;

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

bool operator==(const CubicBez& a, const CubicBez& b);

} // namespace kurbo 