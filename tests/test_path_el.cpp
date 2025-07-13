#include <gtest/gtest.h>
#include "kurbo/path_el.hpp"
#include "kurbo/point.hpp"
#include "kurbo/line.hpp"
#include "kurbo/quadbez.hpp"

using namespace kurbo;

TEST(PathElTest, MoveTo) {
    Point p(1.0, 2.0);
    PathEl el(PathElType::MoveTo, p);
    
    EXPECT_EQ(el.type, PathElType::MoveTo);
    EXPECT_EQ(el.point, p);
}

TEST(PathElTest, LineTo) {
    Point p(3.0, 4.0);
    PathEl el(PathElType::LineTo, p);
    
    EXPECT_EQ(el.type, PathElType::LineTo);
    EXPECT_EQ(el.point, p);
}

TEST(PathElTest, QuadTo) {
    Point p1(1.0, 2.0);
    Point p2(3.0, 4.0);
    PathEl el(PathElType::QuadTo, p1, p2);
    
    EXPECT_EQ(el.type, PathElType::QuadTo);
    EXPECT_EQ(el.point, p1);
    EXPECT_EQ(el.point2, p2);
}

TEST(PathElTest, CurveTo) {
    Point p1(1.0, 2.0);
    Point p2(3.0, 4.0);
    Point p3(5.0, 6.0);
    PathEl el(PathElType::CurveTo, p1, p2, p3);
    
    EXPECT_EQ(el.type, PathElType::CurveTo);
    EXPECT_EQ(el.point, p1);
    EXPECT_EQ(el.point2, p2);
    EXPECT_EQ(el.point3, p3);
}

TEST(PathElTest, ClosePath) {
    PathEl el(PathElType::ClosePath);
    
    EXPECT_EQ(el.type, PathElType::ClosePath);
}

TEST(PathElTest, DefaultConstructor) {
    PathEl el(PathElType::MoveTo);
    
    EXPECT_EQ(el.type, PathElType::MoveTo);
    EXPECT_EQ(el.point, Point(0.0, 0.0));
    EXPECT_EQ(el.point2, Point(0.0, 0.0));
    EXPECT_EQ(el.point3, Point(0.0, 0.0));
}

TEST(PathElTest, CopyConstructor) {
    Point p(1.0, 2.0);
    PathEl original(PathElType::LineTo, p);
    PathEl copy(original);
    
    EXPECT_EQ(copy.type, original.type);
    EXPECT_EQ(copy.point, original.point);
    EXPECT_EQ(copy.point2, original.point2);
    EXPECT_EQ(copy.point3, original.point3);
}

TEST(PathElTest, AssignmentOperator) {
    Point p1(1.0, 2.0);
    Point p2(3.0, 4.0);
    PathEl original(PathElType::QuadTo, p1, p2);
    PathEl copy(PathElType::MoveTo);
    copy = original;
    
    EXPECT_EQ(copy.type, original.type);
    EXPECT_EQ(copy.point, original.point);
    EXPECT_EQ(copy.point2, original.point2);
    EXPECT_EQ(copy.point3, original.point3);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
} 