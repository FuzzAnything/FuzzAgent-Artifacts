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

namespace harness_004 {
/*
 * Specialized fuzzing harness for libmagic targeting magiccheck() float comparison branches
 * 
 * Primary targets: 
 * - magiccheck() FILE_FLOAT, FILE_BEFLOAT, FILE_LEFLOAT type comparison paths
 * - 152 blocked branches in magiccheck() function for floating-point comparison cases
 * 
 * APIs targeted:
 * - magic_open, magic_load, magic_buffer, magic_close
 * 
 * Strategy:
 * 1. Generate custom magic database strings with float patterns using fuzzed values
 * 2. Test all relation types ('x', '!', '=', '>', '<') for each float type
 * 3. Generate test buffers with float values at appropriate offsets
 * 4. Load custom magic database and test with magic_buffer
 * 
 * Semantic differentiation from existing harnesses:
 * - harness_000: General API testing with default magic database
 * - harness_001: Parameter configuration functions  
 * - harness_002: File/descriptor operations
 * - harness_003: Custom magic database compilation + float/double type checks (has hardcoded values)
 * - harness_004: Focused float comparison branch testing with fully fuzzed values (NEW)
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size for meaningful testing
    // Need: flags + float values for magic patterns + test buffer data
    if (size < 128) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Consume fixed-size data first
    int flags = fdp.ConsumeIntegral<int>();
    int strategy = fdp.ConsumeIntegralInRange<int>(0, 3); // 0-3 different testing strategies
    bool test_all_relations = fdp.ConsumeBool();
    bool test_both_endian = fdp.ConsumeBool();
    
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
            // Strategy 1: Test all float relation types with random values
            std::string magic_db;
            
            // Generate random float values for magic patterns
            std::vector<float> pattern_values;
            for (int i = 0; i < 10 && fdp.remaining_bytes() >= sizeof(float); i++) {
                float val = fdp.ConsumeFloatingPoint<float>();
                pattern_values.push_back(val);
            }
            
            if (pattern_values.empty()) {
                magic_close(magic);
                return 0;
            }
            
            // Generate magic patterns with different offsets, types, and relations
            for (size_t i = 0; i < pattern_values.size(); i++) {
                int offset = (i * 4) % 64; // Spread offsets to avoid overlap
                float val = pattern_values[i];
                
                // Add FILE_FLOAT patterns with various relations
                magic_db += std::to_string(offset) + " float " + std::to_string(val) + "\n";
                
                if (test_all_relations) {
                    magic_db += std::to_string(offset) + " float != " + std::to_string(val) + "\n";
                    magic_db += std::to_string(offset) + " float > " + std::to_string(val/2.0f) + "\n";
                    magic_db += std::to_string(offset) + " float < " + std::to_string(val*2.0f) + "\n";
                }
                
                if (test_both_endian && fdp.ConsumeBool()) {
                    // Add big-endian and little-endian float patterns
                    magic_db += std::to_string(offset) + " befloat " + std::to_string(val) + "\n";
                    magic_db += std::to_string(offset) + " lefloat " + std::to_string(val) + "\n";
                }
            }
            
            // Create temporary file for magic database
            std::string temp_filename = "/tmp/magic_fuzz_float_" + std::to_string(getpid()) + ".mgc";
            
            // Write magic database to temporary file
            FILE* fp = fopen(temp_filename.c_str(), "w");
            if (fp) {
                fwrite(magic_db.c_str(), 1, magic_db.size(), fp);
                fclose(fp);
                
                // Load the custom magic database
                magic_load(magic, temp_filename.c_str());
                
                // Clean up temp file
                unlink(temp_filename.c_str());
            }
            
            // Generate test buffers with float values
            size_t test_size = fdp.ConsumeIntegralInRange<size_t>(64, 1024);
            if (fdp.remaining_bytes() >= test_size) {
                std::vector<uint8_t> test_buffer = fdp.ConsumeBytes<uint8_t>(test_size);
                
                // Embed float values at offsets matching our magic patterns
                for (size_t i = 0; i < pattern_values.size() && i < 10; i++) {
                    int offset = (i * 4) % 64;
                    if (offset + sizeof(float) <= test_buffer.size()) {
                        float test_val = pattern_values[i];
                        // Modify value based on relation being tested
                        if (i % 3 == 0) {
                            test_val = pattern_values[i] * 1.5f; // For > tests
                        } else if (i % 3 == 1) {
                            test_val = pattern_values[i] * 0.5f; // For < tests
                        }
                        // For != tests, use different value
                        if (i % 4 == 0 && fdp.ConsumeBool()) {
                            test_val = pattern_values[i] + 1.0f;
                        }
                        
                        memcpy(test_buffer.data() + offset, &test_val, sizeof(float));
                    }
                }
                
                const char* result = magic_buffer(magic, test_buffer.data(), test_buffer.size());
                (void)result; // Result can be null
            }
            
            break;
        }
        
        case 1: {
            // Strategy 2: Test specific float values and edge cases
            std::string magic_db;
            

            
            // Generate special float values from fuzzer input
            std::vector<float> special_values;
            for (int i = 0; i < 6 && fdp.remaining_bytes() >= sizeof(float); i++) {
                float val = fdp.ConsumeFloatingPoint<float>();
                special_values.push_back(val);
            }
            
            if (special_values.empty()) {
                magic_close(magic);
                return 0;
            }
            
            // Create patterns with special float values
            for (size_t i = 0; i < special_values.size() && i < 6; i++) {
                float val = special_values[i];
                std::string offset_str = std::to_string(i * 4);
                
                // Test exact equality
                magic_db += offset_str + " float = " + std::to_string(val) + "\n";
                
                // Test inequality
                magic_db += offset_str + " float != " + std::to_string(val + 0.1f) + "\n";
                
                // Test greater than
                magic_db += offset_str + " float > " + std::to_string(val - 1.0f) + "\n";
                
                // Test less than  
                magic_db += offset_str + " float < " + std::to_string(val + 1.0f) + "\n";
                
                // Test "don't care" relation
                magic_db += offset_str + " float x\n";
            }
            
            // Write and load magic database
            std::string temp_filename = "/tmp/magic_fuzz_special_" + std::to_string(getpid()) + ".mgc";
            FILE* fp = fopen(temp_filename.c_str(), "w");
            if (fp) {
                fwrite(magic_db.c_str(), 1, magic_db.size(), fp);
                fclose(fp);
                
                magic_load(magic, temp_filename.c_str());
                unlink(temp_filename.c_str());
            }
            
            // Create test buffer with special values
            std::vector<uint8_t> test_buffer(64, 0);
            for (size_t i = 0; i < special_values.size() && i < 6; i++) {
                int offset = i * 4;
                if (offset + sizeof(float) <= test_buffer.size()) {
                    float test_val = special_values[i];
                    memcpy(test_buffer.data() + offset, &test_val, sizeof(float));
                }
            }
            
            const char* result = magic_buffer(magic, test_buffer.data(), test_buffer.size());
            (void)result;
            
            break;
        }
        
        case 2: {
            // Strategy 3: Test float patterns with mixed endianness
            std::string magic_db;
            
            // Generate test float values
            std::vector<float> float_values;
            for (int i = 0; i < 8 && fdp.remaining_bytes() >= sizeof(float); i++) {
                float val = fdp.ConsumeFloatingPoint<float>();
                float_values.push_back(val);
            }
            
            if (float_values.empty()) {
                magic_close(magic);
                return 0;
            }
            
            // Create mixed endianness patterns
            for (size_t i = 0; i < float_values.size(); i++) {
                int offset = i * 8;
                float val = float_values[i];
                
                // Regular float
                magic_db += std::to_string(offset) + " float " + std::to_string(val) + "\n";
                
                // Big-endian float at offset+4
                magic_db += std::to_string(offset + 4) + " befloat " + std::to_string(val) + "\n";
                
                // Little-endian float at offset+8 (next pattern)
                if (i + 1 < float_values.size()) {
                    magic_db += std::to_string(offset + 8) + " lefloat " + std::to_string(float_values[i + 1]) + "\n";
                }
            }
            
            // Write and load magic database
            std::string temp_filename = "/tmp/magic_fuzz_endian_" + std::to_string(getpid()) + ".mgc";
            FILE* fp = fopen(temp_filename.c_str(), "w");
            if (fp) {
                fwrite(magic_db.c_str(), 1, magic_db.size(), fp);
                fclose(fp);
                
                magic_load(magic, temp_filename.c_str());
                unlink(temp_filename.c_str());
            }
            
            // Create test buffer with proper layout
            size_t buffer_size = float_values.size() * 8 + 8;
            std::vector<uint8_t> test_buffer(buffer_size, 0);
            
            for (size_t i = 0; i < float_values.size(); i++) {
                int offset = i * 8;
                float val = float_values[i];
                
                // Regular float at offset
                memcpy(test_buffer.data() + offset, &val, sizeof(float));
                
                // Big-endian float representation at offset+4
                uint32_t be_val;
                uint8_t* val_bytes = reinterpret_cast<uint8_t*>(&val);
                uint8_t be_bytes[4];
                be_bytes[0] = val_bytes[3];
                be_bytes[1] = val_bytes[2];
                be_bytes[2] = val_bytes[1];
                be_bytes[3] = val_bytes[0];
                memcpy(test_buffer.data() + offset + 4, be_bytes, sizeof(float));
            }
            
            const char* result = magic_buffer(magic, test_buffer.data(), test_buffer.size());
            (void)result;
            
            break;
        }
        
        case 3: {
            // Strategy 4: Test IEEE 754 special values and edge cases
            std::string magic_db;
            
            // Generate magic patterns for IEEE special values
            // Use fuzzed values to determine which special cases to test
            uint8_t special_cases = fdp.ConsumeIntegral<uint8_t>();
            
            // Offset for patterns
            int base_offset = fdp.ConsumeIntegralInRange<int>(0, 32);
            
            // Test NaN patterns
            if (special_cases & 0x01) {
                magic_db += std::to_string(base_offset) + " float x\n"; // Don't care relation for NaN
                magic_db += std::to_string(base_offset + 4) + " float != 0.0\n"; // NaN != 0
            }
            
            // Test infinity patterns
            if (special_cases & 0x02) {
                magic_db += std::to_string(base_offset + 8) + " float > 1e38\n"; // Infinity > large value
                magic_db += std::to_string(base_offset + 12) + " float < -1e38\n"; // -Infinity < -large value
            }
            
            // Test zero and denormal patterns
            if (special_cases & 0x04) {
                magic_db += std::to_string(base_offset + 16) + " float = 0.0\n";
                magic_db += std::to_string(base_offset + 20) + " float != 0.0\n";
                magic_db += std::to_string(base_offset + 24) + " float > -1e-45\n"; // Test with denormal
                magic_db += std::to_string(base_offset + 28) + " float < 1e-45\n";  // Test with denormal
            }
            
            // Write and load magic database
            std::string temp_filename = "/tmp/magic_fuzz_ieee_" + std::to_string(getpid()) + ".mgc";
            FILE* fp = fopen(temp_filename.c_str(), "w");
            if (fp) {
                fwrite(magic_db.c_str(), 1, magic_db.size(), fp);
                fclose(fp);
                
                magic_load(magic, temp_filename.c_str());
                unlink(temp_filename.c_str());
            }
            
            // Create test buffer with IEEE special values
            std::vector<uint8_t> test_buffer(64, 0);
            
            // Generate some float values for testing
            if (fdp.remaining_bytes() >= sizeof(float) * 4) {
                for (int i = 0; i < 4; i++) {
                    float val = fdp.ConsumeFloatingPoint<float>();
                    int offset = base_offset + i * 4;
                    if (offset + sizeof(float) <= test_buffer.size()) {
                        memcpy(test_buffer.data() + offset, &val, sizeof(float));
                    }
                }
                
                // Add some special IEEE values if we have space
                float zero = 0.0f;
                float pos_inf = INFINITY;
                float neg_inf = -INFINITY;
                float nan_val = NAN;
                
                // Place at specific offsets if they fit
                if (base_offset + 32 + sizeof(float) <= test_buffer.size()) {
                    memcpy(test_buffer.data() + base_offset + 32, &zero, sizeof(float));
                }
                if (base_offset + 36 + sizeof(float) <= test_buffer.size()) {
                    memcpy(test_buffer.data() + base_offset + 36, &pos_inf, sizeof(float));
                }
                if (base_offset + 40 + sizeof(float) <= test_buffer.size()) {
                    memcpy(test_buffer.data() + base_offset + 40, &neg_inf, sizeof(float));
                }
                if (base_offset + 44 + sizeof(float) <= test_buffer.size()) {
                    memcpy(test_buffer.data() + base_offset + 44, &nan_val, sizeof(float));
                }
            }
            
            const char* result = magic_buffer(magic, test_buffer.data(), test_buffer.size());
            (void)result;
            
            break;
        }
    }

    // Clean up
    magic_close(magic);
    return 0;
}

} // namespace harness_004
