#include "kurbo/shape.hpp"
#include "kurbo/line.hpp"
#include "kurbo/rect.hpp"
#include "kurbo/circle.hpp"
#include "kurbo/ellipse.hpp"
#include "kurbo/bezpath.hpp"
#include "kurbo/point.hpp"
#include "kurbo/affine.hpp"
#include <iostream>
#include <vector>
#include <memory>

using namespace kurbo;

void print_shape_info(const Shape& shape, const std::string& name) {
    std::cout << "\n=== " << name << " ===" << std::endl;
    
    // Get path elements
    auto elements = shape.path_elements(0.1);
    std::cout << "Path elements: " << elements.size() << " elements" << std::endl;
    
    // Area and perimeter
    std::cout << "Area: " << shape.area() << std::endl;
    std::cout << "Perimeter: " << shape.perimeter(1e-9) << std::endl;
    
    // Bounding box
    Rect bbox = shape.bounding_box();
    std::cout << "Bounding box: " << bbox << std::endl;
    
    // Test some points
    Point test_points[] = {
        Point(5, 5),
        Point(0, 0),
        Point(10, 10),
        Point(15, 15)
    };
    
    std::cout << "Winding numbers:" << std::endl;
    for (const auto& pt : test_points) {
        int winding = shape.winding(pt);
        std::cout << "  Point " << pt << ": winding = " << winding;
        if (shape.contains(pt)) {
            std::cout << " (inside)";
        } else {
            std::cout << " (outside)";
        }
        std::cout << std::endl;
    }
}

void demonstrate_polymorphic_shapes() {
    std::cout << "\n=== Polymorphic Shape Demonstration ===" << std::endl;
    
    std::vector<std::unique_ptr<Shape>> shapes;
    
    // Create different types of shapes
    shapes.push_back(std::make_unique<Line>(Point(0, 0), Point(10, 10)));
    shapes.push_back(std::make_unique<Rect>(0, 0, 10, 10));
    shapes.push_back(std::make_unique<Circle>(Point(5, 5), 3));
    
    // Create a complex BezPath
    BezPath path;
    path.move_to(Point(0, 0));
    path.line_to(Point(5, 0));
    path.line_to(Point(5, 5));
    path.close_path();
    shapes.push_back(std::make_unique<BezPath>(path));
    
    // Process all shapes polymorphically
    for (size_t i = 0; i < shapes.size(); ++i) {
        const auto& shape = shapes[i];
        std::cout << "\nShape " << i + 1 << ":" << std::endl;
        
        // Convert to BezPath
        BezPath bez_path = shape->to_path(0.1);
        std::cout << "  Converted to BezPath with " << bez_path.elements().size() << " elements" << std::endl;
        
        // Get bounding box
        Rect bbox = shape->bounding_box();
        std::cout << "  Bounding box: " << bbox << std::endl;
        
        // Test point containment
        Point test_pt(3, 3);
        if (shape->contains(test_pt)) {
            std::cout << "  Point " << test_pt << " is inside" << std::endl;
        } else {
            std::cout << "  Point " << test_pt << " is outside" << std::endl;
        }
    }
}

void demonstrate_shape_transformations() {
    std::cout << "\n=== Shape Transformations ===" << std::endl;
    
    // Create a rectangle
    Rect rect(0, 0, 5, 5);
    std::cout << "Original rectangle: " << rect << std::endl;
    std::cout << "Area: " << rect.area() << std::endl;
    
    // Apply affine transformation
    Affine transform = Affine::translate(Vec2(10, 10)) * Affine::scale(2.0);
    Rect transformed_rect = transform * rect;
    std::cout << "Transformed rectangle: " << transformed_rect << std::endl;
    std::cout << "Transformed area: " << transformed_rect.area() << std::endl;
    
    // Create a circle and transform it
    Circle circle(Point(0, 0), 2);
    std::cout << "Original circle: center=" << circle.center << ", radius=" << circle.radius << std::endl;
    std::cout << "Area: " << circle.area() << std::endl;
    
    // Transform circle (becomes an ellipse)
    Ellipse transformed_circle = transform * circle;
    std::cout << "Transformed circle (now ellipse): center=" << transformed_circle.center() 
              << ", radii=(" << transformed_circle.radii().x << ", " << transformed_circle.radii().y << ")" << std::endl;
    std::cout << "Transformed area: " << transformed_circle.area() << std::endl;
}

void demonstrate_shape_operations() {
    std::cout << "\n=== Shape Operations ===" << std::endl;
    
    // Create a line
    Line line(Point(0, 0), Point(10, 10));
    std::cout << "Line: " << line << std::endl;
    
    // Test as_line method
    auto line_opt = line.as_line();
    if (line_opt.has_value()) {
        std::cout << "Successfully retrieved as Line: " << line_opt.value() << std::endl;
    }
    
    // Create a rectangle
    Rect rect(0, 0, 10, 10);
    std::cout << "Rectangle: " << rect << std::endl;
    
    // Test as_rect method
    auto rect_opt = rect.as_rect();
    if (rect_opt.has_value()) {
        std::cout << "Successfully retrieved as Rect: " << rect_opt.value() << std::endl;
    }
    
    // Create a circle
    Circle circle(Point(5, 5), 3);
    std::cout << "Circle: center=" << circle.center << ", radius=" << circle.radius << std::endl;
    
    // Test as_circle method
    auto circle_opt = circle.as_circle();
    if (circle_opt.has_value()) {
        std::cout << "Successfully retrieved as Circle: center=" 
                  << circle_opt.value().center << ", radius=" << circle_opt.value().radius << std::endl;
    }
    
    // Create a BezPath
    BezPath path;
    path.move_to(Point(0, 0));
    path.line_to(Point(5, 0));
    path.line_to(Point(5, 5));
    path.close_path();
    std::cout << "BezPath with " << path.elements().size() << " elements" << std::endl;
    
    // Test as_path_slice method
    auto path_slice = path.as_path_slice();
    if (path_slice.has_value()) {
        std::cout << "Successfully retrieved as path slice with " 
                  << path_slice.value().size() << " elements" << std::endl;
    }
}

int main() {
    std::cout << "Kurbo Shape Interface Example" << std::endl;
    std::cout << "=============================" << std::endl;
    
    // Create different shapes
    Line line(Point(0, 0), Point(10, 10));
    Rect rect(0, 0, 10, 10);
    Circle circle(Point(5, 5), 3);
    
    // Print information about each shape
    print_shape_info(line, "Line");
    print_shape_info(rect, "Rectangle");
    print_shape_info(circle, "Circle");
    
    // Demonstrate polymorphic behavior
    demonstrate_polymorphic_shapes();
    
    // Demonstrate transformations
    demonstrate_shape_transformations();
    
    // Demonstrate shape operations
    demonstrate_shape_operations();
    
    std::cout << "\n=== Example Complete ===" << std::endl;
    return 0;
} 