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

namespace harness_007 {
/*
 * Specialized fuzzing harness for libmagic targeting magiccheck() function's 
 * float/double/string comparison paths with 152 blocked branches.
 * 
 * Primary targets:
 * - magiccheck() FILE_FLOAT, FILE_BEFLOAT, FILE_LEFLOAT, FILE_DOUBLE, 
 *   FILE_BEDOUBLE, FILE_LEDOUBLE type comparison paths
 * - magiccheck() string comparison logic (FILE_STRING, FILE_BESTRING16, FILE_LESTRING16)
 * - 152 blocked branches in magiccheck() function
 * 
 * APIs targeted:
 * - magic_open, magic_buffer (primary), magic_close, optionally magic_setflags
 * 
 * Strategy:
 * 1. Generate binary test data containing float/double values at specific offsets
 * 2. Use existing magic database patterns (e.g., from animation file with ledouble at offsets 40,48)
 * 3. Test all float/double types: FILE_FLOAT, FILE_BEFLOAT, FILE_LEFLOAT, 
 *    FILE_DOUBLE, FILE_BEDOUBLE, FILE_LEDOUBLE
 * 4. Include string patterns with various comparison relations (=, >, <, !, x)
 * 5. Focus on exercising blocked branches through actual data pattern matching
 * 
 * Semantic differentiation from existing harnesses:
 * - harness_000: General API testing with default magic database
 * - harness_001: Parameter configuration functions  
 * - harness_002: File/descriptor operations
 * - harness_003: Custom magic database compilation + float/double (has hardcoded values)
 * - harness_004: Focused float comparison branch testing with custom magic db
 * - harness_005: Apprentice parsing + compress decompression
 * - harness_006: CDF/OLE compound file parsing
 * - harness_007: Direct float/double/string data generation for magiccheck() (NEW)
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size for meaningful testing
    // Need: flags + operation + float/double values + string patterns
    if (size < 128) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Consume fixed-size data first
    int flags = fdp.ConsumeIntegral<int>();
    int strategy = fdp.ConsumeIntegralInRange<int>(0, 4); // 0-4 different testing strategies
    bool test_all_float_types = fdp.ConsumeBool();
    bool test_all_double_types = fdp.ConsumeBool();
    bool test_string_comparisons = fdp.ConsumeBool();
    
    // Build flags (limit to reasonable range)
    int actual_flags = flags & 0xFFFFF;
    
    // Create magic instance
    magic_t magic = magic_open(actual_flags);
    if (magic == nullptr) {
        return 0;
    }

    // Try to load default magic database
    // Use default location or local magic file
    const char* magic_path = "/usr/share/misc/magic";
    magic_load(magic, magic_path); // Ignore errors for fuzzing

    // Strategy selection
    switch (strategy) {
        case 0: {
            // Strategy 0: Generate data matching existing magic patterns with float/double
            // Based on animation file patterns: ledouble at offset 40 (aspect ratio) and 48 (fps)
            
            size_t buffer_size = fdp.ConsumeIntegralInRange<size_t>(128, 4096);
            if (fdp.remaining_bytes() < buffer_size) {
                magic_close(magic);
                return 0;
            }
            
            std::vector<uint8_t> buffer = fdp.ConsumeBytes<uint8_t>(buffer_size);
            
            // Ensure buffer is large enough for our patterns
            if (buffer.size() < 64) {
                magic_close(magic);
                return 0;
            }
            
            // Generate float values for testing
            std::vector<float> float_values;
            std::vector<double> double_values;
            
            for (int i = 0; i < 5 && fdp.remaining_bytes() >= sizeof(float); i++) {
                float val = fdp.ConsumeFloatingPoint<float>();
                float_values.push_back(val);
            }
            
            for (int i = 0; i < 5 && fdp.remaining_bytes() >= sizeof(double); i++) {
                double val = fdp.ConsumeFloatingPoint<double>();
                double_values.push_back(val);
            }
            
            // Embed float values at various offsets to potentially match magic patterns
            // Common offsets from magic patterns in various files
            std::array<size_t, 8> common_offsets = {0, 4, 8, 12, 16, 20, 24, 28};
            
            for (size_t i = 0; i < float_values.size() && i < common_offsets.size(); i++) {
                size_t offset = common_offsets[i];
                if (offset + sizeof(float) <= buffer.size()) {
                    float val = float_values[i];
                    memcpy(buffer.data() + offset, &val, sizeof(float));
                    
                    // Also test with modified byte order for big/little endian
                    if (test_all_float_types && i % 2 == 0 && offset + sizeof(float) * 2 <= buffer.size()) {
                        // Store same value at adjacent offset for different type testing
                        uint32_t int_val;
                        memcpy(&int_val, &val, sizeof(float));
                        // Swap bytes for big-endian representation
                        uint32_t swapped_val = ((int_val & 0xFF) << 24) | ((int_val & 0xFF00) << 8) |
                                              ((int_val & 0xFF0000) >> 8) | ((int_val & 0xFF000000) >> 24);
                        memcpy(buffer.data() + offset + sizeof(float), &swapped_val, sizeof(float));
                    }
                }
            }
            
            // Embed double values at offsets for double pattern testing
            // Common offsets for double values (often at 8-byte boundaries)
            std::array<size_t, 8> double_offsets = {32, 40, 48, 56, 64, 72, 80, 88};
            
            for (size_t i = 0; i < double_values.size() && i < double_offsets.size(); i++) {
                size_t offset = double_offsets[i];
                if (offset + sizeof(double) <= buffer.size()) {
                    double val = double_values[i];
                    memcpy(buffer.data() + offset, &val, sizeof(double));
                    
                    // Also test with modified byte order for big/little endian
                    if (test_all_double_types && i % 2 == 0 && offset + sizeof(double) * 2 <= buffer.size()) {
                        // Store same value at adjacent offset for different type testing
                        uint64_t int_val;
                        memcpy(&int_val, &val, sizeof(double));
                        // Swap bytes for big-endian representation (simplified 64-bit swap)
                        uint64_t swapped_val = 0;
                        uint8_t* src = (uint8_t*)&int_val;
                        uint8_t* dst = (uint8_t*)&swapped_val;
                        for (int j = 0; j < 8; j++) {
                            dst[j] = src[7 - j];
                        }
                        memcpy(buffer.data() + offset + sizeof(double), &swapped_val, sizeof(double));
                    }
                }
            }
            
            // Test with magic_buffer
            const char* result = magic_buffer(magic, buffer.data(), buffer.size());
            (void)result; // Result can be null
            
            break;
        }
        
        case 1: {
            // Strategy 1: Test string comparison paths with various string patterns
            
            size_t buffer_size = fdp.ConsumeIntegralInRange<size_t>(64, 2048);
            if (fdp.remaining_bytes() < buffer_size) {
                magic_close(magic);
                return 0;
            }
            
            std::vector<uint8_t> buffer = fdp.ConsumeBytes<uint8_t>(buffer_size);
            
            if (!test_string_comparisons || buffer.size() < 16) {
                magic_close(magic);
                return 0;
            }
            
            // Common magic string patterns to potentially match
            const char* common_magic_strings[] = {
                "MZ",          // MS-DOS executable
                "#!",          // Script
                "%PDF",        // PDF
                "PK",          // ZIP
                "GIF",         // GIF
                "JFIF",        // JPEG
                "PNG",         // PNG
                "RIFF",        // WAV/AVI
                "BM",          // BMP
                "ID3",         // MP3
            };
            
            // Try to embed some magic strings at beginning of buffer
            size_t strings_to_embed = fdp.ConsumeIntegralInRange<size_t>(1, 4);
            size_t current_offset = 0;
            
            for (size_t i = 0; i < strings_to_embed && i < sizeof(common_magic_strings)/sizeof(common_magic_strings[0]); i++) {
                const char* str = common_magic_strings[i];
                size_t str_len = strlen(str);
                
                if (current_offset + str_len < buffer.size()) {
                    memcpy(buffer.data() + current_offset, str, str_len);
                    current_offset += str_len + 1; // Leave space between strings
                }
            }
            
            // Also embed some UTF-16 strings for FILE_BESTRING16/FILE_LESTRING16 testing
            if (current_offset + 32 < buffer.size()) {
                // Simple UTF-16 "MZ" (MS-DOS magic) in little-endian
                uint8_t utf16_mz[] = {0x4D, 0x00, 0x5A, 0x00}; // 'M' (0x004D), 'Z' (0x005A) LE
                memcpy(buffer.data() + current_offset, utf16_mz, sizeof(utf16_mz));
                current_offset += sizeof(utf16_mz);
            }
            
            // Test with magic_buffer
            const char* result = magic_buffer(magic, buffer.data(), buffer.size());
            (void)result;
            
            break;
        }
        
        case 2: {
            // Strategy 2: Comprehensive float/double testing with all relations and edge cases
            
            size_t buffer_size = fdp.ConsumeIntegralInRange<size_t>(256, 4096);
            if (fdp.remaining_bytes() < buffer_size) {
                magic_close(magic);
                return 0;
            }
            
            std::vector<uint8_t> buffer = fdp.ConsumeBytes<uint8_t>(buffer_size);
            
            if (buffer.size() < 128) {
                magic_close(magic);
                return 0;
            }
            
            // Generate special float/double values for edge case testing
            std::vector<float> special_floats;
            std::vector<double> special_doubles;
            
            // Add some known special float values that might trigger different code paths
            special_floats.push_back(0.0f);
            special_floats.push_back(-0.0f);
            special_floats.push_back(1.0f);
            special_floats.push_back(-1.0f);
            special_floats.push_back(FLT_MAX);
            special_floats.push_back(FLT_MIN);
            special_floats.push_back(FLT_EPSILON);
            special_floats.push_back(NAN);
            special_floats.push_back(INFINITY);
            special_floats.push_back(-INFINITY);
            
            // Generate random float values from fuzzer input
            for (int i = 0; i < 5 && fdp.remaining_bytes() >= sizeof(float); i++) {
                float val = fdp.ConsumeFloatingPoint<float>();
                special_floats.push_back(val);
            }
            
            // Add special double values
            special_doubles.push_back(0.0);
            special_doubles.push_back(-0.0);
            special_doubles.push_back(1.0);
            special_doubles.push_back(-1.0);
            special_doubles.push_back(DBL_MAX);
            special_doubles.push_back(DBL_MIN);
            special_doubles.push_back(DBL_EPSILON);
            special_doubles.push_back(static_cast<double>(NAN));
            special_doubles.push_back(static_cast<double>(INFINITY));
            special_doubles.push_back(static_cast<double>(-INFINITY));
            
            // Generate random double values from fuzzer input
            for (int i = 0; i < 5 && fdp.remaining_bytes() >= sizeof(double); i++) {
                double val = fdp.ConsumeFloatingPoint<double>();
                special_doubles.push_back(val);
            }
            
            // Embed float values at various offsets
            for (size_t i = 0; i < special_floats.size() && i < 20; i++) {
                size_t offset = i * 4; // 4-byte aligned for floats
                if (offset + sizeof(float) <= buffer.size()) {
                    float val = special_floats[i];
                    memcpy(buffer.data() + offset, &val, sizeof(float));
                    
                    // Also try byte-swapped versions for endian testing
                    if (test_all_float_types && offset + sizeof(float) * 2 <= buffer.size()) {
                        uint32_t int_val;
                        memcpy(&int_val, &val, sizeof(float));
                        // Simple byte swap
                        uint32_t swapped_val = ((int_val & 0xFF) << 24) | ((int_val & 0xFF00) << 8) |
                                              ((int_val & 0xFF0000) >> 8) | ((int_val & 0xFF000000) >> 24);
                        memcpy(buffer.data() + offset + sizeof(float), &swapped_val, sizeof(float));
                    }
                }
            }
            
            // Embed double values at 8-byte aligned offsets
            for (size_t i = 0; i < special_doubles.size() && i < 10; i++) {
                size_t offset = 80 + i * 8; // Start at offset 80 for doubles
                if (offset + sizeof(double) <= buffer.size()) {
                    double val = special_doubles[i];
                    memcpy(buffer.data() + offset, &val, sizeof(double));
                    
                    // Also try byte-swapped versions for endian testing
                    if (test_all_double_types && offset + sizeof(double) * 2 <= buffer.size()) {
                        uint64_t int_val;
                        memcpy(&int_val, &val, sizeof(double));
                        // Simple 64-bit byte swap
                        uint64_t swapped_val = 0;
                        uint8_t* src = (uint8_t*)&int_val;
                        uint8_t* dst = (uint8_t*)&swapped_val;
                        for (int j = 0; j < 8; j++) {
                            dst[j] = src[7 - j];
                        }
                        memcpy(buffer.data() + offset + sizeof(double), &swapped_val, sizeof(double));
                    }
                }
            }
            
            // Test with magic_buffer
            const char* result = magic_buffer(magic, buffer.data(), buffer.size());
            (void)result;
            
            break;
        }
        
        case 3: {
            // Strategy 3: Mixed data types - combining floats, doubles, and strings
            
            size_t buffer_size = fdp.ConsumeIntegralInRange<size_t>(512, 8192);
            if (fdp.remaining_bytes() < buffer_size) {
                magic_close(magic);
                return 0;
            }
            
            std::vector<uint8_t> buffer = fdp.ConsumeBytes<uint8_t>(buffer_size);
            
            if (buffer.size() < 256) {
                magic_close(magic);
                return 0;
            }
            
            // Create a structured test buffer with different data types
            
            // Section 1: Magic strings at beginning (common file signatures)
            size_t offset = 0;
            const char* magic_strs[] = {"MZ", "#!", "%PDF", "PK"};
            for (size_t i = 0; i < sizeof(magic_strs)/sizeof(magic_strs[0]) && offset < 32; i++) {
                size_t len = strlen(magic_strs[i]);
                if (offset + len < buffer.size()) {
                    memcpy(buffer.data() + offset, magic_strs[i], len);
                    offset += len + 1;
                }
            }
            
            // Align to 4-byte boundary for floats
            offset = (offset + 3) & ~3;
            
            // Section 2: Float values
            for (int i = 0; i < 8 && offset + sizeof(float) <= buffer.size(); i++) {
                float val = 0.0f;
                if (fdp.remaining_bytes() >= sizeof(float)) {
                    val = fdp.ConsumeFloatingPoint<float>();
                } else {
                    // Generate simple pattern if no more fuzzer input
                    val = static_cast<float>(i) * 1.5f;
                }
                memcpy(buffer.data() + offset, &val, sizeof(float));
                offset += sizeof(float);
            }
            
            // Align to 8-byte boundary for doubles
            offset = (offset + 7) & ~7;
            
            // Section 3: Double values
            for (int i = 0; i < 4 && offset + sizeof(double) <= buffer.size(); i++) {
                double val = 0.0;
                if (fdp.remaining_bytes() >= sizeof(double)) {
                    val = fdp.ConsumeFloatingPoint<double>();
                } else {
                    // Generate simple pattern if no more fuzzer input
                    val = static_cast<double>(i) * 2.5;
                }
                memcpy(buffer.data() + offset, &val, sizeof(double));
                offset += sizeof(double);
            }
            
            // Section 4: More strings in the middle
            if (offset + 16 < buffer.size()) {
                const char* mid_strs[] = {"RIFF", "WEBP", "FTYP", "ID3"};
                for (size_t i = 0; i < sizeof(mid_strs)/sizeof(mid_strs[0]) && offset + 8 < buffer.size(); i++) {
                    size_t len = strlen(mid_strs[i]);
                    if (offset + len < buffer.size()) {
                        memcpy(buffer.data() + offset, mid_strs[i], len);
                        offset += len + 1;
                    }
                }
            }
            
            // Test with magic_buffer
            const char* result = magic_buffer(magic, buffer.data(), buffer.size());
            (void)result;
            
            break;
        }
        
        case 4: {
            // Strategy 4: Focus on specific offsets from known magic patterns
            
            // Based on actual magic patterns found in Magdir files:
            // - animation file: ledouble at offset 40 (aspect ratio) and 48 (fps)
            // - images file: float/double at specific offsets for FITS format
            
            size_t buffer_size = fdp.ConsumeIntegralInRange<size_t>(256, 2048);
            if (fdp.remaining_bytes() < buffer_size) {
                magic_close(magic);
                return 0;
            }
            
            std::vector<uint8_t> buffer = fdp.ConsumeBytes<uint8_t>(buffer_size);
            
            if (buffer.size() < 128) {
                magic_close(magic);
                return 0;
            }
            
            // Generate values for known pattern offsets
            
            // For animation file pattern (MythTV NuppelVideo):
            // Offset 40: ledouble for aspect ratio
            // Offset 48: ledouble for fps
            if (buffer.size() >= 56) {
                // Generate reasonable aspect ratio (0.5 to 3.0)
                double aspect_ratio = 1.0;
                if (fdp.remaining_bytes() >= sizeof(double)) {
                    aspect_ratio = fdp.ConsumeFloatingPointInRange<double>(0.5, 3.0);
                }
                memcpy(buffer.data() + 40, &aspect_ratio, sizeof(double));
                
                // Generate reasonable fps (1.0 to 120.0)
                double fps = 30.0;
                if (fdp.remaining_bytes() >= sizeof(double)) {
                    fps = fdp.ConsumeFloatingPointInRange<double>(1.0, 120.0);
                }
                memcpy(buffer.data() + 48, &fps, sizeof(double));
            }
            
            // Also put "MythTV" string at beginning to match the pattern
            if (buffer.size() >= 6) {
                const char* mythtv = "MythTV";
                memcpy(buffer.data(), mythtv, 6);
            }
            
            // Test with magic_buffer
            const char* result = magic_buffer(magic, buffer.data(), buffer.size());
            (void)result;
            
            break;
        }
    }
    
    // Clean up
    magic_close(magic);
    return 0;
}

} // namespace harness_007
