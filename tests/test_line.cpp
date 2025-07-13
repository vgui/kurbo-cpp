#include <gtest/gtest.h>
#include "kurbo/line.hpp"
#include "kurbo/point.hpp"
#include "kurbo/vec2.hpp"
#include "kurbo/affine.hpp"

using namespace kurbo;

static void assert_approx_eq(double x, double y, double tolerance = 1e-9) {
    EXPECT_NEAR(x, y, tolerance);
}

TEST(LineTest, Basic) {
    Point p0{0.0, 0.0};
    Point p1{3.0, 4.0};
    Line line = Line::new_line(p0, p1);
    
    EXPECT_DOUBLE_EQ(line.p0.x, 0.0);
    EXPECT_DOUBLE_EQ(line.p0.y, 0.0);
    EXPECT_DOUBLE_EQ(line.p1.x, 3.0);
    EXPECT_DOUBLE_EQ(line.p1.y, 4.0);
}

TEST(LineTest, Reversed) {
    Point p0{0.0, 0.0};
    Point p1{3.0, 4.0};
    Line line = Line::new_line(p0, p1);
    Line reversed = line.reversed();
    
    EXPECT_DOUBLE_EQ(reversed.p0.x, 3.0);
    EXPECT_DOUBLE_EQ(reversed.p0.y, 4.0);
    EXPECT_DOUBLE_EQ(reversed.p1.x, 0.0);
    EXPECT_DOUBLE_EQ(reversed.p1.y, 0.0);
}

TEST(LineTest, Length) {
    Point p0{0.0, 0.0};
    Point p1{3.0, 4.0};
    Line line = Line::new_line(p0, p1);
    
    EXPECT_DOUBLE_EQ(line.length(), 5.0);
}

TEST(LineTest, Midpoint) {
    Point p0{0.0, 0.0};
    Point p1{6.0, 8.0};
    Line line = Line::new_line(p0, p1);
    Point mid = line.midpoint();
    
    EXPECT_DOUBLE_EQ(mid.x, 3.0);
    EXPECT_DOUBLE_EQ(mid.y, 4.0);
}

TEST(LineTest, CrossingPoint) {
    Line line1 = Line::new_line(Point{0.0, 0.0}, Point{2.0, 2.0});
    Line line2 = Line::new_line(Point{0.0, 2.0}, Point{2.0, 0.0});
    
    auto crossing = line1.crossing_point(line2);
    EXPECT_TRUE(crossing.has_value());
    EXPECT_DOUBLE_EQ(crossing.value().x, 1.0);
    EXPECT_DOUBLE_EQ(crossing.value().y, 1.0);
}

TEST(LineTest, CrossingPointParallel) {
    Line line1 = Line::new_line(Point{0.0, 0.0}, Point{1.0, 1.0});
    Line line2 = Line::new_line(Point{0.0, 1.0}, Point{1.0, 2.0});
    
    auto crossing = line1.crossing_point(line2);
    EXPECT_FALSE(crossing.has_value());
}

TEST(LineTest, Eval) {
    Point p0{0.0, 0.0};
    Point p1{4.0, 4.0};
    Line line = Line::new_line(p0, p1);
    
    Point mid = line.eval(0.5);
    EXPECT_DOUBLE_EQ(mid.x, 2.0);
    EXPECT_DOUBLE_EQ(mid.y, 2.0);
    
    Point start = line.eval(0.0);
    EXPECT_DOUBLE_EQ(start.x, 0.0);
    EXPECT_DOUBLE_EQ(start.y, 0.0);
    
    Point end = line.eval(1.0);
    EXPECT_DOUBLE_EQ(end.x, 4.0);
    EXPECT_DOUBLE_EQ(end.y, 4.0);
}

TEST(LineTest, Subsegment) {
    Point p0{0.0, 0.0};
    Point p1{4.0, 4.0};
    Line line = Line::new_line(p0, p1);
    
    auto sub_ptr = line.subsegment(0.25, 0.75);
    auto sub = dynamic_cast<Line*>(sub_ptr);
    EXPECT_NE(sub, nullptr);
    
    EXPECT_DOUBLE_EQ(sub->p0.x, 1.0);
    EXPECT_DOUBLE_EQ(sub->p0.y, 1.0);
    EXPECT_DOUBLE_EQ(sub->p1.x, 3.0);
    EXPECT_DOUBLE_EQ(sub->p1.y, 3.0);
    
    delete sub_ptr;
}

TEST(LineTest, Subdivide) {
    Point p0{0.0, 0.0};
    Point p1{4.0, 4.0};
    Line line = Line::new_line(p0, p1);
    
    auto [first_ptr, second_ptr] = line.subdivide();
    auto first = dynamic_cast<Line*>(first_ptr);
    auto second = dynamic_cast<Line*>(second_ptr);
    
    EXPECT_NE(first, nullptr);
    EXPECT_NE(second, nullptr);
    
    EXPECT_DOUBLE_EQ(first->p0.x, 0.0);
    EXPECT_DOUBLE_EQ(first->p0.y, 0.0);
    EXPECT_DOUBLE_EQ(first->p1.x, 2.0);
    EXPECT_DOUBLE_EQ(first->p1.y, 2.0);
    
    EXPECT_DOUBLE_EQ(second->p0.x, 2.0);
    EXPECT_DOUBLE_EQ(second->p0.y, 2.0);
    EXPECT_DOUBLE_EQ(second->p1.x, 4.0);
    EXPECT_DOUBLE_EQ(second->p1.y, 4.0);
    
    delete first_ptr;
    delete second_ptr;
}

TEST(LineTest, Derivative) {
    Point p0{0.0, 0.0};
    Point p1{3.0, 4.0};
    Line line = Line::new_line(p0, p1);
    
    auto deriv_ptr = line.deriv();
    auto deriv = dynamic_cast<Line*>(deriv_ptr);
    EXPECT_NE(deriv, nullptr);
    
    EXPECT_DOUBLE_EQ(deriv->p0.x, 0.0);
    EXPECT_DOUBLE_EQ(deriv->p0.y, 0.0);
    EXPECT_DOUBLE_EQ(deriv->p1.x, 3.0);
    EXPECT_DOUBLE_EQ(deriv->p1.y, 4.0);
    
    delete deriv_ptr;
}

TEST(LineTest, Arclen) {
    Point p0{0.0, 0.0};
    Point p1{3.0, 4.0};
    Line line = Line::new_line(p0, p1);
    
    EXPECT_DOUBLE_EQ(line.arclen(1e-9), 5.0);
}

TEST(LineTest, SignedArea) {
    Point p0{0.0, 0.0};
    Point p1{3.0, 4.0};
    Line line = Line::new_line(p0, p1);
    
    EXPECT_DOUBLE_EQ(line.signed_area(), 0.0);
}

TEST(LineTest, Nearest) {
    Point p0{0.0, 0.0};
    Point p1{4.0, 0.0};
    Line line = Line::new_line(p0, p1);
    
    Point test_point(2.0, 3.0);
    Nearest result = line.nearest(test_point, 0.001);
    
    EXPECT_DOUBLE_EQ(result.t, 0.5);
    EXPECT_DOUBLE_EQ(result.distance_sq, 9.0);
}

TEST(LineTest, Extrema) {
    Point p0{0.0, 0.0};
    Point p1{3.0, 4.0};
    Line line = Line::new_line(p0, p1);
    
    auto extrema = line.extrema();
    EXPECT_EQ(extrema.size(), 0); // Lines have no extrema
}

TEST(LineTest, Operators) {
    Point p0{1.0, 2.0};
    Point p1{4.0, 6.0};
    Line line = Line::new_line(p0, p1);
    Vec2 v{5.0, 5.0};
    
    Line translated = line + v;
    EXPECT_DOUBLE_EQ(translated.p0.x, 6.0);
    EXPECT_DOUBLE_EQ(translated.p0.y, 7.0);
    EXPECT_DOUBLE_EQ(translated.p1.x, 9.0);
    EXPECT_DOUBLE_EQ(translated.p1.y, 11.0);
    
    Line back = translated - v;
    EXPECT_DOUBLE_EQ(back.p0.x, line.p0.x);
    EXPECT_DOUBLE_EQ(back.p0.y, line.p0.y);
    EXPECT_DOUBLE_EQ(back.p1.x, line.p1.x);
    EXPECT_DOUBLE_EQ(back.p1.y, line.p1.y);
}

TEST(LineTest, AffineTransform) {
    Point p0{1.0, 2.0};
    Point p1{4.0, 6.0};
    Line line = Line::new_line(p0, p1);
    Affine scale = Affine::scale(2.0);
    
    Line scaled = line * scale;
    EXPECT_DOUBLE_EQ(scaled.p0.x, 2.0);
    EXPECT_DOUBLE_EQ(scaled.p0.y, 4.0);
    EXPECT_DOUBLE_EQ(scaled.p1.x, 8.0);
    EXPECT_DOUBLE_EQ(scaled.p1.y, 12.0);
}

TEST(LineTest, UtilityMethods) {
    Point p0{1.0, 2.0};
    Point p1{4.0, 6.0};
    Line line = Line::new_line(p0, p1);
    
    EXPECT_TRUE(line.is_finite());
    EXPECT_FALSE(line.is_nan());
    
    Point start = line.start();
    EXPECT_DOUBLE_EQ(start.x, 1.0);
    EXPECT_DOUBLE_EQ(start.y, 2.0);
    
    Point end = line.end();
    EXPECT_DOUBLE_EQ(end.x, 4.0);
    EXPECT_DOUBLE_EQ(end.y, 6.0);
}

TEST(LineTest, Zero) {
    Line zero = Line::zero();
    EXPECT_DOUBLE_EQ(zero.p0.x, 0.0);
    EXPECT_DOUBLE_EQ(zero.p0.y, 0.0);
    EXPECT_DOUBLE_EQ(zero.p1.x, 0.0);
    EXPECT_DOUBLE_EQ(zero.p1.y, 0.0);
}

// ConstPoint tests
TEST(ConstPointTest, Basic) {
    Point p{3.0, 4.0};
    ConstPoint cp(p);
    
    EXPECT_DOUBLE_EQ(cp.point.x, 3.0);
    EXPECT_DOUBLE_EQ(cp.point.y, 4.0);
}

TEST(ConstPointTest, Eval) {
    Point p{3.0, 4.0};
    ConstPoint cp(p);
    
    Point result = cp.eval(0.5);
    EXPECT_DOUBLE_EQ(result.x, 3.0);
    EXPECT_DOUBLE_EQ(result.y, 4.0);
}

TEST(ConstPointTest, UtilityMethods) {
    Point p{3.0, 4.0};
    ConstPoint cp(p);
    
    EXPECT_TRUE(cp.is_finite());
    EXPECT_FALSE(cp.is_nan());
    
    EXPECT_DOUBLE_EQ(cp.arclen(1e-9), 0.0);
    EXPECT_DOUBLE_EQ(cp.signed_area(), 0.0);
    EXPECT_DOUBLE_EQ(cp.curvature(0.5), 0.0);
}

TEST(ConstPointTest, Zero) {
    ConstPoint zero = ConstPoint::zero();
    EXPECT_DOUBLE_EQ(zero.point.x, 0.0);
    EXPECT_DOUBLE_EQ(zero.point.y, 0.0);
} 