#include "kurbo/param_curve.hpp"
#include "kurbo/common.hpp"
#include "kurbo/inline_methods.hpp"
#include <cmath>
#include <algorithm>

namespace kurbo {

double ParamCurveDeriv::gauss_arclen(const std::vector<std::pair<double, double>>& coeffs) const {
    auto d = deriv();
    double sum = 0.0;
    for (const auto& [wi, xi] : coeffs) {
        Point eval_point = d->eval(0.5 * (xi + 1.0));
        Vec2 vec = eval_point.to_vec2();
        sum += wi * vec.hypot();
    }
    return sum * 0.5;
}

double ParamCurveArclen::inv_arclen(double arclen, double accuracy) const {
    if (arclen <= 0.0) {
        return 0.0;
    }
    double total_arclen = this->arclen(accuracy);
    if (arclen >= total_arclen) {
        return 1.0;
    }
    
    double t_last = 0.0;
    double arclen_last = 0.0;
    double epsilon = accuracy / total_arclen;
    double n = 1.0 - std::ceil(std::log2(epsilon));
    if (n < 0.0) n = 0.0;
    double inner_accuracy = accuracy / n;
    
    auto f = [this, &t_last, &arclen_last, inner_accuracy, arclen](double t) -> double {
        double dir;
        double start, end;
        if (t > t_last) {
            start = t_last;
            end = t;
            dir = 1.0;
        } else {
            start = t;
            end = t_last;
            dir = -1.0;
        }
        
        auto segment = this->subsegment(start, end);
        double arc = static_cast<ParamCurveArclen*>(segment)->arclen(inner_accuracy);
        delete segment;
        
        arclen_last += arc * dir;
        t_last = t;
        return arclen_last - arclen;
    };
    
    return solve_itp(f, 0.0, 1.0, epsilon, 1, 0.2, -arclen, total_arclen - arclen);
}

double ParamCurveCurvature::curvature(double t) const {
    auto deriv = this->deriv();
    auto deriv2 = deriv->deriv();
    Vec2 d = deriv->eval(t).to_vec2();
    Vec2 d2 = deriv2->eval(t).to_vec2();
    
    delete deriv;
    delete deriv2;
    
    // TODO: What's the convention for sign? I think it should match signed
    // area - a positive area curve should have positive curvature.
    return d2.cross(d) * std::pow(d.hypot2(), -1.5);
}

std::vector<std::pair<double, double>> ParamCurveExtrema::extrema_ranges() const {
    std::vector<std::pair<double, double>> result;
    auto extrema = this->extrema();
    
    double t0 = 0.0;
    for (double t : extrema) {
        result.emplace_back(t0, t);
        t0 = t;
    }
    result.emplace_back(t0, 1.0);
    
    return result;
}

Rect ParamCurveExtrema::bounding_box() const {
    Rect bbox = Rect::from_points(start(), end());
    auto extrema = this->extrema();
    
    for (double t : extrema) {
        bbox = bbox.union_pt(eval(t));
    }
    
    return bbox;
}

} // namespace kurbo 