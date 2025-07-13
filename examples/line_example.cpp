#include "kurbo/kurbo.hpp"
#include <iostream>
#include <iomanip>

using namespace kurbo;

int main() {
    std::cout << "=== Line Example ===" << std::endl;
    
    // Create lines
    Line line1 = Line::new_line(Point{0.0, 0.0}, Point{3.0, 4.0});
    Line line2 = Line::new_line(Point{1.0, 1.0}, Point{4.0, 5.0});
    
    std::cout << "Line 1: (" << line1.p0.x << ", " << line1.p0.y << ") to (" 
              << line1.p1.x << ", " << line1.p1.y << ")" << std::endl;
    std::cout << "Line 2: (" << line2.p0.x << ", " << line2.p0.y << ") to (" 
              << line2.p1.x << ", " << line2.p1.y << ")" << std::endl;
    
    // Basic properties
    std::cout << "\nLine 1 length: " << line1.length() << std::endl;
    std::cout << "Line 1 midpoint: (" << line1.midpoint().x << ", " << line1.midpoint().y << ")" << std::endl;
    
    // Crossing point
    auto crossing = line1.crossing_point(line2);
    if (crossing.has_value()) {
        std::cout << "Crossing point: (" << crossing->x << ", " << crossing->y << ")" << std::endl;
    } else {
        std::cout << "Lines do not cross" << std::endl;
    }
    
    // Parametric evaluation
    Point eval_point = line1.eval(0.5);
    std::cout << "Line 1 at t=0.5: (" << eval_point.x << ", " << eval_point.y << ")" << std::endl;
    
    // Transformations
    Line translated = line1 + Vec2{2.0, 2.0};
    std::cout << "Translated line: (" << translated.p0.x << ", " << translated.p0.y << ") to (" 
              << translated.p1.x << ", " << translated.p1.y << ")" << std::endl;
    
    Affine scale = Affine::scale(2.0);
    Line scaled = line1 * scale;
    std::cout << "Scaled line: (" << scaled.p0.x << ", " << scaled.p0.y << ") to (" 
              << scaled.p1.x << ", " << scaled.p1.y << ")" << std::endl;
    
    return 0;
} 