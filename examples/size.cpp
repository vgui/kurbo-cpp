#include "kurbo/kurbo.hpp"
#include <iostream>

using namespace kurbo;

int main() {
    std::cout << "=== Size Operations ===" << std::endl;
    
    // Create different sizes
    Size s1(10.0, 20.0);
    Size s2(5.0, 10.0);
    Size s3(16.0, 9.0); // 16:9 aspect ratio
    
    std::cout << "Size 1: " << s1 << std::endl;
    std::cout << "Size 2: " << s2 << std::endl;
    std::cout << "Size 3: " << s3 << std::endl;
    
    // Basic properties
    std::cout << "\n=== Basic Properties ===" << std::endl;
    std::cout << "Size 1 area: " << s1.area() << std::endl;
    std::cout << "Size 1 aspect ratio: " << s1.aspect_ratio() << std::endl;
    std::cout << "Size 1 max side: " << s1.max_side() << std::endl;
    std::cout << "Size 1 min side: " << s1.min_side() << std::endl;
    
    std::cout << "Size 3 aspect ratio: " << s3.aspect_ratio() << std::endl;
    
    // Empty check
    Size zero(0.0, 0.0);
    std::cout << "Zero size is empty: " << (zero.is_empty() ? "true" : "false") << std::endl;
    std::cout << "Size 1 is empty: " << (s1.is_empty() ? "true" : "false") << std::endl;
    
    // Arithmetic operations
    std::cout << "\n=== Arithmetic Operations ===" << std::endl;
    
    Size sum = s1 + s2;
    std::cout << "s1 + s2 = " << sum << std::endl;
    
    Size diff = s1 - s2;
    std::cout << "s1 - s2 = " << diff << std::endl;
    
    Size scaled = s1 * 2.0;
    std::cout << "s1 * 2 = " << scaled << std::endl;
    
    Size divided = s1 / 2.0;
    std::cout << "s1 / 2 = " << divided << std::endl;
    
    Size component_product = s1 * s2;
    std::cout << "s1 * s2 (component-wise) = " << component_product << std::endl;
    
    // Min/Max operations
    std::cout << "\n=== Min/Max Operations ===" << std::endl;
    
    Size min_result = s1.min(s2);
    std::cout << "min(s1, s2) = " << min_result << std::endl;
    
    Size max_result = s1.max(s2);
    std::cout << "max(s1, s2) = " << max_result << std::endl;
    
    // Clamp operation
    Size clamp_result = s1.clamp(s2, scaled);
    std::cout << "clamp(s1, s2, s1*2) = " << clamp_result << std::endl;
    
    // Rounding operations
    std::cout << "\n=== Rounding Operations ===" << std::endl;
    
    Size fractional(3.3, 3.6);
    std::cout << "Original: " << fractional << std::endl;
    std::cout << "Rounded: " << fractional.round() << std::endl;
    std::cout << "Ceiled: " << fractional.ceil() << std::endl;
    std::cout << "Floored: " << fractional.floor() << std::endl;
    std::cout << "Expanded: " << fractional.expand() << std::endl;
    std::cout << "Truncated: " << fractional.trunc() << std::endl;
    
    // Validation
    std::cout << "\n=== Validation ===" << std::endl;
    
    Size valid(1.0, 2.0);
    Size inf_size(std::numeric_limits<double>::infinity(), 1.0);
    Size nan_size(std::numeric_limits<double>::quiet_NaN(), 1.0);
    
    std::cout << "Valid size is finite: " << (valid.is_finite() ? "true" : "false") << std::endl;
    std::cout << "Inf size is finite: " << (inf_size.is_finite() ? "true" : "false") << std::endl;
    std::cout << "NaN size is NaN: " << (nan_size.is_nan() ? "true" : "false") << std::endl;
    
    // Conversion to Vec2
    std::cout << "\n=== Conversion ===" << std::endl;
    
    Vec2 vec = s1.to_vec2();
    std::cout << "s1 as Vec2: " << vec << std::endl;
    
    // Common aspect ratios
    std::cout << "\n=== Common Aspect Ratios ===" << std::endl;
    
    Size ratios[] = {
        Size(4.0, 3.0),   // 4:3
        Size(16.0, 9.0),  // 16:9
        Size(21.0, 9.0),  // 21:9 (ultrawide)
        Size(1.0, 1.0),   // 1:1 (square)
        Size(3.0, 2.0),   // 3:2 (classic)
        Size(5.0, 4.0),   // 5:4
    };
    
    const char* names[] = {"4:3", "16:9", "21:9", "1:1", "3:2", "5:4"};
    
    for (int i = 0; i < 6; ++i) {
        std::cout << names[i] << " aspect ratio: " << ratios[i].aspect_ratio() << std::endl;
    }
    
    // Size scaling examples
    std::cout << "\n=== Scaling Examples ===" << std::endl;
    
    Size original(100.0, 200.0);
    std::cout << "Original: " << original << std::endl;
    
    // Scale by factor
    Size scaled_up = original * 1.5;
    std::cout << "Scaled up by 1.5: " << scaled_up << std::endl;
    
    Size scaled_down = original * 0.75;
    std::cout << "Scaled down by 0.75: " << scaled_down << std::endl;
    
    // Scale to fit within bounds
    Size bounds(150.0, 150.0);
    double scale_factor = std::min(bounds.width / original.width, 
                                  bounds.height / original.height);
    Size scaled_to_fit = original * scale_factor;
    std::cout << "Scaled to fit in " << bounds << ": " << scaled_to_fit << std::endl;
    
    // Scale to fill bounds
    scale_factor = std::max(bounds.width / original.width, 
                           bounds.height / original.height);
    Size scaled_to_fill = original * scale_factor;
    std::cout << "Scaled to fill " << bounds << ": " << scaled_to_fill << std::endl;
    
    return 0;
} 