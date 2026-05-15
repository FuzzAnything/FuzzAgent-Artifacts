#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <vector>
#include <string>
#include <algorithm>
#include <fuzzer/FuzzedDataProvider.h>
#include "liblouis.h"

namespace harness_008 {
// Specialized liblouis hyphenation fuzzing harness targeting deep coverage of hyphenation subsystem
// Harness_008: Focuses exclusively on lou_hyphenate (11.54% coverage, 47 undiscovered branches), 
//              hyphenateWord internal function (0% coverage), compileHyphenation (43 blocked branches)
// Strategy: Comprehensive hyphenation testing with diverse table configurations, input patterns,
//           and edge cases to uncover hidden state space in hyphenation logic




// Log callback to suppress output during fuzzing
static void fuzz_log_callback(logLevels level, const char* message) {
    (void)level;
    (void)message;
    // Suppress all logging during fuzzing
}

// Convert std::string to widechar buffer (simple ASCII/Latin-1 conversion)
static widechar* string_to_widechar(const std::string& str, int* out_len) {
    if (str.empty()) {
        *out_len = 0;
        return nullptr;
    }
    
    widechar* wbuf = (widechar*)malloc((str.size() + 1) * sizeof(widechar));
    if (!wbuf) {
        *out_len = 0;
        return nullptr;
    }
    
    for (size_t i = 0; i < str.size(); i++) {
        wbuf[i] = (widechar)(unsigned char)str[i];
    }
    wbuf[str.size()] = 0;
    *out_len = (int)str.size();
    return wbuf;
}

// Generate hyphenation table string based on fuzzer input
static std::string generate_hyphenation_table_rules(FuzzedDataProvider& fdp) {
    std::string rules;
    
    // Choose table language/variant based on fuzzer input
    uint8_t table_type = fdp.ConsumeIntegral<uint8_t>() % 8;
    
    switch (table_type) {
        case 0:
            // Basic English hyphenation rules
            rules = "LANGUAGE en\n"
                    "LEFTHYPHENMIN 2\n"
                    "RIGHTHYPHENMIN 3\n"
                    "HYPHEN a1b1c\n"
                    "HYPHEN a1ba\n"
                    "HYPHEN a1b2a\n"
                    "HYPHEN a1b2c\n"
                    "HYPHEN a1ca\n"
                    "HYPHEN a1cb\n"
                    "HYPHEN a1c2a\n"
                    "HYPHEN a1c2b\n"
                    "HYPHEN a2ba\n"
                    "HYPHEN a2bc\n"
                    "HYPHEN a2c2a\n"
                    "HYPHEN a2c2b\n"
                    "HYPHEN a3b1a\n"
                    "HYPHEN a3b1c\n"
                    "HYPHEN a3b2a\n"
                    "HYPHEN a3b2c\n"
                    "HYPHEN a3c1a\n"
                    "HYPHEN a3c1b\n"
                    "HYPHEN a3c2a\n"
                    "HYPHEN a3c2b\n"
                    "HYPHEN b1a1b\n"
                    "HYPHEN b1a1c\n"
                    "HYPHEN b1a2b\n"
                    "HYPHEN b1a2c\n"
                    "HYPHEN b1c1a\n"
                    "HYPHEN b1c1b\n"
                    "HYPHEN b1c2a\n"
                    "HYPHEN b1c2b\n"
                    "HYPHEN b2a1b\n"
                    "HYPHEN b2a1c\n"
                    "HYPHEN b2a2b\n"
                    "HYPHEN b2a2c\n"
                    "HYPHEN b2c1a\n"
                    "HYPHEN b2c1b\n"
                    "HYPHEN b2c2a\n"
                    "HYPHEN b2c2b\n"
                    "HYPHEN c1a1b\n"
                    "HYPHEN c1a1c\n"
                    "HYPHEN c1a2b\n"
                    "HYPHEN c1a2c\n"
                    "HYPHEN c1b1a\n"
                    "HYPHEN c1b1b\n"
                    "HYPHEN c1b2a\n"
                    "HYPHEN c1b2b\n"
                    "HYPHEN c2a1b\n"
                    "HYPHEN c2a1c\n"
                    "HYPHEN c2a2b\n"
                    "HYPHEN c2a2c\n"
                    "HYPHEN c2b1a\n"
                    "HYPHEN c2b1b\n"
                    "HYPHEN c2b2a\n"
                    "HYPHEN c2b2b\n";
            break;
            
        case 1:
            // German hyphenation rules (more complex)
            rules = "LANGUAGE de\n"
                    "LEFTHYPHENMIN 2\n"
                    "RIGHTHYPHENMIN 2\n"
                    "HYPHEN a1b1c1d1e1f1g1h1i1j1k1l1m1n1o1p1q1r1s1t1u1v1w1x1y1z\n"
                    "HYPHEN a2b2c2d2e2f2g2h2i2j2k2l2m2n2o2p2q2r2s2t2u2v2w2x2y2z\n"
                    "HYPHEN a3b3c3d3e3f3g3h3i3j3k3l3m3n3o3p3q3r3s3t3u3v3w3x3y3z\n"
                    "HYPHEN a4b4c4d4e4f4g4h4i4j4k4l4m4n4o4p4q4r4s4t4u4v4w4x4y4z\n"
                    "HYPHEN .a1 .b1 .c1 .d1 .e1 .f1 .g1 .h1 .i1 .j1 .k1 .l1 .m1 .n1 .o1 .p1 .q1 .r1 .s1 .t1 .u1 .v1 .w1 .x1 .y1 .z1\n"
                    "HYPHEN a1. b1. c1. d1. e1. f1. g1. h1. i1. j1. k1. l1. m1. n1. o1. p1. q1. r1. s1. t1. u1. v1. w1. x1. y1. z1.\n";
            break;
            
        case 2:
            // Simple pattern-based rules
            rules = "LANGUAGE simple\n"
                    "LEFTHYPHENMIN 1\n"
                    "RIGHTHYPHENMIN 1\n"
                    "HYPHEN .test1 .test2 .test3\n"
                    "HYPHEN test1. test2. test3.\n"
                    "HYPHEN 1test 2test 3test\n"
                    "HYPHEN test1 test2 test3\n";
            break;
            
        case 3:
            // Complex patterns with numbers
            rules = "LANGUAGE complex\n"
                    "LEFTHYPHENMIN 3\n"
                    "RIGHTHYPHENMIN 3\n"
                    "HYPHEN 1a2b3c4d5e6f7g8h9i0j\n"
                    "HYPHEN a1b2c3d4e5f6g7h8i9j0\n"
                    "HYPHEN 9z8y7x6w5v4u3t2s1r0q\n";
            break;
            
        case 4:
            // Edge case rules (very short patterns)
            rules = "LANGUAGE edge\n"
                    "LEFTHYPHENMIN 1\n"
                    "RIGHTHYPHENMIN 1\n"
                    "HYPHEN a\n"
                    "HYPHEN b\n"
                    "HYPHEN c\n"
                    "HYPHEN .\n"
                    "HYPHEN 1\n"
                    "HYPHEN 2\n";
            break;
            
        case 5:
            // Mixed character rules
            rules = "LANGUAGE mixed\n"
                    "LEFTHYPHENMIN 2\n"
                    "RIGHTHYPHENMIN 2\n"
                    "HYPHEN a-b c-d e-f g-h i-j\n"
                    "HYPHEN 1-2 3-4 5-6 7-8 9-0\n"
                    "HYPHEN .-. ,-, ;-; :-: !-!\n";
            break;
            
        case 6:
            // Random rules from fuzzer input
            if (fdp.remaining_bytes() > 100) {
                rules = "LANGUAGE random\n"
                        "LEFTHYPHENMIN ";
                rules += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 5));
                rules += "\nRIGHTHYPHENMIN ";
                rules += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 5));
                rules += "\n";
                
                // Generate random hyphenation patterns
                int num_patterns = fdp.ConsumeIntegralInRange<int>(1, 20);
                for (int i = 0; i < num_patterns && fdp.remaining_bytes() > 10; i++) {
                    rules += "HYPHEN ";
                    size_t pattern_len = fdp.ConsumeIntegralInRange<size_t>(1, 50);
                    std::string pattern = fdp.ConsumeBytesAsString(pattern_len);
                    // Sanitize pattern for table format
                    for (char& c : pattern) {
                        if (c == '\n' || c == '\r') c = ' ';
                    }
                    rules += pattern;
                    rules += "\n";
                }
            } else {
                // Fallback to basic rules
                rules = "LANGUAGE en\nLEFTHYPHENMIN 2\nRIGHTHYPHENMIN 3\nHYPHEN a1b1c\n";
            }
            break;
            
        case 7:
            // Empty or minimal rules (edge case)
            rules = "LANGUAGE empty\n";
            if (fdp.ConsumeBool()) {
                rules += "LEFTHYPHENMIN 1\nRIGHTHYPHENMIN 1\n";
            }
            break;
    }
    
    return rules;
}

// Generate test text for hyphenation with diverse characteristics
static std::string generate_hyphenation_text(FuzzedDataProvider& fdp) {
    if (fdp.remaining_bytes() < 4) {
        return "test";
    }
    
    uint8_t text_type = fdp.ConsumeIntegral<uint8_t>() % 10;
    
    switch (text_type) {
        case 0:
            // Simple English words
            return "hyphenation testing with multiple words";
            
        case 1:
            // Long compound word (German-style)
            return "Donaudampfschifffahrtsgesellschaftskapitaenswitwe";
            
        case 2:
            // Mixed case words
            return "HyPheNaTion TeStInG With MiXeD CaSe";
            
        case 3:
            // Words with numbers
            return "test123 word456 hyphen789 at2024";
            
        case 4:
            // Words with special characters
            return "test-word hyphen'ated a.b.c word-with-hyphens";
            
        case 5:
            // Non-ASCII characters
            return "café naïve façade naïve résumé";
            
        case 6:
            // Very short text
            return fdp.ConsumeBytesAsString(fdp.ConsumeIntegralInRange<size_t>(0, 3));
            
        case 7:
            // Repetitive patterns
            return "aaaa bbbb cccc dddd eeee ffff gggg hhhh";
            
        case 8:
            // Random text from fuzzer
            if (fdp.remaining_bytes() > 10) {
                size_t len = fdp.ConsumeIntegralInRange<size_t>(1, 256);
                return fdp.ConsumeBytesAsString(len);
            }
            break;
            
        case 9:
            // Text with spaces and punctuation
            return "This is a test. With punctuation! And, commas; colons: etc.";
    }
    
    return "default test text for hyphenation";
}

// Generate table name for hyphenation testing
static std::string generate_table_name(FuzzedDataProvider& fdp) {
    uint8_t table_choice = fdp.ConsumeIntegral<uint8_t>() % 6;
    
    switch (table_choice) {
        case 0:
            return "en-us-g2.ctb";  // Common table with hyphenation support
        case 1:
            return "en-gb-g2.ctb";  // Another English table
        case 2:
            return "de-de-g2.ctb";  // German table
        case 3:
            return "en-comp8.ctb";  // Computer braille
        case 4:
            return "nemeth.ctb";    // Math/technical
        case 5:
            // Random table name
            if (fdp.remaining_bytes() > 4) {
                size_t len = fdp.ConsumeIntegralInRange<size_t>(1, 32);
                std::string name = fdp.ConsumeBytesAsString(len);
                if (!name.empty() && name.find('.') == std::string::npos) {
                    name += ".ctb";
                }
                return name;
            }
            break;
    }
    
    return "en-us-g2.ctb";
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size check - need enough for configuration
    if (size < 32) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Register log callback to suppress output
    lou_registerLogCallback(fuzz_log_callback);
    lou_setLogLevel(LOU_LOG_ERROR);  // Only log errors
    
    // === PHASE 1: SETUP ===
    // Step 1: lou_setDataPath (optional)
    char* data_path_result = nullptr;
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 10) {
        std::string data_path;
        if (fdp.ConsumeBool()) {
            data_path = "/usr/share/liblouis";  // Default path
        } else {
            size_t path_len = fdp.ConsumeIntegralInRange<size_t>(1, 128);
            data_path = fdp.ConsumeBytesAsString(path_len);
        }
        data_path_result = lou_setDataPath(data_path.c_str());
        (void)data_path_result;  // May be NULL for invalid paths
    }
    
    // Choose between inline compilation and precompiled tables
    bool use_inline_rules = fdp.ConsumeBool();
    const void* loaded_table = nullptr;
    std::string table_list;
    
    if (use_inline_rules && fdp.remaining_bytes() > 100) {
        // Step 2a: lou_compileString with inline hyphenation rules
        std::string hyphenation_rules = generate_hyphenation_table_rules(fdp);
        
        // Generate a unique table name for inline compilation
        std::string inline_table_name = "inline_hyphen_";
        inline_table_name += std::to_string(fdp.ConsumeIntegral<uint32_t>());
        inline_table_name += ".ctb";
        
        // Compile the inline rules
        int compile_result = lou_compileString(inline_table_name.c_str(), hyphenation_rules.c_str());
        (void)compile_result;
        
        table_list = inline_table_name;
    } else {
        // Step 2b: lou_getTable with hyphenation tables
        std::string table_name = generate_table_name(fdp);
        loaded_table = lou_getTable(table_name.c_str());
        table_list = table_name;
    }
    
    // Step 3: lou_checkTable validation
    int check_result = lou_checkTable(table_list.c_str());
    (void)check_result;
    
    // If table loading failed, use a default table
    if (!loaded_table && !use_inline_rules) {
        table_list = "en-us-g2.ctb";
        loaded_table = lou_getTable(table_list.c_str());
    }
    
    // === PHASE 2: OPERATION - Intensive hyphenation testing ===
    if (fdp.remaining_bytes() > 16) {
        // Determine number of hyphenation operations to perform
        int num_hyphenation_tests = fdp.ConsumeIntegralInRange<int>(1, 20);
        
        for (int test_idx = 0; test_idx < num_hyphenation_tests && fdp.remaining_bytes() > 8; test_idx++) {
            // Generate test text
            std::string text_str = generate_hyphenation_text(fdp);
            if (text_str.empty()) {
                continue;
            }
            
            int text_len = 0;
            widechar* text_buf = string_to_widechar(text_str, &text_len);
            if (!text_buf || text_len == 0) {
                free(text_buf);
                continue;
            }
            
            // Create hyphen array (must be at least text_len + 1 for null terminator)
            char* hyphens = (char*)malloc((text_len + 1) * sizeof(char));
            if (!hyphens) {
                free(text_buf);
                continue;
            }
            
            // Choose hyphenation mode with various flags
            int mode = 0;
            
            // Consume mode from fuzzer input
            if (fdp.remaining_bytes() > 4) {
                mode = fdp.ConsumeIntegral<int>();
                
                // Apply common mode flags based on fuzzer input
                if (fdp.ConsumeBool()) mode |= compbrlAtCursor;
                if (fdp.ConsumeBool()) mode |= dotsIO;
                if (fdp.ConsumeBool()) mode |= noUndefined;
                if (fdp.ConsumeBool()) mode |= partialTrans;
                if (fdp.ConsumeBool()) mode |= noContractions;
                
                // Test both text mode (0) and braille mode (1) for hyphenation
                if (fdp.ConsumeBool()) {
                    // Test braille mode hyphenation (requires back-translation)
                    mode |= 1;  // Set braille mode bit
                }
            }
            
            // Ensure text length is within HYPHSTRING limit (100 from source code)
            if (text_len >= 100) {
                // Truncate to fit within limit
                text_len = 99;
                text_buf[text_len] = 0;
            }
            
            // Perform hyphenation
            int hyphen_result = lou_hyphenate(table_list.c_str(), text_buf, text_len, hyphens, mode);
            (void)hyphen_result;
            
            // Additional testing: analyze hyphen results
            if (hyphen_result && text_len > 0) {
                // Check hyphen array content
                for (int i = 0; i < text_len; i++) {
                    char h = hyphens[i];
                    // Hyphens should contain '0', '1', or '2' according to documentation
                    (void)h;
                }
            }
            
            // Test edge cases with the same text
            if (fdp.remaining_bytes() > 4) {
                // Test with different modes
                int alt_mode = fdp.ConsumeIntegral<int>();
                int alt_result = lou_hyphenate(table_list.c_str(), text_buf, text_len, hyphens, alt_mode);
                (void)alt_result;
                
                // Test with null hyphens pointer (should fail)
                if (fdp.ConsumeBool()) {
                    int null_result = lou_hyphenate(table_list.c_str(), text_buf, text_len, nullptr, mode);
                    (void)null_result;
                }
                
                // Test with empty/invalid table
                if (fdp.ConsumeBool()) {
                    int invalid_result = lou_hyphenate("invalid_table.ctb", text_buf, text_len, hyphens, mode);
                    (void)invalid_result;
                }
            }
            
            // Cleanup for this test
            free(hyphens);
            free(text_buf);
            
            // Break early if we're running out of input
            if (fdp.remaining_bytes() < 16) {
                break;
            }
        }
    }
    
    // === PHASE 3: ADDITIONAL HYPHENATION VARIATIONS ===
    // Test hyphenation with very specific patterns that might trigger edge cases
    if (fdp.remaining_bytes() > 8) {
        const char* special_patterns[] = {
            "a", "ab", "abc", "abcd", "abcde",
            "test-test", "word-with-hyphen", "multiple-hyphens-here",
            "1234567890", "abcdefghijklmnopqrstuvwxyz",
            "A", "AB", "ABC", "TESTING", "HYPHENATION",
            ".", "..", "...", "....", ".....",
            "-", "--", "---", "----", "-----",
            " ", "  ", "   ", "    ", "     ",
        };
        
        int num_special_tests = fdp.ConsumeIntegralInRange<int>(0, 10);
        for (int i = 0; i < num_special_tests && fdp.remaining_bytes() > 4; i++) {
            int pattern_idx = fdp.ConsumeIntegralInRange<int>(0, sizeof(special_patterns)/sizeof(special_patterns[0]) - 1);
            const char* pattern = special_patterns[pattern_idx];
            
            int pattern_len = (int)strlen(pattern);
            if (pattern_len >= 100) continue;  // Skip if too long
            
            widechar* pattern_buf = string_to_widechar(pattern, &pattern_len);
            if (!pattern_buf || pattern_len == 0) {
                free(pattern_buf);
                continue;
            }
            
            char* pattern_hyphens = (char*)malloc((pattern_len + 1) * sizeof(char));
            if (pattern_hyphens) {
                int pattern_mode = fdp.ConsumeIntegral<int>() & 0x1;  // Only test mode 0 or 1
                int pattern_result = lou_hyphenate(table_list.c_str(), pattern_buf, pattern_len, pattern_hyphens, pattern_mode);
                (void)pattern_result;
                free(pattern_hyphens);
            }
            
            free(pattern_buf);
        }
    }
    
    // === PHASE 4: CLEANUP ===
    // Step: lou_free to release all resources
    lou_free();
    
    return 0;
}

} // namespace harness_008
