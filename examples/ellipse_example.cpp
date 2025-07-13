#include "kurbo/kurbo.hpp"
#include <iostream>
#include <iomanip>

using namespace kurbo;

int main() {
    std::cout << "=== Ellipse Example ===" << std::endl;
    
    // Create ellipses
    Ellipse ellipse1 = Ellipse::new_ellipse(Point{0.0, 0.0}, Vec2{3.0, 4.0}, 0.0);
    Ellipse ellipse2 = Ellipse::new_ellipse(Point{2.0, 3.0}, Vec2{2.0, 5.0}, M_PI / 4.0);
    
    std::cout << "Ellipse 1: center (" << ellipse1.center().x << ", " << ellipse1.center().y 
              << "), radii (" << ellipse1.radii().x << ", " << ellipse1.radii().y 
              << "), rotation " << ellipse1.rotation() << std::endl;
    std::cout << "Ellipse 2: center (" << ellipse2.center().x << ", " << ellipse2.center().y 
              << "), radii (" << ellipse2.radii().x << ", " << ellipse2.radii().y 
              << "), rotation " << ellipse2.rotation() << std::endl;
    
    // Basic properties
    std::cout << "\nEllipse 1 area: " << ellipse1.area() << std::endl;
    std::cout << "Ellipse 1 perimeter: " << ellipse1.perimeter(1e-9) << std::endl;
    
    // Winding test
    Point inside{1.0, 1.0};
    Point outside{10.0, 10.0};
    Point on_boundary{3.0, 0.0};
    
    std::cout << "Point (" << inside.x << ", " << inside.y << ") winding: " << ellipse1.winding(inside) << std::endl;
    std::cout << "Point (" << outside.x << ", " << outside.y << ") winding: " << ellipse1.winding(outside) << std::endl;
    std::cout << "Point (" << on_boundary.x << ", " << on_boundary.y << ") winding: " << ellipse1.winding(on_boundary) << std::endl;
    
    // Bounding box
    Rect bbox = ellipse1.bounding_box();
    std::cout << "Bounding box: (" << bbox.x0 << ", " << bbox.y0 << ") to (" 
              << bbox.x1 << ", " << bbox.y1 << ")" << std::endl;
    
    // Transformations
    Ellipse translated = ellipse1 + Vec2{2.0, 2.0};
    std::cout << "Translated ellipse: center (" << translated.center().x << ", " << translated.center().y 
              << "), radii (" << translated.radii().x << ", " << translated.radii().y << ")" << std::endl;
    
    // From circle
    Circle circle = Circle::new_circle(Point{1.0, 2.0}, 5.0);
    Ellipse from_circle = Ellipse::from_circle(circle);
    std::cout << "Ellipse from circle: center (" << from_circle.center().x << ", " << from_circle.center().y 
              << "), radii (" << from_circle.radii().x << ", " << from_circle.radii().y << ")" << std::endl;
    
    // From rect
    Rect rect = Rect::new_rect(0.0, 0.0, 6.0, 8.0);
    Ellipse from_rect = Ellipse::from_rect(rect);
    std::cout << "Ellipse from rect: center (" << from_rect.center().x << ", " << from_rect.center().y 
              << "), radii (" << from_rect.radii().x << ", " << from_rect.radii().y << ")" << std::endl;
    
    // Modifiers
    Ellipse with_new_center = ellipse1.with_center(Point{5.0, 6.0});
    std::cout << "Ellipse with new center: (" << with_new_center.center().x << ", " << with_new_center.center().y << ")" << std::endl;
    
    Ellipse with_new_radii = ellipse1.with_radii(Vec2{6.0, 8.0});
    std::cout << "Ellipse with new radii: (" << with_new_radii.radii().x << ", " << with_new_radii.radii().y << ")" << std::endl;
    
    Ellipse with_new_rotation = ellipse1.with_rotation(M_PI / 2.0);
    std::cout << "Ellipse with new rotation: " << with_new_rotation.rotation() << std::endl;
    
    return 0;
} 