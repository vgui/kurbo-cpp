#include "kurbo/shape.hpp"
#include "kurbo/bezpath.hpp"
#include "kurbo/point.hpp"
#include "kurbo/rect.hpp"
#include "kurbo/line.hpp"
#include "kurbo/circle.hpp"
#include "kurbo/rounded_rect.hpp"
#include "kurbo/path_el.hpp"

namespace kurbo {

BezPath Shape::to_path(double tolerance) const {
    auto elements = path_elements(tolerance);
    return BezPath(elements);
}

BezPath Shape::into_path(double tolerance) const {
    return to_path(tolerance);
}

bool Shape::contains(const Point& pt) const {
    return winding(pt) != 0;
}

std::optional<Line> Shape::as_line() const {
    return std::nullopt;
}

std::optional<Rect> Shape::as_rect() const {
    return std::nullopt;
}

std::optional<RoundedRect> Shape::as_rounded_rect() const {
    return std::nullopt;
}

std::optional<Circle> Shape::as_circle() const {
    return std::nullopt;
}

std::optional<std::vector<PathEl>> Shape::as_path_slice() const {
    return std::nullopt;
}

} // namespace kurbo 