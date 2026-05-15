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

namespace harness_017 {
/*
 * Fuzzing harness for libmagic library targeting magiccheck function's 
 * 205/240 blocked branches - focusing on comprehensive numeric and string
 * comparison logic with all relation operators and type combinations.
 * 
 * Target: magiccheck function in softmagic.c (lines 2133-2512)
 * Strategy: Generate custom magic patterns with specific byte patterns
 * to trigger all type comparison paths and relation operators.
 * 
 * Coverage gaps to address:
 * 1. All numeric types: FILE_BYTE, FILE_SHORT, FILE_LONG, FILE_QUAD
 * 2. Floating types: FILE_FLOAT, FILE_DOUBLE with all relations
 * 3. String types: FILE_STRING, FILE_PSTRING, FILE_BESTRING16, FILE_LESTRING16
 * 4. Search type: FILE_SEARCH with range constraints
 * 5. Default switch cases and invalid type handling
 * 6. All relation operators: '=', '!', '>', '<', 'x', '&', '^'
 * 7. Unsigned vs signed comparisons for '>' and '<'
 * 
 * APIs to target:
 * - magic_open (Setup)
 * - magic_load (Load default magic database)
 * - magic_setflags (Enable debug flags for coverage)
 * - magic_buffer (Test patterns)
 * - magic_close (Cleanup)
 */


// Include libmagic headers

// Include necessary system headers for POSIX functions

// Helper to generate magic pattern for specific type and relation
std::string generate_magic_pattern_for_type(FuzzedDataProvider& fdp, uint8_t type_id, bool include_invalid = false) {
    std::string pattern;
    
    // Generate offset (0-4096)
    uint32_t offset = fdp.ConsumeIntegralInRange<uint32_t>(0, 4096);
    pattern += std::to_string(offset);
    pattern += " ";
    
    // Map type_id to magic type string
    const char* type_str = nullptr;
    switch (type_id % 20) { // 20 different types to cover
        case 0: type_str = "byte"; break;
        case 1: type_str = "short"; break;
        case 2: type_str = "beshort"; break;
        case 3: type_str = "leshort"; break;
        case 4: type_str = "long"; break;
        case 5: type_str = "belong"; break;
        case 6: type_str = "lelong"; break;
        case 7: type_str = "quad"; break;
        case 8: type_str = "lequad"; break;
        case 9: type_str = "bequad"; break;
        case 10: type_str = "float"; break;
        case 11: type_str = "befloat"; break;
        case 12: type_str = "lefloat"; break;
        case 13: type_str = "double"; break;
        case 14: type_str = "bedouble"; break;
        case 15: type_str = "ledouble"; break;
        case 16: type_str = "string"; break;
        case 17: type_str = "pstring"; break;
        case 18: type_str = "bestring16"; break;
        case 19: type_str = "lestring16"; break;
        default: type_str = "byte"; break;
    }
    
    pattern += type_str;
    
    // Choose relation character
    char reln;
    if (include_invalid && fdp.ConsumeBool()) {
        // Generate invalid relation character (not '=', '!', '>', '<', 'x', '&', '^')
        const char invalid_chars[] = {
            'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'y', 'z',
            'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
            '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
            '@', '#', '$', '%', '*', '(', ')', '_', '+', '-', '[', ']', '{', '}', '|', '\\', ':', ';', '"', '\'', ',', '.', '/', '?', '~', '`'
        };
        reln = invalid_chars[fdp.ConsumeIntegralInRange<uint8_t>(0, sizeof(invalid_chars) - 1)];
    } else {
        // Choose valid relation, covering all 7 operators
        const char valid_reln_chars[] = {'=', '!', '>', '<', 'x', '&', '^'};
        reln = valid_reln_chars[fdp.ConsumeIntegralInRange<uint8_t>(0, 6)];
    }
    
    pattern += reln;
    pattern += ' ';
    
    // Generate value based on type
    if (strcmp(type_str, "string") == 0 || strcmp(type_str, "pstring") == 0 ||
        strcmp(type_str, "bestring16") == 0 || strcmp(type_str, "lestring16") == 0) {
        // String value
        size_t str_len = fdp.ConsumeIntegralInRange<size_t>(1, 256);
        std::string str_val = fdp.ConsumeBytesAsString(str_len);
        
        // Escape special characters for magic file format
        pattern += "\"";
        for (char c : str_val) {
            if (c == '\\' || c == '"' || c == '\n' || c == '\r' || c == '\t') {
                pattern += '\\';
            }
            pattern += c;
        }
        pattern += "\"";
    } else if (strcmp(type_str, "float") == 0 || strcmp(type_str, "befloat") == 0 || 
               strcmp(type_str, "lefloat") == 0) {
        // Floating point value
        float value = fdp.ConsumeFloatingPoint<float>();
        pattern += std::to_string(value);
    } else if (strcmp(type_str, "double") == 0 || strcmp(type_str, "bedouble") == 0 || 
               strcmp(type_str, "ledouble") == 0) {
        // Double precision value
        double value = fdp.ConsumeFloatingPoint<double>();
        pattern += std::to_string(value);
    } else {
        // Integer value (byte, short, long, quad, etc.)
        uint64_t value = fdp.ConsumeIntegral<uint64_t>();
        pattern += std::to_string(value);
        
        // For certain types, add "U" suffix for unsigned comparison
        if ((type_id % 20 == 0 || type_id % 20 == 1 || type_id % 20 == 4 || type_id % 20 == 7) && 
            fdp.ConsumeBool()) {
            pattern += "U";
        }
    }
    
    // Add description
    pattern += " ";
    std::string desc = fdp.ConsumeRandomLengthString(64);
    pattern += desc;
    
    return pattern;
}

// Helper to generate FILE_SEARCH pattern with range constraints
std::string generate_search_pattern(FuzzedDataProvider& fdp) {
    std::string pattern;
    
    // Generate offset
    uint32_t offset = fdp.ConsumeIntegralInRange<uint32_t>(0, 1024);
    pattern += std::to_string(offset);
    pattern += " search";
    
    // Choose relation
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
    
    // Add search range if applicable
    if (fdp.ConsumeBool()) {
        pattern += " >";
        pattern += std::to_string(fdp.ConsumeIntegralInRange<uint32_t>(0, 65536));
    }
    
    // Add description
    pattern += " ";
    std::string desc = fdp.ConsumeRandomLengthString(64);
    pattern += desc;
    
    return pattern;
}

// Helper to create temporary magic file with comprehensive patterns
std::string create_comprehensive_magic_file(FuzzedDataProvider& fdp) {
    char magic_template[] = "/tmp/magic_comprehensive_magiccheck_XXXXXX";
    int magic_fd = mkstemp(magic_template);
    if (magic_fd == -1) {
        return "";
    }
    
    std::string magic_content;
    
    // Generate header
    magic_content += "# Comprehensive magic file for magiccheck testing\n";
    magic_content += "# Generated by fuzzing harness\n\n";
    
    // Generate patterns for all type combinations
    int num_patterns = fdp.ConsumeIntegralInRange<int>(5, 100);
    
    for (int i = 0; i < num_patterns && fdp.remaining_bytes() > 50; i++) {
        bool include_invalid = (i % 10 == 0); // 10% invalid relations
        
        if (fdp.ConsumeBool() && fdp.remaining_bytes() > 100) {
            // Generate search pattern
            magic_content += generate_search_pattern(fdp) + "\n";
        } else {
            // Generate regular type pattern
            uint8_t type_id = fdp.ConsumeIntegral<uint8_t>();
            magic_content += generate_magic_pattern_for_type(fdp, type_id, include_invalid) + "\n";
        }
    }
    
    // Write to temporary file
    ssize_t written = write(magic_fd, magic_content.c_str(), magic_content.size());
    if (written != static_cast<ssize_t>(magic_content.size())) {
        close(magic_fd);
        unlink(magic_template);
        return "";
    }
    
    fsync(magic_fd);
    close(magic_fd);
    
    return std::string(magic_template);
}

// Helper to generate test buffers that match magic patterns
std::vector<uint8_t> generate_test_buffer_for_pattern(FuzzedDataProvider& fdp, const std::string& pattern_type) {
    std::vector<uint8_t> buffer;
    
    if (pattern_type.find("string") != std::string::npos || 
        pattern_type.find("search") != std::string::npos) {
        // Generate string data
        size_t str_len = fdp.ConsumeIntegralInRange<size_t>(1, 1024);
        std::string str_data = fdp.ConsumeBytesAsString(str_len);
        buffer.insert(buffer.end(), str_data.begin(), str_data.end());
    } else if (pattern_type.find("float") != std::string::npos) {
        // Generate float data
        float fval = fdp.ConsumeFloatingPoint<float>();
        const uint8_t* fbytes = reinterpret_cast<const uint8_t*>(&fval);
        buffer.insert(buffer.end(), fbytes, fbytes + sizeof(float));
    } else if (pattern_type.find("double") != std::string::npos) {
        // Generate double data
        double dval = fdp.ConsumeFloatingPoint<double>();
        const uint8_t* dbytes = reinterpret_cast<const uint8_t*>(&dval);
        buffer.insert(buffer.end(), dbytes, dbytes + sizeof(double));
    } else {
        // Generate integer data
        size_t int_size;
        if (pattern_type.find("quad") != std::string::npos) int_size = 8;
        else if (pattern_type.find("long") != std::string::npos) int_size = 4;
        else if (pattern_type.find("short") != std::string::npos) int_size = 2;
        else int_size = 1; // byte
        
        for (size_t j = 0; j < int_size; j++) {
            buffer.push_back(fdp.ConsumeIntegral<uint8_t>());
        }
    }
    
    return buffer;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum input for comprehensive testing
    if (size < 200) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Step 1: magic_open with CHECK flag to enable error reporting
    int flags = MAGIC_CHECK;
    
    // Add additional flags based on fuzzed input
    if (fdp.ConsumeBool()) flags |= MAGIC_NONE;
    if (fdp.ConsumeBool()) flags |= MAGIC_SYMLINK;
    if (fdp.ConsumeBool()) flags |= MAGIC_COMPRESS;
    if (fdp.ConsumeBool()) flags |= MAGIC_DEVICES;
    if (fdp.ConsumeBool()) flags |= MAGIC_MIME_TYPE;
    if (fdp.ConsumeBool()) flags |= MAGIC_CONTINUE;
    if (fdp.ConsumeBool()) flags |= MAGIC_RAW;
    if (fdp.ConsumeBool()) flags |= MAGIC_ERROR;
    if (fdp.ConsumeBool()) flags |= MAGIC_APPLE;
    if (fdp.ConsumeBool()) flags |= MAGIC_EXTENSION;
    
    // Enable DEBUG flag to trigger debug code paths in magiccheck
    flags |= MAGIC_DEBUG;
    
    magic_t magic = magic_open(flags);
    if (magic == nullptr) {
        // Try with simpler flags
        magic = magic_open(MAGIC_CHECK | MAGIC_DEBUG);
        if (magic == nullptr) {
            return 0;
        }
    }
    
    // Step 2: Create comprehensive magic file with patterns for all types
    std::string magic_file = create_comprehensive_magic_file(fdp);
    if (magic_file.empty()) {
        magic_close(magic);
        return 0;
    }
    
    // Step 3: Load the magic database
    if (magic_load(magic, magic_file.c_str()) == -1) {
        // If custom magic fails, load default database
        magic_load(magic, NULL);
    }
    
    // Step 4: Test magic_buffer with various patterns
    int num_tests = fdp.ConsumeIntegralInRange<int>(1, 20);
    
    for (int i = 0; i < num_tests && fdp.remaining_bytes() > 20; i++) {
        // Determine pattern type for this test
        uint8_t pattern_type = fdp.ConsumeIntegral<uint8_t>();
        std::string type_str;
        
        switch (pattern_type % 8) {
            case 0: type_str = "string"; break;
            case 1: type_str = "float"; break;
            case 2: type_str = "double"; break;
            case 3: type_str = "long"; break;
            case 4: type_str = "short"; break;
            case 5: type_str = "quad"; break;
            case 6: type_str = "search"; break;
            case 7: type_str = "bestring16"; break;
        }
        
        // Generate test buffer
        size_t buffer_size = fdp.ConsumeIntegralInRange<size_t>(0, fdp.remaining_bytes());
        if (buffer_size == 0) {
            buffer_size = std::min<size_t>(fdp.remaining_bytes(), 1024);
        }
        
        std::vector<uint8_t> test_buffer = fdp.ConsumeBytes<uint8_t>(buffer_size);
        
        // Test with various flag combinations
        int original_flags = magic_getflags(magic);
        
        // Toggle flags to test different code paths
        int test_flags = original_flags;
        if (fdp.ConsumeBool()) {
            test_flags |= MAGIC_MIME_TYPE;
        }
        if (fdp.ConsumeBool()) {
            test_flags |= MAGIC_DEBUG;
        }
        if (fdp.ConsumeBool()) {
            test_flags |= MAGIC_APPLE;
        }
        if (fdp.ConsumeBool()) {
            test_flags |= MAGIC_EXTENSION;
        }
        
        magic_setflags(magic, test_flags);
        
        // Call magic_buffer - this exercises magiccheck function
        const char* result = magic_buffer(magic, test_buffer.data(), test_buffer.size());
        (void)result; // Use result to avoid unused variable warning
        
        // Restore original flags
        magic_setflags(magic, original_flags);
        
        // Test empty/null buffer edge cases
        if (fdp.ConsumeBool() && fdp.remaining_bytes() > 0) {
            magic_buffer(magic, nullptr, 0);
        }
    }
    
    // Step 5: Test magic_setflags with all possible flag combinations
    // This affects how magiccheck handles certain types
    int flag_combinations[] = {
        MAGIC_NONE,
        MAGIC_CHECK,
        MAGIC_DEBUG,
        MAGIC_MIME_TYPE | MAGIC_DEBUG,
        MAGIC_APPLE | MAGIC_DEBUG,
        MAGIC_EXTENSION | MAGIC_DEBUG,
        MAGIC_CHECK | MAGIC_DEBUG
    };
    
    for (int combo_flag : flag_combinations) {
        if (fdp.ConsumeBool()) {
            magic_setflags(magic, combo_flag);
            
            // Quick test with small buffer
            if (fdp.remaining_bytes() > 10) {
                size_t small_size = fdp.ConsumeIntegralInRange<size_t>(1, 10);
                std::vector<uint8_t> small_buf = fdp.ConsumeBytes<uint8_t>(small_size);
                magic_buffer(magic, small_buf.data(), small_buf.size());
            }
        }
    }
    
    // Step 6: Clean up temporary file
    unlink(magic_file.c_str());
    
    // Step 7: magic_close to clean up
    magic_close(magic);
    
    return 0;
}

} // namespace harness_017
