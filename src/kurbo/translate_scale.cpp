#include "kurbo/translate_scale.hpp"
#include "kurbo/inline_methods.hpp"
#include <cmath>

namespace kurbo {

TranslateScale TranslateScale::translate_only(const Vec2& translation) {
    return TranslateScale(translation, 1.0);
}

TranslateScale TranslateScale::from_scale_about(double scale, const Point& focus) {
    Vec2 focus_vec = focus.to_vec2();
    Vec2 translation = focus_vec - focus_vec * scale;
    return TranslateScale(translation, scale);
}

TranslateScale TranslateScale::inverse() const {
    double scale_recip = 1.0 / scale;
    return TranslateScale(translation * -scale_recip, scale_recip);
}

bool TranslateScale::is_finite() const {
    return translation.is_finite() && std::isfinite(scale);
}

bool TranslateScale::is_nan() const {
    return translation.is_nan() || std::isnan(scale);
}

bool TranslateScale::is_invertible() const {
    return scale != 0.0 && std::isfinite(scale);
}

Point TranslateScale::operator*(const Point& p) const {
    return (scale * p.to_vec2()).to_point() + translation;
}

TranslateScale TranslateScale::operator*(const TranslateScale& other) const {
    return TranslateScale(translation + scale * other.translation, scale * other.scale);
}

TranslateScale& TranslateScale::operator*=(const TranslateScale& other) {
    *this = *this * other;
    return *this;
}

TranslateScale TranslateScale::operator+(const Vec2& v) const {
    return TranslateScale(translation + v, scale);
}

TranslateScale& TranslateScale::operator+=(const Vec2& v) {
    translation = translation + v;
    return *this;
}

TranslateScale TranslateScale::operator-(const Vec2& v) const {
    return TranslateScale(translation - v, scale);
}

TranslateScale& TranslateScale::operator-=(const Vec2& v) {
    translation = translation - v;
    return *this;
}

Affine TranslateScale::to_affine() const {
    return Affine({scale, 0.0, 0.0, scale, translation.x, translation.y});
}

TranslateScale TranslateScale::identity() {
    return TranslateScale(Vec2{0.0, 0.0}, 1.0);
}

// Free operators
TranslateScale operator*(double scalar, const TranslateScale& ts) {
    return TranslateScale(ts.translation * scalar, ts.scale * scalar);
}

TranslateScale operator+(const Vec2& v, const TranslateScale& ts) {
    return ts + v;
}

// Stream operator
std::ostream& operator<<(std::ostream& os, const TranslateScale& ts) {
    os << "TranslateScale(translation=" << ts.translation << ", scale=" << ts.scale << ")";
    return os;
}

} // namespace kurbo 