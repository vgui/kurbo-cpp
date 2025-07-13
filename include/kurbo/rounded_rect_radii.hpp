#pragma once

namespace kurbo {

/// Radii for rounded rectangle corners.
class RoundedRectRadii {
public:
    /// Create radii with uniform values.
    constexpr RoundedRectRadii(double radius);
    
    /// Create radii with different values for each corner.
    constexpr RoundedRectRadii(double top_left, double top_right, double bottom_right, double bottom_left);
};

} // namespace kurbo 