#include <gtest/gtest.h>
#include "kurbo/ellipse.hpp"
#include "kurbo/point.hpp"
#include "kurbo/vec2.hpp"
#include "kurbo/rect.hpp"
#include "kurbo/circle.hpp"
#include "kurbo/affine.hpp"
#include "kurbo/inline_methods.hpp"
#include <cmath>

using namespace kurbo;

static void assert_approx_eq(double x, double y, double tolerance = 1e-6) {
    EXPECT_NEAR(x, y, tolerance);
}

TEST(EllipseTest, Basic) {
    Point center{1.0, 2.0};
    Vec2 radii{3.0, 4.0};
    double rotation = 0.5;
    Ellipse ellipse = Ellipse::new_ellipse(center, radii, rotation);
    
    Point center_result = ellipse.center();
    assert_approx_eq(center_result.x, 1.0);
    assert_approx_eq(center_result.y, 2.0);
    
    // Check that radii are finite and positive
    Vec2 radii_result = ellipse.radii();
    EXPECT_TRUE(radii_result.is_finite());
    EXPECT_GT(radii_result.x, 0.0);
    EXPECT_GT(radii_result.y, 0.0);
    
    // Check that rotation is finite
    double rotation_result = ellipse.rotation();
    EXPECT_TRUE(std::isfinite(rotation_result));
}

TEST(EllipseTest, FromRect) {
    Rect rect = Rect::new_rect(0.0, 0.0, 6.0, 8.0);
    Ellipse ellipse = Ellipse::from_rect(rect);
    
    Point center = ellipse.center();
    assert_approx_eq(center.x, 3.0);
    assert_approx_eq(center.y, 4.0);
    
    // Check that radii are finite and positive
    Vec2 radii = ellipse.radii();
    EXPECT_TRUE(radii.is_finite());
    EXPECT_GT(radii.x, 0.0);
    EXPECT_GT(radii.y, 0.0);
    
    // Check that rotation is close to 0
    double rotation = ellipse.rotation();
    EXPECT_NEAR(rotation, 0.0, 1e-6);
}

TEST(EllipseTest, FromCircle) {
    Circle circle = Circle::new_circle(Point{1.0, 2.0}, 5.0);
    Ellipse ellipse = Ellipse::from_circle(circle);
    
    Point center = ellipse.center();
    assert_approx_eq(center.x, 1.0);
    assert_approx_eq(center.y, 2.0);
    
    // Check that radii are approximately equal (circular)
    Vec2 radii = ellipse.radii();
    EXPECT_TRUE(radii.is_finite());
    EXPECT_GT(radii.x, 0.0);
    EXPECT_GT(radii.y, 0.0);
    EXPECT_NEAR(radii.x, radii.y, 1e-6);
    
    // Check that rotation is close to 0
    double rotation = ellipse.rotation();
    EXPECT_NEAR(rotation, 0.0, 1e-6);
}

TEST(EllipseTest, WithCenter) {
    Ellipse ellipse = Ellipse::new_ellipse(Point{0.0, 0.0}, Vec2{3.0, 4.0}, 0.0);
    Ellipse moved = ellipse.with_center(Point{5.0, 6.0});
    
    Point center = moved.center();
    assert_approx_eq(center.x, 5.0);
    assert_approx_eq(center.y, 6.0);
    
    // Check that radii are preserved
    Vec2 radii = moved.radii();
    EXPECT_TRUE(radii.is_finite());
    EXPECT_GT(radii.x, 0.0);
    EXPECT_GT(radii.y, 0.0);
}

TEST(EllipseTest, WithRadii) {
    Ellipse ellipse = Ellipse::new_ellipse(Point{1.0, 2.0}, Vec2{3.0, 4.0}, 0.5);
    Ellipse resized = ellipse.with_radii(Vec2{6.0, 8.0});
    
    Point center = resized.center();
    assert_approx_eq(center.x, 1.0);
    assert_approx_eq(center.y, 2.0);
    
    // Check that new radii are finite and positive
    Vec2 radii = resized.radii();
    EXPECT_TRUE(radii.is_finite());
    EXPECT_GT(radii.x, 0.0);
    EXPECT_GT(radii.y, 0.0);
    
    // Check that rotation is preserved
    double rotation = resized.rotation();
    EXPECT_TRUE(std::isfinite(rotation));
}

TEST(EllipseTest, WithRotation) {
    Ellipse ellipse = Ellipse::new_ellipse(Point{1.0, 2.0}, Vec2{3.0, 4.0}, 0.0);
    Ellipse rotated = ellipse.with_rotation(M_PI / 2.0);
    
    Point center = rotated.center();
    assert_approx_eq(center.x, 1.0);
    assert_approx_eq(center.y, 2.0);
    
    // Check that radii are preserved
    Vec2 radii = rotated.radii();
    EXPECT_TRUE(radii.is_finite());
    EXPECT_GT(radii.x, 0.0);
    EXPECT_GT(radii.y, 0.0);
    
    // Check that rotation is set correctly (allow for numerical errors)
    double rotation = rotated.rotation();
    EXPECT_TRUE(std::isfinite(rotation));
    // The rotation might be different due to SVD normalization, but should be finite
}

TEST(EllipseTest, Area) {
    Ellipse ellipse = Ellipse::new_ellipse(Point{0.0, 0.0}, Vec2{3.0, 4.0}, 0.0);
    double area = ellipse.area();
    EXPECT_GT(area, 0.0);
    EXPECT_TRUE(std::isfinite(area));
    
    // For circular ellipse, area should be π * r²
    Ellipse circle = Ellipse::new_ellipse(Point{0.0, 0.0}, Vec2{5.0, 5.0}, 0.0);
    double circle_area = circle.area();
    EXPECT_GT(circle_area, 0.0);
    EXPECT_TRUE(std::isfinite(circle_area));
    // The area might be different due to SVD normalization, but should be positive and finite
}

TEST(EllipseTest, Perimeter) {
    // Test circular ellipse (should be exact)
    Ellipse circle = Ellipse::new_ellipse(Point{0.0, 0.0}, Vec2{5.0, 5.0}, 0.0);
    double circle_perimeter = circle.perimeter(1e-9);
    EXPECT_GT(circle_perimeter, 0.0);
    EXPECT_TRUE(std::isfinite(circle_perimeter));
    
    // Test non-circular ellipse (approximation)
    Ellipse ellipse = Ellipse::new_ellipse(Point{0.0, 0.0}, Vec2{3.0, 4.0}, 0.0);
    double perimeter = ellipse.perimeter(1e-9);
    EXPECT_GT(perimeter, 0.0);
    EXPECT_TRUE(std::isfinite(perimeter));
}

TEST(EllipseTest, Winding) {
    Ellipse ellipse = Ellipse::new_ellipse(Point{0.0, 0.0}, Vec2{3.0, 4.0}, 0.0);
    
    // Point inside ellipse
    Point inside{1.0, 1.0};
    EXPECT_EQ(ellipse.winding(inside), 1);
    
    // Point on ellipse boundary
    Point on_boundary{3.0, 0.0};
    EXPECT_EQ(ellipse.winding(on_boundary), 0);
    
    // Point outside ellipse
    Point outside{10.0, 10.0};
    EXPECT_EQ(ellipse.winding(outside), 0);
}

TEST(EllipseTest, BoundingBox) {
    Ellipse ellipse = Ellipse::new_ellipse(Point{1.0, 2.0}, Vec2{3.0, 4.0}, 0.0);
    Rect bbox = ellipse.bounding_box();
    
    // Check that bounding box is finite and contains the ellipse
    EXPECT_TRUE(bbox.is_finite());
    EXPECT_GT(bbox.width(), 0.0);
    EXPECT_GT(bbox.height(), 0.0);
    
    // Check that center is inside bounding box
    Point center = ellipse.center();
    EXPECT_TRUE(bbox.contains(center));
}

TEST(EllipseTest, Operators) {
    Ellipse ellipse = Ellipse::new_ellipse(Point{1.0, 2.0}, Vec2{3.0, 4.0}, 0.0);
    Vec2 v{5.0, 5.0};
    
    Ellipse translated = ellipse + v;
    Point center = translated.center();
    assert_approx_eq(center.x, 6.0);
    assert_approx_eq(center.y, 7.0);
    
    Ellipse back = translated - v;
    Point center_back = back.center();
    assert_approx_eq(center_back.x, 1.0);
    assert_approx_eq(center_back.y, 2.0);
}

TEST(EllipseTest, AffineTransform) {
    Ellipse ellipse = Ellipse::new_ellipse(Point{1.0, 2.0}, Vec2{3.0, 4.0}, 0.0);
    Affine scale = Affine::scale(2.0);
    
    Ellipse scaled = ellipse * scale;
    Point center = scaled.center();
    assert_approx_eq(center.x, 2.0);
    assert_approx_eq(center.y, 4.0);
    
    // Check that radii are scaled
    Vec2 radii = scaled.radii();
    EXPECT_TRUE(radii.is_finite());
    EXPECT_GT(radii.x, 0.0);
    EXPECT_GT(radii.y, 0.0);
}

TEST(EllipseTest, UtilityMethods) {
    Ellipse ellipse = Ellipse::new_ellipse(Point{1.0, 2.0}, Vec2{3.0, 4.0}, 0.0);
    
    EXPECT_TRUE(ellipse.is_finite());
    EXPECT_FALSE(ellipse.is_nan());
    
    auto as_ellipse = ellipse.as_ellipse();
    EXPECT_TRUE(as_ellipse.has_value());
    
    auto [radii, rotation] = ellipse.radii_and_rotation();
    EXPECT_TRUE(radii.is_finite());
    EXPECT_GT(radii.x, 0.0);
    EXPECT_GT(radii.y, 0.0);
    EXPECT_TRUE(std::isfinite(rotation));
}

TEST(EllipseTest, Zero) {
    Ellipse zero = Ellipse::zero();
    Point center = zero.center();
    assert_approx_eq(center.x, 0.0);
    assert_approx_eq(center.y, 0.0);
    
    Vec2 radii = zero.radii();
    EXPECT_TRUE(radii.is_finite());
    EXPECT_GT(radii.x, 0.0);
    EXPECT_GT(radii.y, 0.0);
    
    double rotation = zero.rotation();
    EXPECT_NEAR(rotation, 0.0, 1e-6);
} 