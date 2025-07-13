#include <iostream>
#include <vector>
#include <cmath>
#include "kurbo/common.hpp"

int main() {
    using namespace kurbo;
    std::cout << "Example: solve_cubic for x^3 - x - 2 = 0\n";
    auto roots = solve_cubic(-2.0, -1.0, 0.0, 1.0);
    for (double r : roots) {
        std::cout << "  root: " << r << "\n";
    }

    std::cout << "\nExample: solve_quadratic for x^2 - 5 = 0\n";
    auto qroots = solve_quadratic(-5.0, 0.0, 1.0);
    for (double r : qroots) {
        std::cout << "  root: " << r << "\n";
    }

    std::cout << "\nExample: solve_itp for x^3 - x - 2 = 0 on [1,2]\n";
    auto f = [](double x) { return x * x * x - x - 2.0; };
    double x = solve_itp(f, 1.0, 2.0, 1e-12, 0, 0.2, f(1.0), f(2.0));
    std::cout << "  root: " << x << ", f(x) = " << f(x) << "\n";

    std::cout << "\nExample: expand function\n";
    std::cout << "  expand(3.7) = " << detail::expand(3.7) << "\n";
    std::cout << "  expand(-3.7) = " << detail::expand(-3.7) << "\n";
    std::cout << "  expand(3.0) = " << detail::expand(3.0) << "\n";
    std::cout << "  expand(-5.1f) = " << detail::expand(-5.1f) << "\n";

    return 0;
} 