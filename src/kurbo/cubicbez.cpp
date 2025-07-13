#include "kurbo/cubicbez.hpp"
#include "kurbo/inline_methods.hpp"
#include "kurbo/line.hpp"
#include "kurbo/common.hpp"
#include <cmath>
#include <algorithm>

namespace kurbo {

CubicBez::CubicBez() : p0(0.0, 0.0), p1(0.0, 0.0), p2(0.0, 0.0), p3(0.0, 0.0) {}

CubicBez::CubicBez(const Point& p0, const Point& p1, const Point& p2, const Point& p3)
    : p0(p0), p1(p1), p2(p2), p3(p3) {}

CubicBez CubicBez::new_curve(const Point& p0, const Point& p1, const Point& p2, const Point& p3) {
    return CubicBez(p0, p1, p2, p3);
}

std::vector<std::pair<double, double>> CubicBez::to_quads(double accuracy) const {
    // This is a simplified implementation
    // In the full implementation, this would return actual quadratic approximations
    std::vector<std::pair<double, double>> result;
    result.push_back({0.0, 1.0});
    return result;
}

std::pair<CubicBez, CubicBez> CubicBez::subdivide() const {
    // De Casteljau's algorithm for subdivision at t = 0.5
    Point mid01 = p0.lerp(p1, 0.5);
    Point mid12 = p1.lerp(p2, 0.5);
    Point mid23 = p2.lerp(p3, 0.5);
    Point mid012 = mid01.lerp(mid12, 0.5);
    Point mid123 = mid12.lerp(mid23, 0.5);
    Point mid0123 = mid012.lerp(mid123, 0.5);

    CubicBez left(p0, mid01, mid012, mid0123);
    CubicBez right(mid0123, mid123, mid23, p3);
    
    return {left, right};
}

std::tuple<CubicBez, CubicBez, CubicBez> CubicBez::subdivide_3() const {
    // Subdivide into three equal parts using De Casteljau's algorithm
    // First subdivision at t = 1/3
    Point mid01_1 = p0.lerp(p1, 1.0/3.0);
    Point mid12_1 = p1.lerp(p2, 1.0/3.0);
    Point mid23_1 = p2.lerp(p3, 1.0/3.0);
    Point mid012_1 = mid01_1.lerp(mid12_1, 1.0/3.0);
    Point mid123_1 = mid12_1.lerp(mid23_1, 1.0/3.0);
    Point mid0123_1 = mid012_1.lerp(mid123_1, 1.0/3.0);
    
    // Second subdivision at t = 2/3
    Point mid01_2 = p0.lerp(p1, 2.0/3.0);
    Point mid12_2 = p1.lerp(p2, 2.0/3.0);
    Point mid23_2 = p2.lerp(p3, 2.0/3.0);
    Point mid012_2 = mid01_2.lerp(mid12_2, 2.0/3.0);
    Point mid123_2 = mid12_2.lerp(mid23_2, 2.0/3.0);
    Point mid0123_2 = mid012_2.lerp(mid123_2, 2.0/3.0);
    
    CubicBez first(p0, mid01_1, mid012_1, mid0123_1);
    CubicBez second(mid0123_1, mid123_1, mid23_1, mid0123_2);
    CubicBez third(mid0123_2, mid012_2, mid01_2, p3);
    
    return {first, second, third};
}

bool CubicBez::is_finite() const {
    return p0.is_finite() && p1.is_finite() && p2.is_finite() && p3.is_finite();
}

bool CubicBez::is_nan() const {
    return p0.is_nan() || p1.is_nan() || p2.is_nan() || p3.is_nan();
}

std::vector<double> CubicBez::inflections() const {
    // Calculate inflection points
    // This is a simplified implementation
    std::vector<double> result;
    // TODO: Implement proper inflection point calculation
    return result;
}

std::vector<double> CubicBez::tangents_to_point(const Point& p) const {
    // Calculate parameter values where tangents point to the given point
    // This is a simplified implementation
    std::vector<double> result;
    // TODO: Implement proper tangent calculation
    return result;
}

CubicBez CubicBez::operator+(const Vec2& v) const {
    return CubicBez(p0 + v, p1 + v, p2 + v, p3 + v);
}

CubicBez& CubicBez::operator+=(const Vec2& v) {
    p0 += v;
    p1 += v;
    p2 += v;
    p3 += v;
    return *this;
}

CubicBez CubicBez::operator-(const Vec2& v) const {
    return CubicBez(p0 - v, p1 - v, p2 - v, p3 - v);
}

CubicBez& CubicBez::operator-=(const Vec2& v) {
    p0 -= v;
    p1 -= v;
    p2 -= v;
    p3 -= v;
    return *this;
}

CubicBez CubicBez::zero() {
    return CubicBez();
}

CubicBez operator*(const Affine& affine, const CubicBez& cubic) {
    return CubicBez(
        affine * cubic.p0,
        affine * cubic.p1,
        affine * cubic.p2,
        affine * cubic.p3
    );
}

std::ostream& operator<<(std::ostream& os, const CubicBez& cubic) {
    os << "CubicBez(" << cubic.p0 << ", " << cubic.p1 << ", " << cubic.p2 << ", " << cubic.p3 << ")";
    return os;
}

} // namespace kurbo 