#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <string>
#include <vector>
#include <algorithm>
#include <fuzzer/FuzzedDataProvider.h>
#include "liblouis/liblouis.h"

namespace harness_003 {
/*
 * liblouis Complex Pattern Expression Matching Fuzzing Harness
 * 
 * This harness specifically targets the pattern_check_expression function in pattern.c
 * which has 59/106 branches blocked (44% coverage). Focuses on exercising complex
 * pattern expression matching in liblouis translation tables.
 * 
 * APIs targeted (from coverage guidance):
 * - lou_translateString: Primary entry point for forward translation
 * - lou_translate: Alternative translation with cursor position
 * - lou_backTranslateString: Backward translation patterns  
 * - lou_hyphenate: Hyphenation pattern testing
 * 
 * Required helper APIs:
 * - lou_getTable/lou_checkTable: Load translation tables with complex patterns
 * - lou_free: Cleanup
 * - lou_registerLogCallback: Suppress logging
 * 
 * Semantic differentiation from existing harnesses:
 * - harness_000.cpp: Tests translation with temporary fuzzer-created tables
 * - harness_001.cpp: Tests translation with real project tables and comprehensive APIs
 * - harness_002.cpp: Focuses on table discovery/metadata operations
 * - This harness: Specifically targets complex pattern expression matching (attributes,
 *   groups, NOT operators, loops, etc.) using tables with rich pattern syntax
 * 
 * Pattern types to exercise (from pattern.c enum):
 * - PTN_ATTRIBUTES: Character attribute matching (vowels, consonants, punctuation)
 * - PTN_ANY: Match any character
 * - PTN_CHARS: Specific character matching
 * - PTN_GROUP: Group patterns
 * - PTN_NOT: NOT operator patterns
 * - PTN_ONE_MORE, PTN_ZERO_MORE: Loop patterns (+ and * operators)
 * - PTN_OPTIONAL: Optional patterns (? operator)
 * - PTN_ALTERNATE: Alternation patterns (| operator)
 * - PTN_END_OF_INPUT: End of input matching
 */


extern "C" {
}

// Simple string to widechar conversion for fuzzing
static int simple_string_to_widechar(const char* input, size_t input_len, widechar* output) {
    for (size_t i = 0; i < input_len && i < 1024; i++) {
        output[i] = (widechar)(unsigned char)input[i];
    }
    return (int)std::min(input_len, (size_t)1024);
}

// Log callback to suppress output during fuzzing
static void fuzz_log_callback(logLevels level, const char* msg) {
    (void)level;
    (void)msg;
    // Suppress all logging during fuzzing
}

// Tables with rich pattern expressions for testing pattern_check_expression
// Selected based on presence of complex pattern syntax (attributes, groups, NOT operators, etc.)
static const char* pattern_rich_tables[] = {
    // afr-za-g2.ctb has extensive pattern matching with attributes, NOT operators, groups
    "/root/src/liblouis/tables/afr-za-g2.ctb",
    
    // en-g3.ctb has pattern matching with attributes
    "/root/src/liblouis/tables/en-g3.ctb",
    
    // en-ueb-g2.ctb has pattern matching
    "/root/src/liblouis/tables/en-ueb-g2.ctb",
    
    // fr-bfu-g2.ctb has pattern matching with attributes
    "/root/src/liblouis/tables/fr-bfu-g2.ctb",
    
    // hu-hu-g2.ctb has pattern matching with attributes
    "/root/src/liblouis/tables/hu-hu-g2.ctb",
    
    // th-g2.ctb has pattern matching with attributes
    "/root/src/liblouis/tables/th-g2.ctb",
    
    // ru-litbrl.ctb has pattern matching
    "/root/src/liblouis/tables/ru-litbrl.ctb",
    
    // zh-tw.ctb has pattern matching
    "/root/src/liblouis/tables/zh-tw.ctb"
};

static const size_t num_pattern_tables = sizeof(pattern_rich_tables) / sizeof(pattern_rich_tables[0]);

// Sample text patterns designed to trigger different pattern expression types
// Based on analysis of pattern syntax in tables like afr-za-g2.ctb
static const char* pattern_test_strings[] = {
    // For attribute matching (vowels, consonants, punctuation)
    "aeiouAEIOU",  // Vowels - should match attribute 1 in afr-za-g2.ctb
    "bcdfgBCDFG",  // Consonants - should match attribute 2
    "!?.,;:",      // Punctuation - should match attribute 3
    
    // For specific pattern matches in tables
    "about",       // Should match "about" pattern in en-us-g2.ctb
    "again",       // Should match "again" pattern
    "always",      // Should match "always" pattern
    
    // For complex pattern syntax with NOT operators
    "xyzXYZ",      // Non-vowels for NOT vowel patterns
    "123456",      // Digits for NOT letter patterns
    
    // For group patterns and alternations
    "en",          // Should match "en" pattern in many tables
    "er",          // Should match "er" pattern
    "ing",         // Should match "ing" pattern
    
    // For loop patterns (ONE_MORE, ZERO_MORE)
    "aaa",         // Repeated characters for +/* patterns
    "bbb",
    
    // Mixed content for comprehensive testing
    "Hello world! This is a test.",
    "Testing 123 punctuation, and symbols!",
    "aeioubcd123!?.",
    
    // Empty string for edge cases
    ""
};

static const size_t num_test_strings = sizeof(pattern_test_strings) / sizeof(pattern_test_strings[0]);

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Initialize logging once
    static int logging_initialized = 0;
    if (!logging_initialized) {
        lou_registerLogCallback(fuzz_log_callback);
        logging_initialized = 1;
    }
    
    // Need minimum input for meaningful testing
    // We need: table selection + operation selection + text pattern selection
    if (size < 8) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // 1. Select which pattern-rich table to use
    uint8_t table_index = fdp.ConsumeIntegralInRange<uint8_t>(0, num_pattern_tables - 1);
    const char* table_name = pattern_rich_tables[table_index];
    
    // 2. Load and check the table
    if (lou_checkTable(table_name) == 0) {
        // Table not found or invalid - try to get it directly
        const void* table = lou_getTable(table_name);
        if (table == NULL) {
            // No valid table available for this run
            return 0;
        }
    }
    
    // 3. Determine which translation operations to test
    uint8_t operation_mask = fdp.ConsumeIntegral<uint8_t>();
    
    // 4. Get text input - either from fuzzer data or predefined patterns
    uint8_t text_source = fdp.ConsumeIntegral<uint8_t>() % 3;
    std::string text_input;
    
    if (text_source == 0) {
        // Use fuzzer-provided text
        text_input = fdp.ConsumeRemainingBytesAsString();
        if (text_input.empty()) {
            lou_free();
            return 0;
        }
    } else if (text_source == 1) {
        // Use predefined pattern test strings
        uint8_t pattern_index = fdp.ConsumeIntegralInRange<uint8_t>(0, num_test_strings - 1);
        text_input = pattern_test_strings[pattern_index];
        
        // Also consume some fuzzer bytes to influence the pattern
        size_t extra_bytes = fdp.ConsumeIntegralInRange<size_t>(0, std::min(fdp.remaining_bytes(), (size_t)100));
        if (extra_bytes > 0) {
            std::string extra = fdp.ConsumeBytesAsString(extra_bytes);
            // Modify pattern based on fuzzer input
            if (!extra.empty()) {
                text_input += " " + extra;
            }
        }
    } else {
        // Mix of fuzzer data and predefined patterns
        uint8_t pattern_index = fdp.ConsumeIntegralInRange<uint8_t>(0, num_test_strings - 1);
        text_input = pattern_test_strings[pattern_index];
        
        // Append fuzzer data
        std::string fuzzer_text = fdp.ConsumeRemainingBytesAsString();
        if (!fuzzer_text.empty()) {
            text_input += " " + fuzzer_text;
        }
    }
    
    // 5. Convert text to widechar for liblouis APIs
    if (text_input.empty()) {
        lou_free();
        return 0;
    }
    
    size_t max_input_len = std::min(text_input.size(), (size_t)1024);
    widechar* inputText = (widechar*)malloc((max_input_len + 1) * sizeof(widechar));
    if (!inputText) {
        lou_free();
        return 0;
    }
    
    int inputLen = simple_string_to_widechar(text_input.c_str(), max_input_len, inputText);
    if (inputLen <= 0) {
        free(inputText);
        lou_free();
        return 0;
    }
    
    // 6. Prepare output buffers
    int outputLen = inputLen * 16;  // Allow for expansion in translation
    widechar* outputText = (widechar*)malloc((outputLen + 1) * sizeof(widechar));
    if (!outputText) {
        free(inputText);
        lou_free();
        return 0;
    }
    
    int backOutputLen = inputLen * 16;
    widechar* backOutputText = (widechar*)malloc((backOutputLen + 1) * sizeof(widechar));
    if (!backOutputText) {
        free(inputText);
        free(outputText);
        lou_free();
        return 0;
    }
    
    // 7. Test different translation APIs based on operation mask
    // Each bit in operation_mask corresponds to a different API to test
    
    // Bit 0: lou_translateString (forward translation)
    if (operation_mask & 0x01) {
        int currentInputLen = inputLen;
        int currentOutputLen = outputLen;
        lou_translateString(table_name, inputText, &currentInputLen, outputText, &currentOutputLen,
                           NULL, NULL, ucBrl);
    }
    
    // Bit 1: lou_translate (with cursor position)
    if (operation_mask & 0x02) {
        int currentInputLen = inputLen;
        int currentOutputLen = outputLen;
        int cursorPos = 0;
        int* cursorPosPtr = NULL;
        
        // Sometimes test with cursor position tracking
        if (fdp.ConsumeBool() && inputLen > 0) {
            cursorPos = fdp.ConsumeIntegralInRange<int>(0, inputLen - 1);
            cursorPosPtr = &cursorPos;
        }
        
        lou_translate(table_name, inputText, &currentInputLen, outputText, &currentOutputLen,
                     NULL, NULL, NULL, NULL, cursorPosPtr, ucBrl);
    }
    // Bit 2: lou_backTranslateString (backward translation)
    if (operation_mask & 0x04) {
        int currentInputLen = inputLen;
        int currentOutputLen = backOutputLen;
        lou_backTranslateString(table_name, inputText, &currentInputLen, backOutputText, &currentOutputLen,
                               NULL, NULL, ucBrl);
    }
    
    // Bit 3: lou_hyphenate (hyphenation patterns)
    if (operation_mask & 0x08) {
        int currentInputLen = inputLen;
        // Hyphenation buffer needs one more element than input length
        char* hyphens = (char*)malloc((currentInputLen + 1) * sizeof(char));
        if (hyphens) {
            lou_hyphenate(table_name, inputText, currentInputLen, hyphens, ucBrl);
            free(hyphens);
        }
    }
    
    // Bit 4: Test with different modes
    if (operation_mask & 0x10) {
        int currentInputLen = inputLen;
        int currentOutputLen = outputLen;
        
        // Test with different mode flags
        int mode = 0;
        if (fdp.ConsumeBool()) mode |= compbrlAtCursor;
        if (fdp.ConsumeBool()) mode |= dotsIO;
        if (fdp.ConsumeBool()) mode |= compbrlLeftCursor;
        
        lou_translateString(table_name, inputText, &currentInputLen, outputText, &currentOutputLen,
                           NULL, NULL, mode);
    }
    
    // Bit 5: Test multiple passes with same input
    if (operation_mask & 0x20) {
        // Do forward then backward translation to test round-trip patterns
        int tempInLen = inputLen;
        int tempOutLen = outputLen;
        int tempBackLen = backOutputLen;
        
        // Forward
        lou_translateString(table_name, inputText, &tempInLen, outputText, &tempOutLen,
                           NULL, NULL, ucBrl);
        
        // Backward (using output as input)
        tempInLen = tempOutLen;
        tempBackLen = inputLen * 16;
        lou_backTranslateString(table_name, outputText, &tempInLen, backOutputText, &tempBackLen,
                               NULL, NULL, ucBrl);
    }
    
    // 8. Cleanup
    free(inputText);
    free(outputText);
    free(backOutputText);
    
    // Free table resources
    lou_free();
    
    return 0;
}

} // namespace harness_003
