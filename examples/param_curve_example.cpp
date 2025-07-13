#include <iostream>
#include <iomanip>
#include "kurbo/param_curve.hpp"
#include "kurbo/line.hpp"
#include "kurbo/quadbez.hpp"
#include "kurbo/point.hpp"
#include "kurbo/affine.hpp"

using namespace kurbo;

int main() {
    std::cout << "=== ParamCurve Example ===" << std::endl;
    
    // Create different types of curves
    Line line(Point(0, 0), Point(100, 50));
    QuadBez quad(Point(0, 0), Point(50, 100), Point(100, 0));
    
    std::cout << "Line: " << line << std::endl;
    std::cout << "QuadBez: " << quad << std::endl;
    
    // Evaluate curves at different parameters
    std::cout << "\n--- Curve Evaluation ---" << std::endl;
    for (double t = 0.0; t <= 1.0; t += 0.25) {
        Point line_point = line.eval(t);
        Point quad_point = quad.eval(t);
        std::cout << "t=" << t << ": Line(" << line_point << ") Quad(" << quad_point << ")" << std::endl;
    }
    
    // Arc length
    std::cout << "\n--- Arc Length ---" << std::endl;
    double line_arclen = line.arclen(1e-9);
    double quad_arclen = quad.arclen(1e-9);
    std::cout << "Line arc length: " << line_arclen << std::endl;
    std::cout << "Quad arc length: " << quad_arclen << std::endl;
    
    // Parameter from arc length
    std::cout << "\n--- Parameter from Arc Length ---" << std::endl;
    double line_t = line.inv_arclen(line_arclen / 2, 1e-9);
    double quad_t = quad.inv_arclen(quad_arclen / 2, 1e-9);
    std::cout << "Line t at half arc length: " << line_t << std::endl;
    std::cout << "Quad t at half arc length: " << quad_t << std::endl;
    
    // Subdivision
    std::cout << "\n--- Subdivision ---" << std::endl;
    auto [line1, line2] = line.subdivide();
    auto [quad1, quad2] = quad.subdivide();
    
    std::cout << "Line subdivision:" << std::endl;
    std::cout << "  First half: start=" << line1->start() << ", end=" << line1->end() << std::endl;
    std::cout << "  Second half: start=" << line2->start() << ", end=" << line2->end() << std::endl;
    
    std::cout << "Quad subdivision:" << std::endl;
    std::cout << "  First half: start=" << quad1->start() << ", end=" << quad1->end() << std::endl;
    std::cout << "  Second half: start=" << quad2->start() << ", end=" << quad2->end() << std::endl;
    
    // Nearest point
    std::cout << "\n--- Nearest Point ---" << std::endl;
    Point test_point(50, 25);
    Nearest line_nearest_result = line.nearest(test_point, 1e-9);
    Nearest quad_nearest_result = quad.nearest(test_point, 1e-9);
    Point line_nearest = line.eval(line_nearest_result.t);
    Point quad_nearest = quad.eval(quad_nearest_result.t);
    
    std::cout << "Test point: " << test_point << std::endl;
    std::cout << "Line nearest t: " << line_nearest_result.t << ", distance: " << std::sqrt(line_nearest_result.distance_sq) << ", point: " << line_nearest << std::endl;
    std::cout << "Quad nearest t: " << quad_nearest_result.t << ", distance: " << std::sqrt(quad_nearest_result.distance_sq) << ", point: " << quad_nearest << std::endl;
    
    // Transform curves
    std::cout << "\n--- Transform Curves ---" << std::endl;
    Affine transform = Affine::translate(Vec2(10, 20)) * Affine::scale(1.5);
    Line transformed_line = line.transform(transform);
    QuadBez transformed_quad = quad.transform(transform);
    
    std::cout << "Transformed line: " << transformed_line << std::endl;
    std::cout << "Transformed quad: " << transformed_quad << std::endl;
    
    // Curve properties
    std::cout << "\n--- Curve Properties ---" << std::endl;
    std::cout << "Line is linear: " << (line.is_linear() ? "true" : "false") << std::endl;
    std::cout << "Quad is linear: " << (quad.is_linear() ? "true" : "false") << std::endl;
    
    // Start and end points
    std::cout << "\n--- Start and End Points ---" << std::endl;
    std::cout << "Line start: " << line.start() << ", end: " << line.end() << std::endl;
    std::cout << "Quad start: " << quad.start() << ", end: " << quad.end() << std::endl;
    
    // Clean up allocated memory
    delete line1;
    delete line2;
    delete quad1;
    delete quad2;
    
    return 0;
} 