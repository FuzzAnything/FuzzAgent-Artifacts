#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <vector>
#include <string>
#include <algorithm>
#include <fuzzer/FuzzedDataProvider.h>
#include "liblouis.h"

namespace harness_011 {
// Targeted liblouis hyphenation harness focusing on hyphenateWord coverage
// Harness_011: Specifically targets hyphenateWord (0% coverage, 26 blocked branches)
// Strategy: Use tables with hyphenation patterns (.dic includes) to populate hyphenStatesArray
//           Test lou_hyphenate with various word lengths to avoid early returns
// Based on coverage guidance from $OUTPUT/coverage_guidance/harness_011_guidance.md




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

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for meaningful testing
    if (size < 32) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize liblouis logging once
    static int initialized = 0;
    if (!initialized) {
        lou_registerLogCallback(fuzz_log_callback);
        initialized = 1;
    }
    
    // Tables that include hyphenation dictionaries (with .dic includes)
    // These should have hyphenStatesArray populated after compilation
    const char* tables_with_hyphenation[] = {
        "da-dk-g26_1993.ctb",      // Danish Grade 2 with da-dk-g2.dic include
        "da-dk-g26.ctb",           // Danish Grade 2 with hyphenation
        "da-dk-g28_1993.ctb",      // Danish Grade 2 with hyphenation (8-dot)
        "da-dk-g28.ctb",           // Danish Grade 2 with hyphenation
        "en-g3.ctb",               // English Grade 3 with en-g3-patterns.dic
        "da-dk-g26-lit_1993.ctb",  // Danish literary with hyphenation
        NULL
    };
    
    // Consume table selection from fuzzer input
    uint8_t table_idx = fdp.ConsumeIntegral<uint8_t>() % 6;
    const char* table_name = tables_with_hyphenation[table_idx];
    
    // Step 1: lou_getTable to load translation table containing hyphenation patterns
    // This should trigger compileHyphenation if table has .dic includes
    const void* table = lou_getTable(table_name);
    if (table == NULL) {
        // Table loading failed, try fallback tables
        const char* fallback_tables[] = {
            "en-us-g2.ctb",        // US English Grade 2 (may have some hyphenation)
            "en-ueb-g2.ctb",       // Unified English Braille Grade 2
            "de-g0.utb",           // German Grade 0
            NULL
        };
        
        uint8_t fallback_idx = fdp.ConsumeIntegral<uint8_t>() % 3;
        table_name = fallback_tables[fallback_idx];
        table = lou_getTable(table_name);
        if (table == NULL) {
            // Still can't load table, can't proceed
            return 0;
        }
    }
    
    // Step 2: Test lou_hyphenate with various word inputs
    // Consume number of test iterations from fuzzer input
    int num_iterations = fdp.ConsumeIntegralInRange<int>(1, 20);
    
    for (int iter = 0; iter < num_iterations && fdp.remaining_bytes() > 16; iter++) {
        // Consume word length from fuzzer input
        // Important: Test with varying lengths to avoid (wordSize + 3) > MAXSTRING early return
        int word_len = fdp.ConsumeIntegralInRange<int>(1, 100);
        
        // Consume word content from fuzzer input
        std::string word_str;
        if (fdp.remaining_bytes() < (size_t)word_len) {
            word_len = fdp.remaining_bytes();
        }
        if (word_len > 0) {
            word_str = fdp.ConsumeBytesAsString(word_len);
        } else {
            word_str = fdp.ConsumeRandomLengthString(100);
            word_len = word_str.length();
        }
        
        // Ensure word length is within reasonable bounds for hyphenation logic
        // HYPHSTRING limit is around 100 characters
        if (word_len > 100) {
            word_len = 100;
            word_str = word_str.substr(0, 100);
        }
        
        // Convert to widechar format for lou_hyphenate
        int wbuf_len = 0;
        widechar* word_buf = string_to_widechar(word_str, &wbuf_len);
        if (!word_buf || wbuf_len == 0) {
            free(word_buf);
            continue;
        }
        
        // Allocate hyphen array (must be at least word_len + 1)
        char* hyphens = (char*)malloc((wbuf_len + 1) * sizeof(char));
        if (!hyphens) {
            free(word_buf);
            continue;
        }
        
        // Initialize hyphens array to zeros
        memset(hyphens, 0, (wbuf_len + 1) * sizeof(char));
        
        // Consume mode parameter from fuzzer input
        int mode = fdp.ConsumeIntegral<int>();
        
        // Apply specific mode flags that might affect hyphenation
        // Based on liblouis source, mode can include various flags
        if (fdp.ConsumeBool()) mode |= compbrlAtCursor;
        if (fdp.ConsumeBool()) mode |= dotsIO;
        if (fdp.ConsumeBool()) mode |= noUndefined;
        if (fdp.ConsumeBool()) mode |= partialTrans;
        if (fdp.ConsumeBool()) mode |= noContractions;
        
        // Test both text mode (0) and braille mode (1) as per documentation
        if (fdp.ConsumeBool()) {
            mode |= 1; // Braille mode
        }
        
        // Step 3: lou_hyphenate with loaded table and appropriate word inputs
        int hyphen_result = lou_hyphenate(table_name, word_buf, wbuf_len, hyphens, mode);
        (void)hyphen_result; // Use result to avoid unused variable warning
        
        // Analyze hyphen results for edge cases
        if (wbuf_len > 0 && hyphens) {
            // Check for valid hyphen values (should be '0', '1', '2' per documentation)
            // This may exercise different paths in hyphenateWord state machine
            for (int i = 0; i < wbuf_len; i++) {
                char h = hyphens[i];
                (void)h; // Just to avoid unused variable warning
                // In real use, we might check h == '0' or '1' or '2'
            }
        }
        
        // Additional tests with same word but different parameters
        if (fdp.remaining_bytes() > 8) {
            // Test with different mode
            int alt_mode = fdp.ConsumeIntegral<int>();
            int alt_result = lou_hyphenate(table_name, word_buf, wbuf_len, hyphens, alt_mode);
            (void)alt_result;
            
            // Test edge cases that might exercise different code paths
            if (fdp.ConsumeBool()) {
                // Null hyphens pointer (should fail but test error handling)
                int null_result = lou_hyphenate(table_name, word_buf, wbuf_len, nullptr, mode);
                (void)null_result;
            }
            
            if (fdp.ConsumeBool()) {
                // Zero length edge case
                int zero_result = lou_hyphenate(table_name, word_buf, 0, hyphens, mode);
                (void)zero_result;
            }
            
            if (fdp.ConsumeBool() && wbuf_len > 5) {
                // Test with substring (partial word)
                int partial_len = fdp.ConsumeIntegralInRange<int>(1, wbuf_len - 1);
                int partial_result = lou_hyphenate(table_name, word_buf, partial_len, hyphens, mode);
                (void)partial_result;
            }
            
            if (fdp.ConsumeBool()) {
                // Test with very short word (1-3 characters)
                if (wbuf_len > 3) {
                    int short_result = lou_hyphenate(table_name, word_buf, 3, hyphens, mode);
                    (void)short_result;
                }
            }
        }
        
        // Clean up for this iteration
        free(hyphens);
        free(word_buf);
        
        // Break early if running low on input
        if (fdp.remaining_bytes() < 32) {
            break;
        }
    }
    
    // Step 4: lou_free cleanup
    lou_free();
    
    return 0;
}

} // namespace harness_011
