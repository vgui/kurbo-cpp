#include "kurbo/point.hpp"
#include "kurbo/inline_methods.hpp"
#include "kurbo/vec2.hpp"
#include "kurbo/common.hpp"
#include <cmath>
#include <iostream>

namespace kurbo {

// Static constants
const Point Point::ZERO = Point(0.0, 0.0);
const Point Point::ORIGIN = Point(0.0, 0.0);

Point Point::lerp(const Point& other, double t) const {
    return to_vec2().lerp(other.to_vec2(), t).to_point();
}

Point Point::midpoint(const Point& other) const {
    return Point(0.5 * (x + other.x), 0.5 * (y + other.y));
}

double Point::distance(const Point& other) const {
    return (*this - other).hypot();
}

double Point::distance_squared(const Point& other) const {
    return (*this - other).hypot2();
}

Point Point::round() const {
    return Point(std::round(x), std::round(y));
}

Point Point::ceil() const {
    return Point(std::ceil(x), std::ceil(y));
}

Point Point::floor() const {
    return Point(std::floor(x), std::floor(y));
}

Point Point::expand() const {
    return Point(detail::expand(x), detail::expand(y));
}

Point Point::trunc() const {
    return Point(std::trunc(x), std::trunc(y));
}

bool Point::is_finite() const {
    return std::isfinite(x) && std::isfinite(y);
}

bool Point::is_nan() const {
    return std::isnan(x) || std::isnan(y);
}

Point Point::zero() {
    return Point(0.0, 0.0);
}

// Operators
Point Point::operator+(const Vec2& other) const {
    return Point(x + other.x, y + other.y);
}

Point& Point::operator+=(const Vec2& other) {
    x += other.x;
    y += other.y;
    return *this;
}

Point Point::operator-(const Vec2& other) const {
    return Point(x - other.x, y - other.y);
}

Point& Point::operator-=(const Vec2& other) {
    x -= other.x;
    y -= other.y;
    return *this;
}

Vec2 Point::operator-(const Point& other) const {
    return Vec2(x - other.x, y - other.y);
}

Point Point::operator+(const std::tuple<double, double>& other) const {
    auto [ox, oy] = other;
    return Point(x + ox, y + oy);
}

Point& Point::operator+=(const std::tuple<double, double>& other) {
    auto [ox, oy] = other;
    x += ox;
    y += oy;
    return *this;
}

Point Point::operator-(const std::tuple<double, double>& other) const {
    auto [ox, oy] = other;
    return Point(x - ox, y - oy);
}

Point& Point::operator-=(const std::tuple<double, double>& other) {
    auto [ox, oy] = other;
    x -= ox;
    y -= oy;
    return *this;
}

bool Point::operator==(const Point& other) const {
    return x == other.x && y == other.y;
}

bool Point::operator!=(const Point& other) const {
    return !(*this == other);
}

// Conversion functions
// Удалено: Point from_tuple(const std::tuple<double, double>& v)
// Удалено: std::tuple<double, double> to_tuple(const Point& v)

// Stream operators
std::ostream& operator<<(std::ostream& os, const Point& point) {
    os << "Point(" << point.x << ", " << point.y << ")";
    return os;
}

} // namespace kurbo 