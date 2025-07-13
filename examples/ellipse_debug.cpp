#include "kurbo/kurbo.hpp"
#include <iostream>
#include <iomanip>

using namespace kurbo;

int main() {
    std::cout << "=== Ellipse Debug ===\n";
    
    // Test basic ellipse
    Point center{1.0, 2.0};
    Vec2 radii{3.0, 4.0};
    double rotation = 0.5;
    Ellipse ellipse = Ellipse::new_ellipse(center, radii, rotation);
    
    std::cout << "Input: center(" << center.x << ", " << center.y 
              << "), radii(" << radii.x << ", " << radii.y 
              << "), rotation(" << rotation << ")\n";
    
    Point center_result = ellipse.center();
    std::cout << "Actual center: (" << center_result.x << ", " << center_result.y << ")\n";
    
    Vec2 radii_result = ellipse.radii();
    std::cout << "Actual radii: (" << radii_result.x << ", " << radii_result.y << ")\n";
    
    double rotation_result = ellipse.rotation();
    std::cout << "Actual rotation: " << rotation_result << "\n";
    
    // Test from rect
    std::cout << "\n=== FromRect Test ===\n";
    Rect rect = Rect::new_rect(0.0, 0.0, 6.0, 8.0);
    Ellipse ellipse2 = Ellipse::from_rect(rect);
    
    Point center2 = ellipse2.center();
    std::cout << "Expected center: (3, 4), Actual: (" << center2.x << ", " << center2.y << ")\n";
    
    Vec2 radii2 = ellipse2.radii();
    std::cout << "Expected radii: (3, 4), Actual: (" << radii2.x << ", " << radii2.y << ")\n";
    
    double rotation2 = ellipse2.rotation();
    std::cout << "Expected rotation: 0, Actual: " << rotation2 << "\n";
    
    return 0;
} 