#include <iostream>
#include <iomanip>
#include "kurbo/quadbez.hpp"
#include "kurbo/point.hpp"
#include "kurbo/line.hpp"
#include "kurbo/affine.hpp"

using namespace kurbo;

int main() {
    std::cout << "=== QuadBez Example ===" << std::endl;
    
    // Create different quadratic Bezier curves
    QuadBez quad1(Point(0, 0), Point(50, 100), Point(100, 0));
    QuadBez quad2(Point(0, 0), Point(100, 0), Point(100, 100));
    QuadBez quad3(Point(0, 0), Point(0, 100), Point(100, 100));
    QuadBez quad4(Point(0, 0), Point(50, 50), Point(100, 100)); // Linear
    
    std::cout << "QuadBez1: " << quad1 << std::endl;
    std::cout << "QuadBez2: " << quad2 << std::endl;
    std::cout << "QuadBez3: " << quad3 << std::endl;
    std::cout << "QuadBez4: " << quad4 << std::endl;
    
    // Evaluate curves at different parameters
    std::cout << "\n--- Curve Evaluation ---" << std::endl;
    for (double t = 0.0; t <= 1.0; t += 0.25) {
        Point p1 = quad1.eval(t);
        Point p2 = quad2.eval(t);
        Point p3 = quad3.eval(t);
        Point p4 = quad4.eval(t);
        std::cout << "t=" << t << ": " << p1 << " " << p2 << " " << p3 << " " << p4 << std::endl;
    }
    
    // Arc length
    std::cout << "\n--- Arc Length ---" << std::endl;
    double arclen1 = quad1.arclen(1e-9);
    double arclen2 = quad2.arclen(1e-9);
    double arclen3 = quad3.arclen(1e-9);
    double arclen4 = quad4.arclen(1e-9);
    std::cout << "Arc lengths: " << arclen1 << " " << arclen2 << " " << arclen3 << " " << arclen4 << std::endl;
    
    // Parameter from arc length
    std::cout << "\n--- Parameter from Arc Length ---" << std::endl;
    double t1 = quad1.inv_arclen(arclen1 / 2, 1e-9);
    double t2 = quad2.inv_arclen(arclen2 / 2, 1e-9);
    double t3 = quad3.inv_arclen(arclen3 / 2, 1e-9);
    double t4 = quad4.inv_arclen(arclen4 / 2, 1e-9);
    std::cout << "Half arc length t: " << t1 << " " << t2 << " " << t3 << " " << t4 << std::endl;
    
    // Subdivision
    std::cout << "\n--- Subdivision ---" << std::endl;
    auto [quad1_1, quad1_2] = quad1.subdivide();
    auto [quad2_1, quad2_2] = quad2.subdivide();
    
    std::cout << "Quad1 subdivision:" << std::endl;
    std::cout << "  First half: start=" << quad1_1->start() << ", end=" << quad1_1->end() << std::endl;
    std::cout << "  Second half: start=" << quad1_2->start() << ", end=" << quad1_2->end() << std::endl;
    
    std::cout << "Quad2 subdivision:" << std::endl;
    std::cout << "  First half: start=" << quad2_1->start() << ", end=" << quad2_1->end() << std::endl;
    std::cout << "  Second half: start=" << quad2_2->start() << ", end=" << quad2_2->end() << std::endl;
    
    // Nearest point
    std::cout << "\n--- Nearest Point ---" << std::endl;
    Point test_point(50, 25);
    Nearest nearest_result1 = quad1.nearest(test_point, 1e-9);
    Nearest nearest_result2 = quad2.nearest(test_point, 1e-9);
    Point nearest1 = quad1.eval(nearest_result1.t);
    Point nearest2 = quad2.eval(nearest_result2.t);
    
    std::cout << "Test point: " << test_point << std::endl;
    std::cout << "Quad1 nearest t: " << nearest_result1.t << ", distance: " << std::sqrt(nearest_result1.distance_sq) << ", point: " << nearest1 << std::endl;
    std::cout << "Quad2 nearest t: " << nearest_result2.t << ", distance: " << std::sqrt(nearest_result2.distance_sq) << ", point: " << nearest2 << std::endl;
    
    // Transform curves
    std::cout << "\n--- Transform Curves ---" << std::endl;
    Affine transform = Affine::translate(Vec2(10, 20)) * Affine::scale(1.5);
    QuadBez transformed1 = quad1.transform(transform);
    QuadBez transformed2 = quad2.transform(transform);
    
    std::cout << "Transformed quad1: " << transformed1 << std::endl;
    std::cout << "Transformed quad2: " << transformed2 << std::endl;
    
    // Curve properties
    std::cout << "\n--- Curve Properties ---" << std::endl;
    std::cout << "Quad1 is linear: " << (quad1.is_linear() ? "true" : "false") << std::endl;
    std::cout << "Quad2 is linear: " << (quad2.is_linear() ? "true" : "false") << std::endl;
    std::cout << "Quad3 is linear: " << (quad3.is_linear() ? "true" : "false") << std::endl;
    std::cout << "Quad4 is linear: " << (quad4.is_linear() ? "true" : "false") << std::endl;
    
    // Start and end points
    std::cout << "\n--- Start and End Points ---" << std::endl;
    std::cout << "Quad1 start: " << quad1.start() << ", end: " << quad1.end() << std::endl;
    std::cout << "Quad2 start: " << quad2.start() << ", end: " << quad2.end() << std::endl;
    std::cout << "Quad3 start: " << quad3.start() << ", end: " << quad3.end() << std::endl;
    std::cout << "Quad4 start: " << quad4.start() << ", end: " << quad4.end() << std::endl;
    
    // Control points
    std::cout << "\n--- Control Points ---" << std::endl;
    std::cout << "Quad1 control points: " << quad1.p0 << " " << quad1.p1 << " " << quad1.p2 << std::endl;
    std::cout << "Quad2 control points: " << quad2.p0 << " " << quad2.p1 << " " << quad2.p2 << std::endl;
    std::cout << "Quad3 control points: " << quad3.p0 << " " << quad3.p1 << " " << quad3.p2 << std::endl;
    std::cout << "Quad4 control points: " << quad4.p0 << " " << quad4.p1 << " " << quad4.p2 << std::endl;
    
    // Signed area
    std::cout << "\n--- Signed Area ---" << std::endl;
    double area1 = quad1.signed_area();
    double area2 = quad2.signed_area();
    double area3 = quad3.signed_area();
    double area4 = quad4.signed_area();
    std::cout << "Signed areas: " << area1 << " " << area2 << " " << area3 << " " << area4 << std::endl;
    
    // Clean up allocated memory
    delete quad1_1;
    delete quad1_2;
    delete quad2_1;
    delete quad2_2;
    
    return 0;
} 