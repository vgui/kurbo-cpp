#include "kurbo/path_el.hpp"
#include "kurbo/affine.hpp"

namespace kurbo {

PathEl::PathEl(PathElType type, const Point& point) 
    : type(type), point(point), point2(0, 0), point3(0, 0) {}

PathEl::PathEl(PathElType type, const Point& point, const Point& point2) 
    : type(type), point(point), point2(point2), point3(0, 0) {}

PathEl::PathEl(PathElType type, const Point& point, const Point& point2, const Point& point3) 
    : type(type), point(point), point2(point2), point3(point3) {}

PathEl::PathEl(PathElType type) 
    : type(type), point(0, 0), point2(0, 0), point3(0, 0) {}

// Stream operator
std::ostream& operator<<(std::ostream& os, const PathEl& el) {
    switch (el.type) {
        case PathElType::MoveTo:
            os << "MoveTo(" << el.point << ")";
            break;
        case PathElType::LineTo:
            os << "LineTo(" << el.point << ")";
            break;
        case PathElType::QuadTo:
            os << "QuadTo(" << el.point << ", " << el.point2 << ")";
            break;
        case PathElType::CurveTo:
            os << "CurveTo(" << el.point << ", " << el.point2 << ", " << el.point3 << ")";
            break;
        case PathElType::ClosePath:
            os << "ClosePath()";
            break;
    }
    return os;
}

bool PathEl::is_finite() const {
    switch (type) {
        case PathElType::MoveTo:
        case PathElType::LineTo:
            return point.is_finite();
        case PathElType::QuadTo:
            return point.is_finite() && point2.is_finite();
        case PathElType::CurveTo:
            return point.is_finite() && point2.is_finite() && point3.is_finite();
        case PathElType::ClosePath:
            return true;
    }
    return false;
}

bool PathEl::is_nan() const {
    switch (type) {
        case PathElType::MoveTo:
        case PathElType::LineTo:
            return point.is_nan();
        case PathElType::QuadTo:
            return point.is_nan() || point2.is_nan();
        case PathElType::CurveTo:
            return point.is_nan() || point2.is_nan() || point3.is_nan();
        case PathElType::ClosePath:
            return false;
    }
    return false;
}

std::optional<Point> PathEl::end_point() const {
    switch (type) {
        case PathElType::MoveTo:
        case PathElType::LineTo:
            return point;
        case PathElType::QuadTo:
            return point2;
        case PathElType::CurveTo:
            return point3;
        case PathElType::ClosePath:
            return std::nullopt;
    }
    return std::nullopt;
}

PathEl operator*(const Affine& affine, const PathEl& el) {
    switch (el.type) {
        case PathElType::MoveTo:
        case PathElType::LineTo:
            return PathEl(el.type, affine * el.point);
        case PathElType::QuadTo:
            return PathEl(el.type, affine * el.point, affine * el.point2);
        case PathElType::CurveTo:
            return PathEl(el.type, affine * el.point, affine * el.point2, affine * el.point3);
        case PathElType::ClosePath:
            return PathEl(el.type);
    }
    return PathEl(el.type); // fallback, should not reach
}

} // namespace kurbo 