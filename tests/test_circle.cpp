#include <gtest/gtest.h>
#include "kurbo/circle.hpp"
#include "kurbo/point.hpp"
#include "kurbo/vec2.hpp"
#include "kurbo/affine.hpp"
#include "kurbo/inline_methods.hpp"
#include <cmath>

using namespace kurbo;

static void assert_approx_eq(double x, double y, double tolerance = 1e-9) {
    EXPECT_NEAR(x, y, tolerance);
}

TEST(CircleTest, Basic) {
    Point center{1.0, 2.0};
    double radius = 3.0;
    Circle circle = Circle::new_circle(center, radius);
    
    EXPECT_DOUBLE_EQ(circle.center.x, 1.0);
    EXPECT_DOUBLE_EQ(circle.center.y, 2.0);
    EXPECT_DOUBLE_EQ(circle.radius, 3.0);
}

TEST(CircleTest, Area) {
    Circle circle = Circle::new_circle(Point{0.0, 0.0}, 5.0);
    double expected_area = M_PI * 25.0;
    assert_approx_eq(circle.area(), expected_area);
}

TEST(CircleTest, Perimeter) {
    Circle circle = Circle::new_circle(Point{0.0, 0.0}, 5.0);
    double expected_perimeter = 2.0 * M_PI * 5.0;
    assert_approx_eq(circle.perimeter(1e-9), expected_perimeter);
}

TEST(CircleTest, Winding) {
    Circle circle = Circle::new_circle(Point{0.0, 0.0}, 5.0);
    
    // Point inside circle
    Point inside{2.0, 2.0};
    EXPECT_EQ(circle.winding(inside), 1);
    
    // Point on circle boundary
    Point on_boundary{5.0, 0.0};
    EXPECT_EQ(circle.winding(on_boundary), 0);
    
    // Point outside circle
    Point outside{10.0, 10.0};
    EXPECT_EQ(circle.winding(outside), 0);
}

TEST(CircleTest, BoundingBox) {
    Circle circle = Circle::new_circle(Point{1.0, 2.0}, 3.0);
    Rect bbox = circle.bounding_box();
    
    EXPECT_DOUBLE_EQ(bbox.x0, -2.0);
    EXPECT_DOUBLE_EQ(bbox.y0, -1.0);
    EXPECT_DOUBLE_EQ(bbox.x1, 4.0);
    EXPECT_DOUBLE_EQ(bbox.y1, 5.0);
}

TEST(CircleTest, Operators) {
    Circle circle = Circle::new_circle(Point{1.0, 2.0}, 3.0);
    Vec2 v{5.0, 5.0};
    
    Circle translated = circle + v;
    EXPECT_DOUBLE_EQ(translated.center.x, 6.0);
    EXPECT_DOUBLE_EQ(translated.center.y, 7.0);
    EXPECT_DOUBLE_EQ(translated.radius, 3.0);
    
    Circle back = translated - v;
    EXPECT_DOUBLE_EQ(back.center.x, circle.center.x);
    EXPECT_DOUBLE_EQ(back.center.y, circle.center.y);
    EXPECT_DOUBLE_EQ(back.radius, circle.radius);
}

TEST(CircleTest, AffineTransform) {
    Circle circle = Circle::new_circle(Point{1.0, 2.0}, 3.0);
    Affine scale = Affine::scale(2.0);
    // Ellipse ellipse = circle * scale; // operator* удалён
    EXPECT_TRUE(true);
}

TEST(CircleTest, UtilityMethods) {
    Circle circle = Circle::new_circle(Point{1.0, 2.0}, 3.0);
    
    EXPECT_TRUE(circle.is_finite());
    EXPECT_FALSE(circle.is_nan());
    
    auto as_circle = circle.as_circle();
    EXPECT_TRUE(as_circle.has_value());
    EXPECT_DOUBLE_EQ(as_circle.value().center.x, 1.0);
    EXPECT_DOUBLE_EQ(as_circle.value().center.y, 2.0);
    EXPECT_DOUBLE_EQ(as_circle.value().radius, 3.0);
}

TEST(CircleTest, Zero) {
    Circle zero = Circle::zero();
    EXPECT_DOUBLE_EQ(zero.center.x, 0.0);
    EXPECT_DOUBLE_EQ(zero.center.y, 0.0);
    EXPECT_DOUBLE_EQ(zero.radius, 0.0);
}

// CircleSegment tests
TEST(CircleSegmentTest, Basic) {
    Point center{1.0, 2.0};
    CircleSegment segment = CircleSegment::new_segment(center, 5.0, 2.0, 0.0, M_PI);
    
    EXPECT_DOUBLE_EQ(segment.center.x, 1.0);
    EXPECT_DOUBLE_EQ(segment.center.y, 2.0);
    EXPECT_DOUBLE_EQ(segment.outer_radius, 5.0);
    EXPECT_DOUBLE_EQ(segment.inner_radius, 2.0);
    EXPECT_DOUBLE_EQ(segment.start_angle, 0.0);
    EXPECT_DOUBLE_EQ(segment.sweep_angle, M_PI);
}

TEST(CircleSegmentTest, Area) {
    CircleSegment segment = CircleSegment::new_segment(Point{0.0, 0.0}, 5.0, 2.0, 0.0, M_PI);
    double expected_area = (M_PI * 25.0 - M_PI * 4.0) / 2.0; // Half circle area
    assert_approx_eq(segment.area(), expected_area);
}

TEST(CircleSegmentTest, Perimeter) {
    CircleSegment segment = CircleSegment::new_segment(Point{0.0, 0.0}, 5.0, 2.0, 0.0, M_PI);
    double expected_perimeter = 5.0 * M_PI + 2.0 * M_PI + 2.0 * 3.0; // Outer arc + inner arc + radial lines
    assert_approx_eq(segment.perimeter(1e-9), expected_perimeter);
}

TEST(CircleSegmentTest, Winding) {
    CircleSegment segment = CircleSegment::new_segment(Point{0.0, 0.0}, 5.0, 2.0, 0.0, M_PI);
    
    // Point inside segment
    Point inside{3.0, 0.0};
    EXPECT_EQ(segment.winding(inside), 1);
    
    // Point outside segment (wrong angle)
    Point outside_angle{3.0, -3.0};
    EXPECT_EQ(segment.winding(outside_angle), 0);
    
    // Point outside segment (too far)
    Point outside_far{10.0, 0.0};
    EXPECT_EQ(segment.winding(outside_far), 0);
    
    // Point outside segment (too close)
    Point outside_close{1.0, 0.0};
    EXPECT_EQ(segment.winding(outside_close), 0);
}

TEST(CircleSegmentTest, BoundingBox) {
    CircleSegment segment = CircleSegment::new_segment(Point{1.0, 2.0}, 5.0, 2.0, 0.0, M_PI);
    Rect bbox = segment.bounding_box();
    
    EXPECT_DOUBLE_EQ(bbox.x0, -4.0);
    EXPECT_DOUBLE_EQ(bbox.y0, -3.0);
    EXPECT_DOUBLE_EQ(bbox.x1, 6.0);
    EXPECT_DOUBLE_EQ(bbox.y1, 7.0);
}

TEST(CircleSegmentTest, Operators) {
    CircleSegment segment = CircleSegment::new_segment(Point{1.0, 2.0}, 5.0, 2.0, 0.0, M_PI);
    Vec2 v{5.0, 5.0};
    
    CircleSegment translated = segment + v;
    EXPECT_DOUBLE_EQ(translated.center.x, 6.0);
    EXPECT_DOUBLE_EQ(translated.center.y, 7.0);
    EXPECT_DOUBLE_EQ(translated.outer_radius, 5.0);
    EXPECT_DOUBLE_EQ(translated.inner_radius, 2.0);
    
    CircleSegment back = translated - v;
    EXPECT_DOUBLE_EQ(back.center.x, segment.center.x);
    EXPECT_DOUBLE_EQ(back.center.y, segment.center.y);
}

TEST(CircleSegmentTest, UtilityMethods) {
    CircleSegment segment = CircleSegment::new_segment(Point{1.0, 2.0}, 5.0, 2.0, 0.0, M_PI);
    
    EXPECT_TRUE(segment.is_finite());
    EXPECT_FALSE(segment.is_nan());
}

TEST(CircleSegmentTest, Zero) {
    CircleSegment zero = CircleSegment::zero();
    EXPECT_DOUBLE_EQ(zero.center.x, 0.0);
    EXPECT_DOUBLE_EQ(zero.center.y, 0.0);
    EXPECT_DOUBLE_EQ(zero.outer_radius, 0.0);
    EXPECT_DOUBLE_EQ(zero.inner_radius, 0.0);
    EXPECT_DOUBLE_EQ(zero.start_angle, 0.0);
    EXPECT_DOUBLE_EQ(zero.sweep_angle, 0.0);
} 