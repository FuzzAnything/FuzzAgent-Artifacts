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

namespace harness_008 {
/*
 * Fuzzing harness for libmagic float/double comparison logic in magiccheck function
 * This harness specifically targets FILE_FLOAT, FILE_BEFLOAT, FILE_LEFLOAT,
 * FILE_DOUBLE, FILE_BEDOUBLE, FILE_LEDOUBLE type comparisons with various relation
 * operators ('=', '!', '>', '<') to exercise 164 blocked branches in magiccheck.
 * 
 * Based on coverage analysis identifying completely uncovered float/double comparison
 * logic including edge cases: NaN, infinity, denormalized values, boundary conditions.
 */


// Include libmagic headers

// Include system headers for POSIX functions

// Helper to generate float special values
float get_special_float(int type) {
    union {
        uint32_t i;
        float f;
    } u;
    
    switch (type % 6) {
        case 0: return 0.0f;  // Zero
        case 1: return -0.0f; // Negative zero
        case 2: return std::numeric_limits<float>::infinity(); // Infinity
        case 3: return -std::numeric_limits<float>::infinity(); // -Infinity
        case 4: 
            u.i = 0x7fc00000; // NaN (quiet)
            return u.f;
        case 5: 
            u.i = 0x7f800001; // Signaling NaN
            return u.f;
        default:
            return 1.0f; // Normal value
    }
}

// Helper to generate double special values
double get_special_double(int type) {
    union {
        uint64_t i;
        double d;
    } u;
    
    switch (type % 6) {
        case 0: return 0.0;  // Zero
        case 1: return -0.0; // Negative zero
        case 2: return std::numeric_limits<double>::infinity(); // Infinity
        case 3: return -std::numeric_limits<double>::infinity(); // -Infinity
        case 4: 
            u.i = 0x7ff8000000000000ULL; // NaN (quiet)
            return u.d;
        case 5: 
            u.i = 0x7ff0000000000001ULL; // Signaling NaN
            return u.d;
        default:
            return 1.0; // Normal value
    }
}

// Helper to create magic pattern for float comparison
std::string create_float_magic_pattern(float value, int type, char relation) {
    char buffer[256];
    
    const char* type_str;
    switch (type % 3) {
        case 0: type_str = "float"; break;      // FILE_FLOAT
        case 1: type_str = "befloat"; break;    // FILE_BEFLOAT
        case 2: type_str = "lefloat"; break;    // FILE_LEFLOAT
        default: type_str = "float"; break;
    }
    
    snprintf(buffer, sizeof(buffer), "0 %s %c%g test float\n", type_str, relation, value);
    return std::string(buffer);
}

// Helper to create magic pattern for double comparison
std::string create_double_magic_pattern(double value, int type, char relation) {
    char buffer[256];
    
    const char* type_str;
    switch (type % 3) {
        case 0: type_str = "double"; break;     // FILE_DOUBLE
        case 1: type_str = "bedouble"; break;   // FILE_BEDOUBLE
        case 2: type_str = "ledouble"; break;   // FILE_LEDOUBLE
        default: type_str = "double"; break;
    }
    
    snprintf(buffer, sizeof(buffer), "0 %s %c%.17g test double\n", type_str, relation, value);
    return std::string(buffer);
}

// Helper to create test data with float value
std::vector<uint8_t> create_float_test_data(float value, int endian_type) {
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

// Helper to create test data with double value
std::vector<uint8_t> create_double_test_data(double value, int endian_type) {
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
    // Need sufficient input for float/double testing
    // We need: operation selection, float/double values, relation operators
    if (size < 20) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Create magic context
    magic_t magic = magic_open(MAGIC_NONE);
    if (magic == nullptr) {
        // Try with simple flags if MAGIC_NONE fails
        magic = magic_open(0);
        if (magic == nullptr) {
            return 0; // Can't proceed without magic handle
        }
    }
    
    // Consume configuration from fuzzed input
    int test_type = fdp.ConsumeIntegralInRange<int>(0, 3); // 0: float, 1: double, 2: mixed
    int num_tests = fdp.ConsumeIntegralInRange<int>(1, 10);
    int use_flags = fdp.ConsumeBool();
    
    // Optionally set flags for float/double handling
    if (use_flags && fdp.remaining_bytes() > 0) {
        int flags = fdp.ConsumeIntegral<int>();
        magic_setflags(magic, flags);
    }
    
    // Build magic database with float/double comparison patterns
    std::string magic_db;
    
    // Add float patterns
    for (int i = 0; i < num_tests && fdp.remaining_bytes() > 4; i++) {
        // Determine relation operator
        char relations[] = {'=', '!', '>', '<', 'x'};
        char relation = relations[fdp.ConsumeIntegralInRange<int>(0, 4)];
        
        // Generate float value
        float float_val;
        if (fdp.remaining_bytes() >= 4) {
            // Consume float from input
            uint32_t float_bits = fdp.ConsumeIntegral<uint32_t>();
            memcpy(&float_val, &float_bits, 4);
            
            // Add some special values from our helper
            if (i % 3 == 0) {
                float_val = get_special_float(fdp.ConsumeIntegralInRange<int>(0, 5));
            }
        } else {
            float_val = get_special_float(i);
        }
        
        // Choose float type
        int float_type = fdp.ConsumeIntegralInRange<int>(0, 2); // 0: float, 1: befloat, 2: lefloat
        
        // Add to magic database
        magic_db += create_float_magic_pattern(float_val, float_type, relation);
    }
    
    // Add double patterns
    for (int i = 0; i < num_tests && fdp.remaining_bytes() > 8; i++) {
        // Determine relation operator
        char relations[] = {'=', '!', '>', '<', 'x'};
        char relation = relations[fdp.ConsumeIntegralInRange<int>(0, 4)];
        
        // Generate double value
        double double_val;
        if (fdp.remaining_bytes() >= 8) {
            // Consume double from input
            uint64_t double_bits = fdp.ConsumeIntegral<uint64_t>();
            memcpy(&double_val, &double_bits, 8);
            
            // Add some special values from our helper
            if (i % 3 == 0) {
                double_val = get_special_double(fdp.ConsumeIntegralInRange<int>(0, 5));
            }
        } else {
            double_val = get_special_double(i);
        }
        
        // Choose double type
        int double_type = fdp.ConsumeIntegralInRange<int>(0, 2); // 0: double, 1: bedouble, 2: ledouble
        
        // Add to magic database
        magic_db += create_double_magic_pattern(double_val, double_type, relation);
    }
    
    // Write magic database to temporary file
    char magic_template[] = "/tmp/magic_float_fuzz_XXXXXX";
    int magic_fd = mkstemp(magic_template);
    if (magic_fd != -1) {
        write(magic_fd, magic_db.c_str(), magic_db.length());
        fsync(magic_fd);
        close(magic_fd);
        
        // Load the magic database
        if (magic_load(magic, magic_template) == -1) {
            // If load fails, we can still test with default database
            // but float/double patterns won't be available
        }
        
        // Clean up temporary file
        unlink(magic_template);
    }
    
    // Generate test data with float/double values and test with magic_buffer
    while (fdp.remaining_bytes() > 8) {
        // Decide what type of data to test
        int data_type = fdp.ConsumeIntegralInRange<int>(0, 3);
        
        std::vector<uint8_t> test_data;
        
        if (data_type == 0 && fdp.remaining_bytes() >= 4) {
            // Test float data
            float test_float;
            uint32_t float_bits = fdp.ConsumeIntegral<uint32_t>();
            memcpy(&test_float, &float_bits, 4);
            
            // Mix in special values
            if (fdp.ConsumeBool()) {
                test_float = get_special_float(fdp.ConsumeIntegralInRange<int>(0, 5));
            }
            
            int endian_type = fdp.ConsumeIntegralInRange<int>(0, 2);
            test_data = create_float_test_data(test_float, endian_type);
            
        } else if (data_type == 1 && fdp.remaining_bytes() >= 8) {
            // Test double data
            double test_double;
            uint64_t double_bits = fdp.ConsumeIntegral<uint64_t>();
            memcpy(&test_double, &double_bits, 8);
            
            // Mix in special values
            if (fdp.ConsumeBool()) {
                test_double = get_special_double(fdp.ConsumeIntegralInRange<int>(0, 5));
            }
            
            int endian_type = fdp.ConsumeIntegralInRange<int>(0, 2);
            test_data = create_double_test_data(test_double, endian_type);
            
        } else {
            // Test mixed data with both float and double
            if (fdp.remaining_bytes() >= 12) {
                test_data.resize(12);
                
                // First 4 bytes: float
                float test_float;
                uint32_t float_bits = fdp.ConsumeIntegral<uint32_t>();
                memcpy(&test_float, &float_bits, 4);
                std::vector<uint8_t> float_part = create_float_test_data(test_float, 0);
                memcpy(test_data.data(), float_part.data(), 4);
                
                // Next 8 bytes: double
                double test_double;
                uint64_t double_bits = fdp.ConsumeIntegral<uint64_t>();
                memcpy(&test_double, &double_bits, 8);
                std::vector<uint8_t> double_part = create_double_test_data(test_double, 0);
                memcpy(test_data.data() + 4, double_part.data(), 8);
            } else {
                break; // Not enough data
            }
        }
        
        // Test the data with magic_buffer
        if (!test_data.empty()) {
            const char* result = magic_buffer(magic, test_data.data(), test_data.size());
            (void)result; // Use to avoid unused variable warning
            
            // Check for errors
            const char* error = magic_error(magic);
            (void)error;
            
            // Test magic_errno
            int err = magic_errno(magic);
            (void)err;
        }
    }
    
    // Additional tests with boundary conditions
    // Test with very small (denormalized) values
    if (fdp.remaining_bytes() > 0) {
        std::vector<uint8_t> denormal_test;
        
        // Test with float denormalized value
        union {
            uint32_t i;
            float f;
        } denormal_float;
        denormal_float.i = 0x00000001; // Smallest positive denormalized float
        
        denormal_test = create_float_test_data(denormal_float.f, 0);
        magic_buffer(magic, denormal_test.data(), denormal_test.size());
        
        // Test with double denormalized value
        union {
            uint64_t i;
            double d;
        } denormal_double;
        denormal_double.i = 0x0000000000000001ULL; // Smallest positive denormalized double
        
        std::vector<uint8_t> denormal_double_test = create_double_test_data(denormal_double.d, 0);
        magic_buffer(magic, denormal_double_test.data(), denormal_double_test.size());
    }
    
    // Test with different flag combinations to affect float/double handling
    if (fdp.remaining_bytes() > 0) {
        for (int i = 0; i < 3 && fdp.remaining_bytes() > 0; i++) {
            int test_flags = fdp.ConsumeIntegral<int>();
            magic_setflags(magic, test_flags);
            
            // Quick test with a small piece of data
            if (fdp.remaining_bytes() > 4) {
                std::vector<uint8_t> quick_test = create_float_test_data(
                    get_special_float(i), 
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

} // namespace harness_008
