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

// TODO: Add more Arc-specific tests as needed, e.g., for append_iter, to_cubic_beziers, etc. 