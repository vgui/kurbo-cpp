#include "kurbo/affine.hpp"
#include "kurbo/inline_methods.hpp"
#include <cmath>

namespace kurbo {

const Affine Affine::IDENTITY = Affine::scale(1.0);
const Affine Affine::FLIP_Y = Affine({1.0, 0.0, 0.0, -1.0, 0.0, 0.0});
const Affine Affine::FLIP_X = Affine({-1.0, 0.0, 0.0, 1.0, 0.0, 0.0});

Affine Affine::scale_about(double s, const Point& center) {
    Vec2 center_vec = center.to_vec2();
    return translate(-center_vec).then_scale(s).then_translate(center_vec);
}

Affine Affine::rotate(double th) {
    double s = std::sin(th);
    double c = std::cos(th);
    return Affine({c, s, -s, c, 0.0, 0.0});
}

Affine Affine::rotate_about(double th, const Point& center) {
    Vec2 center_vec = center.to_vec2();
    return translate(-center_vec).then_rotate(th).then_translate(center_vec);
}

Affine Affine::translate(const Vec2& v) {
    return Affine({1.0, 0.0, 0.0, 1.0, v.x, v.y});
}

Affine Affine::skew(double skew_x, double skew_y) {
    return Affine({1.0, skew_y, skew_x, 1.0, 0.0, 0.0});
}

Affine Affine::reflect(const Point& point, const Vec2& direction) {
    Vec2 n = Vec2{direction.y, -direction.x}.normalize();
    
    // Compute Householder reflection matrix
    double x2 = n.x * n.x;
    double xy = n.x * n.y;
    double y2 = n.y * n.y;
    
    Affine aff = Affine({
        1.0 - 2.0 * x2,
        -2.0 * xy,
        -2.0 * xy,
        1.0 - 2.0 * y2,
        point.x,
        point.y
    });
    
    return aff.pre_translate(-point.to_vec2());
}

// Chaining methods
Affine Affine::pre_rotate(double th) const {
    return *this * rotate(th);
}

Affine Affine::pre_rotate_about(double th, const Point& center) const {
    return rotate_about(th, center) * *this;
}

Affine Affine::pre_scale(double s) const {
    return *this * scale(s);
}

Affine Affine::pre_scale_non_uniform(double sx, double sy) const {
    return *this * scale_non_uniform(sx, sy);
}

Affine Affine::pre_translate(const Vec2& v) const {
    return *this * translate(v);
}

Affine Affine::then_rotate(double th) const {
    return rotate(th) * *this;
}

Affine Affine::then_rotate_about(double th, const Point& center) const {
    return rotate_about(th, center) * *this;
}

Affine Affine::then_scale(double s) const {
    return scale(s) * *this;
}

Affine Affine::then_scale_non_uniform(double sx, double sy) const {
    return scale_non_uniform(sx, sy) * *this;
}

Affine Affine::then_scale_about(double s, const Point& center) const {
    return scale_about(s, center) * *this;
}

Affine Affine::then_translate(const Vec2& v) const {
    Affine result = *this;
    result.coeffs[4] += v.x;
    result.coeffs[5] += v.y;
    return result;
}

// Matrix operations
std::array<double, 6> Affine::as_coeffs() const {
    return coeffs;
}

double Affine::determinant() const {
    return coeffs[0] * coeffs[3] - coeffs[1] * coeffs[2];
}

Affine Affine::inverse() const {
    double inv_det = 1.0 / determinant();
    return Affine({
        inv_det * coeffs[3],
        -inv_det * coeffs[1],
        -inv_det * coeffs[2],
        inv_det * coeffs[0],
        inv_det * (coeffs[2] * coeffs[5] - coeffs[3] * coeffs[4]),
        inv_det * (coeffs[1] * coeffs[4] - coeffs[0] * coeffs[5])
    });
}

// Transformations
Point Affine::operator*(const Point& p) const {
    double x = coeffs[0] * p.x + coeffs[2] * p.y + coeffs[4];
    double y = coeffs[1] * p.x + coeffs[3] * p.y + coeffs[5];
    return Point{x, y};
}

Vec2 Affine::operator*(const Vec2& v) const {
    double x = coeffs[0] * v.x + coeffs[2] * v.y;
    double y = coeffs[1] * v.x + coeffs[3] * v.y;
    return Vec2{x, y};
}

Size Affine::operator*(const Size& s) const {
    double width = coeffs[0] * s.width + coeffs[2] * s.height;
    double height = coeffs[1] * s.width + coeffs[3] * s.height;
    return Size{width, height};
}

// Utility methods
bool Affine::is_finite() const {
    return std::isfinite(coeffs[0]) && std::isfinite(coeffs[1]) &&
           std::isfinite(coeffs[2]) && std::isfinite(coeffs[3]) &&
           std::isfinite(coeffs[4]) && std::isfinite(coeffs[5]);
}

bool Affine::is_nan() const {
    return std::isnan(coeffs[0]) || std::isnan(coeffs[1]) ||
           std::isnan(coeffs[2]) || std::isnan(coeffs[3]) ||
           std::isnan(coeffs[4]) || std::isnan(coeffs[5]);
}

Vec2 Affine::translation() const {
    return Vec2{coeffs[4], coeffs[5]};
}

Affine Affine::with_translation(const Vec2& v) const {
    Affine result = *this;
    result.coeffs[4] = v.x;
    result.coeffs[5] = v.y;
    return result;
}

// Operators
Affine Affine::operator*(const Affine& other) const {
    return Affine({
        coeffs[0] * other.coeffs[0] + coeffs[2] * other.coeffs[1],
        coeffs[1] * other.coeffs[0] + coeffs[3] * other.coeffs[1],
        coeffs[0] * other.coeffs[2] + coeffs[2] * other.coeffs[3],
        coeffs[1] * other.coeffs[2] + coeffs[3] * other.coeffs[3],
        coeffs[0] * other.coeffs[4] + coeffs[2] * other.coeffs[5] + coeffs[4],
        coeffs[1] * other.coeffs[4] + coeffs[3] * other.coeffs[5] + coeffs[5]
    });
}

Affine& Affine::operator*=(const Affine& other) {
    *this = *this * other;
    return *this;
}

Affine Affine::identity() {
    return IDENTITY;
}

std::pair<Vec2, double> Affine::svd() const {
    // Simplified SVD implementation for 2x2 matrices
    // This returns the scale factors and rotation angle
    double det = coeffs[0] * coeffs[3] - coeffs[1] * coeffs[2];
    double trace = coeffs[0] + coeffs[3];
    
    // Calculate eigenvalues
    double discriminant = trace * trace - 4.0 * det;
    if (discriminant < 0) discriminant = 0;
    
    double lambda1 = (trace + std::sqrt(discriminant)) / 2.0;
    double lambda2 = (trace - std::sqrt(discriminant)) / 2.0;
    
    Vec2 scale(std::sqrt(std::abs(lambda1)), std::sqrt(std::abs(lambda2)));
    
    // Calculate rotation angle
    double rotation = std::atan2(coeffs[1] - coeffs[2], coeffs[0] + coeffs[3]) / 2.0;
    
    return std::make_pair(scale, rotation);
}

std::ostream& operator<<(std::ostream& os, const Affine& affine) {
    os << "Affine([";
    for (size_t i = 0; i < 6; ++i) {
        if (i > 0) os << ", ";
        os << affine.coeffs[i];
    }
    os << "])";
    return os;
}

} // namespace kurbo 