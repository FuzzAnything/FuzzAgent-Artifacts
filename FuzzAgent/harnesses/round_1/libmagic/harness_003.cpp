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
#include <unistd.h>
#include <stdio.h>

namespace harness_003 {
/*
 * Specialized fuzzing harness for libmagic targeting FILE_FLOAT/FILE_DOUBLE magic type checks
 * and apprentice.c parsing functions (magic database compilation pipeline)
 * 
 * Primary targets: 
 * - magiccheck() FILE_FLOAT/FILE_DOUBLE type comparison paths (152 blocked branches, 0% coverage)
 * - apprentice.c parsing functions (parse, getvalue, etc. with 0% coverage)
 * 
 * APIs targeted:
 * - magic_open, magic_compile, magic_check, magic_load_buffers, magic_buffer, magic_close
 * 
 * Strategy:
 * 1. Generate custom magic database strings containing float/double entries
 * 2. Validate and compile custom databases using magic_check/magic_compile
 * 3. Load compiled databases with magic_load_buffers
 * 4. Generate test buffers with IEEE 754 float/double values
 * 5. Test against custom databases using magic_buffer
 * 
 * Semantic differentiation from existing harnesses:
 * - harness_000: General API testing with default magic database
 * - harness_001: Parameter configuration functions
 * - harness_002: File/descriptor operations
 * - harness_003: Custom magic database compilation + float/double type checks (NEW)
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size for meaningful testing
    // Need: flags + operation + database size + test buffer size
    if (size < 64) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Consume fixed-size data first
    int flags = fdp.ConsumeIntegral<int>();
    int operation = fdp.ConsumeIntegralInRange<int>(0, 5); // 0-5 different testing strategies
    bool test_compile = fdp.ConsumeBool();
    bool test_load_buffers = fdp.ConsumeBool();
    bool test_all_float_types = fdp.ConsumeBool();

    // Build flags (limit to reasonable range)
    int actual_flags = flags & 0xFFFFF;
    
    // Create magic instance
    magic_t magic = magic_open(actual_flags);
    if (magic == nullptr) {
        return 0;
    }

    // Strategy selection
    switch (operation) {
        case 0: {
            // Strategy 1: Generate custom magic database with float/double entries
            // and test with magic_buffer
            
            // Generate magic database string with float/double entries
            std::string magic_db;
            
            // Add float entries with different relations
            const char* float_entries[] = {
                "0 float 3.14159",
                "0 float != 3.14159",
                "0 float > 1.0",
                "0 float < 10.0",
                "0 befloat 2.71828",
                "0 lefloat 1.41421",
                "4 float 0.0",
                "4 float -1.0",
                "4 float 1.0e10",
                "4 float 1.0e-10"
            };
            
            // Add double entries
            const char* double_entries[] = {
                "0 double 3.141592653589793",
                "0 double != 3.141592653589793",
                "0 double > 1.0",
                "0 double < 100.0",
                "0 bedouble 2.718281828459045",
                "0 ledouble 1.414213562373095",
                "8 double 0.0",
                "8 double -1.0",
                "8 double 1.0e100",
                "8 double 1.0e-100"
            };
            
            // Build magic database string
            for (const auto& entry : float_entries) {
                magic_db += entry;
                magic_db += "\n";
            }
            for (const auto& entry : double_entries) {
                magic_db += entry;
                magic_db += "\n";
            }
            
            // Add special IEEE 754 values
            if (test_all_float_types && fdp.ConsumeBool()) {
                magic_db += "0 float +NaN\n";
                magic_db += "0 float -NaN\n";
                magic_db += "0 float +Inf\n";
                magic_db += "0 float -Inf\n";
                magic_db += "0 double +NaN\n";
                magic_db += "0 double -NaN\n";
                magic_db += "0 double +Inf\n";
                magic_db += "0 double -Inf\n";
            }
            
            // Create temporary file for magic database
            std::string temp_filename = "/tmp/magic_fuzz_" + std::to_string(getpid()) + ".mgc";
            
            // Write magic database to temporary file
            FILE* fp = fopen(temp_filename.c_str(), "w");
            if (fp) {
                fwrite(magic_db.c_str(), 1, magic_db.size(), fp);
                fclose(fp);
                
                // Validate database syntax
                if (test_compile) {
                    magic_check(magic, temp_filename.c_str());
                }
                
                // Compile database
                magic_compile(magic, temp_filename.c_str());
                
                // Clean up temp file
                unlink(temp_filename.c_str());
            }
            
            // Generate test buffers with float/double values
            if (fdp.remaining_bytes() >= sizeof(float) * 4) {
                // Consume float values from input
                std::vector<float> float_values;
                std::vector<double> double_values;
                
                for (int i = 0; i < 4 && fdp.remaining_bytes() >= sizeof(float); i++) {
                    float val = fdp.ConsumeFloatingPoint<float>();
                    float_values.push_back(val);
                }
                
                for (int i = 0; i < 4 && fdp.remaining_bytes() >= sizeof(double); i++) {
                    double val = fdp.ConsumeFloatingPoint<double>();
                    double_values.push_back(val);
                }
                
                // Create test buffers
                if (!float_values.empty()) {
                    std::vector<uint8_t> float_buffer(sizeof(float) * float_values.size());
                    memcpy(float_buffer.data(), float_values.data(), float_buffer.size());
                    
                    const char* result = magic_buffer(magic, float_buffer.data(), float_buffer.size());
                    (void)result; // Result can be null
                }
                
                if (!double_values.empty()) {
                    std::vector<uint8_t> double_buffer(sizeof(double) * double_values.size());
                    memcpy(double_buffer.data(), double_values.data(), double_buffer.size());
                    
                    const char* result = magic_buffer(magic, double_buffer.data(), double_buffer.size());
                    (void)result; // Result can be null
                }
            }
            
            break;
        }
        
        case 1: {
            // Strategy 2: Test magic_load_buffers with custom compiled databases
            
            // Generate simple magic database in memory
            std::string magic_db = "0 float 3.14159\n0 double 2.71828\n";
            
            // Create buffers array for magic_load_buffers
            const size_t num_buffers = 1;
            void* buffers[num_buffers];
            size_t sizes[num_buffers];
            
            // Allocate and fill buffer
            sizes[0] = magic_db.size();
            buffers[0] = malloc(sizes[0]);
            if (buffers[0]) {
                memcpy(buffers[0], magic_db.c_str(), sizes[0]);
                
                if (test_load_buffers) {
                    // Try to load the database from memory buffers
                    magic_load_buffers(magic, buffers, sizes, num_buffers);
                }
                
                free(buffers[0]);
            }
            
            // Test with some float/double data
            if (fdp.remaining_bytes() >= sizeof(float) + sizeof(double)) {
                float fval = fdp.ConsumeFloatingPoint<float>();
                double dval = fdp.ConsumeFloatingPoint<double>();
                
                std::array<uint8_t, sizeof(float) + sizeof(double)> test_buffer;
                memcpy(test_buffer.data(), &fval, sizeof(float));
                memcpy(test_buffer.data() + sizeof(float), &dval, sizeof(double));
                
                const char* result = magic_buffer(magic, test_buffer.data(), test_buffer.size());
                (void)result;
            }
            
            break;
        }
        
        case 2: {
            // Strategy 3: Test edge cases and special float/double values
            
            // Create magic database with special float/double values
            std::string magic_db;
            
            // Add entries for special IEEE 754 values
            magic_db += "0 float 0.0\n";
            magic_db += "0 float -0.0\n";
            magic_db += "4 float 1.17549435e-38\n";  // FLT_MIN
            magic_db += "4 float 3.40282347e+38\n";  // FLT_MAX
            magic_db += "8 double 2.2250738585072014e-308\n";  // DBL_MIN
            magic_db += "8 double 1.7976931348623157e+308\n";  // DBL_MAX
            
            // Test with denormalized values
            if (fdp.ConsumeBool()) {
                magic_db += "0 float 1.0e-45\n";  // Denormal
                magic_db += "0 double 4.9e-324\n"; // Denormal
            }
            
            // Write to temp file and compile
            std::string temp_filename = "/tmp/magic_special_" + std::to_string(getpid()) + ".mgc";
            FILE* fp = fopen(temp_filename.c_str(), "w");
            if (fp) {
                fwrite(magic_db.c_str(), 1, magic_db.size(), fp);
                fclose(fp);
                
                magic_check(magic, temp_filename.c_str());
                magic_compile(magic, temp_filename.c_str());
                unlink(temp_filename.c_str());
            }
            
            // Generate test buffers with special values
            std::array<float, 8> special_floats = {
                0.0f, -0.0f, 1.0f, -1.0f,
                FLT_MIN, FLT_MAX,
                INFINITY, -INFINITY
            };
            
            std::array<double, 8> special_doubles = {
                0.0, -0.0, 1.0, -1.0,
                DBL_MIN, DBL_MAX,
                INFINITY, -INFINITY
            };
            
            // Test each special value
            for (size_t i = 0; i < special_floats.size(); i++) {
                std::array<uint8_t, sizeof(float)> buffer;
                memcpy(buffer.data(), &special_floats[i], sizeof(float));
                const char* result = magic_buffer(magic, buffer.data(), buffer.size());
                (void)result;
                
                // Also test at offset 4
                if (i < special_floats.size() / 2) {
                    std::array<uint8_t, sizeof(float) + 4> buffer_with_offset;
                    memset(buffer_with_offset.data(), 0, 4);
                    memcpy(buffer_with_offset.data() + 4, &special_floats[i], sizeof(float));
                    const char* result2 = magic_buffer(magic, buffer_with_offset.data(), buffer_with_offset.size());
                    (void)result2;
                }
            }
            
            break;
        }
        
        case 3: {
            // Strategy 4: Test different comparison relations for float/double
            
            // Create magic database with various comparison relations
            std::string magic_db;
            
            // Float comparisons
            magic_db += "0 float = 1.5\n";
            magic_db += "0 float != 1.5\n";
            magic_db += "0 float > 1.0\n";
            magic_db += "0 float < 2.0\n";
            
            // Double comparisons  
            magic_db += "0 double = 3.14159\n";
            magic_db += "0 double != 3.14159\n";
            magic_db += "0 double > 2.0\n";
            magic_db += "0 double < 4.0\n";
            
            // Endian-specific comparisons
            magic_db += "0 befloat = 1.5\n";
            magic_db += "0 lefloat = 1.5\n";
            magic_db += "0 bedouble = 3.14159\n";
            magic_db += "0 ledouble = 3.14159\n";
            
            // Write and compile
            std::string temp_filename = "/tmp/magic_relations_" + std::to_string(getpid()) + ".mgc";
            FILE* fp = fopen(temp_filename.c_str(), "w");
            if (fp) {
                fwrite(magic_db.c_str(), 1, magic_db.size(), fp);
                fclose(fp);
                
                magic_check(magic, temp_filename.c_str());
                magic_compile(magic, temp_filename.c_str());
                unlink(temp_filename.c_str());
            }
            
            // Generate test values from fuzzer input
            if (fdp.remaining_bytes() >= sizeof(float) * 2 + sizeof(double) * 2) {
                float f1 = fdp.ConsumeFloatingPoint<float>();
                float f2 = fdp.ConsumeFloatingPoint<float>();
                double d1 = fdp.ConsumeFloatingPoint<double>();
                double d2 = fdp.ConsumeFloatingPoint<double>();
                
                // Test float combinations
                std::array<uint8_t, sizeof(float)> f_buffer1;
                memcpy(f_buffer1.data(), &f1, sizeof(float));
                const char* result1 = magic_buffer(magic, f_buffer1.data(), f_buffer1.size());
                (void)result1;
                
                std::array<uint8_t, sizeof(float)> f_buffer2;
                memcpy(f_buffer2.data(), &f2, sizeof(float));
                const char* result2 = magic_buffer(magic, f_buffer2.data(), f_buffer2.size());
                (void)result2;
                
                // Test double combinations
                std::array<uint8_t, sizeof(double)> d_buffer1;
                memcpy(d_buffer1.data(), &d1, sizeof(double));
                const char* result3 = magic_buffer(magic, d_buffer1.data(), d_buffer1.size());
                (void)result3;
                
                std::array<uint8_t, sizeof(double)> d_buffer2;
                memcpy(d_buffer2.data(), &d2, sizeof(double));
                const char* result4 = magic_buffer(magic, d_buffer2.data(), d_buffer2.size());
                (void)result4;
            }
            
            break;
        }
        
        case 4: {
            // Strategy 5: Test apprentice.c parsing functions with malformed/incomplete databases
            
            // Generate various magic database strings to test parsing
            std::vector<std::string> test_dbs;
            
            // Valid database
            test_dbs.push_back("0 float 3.14159\n0 double 2.71828\n");
            
            // Database with errors (to test error handling in apprentice.c)
            test_dbs.push_back("0 float\n");  // Missing value
            test_dbs.push_back("float 3.14\n");  // Missing offset
            test_dbs.push_back("0 invalid_type 3.14\n");  // Invalid type
            test_dbs.push_back("999999999999 float 3.14\n");  // Huge offset
            
            // Test each database
            for (size_t i = 0; i < test_dbs.size() && i < 3; i++) {
                std::string temp_filename = "/tmp/magic_parse_" + std::to_string(getpid()) + "_" + std::to_string(i) + ".mgc";
                FILE* fp = fopen(temp_filename.c_str(), "w");
                if (fp) {
                    fwrite(test_dbs[i].c_str(), 1, test_dbs[i].size(), fp);
                    fclose(fp);
                    
                    // These may fail, but that's OK - we're testing error paths
                    magic_check(magic, temp_filename.c_str());
                    magic_compile(magic, temp_filename.c_str());
                    
                    unlink(temp_filename.c_str());
                }
            }
            
            break;
        }
        
        case 5: {
            // Strategy 6: Test combination of float/double with other magic types
            
            // Create mixed database
            std::string magic_db;
            
            // Mixed type entries
            magic_db += "0 string ABC\n";
            magic_db += "0 byte 0x42\n";
            magic_db += "0 short 0x1234\n";
            magic_db += "0 long 0x12345678\n";
            magic_db += "0 float 3.14159\n";
            magic_db += "0 double 2.71828\n";
            magic_db += "0 beshort 0x1234\n";
            magic_db += "0 belong 0x12345678\n";
            magic_db += "0 befloat 1.5\n";
            magic_db += "0 bedouble 3.14159\n";
            
            // Write and compile
            std::string temp_filename = "/tmp/magic_mixed_" + std::to_string(getpid()) + ".mgc";
            FILE* fp = fopen(temp_filename.c_str(), "w");
            if (fp) {
                fwrite(magic_db.c_str(), 1, magic_db.size(), fp);
                fclose(fp);
                
                magic_check(magic, temp_filename.c_str());
                magic_compile(magic, temp_filename.c_str());
                unlink(temp_filename.c_str());
            }
            
            // Generate mixed test data
            size_t test_size = fdp.ConsumeIntegralInRange<size_t>(1, 256);
            if (fdp.remaining_bytes() >= test_size) {
                std::vector<uint8_t> test_data = fdp.ConsumeBytes<uint8_t>(test_size);
                
                // Ensure we have enough space for float at offset 0 and double at offset 8
                if (test_data.size() >= 16) {
                    // Embed float and double at specific offsets
                    float fval = 3.14159f;
                    double dval = 2.71828;
                    
                    memcpy(test_data.data(), &fval, sizeof(float));
                    memcpy(test_data.data() + 8, &dval, sizeof(double));
                }
                
                const char* result = magic_buffer(magic, test_data.data(), test_data.size());
                (void)result;
            }
            
            break;
        }
    }

    // Clean up
    magic_close(magic);
    return 0;
}

} // namespace harness_003
