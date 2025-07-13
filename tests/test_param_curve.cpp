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
    
private:
    Point start_pt;
    Point end_pt;
};

class MockParamCurveDeriv : public ParamCurveDeriv {
public:
    MockParamCurveDeriv(Point start, Point end) : start_pt(start), end_pt(end) {}
    
    Point eval(double t) const override {
        return start_pt + (end_pt - start_pt) * t;
    }
    
    ParamCurve* subsegment(double start, double end) const override {
        Point new_start = eval(start);
        Point new_end = eval(end);
        return new MockParamCurveDeriv(new_start, new_end);
    }
    
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
    
    Point eval(double t) const override {
        return start_pt + (end_pt - start_pt) * t;
    }
    
    ParamCurve* subsegment(double start, double end) const override {
        Point new_start = eval(start);
        Point new_end = eval(end);
        return new MockParamCurveArclen(new_start, new_end);
    }
    
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
    
    Point eval(double t) const override {
        return start_pt + (end_pt - start_pt) * t;
    }
    
    ParamCurve* subsegment(double start, double end) const override {
        Point new_start = eval(start);
        Point new_end = eval(end);
        return new MockParamCurveNearest(new_start, new_end);
    }
    
    Nearest nearest(const Point& p, double accuracy) const override {
        Vec2 line_vec = (end_pt - start_pt);
        Vec2 point_vec = (p - start_pt);
        
        double t = point_vec.dot(line_vec) / line_vec.dot(line_vec);
        t = std::max(0.0, std::min(1.0, t));
        
        Point nearest_pt = eval(t);
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
    
    Point eval(double t) const override {
        return start_pt + (end_pt - start_pt) * t;
    }
    
    ParamCurve* subsegment(double start, double end) const override {
        Point new_start = eval(start);
        Point new_end = eval(end);
        return new MockParamCurveExtrema(new_start, new_end);
    }
    
    std::vector<double> extrema() const override {
        // For a line segment, there are no interior extrema
        return {};
    }
    
private:
    Point start_pt;
    Point end_pt;
};

TEST(ParamCurveTest, BasicEvaluation) {
    Point start(0.0, 0.0);
    Point end(10.0, 10.0);
    MockParamCurve curve(start, end);
    
    Point result = curve.eval(0.5);
    EXPECT_DOUBLE_EQ(result.x, 5.0);
    EXPECT_DOUBLE_EQ(result.y, 5.0);
    
    EXPECT_EQ(curve.start(), start);
    EXPECT_EQ(curve.end(), end);
}

TEST(ParamCurveTest, Subdivision) {
    Point start(0.0, 0.0);
    Point end(10.0, 10.0);
    MockParamCurve curve(start, end);
    
    auto [first, second] = curve.subdivide();
    
    EXPECT_EQ(first->start(), start);
    EXPECT_EQ(first->end(), Point(5.0, 5.0));
    EXPECT_EQ(second->start(), Point(5.0, 5.0));
    EXPECT_EQ(second->end(), end);
    
    delete first;
    delete second;
}

TEST(ParamCurveTest, Subsegment) {
    Point start(0.0, 0.0);
    Point end(10.0, 10.0);
    MockParamCurve curve(start, end);
    
    auto segment = curve.subsegment(0.25, 0.75);
    EXPECT_EQ(segment->start(), Point(2.5, 2.5));
    EXPECT_EQ(segment->end(), Point(7.5, 7.5));
    
    delete segment;
}

TEST(ParamCurveDerivTest, Derivative) {
    Point start(0.0, 0.0);
    Point end(10.0, 10.0);
    MockParamCurveDeriv curve(start, end);
    
    auto deriv = curve.deriv();
    EXPECT_EQ(deriv->start(), Point(0.0, 0.0));
    EXPECT_EQ(deriv->end(), Point(10.0, 10.0));
    
    delete deriv;
}

TEST(ParamCurveArclenTest, ArcLength) {
    Point start(0.0, 0.0);
    Point end(3.0, 4.0);
    MockParamCurveArclen curve(start, end);
    
    double length = curve.arclen(0.001);
    EXPECT_DOUBLE_EQ(length, 5.0); // 3-4-5 triangle
    
    double t = curve.inv_arclen(2.5, 0.001);
    EXPECT_DOUBLE_EQ(t, 0.5);
}

TEST(ParamCurveNearestTest, NearestPoint) {
    Point start(0.0, 0.0);
    Point end(10.0, 0.0);
    MockParamCurveNearest curve(start, end);
    
    Point test_point(5.0, 3.0);
    Nearest result = curve.nearest(test_point, 0.001);
    
    EXPECT_DOUBLE_EQ(result.t, 0.5);
    EXPECT_DOUBLE_EQ(result.distance_sq, 9.0); // Distance should be 3
}

TEST(ParamCurveExtremaTest, Extrema) {
    Point start(0.0, 0.0);
    Point end(10.0, 10.0);
    MockParamCurveExtrema curve(start, end);
    
    auto extrema = curve.extrema();
    EXPECT_TRUE(extrema.empty()); // Line has no interior extrema
    
    auto ranges = curve.extrema_ranges();
    EXPECT_EQ(ranges.size(), 1);
    EXPECT_DOUBLE_EQ(ranges[0].first, 0.0);
    EXPECT_DOUBLE_EQ(ranges[0].second, 1.0);
}

TEST(ParamCurveExtremaTest, BoundingBox) {
    Point start(0.0, 0.0);
    Point end(10.0, 10.0);
    MockParamCurveExtrema curve(start, end);
    
    Rect bbox = curve.bounding_box();
    EXPECT_EQ(bbox.x0, 0.0);
    EXPECT_EQ(bbox.y0, 0.0);
    EXPECT_EQ(bbox.x1, 10.0);
    EXPECT_EQ(bbox.y1, 10.0);
}

TEST(ParamCurveTest, Constants) {
    EXPECT_DOUBLE_EQ(DEFAULT_ACCURACY, 1e-6);
    EXPECT_EQ(MAX_EXTREMA, 4);
}

TEST(NearestTest, Structure) {
    Nearest nearest{25.0, 0.5};
    EXPECT_DOUBLE_EQ(nearest.distance_sq, 25.0);
    EXPECT_DOUBLE_EQ(nearest.t, 0.5);
} 