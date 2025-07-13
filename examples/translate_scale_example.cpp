#include <iostream>
#include <iomanip>
#include "kurbo/translate_scale.hpp"
#include "kurbo/point.hpp"
#include "kurbo/vec2.hpp"

using namespace kurbo;

int main() {
    std::cout << "=== TranslateScale Example ===" << std::endl;
    
    // Create different translate-scale transformations
    TranslateScale identity = TranslateScale::identity();
    TranslateScale translate_only = TranslateScale::translate(Vec2(10, 20));
    TranslateScale scale_only = TranslateScale::scale_only(2.0);
    TranslateScale scale_xy = TranslateScale::scale_non_uniform(2.0, 3.0);
    TranslateScale translate_scale = TranslateScale::new_translate_scale(Vec2(5, 10), 1.5);
    TranslateScale translate_scale_xy = TranslateScale::new_translate_scale_non_uniform(Vec2(5, 10), Vec2(1.5, 2.0));
    
    std::cout << "Identity: " << identity << std::endl;
    std::cout << "Translate only: " << translate_only << std::endl;
    std::cout << "Scale only: " << scale_only << std::endl;
    std::cout << "Scale XY: " << scale_xy << std::endl;
    std::cout << "Translate + Scale: " << translate_scale << std::endl;
    std::cout << "Translate + Scale XY: " << translate_scale_xy << std::endl;
    
    // Transform points
    Point p(5, 10);
    std::cout << "\n--- Point Transformations ---" << std::endl;
    std::cout << "Original point: " << p << std::endl;
    std::cout << "Translated: " << translate_only * p << std::endl;
    std::cout << "Scaled: " << scale_only * p << std::endl;
    std::cout << "Scaled XY: " << scale_xy * p << std::endl;
    std::cout << "Translate + Scale: " << translate_scale * p << std::endl;
    std::cout << "Translate + Scale XY: " << translate_scale_xy * p << std::endl;
    
    // Transform vectors
    std::cout << "\n--- Vector Transformations ---" << std::endl;
    Vec2 v(3, 4);
    std::cout << "Original vector: " << v << std::endl;
    std::cout << "Translated vector: " << v << " (translation does not affect vectors)" << std::endl;
    std::cout << "Scaled vector: " << scale_only.get_scale() * v << std::endl;
    std::cout << "Scaled XY vector: " << scale_xy.get_scale() * v << " (non-uniform not supported, uses x scale)" << std::endl;
    
    // Properties
    std::cout << "\n--- Properties ---" << std::endl;
    std::cout << "Scale only translation: " << scale_only.get_translation() << std::endl;
    std::cout << "Scale only scale: " << scale_only.scale << std::endl;
    std::cout << "Scale XY scale: " << scale_xy.scale << std::endl;
    std::cout << "Translate + Scale translation: " << translate_scale.get_translation() << std::endl;
    std::cout << "Translate + Scale scale: " << translate_scale.scale << std::endl;
    
    // Inversion
    std::cout << "\n--- Inversion ---" << std::endl;
    TranslateScale scale_inv = scale_only.inverse();
    std::cout << "Scale inverse: " << scale_inv << std::endl;
    std::cout << "Scale * inverse: " << scale_only * scale_inv << std::endl;
    
    TranslateScale translate_scale_inv = translate_scale.inverse();
    std::cout << "Translate + Scale inverse: " << translate_scale_inv << std::endl;
    std::cout << "Translate + Scale * inverse: " << translate_scale * translate_scale_inv << std::endl;
    
    // Composition
    std::cout << "\n--- Composition ---" << std::endl;
    TranslateScale combined = translate_only * scale_only;
    std::cout << "Translate * Scale: " << combined << std::endl;
    std::cout << "Point after combined: " << combined * p << std::endl;
    
    TranslateScale combined2 = scale_only * translate_only;
    std::cout << "Scale * Translate: " << combined2 << std::endl;
    std::cout << "Point after combined2: " << combined2 * p << std::endl;
    
    // Special cases
    std::cout << "\n--- Special Cases ---" << std::endl;
    TranslateScale flip_x = TranslateScale::scale_non_uniform(-1, 1);
    TranslateScale flip_y = TranslateScale::scale_non_uniform(1, -1);
    TranslateScale flip_both = TranslateScale::scale_non_uniform(-1, -1);
    
    std::cout << "Flip X: " << flip_x << std::endl;
    std::cout << "Flip Y: " << flip_y << std::endl;
    std::cout << "Flip both: " << flip_both << std::endl;
    
    Point test_point(10, 5);
    std::cout << "Test point: " << test_point << std::endl;
    std::cout << "Flipped X: " << flip_x * test_point << std::endl;
    std::cout << "Flipped Y: " << flip_y * test_point << std::endl;
    std::cout << "Flipped both: " << flip_both * test_point << std::endl;
    
    // Zero scale handling
    std::cout << "\n--- Zero Scale Handling ---" << std::endl;
    TranslateScale zero_scale = TranslateScale::scale_only(0.0);
    std::cout << "Zero scale: " << zero_scale << std::endl;
    std::cout << "Zero scale is invertible: " << (zero_scale.is_invertible() ? "true" : "false") << std::endl;
    
    // Negative scale
    std::cout << "\n--- Negative Scale ---" << std::endl;
    TranslateScale negative_scale = TranslateScale::scale_only(-2.0);
    std::cout << "Negative scale: " << negative_scale << std::endl;
    std::cout << "Point with negative scale: " << negative_scale * test_point << std::endl;
    
    return 0;
} 