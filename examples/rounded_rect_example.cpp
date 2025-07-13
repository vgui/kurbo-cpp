#include <iostream>
#include <iomanip>
#include "kurbo/rounded_rect.hpp"
#include "kurbo/rect.hpp"
#include "kurbo/point.hpp"
#include "kurbo/rounded_rect_radii.hpp"
#include "kurbo/size.hpp"
#include "kurbo/affine.hpp"
#include "kurbo/vec2.hpp"

using namespace kurbo;

int main() {
    std::cout << "=== RoundedRect Example ===" << std::endl;
    
    // Create rounded rectangles in different ways
    Rect base_rect(0, 0, 100, 60);  // x0, y0, x1, y1
    RoundedRect rounded1(base_rect, 10.0);
    RoundedRect rounded2(base_rect, RoundedRectRadii(15.0, 20.0, 25.0, 30.0));
    RoundedRect rounded3 = RoundedRect::from_origin_size(Point(10, 10), Size(80, 40), 5.0);
    
    std::cout << "RoundedRect1: " << rounded1 << std::endl;
    std::cout << "RoundedRect2: " << rounded2 << std::endl;
    std::cout << "RoundedRect3: " << rounded3 << std::endl;
    
    // Basic properties
    std::cout << "\n--- Basic Properties ---" << std::endl;
    std::cout << "RoundedRect1 rect: " << rounded1.get_rect() << std::endl;
    std::cout << "RoundedRect1 radii: " << rounded1.get_radii() << std::endl;
    std::cout << "RoundedRect1 origin: " << rounded1.origin() << std::endl;
    std::cout << "RoundedRect1 width: " << rounded1.width() << std::endl;
    std::cout << "RoundedRect1 height: " << rounded1.height() << std::endl;
    
    // Radii properties
    std::cout << "\n--- Radii Properties ---" << std::endl;
    auto radii = rounded2.get_radii();
    std::cout << "Top-left radius: " << radii.top_left << std::endl;
    std::cout << "Top-right radius: " << radii.top_right << std::endl;
    std::cout << "Bottom-left radius: " << radii.bottom_left << std::endl;
    std::cout << "Bottom-right radius: " << radii.bottom_right << std::endl;
    
    // Contains point (using the underlying rect)
    std::cout << "\n--- Point Containment ---" << std::endl;
    Point inside(50, 30);
    Point outside(150, 75);
    std::cout << "Point " << inside << " inside rounded1 rect: " << (rounded1.get_rect().contains(inside) ? "true" : "false") << std::endl;
    std::cout << "Point " << outside << " inside rounded1 rect: " << (rounded1.get_rect().contains(outside) ? "true" : "false") << std::endl;
    
    // Note: inset and expand methods don't exist in our implementation
    std::cout << "\n--- Note: inset and expand methods not implemented ---" << std::endl;
    
    // Transform (using the underlying rect)
    std::cout << "\n--- Transform ---" << std::endl;
    Affine transform = Affine::translate(Vec2(50, 30)) * Affine::scale(1.5);
    std::cout << "Transform matrix: " << transform << std::endl;
    std::cout << "Original rounded rect: " << rounded1 << std::endl;
    
    // Different radius configurations
    std::cout << "\n--- Different Radius Configurations ---" << std::endl;
    RoundedRectRadii uniform_radii(10.0);
    RoundedRectRadii different_radii(5.0, 15.0, 20.0, 25.0);
    RoundedRectRadii horizontal_radii(10.0, 10.0, 5.0, 5.0);
    RoundedRectRadii vertical_radii(5.0, 10.0, 10.0, 5.0);
    
    std::cout << "Uniform radii: " << uniform_radii << std::endl;
    std::cout << "Different radii: " << different_radii << std::endl;
    std::cout << "Horizontal radii: " << horizontal_radii << std::endl;
    std::cout << "Vertical radii: " << vertical_radii << std::endl;
    
    // Create rounded rectangles with different radius configurations
    RoundedRect uniform_rounded(base_rect, uniform_radii);
    RoundedRect different_rounded(base_rect, different_radii);
    RoundedRect horizontal_rounded(base_rect, horizontal_radii);
    RoundedRect vertical_rounded(base_rect, vertical_radii);
    
    std::cout << "Uniform rounded: " << uniform_rounded << std::endl;
    std::cout << "Different rounded: " << different_rounded << std::endl;
    std::cout << "Horizontal rounded: " << horizontal_rounded << std::endl;
    std::cout << "Vertical rounded: " << vertical_rounded << std::endl;
    
    return 0;
} 