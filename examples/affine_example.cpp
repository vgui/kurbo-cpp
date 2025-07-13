#include <iostream>
#include <iomanip>
#include "kurbo/affine.hpp"
#include "kurbo/point.hpp"
#include "kurbo/vec2.hpp"

using namespace kurbo;

int main() {
    std::cout << "=== Affine Example ===" << std::endl;
    
    // Create different types of transformations
    Affine identity = Affine::identity();
    Affine translate = Affine::translate(Vec2(10, 20));
    Affine scale = Affine::scale(2.0);
    Affine scale_xy = Affine::scale_non_uniform(2.0, 3.0);
    Affine rotate = Affine::rotate(M_PI / 4); // 45 degrees
    // Note: shear method doesn't exist in our implementation
    Affine skew = Affine::skew(0.5, 0.3);
    
    std::cout << "Identity: " << identity << std::endl;
    std::cout << "Translate: " << translate << std::endl;
    std::cout << "Scale: " << scale << std::endl;
    std::cout << "Scale XY: " << scale_xy << std::endl;
    std::cout << "Rotate: " << rotate << std::endl;
    std::cout << "Skew: " << skew << std::endl;
    
    // Transform points
    Point p(5, 10);
    std::cout << "\n--- Point Transformations ---" << std::endl;
    std::cout << "Original point: " << p << std::endl;
    std::cout << "Translated: " << translate * p << std::endl;
    std::cout << "Scaled: " << scale * p << std::endl;
    std::cout << "Rotated: " << rotate * p << std::endl;
    std::cout << "Skewed: " << skew * p << std::endl;
    
    // Combine transformations
    std::cout << "\n--- Combined Transformations ---" << std::endl;
    Affine combined = translate * scale * rotate;
    std::cout << "Combined (translate * scale * rotate): " << combined << std::endl;
    std::cout << "Point after combined transform: " << combined * p << std::endl;
    
    // Matrix elements
    std::cout << "\n--- Matrix Elements ---" << std::endl;
    std::cout << "Scale matrix coefficients:" << std::endl;
    auto coeffs = scale.as_coeffs();
    std::cout << "  [ " << coeffs[0] << " " << coeffs[2] << " " << coeffs[4] << " ]" << std::endl;
    std::cout << "  [ " << coeffs[1] << " " << coeffs[3] << " " << coeffs[5] << " ]" << std::endl;
    
    // Determinant and invertibility
    std::cout << "\n--- Determinant and Inversion ---" << std::endl;
    std::cout << "Scale determinant: " << scale.determinant() << std::endl;
    std::cout << "Scale is invertible: " << (scale.determinant() != 0.0 ? "true" : "false") << std::endl;
    
    Affine scale_inv = scale.inverse();
    std::cout << "Scale inverse: " << scale_inv << std::endl;
    std::cout << "Scale * inverse: " << scale * scale_inv << std::endl;
    
    // Transform vectors
    std::cout << "\n--- Vector Transformations ---" << std::endl;
    Vec2 v(3, 4);
    std::cout << "Original vector: " << v << std::endl;
    std::cout << "Translated vector: " << translate * v << std::endl;
    std::cout << "Scaled vector: " << scale * v << std::endl;
    std::cout << "Rotated vector: " << rotate * v << std::endl;
    
    // Special transformations
    std::cout << "\n--- Special Transformations ---" << std::endl;
    Affine flip_x = Affine::scale_non_uniform(-1, 1);
    Affine flip_y = Affine::scale_non_uniform(1, -1);
    Affine rotate_90 = Affine::rotate(M_PI / 2);
    Affine rotate_180 = Affine::rotate(M_PI);
    Affine rotate_270 = Affine::rotate(3 * M_PI / 2);
    
    std::cout << "Flip X: " << flip_x << std::endl;
    std::cout << "Flip Y: " << flip_y << std::endl;
    std::cout << "Rotate 90°: " << rotate_90 << std::endl;
    std::cout << "Rotate 180°: " << rotate_180 << std::endl;
    std::cout << "Rotate 270°: " << rotate_270 << std::endl;
    
    Point test_point(10, 5);
    std::cout << "Test point: " << test_point << std::endl;
    std::cout << "Flipped X: " << flip_x * test_point << std::endl;
    std::cout << "Flipped Y: " << flip_y * test_point << std::endl;
    std::cout << "Rotated 90°: " << rotate_90 * test_point << std::endl;
    std::cout << "Rotated 180°: " << rotate_180 * test_point << std::endl;
    std::cout << "Rotated 270°: " << rotate_270 * test_point << std::endl;
    
    // Transform composition
    std::cout << "\n--- Transform Composition ---" << std::endl;
    Affine complex = Affine::translate(Vec2(5, 10)) * 
                     Affine::rotate(M_PI / 6) * 
                     Affine::scale(1.5);
    std::cout << "Complex transform: " << complex << std::endl;
    std::cout << "Point after complex transform: " << complex * test_point << std::endl;
    
    return 0;
} 