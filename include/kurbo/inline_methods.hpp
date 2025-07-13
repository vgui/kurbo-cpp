#pragma once

#include "kurbo/point.hpp"
#include "kurbo/vec2.hpp"
#include "kurbo/size.hpp"

namespace kurbo {

// Inline methods that require full type definitions
inline constexpr Vec2 Point::to_vec2() const { return Vec2(x, y); }
inline constexpr Point Vec2::to_point() const { return Point(x, y); }
inline constexpr Size Vec2::to_size() const { return Size(x, y); }
inline constexpr Vec2 Size::to_vec2() const { return Vec2(width, height); }

} // namespace kurbo 