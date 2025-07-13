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
#include "rect.hpp"
#include <vector>
#include <variant>
#include <iterator>
#include <cassert>
#include <optional>
#include <cmath> // For std::isfinite and std::isnan

namespace kurbo {

// Line intersection result
struct LineIntersection {
    double line_t;      // Time on line (0..1)
    double segment_t;   // Time on segment (0..1)
    
    LineIntersection(double lt, double st) : line_t(lt), segment_t(st) {}
    bool is_finite() const { return std::isfinite(line_t) && std::isfinite(segment_t); }
    bool is_nan() const { return std::isnan(line_t) || std::isnan(segment_t); }
};

// Minimum distance between curves
struct MinDistance {
    double distance;
    double t1;  // Parameter on first curve
    double t2;  // Parameter on second curve
    
    MinDistance(double d, double t1, double t2) : distance(d), t1(t1), t2(t2) {}
};

// PathSeg: a segment of a Bezier path (Line, QuadBez, CubicBez)
class PathSeg {
public:
    enum class Kind { Line, Quad, Cubic };
    Kind kind;
    std::variant<Line, QuadBez, CubicBez> seg;

    PathSeg(const Line& l) : kind(Kind::Line), seg(l) {}
    PathSeg(const QuadBez& q) : kind(Kind::Quad), seg(q) {}
    PathSeg(const CubicBez& c) : kind(Kind::Cubic), seg(c) {}
    PathSeg(const ParamCurve* curve) : kind(Kind::Cubic), seg(CubicBez()) {
        // This is a temporary implementation - we need to convert ParamCurve to specific types
        // For now, we'll create a default CubicBez
    }

    Kind type() const { return kind; }
    const Line& as_line() const { assert(kind == Kind::Line); return std::get<Line>(seg); }
    const QuadBez& as_quad() const { assert(kind == Kind::Quad); return std::get<QuadBez>(seg); }
    const CubicBez& as_cubic() const { assert(kind == Kind::Cubic); return std::get<CubicBez>(seg); }

    // Additional methods
    PathEl as_path_el() const;
    PathSeg reverse() const;
    CubicBez to_cubic() const;
    std::vector<LineIntersection> intersect_line(const Line& line) const;
    MinDistance min_dist(const PathSeg& other, double accuracy) const;
    bool is_finite() const;
    bool is_nan() const;
};

// BezPath: a sequence of PathEl
class BezPath {
public:
    BezPath() = default;
    explicit BezPath(const std::vector<PathEl>& els) : elements_(els) {}
    
    // Constructors
    static BezPath with_capacity(size_t capacity);
    static BezPath from_vec(const std::vector<PathEl>& v);

    // Basic operations
    void push(const PathEl& el);
    void move_to(const Point& p);
    void line_to(const Point& p);
    void quad_to(const Point& p1, const Point& p2);
    void curve_to(const Point& p1, const Point& p2, const Point& p3);
    void close_path();
    
    // Manipulation
    std::optional<PathEl> pop();
    void truncate(size_t len);
    std::optional<PathSeg> get_seg(size_t ix) const;

    // Access
    const std::vector<PathEl>& elements() const { return elements_; }
    std::vector<PathEl>& elements_mut() { return elements_; }
    bool is_empty() const;
    bool is_finite() const;
    bool is_nan() const;
    
    // Geometric operations
    void apply_affine(const Affine& affine);
    Rect control_box() const;
    BezPath reverse_subpaths() const;

    // Flatten the path into line segments with given tolerance
    void flatten(double tolerance, std::vector<PathEl>& out) const;

    // Area, perimeter, winding, bounding box
    double area() const;
    double perimeter(double accuracy = 1e-9) const;
    int winding(const Point& pt) const;
    Rect bounding_box() const;

    // Extend from another container
    template<typename InputIt>
    void extend(InputIt first, InputIt last) {
        elements_.insert(elements_.end(), first, last);
    }

    // From iterator
    template<typename InputIt>
    static BezPath from_iter(InputIt first, InputIt last) {
        return BezPath(std::vector<PathEl>(first, last));
    }

    // Into iterator
    std::vector<PathEl>::const_iterator begin() const { return elements_.begin(); }
    std::vector<PathEl>::const_iterator end() const { return elements_.end(); }

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
            bool operator==(const Iter& other) const;
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

// Free functions
std::vector<PathEl> flatten(const std::vector<PathEl>& path, double tolerance);
void reverse_subpath(Point start_pt, const std::vector<PathEl>& els, BezPath& reversed);

// Operators
BezPath operator*(const Affine& affine, const BezPath& path);
PathSeg operator*(const Affine& affine, const PathSeg& seg);

// Сравнение PathEl
bool operator==(const PathEl& a, const PathEl& b);
bool operator!=(const PathEl& a, const PathEl& b);

// Сравнение BezPath
bool operator==(const BezPath& a, const BezPath& b);
bool operator!=(const BezPath& a, const BezPath& b);

} // namespace kurbo 