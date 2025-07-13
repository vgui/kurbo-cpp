#include "kurbo/quadbez.hpp"
#include "kurbo/common.hpp"
#include "kurbo/inline_methods.hpp"
#include <cmath>
#include <algorithm>

namespace kurbo {

// Forward declaration - will be implemented later
class CubicBez {
public:
    CubicBez(const Point& p0, const Point& p1, const Point& p2, const Point& p3) 
        : p0(p0), p1(p1), p2(p2), p3(p3) {}
    Point p0, p1, p2, p3;
};

QuadBez::QuadBez(const Point& p0, const Point& p1, const Point& p2) 
    : p0(p0), p1(p1), p2(p2) {}

CubicBez QuadBez::raise() const {
    return CubicBez(
        p0,
        p0 + (2.0 / 3.0) * (p1 - p0),
        p2 + (2.0 / 3.0) * (p1 - p2),
        p2
    );
}

bool QuadBez::is_finite() const {
    return p0.is_finite() && p1.is_finite() && p2.is_finite();
}

bool QuadBez::is_nan() const {
    return p0.is_nan() || p1.is_nan() || p2.is_nan();
}

Point QuadBez::eval(double t) const {
    double mt = 1.0 - t;
    Vec2 result = p0.to_vec2() * (mt * mt) + 
                  (p1.to_vec2() * (mt * 2.0) + p2.to_vec2() * t) * t;
    return result.to_point();
}

ParamCurve* QuadBez::subsegment(double start, double end) const {
    Point p0 = eval(start);
    Point p2 = eval(end);
    Point p1 = p0 + (p1 - p0).lerp(p2 - p1, start) * (end - start);
    return new QuadBez(p0, p1, p2);
}

std::pair<ParamCurve*, ParamCurve*> QuadBez::subdivide() const {
    Point pm = eval(0.5);
    auto first = new QuadBez(p0, p0.midpoint(p1), pm);
    auto second = new QuadBez(pm, p1.midpoint(p2), p2);
    return {first, second};
}

Point QuadBez::start() const {
    return p0;
}

Point QuadBez::end() const {
    return p2;
}

ParamCurveDeriv* QuadBez::deriv() const {
    return new Line(
        (2.0 * (p1 - p0)).to_point(),
        (2.0 * (p2 - p1)).to_point()
    );
}

double QuadBez::arclen(double accuracy) const {
    Vec2 d2 = p0.to_vec2() - 2.0 * p1.to_vec2() + p2.to_vec2();
    double a = d2.hypot2();
    Vec2 d1 = p1 - p0;
    double c = d1.hypot2();
    
    if (a < 5e-4 * c) {
        // This case happens for nearly straight Béziers.
        //
        // Calculate arclength using Legendre-Gauss quadrature using formula from Behdad
        // in https://github.com/Pomax/BezierInfo-2/issues/77
        double v0 = (-0.492943519233745 * p0.to_vec2()
            + 0.430331482911935 * p1.to_vec2()
            + 0.0626120363218102 * p2.to_vec2()).hypot();
        double v1 = ((p2 - p0) * 0.4444444444444444).hypot();
        double v2 = (-0.0626120363218102 * p0.to_vec2()
            - 0.430331482911935 * p1.to_vec2()
            + 0.492943519233745 * p2.to_vec2()).hypot();
        return v0 + v1 + v2;
    }
    
    double b = 2.0 * d2.dot(d1);
    double sabc = std::sqrt(a + b + c);
    double a2 = std::pow(a, -0.5);
    double a32 = std::pow(a2, 3);
    double c2 = 2.0 * std::sqrt(c);
    double ba_c2 = b * a2 + c2;
    
    double v0 = 0.25 * a2 * a2 * b * (2.0 * sabc - c2) + sabc;
    
    // TODO: justify and fine-tune this exact constant.
    if (ba_c2 < 1e-13) {
        // This case happens for Béziers with a sharp kink.
        return v0;
    } else {
        return v0 + 0.25 * a32 * (4.0 * c * a - b * b) * 
               std::log(((2.0 * a + b) * a2 + 2.0 * sabc) / ba_c2);
    }
}

double QuadBez::signed_area() const {
    return (p0.x * (2.0 * p1.y + p2.y) + 2.0 * p1.x * (p2.y - p0.y)
            - p2.x * (p0.y + 2.0 * p1.y)) * (1.0 / 6.0);
}

Nearest QuadBez::nearest(const Point& p, double accuracy) const {
    double r_best = -1.0;
    double t_best = 0.0;
    
    auto eval_t = [&](double t, const Point& p0) {
        double r = p0.distance_squared(p);
        if (r_best < 0 || r < r_best) {
            r_best = r;
            t_best = t;
        }
    };
    
    auto try_t = [&](double t) -> bool {
        if (t < 0.0 || t > 1.0) {
            return true;
        }
        eval_t(t, eval(t));
        return false;
    };
    
    Vec2 d0 = p1 - p0;
    Vec2 d1 = p0.to_vec2() + p2.to_vec2() - 2.0 * p1.to_vec2();
    Vec2 d = p0 - p;
    double c0 = d.dot(d0);
    double c1 = 2.0 * d0.hypot2() + d.dot(d1);
    double c2 = 3.0 * d1.dot(d0);
    double c3 = d1.hypot2();
    
    std::vector<double> roots = solve_cubic(c0, c1, c2, c3);
    
    bool need_ends = false;
    
    if (roots.empty()) {
        need_ends = true;
    }
    
    for (double t : roots) {
        need_ends |= try_t(t);
    }
    
    if (need_ends) {
        eval_t(0.0, p0);
        eval_t(1.0, p2);
    }
    
    return {r_best, t_best};
}

std::vector<double> QuadBez::extrema() const {
    std::vector<double> result;
    Vec2 d0 = p1 - p0;
    Vec2 d1 = p2 - p1;
    Vec2 dd = d1 - d0;
    
    if (dd.x != 0.0) {
        double t = -d0.x / dd.x;
        if (t > 0.0 && t < 1.0) {
            result.push_back(t);
        }
    }
    
    if (dd.y != 0.0) {
        double t = -d0.y / dd.y;
        if (t > 0.0 && t < 1.0) {
            result.push_back(t);
            if (result.size() == 2 && result[0] > t) {
                std::swap(result[0], result[1]);
            }
        }
    }
    
    return result;
}

QuadBezIter::QuadBezIter(const QuadBez& quad) : quad(quad), ix(0) {}

PathEl* QuadBezIter::next() {
    ix++;
    switch (ix) {
        case 1:
            return new PathEl(PathElType::MoveTo, quad.p0);
        case 2:
            return new PathEl(PathElType::QuadTo, quad.p1, quad.p2);
        default:
            return nullptr;
    }
}

double approx_parabola_integral(double x) {
    const double D = 0.67;
    return x / (1.0 - D + std::sqrt(std::sqrt(std::pow(D, 4) + 0.25 * x * x)));
}

double approx_parabola_inv_integral(double x) {
    const double B = 0.39;
    return x * (1.0 - B + std::sqrt(B * B + 0.25 * x * x));
}

QuadBez operator*(const Affine& affine, const QuadBez& quad) {
    return QuadBez(
        affine * quad.p0,
        affine * quad.p1,
        affine * quad.p2
    );
}

QuadBez QuadBez::transform(const Affine& affine) const {
    return QuadBez(affine * p0, affine * p1, affine * p2);
}

bool QuadBez::is_linear() const {
    // A quadratic Bezier is linear if the control point lies on the line
    // between the start and end points
    Vec2 start_to_end = p2 - p0;
    Vec2 start_to_control = p1 - p0;
    
    if (start_to_end.hypot2() == 0.0) {
        return true; // Start and end are the same point
    }
    
    // Check if control point lies on the line segment
    double t = start_to_control.dot(start_to_end) / start_to_end.hypot2();
    if (t < 0.0 || t > 1.0) {
        return false;
    }
    
    Point expected_control = p0.lerp(p2, t);
    return p1.distance_squared(expected_control) < 1e-10;
}

std::ostream& operator<<(std::ostream& os, const QuadBez& quad) {
    os << "QuadBez(" << quad.p0 << ", " << quad.p1 << ", " << quad.p2 << ")";
    return os;
}

} // namespace kurbo 