#pragma once

// RoundedRectRadii class (radii for rounded rectangle corners)
// Ported from kurbo::RoundedRectRadii (Rust)
// Copyright 2024
//
// This class represents the radii for the four corners of a rounded rectangle.

#include <cmath>
#include <optional>

namespace kurbo {

/// Radii for rounded rectangle corners.
class RoundedRectRadii {
public:
    // Corner radii
    double top_left;
    double top_right;
    double bottom_right;
    double bottom_left;

    /// Create radii with uniform values.
    constexpr RoundedRectRadii(double radius) 
        : top_left(radius), top_right(radius), bottom_right(radius), bottom_left(radius) {}
    
    /// Create radii with different values for each corner.
    constexpr RoundedRectRadii(double top_left, double top_right, double bottom_right, double bottom_left)
        : top_left(top_left), top_right(top_right), bottom_right(bottom_right), bottom_left(bottom_left) {}

    // Static constructors
    static constexpr RoundedRectRadii new_radii(double top_left, double top_right, double bottom_right, double bottom_left) { return RoundedRectRadii(top_left, top_right, bottom_right, bottom_left); }
    static constexpr RoundedRectRadii from_single_radius(double radius) { return RoundedRectRadii(radius); }

    // Utility methods
    bool is_finite() const;
    bool is_nan() const;
    RoundedRectRadii clamp(double max_radius) const;
    RoundedRectRadii abs() const;
    std::optional<double> as_single_radius() const;

    // Static constructors
    static RoundedRectRadii zero();
};

} // namespace kurbo 