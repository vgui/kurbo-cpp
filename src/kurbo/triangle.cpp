#include "kurbo/triangle.hpp"
#include "kurbo/inline_methods.hpp"
#include <cmath>

namespace kurbo {

// Constructors are now inline in header

// Static constants
const Triangle Triangle::ZERO = Triangle::from_coords(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
const Triangle Triangle::EQUILATERAL = Triangle::from_coords(
    0.5, 0.866025403784438646763723170752936183, // sqrt(3)/2
    0.0, 0.0,
    1.0, 0.0
);

Point Triangle::centroid() const {
    return (1.0 / 3.0 * (a.to_vec2() + b.to_vec2() + c.to_vec2())).to_point();
}

std::array<Vec2, 3> Triangle::offsets() const {
    Vec2 centroid_vec = centroid().to_vec2();
    
    return {
        a.to_vec2() - centroid_vec,
        b.to_vec2() - centroid_vec,
        c.to_vec2() - centroid_vec
    };
}

double Triangle::area() const {
    return 0.5 * (b - a).cross(c - a);
}

bool Triangle::is_zero_area() const {
    return area() == 0.0;
}

Circle Triangle::inscribed_circle() const {
    double ab = a.distance(b);
    double bc = b.distance(c);
    double ac = a.distance(c);
    
    double perimeter_recip = 1.0 / (ab + bc + ac);
    Vec2 incenter = (a.to_vec2() * bc + b.to_vec2() * ac + c.to_vec2() * ab) * perimeter_recip;
    
    return Circle::new_circle(incenter.to_point(), 2.0 * area() * perimeter_recip);
}

Circle Triangle::circumscribed_circle() const {
    Vec2 b_vec = b - a;
    Vec2 c_vec = c - a;
    double b_len2 = b_vec.hypot2();
    double c_len2 = c_vec.hypot2();
    double d_recip = 0.5 / b_vec.cross(c_vec);
    
    double x = (c_vec.y * b_len2 - b_vec.y * c_len2) * d_recip;
    double y = (b_vec.x * c_len2 - c_vec.x * b_len2) * d_recip;
    double r = std::sqrt(b_len2 * c_len2) * (c_vec - b_vec).hypot() * d_recip;
    
    return Circle::new_circle(a + Vec2(x, y), r);
}

Triangle Triangle::inflate(double scalar) const {
    Point centroid_pt = centroid();
    
    return Triangle::new_triangle(
        centroid_pt + Vec2(0.0, scalar),
        centroid_pt + scalar * Vec2::from_angle(5.0 * M_PI / 4.0),
        centroid_pt + scalar * Vec2::from_angle(7.0 * M_PI / 4.0)
    );
}

bool Triangle::is_finite() const {
    return a.is_finite() && b.is_finite() && c.is_finite();
}

bool Triangle::is_nan() const {
    return a.is_nan() || b.is_nan() || c.is_nan();
}

double Triangle::perimeter(double accuracy) const {
    return a.distance(b) + b.distance(c) + c.distance(a);
}

int Triangle::winding(const Point& pt) const {
    double s0 = (b - a).cross(pt - a);
    double s1 = (c - b).cross(pt - b);
    double s2 = (a - c).cross(pt - c);
    
    // Use signum-like logic
    int sign0 = (s0 > 0) ? 1 : (s0 < 0) ? -1 : 0;
    int sign1 = (s1 > 0) ? 1 : (s1 < 0) ? -1 : 0;
    int sign2 = (s2 > 0) ? 1 : (s2 < 0) ? -1 : 0;
    
    if (sign0 == sign1 && sign1 == sign2) {
        return sign0;
    } else {
        return 0;
    }
}

Rect Triangle::bounding_box() const {
    double min_x = std::min(std::min(a.x, b.x), c.x);
    double min_y = std::min(std::min(a.y, b.y), c.y);
    double max_x = std::max(std::max(a.x, b.x), c.x);
    double max_y = std::max(std::max(a.y, b.y), c.y);
    
    return Rect::new_rect(min_x, min_y, max_x, max_y);
}

std::optional<Triangle> Triangle::as_triangle() const {
    return *this;
}

Triangle Triangle::operator+(const Vec2& v) const {
    return Triangle::new_triangle(a + v, b + v, c + v);
}

Triangle& Triangle::operator+=(const Vec2& v) {
    a += v;
    b += v;
    c += v;
    return *this;
}

Triangle Triangle::operator-(const Vec2& v) const {
    return Triangle::new_triangle(a - v, b - v, c - v);
}

Triangle& Triangle::operator-=(const Vec2& v) {
    a -= v;
    b -= v;
    c -= v;
    return *this;
}

Triangle Triangle::zero() {
    return Triangle::ZERO;
}

} // namespace kurbo 