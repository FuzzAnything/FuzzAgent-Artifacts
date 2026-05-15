#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <cmath>
#include <cfloat>
#include <limits>
#include "magic.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

namespace harness_009 {
/*
 * Fuzzing harness for libmagic focusing specifically on 'x' (any/always match) 
 * relation operator branches in magiccheck function's float/double comparison logic
 * 
 * This harness targets the 199 blocked branches identified by CoverageAnalyzerAgent,
 * with specific focus on the 'x' relation operator cases for FILE_FLOAT, FILE_BEFLOAT,
 * FILE_LEFLOAT, FILE_DOUBLE, FILE_BEDOUBLE, FILE_LEDOUBLE types.
 * 
 * Differentiation from harness_008.cpp:
 * - harness_008 targets all relation operators (=, !, >, <, x) with mixed patterns
 * - This harness focuses EXCLUSIVELY on 'x' operator to maximize coverage of those
 *   specific 199 blocked branches
 * - Uses specialized test data generation for edge cases around 'x' operator semantics
 * - Tests 'x' operator with special float/double values (NaN, infinity, denormalized)
 * - Tests 'x' operator with different endianness configurations
 * - Tests 'x' operator with various magic database loading strategies
 */


// Include libmagic headers

// Include system headers for POSIX functions

// Helper to generate comprehensive float special values for 'x' operator testing
float get_x_operator_float(int index) {
    union {
        uint32_t i;
        float f;
    } u;
    
    // Generate different types of float values that should match with 'x' operator
    switch (index % 12) {
        case 0: return 0.0f;                     // Positive zero
        case 1: return -0.0f;                    // Negative zero  
        case 2: return std::numeric_limits<float>::infinity(); // +Infinity
        case 3: return -std::numeric_limits<float>::infinity(); // -Infinity
        case 4:                                  // Quiet NaN
            u.i = 0x7fc00000;
            return u.f;
        case 5:                                  // Signaling NaN
            u.i = 0x7f800001;
            return u.f;
        case 6:                                  // Smallest positive denormalized
            u.i = 0x00000001;
            return u.f;
        case 7:                                  // Largest positive denormalized
            u.i = 0x007fffff;
            return u.f;
        case 8:                                  // Smallest positive normalized
            u.i = 0x00800000;
            return u.f;
        case 9:                                  // Largest positive normalized
            u.i = 0x7f7fffff;
            return u.f;
        case 10:                                 // Random value 1
            return 1.23456f;
        case 11:                                 // Random value 2
            return -3.14159f;
        default:
            return 1.0f;
    }
}

// Helper to generate comprehensive double special values for 'x' operator testing
double get_x_operator_double(int index) {
    union {
        uint64_t i;
        double d;
    } u;
    
    // Generate different types of double values that should match with 'x' operator
    switch (index % 12) {
        case 0: return 0.0;                      // Positive zero
        case 1: return -0.0;                     // Negative zero  
        case 2: return std::numeric_limits<double>::infinity(); // +Infinity
        case 3: return -std::numeric_limits<double>::infinity(); // -Infinity
        case 4:                                  // Quiet NaN
            u.i = 0x7ff8000000000000ULL;
            return u.d;
        case 5:                                  // Signaling NaN
            u.i = 0x7ff0000000000001ULL;
            return u.d;
        case 6:                                  // Smallest positive denormalized
            u.i = 0x0000000000000001ULL;
            return u.d;
        case 7:                                  // Largest positive denormalized
            u.i = 0x000fffffffffffffULL;
            return u.d;
        case 8:                                  // Smallest positive normalized
            u.i = 0x0010000000000000ULL;
            return u.d;
        case 9:                                  // Largest positive normalized
            u.i = 0x7fefffffffffffffULL;
            return u.d;
        case 10:                                 // Random value 1
            return 1.23456789012345;
        case 11:                                 // Random value 2
            return -3.14159265358979;
        default:
            return 1.0;
    }
}

// Create magic pattern for float with 'x' operator
std::string create_x_float_magic_pattern(float value, int type) {
    char buffer[256];
    
    const char* type_str;
    switch (type % 3) {
        case 0: type_str = "float"; break;      // FILE_FLOAT
        case 1: type_str = "befloat"; break;    // FILE_BEFLOAT
        case 2: type_str = "lefloat"; break;    // FILE_LEFLOAT
        default: type_str = "float"; break;
    }
    
    // Always use 'x' relation operator for this harness
    snprintf(buffer, sizeof(buffer), "0 %s x%g test_float_x_operator\n", type_str, value);
    return std::string(buffer);
}

// Create magic pattern for double with 'x' operator
std::string create_x_double_magic_pattern(double value, int type) {
    char buffer[256];
    
    const char* type_str;
    switch (type % 3) {
        case 0: type_str = "double"; break;     // FILE_DOUBLE
        case 1: type_str = "bedouble"; break;   // FILE_BEDOUBLE
        case 2: type_str = "ledouble"; break;   // FILE_LEDOUBLE
        default: type_str = "double"; break;
    }
    
    // Always use 'x' relation operator for this harness
    snprintf(buffer, sizeof(buffer), "0 %s x%.17g test_double_x_operator\n", type_str, value);
    return std::string(buffer);
}

// Create test data with float value for 'x' operator testing
std::vector<uint8_t> create_x_float_test_data(float value, int endian_type) {
    std::vector<uint8_t> data(4, 0);
    union {
        float f;
        uint32_t i;
        uint8_t bytes[4];
    } u;
    
    u.f = value;
    
    if (endian_type == 0) { // Native/little-endian for FILE_FLOAT
        memcpy(data.data(), &u.i, 4);
    } else if (endian_type == 1) { // Big-endian for FILE_BEFLOAT
        for (int i = 0; i < 4; i++) {
            data[i] = u.bytes[3 - i];
        }
    } else { // Little-endian for FILE_LEFLOAT (already little-endian)
        memcpy(data.data(), &u.i, 4);
    }
    
    return data;
}

// Create test data with double value for 'x' operator testing
std::vector<uint8_t> create_x_double_test_data(double value, int endian_type) {
    std::vector<uint8_t> data(8, 0);
    union {
        double d;
        uint64_t i;
        uint8_t bytes[8];
    } u;
    
    u.d = value;
    
    if (endian_type == 0) { // Native/little-endian for FILE_DOUBLE
        memcpy(data.data(), &u.i, 8);
    } else if (endian_type == 1) { // Big-endian for FILE_BEDOUBLE
        for (int i = 0; i < 8; i++) {
            data[i] = u.bytes[7 - i];
        }
    } else { // Little-endian for FILE_LEDOUBLE (already little-endian)
        memcpy(data.data(), &u.i, 8);
    }
    
    return data;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input required: operation selection + at least one float/double test
    if (size < 12) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Create magic context with different flags to test 'x' operator behavior
    int flags = MAGIC_NONE;
    if (fdp.ConsumeBool()) flags |= MAGIC_CONTINUE;
    if (fdp.ConsumeBool()) flags |= MAGIC_ERROR;
    
    magic_t magic = magic_open(flags);
    if (magic == nullptr) {
        // Try with simple flags if complex flags fail
        magic = magic_open(MAGIC_NONE);
        if (magic == nullptr) {
            return 0; // Can't proceed without magic handle
        }
    }
    
    // Consume configuration from fuzzed input
    int test_strategy = fdp.ConsumeIntegralInRange<int>(0, 2); // 0: float only, 1: double only, 2: mixed
    int num_patterns = fdp.ConsumeIntegralInRange<int>(1, 8);
    
    // Build magic database EXCLUSIVELY with 'x' operator patterns
    std::string magic_db;
    
    // Add float patterns with 'x' operator
    for (int i = 0; i < num_patterns && fdp.remaining_bytes() > 4; i++) {
        // Generate float value from fuzzed input or helper
        float float_val;
        if (fdp.remaining_bytes() >= 4) {
            uint32_t float_bits = fdp.ConsumeIntegral<uint32_t>();
            memcpy(&float_val, &float_bits, 4);
        } else {
            float_val = get_x_operator_float(i);
        }
        
        // Choose float type
        int float_type = fdp.ConsumeIntegralInRange<int>(0, 2); // 0: float, 1: befloat, 2: lefloat
        
        // Add 'x' operator pattern to magic database
        magic_db += create_x_float_magic_pattern(float_val, float_type);
    }
    
    // Add double patterns with 'x' operator
    for (int i = 0; i < num_patterns && fdp.remaining_bytes() > 8; i++) {
        // Generate double value from fuzzed input or helper
        double double_val;
        if (fdp.remaining_bytes() >= 8) {
            uint64_t double_bits = fdp.ConsumeIntegral<uint64_t>();
            memcpy(&double_val, &double_bits, 8);
        } else {
            double_val = get_x_operator_double(i);
        }
        
        // Choose double type
        int double_type = fdp.ConsumeIntegralInRange<int>(0, 2); // 0: double, 1: bedouble, 2: ledouble
        
        // Add 'x' operator pattern to magic database
        magic_db += create_x_double_magic_pattern(double_val, double_type);
    }
    
    // Write magic database to temporary file
    char magic_template[] = "/tmp/magic_x_operator_fuzz_XXXXXX";
    int magic_fd = mkstemp(magic_template);
    if (magic_fd != -1) {
        write(magic_fd, magic_db.c_str(), magic_db.length());
        fsync(magic_fd);
        close(magic_fd);
        
        // Load the magic database with 'x' operator patterns
        if (magic_load(magic, magic_template) == -1) {
            // If load fails, we can still test with default database
        }
        
        // Clean up temporary file
        unlink(magic_template);
    }
    
    // Test 'x' operator behavior with various float/double values
    while (fdp.remaining_bytes() > 8) {
        // Decide what type of data to test based on strategy
        int data_type;
        if (test_strategy == 0) {
            data_type = 0; // Float only
        } else if (test_strategy == 1) {
            data_type = 1; // Double only
        } else {
            data_type = fdp.ConsumeIntegralInRange<int>(0, 1); // Mixed
        }
        
        std::vector<uint8_t> test_data;
        
        if (data_type == 0 && fdp.remaining_bytes() >= 4) {
            // Test float data with 'x' operator
            float test_float;
            if (fdp.ConsumeBool()) {
                // Use special float value for 'x' operator testing
                test_float = get_x_operator_float(fdp.ConsumeIntegralInRange<int>(0, 11));
            } else {
                // Consume float from fuzzed input
                uint32_t float_bits = fdp.ConsumeIntegral<uint32_t>();
                memcpy(&test_float, &float_bits, 4);
            }
            
            int endian_type = fdp.ConsumeIntegralInRange<int>(0, 2);
            test_data = create_x_float_test_data(test_float, endian_type);
            
        } else if (data_type == 1 && fdp.remaining_bytes() >= 8) {
            // Test double data with 'x' operator
            double test_double;
            if (fdp.ConsumeBool()) {
                // Use special double value for 'x' operator testing
                test_double = get_x_operator_double(fdp.ConsumeIntegralInRange<int>(0, 11));
            } else {
                // Consume double from fuzzed input
                uint64_t double_bits = fdp.ConsumeIntegral<uint64_t>();
                memcpy(&test_double, &double_bits, 8);
            }
            
            int endian_type = fdp.ConsumeIntegralInRange<int>(0, 2);
            test_data = create_x_double_test_data(test_double, endian_type);
        } else {
            break; // Not enough data
        }
        
        // Test the data with magic_buffer - 'x' operator should always match
        if (!test_data.empty()) {
            const char* result = magic_buffer(magic, test_data.data(), test_data.size());
            (void)result; // Use to avoid unused variable warning
            
            // Check for errors (should not have errors with 'x' operator)
            const char* error = magic_error(magic);
            (void)error;
            
            // Test magic_errno
            int err = magic_errno(magic);
            (void)err;
        }
    }
    
    // Special test cases for 'x' operator edge conditions
    // Test 1: Empty data with 'x' operator patterns
    if (fdp.remaining_bytes() > 0) {
        std::vector<uint8_t> empty_data;
        magic_buffer(magic, empty_data.data(), 0);
    }
    
    // Test 2: Single byte data with 'x' operator patterns
    if (fdp.remaining_bytes() > 0) {
        std::vector<uint8_t> single_byte(1, fdp.ConsumeIntegral<uint8_t>());
        magic_buffer(magic, single_byte.data(), single_byte.size());
    }
    
    // Test 3: Very small float/double values with 'x' operator
    if (fdp.remaining_bytes() > 0) {
        // Test denormalized float
        union {
            uint32_t i;
            float f;
        } tiny_float;
        tiny_float.i = 0x00000001; // Smallest positive denormalized float
        std::vector<uint8_t> tiny_float_data = create_x_float_test_data(tiny_float.f, 0);
        magic_buffer(magic, tiny_float_data.data(), tiny_float_data.size());
    }
    
    // Test 4: NaN values with 'x' operator
    if (fdp.remaining_bytes() > 0) {
        union {
            uint32_t i;
            float f;
        } nan_float;
        nan_float.i = 0x7fc00000; // Quiet NaN
        
        std::vector<uint8_t> nan_float_data = create_x_float_test_data(nan_float.f, 0);
        magic_buffer(magic, nan_float_data.data(), nan_float_data.size());
    }
    
    // Test 5: Infinity values with 'x' operator
    if (fdp.remaining_bytes() > 0) {
        float inf_float = std::numeric_limits<float>::infinity();
        std::vector<uint8_t> inf_float_data = create_x_float_test_data(inf_float, 0);
        magic_buffer(magic, inf_float_data.data(), inf_float_data.size());
    }
    
    // Test different flag combinations with 'x' operator
    if (fdp.remaining_bytes() > 0) {
        for (int i = 0; i < 3 && fdp.remaining_bytes() > 0; i++) {
            int test_flags = fdp.ConsumeIntegral<int>();
            magic_setflags(magic, test_flags);
            
            // Quick test with a float value
            if (fdp.remaining_bytes() > 4) {
                std::vector<uint8_t> quick_test = create_x_float_test_data(
                    get_x_operator_float(i), 
                    fdp.ConsumeIntegralInRange<int>(0, 2)
                );
                magic_buffer(magic, quick_test.data(), quick_test.size());
            }
        }
    }
    
    // Clean up
    magic_close(magic);
    
    return 0;
}

} // namespace harness_009
