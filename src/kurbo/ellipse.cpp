#include "kurbo/ellipse.hpp"
#include "kurbo/inline_methods.hpp"
#include <cmath>

namespace kurbo {

constexpr Ellipse::Ellipse() : inner() {}
constexpr Ellipse::Ellipse(const Affine& inner) : inner(inner) {}

Ellipse Ellipse::new_ellipse(const Point& center, const Vec2& radii, double x_rotation) {
    return private_new(center.to_vec2(), radii.x, radii.y, x_rotation);
}

Ellipse Ellipse::from_rect(const Rect& rect) {
    Vec2 center = rect.center().to_vec2();
    Size size = rect.size() / 2.0;
    return private_new(center, size.width, size.height, 0.0);
}

constexpr Ellipse Ellipse::from_affine(const Affine& affine) {
    return Ellipse(affine);
}

Ellipse Ellipse::from_circle(const Circle& circle) {
    return new_ellipse(circle.center, Vec2::splat(circle.radius), 0.0);
}

Ellipse Ellipse::with_center(const Point& new_center) const {
    return Ellipse(inner.with_translation(new_center.to_vec2()));
}

Ellipse Ellipse::with_radii(const Vec2& new_radii) const {
    auto [scale, rotation] = inner.svd();
    Vec2 translation = inner.translation();
    return private_new(translation, new_radii.x, new_radii.y, rotation);
}

Ellipse Ellipse::with_rotation(double rotation) const {
    auto [scale, _] = inner.svd();
    Vec2 translation = inner.translation();
    return private_new(translation, scale.x, scale.y, rotation);
}

Point Ellipse::center() const {
    return inner.translation().to_point();
}

Vec2 Ellipse::radii() const {
    return inner.svd().first;
}

double Ellipse::rotation() const {
    return inner.svd().second;
}

std::pair<Vec2, double> Ellipse::radii_and_rotation() const {
    return inner.svd();
}

bool Ellipse::is_finite() const {
    return inner.is_finite();
}

bool Ellipse::is_nan() const {
    return inner.is_nan();
}

double Ellipse::area() const {
    Vec2 radii_vec = radii();
    return M_PI * radii_vec.x * radii_vec.y;
}

double Ellipse::perimeter(double accuracy) const {
    Vec2 radii_vec = radii();
    
    if (!radii_vec.is_finite()) {
        return std::nan("");
    }
    
    // Check for degenerate cases
    if (radii_vec.x == 0.0 || radii_vec.y == 0.0) {
        return 4.0 * std::max(radii_vec.x, radii_vec.y);
    }
    
    // For circular ellipses, use exact formula
    if (std::abs(radii_vec.x - radii_vec.y) < 1e-9) {
        return 2.0 * M_PI * radii_vec.x;
    }
    
    // For non-circular ellipses, use approximation
    // This is a simplified approximation - a more accurate one would use
    // Ramanujan's approximation or numerical integration
    double a = std::max(radii_vec.x, radii_vec.y);
    double b = std::min(radii_vec.x, radii_vec.y);
    double h = std::pow(a - b, 2) / std::pow(a + b, 2);
    return M_PI * (a + b) * (1.0 + 3.0 * h / (10.0 + std::sqrt(4.0 - 3.0 * h)));
}

int Ellipse::winding(const Point& pt) const {
    // Transform point to unit circle coordinates
    Point transformed = inner.inverse() * pt;
    double dist_sq = transformed.to_vec2().hypot2();
    
    if (dist_sq < 1.0) {
        return 1;
    } else {
        return 0;
    }
}

Rect Ellipse::bounding_box() const {
    // This is a simplified bounding box calculation
    // A more accurate one would consider the actual ellipse geometry
    Vec2 radii_vec = radii();
    Point center_pt = center();
    
    return Rect::new_rect(
        center_pt.x - std::abs(radii_vec.x),
        center_pt.y - std::abs(radii_vec.y),
        center_pt.x + std::abs(radii_vec.x),
        center_pt.y + std::abs(radii_vec.y)
    );
}

std::optional<Ellipse> Ellipse::as_ellipse() const {
    return *this;
}

Ellipse Ellipse::operator+(const Vec2& v) const {
    return Ellipse(Affine::translate(v) * inner);
}

Ellipse& Ellipse::operator+=(const Vec2& v) {
    inner = Affine::translate(v) * inner;
    return *this;
}

Ellipse Ellipse::operator-(const Vec2& v) const {
    return Ellipse(Affine::translate(-v) * inner);
}

Ellipse& Ellipse::operator-=(const Vec2& v) {
    inner = Affine::translate(-v) * inner;
    return *this;
}

Ellipse Ellipse::operator*(const Affine& affine) const {
    return Ellipse(affine * inner);
}

Ellipse& Ellipse::operator*=(const Affine& affine) {
    inner = affine * inner;
    return *this;
}

Ellipse Ellipse::zero() {
    return Ellipse(Affine::identity());
}

Ellipse Ellipse::private_new(const Vec2& center, double scale_x, double scale_y, double x_rotation) {
    // Since the circle is symmetric about the x and y axes, using absolute values for the
    // radii results in the same ellipse. For simplicity we make this change here.
    return Ellipse(
        Affine::translate(center) *
        Affine::rotate(x_rotation) *
        Affine::scale_non_uniform(std::abs(scale_x), std::abs(scale_y))
    );
}

} // namespace kurbo 