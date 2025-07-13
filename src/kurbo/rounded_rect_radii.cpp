#include "kurbo/rounded_rect_radii.hpp"
#include <cmath>

namespace kurbo {

bool RoundedRectRadii::is_finite() const {
    return std::isfinite(top_left) && std::isfinite(top_right) && 
           std::isfinite(bottom_right) && std::isfinite(bottom_left);
}

bool RoundedRectRadii::is_nan() const {
    return std::isnan(top_left) || std::isnan(top_right) || 
           std::isnan(bottom_right) || std::isnan(bottom_left);
}

RoundedRectRadii RoundedRectRadii::clamp(double max_radius) const {
    return RoundedRectRadii(
        std::min(top_left, max_radius),
        std::min(top_right, max_radius),
        std::min(bottom_right, max_radius),
        std::min(bottom_left, max_radius)
    );
}

RoundedRectRadii RoundedRectRadii::abs() const {
    return RoundedRectRadii(
        std::abs(top_left),
        std::abs(top_right),
        std::abs(bottom_right),
        std::abs(bottom_left)
    );
}

std::optional<double> RoundedRectRadii::as_single_radius() const {
    const double epsilon = 1e-9;
    
    if (std::abs(top_left - top_right) < epsilon &&
        std::abs(top_right - bottom_right) < epsilon &&
        std::abs(bottom_right - bottom_left) < epsilon) {
        return top_left;
    } else {
        return std::nullopt;
    }
}

RoundedRectRadii RoundedRectRadii::zero() {
    return RoundedRectRadii(0.0);
}

} // namespace kurbo 