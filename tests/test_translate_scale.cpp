#include <gtest/gtest.h>
#include "kurbo/translate_scale.hpp"
#include "kurbo/point.hpp"
#include "kurbo/vec2.hpp"
#include "kurbo/affine.hpp"
#include "kurbo/inline_methods.hpp"
#include <cmath>

using namespace kurbo;

static void assert_near(const Point& p0, const Point& p1, double tolerance = 1e-9) {
    double distance = (p1 - p0).hypot();
    EXPECT_NEAR(distance, 0.0, tolerance) << "Points not near: " << p0.x << "," << p0.y << " vs " << p1.x << "," << p1.y;
}

TEST(TranslateScaleTest, Basic) {
    TranslateScale ts = TranslateScale::identity();
    Point p{1.0, 2.0};
    assert_near(ts * p, p);
}

TEST(TranslateScaleTest, TranslateScale) {
    Point p{3.0, 4.0};
    TranslateScale ts = TranslateScale::new_transform(Vec2{5.0, 6.0}, 2.0);
    
    assert_near(ts * p, Point{11.0, 14.0});
}

TEST(TranslateScaleTest, Conversions) {
    Point p{3.0, 4.0};
    double s = 2.0;
    Vec2 t{5.0, 6.0};
    TranslateScale ts = TranslateScale::new_transform(t, s);

    // Test that conversion to affine is consistent
    Affine a = ts.to_affine();
    assert_near(ts * p, a * p);

    assert_near((s * p.to_vec2()).to_point(), TranslateScale::scale_only(s) * p);
    assert_near(p + t, TranslateScale::translate_only(t) * p);
}

TEST(TranslateScaleTest, Inverse) {
    Point p{3.0, 4.0};
    TranslateScale ts = TranslateScale::new_transform(Vec2{5.0, 6.0}, 2.0);

    assert_near(p, (ts * ts.inverse()) * p);
    assert_near(p, (ts.inverse() * ts) * p);
}

TEST(TranslateScaleTest, Operators) {
    TranslateScale ts1 = TranslateScale::new_transform(Vec2{1.0, 2.0}, 2.0);
    TranslateScale ts2 = TranslateScale::new_transform(Vec2{3.0, 4.0}, 3.0);
    
    // Test multiplication
    TranslateScale result = ts1 * ts2;
    EXPECT_DOUBLE_EQ(result.scale, 6.0);
    EXPECT_DOUBLE_EQ(result.translation.x, 7.0);
    EXPECT_DOUBLE_EQ(result.translation.y, 10.0);
    
    // Test compound assignment
    TranslateScale ts3 = ts1;
    ts3 *= ts2;
    EXPECT_DOUBLE_EQ(ts3.scale, result.scale);
    EXPECT_DOUBLE_EQ(ts3.translation.x, result.translation.x);
    EXPECT_DOUBLE_EQ(ts3.translation.y, result.translation.y);
    
    // Test vector addition
    Vec2 v{1.0, 2.0};
    TranslateScale ts4 = ts1 + v;
    EXPECT_DOUBLE_EQ(ts4.scale, ts1.scale);
    EXPECT_DOUBLE_EQ(ts4.translation.x, ts1.translation.x + v.x);
    EXPECT_DOUBLE_EQ(ts4.translation.y, ts1.translation.y + v.y);
    
    // Test vector subtraction
    TranslateScale ts5 = ts1 - v;
    EXPECT_DOUBLE_EQ(ts5.scale, ts1.scale);
    EXPECT_DOUBLE_EQ(ts5.translation.x, ts1.translation.x - v.x);
    EXPECT_DOUBLE_EQ(ts5.translation.y, ts1.translation.y - v.y);
}

TEST(TranslateScaleTest, FreeOperators) {
    TranslateScale ts = TranslateScale::new_transform(Vec2{1.0, 2.0}, 2.0);
    double scalar = 3.0;
    Vec2 v{1.0, 2.0};
    
    // Test scalar multiplication
    TranslateScale result1 = scalar * ts;
    EXPECT_DOUBLE_EQ(result1.scale, ts.scale * scalar);
    EXPECT_DOUBLE_EQ(result1.translation.x, ts.translation.x * scalar);
    EXPECT_DOUBLE_EQ(result1.translation.y, ts.translation.y * scalar);
    
    // Test vector addition (commutative)
    TranslateScale result2 = v + ts;
    TranslateScale result3 = ts + v;
    EXPECT_DOUBLE_EQ(result2.scale, result3.scale);
    EXPECT_DOUBLE_EQ(result2.translation.x, result3.translation.x);
    EXPECT_DOUBLE_EQ(result2.translation.y, result3.translation.y);
}

TEST(TranslateScaleTest, UtilityMethods) {
    TranslateScale ts = TranslateScale::new_transform(Vec2{1.0, 2.0}, 2.0);
    
    // Test is_finite
    EXPECT_TRUE(ts.is_finite());
    
    // Test is_nan
    EXPECT_FALSE(ts.is_nan());
    
    // Test from_scale_about
    Point center{1.0, 1.0};
    TranslateScale ts2 = TranslateScale::from_scale_about(2.0, center);
    assert_near(ts2 * center, center); // Should keep center stationary
    assert_near(ts2 * Point{2.0, 2.0}, Point{3.0, 3.0});
} 