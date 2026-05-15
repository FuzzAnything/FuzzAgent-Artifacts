#include <fuzzer/FuzzedDataProvider.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <liblouis/liblouis.h>
#include <assert.h>

namespace harness_000 {
// liblouis fuzzing harness - Comprehensive API testing
// Targets: lou_translateString, lou_backTranslateString, lou_checkTable, lou_getTable
// Uses FuzzedDataProvider for structured input processing
// Based on existing liblouis fuzzing harness patterns


// Global initialization flag
static int initialized = 0;

// Log callback to suppress output during fuzzing
void avoid_log(logLevels level, const char *msg) {
    (void)level;
    (void)msg;
}

// Cleanup function
static void free_resources(void) {
    lou_free();
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Minimum size check - need enough data for meaningful testing
    if (size < 32) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize liblouis once
    if (!initialized) {
        lou_registerLogCallback(avoid_log);
        initialized = 1;
        // Register cleanup
        atexit(free_resources);
    }
    
    // Consume operation type to decide which API to test
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 5;
    
    // Consume table name from input (limited to 256 chars)
    std::string table_name = fdp.ConsumeRandomLengthString(256);
    if (table_name.empty()) {
        table_name = "en-us-g1.ctb"; // Default table if no name provided
    }
    
    // Consume input text for translation/backtranslation
    std::string input_text = fdp.ConsumeRemainingBytesAsString();
    if (input_text.empty()) {
        return 0; // No input text to process
    }
    
    // Convert input text to widechar format (simplified conversion)
    size_t wide_len = input_text.length();
    widechar *wide_input = (widechar *)malloc((wide_len + 1) * sizeof(widechar));
    if (!wide_input) return 0;
    
    for (size_t i = 0; i < wide_len; i++) {
        wide_input[i] = (widechar)input_text[i];
    }
    wide_input[wide_len] = 0;
    
    int inlen = (int)wide_len;
    int outlen = inlen * 16; // Conservative output buffer size
    
    switch (operation) {
        case 0: {
            // Test lou_translateString
            widechar *output = (widechar *)malloc((outlen + 1) * sizeof(widechar));
            if (!output) {
                free(wide_input);
                return 0;
            }
            
            int result = lou_translateString(table_name.c_str(), wide_input, &inlen, 
                                            output, &outlen, NULL, NULL, ucBrl);
            (void)result; // Result ignored for fuzzing
            
            free(output);
            break;
        }
        
        case 1: {
            // Test lou_backTranslateString
            widechar *output = (widechar *)malloc((outlen + 1) * sizeof(widechar));
            if (!output) {
                free(wide_input);
                return 0;
            }
            
            int result = lou_backTranslateString(table_name.c_str(), wide_input, &inlen,
                                                output, &outlen, NULL, NULL, ucBrl);
            (void)result;
            
            free(output);
            break;
        }
        
        case 2: {
            // Test lou_checkTable
            int check_result = lou_checkTable(table_name.c_str());
            (void)check_result;
            
            // If table is valid, also try to get it
            if (check_result != 0) {
                const void *table = lou_getTable(table_name.c_str());
                (void)table;
            }
            break;
        }
        
        case 3: {
            // Test lou_getTable directly
            const void *table = lou_getTable(table_name.c_str());
            (void)table;
            break;
        }
        
        case 4: {
            // Test lou_hyphenate
            char *hyphens = (char *)malloc(wide_len + 1);
            if (!hyphens) {
                free(wide_input);
                return 0;
            }
            
            int result = lou_hyphenate(table_name.c_str(), wide_input, inlen, hyphens, 0);
            (void)result;
            
            free(hyphens);
            break;
        }
    }
    
    free(wide_input);
    return 0;
}

} // namespace harness_000
