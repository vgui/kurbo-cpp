#pragma once

namespace kurbo {

class Point;
class Size;
class RoundedRectRadii;

/// A 2D rounded rectangle.
class RoundedRect {
public:
    /// Create a rounded rectangle from origin, size and radii.
    static RoundedRect from_origin_size(const Point& origin, const Size& size, const RoundedRectRadii& radii);
};

} // namespace kurbo 