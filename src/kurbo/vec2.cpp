#include "kurbo/vec2.hpp"
#include "kurbo/inline_methods.hpp"
#include "kurbo/point.hpp"
#include "kurbo/size.hpp"
#include "kurbo/common.hpp"
#include <cmath>
#include <iostream>
#include <numeric>

namespace kurbo {

// Static constants
const Vec2 Vec2::ZERO = Vec2(0.0, 0.0);



double Vec2::dot(const Vec2& other) const {
    return x * other.x + y * other.y;
}

double Vec2::cross(const Vec2& other) const {
    return x * other.y - y * other.x;
}

double Vec2::hypot() const {
    // Avoid std::hypot as it calls a slow library function.
    return std::sqrt(hypot2());
}

double Vec2::length() const {
    return hypot();
}

double Vec2::hypot2() const {
    return dot(*this);
}

double Vec2::length_squared() const {
    return hypot2();
}

double Vec2::atan2() const {
    return std::atan2(y, x);
}

double Vec2::angle() const {
    return atan2();
}

Vec2 Vec2::from_angle(double th) {
    double th_sin, th_cos;
    th_sin = std::sin(th);
    th_cos = std::cos(th);
    return Vec2(th_cos, th_sin);
}

Vec2 Vec2::lerp(const Vec2& other, double t) const {
    return *this + t * (other - *this);
}

Vec2 Vec2::normalize() const {
    return *this / hypot();
}

Vec2 Vec2::round() const {
    return Vec2(std::round(x), std::round(y));
}

Vec2 Vec2::ceil() const {
    return Vec2(std::ceil(x), std::ceil(y));
}

Vec2 Vec2::floor() const {
    return Vec2(std::floor(x), std::floor(y));
}

Vec2 Vec2::expand() const {
    return Vec2(detail::expand(x), detail::expand(y));
}

Vec2 Vec2::trunc() const {
    return Vec2(std::trunc(x), std::trunc(y));
}

bool Vec2::is_finite() const {
    return std::isfinite(x) && std::isfinite(y);
}

bool Vec2::is_nan() const {
    return std::isnan(x) || std::isnan(y);
}

Vec2 Vec2::turn_90() const {
    return Vec2(-y, x);
}

Vec2 Vec2::rotate_scale(const Vec2& rhs) const {
    return Vec2(x * rhs.x - y * rhs.y, x * rhs.y + y * rhs.x);
}

// Arithmetic operators
Vec2 Vec2::operator+(const Vec2& other) const {
    return Vec2(x + other.x, y + other.y);
}

Vec2& Vec2::operator+=(const Vec2& other) {
    x += other.x;
    y += other.y;
    return *this;
}

Vec2 Vec2::operator-(const Vec2& other) const {
    return Vec2(x - other.x, y - other.y);
}

Vec2& Vec2::operator-=(const Vec2& other) {
    x -= other.x;
    y -= other.y;
    return *this;
}

Vec2 Vec2::operator*(double scalar) const {
    return Vec2(x * scalar, y * scalar);
}

Vec2& Vec2::operator*=(double scalar) {
    x *= scalar;
    y *= scalar;
    return *this;
}

Vec2 Vec2::operator*(const Vec2& other) const {
    return Vec2(x * other.x, y * other.y);
}

Vec2 Vec2::operator/(double scalar) const {
    if (scalar == 0.0) {
        // Handle division by zero - return NaN
        return Vec2(std::numeric_limits<double>::quiet_NaN(), 
                   std::numeric_limits<double>::quiet_NaN());
    }
    return Vec2(x / scalar, y / scalar);
}

Vec2& Vec2::operator/=(double scalar) {
    if (scalar == 0.0) {
        x = std::numeric_limits<double>::quiet_NaN();
        y = std::numeric_limits<double>::quiet_NaN();
    } else {
        x /= scalar;
        y /= scalar;
    }
    return *this;
}

Vec2 Vec2::operator-() const {
    return Vec2(-x, -y);
}

bool Vec2::operator==(const Vec2& other) const {
    return x == other.x && y == other.y;
}

bool Vec2::operator!=(const Vec2& other) const {
    return !(*this == other);
}

// Free functions
Vec2 operator*(double scalar, const Vec2& vec) {
    return vec * scalar;
}

// Conversion functions
// Удалено: Vec2 from_tuple(const std::tuple<double, double>& v)
// Удалено: std::tuple<double, double> to_tuple(const Vec2& v)

// Stream operators
std::ostream& operator<<(std::ostream& os, const Vec2& vec) {
    os << "Vec2(" << vec.x << ", " << vec.y << ")";
    return os;
}

} // namespace kurbo 