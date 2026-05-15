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

namespace harness_016 {
/*
 * Fuzzing harness for libmagic targeting magic file compilation and specialized type detection
 * 
 * Coverage gaps identified:
 * 1. magiccheck function with 205/240 blocked branches (softmagic.c:2143)
 * 2. apprentice.c parser functions with 0% coverage: parse_extra, parse_apple, parse_mime, parse_ext, parse_strength, goodchar
 * 3. CDF format functions in cdf.c (0% coverage)
 * 
 * Target: Create harness that exercises magic file compilation and specialized type detection
 * to cover apprentice.c parsing functions and unlock blocked branches in magiccheck.
 * 
 * APIs to target:
 * - magic_compile (Target: Magic file compilation)
 * - magic_load (Required Helper: Load compiled magic)
 * - magic_buffer (Target: Test detection with compiled magic)
 * - magic_open (Required Helper: Initialize)
 * - magic_close (Required Helper: Cleanup)
 * 
 * Key test data to generate:
 * 1. Magic files with Apple CREATOR/TYPE annotations
 * 2. Magic files with MIME type specifications  
 * 3. Magic files with file extension mappings
 * 4. Magic files with strength modifiers
 * 5. Test buffers that trigger specialized type comparisons in magiccheck
 */


// Include libmagic headers

// Include necessary system headers for POSIX functions

// Helper to generate magic pattern with specialized annotations
std::string generate_magic_pattern_with_annotations(FuzzedDataProvider& fdp) {
    std::string pattern;
    
    // Generate offset (0-1024)
    uint32_t offset = fdp.ConsumeIntegralInRange<uint32_t>(0, 1024);
    pattern += std::to_string(offset);
    
    // Choose data type
    const char* types[] = {
        "byte", "short", "long", "string", "beshort", "belong", "leshort", "lelong",
        "ledate", "bedate", "leldate", "beldate", "lequad", "bequad", "float", "double"
    };
    const char* type = types[fdp.ConsumeIntegralInRange<uint8_t>(0, 15)];
    pattern += " ";
    pattern += type;
    
    // Choose relation character
    const char relations[] = {'=', '!', '>', '<', 'x'};
    char reln = relations[fdp.ConsumeIntegralInRange<uint8_t>(0, 4)];
    pattern += reln;
    pattern += " ";
    
    // Generate value based on type
    if (strcmp(type, "string") == 0) {
        // String value
        size_t str_len = fdp.ConsumeIntegralInRange<size_t>(1, 256);
        std::string str_val = fdp.ConsumeBytesAsString(str_len);
        // Escape special characters for magic file format
        for (char c : str_val) {
            if (c == '\\' || c == '"' || c == '\n' || c == '\r' || c == '\t') {
                pattern += '\\';
            }
            pattern += c;
        }
    } else if (strcmp(type, "float") == 0 || strcmp(type, "double") == 0) {
        // Floating point value
        if (strcmp(type, "float") == 0) {
            float val = fdp.ConsumeFloatingPoint<float>();
            pattern += std::to_string(val);
        } else {
            double val = fdp.ConsumeFloatingPoint<double>();
            pattern += std::to_string(val);
        }
    } else {
        // Integer value
        uint64_t val = fdp.ConsumeIntegral<uint64_t>();
        pattern += std::to_string(val);
    }
    
    pattern += " ";
    
    // Generate description
    std::string desc = fdp.ConsumeRandomLengthString(128);
    pattern += desc;
    
    // Randomly add specialized annotations
    int annotation_type = fdp.ConsumeIntegralInRange<int>(0, 4);
    
    switch (annotation_type) {
        case 0: // Apple CREATOR/TYPE annotation
            pattern += "\n!:apple ";
            pattern += fdp.ConsumeBytesAsString(fdp.ConsumeIntegralInRange<size_t>(4, 8));
            break;
        case 1: // MIME type annotation  
            pattern += "\n!:mime ";
            pattern += fdp.ConsumeBytesAsString(fdp.ConsumeIntegralInRange<size_t>(5, 128));
            break;
        case 2: // File extension annotation
            pattern += "\n!:ext ";
            pattern += fdp.ConsumeBytesAsString(fdp.ConsumeIntegralInRange<size_t>(1, 32));
            break;
        case 3: { // Strength modifier annotation
            pattern += "\n!:strength ";
            const char strength_ops[] = {'+', '-', '*', '/'};
            pattern += strength_ops[fdp.ConsumeIntegralInRange<uint8_t>(0, 3)];
            pattern += " ";
            pattern += std::to_string(fdp.ConsumeIntegralInRange<uint8_t>(1, 255));
            break;
        }
        case 4: // Multiple annotations
            pattern += "\n!:apple ";
            pattern += fdp.ConsumeBytesAsString(fdp.ConsumeIntegralInRange<size_t>(4, 8));
            pattern += "\n!:mime ";
            pattern += fdp.ConsumeBytesAsString(fdp.ConsumeIntegralInRange<size_t>(5, 64));
            pattern += "\n!:ext ";
            pattern += fdp.ConsumeBytesAsString(fdp.ConsumeIntegralInRange<size_t>(1, 16));
            break;
    }
    
    return pattern;
}

// Helper to create temporary magic file with targeted patterns
std::string create_magic_file_with_annotations(FuzzedDataProvider& fdp) {
    char magic_template[] = "/tmp/magic_compile_XXXXXX";
    int magic_fd = mkstemp(magic_template);
    if (magic_fd == -1) {
        return "";
    }
    
    std::string magic_content;
    
    // Generate multiple magic pattern lines with various annotations
    int num_patterns = fdp.ConsumeIntegralInRange<int>(1, 50);
    
    for (int i = 0; i < num_patterns && fdp.remaining_bytes() > 100; i++) {
        std::string pattern = generate_magic_pattern_with_annotations(fdp);
        magic_content += pattern + "\n";
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

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum input for meaningful compilation testing
    if (size < 100) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Step 1: magic_open with CHECK flag to enable warnings for parse errors
    int flags = MAGIC_CHECK; // Enable checking to trigger parse_* error paths
    
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
    
    magic_t magic = magic_open(flags);
    if (magic == nullptr) {
        // If magic_open fails with complex flags, try with simple flags
        magic = magic_open(MAGIC_CHECK);
        if (magic == nullptr) {
            return 0; // Can't proceed without magic handle
        }
    }
    
    // Step 2: Create custom magic file with specialized annotations
    std::string magic_file = create_magic_file_with_annotations(fdp);
    if (magic_file.empty()) {
        magic_close(magic);
        return 0;
    }
    
    // Step 3: magic_compile to compile custom magic file
    // This exercises apprentice.c parsing functions: parse_apple, parse_mime, parse_ext, parse_strength
    int compile_result = magic_compile(magic, magic_file.c_str());
    
    // Step 4: magic_load to load compiled magic database (if compilation succeeded)
    if (compile_result == 0) {
        magic_load(magic, magic_file.c_str());
    } else {
        // Even if compilation fails, try to load default or empty database
        // to test magic_buffer with potentially invalid state
        magic_load(magic, NULL);
    }
    
    // Step 5: magic_buffer with test data matching custom magic rules
    // Generate test buffers to trigger magiccheck comparisons
    int num_tests = fdp.ConsumeIntegralInRange<int>(1, 10);
    
    for (int i = 0; i < num_tests && fdp.remaining_bytes() > 10; i++) {
        size_t buffer_size = fdp.ConsumeIntegralInRange<size_t>(0, fdp.remaining_bytes());
        if (buffer_size == 0) {
            break;
        }
        
        std::vector<uint8_t> test_buffer = fdp.ConsumeBytes<uint8_t>(buffer_size);
        
        // Test with various flags to exercise different code paths
        int test_flags = magic_getflags(magic);
        
        // Toggle flags to test different detection modes
        if (fdp.ConsumeBool()) {
            magic_setflags(magic, test_flags | MAGIC_MIME_TYPE);
        }
        if (fdp.ConsumeBool()) {
            magic_setflags(magic, test_flags | MAGIC_APPLE);
        }
        if (fdp.ConsumeBool()) {
            magic_setflags(magic, test_flags | MAGIC_EXTENSION);
        }
        
        // Call magic_buffer - this exercises magiccheck function in softmagic.c
        const char* result = magic_buffer(magic, test_buffer.data(), test_buffer.size());
        (void)result; // Use result to avoid unused variable warning
        
        // Restore original flags
        magic_setflags(magic, test_flags);
    }
    
    // Step 6: Test magic_check function specifically (if available)
    // This directly tests the checking functionality mentioned in coverage gaps
    magic_check(magic, magic_file.c_str());
    
    // Step 7: Clean up temporary file
    unlink(magic_file.c_str());
    
    // Step 8: magic_close to clean up
    magic_close(magic);
    
    return 0;
}

} // namespace harness_016
