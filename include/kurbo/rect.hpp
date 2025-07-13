#pragma once

namespace kurbo {

class Point;
class Size;

/// A 2D rectangle.
class Rect {
public:
    /// Create a rectangle from origin and size.
    static Rect from_origin_size(const Point& origin, const Size& size);
};

} // namespace kurbo 