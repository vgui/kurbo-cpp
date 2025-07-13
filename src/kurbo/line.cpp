#include "kurbo/line.hpp"
#include "kurbo/inline_methods.hpp"
#include <cmath>

namespace kurbo {

// Constructors are now inline in header

Line Line::reversed() const {
    return Line(p1, p0);
}

double Line::length() const {
    return arclen(1e-9); // DEFAULT_ACCURACY
}

Point Line::midpoint() const {
    return p0.midpoint(p1);
}

std::optional<Point> Line::crossing_point(const Line& other) const {
    Vec2 ab = p1 - p0;
    Vec2 cd = other.p1 - other.p0;
    double pcd = ab.cross(cd);
    if (pcd == 0.0) {
        return std::nullopt;
    }
    double h = ab.cross(p0 - other.p0) / pcd;
    return other.p0 + cd * h;
}

bool Line::is_finite() const {
    return p0.is_finite() && p1.is_finite();
}

bool Line::is_nan() const {
    return p0.is_nan() || p1.is_nan();
}

Point Line::eval(double t) const {
    return p0.lerp(p1, t);
}

Line Line::subsegment(double start_t, double end_t) const {
    return Line(eval(start_t), eval(end_t));
}

Point Line::start() const {
    return p0;
}

Point Line::end() const {
    return p1;
}

double Line::arclen(double accuracy) const {
    return (p1 - p0).hypot();
}

double Line::inv_arclen(double arclen, double accuracy) const {
    return arclen / (p1 - p0).hypot();
}

double Line::signed_area() const {
    return p0.to_vec2().cross(p1.to_vec2()) * 0.5;
}

double Line::curvature(double t) const {
    return 0.0; // Lines have zero curvature
}

Line Line::operator+(const Vec2& v) const {
    return Line(p0 + v, p1 + v);
}

Line& Line::operator+=(const Vec2& v) {
    p0 += v;
    p1 += v;
    return *this;
}

Line Line::operator-(const Vec2& v) const {
    return Line(p0 - v, p1 - v);
}

Line& Line::operator-=(const Vec2& v) {
    p0 -= v;
    p1 -= v;
    return *this;
}

Line Line::operator*(const Affine& affine) const {
    return Line(affine * p0, affine * p1);
}

Line& Line::operator*=(const Affine& affine) {
    p0 = affine * p0;
    p1 = affine * p1;
    return *this;
}

Line Line::zero() {
    return Line(Point::zero(), Point::zero());
}

// ConstPoint implementation
// Constructor is now inline in header

bool ConstPoint::is_finite() const {
    return point.is_finite();
}

bool ConstPoint::is_nan() const {
    return point.is_nan();
}

Point ConstPoint::eval(double t) const {
    return point;
}

ConstPoint ConstPoint::subsegment(double start_t, double end_t) const {
    return *this;
}

Point ConstPoint::start() const {
    return point;
}

Point ConstPoint::end() const {
    return point;
}

double ConstPoint::arclen(double accuracy) const {
    return 0.0;
}

double ConstPoint::inv_arclen(double arclen, double accuracy) const {
    return 0.0;
}

double ConstPoint::signed_area() const {
    return 0.0;
}

double ConstPoint::curvature(double t) const {
    return 0.0;
}

ConstPoint ConstPoint::zero() {
    return ConstPoint(Point::zero());
}

} // namespace kurbo 