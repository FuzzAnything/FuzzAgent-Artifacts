#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <cmath>
#include <cfloat>
#include <algorithm>
#include "magic.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

namespace harness_019 {
/*
 * Fuzzing harness for libmagic library targeting magiccheck function's 
 * 206 blocked branches which remain the #1 coverage blocker despite 19 previous harnesses.
 * 
 * TARGET: magiccheck function in softmagic.c (206/240 branches still blocked)
 * 
 * FOCUSED APPROACH: Specialized targeting of error conditions and edge cases
 * to penetrate the magiccheck function's blocked branches.
 * 
 * ANALYSIS: Previous harnesses have not reduced magiccheck's blocked branch count.
 * Need specialized approach focusing on:
 * 1. Error conditions (default cases in type switches for float/double comparisons)
 * 2. Specific type comparisons not exercised by current test inputs  
 * 3. Complex relational operators for floating-point types
 * 4. Type-specific comparisons targeting FILE_FLOAT, FILE_DOUBLE, FILE_BYTE, FILE_SHORT, FILE_LONG, FILE_QUAD
 * 5. String comparison variations - FILE_STRING, FILE_PSTRING, FILE_OCTAL with different str_flags
 * 6. SEARCH type patterns with various str_range values
 * 7. Malformed relation operators to trigger error conditions
 * 8. Invalid type values to trigger default case (line 2412)
 * 
 * SPECIFIC ERROR PATHS TO TARGET:
 * - magiccheck() lines 2216-2220: default case for float relation operators
 * - magiccheck() lines 2249-2253: default case for double relation operators  
 * - magiccheck() lines 2412-2413: default case for invalid type
 * - FILE_SEARCH with various str_range values and edge cases
 * - FILE_REGEX with malformed regex patterns
 * - FILE_DER with invalid DER data
 * - FILE_GUID with malformed GUID data
 * 
 * APIs to target:
 * - magic_buffer (Primary execution path)
 * - magic_open (Initialization)
 * - magic_load or magic_load_buffers (Pattern loading)
 * - magic_close (Cleanup)
 * 
 * INPUT STRATEGY:
 * 1. Floating-point magic patterns with malformed relation operators
 * 2. Type-specific comparison patterns for each FILE_TYPE with edge values
 * 3. Error condition triggers - malformed relation operators, invalid types
 * 4. Comprehensive string comparison variations with different str_flags
 * 5. SEARCH type patterns with different str_range values (0, 1, >1)
 * 6. Mixed valid and invalid patterns to test error recovery
 * 
 * DIFFERENTIATION FROM PREVIOUS HARNESSES:
 * - Focus on ERROR CONDITIONS rather than valid patterns
 * - Target DEFAULT CASES in magiccheck switches
 * - Use MALFORMED inputs to trigger error paths
 * - Test EDGE CASES for each type (NaN, Infinity, max values, zero)
 * - Mix valid and invalid patterns to test error recovery
 */

// Include libmagic headers

// Include necessary system headers

// Helper to generate malformed relation operators for float/double
std::string generate_malformed_relation_pattern(FuzzedDataProvider& fdp, const char* type_str) {
    std::string pattern;
    
    // Generate offset (0-4096)
    uint32_t offset = fdp.ConsumeIntegralInRange<uint32_t>(0, 4096);
    pattern += std::to_string(offset);
    pattern += " ";
    pattern += type_str;
    
    const char malformed_chars[] = {
        'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'y', 'z',
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
        '@', '#', '$', '%', '*', '(', ')', '_', '+', '-', '[', ']', '{', '}', '|', '\\', ':', ';', '"', '\'', ',', '.', '/', '?', '~', '`'
    };
    char reln = malformed_chars[fdp.ConsumeIntegralInRange<uint8_t>(0, sizeof(malformed_chars) - 1)];
    pattern += reln;
    pattern += ' ';
    
    // Generate float/double value
    if (strcmp(type_str, "float") == 0 || strcmp(type_str, "befloat") == 0 || 
        strcmp(type_str, "lefloat") == 0) {
        float value = fdp.ConsumeFloatingPoint<float>();
        pattern += std::to_string(value);
    } else {
        double value = fdp.ConsumeFloatingPoint<double>();
        pattern += std::to_string(value);
    }
    
    pattern += " Malformed relation test\n";
    return pattern;
}

// Helper to generate invalid type pattern (for default case coverage - line 2412)
std::string generate_invalid_type_pattern(FuzzedDataProvider& fdp) {
    std::string pattern;
    
    // Generate offset (0-4096)
    uint32_t offset = fdp.ConsumeIntegralInRange<uint32_t>(0, 4096);
    pattern += std::to_string(offset);
    pattern += " ";
    
    // Generate invalid type name (not a valid FILE_ type)
    // Target: magiccheck() line 2412 default case
    const char* invalid_types[] = {
        "invalid", "badtype", "unknown", "wrong", "fake", 
        "bogus", "dummy", "test", "error", "bug"
    };
    
    pattern += invalid_types[fdp.ConsumeIntegralInRange<uint8_t>(0, 9)];
    
    // Generate a relation character
    pattern += "= ";
    
    // Generate some value
    pattern += std::to_string(fdp.ConsumeIntegral<uint32_t>());
    
    pattern += " Invalid type test for default case\n";
    return pattern;
}

// Helper to generate SEARCH pattern with various str_range values
std::string generate_search_pattern_with_range(FuzzedDataProvider& fdp) {
    std::string pattern;
    
    // Generate offset (0-4096)
    uint32_t offset = fdp.ConsumeIntegralInRange<uint32_t>(0, 4096);
    pattern += std::to_string(offset);
    pattern += " search";
    
    // Generate search string
    size_t search_len = fdp.ConsumeIntegralInRange<size_t>(1, 32);
    std::string search_str = fdp.ConsumeRandomLengthString(search_len);
    
    // Escape special characters
    pattern += " \"";
    for (char c : search_str) {
        if (c == '"' || c == '\\') {
            pattern += '\\';
        }
        pattern += c;
    }
    pattern += "\"";
    
    // Add str_range value (0, 1, or >1)
    // This affects search behavior in magiccheck lines 2298-2320
    uint32_t str_range = fdp.ConsumeIntegralInRange<uint32_t>(0, 100);
    pattern += " >";
    pattern += std::to_string(str_range);
    
    pattern += " Search pattern with range\n";
    return pattern;
}

// Helper to generate string comparison with different str_flags
std::string generate_string_pattern_with_flags(FuzzedDataProvider& fdp, const char* type_str) {
    std::string pattern;
    
    // Generate offset (0-4096)
    uint32_t offset = fdp.ConsumeIntegralInRange<uint32_t>(0, 4096);
    pattern += std::to_string(offset);
    pattern += " ";
    pattern += type_str;
    
    // Generate relation (valid ones for string comparisons)
    const char reln_chars[] = {'=', '!', '>', '<'};
    char reln = reln_chars[fdp.ConsumeIntegralInRange<uint8_t>(0, 3)];
    pattern += reln;
    
    // Generate string value
    size_t str_len = fdp.ConsumeIntegralInRange<size_t>(1, 64);
    std::string str_val = fdp.ConsumeRandomLengthString(str_len);
    
    pattern += " \"";
    for (char c : str_val) {
        if (c == '"' || c == '\\') {
            pattern += '\\';
        }
        pattern += c;
    }
    pattern += "\"";
    
    // Add str_flags (affects file_strncmp behavior)
    // Magic values: STRING_COMPACT_WHITESPACE, STRING_COMPACT_OPTIONAL_WHITESPACE, etc.
    // We'll use numeric flags to test different code paths
    if (fdp.ConsumeBool()) {
        pattern += " W";
    }
    if (fdp.ConsumeBool()) {
        pattern += " w";
    }
    if (fdp.ConsumeBool()) {
        pattern += " B";
    }
    if (fdp.ConsumeBool()) {
        pattern += " b";
    }
    
    pattern += " String pattern with flags\n";
    return pattern;
}

// Helper to generate numeric type patterns with edge values
std::string generate_numeric_pattern(FuzzedDataProvider& fdp, const char* type_str) {
    std::string pattern;
    
    // Generate offset (0-4096)
    uint32_t offset = fdp.ConsumeIntegralInRange<uint32_t>(0, 4096);
    pattern += std::to_string(offset);
    pattern += " ";
    pattern += type_str;
    
    // Generate relation
    const char reln_chars[] = {'=', '!', '>', '<', 'x', '&', '^'};
    char reln = reln_chars[fdp.ConsumeIntegralInRange<uint8_t>(0, 6)];
    pattern += reln;
    pattern += ' ';
    
    // Generate value based on type
    if (strstr(type_str, "byte") || strstr(type_str, "short") || 
        strstr(type_str, "long") || strstr(type_str, "quad")) {
        // Integer types
        uint64_t value = fdp.ConsumeIntegral<uint64_t>();
        pattern += std::to_string(value);
        
        // Add unsigned flag sometimes
        if (fdp.ConsumeBool()) {
            pattern += " U";
        }
    } else if (strstr(type_str, "float")) {
        // Float with edge cases
        float value;
        switch (fdp.ConsumeIntegralInRange<uint8_t>(0, 7)) {
            case 0: value = 0.0f; break;
            case 1: value = -0.0f; break;
            case 2: value = INFINITY; break;
            case 3: value = -INFINITY; break;
            case 4: value = NAN; break;
            case 5: value = FLT_MIN; break;
            case 6: value = FLT_MAX; break;
            case 7: value = fdp.ConsumeFloatingPoint<float>(); break;
        }
        pattern += std::to_string(value);
    } else if (strstr(type_str, "double")) {
        // Double with edge cases
        double value;
        switch (fdp.ConsumeIntegralInRange<uint8_t>(0, 7)) {
            case 0: value = 0.0; break;
            case 1: value = -0.0; break;
            case 2: value = INFINITY; break;
            case 3: value = -INFINITY; break;
            case 4: value = NAN; break;
            case 5: value = DBL_MIN; break;
            case 6: value = DBL_MAX; break;
            case 7: value = fdp.ConsumeFloatingPoint<double>(); break;
        }
        pattern += std::to_string(value);
    }
    
    pattern += " Numeric edge case test\n";
    return pattern;
}

// Helper to generate regex pattern (FILE_REGEX)
std::string generate_regex_pattern(FuzzedDataProvider& fdp) {
    std::string pattern;
    
    // Generate offset (0-4096)
    uint32_t offset = fdp.ConsumeIntegralInRange<uint32_t>(0, 4096);
    pattern += std::to_string(offset);
    pattern += " regex";
    
    // Generate regex pattern (may be malformed)
    size_t regex_len = fdp.ConsumeIntegralInRange<size_t>(1, 64);
    std::string regex_str = fdp.ConsumeRandomLengthString(regex_len);
    
    pattern += " \"";
    for (char c : regex_str) {
        if (c == '"' || c == '\\') {
            pattern += '\\';
        }
        pattern += c;
    }
    pattern += "\"";
    
    pattern += " Regex pattern test\n";
    return pattern;
}

// Main function to create comprehensive magic patterns focusing on error conditions
std::vector<std::string> create_error_focused_magic_patterns(FuzzedDataProvider& fdp) {
    std::vector<std::string> patterns;
    
    // Determine how many patterns to generate (1-20)
    size_t num_patterns = fdp.ConsumeIntegralInRange<size_t>(1, 20);
    
    for (size_t i = 0; i < num_patterns && fdp.remaining_bytes() > 0; i++) {
        // Choose pattern type based on weighted distribution
        // More weight on error conditions and uncovered paths
        uint8_t pattern_type = fdp.ConsumeIntegralInRange<uint8_t>(0, 100);
        
        if (pattern_type < 15) { // 15% malformed float/double relations
            const char* float_types[] = {"float", "befloat", "lefloat", "double", "bedouble", "ledouble"};
            const char* type = float_types[fdp.ConsumeIntegralInRange<uint8_t>(0, 5)];
            patterns.push_back(generate_malformed_relation_pattern(fdp, type));
        }
        else if (pattern_type < 25) { // 10% invalid types
            patterns.push_back(generate_invalid_type_pattern(fdp));
        }
        else if (pattern_type < 40) { // 15% SEARCH with range
            patterns.push_back(generate_search_pattern_with_range(fdp));
        }
        else if (pattern_type < 55) { // 15% string patterns with flags
            const char* string_types[] = {"string", "pstring", "bestring16", "lestring16", "octal"};
            const char* type = string_types[fdp.ConsumeIntegralInRange<uint8_t>(0, 4)];
            patterns.push_back(generate_string_pattern_with_flags(fdp, type));
        }
        else if (pattern_type < 75) { // 20% numeric patterns with edge values
            const char* numeric_types[] = {
                "byte", "short", "beshort", "leshort", 
                "long", "belong", "lelong", "melong",
                "quad", "lequad", "bequad",
                "float", "befloat", "lefloat",
                "double", "bedouble", "ledouble"
            };
            const char* type = numeric_types[fdp.ConsumeIntegralInRange<uint8_t>(0, 16)];
            patterns.push_back(generate_numeric_pattern(fdp, type));
        }
        else if (pattern_type < 90) { // 15% regex patterns
            patterns.push_back(generate_regex_pattern(fdp));
        }
        else { // 10% mixed/combination patterns
            // Create a pattern with multiple lines to test multiple types
            std::string combined;
            combined += generate_numeric_pattern(fdp, "byte");
            combined += generate_string_pattern_with_flags(fdp, "string");
            combined += generate_search_pattern_with_range(fdp);
            patterns.push_back(combined);
        }
    }
    
    return patterns;
}

// Helper to create test data that matches the magic patterns
std::vector<uint8_t> create_test_data_for_patterns(FuzzedDataProvider& fdp, const std::vector<std::string>& patterns) {
    std::vector<uint8_t> test_data;
    
    // Determine total test data size (0-4096 bytes)
    size_t data_size = fdp.ConsumeIntegralInRange<size_t>(0, 4096);
    
    // Fill with random bytes
    while (test_data.size() < data_size && fdp.remaining_bytes() > 0) {
        test_data.push_back(fdp.ConsumeIntegral<uint8_t>());
    }
    
    // Also add some structured data based on pattern types
    // For example, add float/double values if patterns include them
    for (const auto& pattern : patterns) {
        if (pattern.find("float") != std::string::npos || pattern.find("double") != std::string::npos) {
            // Add some float/double values to test data
            for (int i = 0; i < 4 && test_data.size() + sizeof(float) <= data_size; i++) {
                float f = fdp.ConsumeFloatingPoint<float>();
                uint8_t* bytes = reinterpret_cast<uint8_t*>(&f);
                test_data.insert(test_data.end(), bytes, bytes + sizeof(float));
            }
            break;
        }
    }
    
    return test_data;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum input for meaningful fuzzing
    // We need enough for magic patterns and test data
    if (size < 100) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Create magic handle with debug flags to potentially trigger more code paths
    int flags = MAGIC_NONE;
    magic_t magic = magic_open(flags);
    if (magic == nullptr) {
        // Try with simple flags
        magic = magic_open(MAGIC_NONE);
        if (magic == nullptr) {
            return 0;
        }
    }
    
    // Enable debug flags to trigger debug output paths in magiccheck
    magic_setflags(magic, MAGIC_DEBUG);
    
    // Create error-focused magic patterns targeting uncovered branches
    std::vector<std::string> patterns = create_error_focused_magic_patterns(fdp);
    if (patterns.empty()) {
        magic_close(magic);
        return 0;
    }
    
    // Combine patterns into a single magic database
    std::string magic_db;
    for (const auto& pattern : patterns) {
        magic_db += pattern;
    }
    
    // Load magic database from buffer
    // Try magic_load_buffers first, fall back to temporary file if needed
    void* magic_buffer_ptr = const_cast<char*>(magic_db.c_str());
    size_t magic_buffer_size = magic_db.size();
    
    void** buffers = static_cast<void**>(malloc(sizeof(void*)));
    size_t* sizes = static_cast<size_t*>(malloc(sizeof(size_t)));
    
    if (buffers == nullptr || sizes == nullptr) {
        free(buffers);
        free(sizes);
        magic_close(magic);
        return 0;
    }
    
    buffers[0] = magic_buffer_ptr;
    sizes[0] = magic_buffer_size;
    
    // Load the custom magic database
    int load_result = magic_load_buffers(magic, buffers, sizes, 1);
    
    free(buffers);
    free(sizes);
    
    if (load_result == -1) {
        // If load fails, we can still test with default database
        // Create a temporary file as fallback
        char magic_template[] = "/tmp/magic_error_fuzz_XXXXXX";
        int magic_fd = mkstemp(magic_template);
        if (magic_fd != -1) {
            write(magic_fd, magic_db.c_str(), magic_db.size());
            fsync(magic_fd);
            close(magic_fd);
            
            magic_load(magic, magic_template);
            unlink(magic_template);
        }
    }
    
    // Create test data for magic_buffer
    std::vector<uint8_t> test_data = create_test_data_for_patterns(fdp, patterns);
    
    // Test magic_buffer with generated test data
    if (!test_data.empty()) {
        const char* result = magic_buffer(magic, test_data.data(), test_data.size());
        // Result can be nullptr, that's OK for testing error paths
        (void)result;
    }
    
    // Test with additional random data chunks to exercise more code paths
    size_t num_additional_tests = fdp.ConsumeIntegralInRange<size_t>(0, 10);
    for (size_t i = 0; i < num_additional_tests && fdp.remaining_bytes() > 0; i++) {
        size_t chunk_size = fdp.ConsumeIntegralInRange<size_t>(0, std::min(fdp.remaining_bytes(), size_t(1024)));
        if (chunk_size > 0) {
            std::vector<uint8_t> chunk = fdp.ConsumeBytes<uint8_t>(chunk_size);
            const char* result = magic_buffer(magic, chunk.data(), chunk.size());
            (void)result;
        }
    }
    
    // Test with edge case buffer sizes (0, 1, very large)
    if (fdp.ConsumeBool()) {
        // Test with zero size buffer
        const char* result = magic_buffer(magic, nullptr, 0);
        (void)result;
    }
    
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 0) {
        // Test with 1-byte buffer
        std::vector<uint8_t> single_byte = fdp.ConsumeBytes<uint8_t>(1);
        const char* result = magic_buffer(magic, single_byte.data(), 1);
        (void)result;
    }
    
    // Clean up
    magic_close(magic);
    
    return 0;
}

} // namespace harness_019
