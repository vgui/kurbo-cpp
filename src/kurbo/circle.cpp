#include "kurbo/circle.hpp"
#include "kurbo/inline_methods.hpp"
#include <cmath>

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
    if ((pt - center).hypot2() < radius * radius) {
        return 1;
    } else {
        return 0;
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

} // namespace kurbo 