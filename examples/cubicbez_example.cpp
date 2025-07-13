#include <iostream>
#include <iomanip>
#include "kurbo/cubicbez.hpp"
#include "kurbo/point.hpp"
#include "kurbo/vec2.hpp"
#include "kurbo/affine.hpp"

using namespace kurbo;

int main() {
    std::cout << "=== CubicBez Example ===" << std::endl;
    
    // Create different cubic Bézier curves
    CubicBez cubic1 = CubicBez::new_curve(
        Point(0, 0),    // p0
        Point(1, 2),    // p1
        Point(2, 2),    // p2
        Point(3, 0)     // p3
    );
    
    CubicBez cubic2 = CubicBez::new_curve(
        Point(0, 0),    // p0
        Point(3, 3),    // p1
        Point(0, 3),    // p2
        Point(3, 0)     // p3
    );
    
    CubicBez cubic3 = CubicBez::new_curve(
        Point(0, 0),    // p0
        Point(1, 1),    // p1
        Point(2, 1),    // p2
        Point(3, 0)     // p3
    );
    
    std::cout << "CubicBez1: " << cubic1 << std::endl;
    std::cout << "CubicBez2: " << cubic2 << std::endl;
    std::cout << "CubicBez3: " << cubic3 << std::endl;
    
    // Basic properties
    std::cout << "\n--- Basic Properties ---" << std::endl;
    std::cout << "CubicBez1 start: " << cubic1.p0 << std::endl;
    std::cout << "CubicBez1 end: " << cubic1.p3 << std::endl;
    std::cout << "CubicBez1 is finite: " << (cubic1.is_finite() ? "true" : "false") << std::endl;
    std::cout << "CubicBez1 is NaN: " << (cubic1.is_nan() ? "true" : "false") << std::endl;
    
    // Subdivision
    std::cout << "\n--- Subdivision ---" << std::endl;
    auto [left1, right1] = cubic1.subdivide_concrete();
    std::cout << "Left half: start=" << left1.p0 << ", end=" << left1.p3 << std::endl;
    std::cout << "Right half: start=" << right1.p0 << ", end=" << right1.p3 << std::endl;
    
    // Subdivision into three parts
    std::cout << "\n--- Subdivision into Three Parts ---" << std::endl;
    auto [left2, mid2, right2] = cubic2.subdivide_3();
    std::cout << "Left third: start=" << left2.p0 << ", end=" << left2.p3 << std::endl;
    std::cout << "Middle third: start=" << mid2.p0 << ", end=" << mid2.p3 << std::endl;
    std::cout << "Right third: start=" << right2.p0 << ", end=" << right2.p3 << std::endl;
    
    // Utility methods
    std::cout << "\n--- Utility Methods ---" << std::endl;
    auto inflections = cubic1.inflections();
    std::cout << "Inflection points: " << inflections.size() << std::endl;
    
    auto tangents = cubic1.tangents_to_point(Point(1.5, 1.0));
    std::cout << "Tangents to point (1.5, 1.0): " << tangents.size() << std::endl;
    
    // Conversion to quadratics
    std::cout << "\n--- Conversion to Quadratics ---" << std::endl;
    auto quads = cubic1.to_quads(0.1);
    std::cout << "Number of quadratic segments: " << quads.size() << std::endl;
    for (size_t i = 0; i < quads.size(); ++i) {
        std::cout << "  Segment " << i << ": t=[" << quads[i].first << ", " << quads[i].second << "]" << std::endl;
    }
    
    // Approximation to spline
    std::cout << "\n--- Approximation to Spline ---" << std::endl;
    std::cout << "Spline approximation not implemented yet (requires QuadSpline)" << std::endl;
    
    // Transformations
    std::cout << "\n--- Transformations ---" << std::endl;
    Affine scale = Affine::scale(2.0);
    Affine translate = Affine::translate(Vec2(10, 20));
    Affine transform = translate * scale;
    
    CubicBez transformed = transform * cubic1;
    std::cout << "Original: " << cubic1 << std::endl;
    std::cout << "Transformed: " << transformed << std::endl;
    
    // Operators
    std::cout << "\n--- Operators ---" << std::endl;
    Vec2 offset(5, 10);
    CubicBez translated = cubic1 + offset;
    std::cout << "Translated by " << offset << ": " << translated << std::endl;
    
    CubicBez scaled_down = cubic1 - offset;
    std::cout << "Subtracted " << offset << ": " << scaled_down << std::endl;
    
    // Compound assignment
    CubicBez modified = cubic1;
    modified += offset;
    std::cout << "Modified += " << offset << ": " << modified << std::endl;
    
    modified = cubic1;
    modified -= offset;
    std::cout << "Modified -= " << offset << ": " << modified << std::endl;
    
    // Special cases
    std::cout << "\n--- Special Cases ---" << std::endl;
    CubicBez zero = CubicBez::zero();
    std::cout << "Zero curve: " << zero << std::endl;
    
    // Linear curve (control points on a line)
    CubicBez linear = CubicBez::new_curve(
        Point(0, 0),
        Point(1, 1),
        Point(2, 2),
        Point(3, 3)
    );
    std::cout << "Linear curve: " << linear << std::endl;
    
    return 0;
} 