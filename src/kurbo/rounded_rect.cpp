#include "kurbo/rounded_rect.hpp"
#include "kurbo/inline_methods.hpp"
#include <cmath>

namespace kurbo {

RoundedRect::RoundedRect() : rect(), radii(0.0) {}

RoundedRect::RoundedRect(const Rect& rect, const RoundedRectRadii& radii) : rect(rect), radii(radii) {}

RoundedRect RoundedRect::new_rounded_rect(double x0, double y0, double x1, double y1, double radius) {
    return from_rect(Rect::new_rect(x0, y0, x1, y1), radius);
}

RoundedRect RoundedRect::new_rounded_rect(double x0, double y0, double x1, double y1, const RoundedRectRadii& radii) {
    return from_rect(Rect::new_rect(x0, y0, x1, y1), radii);
}

RoundedRect RoundedRect::from_rect(const Rect& rect, const RoundedRectRadii& radii) {
    Rect abs_rect = rect.abs();
    double shortest_side_length = std::min(abs_rect.width(), abs_rect.height());
    RoundedRectRadii clamped_radii = radii.clamp(shortest_side_length / 2.0);
    return RoundedRect(abs_rect, clamped_radii);
}

RoundedRect RoundedRect::from_rect(const Rect& rect, double radius) {
    return from_rect(rect, RoundedRectRadii(radius));
}

RoundedRect RoundedRect::from_points(const Point& p0, const Point& p1, const RoundedRectRadii& radii) {
    return Rect::from_points(p0, p1).to_rounded_rect(radii);
}

RoundedRect RoundedRect::from_points(const Point& p0, const Point& p1, double radius) {
    return from_points(p0, p1, RoundedRectRadii(radius));
}

RoundedRect RoundedRect::from_origin_size(const Point& origin, const Size& size, const RoundedRectRadii& radii) {
    return Rect::from_origin_size(origin, size).to_rounded_rect(radii);
}

RoundedRect RoundedRect::from_origin_size(const Point& origin, const Size& size, double radius) {
    return from_origin_size(origin, size, RoundedRectRadii(radius));
}

double RoundedRect::width() const {
    return rect.width();
}

double RoundedRect::height() const {
    return rect.height();
}

RoundedRectRadii RoundedRect::get_radii() const {
    return radii;
}

Rect RoundedRect::get_rect() const {
    return rect;
}

Point RoundedRect::origin() const {
    return rect.origin();
}

Point RoundedRect::center() const {
    return rect.center();
}

bool RoundedRect::is_finite() const {
    return rect.is_finite() && radii.is_finite();
}

bool RoundedRect::is_nan() const {
    return rect.is_nan() || radii.is_nan();
}

RoundedRect RoundedRect::operator+(const Vec2& v) const {
    return RoundedRect(rect + v, radii);
}

RoundedRect& RoundedRect::operator+=(const Vec2& v) {
    rect += v;
    return *this;
}

RoundedRect RoundedRect::operator-(const Vec2& v) const {
    return RoundedRect(rect - v, radii);
}

RoundedRect& RoundedRect::operator-=(const Vec2& v) {
    rect -= v;
    return *this;
}

RoundedRect RoundedRect::zero() {
    return RoundedRect(Rect::zero(), RoundedRectRadii(0.0));
}

std::ostream& operator<<(std::ostream& os, const RoundedRect& rr) {
    os << "RoundedRect(rect=" << rr.rect << ", radii=" << rr.radii << ")";
    return os;
}

} // namespace kurbo 