#include <gtest/gtest.h>
#include "kurbo/param_curve.hpp"
#include "kurbo/point.hpp"
#include "kurbo/vec2.hpp"
#include "kurbo/rect.hpp"

using namespace kurbo;

// Mock implementation for testing
class MockParamCurve : public ParamCurve {
public:
    MockParamCurve(Point start, Point end) : start_pt(start), end_pt(end) {}
    
    Point eval(double t) const override {
        return start_pt + (end_pt - start_pt) * t;
    }
    
    ParamCurve* subsegment(double start, double end) const override {
        Point new_start = eval(start);
        Point new_end = eval(end);
        return new MockParamCurve(new_start, new_end);
    }
    
    std::pair<ParamCurve*, ParamCurve*> subdivide() const override {
        Point mid = eval(0.5);
        auto first = new MockParamCurve(start_pt, mid);
        auto second = new MockParamCurve(mid, end_pt);
        return {first, second};
    }
    
    Point start() const override {
        return start_pt;
    }
    
    Point end() const override {
        return end_pt;
    }
    
private:
    Point start_pt;
    Point end_pt;
};

class MockParamCurveDeriv : public ParamCurveDeriv {
public:
    MockParamCurveDeriv(Point start, Point end) : start_pt(start), end_pt(end) {}
    
    ParamCurveDeriv* deriv() const override {
        Vec2 derivative = (end_pt - start_pt);
        return new MockParamCurveDeriv(Point(0, 0), Point(derivative.x, derivative.y));
    }
    
private:
    Point start_pt;
    Point end_pt;
};

class MockParamCurveArclen : public ParamCurveArclen {
public:
    MockParamCurveArclen(Point start, Point end) : start_pt(start), end_pt(end) {}
    
    double arclen(double accuracy) const override {
        return (end_pt - start_pt).hypot();
    }
    
private:
    Point start_pt;
    Point end_pt;
};

class MockParamCurveNearest : public ParamCurveNearest {
public:
    MockParamCurveNearest(Point start, Point end) : start_pt(start), end_pt(end) {}
    
    Nearest nearest(const Point& p, double accuracy) const override {
        Vec2 line_vec = (end_pt - start_pt);
        Vec2 point_vec = (p - start_pt);
        
        double t = point_vec.dot(line_vec) / line_vec.dot(line_vec);
        t = std::max(0.0, std::min(1.0, t));
        
        Point nearest_pt = start_pt + (end_pt - start_pt) * t;
        double distance_sq = p.distance_squared(nearest_pt);
        
        return {distance_sq, t};
    }
    
private:
    Point start_pt;
    Point end_pt;
};

class MockParamCurveExtrema : public ParamCurveExtrema {
public:
    MockParamCurveExtrema(Point start, Point end) : start_pt(start), end_pt(end) {}
    
    std::vector<double> extrema() const override {
        // Mock implementation - no extrema
        return {};
    }
    
private:
    Point start_pt;
    Point end_pt;
};

TEST(ParamCurveTest, Basic) {
    MockParamCurve curve(Point(0, 0), Point(10, 10));
    
    EXPECT_EQ(curve.start(), Point(0, 0));
    EXPECT_EQ(curve.end(), Point(10, 10));
    
    Point mid = curve.eval(0.5);
    EXPECT_DOUBLE_EQ(mid.x, 5.0);
    EXPECT_DOUBLE_EQ(mid.y, 5.0);
}

TEST(ParamCurveTest, Subsegment) {
    MockParamCurve curve(Point(0, 0), Point(10, 10));
    
    auto segment = curve.subsegment(0.25, 0.75);
    EXPECT_EQ(segment->start(), curve.eval(0.25));
    EXPECT_EQ(segment->end(), curve.eval(0.75));
    
    delete segment;
}

TEST(ParamCurveTest, Subdivide) {
    MockParamCurve curve(Point(0, 0), Point(10, 10));
    
    auto [first, second] = curve.subdivide();
    EXPECT_EQ(first->start(), Point(0, 0));
    EXPECT_EQ(first->end(), Point(5, 5));
    EXPECT_EQ(second->start(), Point(5, 5));
    EXPECT_EQ(second->end(), Point(10, 10));
    
    delete first;
    delete second;
}

TEST(ParamCurveDerivTest, Derivative) {
    MockParamCurveDeriv deriv(Point(0, 0), Point(10, 10));
    
    auto result = deriv.deriv();
    EXPECT_NE(result, nullptr);
    
    delete result;
}

TEST(ParamCurveArclenTest, ArcLength) {
    MockParamCurveArclen curve(Point(0, 0), Point(3, 4));
    
    double length = curve.arclen(0.001);
    EXPECT_DOUBLE_EQ(length, 5.0); // 3-4-5 triangle
}

TEST(ParamCurveNearestTest, Nearest) {
    MockParamCurveNearest curve(Point(0, 0), Point(10, 0));
    
    Point test_point(5, 3);
    Nearest result = curve.nearest(test_point, 0.001);
    
    EXPECT_GE(result.t, 0.0);
    EXPECT_LE(result.t, 1.0);
    EXPECT_GT(result.distance_sq, 0.0);
}

TEST(ParamCurveExtremaTest, Extrema) {
    MockParamCurveExtrema curve(Point(0, 0), Point(10, 10));
    
    auto extrema = curve.extrema();
    EXPECT_EQ(extrema.size(), 0); // No extrema for straight line
}

TEST(ParamCurveExtremaTest, ExtremaRanges) {
    MockParamCurveExtrema curve(Point(0, 0), Point(10, 10));
    
    auto ranges = curve.extrema_ranges();
    EXPECT_EQ(ranges.size(), 1);
    EXPECT_DOUBLE_EQ(ranges[0].first, 0.0);
    EXPECT_DOUBLE_EQ(ranges[0].second, 1.0);
}

TEST(ParamCurveExtremaTest, BoundingBox) {
    MockParamCurveExtrema curve(Point(0, 0), Point(10, 10));
    
    Rect bbox = curve.bounding_box();
    EXPECT_EQ(bbox.min_x(), 0.0);
    EXPECT_EQ(bbox.min_y(), 0.0);
    EXPECT_EQ(bbox.max_x(), 1.0);
    EXPECT_EQ(bbox.max_y(), 1.0);
} 