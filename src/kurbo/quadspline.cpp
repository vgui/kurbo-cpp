#include "kurbo/quadspline.hpp"
#include <cassert>

namespace kurbo {

QuadSpline::QuadSpline() : points_() {}
QuadSpline::QuadSpline(const std::vector<Point>& points) : points_(points) {}
QuadSpline::QuadSpline(std::vector<Point>&& points) : points_(std::move(points)) {}

QuadSpline QuadSpline::new_spline(const std::vector<Point>& points) {
    return QuadSpline(points);
}

const std::vector<Point>& QuadSpline::points() const {
    return points_;
}

size_t QuadSpline::size() const {
    return points_.size();
}

bool QuadSpline::empty() const {
    return points_.empty();
}

// Iterator implementation
QuadSpline::QuadIterator::QuadIterator(const std::vector<Point>& points, size_t idx)
    : points_(points), idx_(idx) {}

QuadBez QuadSpline::QuadIterator::operator*() const {
    // Rust: let [mut p0, p1, mut p2]: [Point; 3] = self.points.get(self.idx..=self.idx + 2)?.try_into().ok()?;
    assert(idx_ + 2 < points_.size());
    Point p0 = points_[idx_];
    Point p1 = points_[idx_ + 1];
    Point p2 = points_[idx_ + 2];
    if (idx_ != 0) {
        p0 = p0.midpoint(p1);
    }
    if (idx_ + 2 < points_.size() - 1) {
        p2 = p1.midpoint(p2);
    }
    return QuadBez(p0, p1, p2);
}

QuadSpline::QuadIterator& QuadSpline::QuadIterator::operator++() {
    ++idx_;
    return *this;
}

bool QuadSpline::QuadIterator::operator!=(const QuadIterator& other) const {
    return idx_ != other.idx_ || &points_ != &other.points_;
}

bool QuadSpline::QuadIterator::operator==(const QuadIterator& other) const {
    return idx_ == other.idx_ && &points_ == &other.points_;
}

QuadSpline::QuadIterator QuadSpline::begin() const {
    if (points_.size() < 3) {
        return QuadIterator(points_, points_.size()); // end
    }
    return QuadIterator(points_, 0);
}

QuadSpline::QuadIterator QuadSpline::end() const {
    if (points_.size() < 3) {
        return QuadIterator(points_, points_.size());
    }
    return QuadIterator(points_, points_.size() - 2);
}

} // namespace kurbo 