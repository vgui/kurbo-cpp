#include <gtest/gtest.h>
#include "kurbo/vec2.hpp"
#include "kurbo/point.hpp"
#include "kurbo/size.hpp"
#include "kurbo/inline_methods.hpp"

using namespace kurbo;

TEST(Vec2Test, Construction) {
    Vec2 v1(1.0, 2.0);
    EXPECT_EQ(v1.x, 1.0);
    EXPECT_EQ(v1.y, 2.0);
    
    Vec2 v2;
    EXPECT_EQ(v2.x, 0.0);
    EXPECT_EQ(v2.y, 0.0);
    
    EXPECT_EQ(Vec2::ZERO.x, 0.0);
    EXPECT_EQ(Vec2::ZERO.y, 0.0);
}

TEST(Vec2Test, Conversion) {
    Vec2 v(3.0, 4.0);
    
    Point p = v.to_point();
    EXPECT_EQ(p.x, 3.0);
    EXPECT_EQ(p.y, 4.0);
    
    Size s = v.to_size();
    EXPECT_EQ(s.width, 3.0);
    EXPECT_EQ(s.height, 4.0);
}

TEST(Vec2Test, Splat) {
    Vec2 v = Vec2::splat(5.0);
    EXPECT_EQ(v.x, 5.0);
    EXPECT_EQ(v.y, 5.0);
}

TEST(Vec2Test, DotProduct) {
    Vec2 v1(1.0, 2.0);
    Vec2 v2(3.0, 4.0);
    
    double dot = v1.dot(v2);
    EXPECT_EQ(dot, 11.0); // 1*3 + 2*4 = 11
}

TEST(Vec2Test, CrossProduct) {
    Vec2 v1(1.0, 0.0);
    Vec2 v2(0.0, 1.0);
    
    double cross = v1.cross(v2);
    EXPECT_EQ(cross, 1.0);
    
    cross = v2.cross(v1);
    EXPECT_EQ(cross, -1.0);
    
    cross = v1.cross(v1);
    EXPECT_EQ(cross, 0.0);
}

TEST(Vec2Test, Magnitude) {
    Vec2 v(3.0, 4.0);
    
    EXPECT_EQ(v.hypot(), 5.0);
    EXPECT_EQ(v.length(), 5.0);
    EXPECT_EQ(v.hypot2(), 25.0);
    EXPECT_EQ(v.length_squared(), 25.0);
}

TEST(Vec2Test, Angle) {
    Vec2 v1(1.0, 0.0);
    Vec2 v2(0.0, 1.0);
    Vec2 v3(-1.0, 0.0);
    
    EXPECT_NEAR(v1.atan2(), 0.0, 1e-10);
    EXPECT_NEAR(v2.atan2(), M_PI/2, 1e-10);
    EXPECT_NEAR(v3.atan2(), M_PI, 1e-10);
    
    EXPECT_NEAR(v1.angle(), 0.0, 1e-10);
    EXPECT_NEAR(v2.angle(), M_PI/2, 1e-10);
}

TEST(Vec2Test, FromAngle) {
    Vec2 v1 = Vec2::from_angle(0.0);
    EXPECT_NEAR(v1.x, 1.0, 1e-10);
    EXPECT_NEAR(v1.y, 0.0, 1e-10);
    
    Vec2 v2 = Vec2::from_angle(M_PI/2);
    EXPECT_NEAR(v2.x, 0.0, 1e-10);
    EXPECT_NEAR(v2.y, 1.0, 1e-10);
    
    Vec2 v3 = Vec2::from_angle(M_PI);
    EXPECT_NEAR(v3.x, -1.0, 1e-10);
    EXPECT_NEAR(v3.y, 0.0, 1e-10);
}

TEST(Vec2Test, Lerp) {
    Vec2 v1(0.0, 0.0);
    Vec2 v2(4.0, 6.0);
    
    Vec2 v3 = v1.lerp(v2, 0.5);
    EXPECT_EQ(v3.x, 2.0);
    EXPECT_EQ(v3.y, 3.0);
    
    Vec2 v4 = v1.lerp(v2, 0.0);
    EXPECT_EQ(v4.x, 0.0);
    EXPECT_EQ(v4.y, 0.0);
    
    Vec2 v5 = v1.lerp(v2, 1.0);
    EXPECT_EQ(v5.x, 4.0);
    EXPECT_EQ(v5.y, 6.0);
}

TEST(Vec2Test, Normalize) {
    Vec2 v(3.0, 4.0);
    Vec2 normalized = v.normalize();
    
    EXPECT_NEAR(normalized.hypot(), 1.0, 1e-10);
    EXPECT_NEAR(normalized.x, 0.6, 1e-10);
    EXPECT_NEAR(normalized.y, 0.8, 1e-10);
}

TEST(Vec2Test, Rounding) {
    Vec2 v(3.3, 3.6);
    
    Vec2 rounded = v.round();
    EXPECT_EQ(rounded.x, 3.0);
    EXPECT_EQ(rounded.y, 4.0);
    
    Vec2 ceiled = v.ceil();
    EXPECT_EQ(ceiled.x, 4.0);
    EXPECT_EQ(ceiled.y, 4.0);
    
    Vec2 floored = v.floor();
    EXPECT_EQ(floored.x, 3.0);
    EXPECT_EQ(floored.y, 3.0);
    
    Vec2 expanded = v.expand();
    EXPECT_EQ(expanded.x, 4.0);
    EXPECT_EQ(expanded.y, 4.0);
    
    Vec2 truncated = v.trunc();
    EXPECT_EQ(truncated.x, 3.0);
    EXPECT_EQ(truncated.y, 3.0);
}

TEST(Vec2Test, Turn90) {
    Vec2 v(1.0, 2.0);
    Vec2 turned = v.turn_90();
    
    EXPECT_EQ(turned.x, -2.0);
    EXPECT_EQ(turned.y, 1.0);
}

TEST(Vec2Test, RotateScale) {
    Vec2 u(0.1, 0.2);
    Vec2 v(0.3, -0.4);
    Vec2 uv = u.rotate_scale(v);
    double EPSILON = 1e-12;
    EXPECT_NEAR(u.hypot() * v.hypot(), uv.hypot(), EPSILON);
    EXPECT_NEAR(u.angle() + v.angle(), uv.angle(), EPSILON);
}

TEST(Vec2Test, Validation) {
    Vec2 v(1.0, 2.0);
    EXPECT_TRUE(v.is_finite());
    EXPECT_FALSE(v.is_nan());
    
    Vec2 inf_v(std::numeric_limits<double>::infinity(), 1.0);
    EXPECT_FALSE(inf_v.is_finite());
    
    Vec2 nan_v(std::numeric_limits<double>::quiet_NaN(), 1.0);
    EXPECT_TRUE(nan_v.is_nan());
}

TEST(Vec2Test, Arithmetic) {
    Vec2 v1(1.0, 2.0);
    Vec2 v2(3.0, 4.0);
    
    // Addition
    Vec2 v3 = v1 + v2;
    EXPECT_EQ(v3.x, 4.0);
    EXPECT_EQ(v3.y, 6.0);
    
    v1 += v2;
    EXPECT_EQ(v1.x, 4.0);
    EXPECT_EQ(v1.y, 6.0);
    
    // Subtraction
    Vec2 v4 = v2 - v1;
    EXPECT_EQ(v4.x, -1.0);
    EXPECT_EQ(v4.y, -2.0);
    
    v2 -= v1;
    EXPECT_EQ(v2.x, -1.0);
    EXPECT_EQ(v2.y, -2.0);
    
    // Scalar multiplication
    Vec2 v5 = v1 * 2.0;
    EXPECT_EQ(v5.x, 8.0);
    EXPECT_EQ(v5.y, 12.0);
    
    Vec2 v6 = 2.0 * v1;
    EXPECT_EQ(v6.x, 8.0);
    EXPECT_EQ(v6.y, 12.0);
    
    v1 *= 2.0;
    EXPECT_EQ(v1.x, 8.0);
    EXPECT_EQ(v1.y, 12.0);
    
    // Component-wise multiplication
    Vec2 v7 = v1 * Vec2(0.5, 0.25);
    EXPECT_EQ(v7.x, 4.0);
    EXPECT_EQ(v7.y, 3.0);
    
    // Division
    Vec2 v8 = v1 / 2.0;
    EXPECT_EQ(v8.x, 4.0);
    EXPECT_EQ(v8.y, 6.0);
    
    v1 /= 2.0;
    EXPECT_EQ(v1.x, 4.0);
    EXPECT_EQ(v1.y, 6.0);
    
    // Negation
    Vec2 v9 = -v1;
    EXPECT_EQ(v9.x, -4.0);
    EXPECT_EQ(v9.y, -6.0);
}

TEST(Vec2Test, DivisionByZero) {
    Vec2 v(1.0, 2.0);
    Vec2 result = v / 0.0;
    
    EXPECT_TRUE(std::isnan(result.x));
    EXPECT_TRUE(std::isnan(result.y));
}

TEST(Vec2Test, Comparison) {
    Vec2 v1(1.0, 2.0);
    Vec2 v2(1.0, 2.0);
    Vec2 v3(2.0, 1.0);
    
    EXPECT_EQ(v1, v2);
    EXPECT_NE(v1, v3);
    EXPECT_NE(v2, v3);
}

TEST(Vec2Test, ConversionFunctions) {
    std::tuple<double, double> t(3.0, 4.0);
    Vec2 v = Vec2::from_tuple(t);
    EXPECT_EQ(v.x, 3.0);
    EXPECT_EQ(v.y, 4.0);
    
    std::tuple<double, double> t2 = v.to_tuple();
    EXPECT_EQ(std::get<0>(t2), 3.0);
    EXPECT_EQ(std::get<1>(t2), 4.0);
}

TEST(Vec2Test, StreamOutput) {
    Vec2 v(1.5, 2.5);
    std::ostringstream oss;
    oss << v;
    EXPECT_EQ(oss.str(), "Vec2(1.5, 2.5)");
} 