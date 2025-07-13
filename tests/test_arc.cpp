#include <gtest/gtest.h>
#include "kurbo/arc.hpp"
#include <cmath>
#include "kurbo/bezpath.hpp"

using namespace kurbo;

// Helper for floating point comparison
static void assert_near(double a, double b, double eps = 1e-12) {
    ASSERT_NEAR(a, b, eps);
}

TEST(ArcTest, ReversedArc) {
    // Equivalent to Rust: let a = Arc::new((0., 0.), (1., 0.), 0., PI, 0.);
    Arc a = Arc::new_arc(Point(0.0, 0.0), Vec2(1.0, 0.0), 0.0, M_PI, 0.0);
    Arc f = a.reversed();

    // Most fields should be unchanged
    EXPECT_EQ(a.center, f.center);
    EXPECT_EQ(a.radii, f.radii);
    EXPECT_EQ(a.x_rotation, f.x_rotation);

    // Sweep angle should be in reverse
    EXPECT_DOUBLE_EQ(a.sweep_angle, -f.sweep_angle);

    // Reversing it again should result in the original arc
    Arc r = f.reversed();
    EXPECT_EQ(a.center, r.center);
    EXPECT_EQ(a.radii, r.radii);
    EXPECT_EQ(a.x_rotation, r.x_rotation);
    EXPECT_DOUBLE_EQ(a.start_angle, r.start_angle);
    EXPECT_DOUBLE_EQ(a.sweep_angle, r.sweep_angle);
}

TEST(ArcTest, BezPathSmoke) {
    kurbo::BezPath path;
    path.move_to({0, 0});
    path.line_to({1, 0});
    path.quad_to({1, 1}, {0, 1});
    path.curve_to({-1, 1}, {-1, 0}, {0, 0});
    path.close_path();
    EXPECT_FALSE(path.is_empty());
    EXPECT_EQ(path.elements().size(), 5);
}

TEST(ArcTest, BezPathComprehensive) {
    // Test constructors
    BezPath empty;
    EXPECT_TRUE(empty.is_empty());
    
    BezPath with_cap = BezPath::with_capacity(10);
    EXPECT_TRUE(with_cap.is_empty());
    
    std::vector<PathEl> els = {
        PathEl(PathElType::MoveTo, Point(0, 0)),
        PathEl(PathElType::LineTo, Point(1, 0)),
        PathEl(PathElType::QuadTo, Point(1, 1), Point(0, 1)),
        PathEl(PathElType::CurveTo, Point(-1, 1), Point(-1, 0), Point(0, 0)),
        PathEl(PathElType::ClosePath)
    };
    BezPath from_vec = BezPath::from_vec(els);
    EXPECT_FALSE(from_vec.is_empty());
    EXPECT_EQ(from_vec.elements().size(), 5);
    
    // Test manipulation
    BezPath path;
    path.move_to({0, 0});
    path.line_to({1, 0});
    path.quad_to({1, 1}, {0, 1});
    path.curve_to({-1, 1}, {-1, 0}, {0, 0});
    path.close_path();
    
    EXPECT_FALSE(path.is_empty());
    EXPECT_EQ(path.elements().size(), 5);
    
    // Test pop
    auto popped = path.pop();
    EXPECT_TRUE(popped.has_value());
    EXPECT_EQ(popped->type, PathElType::ClosePath);
    EXPECT_EQ(path.elements().size(), 4);
    
    // Test truncate
    path.truncate(2);
    EXPECT_EQ(path.elements().size(), 2);
    
    // Test get_seg
    auto seg = path.get_seg(1);
    EXPECT_TRUE(seg.has_value());
    EXPECT_EQ(seg->type(), PathSeg::Kind::Line);
    
    // Test is_finite and is_nan
    EXPECT_TRUE(path.is_finite());
    EXPECT_FALSE(path.is_nan());
    
    // Test control_box
    Rect cbox = path.control_box();
    EXPECT_TRUE(cbox.is_finite());
    
    // Test reverse_subpaths
    BezPath reversed = path.reverse_subpaths();
    EXPECT_EQ(reversed.elements().size(), 2);
    
    // Test segments iterator
    auto segments = path.segments();
    auto it = segments.begin();
    EXPECT_NE(it, segments.end());
    
    // Test operators
    Affine scale = Affine::scale(2.0);
    BezPath scaled = scale * path;
    EXPECT_EQ(scaled.elements().size(), 2);
}

TEST(ArcTest, BezPathFlattenAreaPerimeterWindingBBox) {
    // Square path
    BezPath path;
    path.move_to({0, 0});
    path.line_to({1, 0});
    path.line_to({1, 1});
    path.line_to({0, 1});
    path.close_path();

    // Flatten
    std::vector<PathEl> flat;
    path.flatten(1e-6, flat);
    int line_count = 0;
    for (const auto& el : flat) {
        if (el.type == PathElType::LineTo) ++line_count;
    }
    EXPECT_EQ(line_count, 4);

    // Area
    double area = path.area();
    EXPECT_NEAR(area, 1.0, 1e-9);

    // Perimeter
    double peri = path.perimeter();
    EXPECT_NEAR(peri, 4.0, 1e-9);

    // Winding (inside)
    int wind = path.winding(Point(0.5, 0.5));
    EXPECT_EQ(wind, 1);

    // Winding (outside)
    wind = path.winding(Point(2, 2));
    EXPECT_EQ(wind, 0);

    // Bounding box
    Rect bbox = path.bounding_box();
    EXPECT_NEAR(bbox.x0, 0.0, 1e-9);
    EXPECT_NEAR(bbox.y0, 0.0, 1e-9);
    EXPECT_NEAR(bbox.x1, 1.0, 1e-9);
    EXPECT_NEAR(bbox.y1, 1.0, 1e-9);
}

TEST(ArcTest, PathSegMethods) {
    // Test Line PathSeg
    Line line(Point(0, 0), Point(1, 1));
    PathSeg line_seg(line);
    EXPECT_EQ(line_seg.type(), PathSeg::Kind::Line);
    EXPECT_EQ(line_seg.as_line(), line);
    
    // Test QuadBez PathSeg
    QuadBez quad(Point(0, 0), Point(1, 1), Point(2, 0));
    PathSeg quad_seg(quad);
    EXPECT_EQ(quad_seg.type(), PathSeg::Kind::Quad);
    EXPECT_EQ(quad_seg.as_quad(), quad);
    
    // Test CubicBez PathSeg
    CubicBez cubic(Point(0, 0), Point(1, 1), Point(2, 0), Point(3, 1));
    PathSeg cubic_seg(cubic);
    EXPECT_EQ(cubic_seg.type(), PathSeg::Kind::Cubic);
    EXPECT_EQ(cubic_seg.as_cubic(), cubic);
    
    // Test as_path_el
    PathEl line_el = line_seg.as_path_el();
    EXPECT_EQ(line_el.type, PathElType::LineTo);
    EXPECT_EQ(line_el.point, Point(1, 1));
    
    // Test reverse
    PathSeg reversed_line = line_seg.reverse();
    EXPECT_EQ(reversed_line.type(), PathSeg::Kind::Line);
    EXPECT_EQ(reversed_line.as_line().p0, Point(1, 1));
    EXPECT_EQ(reversed_line.as_line().p1, Point(0, 0));
    
    // Test to_cubic
    CubicBez line_as_cubic = line_seg.to_cubic();
    EXPECT_TRUE(line_as_cubic.is_finite());
    
    // Test is_finite and is_nan
    EXPECT_TRUE(line_seg.is_finite());
    EXPECT_FALSE(line_seg.is_nan());
    
    // Test operators
    Affine scale = Affine::scale(2.0);
    PathSeg scaled = scale * line_seg;
    EXPECT_EQ(scaled.type(), PathSeg::Kind::Line);
}

TEST(ArcTest, LineIntersectionAndMinDistance) {
    LineIntersection intersection(0.5, 0.3);
    EXPECT_TRUE(intersection.is_finite());
    EXPECT_FALSE(intersection.is_nan());
    EXPECT_DOUBLE_EQ(intersection.line_t, 0.5);
    EXPECT_DOUBLE_EQ(intersection.segment_t, 0.3);
    
    MinDistance min_dist(1.5, 0.2, 0.8);
    EXPECT_DOUBLE_EQ(min_dist.distance, 1.5);
    EXPECT_DOUBLE_EQ(min_dist.t1, 0.2);
    EXPECT_DOUBLE_EQ(min_dist.t2, 0.8);
}

// TODO: Add more Arc-specific tests as needed, e.g., for append_iter, to_cubic_beziers, etc. 