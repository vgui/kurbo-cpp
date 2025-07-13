#pragma once

// Main kurbo library header
// This file includes all public headers in dependency order

// Basic types (most fundamental)
#include "point.hpp"
#include "vec2.hpp"
#include "size.hpp"
#include "inline_methods.hpp"

// Common utilities
#include "common.hpp"

// Geometric primitives
#include "line.hpp"
#include "rect.hpp"
#include "circle.hpp"
#include "ellipse.hpp"
#include "triangle.hpp"
#include "rounded_rect.hpp"
#include "rounded_rect_radii.hpp"

// Curves and paths
#include "cubicbez.hpp"
#include "quadbez.hpp"
#include "quadspline.hpp"
#include "path_el.hpp"
// #include "arc.hpp"
// #include "bezpath.hpp"

// Transformations
#include "affine.hpp"
#include "translate_scale.hpp"

// Advanced operations
#include "param_curve.hpp"
// #include "shape.hpp"
// #include "offset.hpp"
// #include "simplify.hpp"
// #include "stroke.hpp"
// #include "mindist.hpp"
// #include "fit.hpp"
// #include "insets.hpp"

// SVG support
// #include "svg.hpp" 