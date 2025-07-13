#include "kurbo/common.hpp"
#include <cmath>
#include <algorithm>
#include <functional>

namespace kurbo {

// Forward declaration
std::vector<std::pair<double, double>> factor_quartic_inner(double a, double b, double c, double d, bool rescale);

std::vector<double> solve_cubic(double c0, double c1, double c2, double c3) {
    std::vector<double> result;
    
    if (c3 == 0.0) {
        // Cubic coefficient is zero, solve quadratic equation
        return solve_quadratic(c0, c1, c2);
    }
    
    const double c3_recip = 1.0 / c3;
    const double ONETHIRD = 1.0 / 3.0;
    const double scaled_c2 = c2 * (ONETHIRD * c3_recip);
    const double scaled_c1 = c1 * (ONETHIRD * c3_recip);
    const double scaled_c0 = c0 * c3_recip;
    
    if (!std::isfinite(scaled_c0) || !std::isfinite(scaled_c1) || !std::isfinite(scaled_c2)) {
        // Cubic coefficient is zero or nearly so
        return solve_quadratic(c0, c1, c2);
    }
    
    const double c0_scaled = scaled_c0;
    const double c1_scaled = scaled_c1;
    const double c2_scaled = scaled_c2;
    
    // (d0, d1, d2) is called "Delta" in article
    const double d0 = (-c2_scaled) * c2_scaled + c1_scaled;
    const double d1 = (-c1_scaled) * c2_scaled + c0_scaled;
    const double d2 = c2_scaled * c0_scaled - c1_scaled * c1_scaled;
    
    // d is called "Discriminant"
    const double d = 4.0 * d0 * d2 - d1 * d1;
    
    // de is called "Depressed.x", Depressed.y = d0
    const double de = (-2.0 * c2_scaled) * d0 + d1;
    
    if (d < 0.0) {
        const double sq = std::sqrt(-0.25 * d);
        const double r = -0.5 * de;
        const double t1 = std::cbrt(r + sq) + std::cbrt(r - sq);
        result.push_back(t1 - c2_scaled);
    } else if (d == 0.0) {
        const double t1 = std::sqrt(-d0) * (de >= 0.0 ? 1.0 : -1.0);
        result.push_back(t1 - c2_scaled);
        result.push_back(-2.0 * t1 - c2_scaled);
    } else {
        const double th = std::atan2(std::sqrt(d), -de) * ONETHIRD;
        const double th_sin = std::sin(th);
        const double th_cos = std::cos(th);
        
        // (r0, r1, r2) is called "Root"
        const double r0 = th_cos;
        const double ss3 = th_sin * std::sqrt(3.0);
        const double r1 = 0.5 * (-th_cos + ss3);
        const double r2 = 0.5 * (-th_cos - ss3);
        const double t = 2.0 * std::sqrt(-d0);
        
        result.push_back(t * r0 - c2_scaled);
        result.push_back(t * r1 - c2_scaled);
        result.push_back(t * r2 - c2_scaled);
    }
    
    return result;
}

std::vector<double> solve_quadratic(double c0, double c1, double c2) {
    std::vector<double> result;
    
    if (c2 == 0.0) {
        // c2 is zero, treat as linear equation
        if (c1 != 0.0) {
            const double root = -c0 / c1;
            if (std::isfinite(root)) {
                result.push_back(root);
            }
        } else if (c0 == 0.0 && c1 == 0.0) {
            // Degenerate case
            result.push_back(0.0);
        }
        return result;
    }
    
    const double sc0 = c0 / c2;
    const double sc1 = c1 / c2;
    
    if (!std::isfinite(sc0) || !std::isfinite(sc1)) {
        // c2 is zero or very small, treat as linear equation
        const double root = -c0 / c1;
        if (std::isfinite(root)) {
            result.push_back(root);
        } else if (c0 == 0.0 && c1 == 0.0) {
            // Degenerate case
            result.push_back(0.0);
        }
        return result;
    }
    
    const double arg = sc1 * sc1 - 4.0 * sc0;
    
    if (!std::isfinite(arg)) {
        // Likely, calculation of sc1 * sc1 overflowed. Find one root
        // using sc1 x + x² = 0, other root as sc0 / root1.
        const double root1 = -sc1;
        result.push_back(root1);
        return result;
    }
    
    if (arg < 0.0) {
        return result; // No real roots
    } else if (arg == 0.0) {
        result.push_back(-0.5 * sc1);
        return result;
    }
    
    // See https://math.stackexchange.com/questions/866331
    const double root1 = -0.5 * (sc1 + std::sqrt(arg) * (sc1 >= 0.0 ? 1.0 : -1.0));
    const double root2 = sc0 / root1;
    
    if (std::isfinite(root2)) {
        // Sort just to be friendly and make results deterministic.
        if (root2 > root1) {
            result.push_back(root1);
            result.push_back(root2);
        } else {
            result.push_back(root2);
            result.push_back(root1);
        }
    } else {
        result.push_back(root1);
    }
    
    return result;
}

std::vector<double> solve_quartic(double c0, double c1, double c2, double c3, double c4) {
    if (c4 == 0.0) {
        return solve_cubic(c0, c1, c2, c3);
    }
    
    // Normalize to monic form
    const double c4_recip = 1.0 / c4;
    const double a = c3 * c4_recip;
    const double b = c2 * c4_recip;
    const double c = c1 * c4_recip;
    const double d = c0 * c4_recip;
    
    // Try to factor into quadratics
    auto factors = factor_quartic_inner(a, b, c, d, false);
    if (factors.empty()) {
        factors = factor_quartic_inner(a, b, c, d, true);
    }
    
    std::vector<double> result;
    for (const auto& [p, q] : factors) {
        auto quad_roots = solve_quadratic(q, p, 1.0);
        result.insert(result.end(), quad_roots.begin(), quad_roots.end());
    }
    
    return result;
}

std::vector<std::pair<double, double>> factor_quartic(double c0, double c1, double c2, double c3, double c4) {
    if (c4 == 0.0) {
        return {}; // Not a quartic
    }
    
    const double c4_recip = 1.0 / c4;
    const double a = c3 * c4_recip;
    const double b = c2 * c4_recip;
    const double c = c1 * c4_recip;
    const double d = c0 * c4_recip;
    
    auto result = factor_quartic_inner(a, b, c, d, false);
    if (result.empty()) {
        result = factor_quartic_inner(a, b, c, d, true);
    }
    
    return result;
}

std::vector<std::pair<double, double>> factor_quartic_inner(double a, double b, double c, double d, bool rescale) {
    // Implementation of quartic factoring algorithm
    // This is a simplified version - full implementation would be more complex
    
    // For now, return empty vector to indicate no real quadratic factors found
    return {};
}

double solve_itp(std::function<double(double)> f, double a, double b, 
                 double epsilon, size_t n0, double k1, double ya, double yb) {
    if (ya * yb > 0.0) {
        // Function values have same sign, no root in interval
        return std::numeric_limits<double>::quiet_NaN();
    }
    
    double x1_2 = 0.5 * (a + b);
    const double eps_x = epsilon * (b - a);
    const double eps_t = epsilon * std::min(std::abs(ya), std::abs(yb));
    
    size_t n = 0;
    double x_t = x1_2;
    
    while (b - a > eps_x) {
        const double x1_2_prev = x1_2;
        const double x_t_prev = x_t;
        
        // Interpolate
        const double r = (yb - ya) / (b - a);
        const double x_i = a - ya / r;
        
        // Truncate
        const double sigma = std::pow(0.5, n);
        const double x_t_new = x_i + sigma * (x1_2 - x_i);
        x_t = x_t_new;
        
        // Project
        const double delta = k1 * std::pow(b - a, 2.0);
        const double x_p = x_t + delta * (x_t_prev - x_t);
        
        const double x_new = (std::abs(x_p - x1_2) < 0.5 * (b - a)) ? x_p : x1_2;
        
        const double y_new = f(x_new);
        
        if (std::abs(y_new) < eps_t) {
            return x_new;
        }
        
        if (y_new * ya > 0.0) {
            a = x_new;
            ya = y_new;
        } else {
            b = x_new;
            yb = y_new;
        }
        
        x1_2 = 0.5 * (a + b);
        n++;
        
        if (n >= n0) {
            // Switch to secant method
            const double x_s = a - ya * (b - a) / (yb - ya);
            const double y_s = f(x_s);
            
            if (std::abs(y_s) < eps_t) {
                return x_s;
            }
            
            if (y_s * ya > 0.0) {
                a = x_s;
                ya = y_s;
            } else {
                b = x_s;
                yb = y_s;
            }
            
            x1_2 = 0.5 * (a + b);
        }
    }
    
    return x1_2;
}

// Gauss-Legendre quadrature coefficients
const std::array<std::pair<double, double>, 3> GAUSS_LEGENDRE_COEFFS_3 = {{
    {0.5555555555555556, 0.8888888888888888},
    {0.8888888888888888, 0.5555555555555556},
    {0.5555555555555556, 0.8888888888888888}
}};

const std::array<std::pair<double, double>, 4> GAUSS_LEGENDRE_COEFFS_4 = {{
    {0.3478548451374538, 0.6521451548625461},
    {0.6521451548625461, 0.3478548451374538},
    {0.6521451548625461, 0.3478548451374538},
    {0.3478548451374538, 0.6521451548625461}
}};

const std::array<std::pair<double, double>, 5> GAUSS_LEGENDRE_COEFFS_5 = {{
    {0.2369268850561891, 0.5688888888888889},
    {0.4786286704993665, 0.4786286704993665},
    {0.5688888888888889, 0.2369268850561891},
    {0.4786286704993665, 0.4786286704993665},
    {0.2369268850561891, 0.5688888888888889}
}};

const std::array<std::pair<double, double>, 6> GAUSS_LEGENDRE_COEFFS_6 = {{
    {0.1713244923791704, 0.3607615730481386},
    {0.3607615730481386, 0.1713244923791704},
    {0.4679139345726910, 0.4679139345726910},
    {0.4679139345726910, 0.4679139345726910},
    {0.3607615730481386, 0.1713244923791704},
    {0.1713244923791704, 0.3607615730481386}
}};

const std::array<std::pair<double, double>, 7> GAUSS_LEGENDRE_COEFFS_7 = {{
    {0.1294849661688697, 0.2797053914892766},
    {0.2797053914892766, 0.1294849661688697},
    {0.3818300505051189, 0.3818300505051189},
    {0.4179591836734694, 0.4179591836734694},
    {0.3818300505051189, 0.3818300505051189},
    {0.2797053914892766, 0.1294849661688697},
    {0.1294849661688697, 0.2797053914892766}
}};

const std::array<std::pair<double, double>, 8> GAUSS_LEGENDRE_COEFFS_8 = {{
    {0.1012285362903763, 0.2223810344533745},
    {0.2223810344533745, 0.1012285362903763},
    {0.3137066458778873, 0.3137066458778873},
    {0.3626837833783620, 0.3626837833783620},
    {0.3626837833783620, 0.3626837833783620},
    {0.3137066458778873, 0.3137066458778873},
    {0.2223810344533745, 0.1012285362903763},
    {0.1012285362903763, 0.2223810344533745}
}};

const std::array<std::pair<double, double>, 8> GAUSS_LEGENDRE_COEFFS_8_HALF = {{
    {0.1012285362903763, 0.2223810344533745},
    {0.2223810344533745, 0.1012285362903763},
    {0.3137066458778873, 0.3137066458778873},
    {0.3626837833783620, 0.3626837833783620}
}};

const std::array<std::pair<double, double>, 9> GAUSS_LEGENDRE_COEFFS_9 = {{
    {0.0812743883615744, 0.1806481606948574},
    {0.1806481606948574, 0.0812743883615744},
    {0.2606106964029354, 0.2606106964029354},
    {0.3123470770400029, 0.3123470770400029},
    {0.3302393550012598, 0.3302393550012598},
    {0.3123470770400029, 0.3123470770400029},
    {0.2606106964029354, 0.2606106964029354},
    {0.1806481606948574, 0.0812743883615744},
    {0.0812743883615744, 0.1806481606948574}
}};

const std::array<std::pair<double, double>, 11> GAUSS_LEGENDRE_COEFFS_11 = {{
    {0.0666713443086881, 0.1494513491505806},
    {0.1494513491505806, 0.0666713443086881},
    {0.2190863625159821, 0.2190863625159821},
    {0.2692667193099963, 0.2692667193099963},
    {0.2955242247147529, 0.2955242247147529},
    {0.2955242247147529, 0.2955242247147529},
    {0.2692667193099963, 0.2692667193099963},
    {0.2190863625159821, 0.2190863625159821},
    {0.1494513491505806, 0.0666713443086881},
    {0.0666713443086881, 0.1494513491505806}
}};

const std::array<std::pair<double, double>, 16> GAUSS_LEGENDRE_COEFFS_16 = {{
    {0.0471753363865118, 0.1069393259953184},
    {0.1069393259953184, 0.0471753363865118},
    {0.1600783285433462, 0.1600783285433462},
    {0.2031674267230659, 0.2031674267230659},
    {0.2334925365383548, 0.2334925365383548},
    {0.2491470458134028, 0.2491470458134028},
    {0.2491470458134028, 0.2491470458134028},
    {0.2334925365383548, 0.2334925365383548},
    {0.2031674267230659, 0.2031674267230659},
    {0.1600783285433462, 0.1600783285433462},
    {0.1069393259953184, 0.0471753363865118},
    {0.0471753363865118, 0.1069393259953184}
}};

const std::array<std::pair<double, double>, 16> GAUSS_LEGENDRE_COEFFS_16_HALF = {{
    {0.0471753363865118, 0.1069393259953184},
    {0.1069393259953184, 0.0471753363865118},
    {0.1600783285433462, 0.1600783285433462},
    {0.2031674267230659, 0.2031674267230659},
    {0.2334925365383548, 0.2334925365383548},
    {0.2491470458134028, 0.2491470458134028}
}};

const std::array<std::pair<double, double>, 24> GAUSS_LEGENDRE_COEFFS_24 = {{
    {0.0325581623079647, 0.0741587837571950},
    {0.0741587837571950, 0.0325581623079647},
    {0.1124889431331866, 0.1124889431331866},
    {0.1460916426319828, 0.1460916426319828},
    {0.1736870776440010, 0.1736870776440010},
    {0.1946171403342249, 0.1946171403342249},
    {0.2081899110804994, 0.2081899110804994},
    {0.2140130170492664, 0.2140130170492664},
    {0.2140130170492664, 0.2140130170492664},
    {0.2081899110804994, 0.2081899110804994},
    {0.1946171403342249, 0.1946171403342249},
    {0.1736870776440010, 0.1736870776440010},
    {0.1460916426319828, 0.1460916426319828},
    {0.1124889431331866, 0.1124889431331866},
    {0.0741587837571950, 0.0325581623079647},
    {0.0325581623079647, 0.0741587837571950}
}};

const std::array<std::pair<double, double>, 24> GAUSS_LEGENDRE_COEFFS_24_HALF = {{
    {0.0325581623079647, 0.0741587837571950},
    {0.0741587837571950, 0.0325581623079647},
    {0.1124889431331866, 0.1124889431331866},
    {0.1460916426319828, 0.1460916426319828},
    {0.1736870776440010, 0.1736870776440010},
    {0.1946171403342249, 0.1946171403342249},
    {0.2081899110804994, 0.2081899110804994},
    {0.2140130170492664, 0.2140130170492664}
}};

const std::array<std::pair<double, double>, 32> GAUSS_LEGENDRE_COEFFS_32 = {{
    {0.0244461801962625, 0.0563443130465926},
    {0.0563443130465926, 0.0244461801962625},
    {0.0876520930044038, 0.0876520930044038},
    {0.1151937607688005, 0.1151937607688005},
    {0.1397752667783648, 0.1397752667783648},
    {0.1603586456402254, 0.1603586456402254},
    {0.1765627053669926, 0.1765627053669926},
    {0.1880397373910009, 0.1880397373910009},
    {0.1945271090693755, 0.1945271090693755},
    {0.1945271090693755, 0.1945271090693755},
    {0.1880397373910009, 0.1880397373910009},
    {0.1765627053669926, 0.1765627053669926},
    {0.1603586456402254, 0.1603586456402254},
    {0.1397752667783648, 0.1397752667783648},
    {0.1151937607688005, 0.1151937607688005},
    {0.0876520930044038, 0.0876520930044038},
    {0.0563443130465926, 0.0244461801962625},
    {0.0244461801962625, 0.0563443130465926}
}};

const std::array<std::pair<double, double>, 32> GAUSS_LEGENDRE_COEFFS_32_HALF = {{
    {0.0244461801962625, 0.0563443130465926},
    {0.0563443130465926, 0.0244461801962625},
    {0.0876520930044038, 0.0876520930044038},
    {0.1151937607688005, 0.1151937607688005},
    {0.1397752667783648, 0.1397752667783648},
    {0.1603586456402254, 0.1603586456402254},
    {0.1765627053669926, 0.1765627053669926},
    {0.1880397373910009, 0.1880397373910009},
    {0.1945271090693755, 0.1945271090693755}
}};

} // namespace kurbo 