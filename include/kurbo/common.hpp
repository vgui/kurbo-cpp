#pragma once

#include <array>
#include <cmath>
#include <vector>
#include <functional>

namespace kurbo {

/// Adds convenience methods to float types.
namespace detail {
    /// Rounds to the nearest integer away from zero,
    /// unless the provided value is already an integer.
    ///
    /// It is to ceil what trunc is to floor.
    template<typename T>
    T expand(T value) {
        return std::copysign(std::ceil(std::abs(value)), value);
    }
}

/// Find real roots of cubic equation.
///
/// The implementation is not (yet) fully robust, but it does handle the case
/// where c3 is zero (in that case, solving the quadratic equation).
///
/// Return values of x for which c0 + c1 x + c2 x² + c3 x³ = 0.
std::vector<double> solve_cubic(double c0, double c1, double c2, double c3);

/// Find real roots of quadratic equation.
///
/// Return values of x for which c0 + c1 x + c2 x² = 0.
///
/// This function tries to be quite numerically robust. If the equation
/// is nearly linear, it will return the root ignoring the quadratic term;
/// the other root might be out of representable range. In the degenerate
/// case where all coefficients are zero, so that all values of x satisfy
/// the equation, a single 0.0 is returned.
std::vector<double> solve_quadratic(double c0, double c1, double c2);

/// Find real roots of quartic equation.
///
/// Return values of x for which c0 + c1 x + c2 x² + c3 x³ + c4 x⁴ = 0.
std::vector<double> solve_quartic(double c0, double c1, double c2, double c3, double c4);

/// Factor quartic equation into quadratic factors.
///
/// Returns pairs of (a, b) where each pair represents a factor (x² + a x + b).
/// If the quartic cannot be factored into real quadratics, returns empty vector.
std::vector<std::pair<double, double>> factor_quartic(double c0, double c1, double c2, double c3, double c4);

/// Solve equation using ITP (Interpolate Truncate Project) method.
///
/// This is a robust root-finding algorithm that combines the reliability
/// of bisection with the speed of secant method.
///
/// Parameters:
/// - f: function to find root of
/// - a, b: interval bounds
/// - epsilon: desired accuracy
/// - n0: number of iterations before switching to secant
/// - k1: parameter controlling projection
/// - ya, yb: function values at a and b
///
/// Returns the root x where f(x) ≈ 0.
double solve_itp(std::function<double(double)> f, double a, double b, 
                 double epsilon, size_t n0, double k1, double ya, double yb);

/// Gauss-Legendre quadrature coefficients for various orders
extern const std::array<std::pair<double, double>, 3> GAUSS_LEGENDRE_COEFFS_3;
extern const std::array<std::pair<double, double>, 4> GAUSS_LEGENDRE_COEFFS_4;
extern const std::array<std::pair<double, double>, 5> GAUSS_LEGENDRE_COEFFS_5;
extern const std::array<std::pair<double, double>, 6> GAUSS_LEGENDRE_COEFFS_6;
extern const std::array<std::pair<double, double>, 7> GAUSS_LEGENDRE_COEFFS_7;
extern const std::array<std::pair<double, double>, 8> GAUSS_LEGENDRE_COEFFS_8;
extern const std::array<std::pair<double, double>, 4> GAUSS_LEGENDRE_COEFFS_8_HALF;
extern const std::array<std::pair<double, double>, 9> GAUSS_LEGENDRE_COEFFS_9;
extern const std::array<std::pair<double, double>, 11> GAUSS_LEGENDRE_COEFFS_11;
extern const std::array<std::pair<double, double>, 16> GAUSS_LEGENDRE_COEFFS_16;
extern const std::array<std::pair<double, double>, 8> GAUSS_LEGENDRE_COEFFS_16_HALF;
extern const std::array<std::pair<double, double>, 24> GAUSS_LEGENDRE_COEFFS_24;
extern const std::array<std::pair<double, double>, 12> GAUSS_LEGENDRE_COEFFS_24_HALF;
extern const std::array<std::pair<double, double>, 32> GAUSS_LEGENDRE_COEFFS_32;
extern const std::array<std::pair<double, double>, 16> GAUSS_LEGENDRE_COEFFS_32_HALF;

} // namespace kurbo 