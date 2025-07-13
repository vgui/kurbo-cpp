#include "kurbo/kurbo.hpp"
#include <iostream>
#include <iomanip>

using namespace kurbo;

int main() {
    std::cout << "=== Triangle Example ===" << std::endl;
    
    // Create triangles
    Triangle triangle1 = Triangle::new_triangle(Point{0.0, 0.0}, Point{3.0, 0.0}, Point{1.5, 2.0});
    Triangle triangle2 = Triangle::from_coords(1.0, 1.0, 4.0, 1.0, 2.5, 3.0);
    
    std::cout << "Triangle 1: (" << triangle1.a.x << ", " << triangle1.a.y << "), (" 
              << triangle1.b.x << ", " << triangle1.b.y << "), (" 
              << triangle1.c.x << ", " << triangle1.c.y << ")" << std::endl;
    std::cout << "Triangle 2: (" << triangle2.a.x << ", " << triangle2.a.y << "), (" 
              << triangle2.b.x << ", " << triangle2.b.y << "), (" 
              << triangle2.c.x << ", " << triangle2.c.y << ")" << std::endl;
    
    // Basic properties
    Point centroid = triangle1.centroid();
    std::cout << "\nTriangle 1 centroid: (" << centroid.x << ", " << centroid.y << ")" << std::endl;
    std::cout << "Triangle 1 area: " << triangle1.area() << std::endl;
    std::cout << "Triangle 1 perimeter: " << triangle1.perimeter(1e-9) << std::endl;
    std::cout << "Triangle 1 is zero area: " << (triangle1.is_zero_area() ? "true" : "false") << std::endl;
    
    // Winding test
    Point inside{1.5, 0.5};
    Point outside{5.0, 5.0};
    Point on_boundary{1.5, 0.0};
    
    std::cout << "Point (" << inside.x << ", " << inside.y << ") winding: " << triangle1.winding(inside) << std::endl;
    std::cout << "Point (" << outside.x << ", " << outside.y << ") winding: " << triangle1.winding(outside) << std::endl;
    std::cout << "Point (" << on_boundary.x << ", " << on_boundary.y << ") winding: " << triangle1.winding(on_boundary) << std::endl;
    
    // Circles
    Circle inscribed = triangle1.inscribed_circle();
    Circle circumscribed = triangle1.circumscribed_circle();
    
    std::cout << "Inscribed circle: center (" << inscribed.center.x << ", " << inscribed.center.y 
              << "), radius " << inscribed.radius << std::endl;
    std::cout << "Circumscribed circle: center (" << circumscribed.center.x << ", " << circumscribed.center.y 
              << "), radius " << circumscribed.radius << std::endl;
    
    // Bounding box
    Rect bbox = triangle1.bounding_box();
    std::cout << "Bounding box: (" << bbox.x0 << ", " << bbox.y0 << ") to (" 
              << bbox.x1 << ", " << bbox.y1 << ")" << std::endl;
    
    // Transformations
    Triangle translated = triangle1 + Vec2{2.0, 2.0};
    std::cout << "Translated triangle: (" << translated.a.x << ", " << translated.a.y << "), (" 
              << translated.b.x << ", " << translated.b.y << "), (" 
              << translated.c.x << ", " << translated.c.y << ")" << std::endl;
    
    // Equilateral triangle
    Triangle equilateral = Triangle::EQUILATERAL;
    std::cout << "\nEquilateral triangle area: " << equilateral.area() << std::endl;
    std::cout << "Equilateral triangle perimeter: " << equilateral.perimeter(1e-9) << std::endl;
    
    return 0;
} 