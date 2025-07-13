#pragma once

// Arc class (ellipse arc segment)
// Ported from kurbo::Arc (Rust)
// Copyright 2024
//
// This class represents a single elliptical arc segment.

#include "point.hpp"
#include "vec2.hpp"
#include "affine.hpp"
#include <vector>
#include <cmath>

namespace kurbo {

class Arc {
public:
    // Fields
    Point center;
    Vec2 radii;
    double start_angle;
    double sweep_angle;
    double x_rotation;

    // Constructors
    Arc();
    Arc(const Point& center, const Vec2& radii, double start_angle, double sweep_angle, double x_rotation);

    // Static constructor
    static Arc new_arc(const Point& center, const Vec2& radii, double start_angle, double sweep_angle, double x_rotation);

    // Reverse arc direction
    Arc reversed() const;

    // Operators
    friend Arc operator*(const Affine& affine, const Arc& arc);

    // Default
    static Arc zero();
};

// Stream operator
std::ostream& operator<<(std::ostream& os, const Arc& arc);

} // namespace kurbo 