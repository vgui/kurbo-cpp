#pragma once

// QuadSpline class (quadratic Bézier spline in B-spline format)
// Ported from kurbo::QuadSpline (Rust)
// Copyright 2024
//
// This class represents a quadratic Bézier spline as a sequence of control points.

#include "point.hpp"
#include "quadbez.hpp"
#include <vector>
#include <iterator>

namespace kurbo {

class QuadSpline {
public:
    // Constructors
    QuadSpline();
    explicit QuadSpline(const std::vector<Point>& points);
    explicit QuadSpline(std::vector<Point>&& points);

    // Static constructor
    static QuadSpline new_spline(const std::vector<Point>& points);

    // Access to control points
    const std::vector<Point>& points() const;

    // Iterator over implied QuadBez sequence
    class QuadIterator {
    public:
        QuadIterator(const std::vector<Point>& points, size_t idx);
        QuadBez operator*() const;
        QuadIterator& operator++();
        bool operator!=(const QuadIterator& other) const;
        bool operator==(const QuadIterator& other) const;
    private:
        const std::vector<Point>& points_;
        size_t idx_;
    };

    QuadIterator begin() const;
    QuadIterator end() const;

    // Utility
    size_t size() const;
    bool empty() const;

private:
    std::vector<Point> points_;
};

} // namespace kurbo 