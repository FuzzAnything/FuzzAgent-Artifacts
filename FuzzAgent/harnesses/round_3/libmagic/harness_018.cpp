#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>
#include "magic.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

namespace harness_018 {
/*
 * Fuzzing harness for libmagic library targeting magiccheck function's 
 * complex magic file patterns to exercise remaining blocked branches (206/240).
 * 
 * Target: magiccheck function in softmagic.c - Phase 2 deep coverage exploration
 * focusing on complex magic file patterns.
 *
 * SPECIFIC UNCOVERED PATHS IDENTIFIED:
 * 1. FILE_DER (DER certificate comparisons) - line 2400, 0 hits
 * 2. FILE_GUID (GUID comparisons) - line 2410, 0 hits  
 * 3. FILE_REGEX with different regex match results (lines 2386-2392)
 * 4. FILE_SEARCH with different search results
 * 5. Float/Double comparisons with different relation operators ('x', '!', '=', '>', '<') - some have 0 hits
 * 6. Default case for invalid types - line 2414, 0 hits
 *
 * Strategy: Generate and test complex magic patterns focusing on these uncovered paths.
 * Use magic_load_buffers to load custom magic patterns and test with generated data.
 *
 * APIs to target:
 * - magic_open (Setup)
 * - magic_load_buffers (Primary for custom magic patterns)
 * - magic_buffer (Test patterns)
 * - magic_close (Cleanup)
 *
 * KEY PATTERNS TO GENERATE:
 * 1. Magic patterns for DER certificate type detection
 * 2. Magic patterns for GUID/UUID detection
 * 3. Regex patterns with various match groups and outcomes
 * 4. Search patterns with byte offsets and lengths
 * 5. Float/double comparison patterns with all relational operators
 */


// Include libmagic headers

// Include necessary system headers for POSIX functions

// Helper to generate DER certificate magic pattern
std::string generate_der_pattern(FuzzedDataProvider& fdp) {
    std::string pattern;
    
    // Generate offset (0-4096)
    uint32_t offset = fdp.ConsumeIntegralInRange<uint32_t>(0, 4096);
    pattern += std::to_string(offset);
    pattern += " der ";
    
    // Generate DER OID or other certificate pattern
    // Example: "0 der 0x30 0x82 Certificate"
    std::string der_bytes;
    size_t der_len = fdp.ConsumeIntegralInRange<size_t>(1, 16);
    
    for (size_t i = 0; i < der_len; i++) {
        if (i > 0) pattern += " ";
        pattern += "0x";
        char hex[3];
        uint8_t byte = fdp.ConsumeIntegral<uint8_t>();
        snprintf(hex, sizeof(hex), "%02x", byte);
        pattern += hex;
    }
    
    // Add description
    pattern += " ";
    std::string desc = fdp.ConsumeRandomLengthString(32);
    pattern += "DER Certificate: ";
    pattern += desc;
    pattern += "\n";
    
    return pattern;
}

// Helper to generate GUID pattern
std::string generate_guid_pattern(FuzzedDataProvider& fdp) {
    std::string pattern;
    
    // Generate offset (0-4096)
    uint32_t offset = fdp.ConsumeIntegralInRange<uint32_t>(0, 4096);
    pattern += std::to_string(offset);
    pattern += " guid ";
    
    // Generate GUID pattern in magic format
    // GUID format: XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX
    pattern += "\"";
    for (int i = 0; i < 36; i++) {
        if (i == 8 || i == 13 || i == 18 || i == 23) {
            pattern += "-";
        } else {
            // Generate hex character (0-9, a-f, A-F) or wildcard
            uint8_t choice = fdp.ConsumeIntegralInRange<uint8_t>(0, 2);
            if (choice == 0) {
                pattern += fdp.ConsumeIntegralInRange<char>('0', '9');
            } else if (choice == 1) {
                pattern += fdp.ConsumeIntegralInRange<char>('a', 'f');
            } else {
                pattern += '?'; // Wildcard character
            }
        }
    }
    pattern += "\"";
    
    // Add description
    pattern += " ";
    std::string desc = fdp.ConsumeRandomLengthString(32);
    pattern += "GUID/UUID: ";
    pattern += desc;
    pattern += "\n";
    
    return pattern;
}

// Helper to generate regex pattern
std::string generate_regex_pattern(FuzzedDataProvider& fdp) {
    std::string pattern;
    
    // Generate offset (0-4096)
    uint32_t offset = fdp.ConsumeIntegralInRange<uint32_t>(0, 4096);
    pattern += std::to_string(offset);
    pattern += " regex";
    
    // Choose relation ('/' or '=')
    char reln = fdp.ConsumeBool() ? '/' : '=';
    pattern += reln;
    pattern += ' ';
    
    // Generate regex pattern with various components
    pattern += "\"";
    
    // Generate regex length (1-128)
    size_t regex_len = fdp.ConsumeIntegralInRange<size_t>(1, 128);
    std::string regex_str = fdp.ConsumeBytesAsString(regex_len);
    
    // Escape special characters for magic file format
    for (char c : regex_str) {
        if (c == '\\' || c == '"' || c == '\n' || c == '\r' || c == '\t') {
            pattern += '\\';
        }
        pattern += c;
    }
    
    pattern += "\"";
    
    // Add description
    pattern += " ";
    std::string desc = fdp.ConsumeRandomLengthString(32);
    pattern += "Regex pattern: ";
    pattern += desc;
    pattern += "\n";
    
    return pattern;
}

// Helper to generate search pattern with range constraints
std::string generate_search_pattern_with_range(FuzzedDataProvider& fdp) {
    std::string pattern;
    
    // Generate offset (0-4096)
    uint32_t offset = fdp.ConsumeIntegralInRange<uint32_t>(0, 4096);
    pattern += std::to_string(offset);
    pattern += " search";
    
    // Choose relation ('/' or '=')
    char reln = fdp.ConsumeBool() ? '/' : '=';
    pattern += reln;
    pattern += ' ';
    
    // Generate search string
    size_t search_len = fdp.ConsumeIntegralInRange<size_t>(1, 128);
    std::string search_str = fdp.ConsumeBytesAsString(search_len);
    
    // Escape special characters
    pattern += "\"";
    for (char c : search_str) {
        if (c == '\\' || c == '"' || c == '\n' || c == '\r' || c == '\t') {
            pattern += '\\';
        }
        pattern += c;
    }
    pattern += "\"";
    
    // Add search range constraints (>, <, >=, <=)
    if (fdp.ConsumeBool()) {
        pattern += " >";
        pattern += std::to_string(fdp.ConsumeIntegralInRange<uint32_t>(0, 65536));
    }
    if (fdp.ConsumeBool()) {
        pattern += " <";
        pattern += std::to_string(fdp.ConsumeIntegralInRange<uint32_t>(0, 65536));
    }
    
    // Add description
    pattern += " ";
    std::string desc = fdp.ConsumeRandomLengthString(32);
    pattern += "Search pattern: ";
    pattern += desc;
    pattern += "\n";
    
    return pattern;
}

// Helper to generate float/double pattern with all relation operators
std::string generate_float_pattern(FuzzedDataProvider& fdp, const char* type_str) {
    std::string pattern;
    
    // Generate offset (0-4096)
    uint32_t offset = fdp.ConsumeIntegralInRange<uint32_t>(0, 4096);
    pattern += std::to_string(offset);
    pattern += " ";
    pattern += type_str;
    
    // Choose relation character from all possible operators
    const char reln_chars[] = {'=', '!', '>', '<', 'x', '&', '^'};
    char reln = reln_chars[fdp.ConsumeIntegralInRange<uint8_t>(0, 6)];
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
    
    // Add description
    pattern += " ";
    std::string desc = fdp.ConsumeRandomLengthString(32);
    pattern += "Float/Double ";
    pattern += type_str;
    pattern += " test: ";
    pattern += desc;
    pattern += "\n";
    
    return pattern;
}

// Helper to generate invalid type pattern (for default case coverage)
std::string generate_invalid_pattern(FuzzedDataProvider& fdp) {
    std::string pattern;
    
    // Generate offset (0-4096)
    uint32_t offset = fdp.ConsumeIntegralInRange<uint32_t>(0, 4096);
    pattern += std::to_string(offset);
    pattern += " ";
    
    // Generate invalid type name
    std::string invalid_type = fdp.ConsumeRandomLengthString(8);
    pattern += invalid_type;
    
    // Generate invalid relation character
    const char invalid_chars[] = {
        'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'y', 'z',
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
        '@', '#', '$', '%', '*', '(', ')', '_', '+', '-', '[', ']', '{', '}', '|', '\\', ':', ';', '"', '\'', ',', '.', '/', '?', '~', '`'
    };
    char reln = invalid_chars[fdp.ConsumeIntegralInRange<uint8_t>(0, sizeof(invalid_chars) - 1)];
    pattern += reln;
    pattern += ' ';
    
    // Generate random value
    pattern += std::to_string(fdp.ConsumeIntegral<uint32_t>());
    
    // Add description
    pattern += " ";
    std::string desc = fdp.ConsumeRandomLengthString(32);
    pattern += "Invalid type test: ";
    pattern += desc;
    pattern += "\n";
    
    return pattern;
}

// Helper to create magic database with comprehensive patterns
std::vector<std::string> create_magic_patterns(FuzzedDataProvider& fdp) {
    std::vector<std::string> patterns;
    
    // Generate various pattern types
    size_t num_patterns = fdp.ConsumeIntegralInRange<size_t>(1, 20);
    
    for (size_t i = 0; i < num_patterns && fdp.remaining_bytes() > 100; i++) {
        uint8_t pattern_type = fdp.ConsumeIntegral<uint8_t>() % 10;
        
        switch (pattern_type) {
            case 0:
                // DER pattern
                patterns.push_back(generate_der_pattern(fdp));
                break;
            case 1:
                // GUID pattern
                patterns.push_back(generate_guid_pattern(fdp));
                break;
            case 2:
                // Regex pattern
                patterns.push_back(generate_regex_pattern(fdp));
                break;
            case 3:
                // Search pattern with range
                patterns.push_back(generate_search_pattern_with_range(fdp));
                break;
            case 4:
                // Float pattern
                patterns.push_back(generate_float_pattern(fdp, "float"));
                break;
            case 5:
                // Double pattern
                patterns.push_back(generate_float_pattern(fdp, "double"));
                break;
            case 6:
                // BEFloat pattern
                patterns.push_back(generate_float_pattern(fdp, "befloat"));
                break;
            case 7:
                // BEDouble pattern
                patterns.push_back(generate_float_pattern(fdp, "bedouble"));
                break;
            case 8:
                // Invalid pattern (for default case)
                patterns.push_back(generate_invalid_pattern(fdp));
                break;
            case 9:
                // Default to search pattern
                patterns.push_back(generate_search_pattern_with_range(fdp));
                break;
        }
    }
    
    return patterns;
}

// Helper to create test data for magic_buffer
std::vector<uint8_t> create_test_data(FuzzedDataProvider& fdp, const std::vector<std::string>& patterns) {
    std::vector<uint8_t> test_data;
    
    // Generate test data based on pattern types
    size_t data_size = fdp.ConsumeIntegralInRange<size_t>(64, 4096);
    
    // Consume some bytes for fixed test data
    size_t consume_size = std::min(data_size, fdp.remaining_bytes());
    std::vector<uint8_t> fuzzed_data = fdp.ConsumeBytes<uint8_t>(consume_size);
    test_data.insert(test_data.end(), fuzzed_data.begin(), fuzzed_data.end());
    
    // Add some specific patterns for DER, GUID, etc.
    // For DER: Add some ASN.1 structure
    if (fdp.ConsumeBool() && test_data.size() < data_size) {
        // Add DER-like structure (0x30 = SEQUENCE)
        test_data.push_back(0x30);
        test_data.push_back(fdp.ConsumeIntegral<uint8_t>());
    }
    
    // For GUID: Add GUID-like structure
    if (fdp.ConsumeBool() && test_data.size() + 16 <= data_size) {
        // Add 16 bytes that could be a GUID
        for (int i = 0; i < 16; i++) {
            test_data.push_back(fdp.ConsumeIntegral<uint8_t>());
        }
    }
    
    // Fill remaining space with random data
    while (test_data.size() < data_size && fdp.remaining_bytes() > 0) {
        test_data.push_back(fdp.ConsumeIntegral<uint8_t>());
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
    
    // Create magic handle
    int flags = MAGIC_NONE;
    magic_t magic = magic_open(flags);
    if (magic == nullptr) {
        // Try with simple flags
        magic = magic_open(MAGIC_NONE);
        if (magic == nullptr) {
            return 0;
        }
    }
    
    // Enable debug flags to potentially trigger more code paths
    magic_setflags(magic, MAGIC_DEBUG);
    
    // Create comprehensive magic patterns
    std::vector<std::string> patterns = create_magic_patterns(fdp);
    if (patterns.empty()) {
        magic_close(magic);
        return 0;
    }
    
    // Combine patterns into a single magic database
    std::string magic_db;
    for (const auto& pattern : patterns) {
        magic_db += pattern;
    }
    
    // Prepare buffers for magic_load_buffers
    // Note: magic_load_buffers expects array of void* pointers and array of sizes
    // We'll create a single buffer for simplicity
    void* magic_buffer_ptr = const_cast<char*>(magic_db.c_str());
    size_t magic_buffer_size = magic_db.size();
    
    // Create arrays for magic_load_buffers
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
        // or continue with whatever was loaded
    }
    
    // Create test data for magic_buffer
    std::vector<uint8_t> test_data = create_test_data(fdp, patterns);
    
    // Test magic_buffer with generated test data
    if (!test_data.empty()) {
        const char* result = magic_buffer(magic, test_data.data(), test_data.size());
        // Result can be nullptr, that's OK for testing error paths
        (void)result;
    }
    
    // Test with additional random data chunks
    size_t num_additional_tests = fdp.ConsumeIntegralInRange<size_t>(0, 10);
    for (size_t i = 0; i < num_additional_tests && fdp.remaining_bytes() > 0; i++) {
        size_t chunk_size = fdp.ConsumeIntegralInRange<size_t>(0, std::min(fdp.remaining_bytes(), size_t(1024)));
        if (chunk_size > 0) {
            std::vector<uint8_t> chunk = fdp.ConsumeBytes<uint8_t>(chunk_size);
            const char* result = magic_buffer(magic, chunk.data(), chunk.size());
            (void)result;
        }
    }
    
    // Clean up
    magic_close(magic);
    
    return 0;
}

} // namespace harness_018
