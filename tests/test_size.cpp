#include <gtest/gtest.h>
#include "kurbo/size.hpp"
#include "kurbo/vec2.hpp"
#include "kurbo/inline_methods.hpp"

using namespace kurbo;

TEST(SizeTest, Construction) {
    Size s1(10.0, 20.0);
    EXPECT_EQ(s1.width, 10.0);
    EXPECT_EQ(s1.height, 20.0);
    
    Size s2;
    EXPECT_EQ(s2.width, 0.0);
    EXPECT_EQ(s2.height, 0.0);
    
    EXPECT_EQ(Size::ZERO.width, 0.0);
    EXPECT_EQ(Size::ZERO.height, 0.0);
    
    EXPECT_TRUE(std::isinf(Size::INFINITE.width));
    EXPECT_TRUE(std::isinf(Size::INFINITE.height));
}

TEST(SizeTest, Conversion) {
    Size s(3.0, 4.0);
    Vec2 v = s.to_vec2();
    EXPECT_EQ(v.x, 3.0);
    EXPECT_EQ(v.y, 4.0);
}

TEST(SizeTest, MinMaxSides) {
    Size s1(10.0, 20.0);
    EXPECT_EQ(s1.max_side(), 20.0);
    EXPECT_EQ(s1.min_side(), 10.0);
    
    Size s2(30.0, 15.0);
    EXPECT_EQ(s2.max_side(), 30.0);
    EXPECT_EQ(s2.min_side(), 15.0);
    
    Size s3(5.0, 5.0);
    EXPECT_EQ(s3.max_side(), 5.0);
    EXPECT_EQ(s3.min_side(), 5.0);
}

TEST(SizeTest, Area) {
    Size s(5.0, 4.0);
    EXPECT_EQ(s.area(), 20.0);
    
    Size zero(0.0, 0.0);
    EXPECT_EQ(zero.area(), 0.0);
    
    Size negative(-2.0, 3.0);
    EXPECT_EQ(negative.area(), -6.0);
}

TEST(SizeTest, EmptyCheck) {
    Size zero(0.0, 0.0);
    EXPECT_TRUE(zero.is_zero_area());
    EXPECT_TRUE(zero.is_empty());
    
    Size non_zero(1.0, 1.0);
    EXPECT_FALSE(non_zero.is_zero_area());
    EXPECT_FALSE(non_zero.is_empty());
    
    Size negative(-2.0, 3.0);
    EXPECT_FALSE(negative.is_zero_area());
    EXPECT_FALSE(negative.is_empty());
}

TEST(SizeTest, MinMax) {
    Size s1(10.0, 20.0);
    Size s2(5.0, 25.0);
    
    Size min_result = s1.min(s2);
    EXPECT_EQ(min_result.width, 5.0);
    EXPECT_EQ(min_result.height, 20.0);
    
    Size max_result = s1.max(s2);
    EXPECT_EQ(max_result.width, 10.0);
    EXPECT_EQ(max_result.height, 25.0);
}

TEST(SizeTest, Clamp) {
    Size s(15.0, 25.0);
    Size min(10.0, 20.0);
    Size max(20.0, 30.0);
    
    Size clamped = s.clamp(min, max);
    EXPECT_EQ(clamped.width, 15.0);
    EXPECT_EQ(clamped.height, 25.0);
    
    Size too_small(5.0, 15.0);
    Size clamped_small = too_small.clamp(min, max);
    EXPECT_EQ(clamped_small.width, 10.0);
    EXPECT_EQ(clamped_small.height, 20.0);
    
    Size too_large(25.0, 35.0);
    Size clamped_large = too_large.clamp(min, max);
    EXPECT_EQ(clamped_large.width, 20.0);
    EXPECT_EQ(clamped_large.height, 30.0);
}

TEST(SizeTest, Rounding) {
    Size s(3.3, 3.6);
    
    Size rounded = s.round();
    EXPECT_EQ(rounded.width, 3.0);
    EXPECT_EQ(rounded.height, 4.0);
    
    Size ceiled = s.ceil();
    EXPECT_EQ(ceiled.width, 4.0);
    EXPECT_EQ(ceiled.height, 4.0);
    
    Size floored = s.floor();
    EXPECT_EQ(floored.width, 3.0);
    EXPECT_EQ(floored.height, 3.0);
    
    Size expanded = s.expand();
    EXPECT_EQ(expanded.width, 4.0);
    EXPECT_EQ(expanded.height, 4.0);
    
    Size truncated = s.trunc();
    EXPECT_EQ(truncated.width, 3.0);
    EXPECT_EQ(truncated.height, 3.0);
}

TEST(SizeTest, AspectRatio) {
    Size s1(16.0, 9.0);
    EXPECT_NEAR(s1.aspect_ratio(), 16.0/9.0, 1e-10);
    
    Size s2(4.0, 3.0);
    EXPECT_NEAR(s2.aspect_ratio(), 4.0/3.0, 1e-10);
    
    Size s3(1.0, 1.0);
    EXPECT_NEAR(s3.aspect_ratio(), 1.0, 1e-10);
}

TEST(SizeTest, Validation) {
    Size s(1.0, 2.0);
    EXPECT_TRUE(s.is_finite());
    EXPECT_FALSE(s.is_nan());
    
    Size inf_s(std::numeric_limits<double>::infinity(), 1.0);
    EXPECT_FALSE(inf_s.is_finite());
    
    Size nan_s(std::numeric_limits<double>::quiet_NaN(), 1.0);
    EXPECT_TRUE(nan_s.is_nan());
}

TEST(SizeTest, Arithmetic) {
    Size s1(10.0, 20.0);
    Size s2(5.0, 10.0);
    
    // Addition
    Size s3 = s1 + s2;
    EXPECT_EQ(s3.width, 15.0);
    EXPECT_EQ(s3.height, 30.0);
    
    s1 += s2;
    EXPECT_EQ(s1.width, 15.0);
    EXPECT_EQ(s1.height, 30.0);
    
    // Subtraction
    Size s4 = s1 - s2;
    EXPECT_EQ(s4.width, 10.0);
    EXPECT_EQ(s4.height, 20.0);
    
    s1 -= s2;
    EXPECT_EQ(s1.width, 10.0);
    EXPECT_EQ(s1.height, 20.0);
    
    // Scalar multiplication
    Size s5 = s1 * 2.0;
    EXPECT_EQ(s5.width, 20.0);
    EXPECT_EQ(s5.height, 40.0);
    
    Size s6 = 2.0 * s1;
    EXPECT_EQ(s6.width, 20.0);
    EXPECT_EQ(s6.height, 40.0);
    
    s1 *= 2.0;
    EXPECT_EQ(s1.width, 20.0);
    EXPECT_EQ(s1.height, 40.0);
    
    // Component-wise multiplication
    Size s7 = s1 * s2;
    EXPECT_EQ(s7.width, 100.0);
    EXPECT_EQ(s7.height, 400.0);
    
    // Division
    Size s8 = s1 / 2.0;
    EXPECT_EQ(s8.width, 10.0);
    EXPECT_EQ(s8.height, 20.0);
    
    s1 /= 2.0;
    EXPECT_EQ(s1.width, 10.0);
    EXPECT_EQ(s1.height, 20.0);
}

TEST(SizeTest, DivisionByZero) {
    Size s(1.0, 2.0);
    Size result = s / 0.0;
    
    EXPECT_TRUE(std::isnan(result.width));
    EXPECT_TRUE(std::isnan(result.height));
}

TEST(SizeTest, Comparison) {
    Size s1(10.0, 20.0);
    Size s2(10.0, 20.0);
    Size s3(20.0, 10.0);
    
    EXPECT_EQ(s1, s2);
    EXPECT_NE(s1, s3);
    EXPECT_NE(s2, s3);
}

TEST(SizeTest, ConversionFunctions) {
    std::tuple<double, double> t(3.0, 4.0);
    Size s = Size::from_tuple(t);
    EXPECT_EQ(s.width, 3.0);
    EXPECT_EQ(s.height, 4.0);
    
    std::tuple<double, double> t2 = s.to_tuple();
    EXPECT_EQ(std::get<0>(t2), 3.0);
    EXPECT_EQ(std::get<1>(t2), 4.0);
}

TEST(SizeTest, StreamOutput) {
    Size s(10.5, 20.5);
    std::ostringstream oss;
    oss << s;
    EXPECT_EQ(oss.str(), "Size(10.5, 20.5)");
} 