#include "kurbo/bezpath.hpp"
#include <cassert>

namespace kurbo {

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

bool BezPath::is_empty() const {
    for (const auto& el : elements_) {
        if (el.type != PathElType::MoveTo && el.type != PathElType::ClosePath) {
            return false;
        }
    }
    return true;
}

void BezPath::apply_affine(const Affine& affine) {
    for (auto& el : elements_) {
        el = affine * el;
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

PathSeg BezPath::Segments::Iter::operator*() const {
    assert(elements && index < elements->size());
    if (index == 0 || index >= elements->size()) throw std::out_of_range("No segment");
    const auto& prev = (*elements)[index - 1];
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

BezPath::Segments::Iter BezPath::Segments::begin() const { return Iter(elements); }
BezPath::Segments::Iter BezPath::Segments::end() const { Iter it(elements); it.index = elements.size(); return it; }

BezPath::Segments BezPath::segments() const { return Segments(elements_); }

} // namespace kurbo 