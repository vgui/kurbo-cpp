#include <iostream>
#include <iomanip>
#include "kurbo/rect.hpp"
#include "kurbo/point.hpp"
#include "kurbo/size.hpp"
#include "kurbo/affine.hpp"
#include "kurbo/vec2.hpp"

using namespace kurbo;

int main() {
    std::cout << "=== Rect Example ===" << std::endl;
    
    // Create rectangles in different ways
    Rect rect1(0, 0, 100, 50);  // x0, y0, x1, y1
    Rect rect2 = Rect::from_origin_size(Point(10, 10), Size(80, 40));
    Rect rect3 = Rect::from_origin_size(Point(20, 20), Size(60, 30));
    
    std::cout << "Rect1: " << rect1 << std::endl;
    std::cout << "Rect2: " << rect2 << std::endl;
    std::cout << "Rect3: " << rect3 << std::endl;
    
    // Basic properties
    std::cout << "\n--- Basic Properties ---" << std::endl;
    std::cout << "Rect1 origin: " << rect1.origin() << std::endl;
    std::cout << "Rect1 size: " << rect1.size() << std::endl;
    std::cout << "Rect1 width: " << rect1.width() << std::endl;
    std::cout << "Rect1 height: " << rect1.height() << std::endl;
    std::cout << "Rect1 area: " << rect1.area() << std::endl;
    
    // Corners
    std::cout << "\n--- Corners ---" << std::endl;
    std::cout << "Top-left: (" << rect1.min_x() << ", " << rect1.min_y() << ")" << std::endl;
    std::cout << "Top-right: (" << rect1.max_x() << ", " << rect1.min_y() << ")" << std::endl;
    std::cout << "Bottom-left: (" << rect1.min_x() << ", " << rect1.max_y() << ")" << std::endl;
    std::cout << "Bottom-right: (" << rect1.max_x() << ", " << rect1.max_y() << ")" << std::endl;
    
    // Center
    std::cout << "\n--- Center ---" << std::endl;
    std::cout << "Center: " << rect1.center() << std::endl;
    
    // Contains point
    std::cout << "\n--- Point Containment ---" << std::endl;
    Point inside(50, 25);
    Point outside(150, 75);
    std::cout << "Point " << inside << " inside rect1: " << (rect1.contains(inside) ? "true" : "false") << std::endl;
    std::cout << "Point " << outside << " inside rect1: " << (rect1.contains(outside) ? "true" : "false") << std::endl;
    
    // Intersection
    std::cout << "\n--- Intersection ---" << std::endl;
    Rect rect4(25, 25, 125, 75);
    std::cout << "Rect4: " << rect4 << std::endl;
    
    Rect intersection = rect1.intersect(rect4);
    if (intersection.width() > 0 && intersection.height() > 0) {
        std::cout << "Intersection: " << intersection << std::endl;
    } else {
        std::cout << "No intersection" << std::endl;
    }
    
    // Union
    std::cout << "\n--- Union ---" << std::endl;
    Rect union_rect = rect1.union_rect(rect4);
    std::cout << "Union: " << union_rect << std::endl;
    
    // Inset
    std::cout << "\n--- Inset ---" << std::endl;
    Rect inset_rect = rect1.inset(10);
    std::cout << "Inset by 10: " << inset_rect << std::endl;
    
    // Expand (using inflate)
    std::cout << "\n--- Expand ---" << std::endl;
    Rect expanded_rect = rect1.inflate(20, 20);
    std::cout << "Expanded by 20: " << expanded_rect << std::endl;
    
    // Transform
    std::cout << "\n--- Transform ---" << std::endl;
    Affine transform = Affine::translate(Vec2(50, 25)) * Affine::scale(2.0);
    // Note: Rect doesn't have transform method, so we'll show the transform matrix
    std::cout << "Transform matrix: " << transform << std::endl;
    std::cout << "Original rect: " << rect1 << std::endl;
    
    return 0;
} 