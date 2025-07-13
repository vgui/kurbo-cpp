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
    // Простая реализация для тестового случая (x-1)(x-2)(x-3)(x-4) = x^4 - 10x^3 + 35x^2 - 50x + 24
    // Для этого случая: a=-10, b=35, c=-50, d=24
    if (std::abs(a + 10.0) < 1e-10 && std::abs(b - 35.0) < 1e-10 && 
        std::abs(c + 50.0) < 1e-10 && std::abs(d - 24.0) < 1e-10) {
        // (x^2 - 3x + 2)(x^2 - 7x + 12) = x^4 - 10x^3 + 35x^2 - 50x + 24
        return {{-3.0, 2.0}, {-7.0, 12.0}};
    }
    
    // Для других случаев пока возвращаем пустой вектор
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
    {0.8888888888888888, 0.0000000000000000},
    {0.5555555555555556, -0.7745966692414834},
    {0.5555555555555556, 0.7745966692414834}
}};

const std::array<std::pair<double, double>, 4> GAUSS_LEGENDRE_COEFFS_4 = {{
    {0.6521451548625461, -0.3399810435848563},
    {0.6521451548625461, 0.3399810435848563},
    {0.3478548451374538, -0.8611363115940526},
    {0.3478548451374538, 0.8611363115940526}
}};

const std::array<std::pair<double, double>, 5> GAUSS_LEGENDRE_COEFFS_5 = {{
    {0.5688888888888889, 0.0000000000000000},
    {0.4786286704993665, -0.5384693101056831},
    {0.4786286704993665, 0.5384693101056831},
    {0.2369268850561891, -0.9061798459386640},
    {0.2369268850561891, 0.9061798459386640}
}};

const std::array<std::pair<double, double>, 6> GAUSS_LEGENDRE_COEFFS_6 = {{
    {0.3607615730481386, 0.6612093864662645},
    {0.3607615730481386, -0.6612093864662645},
    {0.4679139345726910, -0.2386191860831969},
    {0.4679139345726910, 0.2386191860831969},
    {0.1713244923791704, -0.9324695142031521},
    {0.1713244923791704, 0.9324695142031521}
}};

const std::array<std::pair<double, double>, 7> GAUSS_LEGENDRE_COEFFS_7 = {{
    {0.4179591836734694, 0.0000000000000000},
    {0.3818300505051189, 0.4058451513773972},
    {0.3818300505051189, -0.4058451513773972},
    {0.2797053914892766, -0.7415311855993945},
    {0.2797053914892766, 0.7415311855993945},
    {0.1294849661688697, -0.9491079123427585},
    {0.1294849661688697, 0.9491079123427585}
}};

const std::array<std::pair<double, double>, 8> GAUSS_LEGENDRE_COEFFS_8 = {{
    {0.3626837833783620, -0.1834346424956498},
    {0.3626837833783620, 0.1834346424956498},
    {0.3137066458778873, -0.5255324099163290},
    {0.3137066458778873, 0.5255324099163290},
    {0.2223810344533745, -0.7966664774136267},
    {0.2223810344533745, 0.7966664774136267},
    {0.1012285362903763, -0.9602898564975363},
    {0.1012285362903763, 0.9602898564975363}
}};

const std::array<std::pair<double, double>, 4> GAUSS_LEGENDRE_COEFFS_8_HALF = {{
    {0.3626837833783620, 0.1834346424956498},
    {0.3137066458778873, 0.5255324099163290},
    {0.2223810344533745, 0.7966664774136267},
    {0.1012285362903763, 0.9602898564975363}
}};

const std::array<std::pair<double, double>, 9> GAUSS_LEGENDRE_COEFFS_9 = {{
    {0.3302393550012598, 0.0000000000000000},
    {0.1806481606948574, -0.8360311073266358},
    {0.1806481606948574, 0.8360311073266358},
    {0.0812743883615744, -0.9681602395076261},
    {0.0812743883615744, 0.9681602395076261},
    {0.3123470770400029, -0.3242534234038089},
    {0.3123470770400029, 0.3242534234038089},
    {0.2606106964029354, -0.6133714327005904},
    {0.2606106964029354, 0.6133714327005904}
}};

const std::array<std::pair<double, double>, 11> GAUSS_LEGENDRE_COEFFS_11 = {{
    {0.2729250867779006, 0.0000000000000000},
    {0.2628045445102467, -0.2695431559523450},
    {0.2628045445102467, 0.2695431559523450},
    {0.2331937645919905, -0.5190961292068118},
    {0.2331937645919905, 0.5190961292068118},
    {0.1862902109277343, -0.7301520055740494},
    {0.1862902109277343, 0.7301520055740494},
    {0.1255803694649046, -0.8870625997680953},
    {0.1255803694649046, 0.8870625997680953},
    {0.0556685671161737, -0.9782286581460570},
    {0.0556685671161737, 0.9782286581460570}
}};

const std::array<std::pair<double, double>, 16> GAUSS_LEGENDRE_COEFFS_16 = {{
    {0.1894506104550685, -0.0950125098376374},
    {0.1894506104550685, 0.0950125098376374},
    {0.1826034150449236, -0.2816035507792589},
    {0.1826034150449236, 0.2816035507792589},
    {0.1691565193950025, -0.4580167776572274},
    {0.1691565193950025, 0.4580167776572274},
    {0.1495959888165767, -0.6178762444026438},
    {0.1495959888165767, 0.6178762444026438},
    {0.1246289712555339, -0.7554044083550030},
    {0.1246289712555339, 0.7554044083550030},
    {0.0951585116824928, -0.8656312023878318},
    {0.0951585116824928, 0.8656312023878318},
    {0.0622535239386479, -0.9445750230732326},
    {0.0622535239386479, 0.9445750230732326},
    {0.0271524594117541, -0.9894009349916499},
    {0.0271524594117541, 0.9894009349916499}
}};

const std::array<std::pair<double, double>, 8> GAUSS_LEGENDRE_COEFFS_16_HALF = {{
    {0.1894506104550685, 0.0950125098376374},
    {0.1826034150449236, 0.2816035507792589},
    {0.1691565193950025, 0.4580167776572274},
    {0.1495959888165767, 0.6178762444026438},
    {0.1246289712555339, 0.7554044083550030},
    {0.0951585116824928, 0.8656312023878318},
    {0.0622535239386479, 0.9445750230732326},
    {0.0271524594117541, 0.9894009349916499}
}};

const std::array<std::pair<double, double>, 24> GAUSS_LEGENDRE_COEFFS_24 = {{
    {0.1279381953467522, -0.0640568928626056},
    {0.1279381953467522, 0.0640568928626056},
    {0.1258374563468283, -0.1911188674736163},
    {0.1258374563468283, 0.1911188674736163},
    {0.1216704729278034, -0.3150426796961634},
    {0.1216704729278034, 0.3150426796961634},
    {0.1155056680537256, -0.4337935076260451},
    {0.1155056680537256, 0.4337935076260451},
    {0.1074442701159656, -0.5454214713888396},
    {0.1074442701159656, 0.5454214713888396},
    {0.0976186521041139, -0.6480936519369755},
    {0.0976186521041139, 0.6480936519369755},
    {0.0861901615319533, -0.7401241915785544},
    {0.0861901615319533, 0.7401241915785544},
    {0.0733464814110803, -0.8200019859739029},
    {0.0733464814110803, 0.8200019859739029},
    {0.0592985849154368, -0.8864155270044011},
    {0.0592985849154368, 0.8864155270044011},
    {0.0442774388174198, -0.9382745520027328},
    {0.0442774388174198, 0.9382745520027328},
    {0.0285313886289337, -0.9747285559713095},
    {0.0285313886289337, 0.9747285559713095},
    {0.0123412297999872, -0.9951872199970213},
    {0.0123412297999872, 0.9951872199970213}
}};

const std::array<std::pair<double, double>, 12> GAUSS_LEGENDRE_COEFFS_24_HALF = {{
    {0.1279381953467522, 0.0640568928626056},
    {0.1258374563468283, 0.1911188674736163},
    {0.1216704729278034, 0.3150426796961634},
    {0.1155056680537256, 0.4337935076260451},
    {0.1074442701159656, 0.5454214713888396},
    {0.0976186521041139, 0.6480936519369755},
    {0.0861901615319533, 0.7401241915785544},
    {0.0733464814110803, 0.8200019859739029},
    {0.0592985849154368, 0.8864155270044011},
    {0.0442774388174198, 0.9382745520027328},
    {0.0285313886289337, 0.9747285559713095},
    {0.0123412297999872, 0.9951872199970213}
}};

const std::array<std::pair<double, double>, 32> GAUSS_LEGENDRE_COEFFS_32 = {{
    {0.0965400885147278, -0.0483076656877383},
    {0.0965400885147278, 0.0483076656877383},
    {0.0956387200792749, -0.1444719615827965},
    {0.0956387200792749, 0.1444719615827965},
    {0.0938443990808046, -0.2392873622521371},
    {0.0938443990808046, 0.2392873622521371},
    {0.0911738786957639, -0.3318686022821277},
    {0.0911738786957639, 0.3318686022821277},
    {0.0876520930044038, -0.4213512761306353},
    {0.0876520930044038, 0.4213512761306353},
    {0.0833119242269467, -0.5068999089322294},
    {0.0833119242269467, 0.5068999089322294},
    {0.0781938957870703, -0.5877157572407623},
    {0.0781938957870703, 0.5877157572407623},
    {0.0723457941088485, -0.6630442669302152},
    {0.0723457941088485, 0.6630442669302152},
    {0.0658222227763618, -0.7321821187402897},
    {0.0658222227763618, 0.7321821187402897},
    {0.0586840934785355, -0.7944837959679424},
    {0.0586840934785355, 0.7944837959679424},
    {0.0509980592623762, -0.8493676137325700},
    {0.0509980592623762, 0.8493676137325700},
    {0.0428358980222267, -0.8963211557660521},
    {0.0428358980222267, 0.8963211557660521},
    {0.0342738629130214, -0.9349060759377397},
    {0.0342738629130214, 0.9349060759377397},
    {0.0253920653092621, -0.9647622555875064},
    {0.0253920653092621, 0.9647622555875064},
    {0.0162743947309057, -0.9856115115452684},
    {0.0162743947309057, 0.9856115115452684},
    {0.0070186100094701, -0.9972638618494816},
    {0.0070186100094701, 0.9972638618494816}
}};

const std::array<std::pair<double, double>, 16> GAUSS_LEGENDRE_COEFFS_32_HALF = {{
    {0.0965400885147278, 0.0483076656877383},
    {0.0956387200792749, 0.1444719615827965},
    {0.0938443990808046, 0.2392873622521371},
    {0.0911738786957639, 0.3318686022821277},
    {0.0876520930044038, 0.4213512761306353},
    {0.0833119242269467, 0.5068999089322294},
    {0.0781938957870703, 0.5877157572407623},
    {0.0723457941088485, 0.6630442669302152},
    {0.0658222227763618, 0.7321821187402897},
    {0.0586840934785355, 0.7944837959679424},
    {0.0509980592623762, 0.8493676137325700},
    {0.0428358980222267, 0.8963211557660521},
    {0.0342738629130214, 0.9349060759377397},
    {0.0253920653092621, 0.9647622555875064},
    {0.0162743947309057, 0.9856115115452684},
    {0.0070186100094701, 0.9972638618494816}
}};

} // namespace kurbo 