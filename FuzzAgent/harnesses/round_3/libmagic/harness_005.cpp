#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <cmath>
#include <cfloat>
#include "magic.h"

namespace harness_005 {
/*
 * Fuzzing harness for libmagic float/double magic type detection
 * This harness specifically targets the magiccheck function's float/double
 * comparison logic to unlock 160+ blocked branches identified in coverage analysis.
 * 
 * Targets: FILE_FLOAT, FILE_BEFLOAT, FILE_LEFLOAT (IEEE 754 float)
 *          FILE_DOUBLE, FILE_BEDOUBLE, FILE_LEDOUBLE (IEEE 754 double)
 * 
 * Strategies:
 * 1. Generate binary float/double data in various IEEE 754 formats
 * 2. Test different endianness variations (big, little, native)
 * 3. Exercise all relational operators (>, <, =, !, x) for float/double comparisons
 * 4. Include edge cases: NaN, Infinity, zero, denormalized numbers
 * 5. Test with MAGIC_DEBUG flag to enable debugging output for better coverage
 * 
 * Based on analysis of magiccheck() function in softmagic.c which shows
 * complex float/double comparison logic with isunordered(), isgreater(), isless()
 */


// Include libmagic headers

// Helper function to reinterpret float as bytes for IEEE 754 representation
template<typename T>
std::vector<uint8_t> float_to_bytes(T value) {
    std::vector<uint8_t> bytes(sizeof(T));
    memcpy(bytes.data(), &value, sizeof(T));
    return bytes;
}

// Helper to create test buffer with mixed float/double data
std::vector<uint8_t> create_mixed_float_double_buffer(FuzzedDataProvider& fdp, size_t max_size) {
    std::vector<uint8_t> buffer;
    
    // Determine how many float/double values to create
    size_t num_values = fdp.ConsumeIntegralInRange<size_t>(1, max_size / (sizeof(float) + sizeof(double)));
    
    for (size_t i = 0; i < num_values && buffer.size() < max_size; i++) {
        // Randomly choose between float and double
        bool use_float = fdp.ConsumeBool();
        
        if (use_float && (buffer.size() + sizeof(float)) <= max_size) {
            // Create a float value with various characteristics
            float f;
            uint32_t float_bits = fdp.ConsumeIntegral<uint32_t>();
            memcpy(&f, &float_bits, sizeof(float));
            
            // Special cases for edge values
            switch (fdp.ConsumeIntegralInRange(0, 7)) {
                case 0: f = 0.0f; break;
                case 1: f = -0.0f; break;
                case 2: f = INFINITY; break;
                case 3: f = -INFINITY; break;
                case 4: f = NAN; break;
                case 5: f = FLT_MIN; break;
                case 6: f = FLT_MAX; break;
                case 7: f = FLT_EPSILON; break;
            }
            
            auto bytes = float_to_bytes(f);
            buffer.insert(buffer.end(), bytes.begin(), bytes.end());
        } 
        else if ((buffer.size() + sizeof(double)) <= max_size) {
            // Create a double value with various characteristics
            double d;
            uint64_t double_bits = fdp.ConsumeIntegral<uint64_t>();
            memcpy(&d, &double_bits, sizeof(double));
            
            // Special cases for edge values
            switch (fdp.ConsumeIntegralInRange(0, 7)) {
                case 0: d = 0.0; break;
                case 1: d = -0.0; break;
                case 2: d = INFINITY; break;
                case 3: d = -INFINITY; break;
                case 4: d = NAN; break;
                case 5: d = DBL_MIN; break;
                case 6: d = DBL_MAX; break;
                case 7: d = DBL_EPSILON; break;
            }
            
            auto bytes = float_to_bytes(d);
            buffer.insert(buffer.end(), bytes.begin(), bytes.end());
        }
    }
    
    return buffer;
}

// Create buffer with specific endianness patterns for float/double
std::vector<uint8_t> create_endian_specific_buffer(FuzzedDataProvider& fdp, size_t max_size) {
    std::vector<uint8_t> buffer;
    
    // Create patterns that might match different endianness expectations
    // FILE_BEFLOAT, FILE_LEFLOAT, FILE_BEDOUBLE, FILE_LEDOUBLE
    
    // Pattern 1: Mixed endian float values
    for (int i = 0; i < 4 && buffer.size() + sizeof(float) <= max_size; i++) {
        float f = fdp.ConsumeFloatingPoint<float>();
        auto bytes = float_to_bytes(f);
        
        // Potentially reverse bytes for endianness testing
        if (fdp.ConsumeBool()) {
            std::reverse(bytes.begin(), bytes.end());
        }
        
        buffer.insert(buffer.end(), bytes.begin(), bytes.end());
    }
    
    // Pattern 2: Mixed endian double values
    for (int i = 0; i < 2 && buffer.size() + sizeof(double) <= max_size; i++) {
        double d = fdp.ConsumeFloatingPoint<double>();
        auto bytes = float_to_bytes(d);
        
        // Potentially reverse bytes for endianness testing
        if (fdp.ConsumeBool()) {
            std::reverse(bytes.begin(), bytes.end());
        }
        
        buffer.insert(buffer.end(), bytes.begin(), bytes.end());
    }
    
    return buffer;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum input for meaningful float/double testing
    // We need at least: flags (4 bytes) + some float/double data
    if (size < 12) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Test 1: magic_open with debugging flags to enable detailed output
    int flags = 0;
    
    // Always include MAGIC_DEBUG for better coverage of debug paths
    flags |= MAGIC_DEBUG;
    
    // Add other flags based on fuzzed input
    if (fdp.ConsumeBool()) flags |= MAGIC_SYMLINK;
    if (fdp.ConsumeBool()) flags |= MAGIC_COMPRESS;
    if (fdp.ConsumeBool()) flags |= MAGIC_DEVICES;
    if (fdp.ConsumeBool()) flags |= MAGIC_MIME_TYPE;
    if (fdp.ConsumeBool()) flags |= MAGIC_CONTINUE;
    if (fdp.ConsumeBool()) flags |= MAGIC_RAW;
    if (fdp.ConsumeBool()) flags |= MAGIC_ERROR;
    
    // Enable various NO_CHECK flags to reduce noise
    if (fdp.ConsumeBool()) flags |= MAGIC_NO_CHECK_COMPRESS;
    if (fdp.ConsumeBool()) flags |= MAGIC_NO_CHECK_TAR;
    if (fdp.ConsumeBool()) flags |= MAGIC_NO_CHECK_ELF;
    if (fdp.ConsumeBool()) flags |= MAGIC_NO_CHECK_TEXT;
    
    magic_t magic = magic_open(flags);
    if (magic == nullptr) {
        // Fallback to simple flags if complex flags fail
        magic = magic_open(MAGIC_DEBUG); // Still want debug
        if (magic == nullptr) {
            return 0; // Can't proceed without magic handle
        }
    }
    
    // Test 2: magic_version (should always work)
    int version = magic_version();
    (void)version; // Use to avoid unused variable warning
    
    // Test 3: Get and set flags
    int current_flags = magic_getflags(magic);
    (void)current_flags;
    
    // Try setting different flags from fuzzed input
    int new_flags = fdp.ConsumeIntegral<int>();
    // Ensure debug flag stays on for coverage
    new_flags |= MAGIC_DEBUG;
    magic_setflags(magic, new_flags);
    
    // Test 4: Create and test multiple buffer types for float/double detection
    
    // Strategy 1: Mixed float/double buffer with edge cases
    if (fdp.remaining_bytes() >= 16) {
        std::vector<uint8_t> buffer1 = create_mixed_float_double_buffer(fdp, 1024);
        if (!buffer1.empty()) {
            const char* result1 = magic_buffer(magic, buffer1.data(), buffer1.size());
            (void)result1; // Result may be null, that's okay
        }
    }
    
    // Strategy 2: Endian-specific patterns
    if (fdp.remaining_bytes() >= 8) {
        std::vector<uint8_t> buffer2 = create_endian_specific_buffer(fdp, 512);
        if (!buffer2.empty()) {
            const char* result2 = magic_buffer(magic, buffer2.data(), buffer2.size());
            (void)result2;
        }
    }
    
    // Strategy 3: Direct IEEE 754 representations
    // Create buffers with specific IEEE 754 patterns that might trigger
    // different relational operators in magiccheck()
    while (fdp.remaining_bytes() >= sizeof(float)) {
        size_t test_size = fdp.ConsumeIntegralInRange<size_t>(sizeof(float), 
            std::min(fdp.remaining_bytes(), static_cast<size_t>(256)));
        
        std::vector<uint8_t> test_data = fdp.ConsumeBytes<uint8_t>(test_size);
        
        // Ensure we have valid float/double aligned data
        if (test_data.size() >= sizeof(float)) {
            // Try to interpret as float pattern
            const char* result = magic_buffer(magic, test_data.data(), test_data.size());
            (void)result;
            
            // Also test with different alignments
            if (test_data.size() > sizeof(float)) {
                // Test with offset to catch alignment issues
                const char* result_offset = magic_buffer(magic, test_data.data() + 1, 
                    test_data.size() - 1);
                (void)result_offset;
            }
        }
    }
    
    // Strategy 4: Special NaN/Infinity patterns
    // These are important for testing isunordered() and special float comparisons
    std::vector<uint8_t> special_buffer;
    
    // Add various special float values
    float special_floats[] = {
        NAN, INFINITY, -INFINITY, 0.0f, -0.0f,
        FLT_MIN, FLT_MAX, FLT_EPSILON,
        1.0f, -1.0f, 2.0f, 0.5f
    };
    
    double special_doubles[] = {
        NAN, INFINITY, -INFINITY, 0.0, -0.0,
        DBL_MIN, DBL_MAX, DBL_EPSILON,
        1.0, -1.0, 2.0, 0.5
    };
    
    // Add some special floats
    for (float f : special_floats) {
        auto bytes = float_to_bytes(f);
        special_buffer.insert(special_buffer.end(), bytes.begin(), bytes.end());
    }
    
    // Add some special doubles
    for (double d : special_doubles) {
        auto bytes = float_to_bytes(d);
        special_buffer.insert(special_buffer.end(), bytes.begin(), bytes.end());
    }
    
    if (!special_buffer.empty()) {
        const char* special_result = magic_buffer(magic, special_buffer.data(), 
            special_buffer.size());
        (void)special_result;
    }
    
    // Clean up
    magic_close(magic);
    
    return 0;
}

} // namespace harness_005
