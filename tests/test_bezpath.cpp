#include <gtest/gtest.h>
#include "kurbo/bezpath.hpp"
#include "kurbo/point.hpp"
#include "kurbo/rect.hpp"
#include "kurbo/affine.hpp"

using namespace kurbo;

TEST(BezPathTest, BasicOps) {
    BezPath path;
    EXPECT_TRUE(path.is_empty());

    path.move_to({0, 0});
    path.line_to({1, 0});
    path.quad_to({1, 1}, {0, 1});
    path.curve_to({-1, 1}, {-1, 0}, {0, 0});
    path.close_path();

    EXPECT_FALSE(path.is_empty());
    EXPECT_EQ(path.elements().size(), 5);

    // Test pop
    auto popped = path.pop();
    EXPECT_TRUE(popped.has_value());
    EXPECT_EQ(popped->type, PathElType::ClosePath);
    EXPECT_EQ(path.elements().size(), 4);

    // Test truncate
    path.truncate(2);
    EXPECT_EQ(path.elements().size(), 2);

    // Test get_seg
    auto seg = path.get_seg(1);
    EXPECT_TRUE(seg.has_value());
    EXPECT_EQ(seg->type(), PathSeg::Kind::Line);

    // Test is_finite and is_nan
    EXPECT_TRUE(path.is_finite());
    EXPECT_FALSE(path.is_nan());

    // Test control_box
    Rect cbox = path.control_box();
    EXPECT_TRUE(cbox.is_finite());

    // Test reverse_subpaths
    BezPath reversed = path.reverse_subpaths();
    EXPECT_EQ(reversed.elements().size(), 2);

    // Test segments iterator
    auto segments = path.segments();
    auto it = segments.begin();
    EXPECT_NE(it, segments.end());

    // Test operators
    Affine scale = Affine::scale(2.0);
    BezPath scaled = scale * path;
    EXPECT_EQ(scaled.elements().size(), 2);
}

TEST(BezPathTest, FlattenAreaPerimeterWindingBBox) {
    // Square path
    BezPath path;
    path.move_to({0, 0});
    path.line_to({1, 0});
    path.line_to({1, 1});
    path.line_to({0, 1});
    path.close_path();

    // Flatten
    std::vector<PathEl> flat;
    path.flatten(1e-6, flat);
    int line_count = 0;
    for (const auto& el : flat) {
        if (el.type == PathElType::LineTo) ++line_count;
    }
    EXPECT_EQ(line_count, 4);

    // Area
    double area = path.area();
    EXPECT_NEAR(area, 1.0, 1e-9);

    // Perimeter
    double peri = path.perimeter();
    EXPECT_NEAR(peri, 4.0, 1e-9);

    // Winding (inside)
    int wind = path.winding(Point(0.5, 0.5));
    EXPECT_EQ(wind, 1);

    // Winding (outside)
    wind = path.winding(Point(2, 2));
    EXPECT_EQ(wind, 0);

    // Bounding box
    Rect bbox = path.bounding_box();
    EXPECT_NEAR(bbox.x0, 0.0, 1e-9);
    EXPECT_NEAR(bbox.y0, 0.0, 1e-9);
    EXPECT_NEAR(bbox.x1, 1.0, 1e-9);
    EXPECT_NEAR(bbox.y1, 1.0, 1e-9);
}

TEST(BezPathTest, EqualityAndFromVec) {
    std::vector<PathEl> els = {
        PathEl(PathElType::MoveTo, Point(0, 0)),
        PathEl(PathElType::LineTo, Point(1, 0)),
        PathEl(PathElType::QuadTo, Point(1, 1), Point(0, 1)),
        PathEl(PathElType::CurveTo, Point(-1, 1), Point(-1, 0), Point(0, 0)),
        PathEl(PathElType::ClosePath)
    };
    BezPath path1 = BezPath::from_vec(els);
    BezPath path2 = BezPath::from_vec(els);
    EXPECT_EQ(path1, path2);
    path2.pop();
    EXPECT_NE(path1, path2);
}

TEST(BezPathTest, ExtendAndIter) {
    BezPath path;
    std::vector<PathEl> els = {
        PathEl(PathElType::MoveTo, Point(0, 0)),
        PathEl(PathElType::LineTo, Point(1, 0)),
        PathEl(PathElType::LineTo, Point(1, 1)),
        PathEl(PathElType::ClosePath)
    };
    path.extend(els.begin(), els.end());
    EXPECT_EQ(path.elements().size(), 4);
    int count = 0;
    for (const auto& el : path.elements()) {
        (void)el;
        ++count;
    }
    EXPECT_EQ(count, 4);
}

TEST(BezPathTest, ReverseSubpaths) {
    BezPath path;
    path.move_to({0, 0});
    path.line_to({1, 0});
    path.line_to({1, 1});
    path.line_to({0, 1});
    path.close_path();
    BezPath rev = path.reverse_subpaths();
    EXPECT_EQ(rev.elements().size(), path.elements().size());
    // Проверяем, что первый MoveTo совпадает с последней точкой исходного пути
    EXPECT_EQ(rev.elements().front().point, path.elements().back().point);
}

TEST(BezPathTest, FlattenCurves) {
    BezPath path;
    path.move_to({0, 0});
    path.quad_to({1, 2}, {2, 0});
    path.curve_to({3, 2}, {4, -2}, {5, 0});
    std::vector<PathEl> flat;
    path.flatten(1e-2, flat);
    // Должно быть больше 2 LineTo (flatten кривых)
    int line_count = 0;
    for (const auto& el : flat) {
        if (el.type == PathElType::LineTo) ++line_count;
    }
    EXPECT_GT(line_count, 2);
}

TEST(BezPathTest, PopTruncateEdgeCases) {
    BezPath path;
    path.move_to({0, 0});
    path.line_to({1, 0});
    path.line_to({1, 1});
    path.close_path();
    path.truncate(1);
    EXPECT_EQ(path.elements().size(), 1);
    auto popped = path.pop();
    EXPECT_TRUE(popped.has_value());
    EXPECT_EQ(path.elements().size(), 0);
    popped = path.pop();
    EXPECT_FALSE(popped.has_value());
}

TEST(BezPathTest, AffineTransform) {
    BezPath path;
    path.move_to({0, 0});
    path.line_to({1, 0});
    Affine t = Affine::translate({2, 3});
    BezPath moved = t * path;
    EXPECT_EQ(moved.elements().front().point, Point(2, 3));
    EXPECT_EQ(moved.elements()[1].point, Point(3, 3));
}

TEST(BezPathTest, BoundingBoxComplex) {
    BezPath path;
    path.move_to({-1, -2});
    path.line_to({2, 3});
    path.line_to({-2, 4});
    path.close_path();
    Rect bbox = path.bounding_box();
    EXPECT_LE(bbox.x0, -2.0);
    EXPECT_LE(bbox.y0, -2.0);
    EXPECT_GE(bbox.x1, 2.0);
    EXPECT_GE(bbox.y1, 4.0);
}

TEST(BezPathTest, MultiSubpath) {
    BezPath path;
    path.move_to({0, 0});
    path.line_to({1, 0});
    path.close_path();
    path.move_to({2, 2});
    path.line_to({3, 2});
    path.close_path();
    EXPECT_EQ(path.elements().size(), 6);
    // Проверяем, что обе подцепочки корректно закрыты
    int close_count = 0;
    for (const auto& el : path.elements()) {
        if (el.type == PathElType::ClosePath) ++close_count;
    }
    EXPECT_EQ(close_count, 2);
}

TEST(BezPathTest, EmptyPath) {
    BezPath path;
    EXPECT_TRUE(path.is_empty());
    EXPECT_EQ(path.elements().size(), 0);
    std::vector<PathEl> flat;
    path.flatten(1e-6, flat);
    EXPECT_TRUE(flat.empty());
    Rect bbox = path.bounding_box();
    EXPECT_FALSE(bbox.is_finite());
} 

TEST(BezPathTest, SegmentsIterator) {
    BezPath path;
    path.move_to({0, 0});
    path.line_to({1, 0});
    path.quad_to({1, 1}, {0, 1});
    path.curve_to({-1, 1}, {-1, 0}, {0, 0});
    path.close_path();
    
    auto segments = path.segments();
    int count = 0;
    for (auto it = segments.begin(); it != segments.end(); ++it) {
        ++count;
        EXPECT_TRUE((*it).is_finite());
        EXPECT_FALSE((*it).is_nan());
    }
    EXPECT_GT(count, 0);
}

TEST(BezPathTest, ComplexPath) {
    BezPath path;
    // Создаем сложный путь с несколькими под-путями
    path.move_to({0, 0});
    path.line_to({1, 0});
    path.line_to({1, 1});
    path.close_path();
    
    path.move_to({2, 0});
    path.line_to({3, 0});
    path.line_to({3, 1});
    path.close_path();
    
    path.move_to({4, 0});
    path.quad_to({5, 1}, {6, 0});
    path.curve_to({7, 1}, {8, -1}, {9, 0});
    path.close_path();
    
    EXPECT_EQ(path.elements().size(), 12);
    EXPECT_FALSE(path.is_empty());
    
    // Проверяем, что все элементы валидны
    for (const auto& el : path.elements()) {
        EXPECT_TRUE(el.point.is_finite());
    }
}

TEST(BezPathTest, PathValidation) {
    // Тест на валидность путей
    BezPath valid_path;
    valid_path.move_to({0, 0});
    valid_path.line_to({1, 0});
    valid_path.close_path();
    EXPECT_TRUE(valid_path.is_finite());
    EXPECT_FALSE(valid_path.is_nan());
    
    // Тест на невалидный путь (с NaN)
    BezPath invalid_path;
    invalid_path.move_to({std::numeric_limits<double>::quiet_NaN(), 0});
    EXPECT_FALSE(invalid_path.is_finite());
}

TEST(BezPathTest, ExtendOperations) {
    BezPath path1;
    path1.move_to({0, 0});
    path1.line_to({1, 0});
    
    BezPath path2;
    path2.move_to({2, 0});
    path2.line_to({3, 0});
    
    // Тестируем extend с другим путем
    std::vector<PathEl> els = path2.elements();
    path1.extend(els.begin(), els.end());
    EXPECT_EQ(path1.elements().size(), 4);
}

TEST(BezPathTest, GetSegOperations) {
    BezPath path;
    path.move_to({0, 0});
    path.line_to({1, 0});
    path.quad_to({1, 1}, {0, 1});
    path.curve_to({-1, 1}, {-1, 0}, {0, 0});
    path.close_path();
    
    // Тестируем get_seg для разных индексов
    // Индекс 1 соответствует LineTo
    auto seg0 = path.get_seg(1);
    EXPECT_TRUE(seg0.has_value());
    EXPECT_EQ(seg0->type(), PathSeg::Kind::Line);
    
    // Индекс 2 соответствует QuadTo
    auto seg1 = path.get_seg(2);
    EXPECT_TRUE(seg1.has_value());
    EXPECT_EQ(seg1->type(), PathSeg::Kind::Quad);
    
    // Индекс 3 соответствует CurveTo
    auto seg2 = path.get_seg(3);
    EXPECT_TRUE(seg2.has_value());
    EXPECT_EQ(seg2->type(), PathSeg::Kind::Cubic);
    
    // Индекс 4 соответствует ClosePath
    auto seg3 = path.get_seg(4);
    EXPECT_TRUE(seg3.has_value());
    EXPECT_EQ(seg3->type(), PathSeg::Kind::Line); // ClosePath создает Line сегмент
    
    // Тестируем невалидный индекс
    auto seg_invalid = path.get_seg(10);
    EXPECT_FALSE(seg_invalid.has_value());
}

TEST(BezPathTest, ControlBox) {
    BezPath path;
    path.move_to({0, 0});
    path.line_to({1, 0});
    path.line_to({1, 1});
    path.line_to({0, 1});
    path.close_path();
    
    Rect cbox = path.control_box();
    EXPECT_TRUE(cbox.is_finite());
    EXPECT_LE(cbox.x0, 0.0);
    EXPECT_GE(cbox.x1, 1.0);
    EXPECT_LE(cbox.y0, 0.0);
    EXPECT_GE(cbox.y1, 1.0);
}

TEST(BezPathTest, WithCapacity) {
    BezPath path = BezPath::with_capacity(10);
    EXPECT_TRUE(path.is_empty());
    EXPECT_EQ(path.elements().size(), 0);
    
    // Добавляем элементы
    path.move_to({0, 0});
    path.line_to({1, 0});
    path.close_path();
    
    EXPECT_FALSE(path.is_empty());
    EXPECT_EQ(path.elements().size(), 3);
}

TEST(BezPathTest, IsFiniteIsNan) {
    BezPath path;
    path.move_to({0, 0});
    path.line_to({1, 0});
    path.close_path();
    
    EXPECT_TRUE(path.is_finite());
    EXPECT_FALSE(path.is_nan());
    
    // Тест с NaN
    BezPath nan_path;
    nan_path.move_to({std::numeric_limits<double>::quiet_NaN(), 0});
    EXPECT_FALSE(nan_path.is_finite());
}

TEST(BezPathTest, ApplyAffine) {
    BezPath path;
    path.move_to({0, 0});
    path.line_to({1, 0});
    path.close_path();
    
    Affine scale = Affine::scale(2.0);
    path.apply_affine(scale);
    
    // Проверяем, что точки были масштабированы
    EXPECT_EQ(path.elements()[0].point, Point(0, 0));
    EXPECT_EQ(path.elements()[1].point, Point(2, 0));
}

TEST(BezPathTest, OperatorMultiplication) {
    BezPath path;
    path.move_to({0, 0});
    path.line_to({1, 0});
    path.close_path();
    
    Affine translate = Affine::translate({2, 3});
    BezPath transformed = translate * path;
    
    // Проверяем, что путь был перемещен
    EXPECT_EQ(transformed.elements()[0].point, Point(2, 3));
    EXPECT_EQ(transformed.elements()[1].point, Point(3, 3));
}

TEST(BezPathTest, EmptyPathOperations) {
    BezPath empty;
    
    // Все операции должны работать корректно с пустым путем
    EXPECT_TRUE(empty.is_empty());
    EXPECT_EQ(empty.elements().size(), 0);
    
    std::vector<PathEl> flat;
    empty.flatten(1e-6, flat);
    EXPECT_TRUE(flat.empty());
    
    EXPECT_EQ(empty.area(), 0.0);
    EXPECT_EQ(empty.perimeter(), 0.0);
    EXPECT_EQ(empty.winding({0.5, 0.5}), 0);
    
    Rect bbox = empty.bounding_box();
    EXPECT_FALSE(bbox.is_finite());
    
    auto segments = empty.segments();
    auto it = segments.begin();
    EXPECT_EQ(it, segments.end());
}

TEST(BezPathTest, PathElComparison) {
    PathEl el1(PathElType::MoveTo, Point(0, 0));
    PathEl el2(PathElType::MoveTo, Point(0, 0));
    PathEl el3(PathElType::LineTo, Point(1, 0));
    
    EXPECT_EQ(el1, el2);
    EXPECT_NE(el1, el3);
    
    PathEl quad1(PathElType::QuadTo, Point(1, 1), Point(0, 1));
    PathEl quad2(PathElType::QuadTo, Point(1, 1), Point(0, 1));
    PathEl quad3(PathElType::QuadTo, Point(1, 1), Point(0, 2));
    
    EXPECT_EQ(quad1, quad2);
    EXPECT_NE(quad1, quad3);
}

TEST(BezPathTest, BezPathComparison) {
    BezPath path1;
    path1.move_to({0, 0});
    path1.line_to({1, 0});
    path1.close_path();
    
    BezPath path2;
    path2.move_to({0, 0});
    path2.line_to({1, 0});
    path2.close_path();
    
    BezPath path3;
    path3.move_to({0, 0});
    path3.line_to({1, 1}); // Разная точка
    path3.close_path();
    
    EXPECT_EQ(path1, path2);
    EXPECT_NE(path1, path3);
    
    // Пустые пути должны быть равны
    BezPath empty1, empty2;
    EXPECT_EQ(empty1, empty2);
} 