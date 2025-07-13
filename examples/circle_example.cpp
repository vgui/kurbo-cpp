#include "kurbo/kurbo.hpp"
#include <iostream>
#include <iomanip>

using namespace kurbo;

int main() {
    std::cout << "=== Circle Example ===" << std::endl;
    
    // Create circles
    Circle circle1 = Circle::new_circle(Point{0.0, 0.0}, 5.0);
    Circle circle2 = Circle::new_circle(Point{3.0, 4.0}, 2.0);
    
    std::cout << "Circle 1: center (" << circle1.center.x << ", " << circle1.center.y 
              << "), radius " << circle1.radius << std::endl;
    std::cout << "Circle 2: center (" << circle2.center.x << ", " << circle2.center.y 
              << "), radius " << circle2.radius << std::endl;
    
    // Basic properties
    std::cout << "\nCircle 1 area: " << circle1.area() << std::endl;
    std::cout << "Circle 1 perimeter: " << circle1.perimeter(1e-9) << std::endl;
    
    // Winding test
    Point inside{1.0, 1.0};
    Point outside{10.0, 10.0};
    Point on_boundary{5.0, 0.0};
    
    std::cout << "Point (" << inside.x << ", " << inside.y << ") winding: " << circle1.winding(inside) << std::endl;
    std::cout << "Point (" << outside.x << ", " << outside.y << ") winding: " << circle1.winding(outside) << std::endl;
    std::cout << "Point (" << on_boundary.x << ", " << on_boundary.y << ") winding: " << circle1.winding(on_boundary) << std::endl;
    
    // Bounding box
    Rect bbox = circle1.bounding_box();
    std::cout << "Bounding box: (" << bbox.x0 << ", " << bbox.y0 << ") to (" 
              << bbox.x1 << ", " << bbox.y1 << ")" << std::endl;
    
    // Transformations
    Circle translated = circle1 + Vec2{2.0, 2.0};
    std::cout << "Translated circle: center (" << translated.center.x << ", " << translated.center.y 
              << "), radius " << translated.radius << std::endl;
    
    // Circle segments
    CircleSegment segment = CircleSegment::new_segment(Point{0.0, 0.0}, 5.0, 2.0, 0.0, M_PI / 2.0);
    std::cout << "\nCircle segment area: " << segment.area() << std::endl;
    std::cout << "Circle segment perimeter: " << segment.perimeter(1e-9) << std::endl;
    
    return 0;
} 