#pragma once

#include "point.hpp"

namespace kurbo {

/// Path element types
enum class PathElType {
    MoveTo,
    LineTo,
    QuadTo,
    CurveTo,
    ClosePath
};

/// A path element
class PathEl {
public:
    PathElType type;
    Point point;
    Point point2;  // For QuadTo and CurveTo
    Point point3;  // For CurveTo

    /// Constructor for MoveTo and LineTo
    PathEl(PathElType type, const Point& point);
    
    /// Constructor for QuadTo
    PathEl(PathElType type, const Point& point, const Point& point2);
    
    /// Constructor for CurveTo
    PathEl(PathElType type, const Point& point, const Point& point2, const Point& point3);
    
    /// Constructor for ClosePath
    PathEl(PathElType type);
    
    /// Destructor
    ~PathEl() = default;
};

// Stream operator
std::ostream& operator<<(std::ostream& os, const PathEl& el);

class Affine;
PathEl operator*(const Affine& affine, const PathEl& el);

} // namespace kurbo 