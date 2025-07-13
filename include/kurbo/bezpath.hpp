#pragma once

// BezPath: Bezier path (up to cubic)
// Ported from kurbo::BezPath (Rust)
// Copyright 2024
//
// This class represents a sequence of Bezier path elements (PathEl),
// supporting lines, quadratics, cubics, and subpaths.

#include "path_el.hpp"
#include "line.hpp"
#include "quadbez.hpp"
#include "cubicbez.hpp"
#include "affine.hpp"
#include <vector>
#include <variant>
#include <iterator>
#include <cassert>

namespace kurbo {

// PathSeg: a segment of a Bezier path (Line, QuadBez, CubicBez)
class PathSeg {
public:
    enum class Kind { Line, Quad, Cubic };
    Kind kind;
    std::variant<Line, QuadBez, CubicBez> seg;

    PathSeg(const Line& l) : kind(Kind::Line), seg(l) {}
    PathSeg(const QuadBez& q) : kind(Kind::Quad), seg(q) {}
    PathSeg(const CubicBez& c) : kind(Kind::Cubic), seg(c) {}

    Kind type() const { return kind; }
    const Line& as_line() const { assert(kind == Kind::Line); return std::get<Line>(seg); }
    const QuadBez& as_quad() const { assert(kind == Kind::Quad); return std::get<QuadBez>(seg); }
    const CubicBez& as_cubic() const { assert(kind == Kind::Cubic); return std::get<CubicBez>(seg); }
};

// BezPath: a sequence of PathEl
class BezPath {
public:
    BezPath() = default;
    explicit BezPath(const std::vector<PathEl>& els) : elements_(els) {}

    void push(const PathEl& el);
    void move_to(const Point& p);
    void line_to(const Point& p);
    void quad_to(const Point& p1, const Point& p2);
    void curve_to(const Point& p1, const Point& p2, const Point& p3);
    void close_path();

    const std::vector<PathEl>& elements() const { return elements_; }
    std::vector<PathEl>& elements_mut() { return elements_; }
    bool is_empty() const;
    void apply_affine(const Affine& affine);

    // Segments iterator
    class Segments {
    public:
        using iterator_category = std::input_iterator_tag;
        using value_type = PathSeg;
        using difference_type = std::ptrdiff_t;
        using pointer = const PathSeg*;
        using reference = const PathSeg&;

        explicit Segments(const std::vector<PathEl>& els);
        ~Segments();

        class Iter {
        public:
            explicit Iter(const std::vector<PathEl>& els);
            ~Iter();
            bool operator!=(const Iter& other) const;
            PathSeg operator*() const;
            Iter& operator++();
            size_t index; // Make public for end()
        private:
            const std::vector<PathEl>* elements;
            mutable Point last_point;
            mutable Point start_point;
            void advance_to_next();
        };

        Iter begin() const;
        Iter end() const;
    private:
        const std::vector<PathEl>& elements;
    };

    Segments segments() const;

private:
    std::vector<PathEl> elements_;
};

} // namespace kurbo 