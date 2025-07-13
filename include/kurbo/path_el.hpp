#pragma once

#include "point.hpp"
#include <optional>
#include <iostream>

namespace kurbo {

/// Path element types
enum class PathElType {
    MoveTo,
    LineTo,
    QuadTo,
    CurveTo,
    ClosePath
};

/// Path element - represents a single command in a Bezier path
class PathEl {
public:
    PathElType type;
    Point point;
    Point point2;  // For QuadTo and CurveTo
    Point point3;  // For CurveTo

    // Constructors
    PathEl(PathElType type, const Point& point);
    PathEl(PathElType type, const Point& point, const Point& point2);
    PathEl(PathElType type, const Point& point, const Point& point2, const Point& point3);
    PathEl(PathElType type);
    PathEl() : type(PathElType::ClosePath), point(), point2(), point3() {} // Default constructor

    /// Copy constructor
    PathEl(const PathEl& other) = default;
    
    /// Assignment operator
    PathEl& operator=(const PathEl& other) = default;
    
    /// Destructor
    ~PathEl() = default;
    
    /// Check if all points are finite
    bool is_finite() const;
    
    /// Check if any point is NaN
    bool is_nan() const;
    
    /// Get the end point of this path element, if it exists
    std::optional<Point> end_point() const;
};

// Stream operator
std::ostream& operator<<(std::ostream& os, const PathEl& el);

class Affine;
PathEl operator*(const Affine& affine, const PathEl& el);

} // namespace kurbo 