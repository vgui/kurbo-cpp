#include "kurbo/arc.hpp"
#include "kurbo/ellipse.hpp"
#include <ostream>
#include <cmath>

namespace kurbo {

Arc::Arc() : center(0.0, 0.0), radii(0.0, 0.0), start_angle(0.0), sweep_angle(0.0), x_rotation(0.0) {}

Arc::Arc(const Point& center, const Vec2& radii, double start_angle, double sweep_angle, double x_rotation)
    : center(center), radii(radii), start_angle(start_angle), sweep_angle(sweep_angle), x_rotation(x_rotation) {}

Arc Arc::new_arc(const Point& center, const Vec2& radii, double start_angle, double sweep_angle, double x_rotation) {
    return Arc(center, radii, start_angle, sweep_angle, x_rotation);
}

Arc Arc::reversed() const {
    return Arc(center, radii, start_angle + sweep_angle, -sweep_angle, x_rotation);
}

Arc Arc::zero() {
    return Arc();
}

Arc operator*(const Affine& affine, const Arc& arc) {
    // Transform the arc as an ellipse, then reconstruct the arc with new center/radii/rotation
    Ellipse ellipse = affine * Ellipse(arc.center, arc.radii, arc.x_rotation);
    Point center = ellipse.center();
    auto [radii, rotation] = ellipse.radii_and_rotation();
    return Arc(center, radii, arc.start_angle, arc.sweep_angle, rotation);
}

std::ostream& operator<<(std::ostream& os, const Arc& arc) {
    os << "Arc(center=" << arc.center << ", radii=" << arc.radii << ", start_angle=" << arc.start_angle
       << ", sweep_angle=" << arc.sweep_angle << ", x_rotation=" << arc.x_rotation << ")";
    return os;
}

} // namespace kurbo 