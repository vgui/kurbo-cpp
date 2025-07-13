#include "kurbo/kurbo.hpp"
#include <iostream>

using namespace kurbo;

int main() {
    std::cout << "=== Kurbo C++ Basic Example ===" << std::endl;
    
    // Create points
    Point p1(10.0, 10.0);
    Point p2(15.0, 5.0);
    
    std::cout << "Point 1: " << p1 << std::endl;
    std::cout << "Point 2: " << p2 << std::endl;
    
    // Create vectors
    Vec2 v1(5.0, -5.0);
    Vec2 v2(3.0, 4.0);
    
    std::cout << "Vector 1: " << v1 << std::endl;
    std::cout << "Vector 2: " << v2 << std::endl;
    
    // Point arithmetic
    Point p3 = p1 + v1;
    std::cout << "Point 1 + Vector 1 = " << p3 << std::endl;
    
    Vec2 v3 = p2 - p1;
    std::cout << "Point 2 - Point 1 = " << v3 << std::endl;
    
    // Vector operations
    double dot_product = v1.dot(v2);
    double cross_product = v1.cross(v2);
    double magnitude = v2.hypot();
    
    std::cout << "Dot product: " << dot_product << std::endl;
    std::cout << "Cross product: " << cross_product << std::endl;
    std::cout << "Magnitude of v2: " << magnitude << std::endl;
    
    // Size operations
    Size s1(10.0, 20.0);
    Size s2(5.0, 10.0);
    
    std::cout << "Size 1: " << s1 << std::endl;
    std::cout << "Size 2: " << s2 << std::endl;
    std::cout << "Area of Size 1: " << s1.area() << std::endl;
    std::cout << "Aspect ratio of Size 1: " << s1.aspect_ratio() << std::endl;
    
    // Distance calculations
    double distance = p1.distance(p2);
    double distance_squared = p1.distance_squared(p2);
    
    std::cout << "Distance between points: " << distance << std::endl;
    std::cout << "Squared distance: " << distance_squared << std::endl;
    
    // Midpoint
    Point midpoint = p1.midpoint(p2);
    std::cout << "Midpoint: " << midpoint << std::endl;
    
    // Linear interpolation
    Point lerp_point = p1.lerp(p2, 0.5);
    std::cout << "Lerp at t=0.5: " << lerp_point << std::endl;
    
    // Vector from angle
    Vec2 unit_vector = Vec2::from_angle(M_PI / 4); // 45 degrees
    std::cout << "Unit vector at 45°: " << unit_vector << std::endl;
    
    // Normalized vector
    Vec2 normalized = v2.normalize();
    std::cout << "Normalized v2: " << normalized << std::endl;
    std::cout << "Magnitude of normalized: " << normalized.hypot() << std::endl;
    
    return 0;
} 