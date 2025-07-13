#include "kurbo/rect.hpp"
#include "kurbo/rounded_rect.hpp"
#include "kurbo/inline_methods.hpp"
#include <cmath>

namespace kurbo {

const Rect Rect::ZERO = Rect(0.0, 0.0, 0.0, 0.0);

Rect Rect::from_points(const Point& p0, const Point& p1) {
    return Rect(p0.x, p0.y, p1.x, p1.y).abs();
}

Rect Rect::from_origin_size(const Point& origin, const Size& size) {
    return from_points(origin, origin + size.to_vec2());
}

Rect Rect::from_center_size(const Point& center, const Size& size) {
    Size half_size = size * 0.5;
    return Rect(
        center.x - half_size.width,
        center.y - half_size.height,
        center.x + half_size.width,
        center.y + half_size.height
    );
}

Rect Rect::with_origin(const Point& origin) const {
    return from_origin_size(origin, size());
}

Rect Rect::with_size(const Size& size) const {
    return from_origin_size(origin(), size);
}

Rect Rect::inset(double insets) const {
    return Rect(x0 - insets, y0 - insets, x1 + insets, y1 + insets);
}

double Rect::width() const {
    return x1 - x0;
}

double Rect::height() const {
    return y1 - y0;
}

double Rect::min_x() const {
    return std::min(x0, x1);
}

double Rect::max_x() const {
    return std::max(x0, x1);
}

double Rect::min_y() const {
    return std::min(y0, y1);
}

double Rect::max_y() const {
    return std::max(y0, y1);
}

Point Rect::origin() const {
    return Point(x0, y0);
}

Size Rect::size() const {
    return Size(width(), height());
}

double Rect::area() const {
    return width() * height();
}

bool Rect::is_zero_area() const {
    return area() == 0.0;
}

Point Rect::center() const {
    return Point(0.5 * (x0 + x1), 0.5 * (y0 + y1));
}

bool Rect::contains(const Point& point) const {
    return point.x >= x0 && point.x < x1 && point.y >= y0 && point.y < y1;
}

Rect Rect::abs() const {
    return Rect(std::min(x0, x1), std::min(y0, y1), std::max(x0, x1), std::max(y0, y1));
}

Rect Rect::union_rect(const Rect& other) const {
    return Rect(
        std::min(x0, other.x0),
        std::min(y0, other.y0),
        std::max(x1, other.x1),
        std::max(y1, other.y1)
    );
}

Rect Rect::union_pt(const Point& pt) const {
    return Rect(
        std::min(x0, pt.x),
        std::min(y0, pt.y),
        std::max(x1, pt.x),
        std::max(y1, pt.y)
    );
}

Rect Rect::intersect(const Rect& other) const {
    double new_x0 = std::max(x0, other.x0);
    double new_y0 = std::max(y0, other.y0);
    double new_x1 = std::max(std::min(x1, other.x1), new_x0);
    double new_y1 = std::max(std::min(y1, other.y1), new_y0);
    return Rect(new_x0, new_y0, new_x1, new_y1);
}

bool Rect::overlaps(const Rect& other) const {
    return x0 < other.x1 && x1 > other.x0 && y0 < other.y1 && y1 > other.y0;
}

bool Rect::contains_rect(const Rect& other) const {
    return x0 <= other.x0 && x1 >= other.x1 && y0 <= other.y0 && y1 >= other.y1;
}

Rect Rect::inflate(double width, double height) const {
    return Rect(x0 - width, y0 - height, x1 + width, y1 + height);
}

Rect Rect::round() const {
    return Rect(std::round(x0), std::round(y0), std::round(x1), std::round(y1));
}

Rect Rect::ceil() const {
    return Rect(std::ceil(x0), std::ceil(y0), std::ceil(x1), std::ceil(y1));
}

Rect Rect::floor() const {
    return Rect(std::floor(x0), std::floor(y0), std::floor(x1), std::floor(y1));
}

Rect Rect::expand() const {
    return Rect(
        x0 < 0 ? std::floor(x0) : std::ceil(x0),
        y0 < 0 ? std::floor(y0) : std::ceil(y0),
        x1 < 0 ? std::floor(x1) : std::ceil(x1),
        y1 < 0 ? std::floor(y1) : std::ceil(y1)
    );
}

Rect Rect::trunc() const {
    return Rect(std::trunc(x0), std::trunc(y0), std::trunc(x1), std::trunc(y1));
}

Rect Rect::scale_from_origin(double factor) const {
    return Rect(x0 * factor, y0 * factor, x1 * factor, y1 * factor);
}

RoundedRect Rect::to_rounded_rect(const RoundedRectRadii& radii) const {
    return RoundedRect::from_rect(*this, radii);
}

RoundedRect Rect::to_rounded_rect(double radius) const {
    return to_rounded_rect(RoundedRectRadii(radius));
}

double Rect::aspect_ratio() const {
    double w = width();
    double h = height();
    if (h == 0.0) return std::numeric_limits<double>::infinity();
    return w / h;
}

Rect Rect::contained_rect_with_aspect_ratio(double aspect_ratio) const {
    double w = width();
    double h = height();
    double current_ratio = w / h;
    
    if (current_ratio > aspect_ratio) {
        // Too wide, reduce width
        double new_width = h * aspect_ratio;
        double x_center = (x0 + x1) * 0.5;
        return Rect(x_center - new_width * 0.5, y0, x_center + new_width * 0.5, y1);
    } else {
        // Too tall, reduce height
        double new_height = w / aspect_ratio;
        double y_center = (y0 + y1) * 0.5;
        return Rect(x0, y_center - new_height * 0.5, x1, y_center + new_height * 0.5);
    }
}

bool Rect::is_finite() const {
    return std::isfinite(x0) && std::isfinite(y0) && std::isfinite(x1) && std::isfinite(y1);
}

bool Rect::is_nan() const {
    return std::isnan(x0) || std::isnan(y0) || std::isnan(x1) || std::isnan(y1);
}

Rect Rect::operator+(const Vec2& v) const {
    return Rect(x0 + v.x, y0 + v.y, x1 + v.x, y1 + v.y);
}

Rect& Rect::operator+=(const Vec2& v) {
    x0 += v.x;
    y0 += v.y;
    x1 += v.x;
    y1 += v.y;
    return *this;
}

Rect Rect::operator-(const Vec2& v) const {
    return Rect(x0 - v.x, y0 - v.y, x1 - v.x, y1 - v.y);
}

Rect& Rect::operator-=(const Vec2& v) {
    x0 -= v.x;
    y0 -= v.y;
    x1 -= v.x;
    y1 -= v.y;
    return *this;
}

Rect Rect::zero() {
    return ZERO;
}

} // namespace kurbo 