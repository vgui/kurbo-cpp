#include <iostream>
#include <iomanip>
#include <vector>
#include "kurbo/path_el.hpp"
#include "kurbo/point.hpp"

using namespace kurbo;

int main() {
    std::cout << "=== PathEl Example ===" << std::endl;
    
    // Create different path elements
    PathEl move_to(PathElType::MoveTo, Point(0, 0));
    PathEl line_to(PathElType::LineTo, Point(100, 0));
    PathEl quad_to(PathElType::QuadTo, Point(50, 100), Point(100, 100));
    PathEl curve_to(PathElType::CurveTo, Point(25, 50), Point(75, 50), Point(100, 100));
    PathEl close_path(PathElType::ClosePath);
    
    std::cout << "MoveTo: " << move_to << std::endl;
    std::cout << "LineTo: " << line_to << std::endl;
    std::cout << "QuadTo: " << quad_to << std::endl;
    std::cout << "CurveTo: " << curve_to << std::endl;
    std::cout << "ClosePath: " << close_path << std::endl;
    
    // Create a simple path
    std::cout << "\n--- Simple Path ---" << std::endl;
    std::vector<PathEl> path = {
        PathEl(PathElType::MoveTo, Point(0, 0)),
        PathEl(PathElType::LineTo, Point(100, 0)),
        PathEl(PathElType::LineTo, Point(100, 100)),
        PathEl(PathElType::LineTo, Point(0, 100)),
        PathEl(PathElType::ClosePath)
    };
    
    std::cout << "Simple rectangle path:" << std::endl;
    for (size_t i = 0; i < path.size(); ++i) {
        std::cout << "  " << i << ": " << path[i] << std::endl;
    }
    
    // Create a curved path
    std::cout << "\n--- Curved Path ---" << std::endl;
    std::vector<PathEl> curved_path = {
        PathEl(PathElType::MoveTo, Point(0, 50)),
        PathEl(PathElType::QuadTo, Point(25, 0), Point(50, 50)),
        PathEl(PathElType::QuadTo, Point(75, 100), Point(100, 50)),
        PathEl(PathElType::LineTo, Point(100, 100)),
        PathEl(PathElType::LineTo, Point(0, 100)),
        PathEl(PathElType::ClosePath)
    };
    
    std::cout << "Curved path:" << std::endl;
    for (size_t i = 0; i < curved_path.size(); ++i) {
        std::cout << "  " << i << ": " << curved_path[i] << std::endl;
    }
    
    // Create a complex path with cubic curves
    std::cout << "\n--- Complex Path ---" << std::endl;
    std::vector<PathEl> complex_path = {
        PathEl(PathElType::MoveTo, Point(0, 0)),
        PathEl(PathElType::CurveTo, Point(25, 0), Point(75, 0), Point(100, 0)),
        PathEl(PathElType::CurveTo, Point(125, 25), Point(125, 75), Point(100, 100)),
        PathEl(PathElType::CurveTo, Point(75, 125), Point(25, 125), Point(0, 100)),
        PathEl(PathElType::CurveTo, Point(-25, 75), Point(-25, 25), Point(0, 0)),
        PathEl(PathElType::ClosePath)
    };
    
    std::cout << "Complex path with cubic curves:" << std::endl;
    for (size_t i = 0; i < complex_path.size(); ++i) {
        std::cout << "  " << i << ": " << complex_path[i] << std::endl;
    }
    
    // Access path element properties
    std::cout << "\n--- Path Element Properties ---" << std::endl;
    std::cout << "MoveTo type: " << static_cast<int>(move_to.type) << std::endl;
    std::cout << "MoveTo point: " << move_to.point << std::endl;
    std::cout << "LineTo point: " << line_to.point << std::endl;
    std::cout << "QuadTo points: " << quad_to.point << " " << quad_to.point2 << std::endl;
    std::cout << "CurveTo points: " << curve_to.point << " " << curve_to.point2 << " " << curve_to.point3 << std::endl;
    
    // Create path elements with different coordinates
    std::cout << "\n--- Different Coordinates ---" << std::endl;
    PathEl move_high(PathElType::MoveTo, Point(1000, 500));
    PathEl line_diagonal(PathElType::LineTo, Point(1500, 800));
    PathEl quad_wide(PathElType::QuadTo, Point(1250, 200), Point(2000, 600));
    PathEl curve_complex(PathElType::CurveTo, Point(1200, 300), Point(1800, 400), Point(2000, 800));
    
    std::cout << "MoveTo (high): " << move_high << std::endl;
    std::cout << "LineTo (diagonal): " << line_diagonal << std::endl;
    std::cout << "QuadTo (wide): " << quad_wide << std::endl;
    std::cout << "CurveTo (complex): " << curve_complex << std::endl;
    
    // Test default constructor
    std::cout << "\n--- Default Constructor ---" << std::endl;
    PathEl default_el(PathElType::MoveTo);
    std::cout << "Default element: " << default_el << std::endl;
    std::cout << "Default type: " << static_cast<int>(default_el.type) << std::endl;
    std::cout << "Default point: " << default_el.point << std::endl;
    
    // Test copy constructor and assignment
    std::cout << "\n--- Copy Operations ---" << std::endl;
    PathEl original(PathElType::QuadTo, Point(10, 20), Point(30, 40));
    PathEl copy(original);
    PathEl assigned(PathElType::MoveTo);
    assigned = original;
    
    std::cout << "Original: " << original << std::endl;
    std::cout << "Copy: " << copy << std::endl;
    std::cout << "Assigned: " << assigned << std::endl;
    
    // Create a star-like path
    std::cout << "\n--- Star Path ---" << std::endl;
    std::vector<PathEl> star_path = {
        PathEl(PathElType::MoveTo, Point(50, 0)),
        PathEl(PathElType::LineTo, Point(61, 35)),
        PathEl(PathElType::LineTo, Point(100, 35)),
        PathEl(PathElType::LineTo, Point(69, 57)),
        PathEl(PathElType::LineTo, Point(80, 100)),
        PathEl(PathElType::LineTo, Point(50, 70)),
        PathEl(PathElType::LineTo, Point(20, 100)),
        PathEl(PathElType::LineTo, Point(31, 57)),
        PathEl(PathElType::LineTo, Point(0, 35)),
        PathEl(PathElType::LineTo, Point(39, 35)),
        PathEl(PathElType::ClosePath)
    };
    
    std::cout << "Star path:" << std::endl;
    for (size_t i = 0; i < star_path.size(); ++i) {
        std::cout << "  " << i << ": " << star_path[i] << std::endl;
    }
    
    return 0;
} 