#include "kurbo/kurbo.hpp"
#include <iostream>
#include <cmath>

using namespace kurbo;

int main() {
    std::cout << "=== Point and Vector Operations ===" << std::endl;
    
    // Create a triangle using points
    Point a(0.0, 0.0);
    Point b(4.0, 0.0);
    Point c(2.0, 3.0);
    
    std::cout << "Triangle vertices:" << std::endl;
    std::cout << "  A: " << a << std::endl;
    std::cout << "  B: " << b << std::endl;
    std::cout << "  C: " << c << std::endl;
    
    // Calculate side vectors
    Vec2 ab = b - a;
    Vec2 bc = c - b;
    Vec2 ca = a - c;
    
    std::cout << "\nSide vectors:" << std::endl;
    std::cout << "  AB: " << ab << std::endl;
    std::cout << "  BC: " << bc << std::endl;
    std::cout << "  CA: " << ca << std::endl;
    
    // Calculate side lengths
    double side_ab = ab.hypot();
    double side_bc = bc.hypot();
    double side_ca = ca.hypot();
    
    std::cout << "\nSide lengths:" << std::endl;
    std::cout << "  AB: " << side_ab << std::endl;
    std::cout << "  BC: " << side_bc << std::endl;
    std::cout << "  CA: " << side_ca << std::endl;
    
    // Calculate perimeter
    double perimeter = side_ab + side_bc + side_ca;
    std::cout << "Perimeter: " << perimeter << std::endl;
    
    // Calculate area using cross product
    double area = 0.5 * std::abs(ab.cross(bc));
    std::cout << "Area: " << area << std::endl;
    
    // Find centroid
    Point centroid = a.midpoint(b).midpoint(c);
    std::cout << "Centroid: " << centroid << std::endl;
    
    // Vector operations demonstration
    std::cout << "\n=== Vector Operations ===" << std::endl;
    
    Vec2 v1(3.0, 4.0);
    Vec2 v2(1.0, 2.0);
    
    std::cout << "v1: " << v1 << std::endl;
    std::cout << "v2: " << v2 << std::endl;
    
    // Addition
    Vec2 sum = v1 + v2;
    std::cout << "v1 + v2 = " << sum << std::endl;
    
    // Subtraction
    Vec2 diff = v1 - v2;
    std::cout << "v1 - v2 = " << diff << std::endl;
    
    // Scalar multiplication
    Vec2 scaled = v1 * 2.0;
    std::cout << "v1 * 2 = " << scaled << std::endl;
    
    // Dot product
    double dot = v1.dot(v2);
    std::cout << "v1 · v2 = " << dot << std::endl;
    
    // Cross product
    double cross = v1.cross(v2);
    std::cout << "v1 × v2 = " << cross << std::endl;
    
    // Angle between vectors
    double angle = std::acos(dot / (v1.hypot() * v2.hypot()));
    std::cout << "Angle between v1 and v2: " << angle << " radians (" 
              << angle * 180.0 / M_PI << " degrees)" << std::endl;
    
    // Normalization
    Vec2 normalized = v1.normalize();
    std::cout << "Normalized v1: " << normalized << std::endl;
    std::cout << "Magnitude of normalized: " << normalized.hypot() << std::endl;
    
    // Rotation
    Vec2 rotated_90 = v1.turn_90();
    std::cout << "v1 rotated 90°: " << rotated_90 << std::endl;
    
    // Linear interpolation
    Vec2 lerp_result = v1.lerp(v2, 0.5);
    std::cout << "Lerp(v1, v2, 0.5) = " << lerp_result << std::endl;
    
    // Point operations demonstration
    std::cout << "\n=== Point Operations ===" << std::endl;
    
    Point p1(1.0, 2.0);
    Point p2(4.0, 6.0);
    
    std::cout << "p1: " << p1 << std::endl;
    std::cout << "p2: " << p2 << std::endl;
    
    // Distance
    double dist = p1.distance(p2);
    std::cout << "Distance between p1 and p2: " << dist << std::endl;
    
    // Midpoint
    Point mid = p1.midpoint(p2);
    std::cout << "Midpoint: " << mid << std::endl;
    
    // Linear interpolation
    Point lerp_point = p1.lerp(p2, 0.25);
    std::cout << "Lerp(p1, p2, 0.25) = " << lerp_point << std::endl;
    
    // Point + Vector
    Vec2 translation(2.0, -1.0);
    Point translated = p1 + translation;
    std::cout << "p1 + " << translation << " = " << translated << std::endl;
    
    // Vector from two points
    Vec2 direction = p2 - p1;
    std::cout << "Vector from p1 to p2: " << direction << std::endl;
    
    return 0;
} 