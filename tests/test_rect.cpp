#include <gtest/gtest.h>
#include "kurbo/rect.hpp"
#include "kurbo/point.hpp"
#include "kurbo/vec2.hpp"
#include "kurbo/size.hpp"
#include "kurbo/inline_methods.hpp"
#include <cmath>

using namespace kurbo;

static void assert_approx_eq(double x, double y, double tolerance = 1e-9) {
    EXPECT_NEAR(x, y, tolerance);
}

TEST(RectTest, Basic) {
    Rect r = Rect::new_rect(0.0, 0.0, 10.0, 10.0);
    EXPECT_DOUBLE_EQ(r.width(), 10.0);
    EXPECT_DOUBLE_EQ(r.height(), 10.0);
    EXPECT_DOUBLE_EQ(r.area(), 100.0);
}

TEST(RectTest, FromPoints) {
    Point p1{0.0, 0.0};
    Point p2{10.0, 10.0};
    Rect r = Rect::from_points(p1, p2);
    EXPECT_DOUBLE_EQ(r.x0, 0.0);
    EXPECT_DOUBLE_EQ(r.y0, 0.0);
    EXPECT_DOUBLE_EQ(r.x1, 10.0);
    EXPECT_DOUBLE_EQ(r.y1, 10.0);
}

TEST(RectTest, FromOriginSize) {
    Point origin{1.0, 2.0};
    Size size{10.0, 20.0};
    Rect r = Rect::from_origin_size(origin, size);
    EXPECT_DOUBLE_EQ(r.x0, 1.0);
    EXPECT_DOUBLE_EQ(r.y0, 2.0);
    EXPECT_DOUBLE_EQ(r.x1, 11.0);
    EXPECT_DOUBLE_EQ(r.y1, 22.0);
}

TEST(RectTest, FromCenterSize) {
    Point center{5.0, 5.0};
    Size size{10.0, 20.0};
    Rect r = Rect::from_center_size(center, size);
    EXPECT_DOUBLE_EQ(r.x0, 0.0);
    EXPECT_DOUBLE_EQ(r.y0, -5.0);
    EXPECT_DOUBLE_EQ(r.x1, 10.0);
    EXPECT_DOUBLE_EQ(r.y1, 15.0);
}

TEST(RectTest, Contains) {
    Rect r = Rect::new_rect(0.0, 0.0, 10.0, 10.0);
    Point inside{5.0, 5.0};
    Point outside{15.0, 15.0};
    Point edge{10.0, 5.0};
    
    EXPECT_TRUE(r.contains(inside));
    EXPECT_FALSE(r.contains(outside));
    EXPECT_FALSE(r.contains(edge)); // Edge is not included
}

TEST(RectTest, Union) {
    Rect r1 = Rect::new_rect(0.0, 0.0, 5.0, 5.0);
    Rect r2 = Rect::new_rect(3.0, 3.0, 8.0, 8.0);
    Rect union_rect = r1.union_rect(r2);
    
    EXPECT_DOUBLE_EQ(union_rect.x0, 0.0);
    EXPECT_DOUBLE_EQ(union_rect.y0, 0.0);
    EXPECT_DOUBLE_EQ(union_rect.x1, 8.0);
    EXPECT_DOUBLE_EQ(union_rect.y1, 8.0);
}

TEST(RectTest, Intersect) {
    Rect r1 = Rect::new_rect(0.0, 0.0, 5.0, 5.0);
    Rect r2 = Rect::new_rect(3.0, 3.0, 8.0, 8.0);
    Rect intersect_rect = r1.intersect(r2);
    
    EXPECT_DOUBLE_EQ(intersect_rect.x0, 3.0);
    EXPECT_DOUBLE_EQ(intersect_rect.y0, 3.0);
    EXPECT_DOUBLE_EQ(intersect_rect.x1, 5.0);
    EXPECT_DOUBLE_EQ(intersect_rect.y1, 5.0);
}

TEST(RectTest, Overlaps) {
    Rect r1 = Rect::new_rect(0.0, 0.0, 5.0, 5.0);
    Rect r2 = Rect::new_rect(3.0, 3.0, 8.0, 8.0);
    Rect r3 = Rect::new_rect(10.0, 10.0, 15.0, 15.0);
    
    EXPECT_TRUE(r1.overlaps(r2));
    EXPECT_FALSE(r1.overlaps(r3));
}

TEST(RectTest, ContainsRect) {
    Rect outer = Rect::new_rect(0.0, 0.0, 10.0, 10.0);
    Rect inner = Rect::new_rect(2.0, 2.0, 8.0, 8.0);
    Rect overlapping = Rect::new_rect(5.0, 5.0, 15.0, 15.0);
    
    EXPECT_TRUE(outer.contains_rect(inner));
    EXPECT_FALSE(outer.contains_rect(overlapping));
}

TEST(RectTest, Operators) {
    Rect r = Rect::new_rect(1.0, 2.0, 11.0, 12.0);
    Vec2 v{5.0, 5.0};
    
    Rect translated = r + v;
    EXPECT_DOUBLE_EQ(translated.x0, 6.0);
    EXPECT_DOUBLE_EQ(translated.y0, 7.0);
    EXPECT_DOUBLE_EQ(translated.x1, 16.0);
    EXPECT_DOUBLE_EQ(translated.y1, 17.0);
    
    Rect back = translated - v;
    EXPECT_DOUBLE_EQ(back.x0, r.x0);
    EXPECT_DOUBLE_EQ(back.y0, r.y0);
    EXPECT_DOUBLE_EQ(back.x1, r.x1);
    EXPECT_DOUBLE_EQ(back.y1, r.y1);
}

TEST(RectTest, AspectRatio) {
    Rect square = Rect::new_rect(0.0, 0.0, 10.0, 10.0);
    Rect wide = Rect::new_rect(0.0, 0.0, 20.0, 10.0);
    Rect tall = Rect::new_rect(0.0, 0.0, 10.0, 20.0);
    
    assert_approx_eq(square.aspect_ratio(), 1.0);
    assert_approx_eq(wide.aspect_ratio(), 2.0);
    assert_approx_eq(tall.aspect_ratio(), 0.5);
}

TEST(RectTest, ContainedRectWithAspectRatio) {
    Rect outer = Rect::new_rect(0.0, 0.0, 20.0, 10.0);
    Rect contained = outer.contained_rect_with_aspect_ratio(1.0); // Square
    
    // Should be centered and have aspect ratio 1.0
    assert_approx_eq(contained.aspect_ratio(), 1.0);
    assert_approx_eq(contained.width(), 10.0);
    assert_approx_eq(contained.height(), 10.0);
    assert_approx_eq(contained.center().x, outer.center().x);
    assert_approx_eq(contained.center().y, outer.center().y);
}

TEST(RectTest, UtilityMethods) {
    Rect r = Rect::new_rect(1.0, 2.0, 11.0, 12.0);
    
    EXPECT_TRUE(r.is_finite());
    EXPECT_FALSE(r.is_nan());
    EXPECT_FALSE(r.is_zero_area());
    
    Point origin = r.origin();
    EXPECT_DOUBLE_EQ(origin.x, 1.0);
    EXPECT_DOUBLE_EQ(origin.y, 2.0);
    
    Size size = r.size();
    EXPECT_DOUBLE_EQ(size.width, 10.0);
    EXPECT_DOUBLE_EQ(size.height, 10.0);
    
    Point center = r.center();
    EXPECT_DOUBLE_EQ(center.x, 6.0);
    EXPECT_DOUBLE_EQ(center.y, 7.0);
} 