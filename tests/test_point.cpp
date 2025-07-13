#include <gtest/gtest.h>
#include "kurbo/point.hpp"
#include "kurbo/vec2.hpp"
#include "kurbo/inline_methods.hpp"

using namespace kurbo;

TEST(PointTest, Construction) {
    Point p1(1.0, 2.0);
    EXPECT_EQ(p1.x, 1.0);
    EXPECT_EQ(p1.y, 2.0);
    
    Point p2;
    EXPECT_EQ(p2.x, 0.0);
    EXPECT_EQ(p2.y, 0.0);
    
    EXPECT_EQ(Point::ZERO.x, 0.0);
    EXPECT_EQ(Point::ZERO.y, 0.0);
    EXPECT_EQ(Point::ORIGIN.x, 0.0);
    EXPECT_EQ(Point::ORIGIN.y, 0.0);
}

TEST(PointTest, Conversion) {
    Point p(3.0, 4.0);
    Vec2 v = p.to_vec2();
    EXPECT_EQ(v.x, 3.0);
    EXPECT_EQ(v.y, 4.0);
}

TEST(PointTest, Arithmetic) {
    Point p1(1.0, 2.0);
    Point p2(3.0, 4.0);
    Vec2 v(2.0, 1.0);
    
    // Point + Vec2
    Point p3 = p1 + v;
    EXPECT_EQ(p3.x, 3.0);
    EXPECT_EQ(p3.y, 3.0);
    
    // Point - Vec2
    Point p4 = p1 - v;
    EXPECT_EQ(p4.x, -1.0);
    EXPECT_EQ(p4.y, 1.0);
    
    // Point - Point
    Vec2 v2 = p2 - p1;
    EXPECT_EQ(v2.x, 2.0);
    EXPECT_EQ(v2.y, 2.0);
    
    // Point += Vec2
    Point p5 = p1;
    p5 += v;
    EXPECT_EQ(p5.x, 3.0);
    EXPECT_EQ(p5.y, 3.0);
    
    // Point -= Vec2
    Point p6 = p1;
    p6 -= v;
    EXPECT_EQ(p6.x, -1.0);
    EXPECT_EQ(p6.y, 1.0);
}

TEST(PointTest, TupleOperations) {
    Point p(1.0, 2.0);
    std::tuple<double, double> t(3.0, 4.0);
    
    Point p2 = p + t;
    EXPECT_EQ(p2.x, 4.0);
    EXPECT_EQ(p2.y, 6.0);
    
    Point p3 = p - t;
    EXPECT_EQ(p3.x, -2.0);
    EXPECT_EQ(p3.y, -2.0);
    
    Point p4 = p;
    p4 += t;
    EXPECT_EQ(p4.x, 4.0);
    EXPECT_EQ(p4.y, 6.0);
    
    Point p5 = p;
    p5 -= t;
    EXPECT_EQ(p5.x, -2.0);
    EXPECT_EQ(p5.y, -2.0);
}

TEST(PointTest, Distance) {
    Point p1(0.0, 0.0);
    Point p2(3.0, 4.0);
    
    EXPECT_EQ(p1.distance(p2), 5.0);
    EXPECT_EQ(p1.distance_squared(p2), 25.0);
    EXPECT_EQ(p2.distance(p1), 5.0);
    EXPECT_EQ(p2.distance_squared(p1), 25.0);
}

TEST(PointTest, Midpoint) {
    Point p1(0.0, 0.0);
    Point p2(4.0, 6.0);
    
    Point mid = p1.midpoint(p2);
    EXPECT_EQ(mid.x, 2.0);
    EXPECT_EQ(mid.y, 3.0);
    
    mid = p2.midpoint(p1);
    EXPECT_EQ(mid.x, 2.0);
    EXPECT_EQ(mid.y, 3.0);
}

TEST(PointTest, Lerp) {
    Point p1(0.0, 0.0);
    Point p2(4.0, 6.0);
    
    Point p3 = p1.lerp(p2, 0.5);
    EXPECT_EQ(p3.x, 2.0);
    EXPECT_EQ(p3.y, 3.0);
    
    Point p4 = p1.lerp(p2, 0.0);
    EXPECT_EQ(p4.x, 0.0);
    EXPECT_EQ(p4.y, 0.0);
    
    Point p5 = p1.lerp(p2, 1.0);
    EXPECT_EQ(p5.x, 4.0);
    EXPECT_EQ(p5.y, 6.0);
}

TEST(PointTest, Rounding) {
    Point p(3.3, 3.6);
    
    Point rounded = p.round();
    EXPECT_EQ(rounded.x, 3.0);
    EXPECT_EQ(rounded.y, 4.0);
    
    Point ceiled = p.ceil();
    EXPECT_EQ(ceiled.x, 4.0);
    EXPECT_EQ(ceiled.y, 4.0);
    
    Point floored = p.floor();
    EXPECT_EQ(floored.x, 3.0);
    EXPECT_EQ(floored.y, 3.0);
    
    Point expanded = p.expand();
    EXPECT_EQ(expanded.x, 4.0);
    EXPECT_EQ(expanded.y, 4.0);
    
    Point truncated = p.trunc();
    EXPECT_EQ(truncated.x, 3.0);
    EXPECT_EQ(truncated.y, 3.0);
}

TEST(PointTest, Validation) {
    Point p(1.0, 2.0);
    EXPECT_TRUE(p.is_finite());
    EXPECT_FALSE(p.is_nan());
    
    Point inf_p(std::numeric_limits<double>::infinity(), 1.0);
    EXPECT_FALSE(inf_p.is_finite());
    
    Point nan_p(std::numeric_limits<double>::quiet_NaN(), 1.0);
    EXPECT_TRUE(nan_p.is_nan());
}

TEST(PointTest, Comparison) {
    Point p1(1.0, 2.0);
    Point p2(1.0, 2.0);
    Point p3(2.0, 1.0);
    
    EXPECT_EQ(p1, p2);
    EXPECT_NE(p1, p3);
    EXPECT_NE(p2, p3);
}

TEST(PointTest, ConversionFunctions) {
    std::tuple<double, double> t(3.0, 4.0);
    Point p = Point::from_tuple(t);
    EXPECT_EQ(p.x, 3.0);
    EXPECT_EQ(p.y, 4.0);
    
    std::tuple<double, double> t2 = p.to_tuple();
    EXPECT_EQ(std::get<0>(t2), 3.0);
    EXPECT_EQ(std::get<1>(t2), 4.0);
}

TEST(PointTest, StreamOutput) {
    Point p(1.5, 2.5);
    std::ostringstream oss;
    oss << p;
    EXPECT_EQ(oss.str(), "Point(1.5, 2.5)");
} 