#include <fuzzer/FuzzedDataProvider.h>
#include "magic.h"
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <array>
#include <algorithm>
#include <cmath>
#include <cfloat>

namespace harness_016 {
/*
 * Deep magic matching pipeline harness for libmagic targeting softmagic.c (0.90%) 
 * and apprentice.c (22.77%) internal functions to unlock thousands of blocked branches.
 * 
 * Primary internal targets (0% coverage, massive branch complexity):
 * 1. mget() - 0% coverage, 3028 undiscovered branches
 * 2. mconvert() - 0% coverage, 759 undiscovered branches  
 * 3. mprint() - 0% coverage, 714 undiscovered branches
 * 4. magiccheck() - 0% coverage, 687 undiscovered branches
 * 5. apprentice_magic_strength_1 - 97 blocked branches
 * 
 * APIs targeted:
 * - magic_open, magic_load, magic_buffer, magic_close (via public API)
 * 
 * Strategy for deep magic matching pipeline testing:
 * 1. Generate comprehensive test buffers covering ALL FILE_* data types from file.h
 * 2. Test data at various offsets to exercise offset calculation logic in mget()
 * 3. Test endianness variations (BE/LE) for all numeric types
 * 4. Test all comparison relations (=, !=, >, <, &, ^, ~) for magiccheck()
 * 5. Test string, regex, and search patterns for text matching
 * 6. Test complex continuation levels and nested patterns
 * 7. Focus on exercising softmagic.c matching engine directly via magic_buffer
 * 
 * Semantic differentiation from existing harnesses (000-015):
 * - harness_000: General API testing with default magic database
 * - harness_005: Apprentice parsing + compression + float/double
 * - harness_010: Complex multi-type pattern combinations for magiccheck()
 * - harness_011/012/013/014/015: Database compilation and lifecycle testing
 * - harness_016: Deep magic matching pipeline targeting ALL FILE_* types with comprehensive buffer generation (NEW)
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size for comprehensive testing
    // Need: flags + buffer strategy + test data for various types
    if (size < 256) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Consume fixed-size data first
    int flags = fdp.ConsumeIntegral<int>();
    int buffer_strategy = fdp.ConsumeIntegralInRange<int>(0, 4); // 0-4 different buffer strategies
    bool test_all_types = fdp.ConsumeBool();
    bool test_endian_variants = fdp.ConsumeBool();
    bool test_complex_offsets = fdp.ConsumeBool();
    bool test_string_patterns = fdp.ConsumeBool();
    bool test_regex_search = fdp.ConsumeBool();
    
    // Limit flags to reasonable range
    int actual_flags = flags & 0xFFFFFFF;
    
    // Create magic instance
    magic_t magic = magic_open(actual_flags);
    if (magic == nullptr) {
        return 0;
    }

    // Try to load default magic database
    const char* magic_path = "/usr/share/misc/magic";
    magic_load(magic, magic_path); // Ignore errors for fuzzing

    // Strategy selection for test buffer generation
    switch (buffer_strategy) {
        case 0: {
            // Strategy 0: Comprehensive coverage of ALL basic FILE_* types
            std::vector<uint8_t> test_buffer(1024, 0);
            
            // Generate test data for each FILE_* type at different offsets
            size_t current_offset = 0;
            
            // FILE_BYTE (1)
            if (fdp.remaining_bytes() >= 1) {
                uint8_t byte_val = fdp.ConsumeIntegral<uint8_t>();
                if (current_offset < test_buffer.size()) {
                    test_buffer[current_offset] = byte_val;
                }
                current_offset += 4; // Pad for alignment
            }
            
            // FILE_SHORT (2) and variants
            if (fdp.remaining_bytes() >= sizeof(uint16_t)) {
                uint16_t short_val = fdp.ConsumeIntegral<uint16_t>();
                if (current_offset + sizeof(uint16_t) <= test_buffer.size()) {
                    // Native endian
                    memcpy(&test_buffer[current_offset], &short_val, sizeof(uint16_t));
                }
                current_offset += 8;
            }
            
            // FILE_LONG (4) and variants  
            if (fdp.remaining_bytes() >= sizeof(uint32_t)) {
                uint32_t long_val = fdp.ConsumeIntegral<uint32_t>();
                if (current_offset + sizeof(uint32_t) <= test_buffer.size()) {
                    memcpy(&test_buffer[current_offset], &long_val, sizeof(uint32_t));
                }
                current_offset += 12;
            }
            
            // FILE_QUAD (24) and variants
            if (fdp.remaining_bytes() >= sizeof(uint64_t)) {
                uint64_t quad_val = fdp.ConsumeIntegral<uint64_t>();
                if (current_offset + sizeof(uint64_t) <= test_buffer.size()) {
                    memcpy(&test_buffer[current_offset], &quad_val, sizeof(uint64_t));
                }
                current_offset += 16;
            }
            
            // FILE_FLOAT (33) and variants
            if (fdp.remaining_bytes() >= sizeof(float)) {
                float float_val = fdp.ConsumeFloatingPoint<float>();
                if (current_offset + sizeof(float) <= test_buffer.size()) {
                    memcpy(&test_buffer[current_offset], &float_val, sizeof(float));
                }
                current_offset += 20;
            }
            
            // FILE_DOUBLE (36) and variants
            if (fdp.remaining_bytes() >= sizeof(double)) {
                double double_val = fdp.ConsumeFloatingPoint<double>();
                if (current_offset + sizeof(double) <= test_buffer.size()) {
                    memcpy(&test_buffer[current_offset], &double_val, sizeof(double));
                }
                current_offset += 24;
            }
            
            // FILE_STRING (5) - embed string data
            if (test_string_patterns && fdp.remaining_bytes() >= 16) {
                size_t str_len = fdp.ConsumeIntegralInRange<size_t>(1, 64);
                if (fdp.remaining_bytes() >= str_len && current_offset + str_len <= test_buffer.size()) {
                    std::string str_val = fdp.ConsumeBytesAsString(str_len);
                    memcpy(&test_buffer[current_offset], str_val.c_str(), str_val.length());
                    current_offset += str_len + 4;
                }
            }
            
            // Test with magic_buffer
            if (!test_buffer.empty()) {
                const char* result = magic_buffer(magic, test_buffer.data(), test_buffer.size());
                (void)result; // Use result to avoid unused variable warning
            }
            
            break;
        }
        
        case 1: {
            // Strategy 1: Endianness testing with BE/LE variants
            std::vector<uint8_t> test_buffer(512, 0);
            
            // Place different endian variants at different offsets
            size_t offset = 0;
            
            // Test BE variants (BESHORT, BELONG, BEDATE, BEQUAD, BEFLOAT, BEDOUBLE)
            if (fdp.remaining_bytes() >= sizeof(uint32_t) + sizeof(float) + sizeof(double)) {
                uint32_t be_long = fdp.ConsumeIntegral<uint32_t>();
                float be_float = fdp.ConsumeFloatingPoint<float>();
                double be_double = fdp.ConsumeFloatingPoint<double>();
                
                // For BE testing, we'll just place the values - libmagic handles endianness
                if (offset + sizeof(uint32_t) <= test_buffer.size()) {
                    memcpy(&test_buffer[offset], &be_long, sizeof(uint32_t));
                }
                offset += 32;
                
                if (offset + sizeof(float) <= test_buffer.size()) {
                    memcpy(&test_buffer[offset], &be_float, sizeof(float));
                }
                offset += 32;
                
                if (offset + sizeof(double) <= test_buffer.size()) {
                    memcpy(&test_buffer[offset], &be_double, sizeof(double));
                }
            }
            
            // Test LE variants at different offset
            offset = 256; // Start at different offset
            if (fdp.remaining_bytes() >= sizeof(uint32_t) + sizeof(float) + sizeof(double)) {
                uint32_t le_long = fdp.ConsumeIntegral<uint32_t>();
                float le_float = fdp.ConsumeFloatingPoint<float>();
                double le_double = fdp.ConsumeFloatingPoint<double>();
                
                if (offset + sizeof(uint32_t) <= test_buffer.size()) {
                    memcpy(&test_buffer[offset], &le_long, sizeof(uint32_t));
                }
                offset += 32;
                
                if (offset + sizeof(float) <= test_buffer.size()) {
                    memcpy(&test_buffer[offset], &le_float, sizeof(float));
                }
                offset += 32;
                
                if (offset + sizeof(double) <= test_buffer.size()) {
                    memcpy(&test_buffer[offset], &le_double, sizeof(double));
                }
            }
            
            // Test with magic_buffer
            if (!test_buffer.empty()) {
                const char* result = magic_buffer(magic, test_buffer.data(), test_buffer.size());
                (void)result;
            }
            
            break;
        }
        
        case 2: {
            // Strategy 2: Complex offset and continuation testing
            // Create buffer with data at non-aligned, overlapping, and far offsets
            std::vector<uint8_t> test_buffer(2048, 0);
            
            // Generate multiple data chunks at various offsets
            for (int i = 0; i < 10 && fdp.remaining_bytes() >= 8; i++) {
                // Generate random offset
                uint32_t data_offset = fdp.ConsumeIntegralInRange<uint32_t>(0, 2000);
                
                // Generate random data type and value
                uint8_t data_type = fdp.ConsumeIntegral<uint8_t>() % 4;
                size_t data_size = 0;
                
                switch (data_type) {
                    case 0: data_size = 1; break; // byte
                    case 1: data_size = 2; break; // short
                    case 2: data_size = 4; break; // long
                    case 3: data_size = 8; break; // quad/double
                }
                
                if (fdp.remaining_bytes() >= data_size && 
                    data_offset + data_size <= test_buffer.size()) {
                    
                    std::vector<uint8_t> chunk = fdp.ConsumeBytes<uint8_t>(data_size);
                    memcpy(&test_buffer[data_offset], chunk.data(), chunk.size());
                }
            }
            
            // Also embed some string patterns if requested
            if (test_string_patterns && fdp.remaining_bytes() >= 32) {
                size_t str_offset = fdp.ConsumeIntegralInRange<size_t>(0, 1500);
                size_t str_len = fdp.ConsumeIntegralInRange<size_t>(1, 128);
                
                if (fdp.remaining_bytes() >= str_len && 
                    str_offset + str_len <= test_buffer.size()) {
                    
                    std::string str_val = fdp.ConsumeBytesAsString(str_len);
                    memcpy(&test_buffer[str_offset], str_val.c_str(), str_len);
                }
            }
            
            // Test with magic_buffer
            if (!test_buffer.empty()) {
                const char* result = magic_buffer(magic, test_buffer.data(), test_buffer.size());
                (void)result;
            }
            
            break;
        }
        
        case 3: {
            // Strategy 3: String, regex, and search pattern testing
            std::vector<uint8_t> test_buffer(1024, 0);
            
            // Fill buffer with mix of binary and text data
            size_t offset = 0;
            
            // Start with some binary data
            for (int i = 0; i < 5 && fdp.remaining_bytes() >= 4 && offset + 4 <= test_buffer.size(); i++) {
                uint32_t val = fdp.ConsumeIntegral<uint32_t>();
                memcpy(&test_buffer[offset], &val, 4);
                offset += 8;
            }
            
            // Add string patterns
            if (test_string_patterns) {
                // Add common magic strings that might trigger matches
                const char* magic_strings[] = {
                    "MZ", "%PDF", "#!/", "ELF", "PK", "GIF", "PNG", "JFIF", 
                    "RIFF", "ID3", "OggS", "FLAC", "ftyp", "moov"
                };
                
                for (const char* str : magic_strings) {
                    size_t len = strlen(str);
                    if (offset + len <= test_buffer.size()) {
                        memcpy(&test_buffer[offset], str, len);
                        offset += len + 4;
                    }
                }
                
                // Add fuzzer-generated strings
                if (fdp.remaining_bytes() >= 64) {
                    size_t str_len = fdp.ConsumeIntegralInRange<size_t>(1, 128);
                    if (fdp.remaining_bytes() >= str_len && offset + str_len <= test_buffer.size()) {
                        std::string fuzz_str = fdp.ConsumeBytesAsString(str_len);
                        memcpy(&test_buffer[offset], fuzz_str.c_str(), fuzz_str.length());
                        offset += str_len + 4;
                    }
                }
            }
            
            // Add regex-like patterns (sequences that could match regex)
            if (test_regex_search) {
                // Add patterns with repetition
                std::string pattern_area(128, 'A');
                if (offset + pattern_area.size() <= test_buffer.size()) {
                    memcpy(&test_buffer[offset], pattern_area.c_str(), pattern_area.size());
                    offset += pattern_area.size();
                }
                
                // Add null-terminated strings
                if (offset + 16 <= test_buffer.size()) {
                    const char* null_term = "TEST\x00STRING";
                    memcpy(&test_buffer[offset], null_term, 11);
                    offset += 16;
                }
            }
            
            // Fill remaining with random bytes
            if (fdp.remaining_bytes() > 0 && offset < test_buffer.size()) {
                size_t remaining = std::min(test_buffer.size() - offset, fdp.remaining_bytes());
                std::vector<uint8_t> random_bytes = fdp.ConsumeBytes<uint8_t>(remaining);
                memcpy(&test_buffer[offset], random_bytes.data(), random_bytes.size());
            }
            
            // Test with magic_buffer
            if (!test_buffer.empty()) {
                const char* result = magic_buffer(magic, test_buffer.data(), test_buffer.size());
                (void)result;
            }
            
            break;
        }
        
        case 4: {
            // Strategy 4: Special value testing (edge cases, NaN, Inf, zero, max values)
            std::vector<uint8_t> test_buffer(1024, 0);
            size_t offset = 0;
            
            // Test special float values
            float special_floats[] = {
                0.0f, -0.0f, 1.0f, -1.0f, FLT_MAX, FLT_MIN, 
                INFINITY, -INFINITY, NAN
            };
            
            for (size_t i = 0; i < sizeof(special_floats)/sizeof(special_floats[0]) && 
                 offset + sizeof(float) <= test_buffer.size(); i++) {
                memcpy(&test_buffer[offset], &special_floats[i], sizeof(float));
                offset += 8;
            }
            
            // Test special double values
            double special_doubles[] = {
                0.0, -0.0, 1.0, -1.0, DBL_MAX, DBL_MIN,
                INFINITY, -INFINITY, NAN
            };
            
            for (size_t i = 0; i < sizeof(special_doubles)/sizeof(special_doubles[0]) && 
                 offset + sizeof(double) <= test_buffer.size(); i++) {
                memcpy(&test_buffer[offset], &special_doubles[i], sizeof(double));
                offset += 16;
            }
            
            // Test integer edge cases
            uint8_t max_byte = 0xFF;
            uint16_t max_short = 0xFFFF;
            uint32_t max_long = 0xFFFFFFFF;
            uint64_t max_quad = 0xFFFFFFFFFFFFFFFF;
            
            if (offset + sizeof(max_byte) <= test_buffer.size()) {
                memcpy(&test_buffer[offset], &max_byte, sizeof(max_byte));
                offset += 4;
            }
            
            if (offset + sizeof(max_short) <= test_buffer.size()) {
                memcpy(&test_buffer[offset], &max_short, sizeof(max_short));
                offset += 8;
            }
            
            if (offset + sizeof(max_long) <= test_buffer.size()) {
                memcpy(&test_buffer[offset], &max_long, sizeof(max_long));
                offset += 12;
            }
            
            if (offset + sizeof(max_quad) <= test_buffer.size()) {
                memcpy(&test_buffer[offset], &max_quad, sizeof(max_quad));
                offset += 16;
            }
            
            // Add fuzzer data for remaining space
            if (fdp.remaining_bytes() > 0 && offset < test_buffer.size()) {
                size_t remaining = std::min(test_buffer.size() - offset, fdp.remaining_bytes());
                std::vector<uint8_t> fuzz_data = fdp.ConsumeBytes<uint8_t>(remaining);
                memcpy(&test_buffer[offset], fuzz_data.data(), fuzz_data.size());
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

} // namespace harness_016
