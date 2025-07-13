#include "kurbo/circle.hpp"
#include "kurbo/ellipse.hpp"
#include "kurbo/inline_methods.hpp"
#include <cmath>
#include <algorithm>

namespace kurbo {

// Constructors are now inline in header

bool Circle::is_finite() const {
    return center.is_finite() && std::isfinite(radius);
}

bool Circle::is_nan() const {
    return center.is_nan() || std::isnan(radius);
}

double Circle::area() const {
    return M_PI * radius * radius;
}

double Circle::perimeter(double accuracy) const {
    return std::abs(2.0 * M_PI * radius);
}

int Circle::winding(const Point& pt) const {
    double dist_sq = (pt - center).hypot2();
    double radius_sq = radius * radius;
    
    // Point is inside (strictly less than radius)
    if (dist_sq < radius_sq - 1e-12) {
        return 1;
    } else {
        return 0; // Point is on boundary or outside
    }
}

Rect Circle::bounding_box() const {
    double r = std::abs(radius);
    return Rect::new_rect(center.x - r, center.y - r, center.x + r, center.y + r);
}

std::optional<Circle> Circle::as_circle() const {
    return *this;
}

Circle Circle::operator+(const Vec2& v) const {
    return Circle(center + v, radius);
}

Circle& Circle::operator+=(const Vec2& v) {
    center += v;
    return *this;
}

Circle Circle::operator-(const Vec2& v) const {
    return Circle(center - v, radius);
}

Circle& Circle::operator-=(const Vec2& v) {
    center -= v;
    return *this;
}

Circle Circle::zero() {
    return Circle(Point::zero(), 0.0);
}

bool operator==(const Circle& a, const Circle& b) {
    return a.center == b.center && a.radius == b.radius;
}

Ellipse operator*(const Affine& affine, const Circle& circle) {
    // Transform the center
    Point transformed_center = affine * circle.center;
    
    // Get the transformation matrix coefficients
    auto coeffs = affine.as_coeffs();
    double xx = coeffs[0], xy = coeffs[1], yx = coeffs[2], yy = coeffs[3];
    
    // Compute scale factors from the transformation matrix
    double scale_x = std::sqrt(xx * xx + yx * yx);
    double scale_y = std::sqrt(xy * xy + yy * yy);
    
    return Ellipse(transformed_center, Vec2(circle.radius * scale_x, circle.radius * scale_y), 0.0);
}

// CircleSegment implementation
// Constructor is now inline in header

// Constructor is now inline in header

bool CircleSegment::is_finite() const {
    return center.is_finite() && std::isfinite(outer_radius) && std::isfinite(inner_radius) && 
           std::isfinite(start_angle) && std::isfinite(sweep_angle);
}

bool CircleSegment::is_nan() const {
    return center.is_nan() || std::isnan(outer_radius) || std::isnan(inner_radius) || 
           std::isnan(start_angle) || std::isnan(sweep_angle);
}

double CircleSegment::area() const {
    double outer_area = M_PI * outer_radius * outer_radius * std::abs(sweep_angle) / (2.0 * M_PI);
    double inner_area = M_PI * inner_radius * inner_radius * std::abs(sweep_angle) / (2.0 * M_PI);
    return outer_area - inner_area;
}

double CircleSegment::perimeter(double accuracy) const {
    double outer_perimeter = outer_radius * std::abs(sweep_angle);
    double inner_perimeter = inner_radius * std::abs(sweep_angle);
    double radial_length = 2.0 * (outer_radius - inner_radius);
    return outer_perimeter + inner_perimeter + radial_length;
}

int CircleSegment::winding(const Point& pt) const {
    double dist_sq = (pt - center).hypot2();
    double outer_sq = outer_radius * outer_radius;
    double inner_sq = inner_radius * inner_radius;
    
    if (dist_sq >= outer_sq || dist_sq <= inner_sq) {
        return 0;
    }
    
    // Check if point is within the angle range
    double angle = std::atan2(pt.y - center.y, pt.x - center.x);
    double end_angle = start_angle + sweep_angle;
    
    // Normalize angles to [0, 2π)
    double normalized_angle = angle;
    double normalized_start = start_angle;
    double normalized_end = end_angle;
    
    while (normalized_angle < 0) normalized_angle += 2.0 * M_PI;
    while (normalized_start < 0) normalized_start += 2.0 * M_PI;
    while (normalized_end < 0) normalized_end += 2.0 * M_PI;
    
    if (sweep_angle > 0) {
        if (normalized_angle >= normalized_start && normalized_angle <= normalized_end) {
            return 1;
        }
    } else {
        if (normalized_angle >= normalized_end && normalized_angle <= normalized_start) {
            return 1;
        }
    }
    
    return 0;
}

Rect CircleSegment::bounding_box() const {
    // This is a simplified bounding box calculation
    // A more accurate one would consider the actual arc geometry
    double max_radius = std::max(outer_radius, inner_radius);
    return Rect::new_rect(center.x - max_radius, center.y - max_radius, 
                         center.x + max_radius, center.y + max_radius);
}

CircleSegment CircleSegment::operator+(const Vec2& v) const {
    return CircleSegment(center + v, outer_radius, inner_radius, start_angle, sweep_angle);
}

CircleSegment& CircleSegment::operator+=(const Vec2& v) {
    center += v;
    return *this;
}

CircleSegment CircleSegment::operator-(const Vec2& v) const {
    return CircleSegment(center - v, outer_radius, inner_radius, start_angle, sweep_angle);
}

CircleSegment& CircleSegment::operator-=(const Vec2& v) {
    center -= v;
    return *this;
}

CircleSegment CircleSegment::zero() {
    return CircleSegment(Point::zero(), 0.0, 0.0, 0.0, 0.0);
}

// Shape implementation for Circle
std::vector<PathEl> Circle::path_elements(double tolerance) const {
    // Approximate circle with cubic Bézier curves
    // For a circle, we use 4 cubic Bézier curves
    const double k = 0.5522848; // Magic number for circle approximation
    double r = radius;
    
    std::vector<PathEl> elements;
    elements.push_back(PathEl(PathElType::MoveTo, Point(center.x + r, center.y)));
    
    // Top right quadrant
    elements.push_back(PathEl(PathElType::CurveTo,
        Point(center.x + r, center.y + k * r),
        Point(center.x + k * r, center.y + r),
        Point(center.x, center.y + r)
    ));
    
    // Top left quadrant
    elements.push_back(PathEl(PathElType::CurveTo,
        Point(center.x - k * r, center.y + r),
        Point(center.x - r, center.y + k * r),
        Point(center.x - r, center.y)
    ));
    
    // Bottom left quadrant
    elements.push_back(PathEl(PathElType::CurveTo,
        Point(center.x - r, center.y - k * r),
        Point(center.x - k * r, center.y - r),
        Point(center.x, center.y - r)
    ));
    
    // Bottom right quadrant
    elements.push_back(PathEl(PathElType::CurveTo,
        Point(center.x + k * r, center.y - r),
        Point(center.x + r, center.y - k * r),
        Point(center.x + r, center.y)
    ));
    
    elements.push_back(PathEl(PathElType::ClosePath));
    return elements;
}

} // namespace kurbo 