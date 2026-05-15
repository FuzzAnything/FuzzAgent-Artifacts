#include <fuzzer/FuzzedDataProvider.h>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>
#include <algorithm>
#include "liblouis.h"

namespace harness_022 {
// liblouis advanced hyphenation fuzzing harness
// Target: lou_hyphenate (48/60 branches undiscovered, 9.62% coverage)
// Focus: Comprehensive testing of all lou_hyphenate code paths including:
//        - Text mode (mode=0) and braille mode (mode=1) with back-translation
//        - Edge cases with input length near HYPHSTRING limit (100)
//        - Tables with/without hyphenation support (hyphenStatesArray)
//        - Various character types (CTC_Letter attribute checks)
//        - hyphenateWord state machine and pattern matching
//        - Mapping logic for braille mode positions
// Based on coverage analysis guidance targeting lou_hyphenate as most critical coverage gap
// Semantic differentiation from existing hyphenation harnesses (002, 006, 016, 018):
//   Deep focus on internal state machine, character attribute checks, and mode-specific paths
//   Systematic testing of all conditions in lou_hyphenate function


// Include liblouis public header
extern "C" {
}

// Log callback to suppress logs during fuzzing
static void avoid_log_callback(logLevels level, const char *msg) {
    (void)level;
    (void)msg;
    // Suppress all logs during fuzzing
}

// Simple ASCII to widechar conversion for testing
static size_t ascii_to_widechar(const char* ascii, widechar* wide, size_t max_len) {
    size_t i;
    for (i = 0; i < max_len && ascii[i] != '\0'; i++) {
        wide[i] = ascii[i];
    }
    return i;
}

// Generate test text with mixed character types to test CTC_Letter attribute checks
// This helps exercise the word boundary detection logic in lou_hyphenate
static std::string generate_mixed_text(FuzzedDataProvider& fdp) {
    std::string text;
    
    // Determine text length (careful to stay under HYPHSTRING limit of 100)
    size_t text_len = fdp.ConsumeIntegralInRange<size_t>(1, 98);  // Leave room for conversion
    
    // Character type distribution
    for (size_t i = 0; i < text_len; i++) {
        uint8_t char_type = fdp.ConsumeIntegral<uint8_t>() % 10;
        
        switch (char_type) {
            case 0: case 1: case 2: case 3: case 4:  // Letters (50% probability)
                text += fdp.ConsumeIntegralInRange<char>('a', 'z');
                break;
            case 5: case 6:  // Digits (20% probability)
                text += fdp.ConsumeIntegralInRange<char>('0', '9');
                break;
            case 7:  // Space (10% probability)
                text += ' ';
                break;
            case 8:  // Punctuation (10% probability)
                text += fdp.PickValueInArray({'.', ',', ';', ':', '!', '?'});
                break;
            case 9:  // Hyphen (10% probability) - important for isHyphen checks
                text += '-';
                break;
        }
    }
    
    return text;
}

// Generate braille dot patterns for testing mode=1 (braille input)
// Braille dot patterns are represented as widechar values with dot bits
static std::vector<widechar> generate_braille_pattern(FuzzedDataProvider& fdp, size_t length) {
    std::vector<widechar> braille(length);
    
    for (size_t i = 0; i < length; i++) {
        // Generate random braille dot pattern (dots 1-8)
        // In liblouis, braille dots are bits 0-7 with LOU_DOTS flag
        uint8_t dots = fdp.ConsumeIntegral<uint8_t>() & 0xFF;
        braille[i] = dots | LOU_DOTS;
    }
    
    return braille;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough for configuration and text
    if (size < 32) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize liblouis logging once
    static bool initialized = false;
    if (!initialized) {
        lou_registerLogCallback(avoid_log_callback);
        lou_setLogLevel(LOU_LOG_FATAL);  // Only show fatal errors
        initialized = true;
    }
    
    // 1. Select tables - mix of hyphenation-enabled and regular tables
    const char* tables[] = {
        // Tables that likely support hyphenation
        "en-us-g2.ctb",      // US English Grade 2
        "en-ueb-g2.ctb",     // Unified English Braille Grade 2
        "de-de-g2.ctb",      // German Grade 2
        "fr-bfu-g2.ctb",     // French Grade 2
        // Tables that may not support hyphenation
        "en-us-comp8.ctb",   // Computer braille
        "en-us-comp6.ctb",   // Computer braille 6-dot
        "tests/tables_backward.ctb",  // Test table
    };
    const size_t num_tables = sizeof(tables) / sizeof(tables[0]);
    size_t table_idx = fdp.ConsumeIntegralInRange<size_t>(0, num_tables - 1);
    const char* table = tables[table_idx];
    
    // 2. Try to load the table using lou_getTable
    // This will compile the table if needed and check hyphenation support
    const void* loaded_table = lou_getTable(table);
    
    // 3. Determine mode: 0 for text, 1 for braille
    // This is critical for testing both code paths
    int mode = fdp.ConsumeBool() ? 0 : 1;
    
    // 4. Generate input based on mode
    std::vector<widechar> inbuf;
    int inlen = 0;
    
    if (mode == 0) {
        // Text mode: generate mixed text with letters, digits, punctuation
        std::string text = generate_mixed_text(fdp);
        
        if (text.empty()) {
            return 0;
        }
        
        // Convert to widechar
        inbuf.resize(text.size() + 1);
        inlen = static_cast<int>(ascii_to_widechar(text.c_str(), inbuf.data(), text.size()));
    } else {
        // Braille mode: generate braille dot patterns
        size_t braille_len = fdp.ConsumeIntegralInRange<size_t>(1, 98);
        std::vector<widechar> braille = generate_braille_pattern(fdp, braille_len);
        
        inbuf = std::move(braille);
        inlen = static_cast<int>(braille_len);
    }
    
    // Critical: Ensure input length is under HYPHSTRING limit (100)
    // lou_hyphenate returns 0 if inlen >= HYPHSTRING
    if (inlen >= 100) {
        // Reduce length to stay under limit
        inlen = 99;
        if (inbuf.size() > 99) {
            inbuf.resize(99);
        }
    }
    
    // 5. Allocate hyphen result buffer (inlen + 1 for null terminator)
    std::vector<char> hyphens(inlen + 1);
    
    // 6. Call lou_hyphenate - PRIMARY TARGET
    int hyphen_result = lou_hyphenate(table, inbuf.data(), inlen, hyphens.data(), mode);
    
    // 7. Additional testing based on hyphenation result
    if (hyphen_result == 1 && inlen > 0) {
        // Analyze hyphen pattern to understand what was found
        int hyphen_count = 0;
        for (int i = 0; i < inlen; i++) {
            if (hyphens[i] == '1' || hyphens[i] == '2') {
                hyphen_count++;
            }
        }
        
        // If we got hyphens and have enough remaining input, test lou_translatePrehyphenated
        if (hyphen_count > 0 && fdp.remaining_bytes() > 16) {
            // Allocate buffers for translation with hyphenation
            int outlen = inlen * 16;
            if (outlen < 16) outlen = 16;
            std::vector<widechar> outbuf(outlen + 1);
            
            // Allocate position arrays
            std::vector<int> outputPos(inlen);
            std::vector<int> inputPos(outlen);
            int cursorPos = -1;
            
            // Allocate hyphen arrays
            std::vector<char> inputHyphens(inlen + 1);
            std::vector<char> outputHyphens(outlen + 1);
            
            // Copy hyphens to inputHyphens
            for (int i = 0; i < inlen; i++) {
                inputHyphens[i] = hyphens[i];
            }
            
            // Test lou_translatePrehyphenated with actual hyphenation results
            int translate_result = lou_translatePrehyphenated(
                table, inbuf.data(), &inlen, outbuf.data(), &outlen,
                NULL, NULL, outputPos.data(), inputPos.data(), &cursorPos,
                inputHyphens.data(), outputHyphens.data(), 0);
            
            // Result is ignored - we're testing for crashes, not correctness
            (void)translate_result;
        }
    }
    
    // 8. Edge case testing: null pointers and invalid inputs
    // Test with NULL table name (should handle gracefully)
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 4) {
        std::vector<char> test_hyphens(10);
        lou_hyphenate(NULL, inbuf.data(), std::min(inlen, 10), test_hyphens.data(), 0);
    }
    
    // 9. Test with zero length input
    if (fdp.ConsumeBool()) {
        std::vector<char> zero_hyphens(1);
        lou_hyphenate(table, inbuf.data(), 0, zero_hyphens.data(), mode);
    }
    
    // 10. Test with exactly HYPHSTRING-1 length (edge case)
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 10) {
        // Create input of exactly 99 characters (HYPHSTRING-1)
        const int edge_len = 99;
        std::vector<widechar> edge_buf(edge_len);
        std::vector<char> edge_hyphens(edge_len + 1);
        
        // Fill with simple pattern
        for (int i = 0; i < edge_len; i++) {
            edge_buf[i] = 'a' + (i % 26);
        }
        
        lou_hyphenate(table, edge_buf.data(), edge_len, edge_hyphens.data(), 0);
    }
    
    // 11. Test mode variations with same input
    if (mode == 0 && inlen > 0 && fdp.remaining_bytes() > 4) {
        // Try same input with mode=1 (if it was text, this will attempt back-translation)
        std::vector<char> mode1_hyphens(inlen + 1);
        lou_hyphenate(table, inbuf.data(), inlen, mode1_hyphens.data(), 1);
    }
    
    // 12. Cleanup
    lou_free();
    
    return 0;
}

} // namespace harness_022
