#include "kurbo/bezpath.hpp"
#include <cassert>
#include <algorithm>
#include <functional>

namespace kurbo {

// Helper function to count ray intersections for winding number calculation
int ray_intersection_count(const Point& p1, const Point& p2, const Point& pt) {
    // Cast a ray from pt to the right and check if it intersects the line segment p1-p2
    
    // Check if the line segment is horizontal
    if (std::abs(p1.y - p2.y) < 1e-12) {
        return 0; // Horizontal lines don't contribute to winding
    }
    
    // Check if the ray intersects the line segment
    if ((p1.y > pt.y && p2.y <= pt.y) || (p2.y > pt.y && p1.y <= pt.y)) {
        // The ray might intersect this line segment
        double t = (pt.y - p1.y) / (p2.y - p1.y);
        double x_intersect = p1.x + t * (p2.x - p1.x);
        
        if (x_intersect > pt.x) {
            // Ray intersects the line segment to the right of pt
            return (p2.y > p1.y) ? 1 : -1;
        }
    }
    
    return 0;
}

// --- PathSeg methods ---

PathEl PathSeg::as_path_el() const {
    switch (kind) {
        case Kind::Line: {
            const Line& line = as_line();
            return PathEl(PathElType::LineTo, line.p1);
        }
        case Kind::Quad: {
            const QuadBez& quad = as_quad();
            return PathEl(PathElType::QuadTo, quad.p1, quad.p2);
        }
        case Kind::Cubic: {
            const CubicBez& cubic = as_cubic();
            return PathEl(PathElType::CurveTo, cubic.p1, cubic.p2, cubic.p3);
        }
    }
    return PathEl(PathElType::ClosePath); // Should not reach
}

PathSeg PathSeg::reverse() const {
    switch (kind) {
        case Kind::Line: {
            const Line& line = as_line();
            return PathSeg(Line(line.p1, line.p0));
        }
        case Kind::Quad: {
            const QuadBez& quad = as_quad();
            return PathSeg(QuadBez(quad.p2, quad.p1, quad.p0));
        }
        case Kind::Cubic: {
            const CubicBez& cubic = as_cubic();
            return PathSeg(CubicBez(cubic.p3, cubic.p2, cubic.p1, cubic.p0));
        }
    }
    return *this; // Should not reach
}

CubicBez PathSeg::to_cubic() const {
    switch (kind) {
        case Kind::Line: {
            const Line& line = as_line();
            // Convert line to cubic (degenerate)
            Point p1 = line.p0 + (line.p1 - line.p0) / 3.0;
            Point p2 = line.p0 + 2.0 * (line.p1 - line.p0) / 3.0;
            return CubicBez(line.p0, p1, p2, line.p1);
        }
        case Kind::Quad: {
            const QuadBez& quad = as_quad();
            // Convert quad to cubic
            Point p1 = quad.p0 + 2.0 * (quad.p1 - quad.p0) / 3.0;
            Point p2 = quad.p1 + (quad.p2 - quad.p1) / 3.0;
            return CubicBez(quad.p0, p1, p2, quad.p2);
        }
        case Kind::Cubic:
            return as_cubic();
    }
    return CubicBez(); // Should not reach
}

std::vector<LineIntersection> PathSeg::intersect_line(const Line& line) const {
    std::vector<LineIntersection> intersections;
    // Simplified implementation - just return empty for now
    // TODO: Implement proper intersection logic
    return intersections;
}

MinDistance PathSeg::min_dist(const PathSeg& other, double accuracy) const {
    // Simplified implementation
    // TODO: Implement proper distance calculation
    return MinDistance(0.0, 0.0, 0.0);
}

bool PathSeg::is_finite() const {
    switch (kind) {
        case Kind::Line:
            return as_line().is_finite();
        case Kind::Quad:
            return as_quad().is_finite();
        case Kind::Cubic:
            return as_cubic().is_finite();
    }
    return false;
}

bool PathSeg::is_nan() const {
    switch (kind) {
        case Kind::Line:
            return as_line().is_nan();
        case Kind::Quad:
            return as_quad().is_nan();
        case Kind::Cubic:
            return as_cubic().is_nan();
    }
    return false;
}

// --- BezPath methods ---

BezPath BezPath::with_capacity(size_t capacity) {
    BezPath path;
    path.elements_.reserve(capacity);
    return path;
}

BezPath BezPath::from_vec(const std::vector<PathEl>& v) {
    assert(v.empty() || v.front().type == PathElType::MoveTo);
    return BezPath(v);
}

void BezPath::push(const PathEl& el) {
    elements_.push_back(el);
    assert(elements_.empty() || elements_.front().type == PathElType::MoveTo);
}

void BezPath::move_to(const Point& p) {
    push(PathEl(PathElType::MoveTo, p));
}

void BezPath::line_to(const Point& p) {
    assert(!elements_.empty());
    push(PathEl(PathElType::LineTo, p));
}

void BezPath::quad_to(const Point& p1, const Point& p2) {
    assert(!elements_.empty());
    push(PathEl(PathElType::QuadTo, p1, p2));
}

void BezPath::curve_to(const Point& p1, const Point& p2, const Point& p3) {
    assert(!elements_.empty());
    push(PathEl(PathElType::CurveTo, p1, p2, p3));
}

void BezPath::close_path() {
    assert(!elements_.empty());
    push(PathEl(PathElType::ClosePath));
}

std::optional<PathEl> BezPath::pop() {
    if (elements_.empty()) return std::nullopt;
    PathEl el = elements_.back();
    elements_.pop_back();
    return el;
}

void BezPath::truncate(size_t len) {
    if (len < elements_.size()) {
        elements_.resize(len);
    }
}

std::optional<PathSeg> BezPath::get_seg(size_t ix) const {
    if (ix >= elements_.size()) return std::nullopt;
    
    const auto& el = elements_[ix];
    Point last_pt;
    
    // Находим последнюю точку из предыдущих элементов
    for (size_t i = 0; i < ix; ++i) {
        switch (elements_[i].type) {
            case PathElType::MoveTo:
                last_pt = elements_[i].point;
                break;
            case PathElType::LineTo:
                last_pt = elements_[i].point;
                break;
            case PathElType::QuadTo:
                last_pt = elements_[i].point2;
                break;
            case PathElType::CurveTo:
                last_pt = elements_[i].point3;
                break;
            case PathElType::ClosePath:
                // Для ClosePath нужно найти начальную точку подпути
                for (int j = i - 1; j >= 0; --j) {
                    if (elements_[j].type == PathElType::MoveTo) {
                        last_pt = elements_[j].point;
                        break;
                    }
                }
                break;
        }
    }
    
    switch (el.type) {
        case PathElType::LineTo:
            return PathSeg(Line(last_pt, el.point));
        case PathElType::QuadTo:
            return PathSeg(QuadBez(last_pt, el.point, el.point2));
        case PathElType::CurveTo:
            return PathSeg(CubicBez(last_pt, el.point, el.point2, el.point3));
        case PathElType::ClosePath: {
            // Для ClosePath нужно найти начальную точку подпути
            Point start_pt = last_pt;
            for (int j = ix - 1; j >= 0; --j) {
                if (elements_[j].type == PathElType::MoveTo) {
                    start_pt = elements_[j].point;
                    break;
                }
            }
            return PathSeg(Line(last_pt, start_pt));
        }
        case PathElType::MoveTo:
        default:
            return std::nullopt;
    }
}

bool BezPath::is_empty() const {
    for (const auto& el : elements_) {
        if (el.type != PathElType::MoveTo && el.type != PathElType::ClosePath) {
            return false;
        }
    }
    return true;
}

bool BezPath::is_finite() const {
    for (const auto& el : elements_) {
        if (!el.is_finite()) return false;
    }
    return true;
}

bool BezPath::is_nan() const {
    for (const auto& el : elements_) {
        if (el.is_nan()) return true;
    }
    return false;
}

void BezPath::apply_affine(const Affine& affine) {
    for (auto& el : elements_) {
        el = affine * el;
    }
}

Rect BezPath::control_box() const {
    Rect cbox;
    bool first = true;
    
    for (const auto& el : elements_) {
        switch (el.type) {
            case PathElType::MoveTo:
            case PathElType::LineTo:
                if (first) {
                    cbox = Rect::from_points(el.point, el.point);
                    first = false;
                } else {
                    cbox = cbox.union_pt(el.point);
                }
                break;
            case PathElType::QuadTo:
                if (first) {
                    cbox = Rect::from_points(el.point, el.point);
                    first = false;
                } else {
                    cbox = cbox.union_pt(el.point);
                }
                cbox = cbox.union_pt(el.point2);
                break;
            case PathElType::CurveTo:
                if (first) {
                    cbox = Rect::from_points(el.point, el.point);
                    first = false;
                } else {
                    cbox = cbox.union_pt(el.point);
                }
                cbox = cbox.union_pt(el.point2);
                cbox = cbox.union_pt(el.point3);
                break;
            case PathElType::ClosePath:
                break;
        }
    }
    
    return cbox;
}

BezPath BezPath::reverse_subpaths() const {
    BezPath result;
    std::vector<PathEl> current_subpath;
    Point start_pt;
    
    for (const auto& el : elements_) {
        switch (el.type) {
            case PathElType::MoveTo:
                if (!current_subpath.empty()) {
                    // Process previous subpath
                    reverse_subpath(start_pt, current_subpath, result);
                    current_subpath.clear();
                }
                start_pt = el.point;
                current_subpath.push_back(el);
                break;
            case PathElType::LineTo:
            case PathElType::QuadTo:
            case PathElType::CurveTo:
            case PathElType::ClosePath:
                current_subpath.push_back(el);
                break;
        }
    }
    
    // Process last subpath
    if (!current_subpath.empty()) {
        reverse_subpath(start_pt, current_subpath, result);
    }
    
    return result;
}

// Helper function for reverse_subpaths
void reverse_subpath(Point start_pt, const std::vector<PathEl>& els, BezPath& reversed) {
    if (els.empty()) return;
    
    // Find end point
    Point end_pt = start_pt;
    for (const auto& el : els) {
        switch (el.type) {
            case PathElType::MoveTo:
            case PathElType::LineTo:
                end_pt = el.point;
                break;
            case PathElType::QuadTo:
                end_pt = el.point2;
                break;
            case PathElType::CurveTo:
                end_pt = el.point3;
                break;
            case PathElType::ClosePath:
                // Find the most recent MoveTo
                for (auto it = els.rbegin(); it != els.rend(); ++it) {
                    if (it->type == PathElType::MoveTo) {
                        end_pt = it->point;
                        break;
                    }
                }
                break;
        }
    }
    
    reversed.move_to(end_pt);
    
    // Reverse the elements
    for (auto it = els.rbegin(); it != els.rend(); ++it) {
        switch (it->type) {
            case PathElType::LineTo:
                reversed.line_to(it->point);
                break;
            case PathElType::QuadTo:
                reversed.quad_to(it->point, it->point2);
                break;
            case PathElType::CurveTo:
                reversed.curve_to(it->point2, it->point, it->point3);
                break;
            case PathElType::ClosePath:
                reversed.close_path();
                break;
            case PathElType::MoveTo:
                // Skip MoveTo in reverse
                break;
        }
    }
}

// --- Segments implementation ---

BezPath::Segments::Segments(const std::vector<PathEl>& els) : elements(els) {}
BezPath::Segments::~Segments() = default;

BezPath::Segments::Iter::Iter(const std::vector<PathEl>& els)
    : elements(&els), index(0), last_point(), start_point() { advance_to_next(); }
BezPath::Segments::Iter::~Iter() = default;

bool BezPath::Segments::Iter::operator!=(const Iter& other) const {
    return index != other.index || elements != other.elements;
}

bool BezPath::Segments::Iter::operator==(const Iter& other) const {
    return index == other.index && elements == other.elements;
}

PathSeg BezPath::Segments::Iter::operator*() const {
    assert(elements && index < elements->size());
    if (index == 0 || index >= elements->size()) {
        throw std::out_of_range("No segment");
    }
    
    const auto& curr = (*elements)[index];
    Point last = last_point;
    
    switch (curr.type) {
        case PathElType::LineTo:
            return PathSeg(Line(last, curr.point));
        case PathElType::QuadTo:
            return PathSeg(QuadBez(last, curr.point, curr.point2));
        case PathElType::CurveTo:
            return PathSeg(CubicBez(last, curr.point, curr.point2, curr.point3));
        case PathElType::ClosePath: {
            Point start = start_point;
            return PathSeg(Line(last, start));
        }
        default:
            throw std::logic_error("Invalid PathEl for segment");
    }
}

BezPath::Segments::Iter& BezPath::Segments::Iter::operator++() {
    ++index;
    advance_to_next();
    return *this;
}

void BezPath::Segments::Iter::advance_to_next() {
    if (!elements || index >= elements->size()) return;
    
    // Пропускаем MoveTo элементы
    while (index < elements->size() && (*elements)[index].type == PathElType::MoveTo) {
        const auto& el = (*elements)[index];
        last_point = el.point;
        start_point = el.point;
        ++index;
    }
    
    if (index >= elements->size()) return;
    
    const auto& el = (*elements)[index];
    switch (el.type) {
        case PathElType::MoveTo:
            last_point = el.point;
            start_point = el.point;
            break;
        case PathElType::LineTo:
            last_point = el.point;
            break;
        case PathElType::QuadTo:
            last_point = el.point2;
            break;
        case PathElType::CurveTo:
            last_point = el.point3;
            break;
        case PathElType::ClosePath:
            last_point = start_point;
            break;
    }
}

BezPath::Segments::Iter BezPath::Segments::begin() const { 
    return Iter(elements); 
}

BezPath::Segments::Iter BezPath::Segments::end() const { 
    Iter it(elements); 
    it.index = elements.size(); 
    return it; 
}

BezPath::Segments BezPath::segments() const { return Segments(elements_); }

// --- Free functions ---

std::vector<PathEl> flatten(const std::vector<PathEl>& path, double tolerance) {
    std::vector<PathEl> result;
    // Simplified implementation - just return the original path
    // TODO: Implement proper flattening
    result = path;
    return result;
}

// Вспомогательная функция для аппроксимации квадратичной кривой
static void flatten_quad_bezier(const QuadBez& quad, double tolerance, 
                               std::vector<PathEl>& out, Point& last_pt) {
    constexpr int MAX_DEPTH = 16;
    std::function<void(const QuadBez&, int)> recur;
    recur = [&](const QuadBez& q, int depth) {
        // Проверяем, насколько кривая близка к прямой
        double d = (q.p1.to_vec2() - (q.p0.to_vec2() + q.p2.to_vec2()) * 0.5).hypot();
        if (d < tolerance || depth > MAX_DEPTH) {
            out.emplace_back(PathElType::LineTo, q.p2);
            last_pt = q.p2;
            return;
        }
        // Подразделяем кривую и рекурсивно обрабатываем
        auto subdiv = q.subdivide();
        // Приводим ParamCurve* к QuadBez*
        auto* first = dynamic_cast<QuadBez*>(subdiv.first);
        auto* second = dynamic_cast<QuadBez*>(subdiv.second);
        if (first && second) {
            recur(*first, depth + 1);
            recur(*second, depth + 1);
        }
    };
    recur(quad, 0);
}

// Вспомогательная функция для аппроксимации кубической кривой
static void flatten_cubic_bezier(const CubicBez& cubic, double tolerance, 
                                std::vector<PathEl>& out, Point& last_pt) {
    constexpr int MAX_DEPTH = 16;
    std::function<void(const CubicBez&, int)> recur;
    recur = [&](const CubicBez& c, int depth) {
        // Проверяем, насколько кривая близка к прямой
        double d1 = (c.p1.to_vec2() - (c.p0.to_vec2() + c.p3.to_vec2()) * 0.5).hypot();
        double d2 = (c.p2.to_vec2() - (c.p0.to_vec2() + c.p3.to_vec2()) * 0.5).hypot();
        if ((d1 < tolerance && d2 < tolerance) || depth > MAX_DEPTH) {
            out.emplace_back(PathElType::LineTo, c.p3);
            last_pt = c.p3;
            return;
        }
        // Подразделяем кривую и рекурсивно обрабатываем
        auto subdiv = c.subdivide_concrete();
        recur(subdiv.first, depth + 1);
        recur(subdiv.second, depth + 1);
    };
    recur(cubic, 0);
}

void BezPath::flatten(double tolerance, std::vector<PathEl>& out) const {
    Point last_pt;
    Point start_pt;
    bool have_subpath = false;
    
    for (const auto& el : elements_) {
        switch (el.type) {
            case PathElType::MoveTo:
                out.emplace_back(el);
                last_pt = el.point;
                start_pt = el.point;
                have_subpath = true;
                break;
            case PathElType::LineTo:
                out.emplace_back(el);
                last_pt = el.point;
                break;
            case PathElType::QuadTo: {
                if (!have_subpath) break;
                // Аппроксимируем квадратичную кривую линиями
                QuadBez quad(last_pt, el.point, el.point2);
                flatten_quad_bezier(quad, tolerance, out, last_pt);
                break;
            }
            case PathElType::CurveTo: {
                if (!have_subpath) break;
                // Аппроксимируем кубическую кривую линиями
                CubicBez cubic(last_pt, el.point, el.point2, el.point3);
                flatten_cubic_bezier(cubic, tolerance, out, last_pt);
                break;
            }
            case PathElType::ClosePath:
                if (have_subpath && last_pt != start_pt) {
                    out.emplace_back(PathElType::LineTo, start_pt);
                }
                out.emplace_back(el);
                have_subpath = false;
                break;
        }
    }
}

double BezPath::area() const {
    double sum = 0.0;
    Point last_pt;
    Point start_pt;
    bool have_subpath = false;
    
    for (const auto& el : elements_) {
        switch (el.type) {
            case PathElType::MoveTo:
                last_pt = el.point;
                start_pt = el.point;
                have_subpath = true;
                break;
            case PathElType::LineTo: {
                if (have_subpath) {
                    Line line(last_pt, el.point);
                    sum += line.signed_area();
                }
                last_pt = el.point;
                break;
            }
            case PathElType::ClosePath: {
                if (have_subpath && last_pt != start_pt) {
                    Line line(last_pt, start_pt);
                    sum += line.signed_area();
                }
                have_subpath = false;
                break;
            }
            default:
                // TODO: Handle curves properly
                break;
        }
    }
    return sum;
}

double BezPath::perimeter(double accuracy) const {
    double sum = 0.0;
    Point last_pt;
    Point start_pt;
    bool have_subpath = false;
    
    for (const auto& el : elements_) {
        switch (el.type) {
            case PathElType::MoveTo:
                last_pt = el.point;
                start_pt = el.point;
                have_subpath = true;
                break;
            case PathElType::LineTo: {
                if (have_subpath) {
                    Line line(last_pt, el.point);
                    sum += line.arclen(accuracy);
                }
                last_pt = el.point;
                break;
            }
            case PathElType::ClosePath: {
                if (have_subpath && last_pt != start_pt) {
                    Line line(last_pt, start_pt);
                    sum += line.arclen(accuracy);
                }
                have_subpath = false;
                break;
            }
            default:
                // TODO: Handle curves properly
                break;
        }
    }
    return sum;
}

int BezPath::winding(const Point& pt) const {
    // Simple winding algorithm for closed paths
    // Cast a ray to the right and count intersections
    
    int winding = 0;
    Point last_pt;
    Point start_pt;
    bool have_subpath = false;
    
    for (const auto& el : elements_) {
        switch (el.type) {
            case PathElType::MoveTo:
                last_pt = el.point;
                start_pt = el.point;
                have_subpath = true;
                break;
            case PathElType::LineTo: {
                if (have_subpath) {
                    // Check if ray from pt to right intersects this line segment
                    winding += ray_intersection_count(last_pt, el.point, pt);
                }
                last_pt = el.point;
                break;
            }
            case PathElType::ClosePath: {
                if (have_subpath && last_pt != start_pt) {
                    // Check if ray from pt to right intersects this line segment
                    winding += ray_intersection_count(last_pt, start_pt, pt);
                }
                have_subpath = false;
                break;
            }
            default:
                // TODO: Handle curves properly
                break;
        }
    }
    return winding;
}

Rect BezPath::bounding_box() const {
    if (elements_.empty()) {
        return Rect::invalid(); // Возвращаем невалидный прямоугольник для пустого пути
    }
    
    Rect bbox;
    bool first = true;
    Point last_pt;
    Point start_pt;
    bool have_subpath = false;
    
    for (const auto& el : elements_) {
        switch (el.type) {
            case PathElType::MoveTo:
                last_pt = el.point;
                start_pt = el.point;
                have_subpath = true;
                if (first) {
                    bbox = Rect(el.point.x, el.point.y, el.point.x, el.point.y);
                    first = false;
                } else {
                    bbox = bbox.union_rect(Rect(el.point.x, el.point.y, el.point.x, el.point.y));
                }
                break;
            case PathElType::LineTo: {
                if (have_subpath) {
                    Line line(last_pt, el.point);
                    Rect line_box = line.bounding_box();
                    if (first) {
                        bbox = line_box;
                        first = false;
                    } else {
                        bbox = bbox.union_rect(line_box);
                    }
                }
                last_pt = el.point;
                break;
            }
            case PathElType::ClosePath: {
                if (have_subpath && last_pt != start_pt) {
                    Line line(last_pt, start_pt);
                    Rect line_box = line.bounding_box();
                    if (first) {
                        bbox = line_box;
                        first = false;
                    } else {
                        bbox = bbox.union_rect(line_box);
                    }
                }
                have_subpath = false;
                break;
            }
            default:
                // TODO: Handle curves properly
                break;
        }
    }
    return bbox;
}

// --- Operators ---

BezPath operator*(const Affine& affine, const BezPath& path) {
    BezPath result = path;
    result.apply_affine(affine);
    return result;
}

PathSeg operator*(const Affine& affine, const PathSeg& seg) {
    switch (seg.type()) {
        case PathSeg::Kind::Line:
            return PathSeg(affine * seg.as_line());
        case PathSeg::Kind::Quad:
            return PathSeg(affine * seg.as_quad());
        case PathSeg::Kind::Cubic:
            return PathSeg(affine * seg.as_cubic());
    }
    return seg; // Should not reach
}

bool operator==(const PathEl& a, const PathEl& b) {
    if (a.type != b.type) return false;
    switch (a.type) {
        case PathElType::MoveTo:
        case PathElType::LineTo:
            return a.point == b.point;
        case PathElType::QuadTo:
            return a.point == b.point && a.point2 == b.point2;
        case PathElType::CurveTo:
            return a.point == b.point && a.point2 == b.point2 && a.point3 == b.point3;
        case PathElType::ClosePath:
            return true;
    }
    return false;
}

bool operator!=(const PathEl& a, const PathEl& b) {
    return !(a == b);
}

bool operator==(const BezPath& a, const BezPath& b) {
    return a.elements() == b.elements();
}

bool operator!=(const BezPath& a, const BezPath& b) {
    return !(a == b);
}

// Shape implementation
std::vector<PathEl> BezPath::path_elements(double tolerance) const {
    return elements_;
}

std::optional<std::vector<PathEl>> BezPath::as_path_slice() const {
    return elements_;
}

} // namespace kurbo 