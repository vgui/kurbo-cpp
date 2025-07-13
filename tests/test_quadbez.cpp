#include <gtest/gtest.h>
#include "kurbo/quadbez.hpp"
#include "kurbo/point.hpp"
#include "kurbo/vec2.hpp"
#include "kurbo/line.hpp"
#include "kurbo/affine.hpp"

using namespace kurbo;

TEST(QuadBezTest, Basic) {
    Point p0(0.0, 0.0);
    Point p1(1.0, 1.0);
    Point p2(2.0, 0.0);
    QuadBez quad(p0, p1, p2);
    
    EXPECT_EQ(quad.p0, p0);
    EXPECT_EQ(quad.p1, p1);
    EXPECT_EQ(quad.p2, p2);
}

TEST(QuadBezTest, Eval) {
    Point p0(0.0, 0.0);
    Point p1(1.0, 1.0);
    Point p2(2.0, 0.0);
    QuadBez quad(p0, p1, p2);
    
    Point result = quad.eval(0.5);
    EXPECT_DOUBLE_EQ(result.x, 1.0);
    EXPECT_DOUBLE_EQ(result.y, 0.5);
    
    EXPECT_EQ(quad.start(), p0);
    EXPECT_EQ(quad.end(), p2);
}

TEST(QuadBezTest, Subdivide) {
    Point p0(0.0, 0.0);
    Point p1(1.0, 1.0);
    Point p2(2.0, 0.0);
    QuadBez quad(p0, p1, p2);
    
    auto [first, second] = quad.subdivide();
    
    // Cast to QuadBez to access members
    auto first_quad = dynamic_cast<QuadBez*>(first);
    auto second_quad = dynamic_cast<QuadBez*>(second);
    
    EXPECT_NE(first_quad, nullptr);
    EXPECT_NE(second_quad, nullptr);
    
    EXPECT_EQ(first_quad->p0, p0);
    EXPECT_EQ(first_quad->p1, Point(0.5, 0.5));
    EXPECT_EQ(first_quad->p2, Point(1.0, 0.5));
    
    EXPECT_EQ(second_quad->p0, Point(1.0, 0.5));
    EXPECT_EQ(second_quad->p1, Point(1.5, 0.5));
    EXPECT_EQ(second_quad->p2, p2);
    
    delete first;
    delete second;
}

TEST(QuadBezTest, Subsegment) {
    Point p0(0.0, 0.0);
    Point p1(1.0, 1.0);
    Point p2(2.0, 0.0);
    QuadBez quad(p0, p1, p2);
    
    auto segment = quad.subsegment(0.25, 0.75);
    
    // Cast to QuadBez to access members
    auto segment_quad = dynamic_cast<QuadBez*>(segment);
    EXPECT_NE(segment_quad, nullptr);
    
    EXPECT_EQ(segment_quad->p0, quad.eval(0.25));
    EXPECT_EQ(segment_quad->p2, quad.eval(0.75));
    
    delete segment;
}

TEST(QuadBezTest, Derivative) {
    Point p0(0.0, 0.0);
    Point p1(1.0, 1.0);
    Point p2(2.0, 0.0);
    QuadBez quad(p0, p1, p2);
    
    auto deriv = quad.deriv();
    
    // Cast to Line to access members
    auto deriv_line = dynamic_cast<Line*>(deriv);
    EXPECT_NE(deriv_line, nullptr);
    
    EXPECT_EQ(deriv_line->p0, Point(2.0, 2.0));
    EXPECT_EQ(deriv_line->p1, Point(2.0, -2.0));
    
    delete deriv;
}

TEST(QuadBezTest, ArcLength) {
    Point p0(0.0, 0.0);
    Point p1(1.0, 1.0);
    Point p2(2.0, 0.0);
    QuadBez quad(p0, p1, p2);
    
    double length = quad.arclen(0.001);
    EXPECT_GT(length, 2.0); // Should be longer than straight line
    EXPECT_LT(length, 3.0); // Should be reasonable
}

TEST(QuadBezTest, SignedArea) {
    Point p0(0.0, 0.0);
    Point p1(1.0, 1.0);
    Point p2(2.0, 0.0);
    QuadBez quad(p0, p1, p2);
    
    double area = quad.signed_area();
    EXPECT_NEAR(area, -2.0 / 3.0, 1e-15); // As in reference Rust kurbo
}

TEST(QuadBezTest, Nearest) {
    Point p0(0.0, 0.0);
    Point p1(1.0, 1.0);
    Point p2(2.0, 0.0);
    QuadBez quad(p0, p1, p2);
    
    Point test_point(1.0, 0.5);
    Nearest result = quad.nearest(test_point, 0.001);
    
    EXPECT_GE(result.t, 0.0);
    EXPECT_LE(result.t, 1.0);
    EXPECT_GT(result.distance_sq, 0.0);
}

TEST(QuadBezTest, Extrema) {
    Point p0(0.0, 0.0);
    Point p1(1.0, 1.0);
    Point p2(2.0, 0.0);
    QuadBez quad(p0, p1, p2);
    
    auto extrema = quad.extrema();
    EXPECT_EQ(extrema.size(), 1); // Should have one extremum at t=0.5
    EXPECT_DOUBLE_EQ(extrema[0], 0.5);
}

TEST(QuadBezTest, IsFinite) {
    Point p0(0.0, 0.0);
    Point p1(1.0, 1.0);
    Point p2(2.0, 0.0);
    QuadBez quad(p0, p1, p2);
    
    EXPECT_TRUE(quad.is_finite());
    
    Point nan_point(std::numeric_limits<double>::quiet_NaN(), 0.0);
    QuadBez nan_quad(nan_point, p1, p2);
    EXPECT_FALSE(nan_quad.is_finite());
}

TEST(QuadBezTest, IsNan) {
    Point p0(0.0, 0.0);
    Point p1(1.0, 1.0);
    Point p2(2.0, 0.0);
    QuadBez quad(p0, p1, p2);
    
    EXPECT_FALSE(quad.is_nan());
    
    Point nan_point(std::numeric_limits<double>::quiet_NaN(), 0.0);
    QuadBez nan_quad(nan_point, p1, p2);
    EXPECT_TRUE(nan_quad.is_nan());
}

TEST(QuadBezTest, AffineTransform) {
    Point p0(0.0, 0.0);
    Point p1(1.0, 1.0);
    Point p2(2.0, 0.0);
    QuadBez quad(p0, p1, p2);
    
    Affine scale = Affine::scale(2.0);
    QuadBez transformed = scale * quad;
    
    EXPECT_EQ(transformed.p0, scale * p0);
    EXPECT_EQ(transformed.p1, scale * p1);
    EXPECT_EQ(transformed.p2, scale * p2);
}

TEST(QuadBezTest, Iterator) {
    Point p0(0.0, 0.0);
    Point p1(1.0, 1.0);
    Point p2(2.0, 0.0);
    QuadBez quad(p0, p1, p2);
    
    QuadBezIter iter(quad);
    
    auto first = iter.next();
    EXPECT_NE(first, nullptr);
    EXPECT_EQ(first->type, PathElType::MoveTo);
    EXPECT_EQ(first->point, p0);
    delete first;
    
    auto second = iter.next();
    EXPECT_NE(second, nullptr);
    EXPECT_EQ(second->type, PathElType::QuadTo);
    EXPECT_EQ(second->point, p1);
    EXPECT_EQ(second->point2, p2);
    delete second;
    
    auto third = iter.next();
    EXPECT_EQ(third, nullptr);
}

TEST(QuadBezTest, ApproxParabolaIntegral) {
    double result = approx_parabola_integral(1.0);
    EXPECT_GT(result, 0.0);
    EXPECT_LT(result, 2.0);
    
    double inv_result = approx_parabola_inv_integral(1.0);
    EXPECT_GT(inv_result, 0.0);
    EXPECT_LT(inv_result, 2.0);
} 