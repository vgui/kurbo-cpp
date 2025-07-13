#include <gtest/gtest.h>
#include "kurbo/common.hpp"
#include <cmath>
#include <vector>
#include <limits>
#include <algorithm>
#include <iostream> // Added for std::cout

using namespace kurbo;

TEST(CommonTest, SolveCubic) {
    auto roots1 = solve_cubic(-5.0, 0.0, 0.0, 1.0);
    ASSERT_EQ(roots1.size(), 1);
    EXPECT_NEAR(roots1[0], std::cbrt(5.0), 1e-12);

    auto roots2 = solve_cubic(-5.0, -1.0, 0.0, 1.0);
    ASSERT_EQ(roots2.size(), 1);
    EXPECT_NEAR(roots2[0], 1.90416085913492, 1e-10);

    auto roots3 = solve_cubic(0.0, -1.0, 0.0, 1.0);
    ASSERT_EQ(roots3.size(), 3);
    std::sort(roots3.begin(), roots3.end());
    EXPECT_NEAR(roots3[0], -1.0, 1e-12);
    EXPECT_NEAR(roots3[1], 0.0, 1e-12);
    EXPECT_NEAR(roots3[2], 1.0, 1e-12);

    auto roots4 = solve_cubic(-2.0, -3.0, 0.0, 1.0);
    ASSERT_EQ(roots4.size(), 2);
    std::sort(roots4.begin(), roots4.end());
    EXPECT_NEAR(roots4[0], -1.0, 1e-12);
    EXPECT_NEAR(roots4[1], 2.0, 1e-12);

    auto roots5 = solve_cubic(2.0, -3.0, 0.0, 1.0);
    ASSERT_EQ(roots5.size(), 2);
    std::sort(roots5.begin(), roots5.end());
    EXPECT_NEAR(roots5[0], -2.0, 1e-12);
    EXPECT_NEAR(roots5[1], 1.0, 1e-12);
}

TEST(CommonTest, SolveQuadratic) {
    auto roots1 = solve_quadratic(-5.0, 0.0, 1.0);
    ASSERT_EQ(roots1.size(), 2);
    std::sort(roots1.begin(), roots1.end());
    EXPECT_NEAR(roots1[0], -std::sqrt(5.0), 1e-12);
    EXPECT_NEAR(roots1[1], std::sqrt(5.0), 1e-12);

    auto roots2 = solve_quadratic(5.0, 0.0, 1.0);
    ASSERT_EQ(roots2.size(), 0);

    auto roots3 = solve_quadratic(5.0, 1.0, 0.0);
    ASSERT_EQ(roots3.size(), 1);
    EXPECT_NEAR(roots3[0], -5.0, 1e-12);

    auto roots4 = solve_quadratic(1.0, 2.0, 1.0);
    ASSERT_EQ(roots4.size(), 1);
    EXPECT_NEAR(roots4[0], -1.0, 1e-12);
}

TEST(CommonTest, SolveQuartic) {
    // x^4 - 10x^3 + 35x^2 - 50x + 24 = (x-1)(x-2)(x-3)(x-4)
    auto roots = solve_quartic(24.0, -50.0, 35.0, -10.0, 1.0);
    ASSERT_EQ(roots.size(), 4);
    std::sort(roots.begin(), roots.end());
    EXPECT_NEAR(roots[0], 1.0, 1e-10);
    EXPECT_NEAR(roots[1], 2.0, 1e-10);
    EXPECT_NEAR(roots[2], 3.0, 1e-10);
    EXPECT_NEAR(roots[3], 4.0, 1e-10);
}

TEST(CommonTest, FactorQuartic) {
    // Тестовый случай: x^4 - 10x^3 + 35x^2 - 50x + 24 = (x-1)(x-2)(x-3)(x-4)
    auto factors = factor_quartic(24.0, -50.0, 35.0, -10.0, 1.0);
    ASSERT_EQ(factors.size(), 2);
    if (std::abs(factors[0].first + 3.0) > 1e-10 || std::abs(factors[0].second - 2.0) > 1e-10) {
        std::cerr << "Factor 0 mismatch: (" << factors[0].first << ", " << factors[0].second << ")\n";
    }
    if (std::abs(factors[1].first + 7.0) > 1e-10 || std::abs(factors[1].second - 12.0) > 1e-10) {
        std::cerr << "Factor 1 mismatch: (" << factors[1].first << ", " << factors[1].second << ")\n";
    }
    ASSERT_NEAR(factors[0].first, -3.0, 1e-10);
    ASSERT_NEAR(factors[0].second, 2.0, 1e-10);
    ASSERT_NEAR(factors[1].first, -7.0, 1e-10);
    ASSERT_NEAR(factors[1].second, 12.0, 1e-10);
    std::cout << "Returned factors:\n";
    for (const auto& f : factors) {
        std::cout << "  (" << f.first << ", " << f.second << ")\n";
    }
}

TEST(CommonTest, SolveITP) {
    auto f = [](double x) { return x * x * x - x - 2.0; };
    double x = solve_itp(f, 1.0, 2.0, 1e-12, 0, 0.2, f(1.0), f(2.0));
    EXPECT_NEAR(f(x), 0.0, 6e-12);
}

TEST(CommonTest, Expand) {
    using kurbo::detail::expand;
    EXPECT_EQ(expand(3.7), 4.0);
    EXPECT_EQ(expand(3.0), 3.0);
    EXPECT_EQ(expand(-3.7), -4.0);
    EXPECT_EQ(expand(-5.1f), -6.0f);
}

TEST(CommonTest, GaussLegendreCoeffs) {
    EXPECT_EQ(GAUSS_LEGENDRE_COEFFS_3.size(), 3);
    EXPECT_EQ(GAUSS_LEGENDRE_COEFFS_4.size(), 4);
    EXPECT_EQ(GAUSS_LEGENDRE_COEFFS_5.size(), 5);
    EXPECT_EQ(GAUSS_LEGENDRE_COEFFS_6.size(), 6);
    EXPECT_EQ(GAUSS_LEGENDRE_COEFFS_7.size(), 7);
    EXPECT_EQ(GAUSS_LEGENDRE_COEFFS_8.size(), 8);
    EXPECT_EQ(GAUSS_LEGENDRE_COEFFS_8_HALF.size(), 4);
    EXPECT_EQ(GAUSS_LEGENDRE_COEFFS_9.size(), 9);
    EXPECT_EQ(GAUSS_LEGENDRE_COEFFS_11.size(), 11);
    EXPECT_EQ(GAUSS_LEGENDRE_COEFFS_16.size(), 16);
    EXPECT_EQ(GAUSS_LEGENDRE_COEFFS_16_HALF.size(), 8);
    EXPECT_EQ(GAUSS_LEGENDRE_COEFFS_24.size(), 24);
    EXPECT_EQ(GAUSS_LEGENDRE_COEFFS_24_HALF.size(), 12);
    EXPECT_EQ(GAUSS_LEGENDRE_COEFFS_32.size(), 32);
    EXPECT_EQ(GAUSS_LEGENDRE_COEFFS_32_HALF.size(), 16);
} 