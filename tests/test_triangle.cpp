#include <gtest/gtest.h>
#include "kurbo/triangle.hpp"
#include "kurbo/point.hpp"
#include "kurbo/vec2.hpp"
#include "kurbo/circle.hpp"
#include "kurbo/inline_methods.hpp"
#include <cmath>

using namespace kurbo;

static void assert_approx_eq(double x, double y, double tolerance = 1e-9) {
    EXPECT_NEAR(x, y, tolerance);
}

static void assert_approx_eq_point(const Point& x, const Point& y, double tolerance = 1e-9) {
    EXPECT_NEAR(x.x, y.x, tolerance);
    EXPECT_NEAR(x.y, y.y, tolerance);
}

TEST(TriangleTest, Basic) {
    Point a{0.0, 0.0};
    Point b{3.0, 0.0};
    Point c{1.5, 2.0};
    Triangle triangle = Triangle::new_triangle(a, b, c);
    
    assert_approx_eq_point(triangle.a, a);
    assert_approx_eq_point(triangle.b, b);
    assert_approx_eq_point(triangle.c, c);
}

TEST(TriangleTest, FromCoords) {
    Triangle triangle = Triangle::from_coords(0.0, 0.0, 3.0, 0.0, 1.5, 2.0);
    
    assert_approx_eq_point(triangle.a, Point(0.0, 0.0));
    assert_approx_eq_point(triangle.b, Point(3.0, 0.0));
    assert_approx_eq_point(triangle.c, Point(1.5, 2.0));
}

TEST(TriangleTest, Centroid) {
    Triangle triangle = Triangle::new_triangle(Point{0.0, 0.0}, Point{3.0, 0.0}, Point{1.5, 2.0});
    Point centroid = triangle.centroid();
    
    assert_approx_eq(centroid.x, 1.5);
    assert_approx_eq(centroid.y, 2.0 / 3.0);
}

TEST(TriangleTest, Offsets) {
    Triangle triangle = Triangle::new_triangle(Point{0.0, 0.0}, Point{3.0, 0.0}, Point{1.5, 2.0});
    auto offsets = triangle.offsets();
    
    Point centroid = triangle.centroid();
    assert_approx_eq_point(centroid + offsets[0], triangle.a);
    assert_approx_eq_point(centroid + offsets[1], triangle.b);
    assert_approx_eq_point(centroid + offsets[2], triangle.c);
}

TEST(TriangleTest, Area) {
    Triangle triangle = Triangle::new_triangle(Point{0.0, 0.0}, Point{3.0, 0.0}, Point{1.5, 2.0});
    double expected_area = 0.5 * 3.0 * 2.0; // base * height / 2
    assert_approx_eq(triangle.area(), expected_area);
}

TEST(TriangleTest, IsZeroArea) {
    Triangle zero_triangle = Triangle::new_triangle(Point{0.0, 0.0}, Point{1.0, 0.0}, Point{2.0, 0.0});
    EXPECT_TRUE(zero_triangle.is_zero_area());
    
    Triangle normal_triangle = Triangle::new_triangle(Point{0.0, 0.0}, Point{3.0, 0.0}, Point{1.5, 2.0});
    EXPECT_FALSE(normal_triangle.is_zero_area());
}

TEST(TriangleTest, InscribedCircle) {
    Triangle triangle = Triangle::new_triangle(Point{0.0, 0.0}, Point{3.0, 0.0}, Point{1.5, 2.0});
    Circle inscribed = triangle.inscribed_circle();
    
    Point center = inscribed.center;
    double radius = inscribed.radius;
    
    EXPECT_GT(radius, 0.0);
    EXPECT_TRUE(std::isfinite(radius));
    
    // The inscribed circle should be inside the triangle
    EXPECT_EQ(triangle.winding(center), 1);
}

TEST(TriangleTest, CircumscribedCircle) {
    Triangle triangle = Triangle::new_triangle(Point{0.0, 0.0}, Point{3.0, 0.0}, Point{1.5, 2.0});
    Circle circumscribed = triangle.circumscribed_circle();
    
    Point center = circumscribed.center;
    double radius = circumscribed.radius;
    
    EXPECT_GT(radius, 0.0);
    EXPECT_TRUE(std::isfinite(radius));
    
    // All vertices should be on the circumscribed circle
    double dist_a = center.distance(triangle.a);
    double dist_b = center.distance(triangle.b);
    double dist_c = center.distance(triangle.c);
    
    assert_approx_eq(dist_a, radius);
    assert_approx_eq(dist_b, radius);
    assert_approx_eq(dist_c, radius);
}

TEST(TriangleTest, Inflate) {
    Triangle triangle = Triangle::new_triangle(Point{0.0, 0.0}, Point{3.0, 0.0}, Point{1.5, 2.0});
    Triangle inflated = triangle.inflate(1.0);
    
    // The inflated triangle should have positive area
    EXPECT_GT(inflated.area(), 0.0);
    EXPECT_TRUE(std::isfinite(inflated.area()));
}

TEST(TriangleTest, Perimeter) {
    Triangle triangle = Triangle::new_triangle(Point{0.0, 0.0}, Point{3.0, 0.0}, Point{1.5, 2.0});
    double perimeter = triangle.perimeter(1e-9);
    
    double expected_perimeter = 3.0 + std::sqrt(2.25 + 4.0) + std::sqrt(2.25 + 4.0);
    assert_approx_eq(perimeter, expected_perimeter);
}

TEST(TriangleTest, Winding) {
    Triangle triangle = Triangle::new_triangle(Point{0.0, 0.0}, Point{3.0, 0.0}, Point{1.5, 2.0});
    
    // Point inside triangle
    Point inside{1.5, 0.5};
    EXPECT_EQ(triangle.winding(inside), 1);
    
    // Point outside triangle
    Point outside{5.0, 5.0};
    EXPECT_EQ(triangle.winding(outside), 0);
    
    // Point on triangle boundary
    Point on_boundary{1.5, 0.0};
    EXPECT_EQ(triangle.winding(on_boundary), 0);
}

TEST(TriangleTest, BoundingBox) {
    Triangle triangle = Triangle::new_triangle(Point{1.0, 2.0}, Point{4.0, 2.0}, Point{2.5, 4.0});
    Rect bbox = triangle.bounding_box();
    
    assert_approx_eq(bbox.x0, 1.0);
    assert_approx_eq(bbox.y0, 2.0);
    assert_approx_eq(bbox.x1, 4.0);
    assert_approx_eq(bbox.y1, 4.0);
}

TEST(TriangleTest, Operators) {
    Triangle triangle = Triangle::new_triangle(Point{1.0, 2.0}, Point{4.0, 2.0}, Point{2.5, 4.0});
    Vec2 v{5.0, 5.0};
    
    Triangle translated = triangle + v;
    assert_approx_eq_point(translated.a, Point{6.0, 7.0});
    assert_approx_eq_point(translated.b, Point{9.0, 7.0});
    assert_approx_eq_point(translated.c, Point{7.5, 9.0});
    
    Triangle back = translated - v;
    assert_approx_eq_point(back.a, triangle.a);
    assert_approx_eq_point(back.b, triangle.b);
    assert_approx_eq_point(back.c, triangle.c);
}

TEST(TriangleTest, UtilityMethods) {
    Triangle triangle = Triangle::new_triangle(Point{1.0, 2.0}, Point{4.0, 2.0}, Point{2.5, 4.0});
    
    EXPECT_TRUE(triangle.is_finite());
    EXPECT_FALSE(triangle.is_nan());
    
    auto as_triangle = triangle.as_triangle();
    EXPECT_TRUE(as_triangle.has_value());
    assert_approx_eq_point(as_triangle.value().a, triangle.a);
    assert_approx_eq_point(as_triangle.value().b, triangle.b);
    assert_approx_eq_point(as_triangle.value().c, triangle.c);
}

TEST(TriangleTest, Zero) {
    Triangle zero = Triangle::zero();
    assert_approx_eq_point(zero.a, Point{0.0, 0.0});
    assert_approx_eq_point(zero.b, Point{0.0, 0.0});
    assert_approx_eq_point(zero.c, Point{0.0, 0.0});
    EXPECT_TRUE(zero.is_zero_area());
}

TEST(TriangleTest, Equilateral) {
    Triangle equilateral = Triangle::EQUILATERAL;
    
    // Check that all sides are equal
    double side1 = equilateral.a.distance(equilateral.b);
    double side2 = equilateral.b.distance(equilateral.c);
    double side3 = equilateral.c.distance(equilateral.a);
    
    assert_approx_eq(side1, side2);
    assert_approx_eq(side2, side3);
    assert_approx_eq(side3, 1.0);
    
    // Check that it's not zero area
    EXPECT_FALSE(equilateral.is_zero_area());
} 