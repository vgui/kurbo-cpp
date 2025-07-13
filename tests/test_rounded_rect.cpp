#include <gtest/gtest.h>
#include "kurbo/rounded_rect.hpp"
#include "kurbo/point.hpp"
#include "kurbo/vec2.hpp"
#include "kurbo/size.hpp"
#include "kurbo/rect.hpp"
#include "kurbo/inline_methods.hpp"
#include <cmath>

using namespace kurbo;

static void assert_approx_eq(double x, double y, double tolerance = 1e-9) {
    EXPECT_NEAR(x, y, tolerance);
}

TEST(RoundedRectTest, Basic) {
    RoundedRect rr = RoundedRect::new_rounded_rect(0.0, 0.0, 10.0, 10.0, 2.0);
    EXPECT_DOUBLE_EQ(rr.width(), 10.0);
    EXPECT_DOUBLE_EQ(rr.height(), 10.0);
    EXPECT_DOUBLE_EQ(rr.get_radii().top_left, 2.0);
    EXPECT_DOUBLE_EQ(rr.get_radii().top_right, 2.0);
    EXPECT_DOUBLE_EQ(rr.get_radii().bottom_right, 2.0);
    EXPECT_DOUBLE_EQ(rr.get_radii().bottom_left, 2.0);
}

TEST(RoundedRectTest, FromRect) {
    Rect rect = Rect::new_rect(1.0, 2.0, 11.0, 12.0);
    RoundedRect rr = RoundedRect::from_rect(rect, 3.0);
    
    EXPECT_DOUBLE_EQ(rr.get_rect().x0, rect.x0);
    EXPECT_DOUBLE_EQ(rr.get_rect().y0, rect.y0);
    EXPECT_DOUBLE_EQ(rr.get_rect().x1, rect.x1);
    EXPECT_DOUBLE_EQ(rr.get_rect().y1, rect.y1);
    EXPECT_DOUBLE_EQ(rr.get_radii().top_left, 3.0);
}

TEST(RoundedRectTest, FromPoints) {
    Point p1{0.0, 0.0};
    Point p2{10.0, 10.0};
    RoundedRect rr = RoundedRect::from_points(p1, p2, 2.0);
    
    EXPECT_DOUBLE_EQ(rr.get_rect().x0, 0.0);
    EXPECT_DOUBLE_EQ(rr.get_rect().y0, 0.0);
    EXPECT_DOUBLE_EQ(rr.get_rect().x1, 10.0);
    EXPECT_DOUBLE_EQ(rr.get_rect().y1, 10.0);
    EXPECT_DOUBLE_EQ(rr.get_radii().top_left, 2.0);
}

TEST(RoundedRectTest, FromOriginSize) {
    Point origin{1.0, 2.0};
    Size size{10.0, 20.0};
    RoundedRect rr = RoundedRect::from_origin_size(origin, size, 3.0);
    
    EXPECT_DOUBLE_EQ(rr.get_rect().x0, 1.0);
    EXPECT_DOUBLE_EQ(rr.get_rect().y0, 2.0);
    EXPECT_DOUBLE_EQ(rr.get_rect().x1, 11.0);
    EXPECT_DOUBLE_EQ(rr.get_rect().y1, 22.0);
    EXPECT_DOUBLE_EQ(rr.get_radii().top_left, 3.0);
}

TEST(RoundedRectTest, RadiiClamping) {
    // Test that radii are clamped to half the shortest side
    Rect rect = Rect::new_rect(0.0, 0.0, 10.0, 5.0); // width=10, height=5
    RoundedRect rr = RoundedRect::from_rect(rect, 10.0); // radius larger than half height
    
    // Should be clamped to 2.5 (half of height=5)
    EXPECT_DOUBLE_EQ(rr.get_radii().top_left, 2.5);
    EXPECT_DOUBLE_EQ(rr.get_radii().top_right, 2.5);
    EXPECT_DOUBLE_EQ(rr.get_radii().bottom_right, 2.5);
    EXPECT_DOUBLE_EQ(rr.get_radii().bottom_left, 2.5);
}

TEST(RoundedRectTest, Properties) {
    RoundedRect rr = RoundedRect::new_rounded_rect(1.0, 2.0, 11.0, 12.0, 2.0);
    
    Point origin = rr.origin();
    EXPECT_DOUBLE_EQ(origin.x, 1.0);
    EXPECT_DOUBLE_EQ(origin.y, 2.0);
    
    Point center = rr.center();
    EXPECT_DOUBLE_EQ(center.x, 6.0);
    EXPECT_DOUBLE_EQ(center.y, 7.0);
    
    Rect rect = rr.get_rect();
    EXPECT_DOUBLE_EQ(rect.x0, 1.0);
    EXPECT_DOUBLE_EQ(rect.y0, 2.0);
    EXPECT_DOUBLE_EQ(rect.x1, 11.0);
    EXPECT_DOUBLE_EQ(rect.y1, 12.0);
}

TEST(RoundedRectTest, Operators) {
    RoundedRect rr = RoundedRect::new_rounded_rect(1.0, 2.0, 11.0, 12.0, 2.0);
    Vec2 v{5.0, 5.0};
    
    RoundedRect translated = rr + v;
    EXPECT_DOUBLE_EQ(translated.get_rect().x0, 6.0);
    EXPECT_DOUBLE_EQ(translated.get_rect().y0, 7.0);
    EXPECT_DOUBLE_EQ(translated.get_rect().x1, 16.0);
    EXPECT_DOUBLE_EQ(translated.get_rect().y1, 17.0);
    // Radii should remain the same
    EXPECT_DOUBLE_EQ(translated.get_radii().top_left, 2.0);
    
    RoundedRect back = translated - v;
    EXPECT_DOUBLE_EQ(back.get_rect().x0, rr.get_rect().x0);
    EXPECT_DOUBLE_EQ(back.get_rect().y0, rr.get_rect().y0);
    EXPECT_DOUBLE_EQ(back.get_rect().x1, rr.get_rect().x1);
    EXPECT_DOUBLE_EQ(back.get_rect().y1, rr.get_rect().y1);
}

TEST(RoundedRectTest, UtilityMethods) {
    RoundedRect rr = RoundedRect::new_rounded_rect(1.0, 2.0, 11.0, 12.0, 2.0);
    
    EXPECT_TRUE(rr.is_finite());
    EXPECT_FALSE(rr.is_nan());
    
    // Test with NaN values
    RoundedRectRadii nan_radii{std::nan(""), std::nan(""), std::nan(""), std::nan("")};
    RoundedRect nan_rr(Rect::new_rect(1.0, 2.0, 11.0, 12.0), nan_radii);
    EXPECT_TRUE(nan_rr.is_nan());
}

TEST(RoundedRectTest, RectToRoundedRect) {
    Rect rect = Rect::new_rect(0.0, 0.0, 10.0, 10.0);
    RoundedRect rr = rect.to_rounded_rect(3.0);
    
    EXPECT_DOUBLE_EQ(rr.get_rect().x0, rect.x0);
    EXPECT_DOUBLE_EQ(rr.get_rect().y0, rect.y0);
    EXPECT_DOUBLE_EQ(rr.get_rect().x1, rect.x1);
    EXPECT_DOUBLE_EQ(rr.get_rect().y1, rect.y1);
    EXPECT_DOUBLE_EQ(rr.get_radii().top_left, 3.0);
    EXPECT_DOUBLE_EQ(rr.get_radii().top_right, 3.0);
    EXPECT_DOUBLE_EQ(rr.get_radii().bottom_right, 3.0);
    EXPECT_DOUBLE_EQ(rr.get_radii().bottom_left, 3.0);
}

TEST(RoundedRectTest, Zero) {
    RoundedRect zero = RoundedRect::zero();
    EXPECT_DOUBLE_EQ(zero.get_rect().x0, 0.0);
    EXPECT_DOUBLE_EQ(zero.get_rect().y0, 0.0);
    EXPECT_DOUBLE_EQ(zero.get_rect().x1, 0.0);
    EXPECT_DOUBLE_EQ(zero.get_rect().y1, 0.0);
    EXPECT_DOUBLE_EQ(zero.get_radii().top_left, 0.0);
    EXPECT_DOUBLE_EQ(zero.get_radii().top_right, 0.0);
    EXPECT_DOUBLE_EQ(zero.get_radii().bottom_right, 0.0);
    EXPECT_DOUBLE_EQ(zero.get_radii().bottom_left, 0.0);
} 