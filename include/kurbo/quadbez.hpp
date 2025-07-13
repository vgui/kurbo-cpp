#pragma once

#include <vector>
#include "point.hpp"
#include "line.hpp"
#include "param_curve.hpp"
#include "affine.hpp"
#include "rect.hpp"
#include "path_el.hpp"

namespace kurbo {

// Forward declaration
class CubicBez;

/// Internal structure for flattening parameters.
struct FlattenParams {
    double a0;
    double a2;
    double u0;
    double uscale;
    /// The number of `subdivisions * 2 * sqrt_tol`.
    double val;
};

/// An approximation to ∫ (1 + 4x²)^(-0.25) dx
///
/// This is used for flattening curves.
double approx_parabola_integral(double x);

/// An approximation to the inverse parabola integral.
double approx_parabola_inv_integral(double x);

/// A single quadratic Bézier segment.
class QuadBez : public ParamCurve, public ParamCurveDeriv, public ParamCurveArclen, 
                public ParamCurveArea, public ParamCurveNearest, public ParamCurveExtrema {
public:
    /// The start point.
    Point p0;
    /// The control point.
    Point p1;
    /// The end point.
    Point p2;

    /// Create a new quadratic Bézier segment.
    QuadBez(const Point& p0, const Point& p1, const Point& p2);

    /// Create a new quadratic Bézier segment from any type convertible to Point.
    template<typename T>
    QuadBez(const T& p0, const T& p1, const T& p2) 
        : p0(Point(p0)), p1(Point(p1)), p2(Point(p2)) {}

    /// Raise the order by 1.
    ///
    /// Returns a cubic Bézier segment that exactly represents this quadratic.
    CubicBez raise() const;

    /// Is this quadratic Bezier curve finite?
    bool is_finite() const;

    /// Is this quadratic Bezier curve NaN?
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
    QuadBez transform(const Affine& affine) const;
    bool is_linear() const;
};

/// An iterator for quadratic beziers.
class QuadBezIter {
public:
    QuadBezIter(const QuadBez& quad);
    
    /// Get the next path element.
    /// Returns nullptr when done.
    PathEl* next();

private:
    QuadBez quad;
    size_t ix;
};

// Operators
QuadBez operator*(const Affine& affine, const QuadBez& quad);

// Stream operator
std::ostream& operator<<(std::ostream& os, const QuadBez& quad);

} // namespace kurbo 