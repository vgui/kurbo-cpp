#include <gtest/gtest.h>
#include "kurbo/rounded_rect_radii.hpp"
#include <cmath>

using namespace kurbo;

static void assert_approx_eq(double x, double y, double tolerance = 1e-9) {
    EXPECT_NEAR(x, y, tolerance);
}

TEST(RoundedRectRadiiTest, Basic) {
    RoundedRectRadii radii(1.0, 2.0, 3.0, 4.0);
    EXPECT_DOUBLE_EQ(radii.top_left, 1.0);
    EXPECT_DOUBLE_EQ(radii.top_right, 2.0);
    EXPECT_DOUBLE_EQ(radii.bottom_right, 3.0);
    EXPECT_DOUBLE_EQ(radii.bottom_left, 4.0);
}

TEST(RoundedRectRadiiTest, UniformRadius) {
    RoundedRectRadii radii(5.0);
    EXPECT_DOUBLE_EQ(radii.top_left, 5.0);
    EXPECT_DOUBLE_EQ(radii.top_right, 5.0);
    EXPECT_DOUBLE_EQ(radii.bottom_right, 5.0);
    EXPECT_DOUBLE_EQ(radii.bottom_left, 5.0);
}

TEST(RoundedRectRadiiTest, NewRadii) {
    RoundedRectRadii radii = RoundedRectRadii::new_radii(1.0, 2.0, 3.0, 4.0);
    EXPECT_DOUBLE_EQ(radii.top_left, 1.0);
    EXPECT_DOUBLE_EQ(radii.top_right, 2.0);
    EXPECT_DOUBLE_EQ(radii.bottom_right, 3.0);
    EXPECT_DOUBLE_EQ(radii.bottom_left, 4.0);
}

TEST(RoundedRectRadiiTest, FromSingleRadius) {
    RoundedRectRadii radii = RoundedRectRadii::from_single_radius(3.0);
    EXPECT_DOUBLE_EQ(radii.top_left, 3.0);
    EXPECT_DOUBLE_EQ(radii.top_right, 3.0);
    EXPECT_DOUBLE_EQ(radii.bottom_right, 3.0);
    EXPECT_DOUBLE_EQ(radii.bottom_left, 3.0);
}

TEST(RoundedRectRadiiTest, Abs) {
    RoundedRectRadii radii(-1.0, -2.0, -3.0, -4.0);
    RoundedRectRadii abs_radii = radii.abs();
    EXPECT_DOUBLE_EQ(abs_radii.top_left, 1.0);
    EXPECT_DOUBLE_EQ(abs_radii.top_right, 2.0);
    EXPECT_DOUBLE_EQ(abs_radii.bottom_right, 3.0);
    EXPECT_DOUBLE_EQ(abs_radii.bottom_left, 4.0);
}

TEST(RoundedRectRadiiTest, Clamp) {
    RoundedRectRadii radii(5.0, 10.0, 15.0, 20.0);
    RoundedRectRadii clamped = radii.clamp(12.0);
    EXPECT_DOUBLE_EQ(clamped.top_left, 5.0);
    EXPECT_DOUBLE_EQ(clamped.top_right, 10.0);
    EXPECT_DOUBLE_EQ(clamped.bottom_right, 12.0);
    EXPECT_DOUBLE_EQ(clamped.bottom_left, 12.0);
}

TEST(RoundedRectRadiiTest, IsFinite) {
    RoundedRectRadii finite_radii(1.0, 2.0, 3.0, 4.0);
    EXPECT_TRUE(finite_radii.is_finite());
    
    RoundedRectRadii inf_radii(std::numeric_limits<double>::infinity(), 2.0, 3.0, 4.0);
    EXPECT_FALSE(inf_radii.is_finite());
}

TEST(RoundedRectRadiiTest, IsNan) {
    RoundedRectRadii normal_radii(1.0, 2.0, 3.0, 4.0);
    EXPECT_FALSE(normal_radii.is_nan());
    
    RoundedRectRadii nan_radii(std::nan(""), 2.0, 3.0, 4.0);
    EXPECT_TRUE(nan_radii.is_nan());
}

TEST(RoundedRectRadiiTest, AsSingleRadius) {
    // All radii equal
    RoundedRectRadii uniform_radii(3.0, 3.0, 3.0, 3.0);
    auto single_radius = uniform_radii.as_single_radius();
    EXPECT_TRUE(single_radius.has_value());
    EXPECT_DOUBLE_EQ(single_radius.value(), 3.0);
    
    // Different radii
    RoundedRectRadii different_radii(1.0, 2.0, 3.0, 4.0);
    auto no_single_radius = different_radii.as_single_radius();
    EXPECT_FALSE(no_single_radius.has_value());
    
    // Nearly equal radii (within epsilon)
    RoundedRectRadii nearly_equal(1.0, 1.0 + 1e-10, 1.0, 1.0);
    auto nearly_single_radius = nearly_equal.as_single_radius();
    EXPECT_TRUE(nearly_single_radius.has_value());
    assert_approx_eq(nearly_single_radius.value(), 1.0);
}

TEST(RoundedRectRadiiTest, Zero) {
    RoundedRectRadii zero = RoundedRectRadii::zero();
    EXPECT_DOUBLE_EQ(zero.top_left, 0.0);
    EXPECT_DOUBLE_EQ(zero.top_right, 0.0);
    EXPECT_DOUBLE_EQ(zero.bottom_right, 0.0);
    EXPECT_DOUBLE_EQ(zero.bottom_left, 0.0);
} 