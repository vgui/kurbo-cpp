#include <gtest/gtest.h>
#include "kurbo/affine.hpp"
#include "kurbo/point.hpp"
#include "kurbo/vec2.hpp"
#include <cmath>

using namespace kurbo;

// Helper function to assert points are near each other
static void assert_near(const Point& p0, const Point& p1, double tolerance = 1e-9) {
    double distance = (p1 - p0).hypot();
    EXPECT_NEAR(distance, 0.0, tolerance) << "Points not near: " << p0.x << "," << p0.y << " vs " << p1.x << "," << p1.y;
}

// Helper function to assert affine transforms are near each other
void affine_assert_near(const Affine& a0, const Affine& a1, double tolerance = 1e-9) {
    auto coeffs0 = a0.as_coeffs();
    auto coeffs1 = a1.as_coeffs();
    for (int i = 0; i < 6; ++i) {
        EXPECT_NEAR(coeffs0[i], coeffs1[i], tolerance) << "Coefficient " << i << " differs";
    }
}

TEST(AffineTest, Identity) {
    Affine a = Affine::identity();
    Point p{1.0, 2.0};
    Point res = a * p;
    EXPECT_DOUBLE_EQ(res.x, 1.0);
    EXPECT_DOUBLE_EQ(res.y, 2.0);
}

TEST(AffineTest, AffineBasic) {
    Point p{3.0, 4.0};
    const double PI = M_PI;

    // Test identity
    assert_near(Affine::identity() * p, p);
    
    // Test scale
    assert_near(Affine::scale(2.0) * p, Point{6.0, 8.0});
    
    // Test rotation
    assert_near(Affine::rotate(0.0) * p, p);
    assert_near(Affine::rotate(PI / 2.0) * p, Point{-4.0, 3.0});
    
    // Test translation
    assert_near(Affine::translate(Vec2{5.0, 6.0}) * p, Point{8.0, 10.0});
    
    // Test skew
    assert_near(Affine::skew(0.0, 0.0) * p, p);
    assert_near(Affine::skew(2.0, 4.0) * p, Point{11.0, 16.0});
}

TEST(AffineTest, AffineMul) {
    Affine a1 = Affine({1.0, 2.0, 3.0, 4.0, 5.0, 6.0});
    Affine a2 = Affine({0.1, 1.2, 2.3, 3.4, 4.5, 5.6});

    Point px{1.0, 0.0};
    Point py{0.0, 1.0};
    Point pxy{1.0, 1.0};
    
    assert_near(a1 * (a2 * px), (a1 * a2) * px);
    assert_near(a1 * (a2 * py), (a1 * a2) * py);
    assert_near(a1 * (a2 * pxy), (a1 * a2) * pxy);
}

TEST(AffineTest, AffineInv) {
    Affine a = Affine({0.1, 1.2, 2.3, 3.4, 4.5, 5.6});
    Affine a_inv = a.inverse();

    Point px{1.0, 0.0};
    Point py{0.0, 1.0};
    Point pxy{1.0, 1.0};
    
    assert_near(a * (a_inv * px), px);
    assert_near(a * (a_inv * py), py);
    assert_near(a * (a_inv * pxy), pxy);
    assert_near(a_inv * (a * px), px);
    assert_near(a_inv * (a * py), py);
    assert_near(a_inv * (a * pxy), pxy);
}

TEST(AffineTest, Reflection) {
    // Test reflection about x-axis
    affine_assert_near(
        Affine::reflect(Point{0, 0}, Vec2{1, 0}),
        Affine({1, 0, 0, -1, 0, 0})
    );
    
    // Test reflection about y-axis
    affine_assert_near(
        Affine::reflect(Point{0, 0}, Vec2{0, 1}),
        Affine({-1, 0, 0, 1, 0, 0})
    );
    
    // Test reflection about y = x line
    affine_assert_near(
        Affine::reflect(Point{0, 0}, Vec2{1, 1}),
        Affine({0, 1, 1, 0, 0, 0})
    );

    // Test reflection with no translation
    Point point{0, 0};
    Vec2 vec{1, 1};
    Affine map = Affine::reflect(point, vec);
    assert_near(map * Point{0, 0}, Point{0, 0});
    assert_near(map * Point{1, 1}, Point{1, 1});
    assert_near(map * Point{1, 2}, Point{2, 1});

    // Test reflection with translation
    point = Point{1, 0};
    map = Affine::reflect(point, vec);
    assert_near(map * Point{1, 0}, Point{1, 0});
    assert_near(map * Point{2, 1}, Point{2, 1});
    assert_near(map * Point{2, 2}, Point{3, 1});
}

TEST(AffineTest, Chaining) {
    Affine a = Affine::identity();
    
    // Test pre_scale
    Affine scaled = a.pre_scale(2.0);
    Point p{1.0, 1.0};
    assert_near(scaled * p, Point{2.0, 2.0});
    
    // Test then_translate
    Affine translated = a.then_translate(Vec2{1.0, 2.0});
    assert_near(translated * p, Point{2.0, 3.0});
    
    // Test combination
    Affine combined = a.pre_scale(2.0).then_translate(Vec2{1.0, 2.0});
    assert_near(combined * p, Point{3.0, 4.0});
}

TEST(AffineTest, UtilityMethods) {
    Affine a = Affine({1.0, 2.0, 3.0, 4.0, 5.0, 6.0});
    
    // Test determinant
    EXPECT_DOUBLE_EQ(a.determinant(), -2.0);
    
    // Test is_finite
    EXPECT_TRUE(a.is_finite());
    
    // Test translation
    Vec2 trans = a.translation();
    EXPECT_DOUBLE_EQ(trans.x, 5.0);
    EXPECT_DOUBLE_EQ(trans.y, 6.0);
    
    // Test with_translation
    Affine new_a = a.with_translation(Vec2{10.0, 20.0});
    Vec2 new_trans = new_a.translation();
    EXPECT_DOUBLE_EQ(new_trans.x, 10.0);
    EXPECT_DOUBLE_EQ(new_trans.y, 20.0);
}

TEST(AffineTest, Operators) {
    Affine a1 = Affine({1.0, 2.0, 3.0, 4.0, 5.0, 6.0});
    Affine a2 = Affine({0.1, 1.2, 2.3, 3.4, 4.5, 5.6});
    
    // Test multiplication
    Affine result = a1 * a2;
    EXPECT_TRUE(result.is_finite());
    
    // Test compound assignment
    Affine a3 = a1;
    a3 *= a2;
    affine_assert_near(result, a3);
    
    // Test vector transformation
    Vec2 v{1.0, 2.0};
    Vec2 transformed = a1 * v;
    EXPECT_TRUE(transformed.is_finite());
    
    // Test size transformation
    Size s{10.0, 20.0};
    Size transformed_size = a1 * s;
    EXPECT_TRUE(transformed_size.is_finite());
} 