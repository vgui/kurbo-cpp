#include "kurbo/size.hpp"
#include "kurbo/inline_methods.hpp"
#include "kurbo/point.hpp"
#include "kurbo/vec2.hpp"
#include "kurbo/rect.hpp"
#include "kurbo/rounded_rect.hpp"
#include "kurbo/rounded_rect_radii.hpp"
#include "kurbo/common.hpp"
#include <cmath>
#include <iostream>
#include <limits>

namespace kurbo {

// Static constants
const Size Size::ZERO = Size(0.0, 0.0);
const Size Size::INFINITE = Size(std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity());

double Size::max_side() const {
    return std::max(width, height);
}

double Size::min_side() const {
    return std::min(width, height);
}

double Size::area() const {
    return width * height;
}

bool Size::is_zero_area() const {
    return area() == 0.0;
}

bool Size::is_empty() const {
    return is_zero_area();
}

Size Size::min(const Size& other) const {
    return Size(std::min(width, other.width), std::min(height, other.height));
}

Size Size::max(const Size& other) const {
    return Size(std::max(width, other.width), std::max(height, other.height));
}

Size Size::clamp(const Size& min, const Size& max) const {
    return this->max(min).min(max);
}



Size Size::round() const {
    return Size(std::round(width), std::round(height));
}

Size Size::ceil() const {
    return Size(std::ceil(width), std::ceil(height));
}

Size Size::floor() const {
    return Size(std::floor(width), std::floor(height));
}

Size Size::expand() const {
    return Size(detail::expand(width), detail::expand(height));
}

Size Size::trunc() const {
    return Size(std::trunc(width), std::trunc(height));
}

double Size::aspect_ratio() const {
    return width / height;
}

Rect Size::to_rect() const {
    return Rect::from_origin_size(Point::ORIGIN, *this);
}

RoundedRect Size::to_rounded_rect(const RoundedRectRadii& radii) const {
    return RoundedRect::from_origin_size(Point::ORIGIN, *this, radii);
}

bool Size::is_finite() const {
    return std::isfinite(width) && std::isfinite(height);
}

bool Size::is_nan() const {
    return std::isnan(width) || std::isnan(height);
}

// Arithmetic operators
Size& Size::operator*=(double scalar) {
    width *= scalar;
    height *= scalar;
    return *this;
}

Size Size::operator*(const Size& other) const {
    return Size(width * other.width, height * other.height);
}

Size Size::operator*(double scalar) const {
    return Size(width * scalar, height * scalar);
}

Size& Size::operator/=(double scalar) {
    if (scalar == 0.0) {
        width = std::numeric_limits<double>::quiet_NaN();
        height = std::numeric_limits<double>::quiet_NaN();
    } else {
        width /= scalar;
        height /= scalar;
    }
    return *this;
}

Size Size::operator/(double scalar) const {
    if (scalar == 0.0) {
        return Size(std::numeric_limits<double>::quiet_NaN(), 
                   std::numeric_limits<double>::quiet_NaN());
    }
    return Size(width / scalar, height / scalar);
}

Size Size::operator+(const Size& other) const {
    return Size(width + other.width, height + other.height);
}

Size& Size::operator+=(const Size& other) {
    width += other.width;
    height += other.height;
    return *this;
}

Size Size::operator-(const Size& other) const {
    return Size(width - other.width, height - other.height);
}

Size& Size::operator-=(const Size& other) {
    width -= other.width;
    height -= other.height;
    return *this;
}

bool Size::operator==(const Size& other) const {
    return width == other.width && height == other.height;
}

bool Size::operator!=(const Size& other) const {
    return !(*this == other);
}

// Free functions
Size operator*(double scalar, const Size& size) {
    return size * scalar;
}

// Conversion functions

// Stream operators
std::ostream& operator<<(std::ostream& os, const Size& size) {
    os << "Size(" << size.width << ", " << size.height << ")";
    return os;
}

} // namespace kurbo 