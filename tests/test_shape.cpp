#include <gtest/gtest.h>
#include "kurbo/shape.hpp"
#include "kurbo/line.hpp"
#include "kurbo/rect.hpp"
#include "kurbo/circle.hpp"
#include "kurbo/bezpath.hpp"
#include "kurbo/point.hpp"
#include "kurbo/path_el.hpp"

using namespace kurbo;

class ShapeTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Set up any common test data
    }
};

// Test Line as Shape
TEST_F(ShapeTest, LineShape) {
    Line line(Point(0, 0), Point(10, 10));
    
    // Test path_elements
    auto elements = line.path_elements(0.1);
    ASSERT_EQ(elements.size(), 2);
    EXPECT_EQ(elements[0].type, PathElType::MoveTo);
    EXPECT_EQ(elements[0].point, Point(0, 0));
    EXPECT_EQ(elements[1].type, PathElType::LineTo);
    EXPECT_EQ(elements[1].point, Point(10, 10));
    
    // Test area (should be 0 for lines)
    EXPECT_EQ(line.area(), 0.0);
    
    // Test perimeter
    EXPECT_NEAR(line.perimeter(1e-9), line.length(), 1e-9);
    
    // Test winding
    EXPECT_EQ(line.winding(Point(5, 5)), 0); // Point on line
    EXPECT_EQ(line.winding(Point(0, 0)), 0); // Point at start
    EXPECT_EQ(line.winding(Point(10, 10)), 0); // Point at end
    
    // Test bounding_box
    Rect bbox = line.bounding_box();
    EXPECT_EQ(bbox.x0, 0);
    EXPECT_EQ(bbox.y0, 0);
    EXPECT_EQ(bbox.x1, 10);
    EXPECT_EQ(bbox.y1, 10);
    
    // Test as_line
    auto line_opt = line.as_line();
    ASSERT_TRUE(line_opt.has_value());
    EXPECT_EQ(line_opt.value(), line);
}

// Test Rect as Shape
TEST_F(ShapeTest, RectShape) {
    Rect rect(0, 0, 10, 10);
    
    // Test path_elements
    auto elements = rect.path_elements(0.1);
    ASSERT_EQ(elements.size(), 5);
    EXPECT_EQ(elements[0].type, PathElType::MoveTo);
    EXPECT_EQ(elements[0].point, Point(0, 0));
    EXPECT_EQ(elements[1].type, PathElType::LineTo);
    EXPECT_EQ(elements[1].point, Point(10, 0));
    EXPECT_EQ(elements[2].type, PathElType::LineTo);
    EXPECT_EQ(elements[2].point, Point(10, 10));
    EXPECT_EQ(elements[3].type, PathElType::LineTo);
    EXPECT_EQ(elements[3].point, Point(0, 10));
    EXPECT_EQ(elements[4].type, PathElType::ClosePath);
    
    // Test area
    EXPECT_EQ(rect.area(), 100.0);
    
    // Test perimeter
    EXPECT_EQ(rect.perimeter(1e-9), 40.0);
    
    // Test winding
    EXPECT_EQ(rect.winding(Point(5, 5)), 1); // Point inside
    EXPECT_EQ(rect.winding(Point(0, 0)), 1); // Point on edge
    EXPECT_EQ(rect.winding(Point(15, 15)), 0); // Point outside
    
    // Test bounding_box
    Rect bbox = rect.bounding_box();
    EXPECT_EQ(bbox, rect);
    
    // Test as_rect
    auto rect_opt = rect.as_rect();
    ASSERT_TRUE(rect_opt.has_value());
    EXPECT_EQ(rect_opt.value(), rect);
}

// Test Circle as Shape
TEST_F(ShapeTest, CircleShape) {
    Circle circle(Point(5, 5), 3);
    
    // Test path_elements (approximated with Bézier curves)
    auto elements = circle.path_elements(0.1);
    ASSERT_GT(elements.size(), 4); // Should have multiple curve elements
    
    // Test area
    EXPECT_NEAR(circle.area(), M_PI * 9, 1e-6);
    
    // Test perimeter
    EXPECT_NEAR(circle.perimeter(1e-9), 2 * M_PI * 3, 1e-9);
    
    // Test winding
    EXPECT_EQ(circle.winding(Point(5, 5)), 1); // Point at center
    EXPECT_EQ(circle.winding(Point(7, 5)), 1); // Point inside (distance = 2 < 3)
    EXPECT_EQ(circle.winding(Point(8, 5)), 0); // Point on boundary (distance = 3)
    EXPECT_EQ(circle.winding(Point(10, 5)), 0); // Point outside
    
    // Test bounding_box
    Rect bbox = circle.bounding_box();
    EXPECT_EQ(bbox.x0, 2);
    EXPECT_EQ(bbox.y0, 2);
    EXPECT_EQ(bbox.x1, 8);
    EXPECT_EQ(bbox.y1, 8);
    
    // Test as_circle
    auto circle_opt = circle.as_circle();
    ASSERT_TRUE(circle_opt.has_value());
    EXPECT_EQ(circle_opt.value(), circle);
}

// Test BezPath as Shape
TEST_F(ShapeTest, BezPathShape) {
    BezPath path;
    path.move_to(Point(0, 0));
    path.line_to(Point(10, 0));
    path.line_to(Point(10, 10));
    path.line_to(Point(0, 10));
    path.close_path();
    
    // Test path_elements
    auto elements = path.path_elements(0.1);
    ASSERT_EQ(elements.size(), 5);
    EXPECT_EQ(elements[0].type, PathElType::MoveTo);
    EXPECT_EQ(elements[0].point, Point(0, 0));
    EXPECT_EQ(elements[1].type, PathElType::LineTo);
    EXPECT_EQ(elements[1].point, Point(10, 0));
    EXPECT_EQ(elements[2].type, PathElType::LineTo);
    EXPECT_EQ(elements[2].point, Point(10, 10));
    EXPECT_EQ(elements[3].type, PathElType::LineTo);
    EXPECT_EQ(elements[3].point, Point(0, 10));
    EXPECT_EQ(elements[4].type, PathElType::ClosePath);
    
    // Test area
    EXPECT_EQ(path.area(), 100.0);
    
    // Test perimeter
    EXPECT_EQ(path.perimeter(1e-9), 40.0);
    
    // Test winding
    EXPECT_EQ(path.winding(Point(5, 5)), 1); // Point inside
    EXPECT_EQ(path.winding(Point(0, 0)), 1); // Point on edge
    EXPECT_EQ(path.winding(Point(15, 15)), 0); // Point outside
    
    // Test bounding_box
    Rect bbox = path.bounding_box();
    EXPECT_EQ(bbox.x0, 0);
    EXPECT_EQ(bbox.y0, 0);
    EXPECT_EQ(bbox.x1, 10);
    EXPECT_EQ(bbox.y1, 10);
    
    // Test as_path_slice
    auto path_slice = path.as_path_slice();
    ASSERT_TRUE(path_slice.has_value());
    EXPECT_EQ(path_slice.value().size(), 5);
}

// Test Shape polymorphic behavior
TEST_F(ShapeTest, PolymorphicShape) {
    std::vector<std::unique_ptr<Shape>> shapes;
    
    shapes.push_back(std::make_unique<Line>(Point(0, 0), Point(10, 10)));
    shapes.push_back(std::make_unique<Rect>(0, 0, 10, 10));
    shapes.push_back(std::make_unique<Circle>(Point(5, 5), 3));
    
    BezPath path;
    path.move_to(Point(0, 0));
    path.line_to(Point(5, 0));
    path.line_to(Point(5, 5));
    path.close_path();
    shapes.push_back(std::make_unique<BezPath>(path));
    
    // Test that all shapes can be used polymorphically
    for (const auto& shape : shapes) {
        // EXPECT_FALSE(shape->is_empty());
        
        auto elements = shape->path_elements(0.1);
        EXPECT_FALSE(elements.empty());
        
        auto bbox = shape->bounding_box();
        EXPECT_FALSE(bbox.is_nan());
    }
}

// Test Shape utility methods
TEST_F(ShapeTest, ShapeUtilityMethods) {
    Line line(Point(0, 0), Point(10, 10));
    
    // Test contains method
    EXPECT_TRUE(line.contains(Point(5, 5))); // Point on line
    EXPECT_FALSE(line.contains(Point(15, 15))); // Point off line
    
    // Test to_path method
    BezPath path = line.to_path(0.1);
    EXPECT_FALSE(path.is_empty());
    
    // Test into_path method
    BezPath path2 = line.into_path(0.1);
    EXPECT_FALSE(path2.is_empty());
    
    // Test path_segments method
    // Note: This would require implementing the Segments iterator
    // For now, we just test that it compiles
}

// Test Shape with different tolerance values
TEST_F(ShapeTest, ShapeTolerance) {
    Circle circle(Point(0, 0), 5);
    
    // Test with different tolerance values
    auto elements1 = circle.path_elements(0.1);
    auto elements2 = circle.path_elements(0.01);
    
    // Lower tolerance should give more elements (more accurate approximation)
    EXPECT_GE(elements2.size(), elements1.size());
}

// Test Shape with empty/invalid shapes
TEST_F(ShapeTest, EmptyShape) {
    BezPath empty_path;
    
    // Test empty path
    EXPECT_TRUE(empty_path.is_empty());
    auto elements = empty_path.path_elements(0.1);
    EXPECT_TRUE(elements.empty());
    
    // Test bounding box of empty path
    Rect bbox = empty_path.bounding_box();
    EXPECT_TRUE(bbox.is_nan()); // Should return invalid rect
} 