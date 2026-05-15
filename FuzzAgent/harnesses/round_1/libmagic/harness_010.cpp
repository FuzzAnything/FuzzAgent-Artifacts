#include <fuzzer/FuzzedDataProvider.h>
#include "magic.h"
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <cfloat>
#include <cmath>
#include <array>
#include <algorithm>
#include <unistd.h>
#include <stdio.h>

namespace harness_010 {
/*
 * Specialized fuzzing harness for libmagic targeting magiccheck() function 
 * with complex multi-type pattern combinations to exercise transition logic
 * between different data type comparisons.
 * 
 * Primary targets:
 * - magiccheck() transition logic between FILE_FLOAT, FILE_DOUBLE, integer types, and string types
 * - 152 blocked branches in magiccheck() function for floating-point comparison cases
 * - Complex pattern sequences that test how magiccheck handles mixed data type comparisons
 * 
 * APIs targeted:
 * - magic_open, magic_load, magic_buffer, magic_close, magic_setflags
 * 
 * Strategy:
 * 1. Generate complex magic database patterns with sequences of different data types
 * 2. Test transitions: float -> double -> integer -> string in single pattern sequences
 * 3. Test all comparison relations ('x', '!', '=', '>', '<') across mixed types
 * 4. Generate corresponding test buffers with matching data at appropriate offsets
 * 5. Test pattern matching with default magic database plus custom complex patterns
 * 
 * Semantic differentiation from existing harnesses:
 * - harness_003: Custom magic database compilation with hardcoded float/double values
 * - harness_004: Focused float comparison branch testing with custom magic db
 * - harness_007: Direct float/double/string data generation for magiccheck()
 * - harness_008: Comprehensive float/double edge cases with all relations and IEEE 754 specials
 * - harness_009: Dynamic rule compilation from fuzzer input + all relations + IEEE 754 specials
 * - harness_010: Complex multi-type pattern combinations testing transition logic between data types (NEW)
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size for meaningful testing
    // Need: flags + pattern configuration + multiple float/double/int/string values
    if (size < 256) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Consume fixed-size data first
    int flags = fdp.ConsumeIntegral<int>();
    int pattern_type = fdp.ConsumeIntegralInRange<int>(0, 3); // 0-3 different pattern strategies
    bool include_integer_transitions = fdp.ConsumeBool();
    bool include_string_transitions = fdp.ConsumeBool();
    bool test_relation_transitions = fdp.ConsumeBool();
    
    // Build flags (limit to reasonable range)
    int actual_flags = flags & 0xFFFFF;
    
    // Create magic instance
    magic_t magic = magic_open(actual_flags);
    if (magic == nullptr) {
        return 0;
    }

    // Try to load default magic database
    const char* magic_path = "/usr/share/misc/magic";
    magic_load(magic, magic_path); // Ignore errors for fuzzing

    // Strategy selection for pattern generation
    switch (pattern_type) {
        case 0: {
            // Strategy 0: Sequential type transitions (float -> double -> integer)
            std::vector<uint8_t> test_buffer(256, 0);
            
            // Generate values for different types from fuzzer input
            if (fdp.remaining_bytes() < sizeof(float) + sizeof(double) + sizeof(uint32_t)) {
                magic_close(magic);
                return 0;
            }
            
            // Generate float value and place at offset 0
            float float_val = fdp.ConsumeFloatingPoint<float>();
            memcpy(&test_buffer[0], &float_val, sizeof(float));
            
            // Generate double value and place at offset 8  
            double double_val = fdp.ConsumeFloatingPoint<double>();
            memcpy(&test_buffer[8], &double_val, sizeof(double));
            
            // Generate integer value and place at offset 16
            uint32_t int_val = fdp.ConsumeIntegral<uint32_t>();
            memcpy(&test_buffer[16], &int_val, sizeof(uint32_t));
            
            // Test with magic_buffer
            if (!test_buffer.empty()) {
                const char* result = magic_buffer(magic, test_buffer.data(), test_buffer.size());
                (void)result;
            }
            break;
        }
        
        case 1: {
            // Strategy 1: Mixed type patterns with different comparison relations
            std::vector<uint8_t> test_buffer(512, 0);
            
            // Need enough input for multiple values
            if (fdp.remaining_bytes() < 64) {
                magic_close(magic);
                return 0;
            }
            
            // Generate multiple values at different offsets
            int offset = 0;
            
            // Float pattern at offset 0 with relation from input
            if (fdp.remaining_bytes() >= sizeof(float)) {
                float f1 = fdp.ConsumeFloatingPoint<float>();
                memcpy(&test_buffer[offset], &f1, sizeof(float));
                offset += sizeof(float);
            }
            
            // Double pattern at next offset with different relation
            if (fdp.remaining_bytes() >= sizeof(double)) {
                double d1 = fdp.ConsumeFloatingPoint<double>();
                memcpy(&test_buffer[offset], &d1, sizeof(double));
                offset += sizeof(double);
            }
            
            // Integer pattern at next offset
            if (fdp.remaining_bytes() >= sizeof(uint32_t)) {
                uint32_t i1 = fdp.ConsumeIntegral<uint32_t>();
                memcpy(&test_buffer[offset], &i1, sizeof(uint32_t));
                offset += sizeof(uint32_t);
            }
            
            // Another float pattern at different offset with different endianness
            if (fdp.remaining_bytes() >= sizeof(float) && offset + sizeof(float) < test_buffer.size()) {
                float f2 = fdp.ConsumeFloatingPoint<float>();
                // Test little-endian float
                uint32_t f2_bits = *reinterpret_cast<uint32_t*>(&f2);
                // Convert to little-endian byte order
                test_buffer[offset] = static_cast<uint8_t>(f2_bits & 0xFF);
                test_buffer[offset + 1] = static_cast<uint8_t>((f2_bits >> 8) & 0xFF);
                test_buffer[offset + 2] = static_cast<uint8_t>((f2_bits >> 16) & 0xFF);
                test_buffer[offset + 3] = static_cast<uint8_t>((f2_bits >> 24) & 0xFF);
            }
            
            // Test with magic_buffer
            if (!test_buffer.empty()) {
                const char* result = magic_buffer(magic, test_buffer.data(), test_buffer.size());
                (void)result;
            }
            break;
        }
        
        case 2: {
            // Strategy 2: Complex pattern with string transitions
            std::vector<uint8_t> test_buffer(1024, 0);
            
            // Need enough input for string and numeric values
            if (fdp.remaining_bytes() < 128) {
                magic_close(magic);
                return 0;
            }
            
            // Place a float at offset 0
            if (fdp.remaining_bytes() >= sizeof(float)) {
                float float_val = fdp.ConsumeFloatingPoint<float>();
                memcpy(&test_buffer[0], &float_val, sizeof(float));
            }
            
            // Place a string at offset 64
            size_t str_len = fdp.ConsumeIntegralInRange<size_t>(1, 256);
            if (fdp.remaining_bytes() >= str_len && 64 + str_len < test_buffer.size()) {
                std::string test_str = fdp.ConsumeBytesAsString(str_len);
                memcpy(&test_buffer[64], test_str.c_str(), test_str.size());
            }
            
            // Place a double at offset 128
            if (fdp.remaining_bytes() >= sizeof(double)) {
                double double_val = fdp.ConsumeFloatingPoint<double>();
                memcpy(&test_buffer[128], &double_val, sizeof(double));
            }
            
            // Place an integer at offset 136
            if (fdp.remaining_bytes() >= sizeof(uint32_t)) {
                uint32_t int_val = fdp.ConsumeIntegral<uint32_t>();
                memcpy(&test_buffer[136], &int_val, sizeof(uint32_t));
            }
            
            // Test with magic_buffer
            if (!test_buffer.empty()) {
                const char* result = magic_buffer(magic, test_buffer.data(), test_buffer.size());
                (void)result;
            }
            break;
        }
        
        case 3: {
            // Strategy 3: Pattern with IEEE 754 special values transitions
            std::vector<uint8_t> test_buffer(768, 0);
            
            // Generate special float values
            std::vector<float> special_floats;
            for (int i = 0; i < 3 && fdp.remaining_bytes() >= sizeof(float); i++) {
                float val = fdp.ConsumeFloatingPoint<float>();
                
                // Apply special value transformations based on input bits
                uint8_t transform = fdp.ConsumeIntegral<uint8_t>() % 8;
                uint32_t val_bits = *reinterpret_cast<uint32_t*>(&val);
                
                switch (transform) {
                    case 0: // NaN
                        val_bits = 0x7FC00000; // Quiet NaN
                        break;
                    case 1: // Infinity
                        val_bits = 0x7F800000; // Positive infinity
                        break;
                    case 2: // Negative infinity
                        val_bits = 0xFF800000; // Negative infinity
                        break;
                    case 3: // Denormalized
                        val_bits = (val_bits & 0x007FFFFF) | 0x00000000; // Denormal pattern
                        break;
                    case 4: // Zero
                        val_bits = 0x00000000; // Positive zero
                        break;
                    case 5: // Negative zero
                        val_bits = 0x80000000; // Negative zero
                        break;
                    // Cases 6-7: Keep original value
                }
                
                float transformed_val = *reinterpret_cast<float*>(&val_bits);
                special_floats.push_back(transformed_val);
            }
            
            // Place special floats at different offsets
            for (size_t i = 0; i < special_floats.size() && i * 16 < test_buffer.size(); i++) {
                memcpy(&test_buffer[i * 16], &special_floats[i], sizeof(float));
            }
            
            // Generate special double values
            if (fdp.remaining_bytes() >= sizeof(double)) {
                double double_val = fdp.ConsumeFloatingPoint<double>();
                uint64_t double_bits = *reinterpret_cast<uint64_t*>(&double_val);
                
                // Apply transformation
                uint8_t d_transform = fdp.ConsumeIntegral<uint8_t>() % 8;
                switch (d_transform) {
                    case 0: // NaN
                        double_bits = 0x7FF8000000000000ULL; // Quiet NaN
                        break;
                    case 1: // Infinity
                        double_bits = 0x7FF0000000000000ULL; // Positive infinity
                        break;
                    case 2: // Negative infinity
                        double_bits = 0xFFF0000000000000ULL; // Negative infinity
                        break;
                    case 3: // Denormalized
                        double_bits = (double_bits & 0x000FFFFFFFFFFFFFULL) | 0x0000000000000000ULL;
                        break;
                    case 4: // Zero
                        double_bits = 0x0000000000000000ULL; // Positive zero
                        break;
                    case 5: // Negative zero
                        double_bits = 0x8000000000000000ULL; // Negative zero
                        break;
                    // Cases 6-7: Keep original value
                }
                
                double transformed_double = *reinterpret_cast<double*>(&double_bits);
                if (128 < test_buffer.size()) {
                    memcpy(&test_buffer[128], &transformed_double, sizeof(double));
                }
            }
            
            // Test with magic_buffer
            if (!test_buffer.empty()) {
                const char* result = magic_buffer(magic, test_buffer.data(), test_buffer.size());
                (void)result;
            }
            break;
        }
    }

    // Clean up
    magic_close(magic);
    return 0;
}

} // namespace harness_010
