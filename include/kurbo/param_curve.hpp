#pragma once

#include <array>
#include <vector>
#include "point.hpp"
#include "rect.hpp"
#include "vec2.hpp"

namespace kurbo {

/// A default value for methods that take an 'accuracy' argument.
///
/// This value is intended to be suitable for general-purpose use, such as
/// 2d graphics.
constexpr double DEFAULT_ACCURACY = 1e-6;

/// The maximum number of extrema that can be reported in the ParamCurveExtrema trait.
///
/// This is 4 to support cubic Béziers. If other curves are used, they should be
/// subdivided to limit the number of extrema.
constexpr size_t MAX_EXTREMA = 4;

/// The nearest position on a curve to some point.
///
/// This is returned by ParamCurveNearest::nearest
struct Nearest {
    /// The square of the distance from the nearest position on the curve
    /// to the given point.
    double distance_sq;
    /// The position on the curve of the nearest point, as a parameter.
    ///
    /// To resolve this to a Point, use ParamCurve::eval.
    double t;
};

/// A curve parameterized by a scalar.
///
/// If the result is interpreted as a point, this represents a curve.
/// But the result can be interpreted as a vector as well.
class ParamCurve {
public:
    virtual ~ParamCurve() = default;

    /// Evaluate the curve at parameter t.
    ///
    /// Generally t is in the range [0..1].
    virtual Point eval(double t) const = 0;

    /// Get a subsegment of the curve for the given parameter range.
    virtual ParamCurve* subsegment(double start, double end) const = 0;

    /// Subdivide into (roughly) halves.
    virtual std::pair<ParamCurve*, ParamCurve*> subdivide() const {
        auto first = subsegment(0.0, 0.5);
        auto second = subsegment(0.5, 1.0);
        return {first, second};
    }

    /// The start point.
    virtual Point start() const {
        return eval(0.0);
    }

    /// The end point.
    virtual Point end() const {
        return eval(1.0);
    }
};

/// A differentiable parameterized curve.
class ParamCurveDeriv {
public:
    virtual ~ParamCurveDeriv() = default;

    /// The derivative of the curve.
    ///
    /// Note that the type of the return value is somewhat inaccurate, as
    /// the derivative of a curve (mapping of param to point) is a mapping
    /// of param to vector. We choose to accept this rather than have a
    /// more complex type scheme.
    virtual ParamCurveDeriv* deriv() const = 0;

    /// Estimate arclength using Gaussian quadrature.
    ///
    /// The coefficients are assumed to cover the range (-1..1), which is
    /// traditional.
    virtual double gauss_arclen(const std::vector<std::pair<double, double>>& coeffs) const;
};

/// A parameterized curve that can have its arc length measured.
class ParamCurveArclen {
public:
    virtual ~ParamCurveArclen() = default;

    /// The arc length of the curve.
    ///
    /// The result is accurate to the given accuracy (subject to
    /// roundoff errors for ridiculously low values). Compute time
    /// may vary with accuracy, if the curve needs to be subdivided.
    virtual double arclen(double accuracy) const = 0;

    /// Solve for the parameter that has the given arc length from the start.
    ///
    /// This implementation uses the IPT method, as provided by
    /// common::solve_itp. This is as robust as bisection but
    /// typically converges faster. In addition, the method takes
    /// care to compute arc lengths of increasingly smaller segments
    /// of the curve, as that is likely faster than repeatedly
    /// computing the arc length of the segment starting at t=0.
    virtual double inv_arclen(double arclen, double accuracy) const;
};

/// A parameterized curve that can have its signed area measured.
class ParamCurveArea {
public:
    virtual ~ParamCurveArea() = default;

    /// Compute the signed area under the curve.
    ///
    /// For a closed path, the signed area of the path is the sum of signed
    /// areas of the segments. This is a variant of the "shoelace formula."
    /// See:
    /// <https://github.com/Pomax/bezierinfo/issues/44> and
    /// <http://ich.deanmcnamee.com/graphics/2016/03/30/CurveArea.html>
    ///
    /// This can be computed exactly for Béziers thanks to Green's theorem,
    /// and also for simple curves such as circular arcs. For more exotic
    /// curves, it's probably best to subdivide to cubics. We leave that
    /// to the caller, which is why we don't give an accuracy param here.
    virtual double signed_area() const = 0;
};

/// A parameterized curve that reports the nearest point.
class ParamCurveNearest {
public:
    virtual ~ParamCurveNearest() = default;

    /// Find the position on the curve that is nearest to the given point.
    ///
    /// This returns a Nearest struct that contains information about
    /// the position.
    virtual Nearest nearest(const Point& p, double accuracy) const = 0;
};

/// A parameterized curve that reports its curvature.
class ParamCurveCurvature {
public:
    virtual ~ParamCurveCurvature() = default;

    /// Compute the signed curvature at parameter t.
    virtual double curvature(double t) const;
};

/// A parameterized curve that reports its extrema.
class ParamCurveExtrema {
public:
    virtual ~ParamCurveExtrema() = default;

    /// Compute the extrema of the curve.
    ///
    /// Only extrema within the interior of the curve count.
    /// At most four extrema can be reported, which is sufficient for
    /// cubic Béziers.
    ///
    /// The extrema should be reported in increasing parameter order.
    virtual std::vector<double> extrema() const = 0;

    /// Return parameter ranges, each of which is monotonic within the range.
    virtual std::vector<std::pair<double, double>> extrema_ranges() const;

    /// The smallest rectangle that encloses the curve in the range (0..1).
    virtual Rect bounding_box() const;
};

} // namespace kurbo 