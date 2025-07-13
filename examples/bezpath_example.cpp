#include "kurbo/kurbo.hpp"
#include <iostream>
#include <iomanip>

using namespace kurbo;

int main() {
    std::cout << "=== BezPath Example ===" << std::endl;
    
    // Создаем простой путь - квадрат
    BezPath square;
    square.move_to({0, 0});
    square.line_to({1, 0});
    square.line_to({1, 1});
    square.line_to({0, 1});
    square.close_path();
    
    std::cout << "Square path:" << std::endl;
    std::cout << "  Elements: " << square.elements().size() << std::endl;
    std::cout << "  Area: " << std::fixed << std::setprecision(6) << square.area() << std::endl;
    std::cout << "  Perimeter: " << square.perimeter() << std::endl;
    
    Rect bbox = square.bounding_box();
    std::cout << "  Bounding box: (" << bbox.x0 << ", " << bbox.y0 
              << ") to (" << bbox.x1 << ", " << bbox.y1 << ")" << std::endl;
    
    // Проверяем winding
    Point inside(0.5, 0.5);
    Point outside(2, 2);
    std::cout << "  Winding at (0.5, 0.5): " << square.winding(inside) << std::endl;
    std::cout << "  Winding at (2, 2): " << square.winding(outside) << std::endl;
    
    // Создаем путь с кривыми
    BezPath curve_path;
    curve_path.move_to({0, 0});
    curve_path.quad_to({1, 1}, {2, 0});
    curve_path.curve_to({3, 1}, {4, -1}, {5, 0});
    curve_path.close_path();
    
    std::cout << "\nCurve path:" << std::endl;
    std::cout << "  Elements: " << curve_path.elements().size() << std::endl;
    std::cout << "  Area: " << curve_path.area() << std::endl;
    std::cout << "  Perimeter: " << curve_path.perimeter() << std::endl;
    
    // Flatten путь
    std::vector<PathEl> flat;
    curve_path.flatten(1e-3, flat);
    std::cout << "  Flattened elements: " << flat.size() << std::endl;
    
    // Аффинное преобразование
    Affine scale = Affine::scale(2.0);
    BezPath scaled = scale * square;
    std::cout << "\nScaled square:" << std::endl;
    std::cout << "  Area: " << scaled.area() << std::endl;
    
    // Обратное преобразование путей
    BezPath reversed = square.reverse_subpaths();
    std::cout << "\nReversed square:" << std::endl;
    std::cout << "  Elements: " << reversed.elements().size() << std::endl;
    
    // Работа с пустыми путями
    BezPath empty;
    std::cout << "\nEmpty path:" << std::endl;
    std::cout << "  Is empty: " << (empty.is_empty() ? "true" : "false") << std::endl;
    std::cout << "  Elements: " << empty.elements().size() << std::endl;
    
    Rect empty_bbox = empty.bounding_box();
    std::cout << "  Bounding box is finite: " << (empty_bbox.is_finite() ? "true" : "false") << std::endl;
    
    // Создание пути из вектора элементов
    std::vector<PathEl> els = {
        PathEl(PathElType::MoveTo, Point(0, 0)),
        PathEl(PathElType::LineTo, Point(1, 0)),
        PathEl(PathElType::LineTo, Point(1, 1)),
        PathEl(PathElType::ClosePath)
    };
    BezPath from_vec = BezPath::from_vec(els);
    std::cout << "\nPath from vector:" << std::endl;
    std::cout << "  Elements: " << from_vec.elements().size() << std::endl;
    std::cout << "  Area: " << from_vec.area() << std::endl;
    
    // Операции pop и truncate
    BezPath test_path;
    test_path.move_to({0, 0});
    test_path.line_to({1, 0});
    test_path.line_to({1, 1});
    test_path.close_path();
    
    std::cout << "\nTest path operations:" << std::endl;
    std::cout << "  Initial elements: " << test_path.elements().size() << std::endl;
    
    auto popped = test_path.pop();
    if (popped.has_value()) {
        std::cout << "  Popped element type: " << static_cast<int>(popped->type) << std::endl;
    }
    std::cout << "  After pop: " << test_path.elements().size() << std::endl;
    
    test_path.truncate(1);
    std::cout << "  After truncate(1): " << test_path.elements().size() << std::endl;
    
    std::cout << "\n=== Example completed ===" << std::endl;
    return 0;
} 