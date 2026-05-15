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
#include <algorithm>

namespace harness_009 {
/*
 * Specialized fuzzing harness for libmagic targeting magiccheck() floating-point
 * comparison logic via dynamic magic rule compilation.
 * 
 * Primary targets:
 * - magiccheck() FILE_FLOAT, FILE_BEFLOAT, FILE_LEFLOAT, FILE_DOUBLE, 
 *   FILE_BEDOUBLE, FILE_LEDOUBLE type comparison paths (152 blocked branches)
 * - Dynamic magic rule compilation using magic_compile and magic_check APIs
 * 
 * APIs targeted:
 * - magic_open, magic_compile, magic_check, magic_buffer, magic_close
 * 
 * Strategy:
 * 1. Use fuzzer input to generate dynamic magic rules with float/double types
 * 2. Test all comparison relations: 'x' (always match), '!' (not equal), 
 *    '=' (equal), '>' (greater than), '<' (less than)
 * 3. Generate corresponding test data from remaining fuzzer input
 * 4. Compile and validate rules using magic_compile and magic_check
 * 5. Test compiled rules against generated data using magic_buffer
 * 
 * Semantic differentiation from existing harnesses:
 * - harness_003: Custom magic database compilation + float/double (has hardcoded values)
 * - harness_004: Focused float comparison branch testing with custom magic db
 * - harness_007: Direct float/double/string data generation for magiccheck()
 * - harness_008: Comprehensive float/double edge cases with all relations
 * - harness_009: Dynamic rule compilation from fuzzer input + all relations + IEEE 754 specials (NEW)
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size for meaningful testing
    // Need: flags + operation + float/double values for rules + test data
    if (size < 256) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Consume fixed-size data first
    int flags = fdp.ConsumeIntegral<int>();
    int strategy = fdp.ConsumeIntegralInRange<int>(0, 3); // 0-3 different strategies
    bool test_all_relations = fdp.ConsumeBool();
    bool include_special_values = fdp.ConsumeBool();
    bool use_magic_check = fdp.ConsumeBool();
    
    // Build flags (limit to reasonable range)
    int actual_flags = flags & 0xFFFFF;
    
    // Create magic instance
    magic_t magic = magic_open(actual_flags);
    if (magic == nullptr) {
        return 0;
    }

    // Strategy selection
    switch (strategy) {
        case 0: {
            // Strategy 0: Dynamic float rule generation with all relations
            std::string magic_db;
            
            // Determine how many float rules to generate from input
            int num_float_rules = fdp.ConsumeIntegralInRange<int>(1, 10);
            
            for (int i = 0; i < num_float_rules; i++) {
                // Need at least: offset + float value + relation type
                if (fdp.remaining_bytes() < sizeof(uint32_t) + sizeof(float) + 1) {
                    break;
                }
                
                // Consume offset
                uint32_t offset = fdp.ConsumeIntegral<uint32_t>() % 256;
                
                // Consume float value for rule
                float rule_value = fdp.ConsumeFloatingPoint<float>();
                
                // Determine relation type from input
                uint8_t rel_type = fdp.ConsumeIntegral<uint8_t>() % 5;
                char relation = 'x'; // default: always match
                switch (rel_type) {
                    case 0: relation = 'x'; break; // always match
                    case 1: relation = '!'; break; // not equal
                    case 2: relation = '='; break; // equal
                    case 3: relation = '>'; break; // greater than
                    case 4: relation = '<'; break; // less than
                }
                
                // Determine float type (regular, big-endian, little-endian)
                uint8_t float_type = fdp.ConsumeIntegral<uint8_t>() % 3;
                const char* type_str = "float";
                switch (float_type) {
                    case 0: type_str = "float"; break;
                    case 1: type_str = "befloat"; break;
                    case 2: type_str = "lefloat"; break;
                }
                
                // Build rule string
                magic_db += std::to_string(offset) + " " + type_str + " ";
                if (relation != 'x') {
                    magic_db += relation;
                    magic_db += " ";
                }
                magic_db += std::to_string(rule_value) + "\n";
            }
            
            // Write dynamic magic database to temp file
            std::string temp_filename = "/tmp/magic_dynamic_" + std::to_string(getpid()) + ".mgc";
            FILE* fp = fopen(temp_filename.c_str(), "w");
            if (fp) {
                fwrite(magic_db.c_str(), 1, magic_db.size(), fp);
                fclose(fp);
                
                // Validate database syntax if requested
                if (use_magic_check) {
                    magic_check(magic, temp_filename.c_str());
                }
                
                // Compile database
                magic_compile(magic, temp_filename.c_str());
                
                // Clean up temp file
                unlink(temp_filename.c_str());
            }
            
            // Generate test data from remaining input
            size_t test_size = fdp.ConsumeIntegralInRange<size_t>(1, 1024);
            if (fdp.remaining_bytes() >= test_size) {
                std::vector<uint8_t> test_data = fdp.ConsumeBytes<uint8_t>(test_size);
                
                // Test with magic_buffer
                const char* result = magic_buffer(magic, test_data.data(), test_data.size());
                (void)result; // Result can be null
            }
            
            break;
        }
        
        case 1: {
            // Strategy 1: Mixed float and double rules with IEEE 754 special values
            std::string magic_db;
            
            // Generate rules with special IEEE 754 values
            const char* special_names[] = {"+NaN", "-NaN", "+Inf", "-Inf", "0.0", "-0.0"};
            
            // Add float rules with special values
            for (int i = 0; i < 6 && fdp.remaining_bytes() >= sizeof(uint32_t); i++) {
                uint32_t offset = fdp.ConsumeIntegral<uint32_t>() % 128;
                uint8_t special_idx = fdp.ConsumeIntegral<uint8_t>() % 6;
                uint8_t rel_type = fdp.ConsumeIntegral<uint8_t>() % 3; // fewer relations for specials
                
                char relation = 'x';
                switch (rel_type) {
                    case 0: relation = 'x'; break;
                    case 1: relation = '='; break;
                    case 2: relation = '!'; break;
                }
                
                // Float type
                uint8_t float_type = fdp.ConsumeIntegral<uint8_t>() % 3;
                const char* type_str = "float";
                switch (float_type) {
                    case 0: type_str = "float"; break;
                    case 1: type_str = "befloat"; break;
                    case 2: type_str = "lefloat"; break;
                }
                
                magic_db += std::to_string(offset) + " " + type_str + " ";
                if (relation != 'x') {
                    magic_db += relation;
                    magic_db += " ";
                }
                magic_db += std::string(special_names[special_idx]) + "\n";
            }
            
            // Add double rules
            for (int i = 0; i < 4 && fdp.remaining_bytes() >= sizeof(uint32_t) + sizeof(double); i++) {
                uint32_t offset = fdp.ConsumeIntegral<uint32_t>() % 256;
                double rule_value = fdp.ConsumeFloatingPoint<double>();
                uint8_t rel_type = fdp.ConsumeIntegral<uint8_t>() % 5;
                
                char relation = 'x';
                switch (rel_type) {
                    case 0: relation = 'x'; break;
                    case 1: relation = '!'; break;
                    case 2: relation = '='; break;
                    case 3: relation = '>'; break;
                    case 4: relation = '<'; break;
                }
                
                // Double type
                uint8_t double_type = fdp.ConsumeIntegral<uint8_t>() % 3;
                const char* type_str = "double";
                switch (double_type) {
                    case 0: type_str = "double"; break;
                    case 1: type_str = "bedouble"; break;
                    case 2: type_str = "ledouble"; break;
                }
                
                magic_db += std::to_string(offset) + " " + type_str + " ";
                if (relation != 'x') {
                    magic_db += relation;
                    magic_db += " ";
                }
                magic_db += std::to_string(rule_value) + "\n";
            }
            
            // Write and compile
            std::string temp_filename = "/tmp/magic_special_" + std::to_string(getpid()) + ".mgc";
            FILE* fp = fopen(temp_filename.c_str(), "w");
            if (fp) {
                fwrite(magic_db.c_str(), 1, magic_db.size(), fp);
                fclose(fp);
                
                if (use_magic_check) {
                    magic_check(magic, temp_filename.c_str());
                }
                
                magic_compile(magic, temp_filename.c_str());
                unlink(temp_filename.c_str());
            }
            
            // Generate test data with special IEEE 754 values
            size_t buffer_size = fdp.ConsumeIntegralInRange<size_t>(256, 2048);
            if (fdp.remaining_bytes() >= buffer_size) {
                std::vector<uint8_t> test_buffer = fdp.ConsumeBytes<uint8_t>(buffer_size);
                
                // Insert some special float/double values at known offsets
                if (test_buffer.size() >= 64) {
                    // Insert NaN at offset 0
                    float nan_val = NAN;
                    memcpy(test_buffer.data(), &nan_val, sizeof(float));
                    
                    // Insert Inf at offset 8
                    float inf_val = INFINITY;
                    memcpy(test_buffer.data() + 8, &inf_val, sizeof(float));
                    
                    // Insert -Inf at offset 16
                    float neg_inf_val = -INFINITY;
                    memcpy(test_buffer.data() + 16, &neg_inf_val, sizeof(float));
                    
                    // Insert -0.0 at offset 24
                    float neg_zero = -0.0f;
                    memcpy(test_buffer.data() + 24, &neg_zero, sizeof(float));
                }
                
                const char* result = magic_buffer(magic, test_buffer.data(), test_buffer.size());
                (void)result;
            }
            
            break;
        }
        
        case 2: {
            // Strategy 2: Comprehensive rule testing with offset chains
            std::string magic_db;
            
            // Create a chain of float/double rules at increasing offsets
            int chain_length = fdp.ConsumeIntegralInRange<int>(3, 8);
            
            for (int i = 0; i < chain_length; i++) {
                // Need: offset + value + type
                if (fdp.remaining_bytes() < sizeof(uint32_t) + sizeof(float)) {
                    break;
                }
                
                uint32_t base_offset = fdp.ConsumeIntegral<uint32_t>() % 64;
                uint32_t offset = base_offset + (i * 8); // Chain offsets
                
                // Alternate between float and double
                if (i % 2 == 0 && fdp.remaining_bytes() >= sizeof(float)) {
                    float rule_value = fdp.ConsumeFloatingPoint<float>();
                    uint8_t rel_type = fdp.ConsumeIntegral<uint8_t>() % 5;
                    
                    char relation = 'x';
                    switch (rel_type) {
                        case 0: relation = 'x'; break;
                        case 1: relation = '!'; break;
                        case 2: relation = '='; break;
                        case 3: relation = '>'; break;
                        case 4: relation = '<'; break;
                    }
                    
                    // Vary float types in chain
                    const char* type_str = (i % 3 == 0) ? "float" : 
                                          (i % 3 == 1) ? "befloat" : "lefloat";
                    
                    magic_db += std::to_string(offset) + " " + type_str + " ";
                    if (relation != 'x') {
                        magic_db += relation;
                        magic_db += " ";
                    }
                    magic_db += std::to_string(rule_value) + "\n";
                } else if (fdp.remaining_bytes() >= sizeof(double)) {
                    double rule_value = fdp.ConsumeFloatingPoint<double>();
                    uint8_t rel_type = fdp.ConsumeIntegral<uint8_t>() % 5;
                    
                    char relation = 'x';
                    switch (rel_type) {
                        case 0: relation = 'x'; break;
                        case 1: relation = '!'; break;
                        case 2: relation = '='; break;
                        case 3: relation = '>'; break;
                        case 4: relation = '<'; break;
                    }
                    
                    // Vary double types in chain
                    const char* type_str = (i % 3 == 0) ? "double" : 
                                          (i % 3 == 1) ? "bedouble" : "ledouble";
                    
                    magic_db += std::to_string(offset) + " " + type_str + " ";
                    if (relation != 'x') {
                        magic_db += relation;
                        magic_db += " ";
                    }
                    magic_db += std::to_string(rule_value) + "\n";
                }
            }
            
            // Write and compile chain rules
            std::string temp_filename = "/tmp/magic_chain_" + std::to_string(getpid()) + ".mgc";
            FILE* fp = fopen(temp_filename.c_str(), "w");
            if (fp) {
                fwrite(magic_db.c_str(), 1, magic_db.size(), fp);
                fclose(fp);
                
                if (use_magic_check) {
                    magic_check(magic, temp_filename.c_str());
                }
                
                magic_compile(magic, temp_filename.c_str());
                unlink(temp_filename.c_str());
            }
            
            // Generate test data matching chain offsets
            size_t test_size = fdp.ConsumeIntegralInRange<size_t>(128, 1024);
            if (fdp.remaining_bytes() >= test_size) {
                std::vector<uint8_t> test_data = fdp.ConsumeBytes<uint8_t>(test_size);
                
                // Fill chain positions with appropriate values
                for (int i = 0; i < chain_length && test_data.size() >= (i * 8 + 8); i++) {
                    uint32_t offset = (i * 8);
                    
                    if (i % 2 == 0) {
                        // Float position
                        if (fdp.remaining_bytes() >= sizeof(float)) {
                            float test_val = fdp.ConsumeFloatingPoint<float>();
                            memcpy(test_data.data() + offset, &test_val, sizeof(float));
                        }
                    } else {
                        // Double position
                        if (fdp.remaining_bytes() >= sizeof(double)) {
                            double test_val = fdp.ConsumeFloatingPoint<double>();
                            memcpy(test_data.data() + offset, &test_val, sizeof(double));
                        }
                    }
                }
                
                const char* result = magic_buffer(magic, test_data.data(), test_data.size());
                (void)result;
            }
            
            break;
        }
        
        case 3: {
            // Strategy 3: Complex rule combinations with mixed types
            std::string magic_db;
            
            // Create complex combination of rules
            // Rule 1: Float with specific value
            if (fdp.remaining_bytes() >= sizeof(uint32_t) + sizeof(float)) {
                uint32_t offset1 = fdp.ConsumeIntegral<uint32_t>() % 32;
                float val1 = fdp.ConsumeFloatingPoint<float>();
                magic_db += std::to_string(offset1) + " float = " + std::to_string(val1) + "\n";
            }
            
            // Rule 2: Double with range comparison
            if (fdp.remaining_bytes() >= sizeof(uint32_t) + sizeof(double)) {
                uint32_t offset2 = fdp.ConsumeIntegral<uint32_t>() % 64 + 8;
                double val2 = fdp.ConsumeFloatingPoint<double>();
                magic_db += std::to_string(offset2) + " double > " + std::to_string(val2 / 2.0) + "\n";
                magic_db += std::to_string(offset2) + " double < " + std::to_string(val2 * 2.0) + "\n";
            }
            
            // Rule 3: Big-endian float with "not equal"
            if (fdp.remaining_bytes() >= sizeof(uint32_t) + sizeof(float)) {
                uint32_t offset3 = fdp.ConsumeIntegral<uint32_t>() % 128 + 16;
                float val3 = fdp.ConsumeFloatingPoint<float>();
                magic_db += std::to_string(offset3) + " befloat != " + std::to_string(val3) + "\n";
            }
            
            // Rule 4: Little-endian double with "always match"
            if (fdp.remaining_bytes() >= sizeof(uint32_t) + sizeof(double)) {
                uint32_t offset4 = fdp.ConsumeIntegral<uint32_t>() % 256 + 32;
                double val4 = fdp.ConsumeFloatingPoint<double>();
                magic_db += std::to_string(offset4) + " ledouble x\n";
            }
            
            // Write and compile complex rules
            std::string temp_filename = "/tmp/magic_complex_" + std::to_string(getpid()) + ".mgc";
            FILE* fp = fopen(temp_filename.c_str(), "w");
            if (fp) {
                fwrite(magic_db.c_str(), 1, magic_db.size(), fp);
                fclose(fp);
                
                if (use_magic_check) {
                    magic_check(magic, temp_filename.c_str());
                }
                
                magic_compile(magic, temp_filename.c_str());
                unlink(temp_filename.c_str());
            }
            
            // Generate comprehensive test data
            size_t test_size = fdp.ConsumeIntegralInRange<size_t>(512, 2048);
            if (fdp.remaining_bytes() >= test_size) {
                std::vector<uint8_t> test_data = fdp.ConsumeBytes<uint8_t>(test_size);
                
                // Try to satisfy some rules with random data
                if (test_data.size() >= 256) {
                    // Add random float/double values at various positions
                    for (int i = 0; i < 8 && fdp.remaining_bytes() >= sizeof(float); i++) {
                        uint32_t pos = fdp.ConsumeIntegral<uint32_t>() % (test_data.size() - sizeof(float));
                        float val = fdp.ConsumeFloatingPoint<float>();
                        memcpy(test_data.data() + pos, &val, sizeof(float));
                    }
                    
                    for (int i = 0; i < 4 && fdp.remaining_bytes() >= sizeof(double); i++) {
                        uint32_t pos = fdp.ConsumeIntegral<uint32_t>() % (test_data.size() - sizeof(double));
                        double val = fdp.ConsumeFloatingPoint<double>();
                        memcpy(test_data.data() + pos, &val, sizeof(double));
                    }
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

} // namespace harness_009
