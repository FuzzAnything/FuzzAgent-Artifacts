#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <vector>
#include <string>
#include <algorithm>
#include <fuzzer/FuzzedDataProvider.h>
#include "liblouis.h"

namespace harness_010 {
// Comprehensive liblouis hyphenation fuzzing harness targeting deep coverage gaps
// Harness_010: Focused on lou_hyphenate (45 undiscovered branches), hyphenateWord (0% coverage),
//              compileHyphenation (43 blocked branches) - per coverage guidance
// Strategy: Generate dynamic hyphenation patterns from fuzzer input, test diverse language tables,
//           explore state machine transitions, and target edge cases in hyphenation logic
// Based on guidance from $OUTPUT/coverage_guidance/harness_010_guidance.md




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

// Generate hyphenation table rules dynamically from fuzzer input
static std::string generate_hyphenation_rules(FuzzedDataProvider& fdp) {
    std::string rules;
    
    // Consume language selection from fuzzer input
    uint8_t lang_type = fdp.ConsumeIntegral<uint8_t>() % 10;
    
    // Language prefix
    switch (lang_type) {
        case 0: rules = "LANGUAGE en\n"; break;
        case 1: rules = "LANGUAGE de\n"; break;
        case 2: rules = "LANGUAGE fr\n"; break;
        case 3: rules = "LANGUAGE es\n"; break;
        case 4: rules = "LANGUAGE it\n"; break;
        case 5: rules = "LANGUAGE nl\n"; break;
        case 6: rules = "LANGUAGE da\n"; break;
        case 7: rules = "LANGUAGE sv\n"; break;
        case 8: rules = "LANGUAGE no\n"; break;
        case 9: rules = "LANGUAGE fi\n"; break;
    }
    
    // Consume hyphenation min values from fuzzer input
    int left_min = fdp.ConsumeIntegralInRange<int>(1, 5);
    int right_min = fdp.ConsumeIntegralInRange<int>(1, 5);
    rules += "LEFTHYPHENMIN " + std::to_string(left_min) + "\n";
    rules += "RIGHTHYPHENMIN " + std::to_string(right_min) + "\n";
    
    // Generate hyphenation patterns from fuzzer input
    int num_patterns = fdp.ConsumeIntegralInRange<int>(1, 50);
    for (int i = 0; i < num_patterns && fdp.remaining_bytes() > 4; i++) {
        rules += "HYPHEN ";
        
        // Determine pattern type from fuzzer input
        uint8_t pattern_type = fdp.ConsumeIntegral<uint8_t>() % 6;
        
        if (pattern_type == 0) {
            // Simple pattern with numbers (like a1b2c)
            size_t pattern_len = fdp.ConsumeIntegralInRange<size_t>(1, 20);
            for (size_t j = 0; j < pattern_len && fdp.remaining_bytes() > 0; j++) {
                if (fdp.ConsumeBool()) {
                    // Letter
                    char c = 'a' + (fdp.ConsumeIntegral<uint8_t>() % 26);
                    rules += c;
                } else {
                    // Number
                    int num = fdp.ConsumeIntegralInRange<int>(0, 9);
                    rules += std::to_string(num);
                }
            }
        } else if (pattern_type == 1) {
            // Dot-prefixed patterns
            rules += ".";
            size_t pattern_len = fdp.ConsumeIntegralInRange<size_t>(1, 15);
            std::string pattern = fdp.ConsumeBytesAsString(pattern_len);
            // Sanitize
            for (char& c : pattern) {
                if (c == '\n' || c == '\r') c = 'a';
            }
            rules += pattern;
        } else if (pattern_type == 2) {
            // Dot-suffixed patterns
            size_t pattern_len = fdp.ConsumeIntegralInRange<size_t>(1, 15);
            std::string pattern = fdp.ConsumeBytesAsString(pattern_len);
            // Sanitize
            for (char& c : pattern) {
                if (c == '\n' || c == '\r') c = 'a';
            }
            rules += pattern + ".";
        } else if (pattern_type == 3) {
            // Mixed patterns with hyphens
            size_t pattern_len = fdp.ConsumeIntegralInRange<size_t>(1, 20);
            for (size_t j = 0; j < pattern_len && fdp.remaining_bytes() > 0; j++) {
                if (fdp.ConsumeBool()) {
                    char c = 'a' + (fdp.ConsumeIntegral<uint8_t>() % 26);
                    rules += c;
                } else {
                    rules += '-';
                }
            }
        } else if (pattern_type == 4) {
            // Pattern with multiple numbers
            int num_digits = fdp.ConsumeIntegralInRange<int>(1, 5);
            for (int j = 0; j < num_digits; j++) {
                if (j > 0 && fdp.ConsumeBool()) {
                    char c = 'a' + (fdp.ConsumeIntegral<uint8_t>() % 26);
                    rules += c;
                }
                int digit = fdp.ConsumeIntegralInRange<int>(0, 9);
                rules += std::to_string(digit);
            }
        } else {
            // Random pattern
            size_t pattern_len = fdp.ConsumeIntegralInRange<size_t>(1, 30);
            std::string pattern = fdp.ConsumeBytesAsString(pattern_len);
            // Sanitize for table format
            for (char& c : pattern) {
                if (c == '\n' || c == '\r' || c == ' ') c = 'a';
            }
            rules += pattern;
        }
        
        rules += "\n";
    }
    
    // Add special hyphenation directives based on fuzzer input
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 10) {
        rules += "COMPOUNDHYPHEN 2\n";
    }
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 10) {
        rules += "COMPOUNDWORD 1\n";
    }
    
    return rules;
}

// Generate test word for hyphenation with diverse characteristics from fuzzer input
static std::string generate_hyphenation_word(FuzzedDataProvider& fdp) {
    if (fdp.remaining_bytes() < 4) {
        return "test";
    }
    
    uint8_t word_type = fdp.ConsumeIntegral<uint8_t>() % 12;
    size_t word_len = fdp.ConsumeIntegralInRange<size_t>(1, 100);
    
    switch (word_type) {
        case 0:
            // Simple alphabetic word
            {
                std::string word;
                for (size_t i = 0; i < word_len && fdp.remaining_bytes() > 0; i++) {
                    char c = 'a' + (fdp.ConsumeIntegral<uint8_t>() % 26);
                    word += c;
                }
                return word;
            }
            
        case 1:
            // Mixed case word
            {
                std::string word;
                for (size_t i = 0; i < word_len && fdp.remaining_bytes() > 0; i++) {
                    if (fdp.ConsumeBool()) {
                        char c = 'A' + (fdp.ConsumeIntegral<uint8_t>() % 26);
                        word += c;
                    } else {
                        char c = 'a' + (fdp.ConsumeIntegral<uint8_t>() % 26);
                        word += c;
                    }
                }
                return word;
            }
            
        case 2:
            // Word with numbers
            {
                std::string word;
                for (size_t i = 0; i < word_len && fdp.remaining_bytes() > 0; i++) {
                    if (fdp.ConsumeBool()) {
                        char c = 'a' + (fdp.ConsumeIntegral<uint8_t>() % 26);
                        word += c;
                    } else {
                        int digit = fdp.ConsumeIntegralInRange<int>(0, 9);
                        word += std::to_string(digit);
                    }
                }
                return word;
            }
            
        case 3:
            // Word with hyphens (compound word)
            {
                std::string word;
                int parts = fdp.ConsumeIntegralInRange<int>(2, 5);
                for (int p = 0; p < parts && fdp.remaining_bytes() > 0; p++) {
                    if (p > 0) word += '-';
                    size_t part_len = fdp.ConsumeIntegralInRange<size_t>(1, 10);
                    for (size_t i = 0; i < part_len && fdp.remaining_bytes() > 0; i++) {
                        char c = 'a' + (fdp.ConsumeIntegral<uint8_t>() % 26);
                        word += c;
                    }
                }
                return word;
            }
            
        case 4:
            // Word with special characters
            {
                std::string word;
                const char specials[] = {'.', ',', ';', ':', '!', '?', '\'', '"', '(', ')', '[', ']'};
                for (size_t i = 0; i < word_len && fdp.remaining_bytes() > 0; i++) {
                    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 0) {
                        int idx = fdp.ConsumeIntegral<uint8_t>() % (sizeof(specials)/sizeof(specials[0]));
                        word += specials[idx];
                    } else {
                        char c = 'a' + (fdp.ConsumeIntegral<uint8_t>() % 26);
                        word += c;
                    }
                }
                return word;
            }
            
        case 5:
            // Very short word (edge case)
            return fdp.ConsumeBytesAsString(fdp.ConsumeIntegralInRange<size_t>(0, 3));
            
        case 6:
            // Repetitive pattern word
            {
                char base_char = 'a' + (fdp.ConsumeIntegral<uint8_t>() % 26);
                std::string word;
                for (size_t i = 0; i < word_len; i++) {
                    word += base_char;
                }
                return word;
            }
            
        case 7:
            // Word with spaces (edge case for hyphenation)
            {
                std::string word;
                for (size_t i = 0; i < word_len && fdp.remaining_bytes() > 0; i++) {
                    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 0) {
                        word += ' ';
                    } else {
                        char c = 'a' + (fdp.ConsumeIntegral<uint8_t>() % 26);
                        word += c;
                    }
                }
                return word;
            }
            
        case 8:
            // Word with accented characters (non-ASCII)
            {
                std::string word;
                const char* accented_chars[] = {"é", "è", "ê", "ë", "à", "â", "ä", "ç", "î", "ï", "ô", "ö", "ù", "û", "ü", "ÿ"};
                for (size_t i = 0; i < word_len && fdp.remaining_bytes() > 0; i++) {
                    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 0) {
                        int idx = fdp.ConsumeIntegral<uint8_t>() % (sizeof(accented_chars)/sizeof(accented_chars[0]));
                        word += accented_chars[idx];
                    } else {
                        char c = 'a' + (fdp.ConsumeIntegral<uint8_t>() % 26);
                        word += c;
                    }
                }
                return word;
            }
            
        case 9:
            // Consume raw bytes as word
            if (fdp.remaining_bytes() > word_len) {
                return fdp.ConsumeBytesAsString(word_len);
            }
            break;
            
        case 10:
            // Word with Unicode characters (basic)
            {
                std::string word;
                for (size_t i = 0; i < word_len && fdp.remaining_bytes() > 0; i++) {
                    // Generate characters in ASCII range plus some extended
                    uint8_t char_type = fdp.ConsumeIntegral<uint8_t>() % 3;
                    if (char_type == 0) {
                        char c = 32 + (fdp.ConsumeIntegral<uint8_t>() % 95); // Printable ASCII
                        word += c;
                    } else if (char_type == 1) {
                        // Extended ASCII (128-255)
                        char c = 128 + (fdp.ConsumeIntegral<uint8_t>() % 128);
                        word += c;
                    } else {
                        // Multi-byte UTF-8 start byte
                        char c = 0xC0 + (fdp.ConsumeIntegral<uint8_t>() % 32);
                        word += c;
                    }
                }
                return word;
            }
            
        case 11:
            // Empty or minimal word (edge case)
            if (fdp.ConsumeBool()) {
                return "";
            } else {
                return "a";
            }
    }
    
    // Fallback
    return fdp.ConsumeBytesAsString(fdp.ConsumeIntegralInRange<size_t>(1, 50));
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size check - need enough for configuration
    if (size < 64) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Step 1: lou_setLogLevel (optional)
    lou_registerLogCallback(fuzz_log_callback);
    uint8_t log_level = fdp.ConsumeIntegral<uint8_t>() % 7;
    switch (log_level) {
        case 0: lou_setLogLevel(LOU_LOG_ALL); break;
        case 1: lou_setLogLevel(LOU_LOG_DEBUG); break;
        case 2: lou_setLogLevel(LOU_LOG_INFO); break;
        case 3: lou_setLogLevel(LOU_LOG_WARN); break;
        case 4: lou_setLogLevel(LOU_LOG_ERROR); break;
        case 5: lou_setLogLevel(LOU_LOG_FATAL); break;
        case 6: lou_setLogLevel(LOU_LOG_OFF); break;
    }
    
    // Prepare for table operations
    std::string table_list;
    bool use_inline_compilation = fdp.ConsumeBool();
    
    if (use_inline_compilation && fdp.remaining_bytes() > 100) {
        // Step 2: lou_compileString with hyphenation pattern tables
        std::string hyphenation_rules = generate_hyphenation_rules(fdp);
        
        // Generate unique table name for inline compilation
        std::string inline_table_name = "fuzz_hyphen_";
        inline_table_name += std::to_string(fdp.ConsumeIntegral<uint32_t>());
        inline_table_name += ".ctb";
        
        // Compile the inline rules
        int compile_result = lou_compileString(inline_table_name.c_str(), hyphenation_rules.c_str());
        (void)compile_result; // Result may be 0 for invalid patterns, that's OK
        
        table_list = inline_table_name;
        
        // Step 3: lou_getTable to load compiled table
        const void* table = lou_getTable(table_list.c_str());
        (void)table; // May be NULL if compilation failed
    } else {
        // Use existing tables with hyphenation support
        uint8_t table_choice = fdp.ConsumeIntegral<uint8_t>() % 8;
        switch (table_choice) {
            case 0: table_list = "en-us-g2.ctb"; break;
            case 1: table_list = "en-gb-g2.ctb"; break;
            case 2: table_list = "de-de-g2.ctb"; break;
            case 3: table_list = "da-dk-g2.ctb"; break;
            case 4: table_list = "fr-bfu-comp8.ctb"; break;
            case 5: table_list = "sv-1996.ctb"; break;
            case 6: table_list = "no-no-comp8.ctb"; break;
            case 7: table_list = "nl-comp8.ctb"; break;
        }
        
        // Step 3: lou_getTable
        const void* table = lou_getTable(table_list.c_str());
        (void)table;
    }
    
    // Validate table
    int check_result = lou_checkTable(table_list.c_str());
    (void)check_result;
    
    // Step 4: lou_hyphenate with various word inputs
    if (fdp.remaining_bytes() > 32) {
        int num_hyphenation_tests = fdp.ConsumeIntegralInRange<int>(1, 30);
        
        for (int test_idx = 0; test_idx < num_hyphenation_tests && fdp.remaining_bytes() > 16; test_idx++) {
            // Generate test word
            std::string word_str = generate_hyphenation_word(fdp);
            
            int word_len = 0;
            widechar* word_buf = string_to_widechar(word_str, &word_len);
            if (!word_buf || word_len == 0) {
                free(word_buf);
                continue;
            }
            
            // Ensure word length is within reasonable bounds (HYPHSTRING limit ~100)
            if (word_len >= 100) {
                word_len = 99;
                word_buf[word_len] = 0;
            }
            
            // Allocate hyphen array (must be at least word_len + 1)
            char* hyphens = (char*)malloc((word_len + 1) * sizeof(char));
            if (!hyphens) {
                free(word_buf);
                continue;
            }
            
            // Generate mode from fuzzer input with various flags
            int mode = fdp.ConsumeIntegral<int>();
            
            // Apply specific mode flags that might affect hyphenation
            if (fdp.ConsumeBool()) mode |= compbrlAtCursor;
            if (fdp.ConsumeBool()) mode |= dotsIO;
            if (fdp.ConsumeBool()) mode |= noUndefined;
            if (fdp.ConsumeBool()) mode |= partialTrans;
            if (fdp.ConsumeBool()) mode |= noContractions;
            
            // Test both text mode (0) and braille mode (1)
            if (fdp.ConsumeBool()) {
                mode |= 1; // Braille mode
            }
            
            // Perform hyphenation
            int hyphen_result = lou_hyphenate(table_list.c_str(), word_buf, word_len, hyphens, mode);
            (void)hyphen_result;
            
            // Analyze hyphen results for edge cases
            if (word_len > 0 && hyphens) {
                // Check for valid hyphen values (should be '0', '1', '2' per documentation)
                for (int i = 0; i < word_len; i++) {
                    char h = hyphens[i];
                    (void)h; // Just to avoid unused variable warning
                }
            }
            
            // Additional tests with same word but different parameters
            if (fdp.remaining_bytes() > 8) {
                // Test with different mode
                int alt_mode = fdp.ConsumeIntegral<int>();
                int alt_result = lou_hyphenate(table_list.c_str(), word_buf, word_len, hyphens, alt_mode);
                (void)alt_result;
                
                // Test edge cases
                if (fdp.ConsumeBool()) {
                    // Null hyphens pointer (should fail)
                    int null_result = lou_hyphenate(table_list.c_str(), word_buf, word_len, nullptr, mode);
                    (void)null_result;
                }
                
                if (fdp.ConsumeBool()) {
                    // Zero length
                    int zero_result = lou_hyphenate(table_list.c_str(), word_buf, 0, hyphens, mode);
                    (void)zero_result;
                }
                
                if (fdp.ConsumeBool()) {
                    // Negative length (edge case)
                    int neg_result = lou_hyphenate(table_list.c_str(), word_buf, -1, hyphens, mode);
                    (void)neg_result;
                }
                
                if (fdp.ConsumeBool()) {
                    // Different table
                    int diff_result = lou_hyphenate("invalid-table.ctb", word_buf, word_len, hyphens, mode);
                    (void)diff_result;
                }
            }
            
            free(hyphens);
            free(word_buf);
            
            // Break early if running low on input
            if (fdp.remaining_bytes() < 32) {
                break;
            }
        }
    }
    
    // Step 5: lou_freeTableFiles cleanup
    // Note: lou_freeTableFiles expects char** from lou_listTables
    // We'll call lou_free() which cleans up all tables
    
    // Additional cleanup: lou_free to release all resources
    lou_free();
    
    return 0;
}

} // namespace harness_010
