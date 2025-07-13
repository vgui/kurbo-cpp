#include "kurbo/line.hpp"
#include "kurbo/inline_methods.hpp"
#include <cmath>

namespace kurbo {

// Constructors are now inline in header

Line Line::reversed() const {
    return Line(p1, p0);
}

double Line::length() const {
    return arclen(1e-9); // DEFAULT_ACCURACY
}

Point Line::midpoint() const {
    return p0.midpoint(p1);
}

std::optional<Point> Line::crossing_point(const Line& other) const {
    Vec2 ab = p1 - p0;
    Vec2 cd = other.p1 - other.p0;
    double pcd = ab.cross(cd);
    if (pcd == 0.0) {
        return std::nullopt;
    }
    double h = ab.cross(p0 - other.p0) / pcd;
    return other.p0 + cd * h;
}

bool Line::is_finite() const {
    return p0.is_finite() && p1.is_finite();
}

bool Line::is_nan() const {
    return p0.is_nan() || p1.is_nan();
}

Point Line::eval(double t) const {
    return p0.lerp(p1, t);
}

ParamCurve* Line::subsegment(double start, double end) const {
    return new Line(eval(start), eval(end));
}

std::pair<ParamCurve*, ParamCurve*> Line::subdivide() const {
    Point mid = midpoint();
    auto first = new Line(p0, mid);
    auto second = new Line(mid, p1);
    return {first, second};
}

Point Line::start() const {
    return p0;
}

Point Line::end() const {
    return p1;
}

ParamCurveDeriv* Line::deriv() const {
    // The derivative of a line is a constant vector
    // We return a Line representing the derivative vector
    Vec2 derivative = p1 - p0;
    return new Line(Point(0, 0), Point(derivative.x, derivative.y));
}

double Line::arclen(double accuracy) const {
    return (p1 - p0).hypot();
}

double Line::signed_area() const {
    return p0.to_vec2().cross(p1.to_vec2()) * 0.5;
}

Nearest Line::nearest(const Point& p, double accuracy) const {
    Vec2 line_vec = p1 - p0;
    Vec2 point_vec = p - p0;
    
    double t = point_vec.dot(line_vec) / line_vec.dot(line_vec);
    t = std::max(0.0, std::min(1.0, t));
    
    Point nearest_pt = eval(t);
    double distance_sq = p.distance_squared(nearest_pt);
    
    return {distance_sq, t};
}

std::vector<double> Line::extrema() const {
    // Lines have no extrema in their interior
    return {};
}

Line Line::operator+(const Vec2& v) const {
    return Line(p0 + v, p1 + v);
}

Line& Line::operator+=(const Vec2& v) {
    p0 += v;
    p1 += v;
    return *this;
}

Line Line::operator-(const Vec2& v) const {
    return Line(p0 - v, p1 - v);
}

Line& Line::operator-=(const Vec2& v) {
    p0 -= v;
    p1 -= v;
    return *this;
}

Line Line::operator*(const Affine& affine) const {
    return Line(affine * p0, affine * p1);
}

Line& Line::operator*=(const Affine& affine) {
    p0 = affine * p0;
    p1 = affine * p1;
    return *this;
}

Line Line::zero() {
    return Line(Point::zero(), Point::zero());
}

Line Line::transform(const Affine& affine) const {
    return Line(affine * p0, affine * p1);
}

bool Line::is_linear() const {
    return true; // Lines are always linear
}

std::ostream& operator<<(std::ostream& os, const Line& line) {
    os << "Line(" << line.p0 << ", " << line.p1 << ")";
    return os;
}

bool operator==(const Line& a, const Line& b) {
    return a.p0 == b.p0 && a.p1 == b.p1;
}

// ConstPoint implementation
// Constructor is now inline in header

bool ConstPoint::is_finite() const {
    return point.is_finite();
}

bool ConstPoint::is_nan() const {
    return point.is_nan();
}

Point ConstPoint::eval(double t) const {
    return point;
}

ConstPoint ConstPoint::subsegment(double start_t, double end_t) const {
    return *this;
}

Point ConstPoint::start() const {
    return point;
}

Point ConstPoint::end() const {
    return point;
}

double ConstPoint::arclen(double accuracy) const {
    return 0.0;
}

double ConstPoint::inv_arclen(double arclen, double accuracy) const {
    return 0.0;
}

double ConstPoint::signed_area() const {
    return 0.0;
}

double ConstPoint::curvature(double t) const {
    return 0.0;
}

ConstPoint ConstPoint::zero() {
    return ConstPoint(Point::zero());
}

int Line::winding(const Point& pt) const {
    // Lines have no winding number - they are not closed shapes
    return 0;
}

Rect Line::bounding_box() const {
    double x0 = std::min(p0.x, p1.x);
    double y0 = std::min(p0.y, p1.y);
    double x1 = std::max(p0.x, p1.x);
    double y1 = std::max(p0.y, p1.y);
    return Rect(x0, y0, x1, y1);
}

// Shape implementation
std::vector<PathEl> Line::path_elements(double tolerance) const {
    return {PathEl(PathElType::MoveTo, p0), PathEl(PathElType::LineTo, p1)};
}

double Line::area() const {
    // Lines have zero area
    return 0.0;
}

double Line::perimeter(double accuracy) const {
    return length();
}

std::optional<Line> Line::as_line() const {
    return *this;
}

bool Line::contains(const Point& pt) const {
    // Check if point is on the line segment
    Vec2 line_vec = p1 - p0;
    Vec2 point_vec = pt - p0;
    
    // Check if point is collinear with line
    double cross_product = line_vec.cross(point_vec);
    if (std::abs(cross_product) > 1e-12) {
        return false; // Point is not on the line
    }
    
    // Check if point is within the segment bounds
    double t = point_vec.dot(line_vec) / line_vec.dot(line_vec);
    return t >= 0.0 && t <= 1.0;
}

} // namespace kurbo 