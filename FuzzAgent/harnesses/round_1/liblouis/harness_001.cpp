#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <string>
#include <vector>
#include <algorithm>
#include <fuzzer/FuzzedDataProvider.h>
#include "liblouis/liblouis.h"

namespace harness_001 {
/*
 * liblouis Comprehensive Translation Lifecycle Fuzzing Harness
 * 
 * This harness targets the core translation functionality of liblouis with 0% coverage.
 * Focuses on exercising the full translation lifecycle from table loading to cleanup.
 * 
 * APIs targeted (from coverage guidance):
 * - lou_translateString, lou_translate: Braille translation
 * - lou_backTranslateString, lou_backTranslate: Reverse translation  
 * - lou_hyphenate: Text hyphenation
 * - lou_translatePrehyphenated: Pre-hyphenated translation
 * - lou_dotsToChar, lou_charToDots: Character conversion
 * - lou_getTable, lou_checkTable: Table loading
 * - lou_registerLogCallback, lou_setLogLevel, lou_logEnd: Logging lifecycle
 * - lou_free: Cleanup
 * 
 * Semantic differentiation from harness_000.cpp:
 * - Uses real table files from the project instead of temporary fuzzer-created tables
 * - Tests multiple table types (both .ctb and .utb files)
 * - Exercises comprehensive logging lifecycle
 * - Tests more complete API parameter combinations
 * - Focuses on translation-specific error conditions
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

// Available table files for testing (full paths to ensure they're found)
static const char* available_tables[] = {
    "/root/src/liblouis/tables/en-us-g2.ctb",
    "/root/src/liblouis/tables/en-us-g1.ctb", 
    "/root/src/liblouis/tables/en-us-comp8.ctb",
    "/root/src/liblouis/tables/afr-za-g1.ctb",
    "/root/src/liblouis/tables/afr-za-g2.ctb",
    "/root/src/liblouis/tables/Es-Es-G0.utb",
    "/root/src/liblouis/tables/IPA.utb",
    "/root/src/liblouis/tables/Lv-Lv-g1.utb",
    "/root/src/liblouis/tables/Pl-Pl-g1.utb"
};
static const size_t num_tables = sizeof(available_tables) / sizeof(available_tables[0]);

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Initialize logging once
    static int logging_initialized = 0;
    if (!logging_initialized) {
        lou_registerLogCallback(fuzz_log_callback);
        lou_setLogLevel(LOU_LOG_INFO);
        logging_initialized = 1;
    }
    
    // Need minimum input for meaningful testing
    // We need: operation selection + table selection + text data
    if (size < 16) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // 1. Select which table to use from available options
    uint8_t table_index = fdp.ConsumeIntegralInRange<uint8_t>(0, num_tables - 1);
    const char* table_name = available_tables[table_index];
    
    // 2. Check if table is valid (this loads the table)
    if (lou_checkTable(table_name) == 0) {
        // Table not found or invalid - try to get it directly
        const void* table = lou_getTable(table_name);
        if (table == NULL) {
            // No valid table available for this run
            return 0;
        }
    }
    
    // 3. Get remaining input for text operations
    size_t remaining_bytes = fdp.remaining_bytes();
    if (remaining_bytes < 8) {
        lou_free();
        return 0;
    }
    
    // 4. Determine which operations to test based on input
    uint8_t operation_type = fdp.ConsumeIntegral<uint8_t>();
    
    // 5. Prepare text input - convert to widechar string
    std::string text_input = fdp.ConsumeRemainingBytesAsString();
    if (text_input.empty()) {
        lou_free();
        return 0;
    }
    
    // Convert to widechar using our simple conversion function
    // Limit input size to prevent excessive memory allocation
    size_t max_input_len = std::min(text_input.size(), (size_t)1024);
    widechar* input_text = (widechar*)malloc((max_input_len + 1) * sizeof(widechar));
    if (!input_text) {
        lou_free();
        return 0;
    }
    
    int input_len = simple_string_to_widechar(text_input.c_str(), text_input.size(), input_text);
    
    if (input_len <= 0) {
        free(input_text);
        lou_free();
        return 0;
    }
    
    // 6. Execute translation operations based on operation_type
    
    // Operation 0: Comprehensive translation workflow
    if (operation_type % 5 == 0) {
        // Test lou_translateString (forward translation)
        int out_len_fwd = input_len * 16;
        widechar* output_fwd = (widechar*)malloc((out_len_fwd + 1) * sizeof(widechar));
        if (output_fwd) {
            int in_len_copy = input_len;
            lou_translateString(table_name, input_text, &in_len_copy, 
                               output_fwd, &out_len_fwd, NULL, NULL, ucBrl);
            free(output_fwd);
        }
        
        // Test lou_backTranslateString (reverse translation)
        int out_len_rev = input_len * 16;
        widechar* output_rev = (widechar*)malloc((out_len_rev + 1) * sizeof(widechar));
        if (output_rev) {
            int in_len_copy = input_len;
            lou_backTranslateString(table_name, input_text, &in_len_copy,
                                   output_rev, &out_len_rev, NULL, NULL, ucBrl);
            free(output_rev);
        }
    }
    
    // Operation 1: Advanced translation with cursor tracking
    if (operation_type % 5 == 1 && input_len > 0) {
        int out_len = input_len * 16;
        widechar* output_text = (widechar*)malloc((out_len + 1) * sizeof(widechar));
        int* output_pos = (int*)malloc(input_len * sizeof(int));
        int* input_pos = (int*)malloc((out_len + 1) * sizeof(int));
        int cursor_pos = fdp.ConsumeIntegralInRange<int>(0, input_len);
        
        if (output_text && output_pos && input_pos) {
            int in_len_copy = input_len;
            // Test lou_translate with cursor tracking
            lou_translate(table_name, input_text, &in_len_copy, output_text, &out_len,
                         NULL, NULL, output_pos, input_pos, &cursor_pos, ucBrl);
            
            // Test lou_backTranslate with cursor tracking
            int back_out_len = input_len * 16;
            widechar* back_output = (widechar*)malloc((back_out_len + 1) * sizeof(widechar));
            if (back_output) {
                int back_in_len = input_len;
                lou_backTranslate(table_name, input_text, &back_in_len, back_output, &back_out_len,
                                 NULL, NULL, output_pos, input_pos, &cursor_pos, ucBrl);
                free(back_output);
            }
        }
        
        free(output_text);
        free(output_pos);
        free(input_pos);
    }
    
    // Operation 2: Hyphenation and pre-hyphenated translation
    if (operation_type % 5 == 2 && input_len > 0) {
        // Test lou_hyphenate
        char* hyphens = (char*)malloc((input_len + 1) * sizeof(char));
        if (hyphens) {
            lou_hyphenate(table_name, input_text, input_len, hyphens, ucBrl);
            
            // Test lou_translatePrehyphenated if we have hyphen data
            int out_len = input_len * 16;
            widechar* output_text = (widechar*)malloc((out_len + 1) * sizeof(widechar));
            int* output_pos = (int*)malloc(input_len * sizeof(int));
            int* input_pos = (int*)malloc((out_len + 1) * sizeof(int));
            int cursor_pos = 0;
            char* output_hyphens = (char*)malloc((out_len + 1) * sizeof(char));
            
            if (output_text && output_pos && input_pos && output_hyphens) {
                int in_len_copy = input_len;
                lou_translatePrehyphenated(table_name, input_text, &in_len_copy,
                                          output_text, &out_len, NULL, NULL,
                                          output_pos, input_pos, &cursor_pos,
                                          hyphens, output_hyphens, ucBrl);
            }
            
            free(output_text);
            free(output_pos);
            free(input_pos);
            free(output_hyphens);
            free(hyphens);
        }
    }
    
    // Operation 3: Character-dots conversion
    if (operation_type % 5 == 3 && input_len > 0) {
        // Test lou_charToDots (characters to braille dots)
        widechar* dots_output = (widechar*)malloc((input_len + 1) * sizeof(widechar));
        if (dots_output) {
            lou_charToDots(table_name, input_text, dots_output, input_len, ucBrl);
            
            // Test lou_dotsToChar (braille dots to characters)
            widechar* chars_output = (widechar*)malloc((input_len + 1) * sizeof(widechar));
            if (chars_output) {
                lou_dotsToChar(table_name, dots_output, chars_output, input_len, ucBrl);
                free(chars_output);
            }
            free(dots_output);
        }
    }
    
    // Operation 4: Table manipulation and logging lifecycle
    if (operation_type % 5 == 4) {
        // Test lou_getTable with the selected table
        const void* table = lou_getTable(table_name);
        (void)table; // Table pointer not used, just checking it loads
        
        // Test logging level changes
        uint8_t log_level = fdp.ConsumeIntegral<uint8_t>() % 6;
        switch (log_level) {
            case 0: lou_setLogLevel(LOU_LOG_ALL); break;
            case 1: lou_setLogLevel(LOU_LOG_DEBUG); break;
            case 2: lou_setLogLevel(LOU_LOG_INFO); break;
            case 3: lou_setLogLevel(LOU_LOG_WARN); break;
            case 4: lou_setLogLevel(LOU_LOG_ERROR); break;
            case 5: lou_setLogLevel(LOU_LOG_FATAL); break;
        }
    }
    
    // 7. Cleanup
    free(input_text);
    
    // Test lou_free to cleanup table resources
    lou_free();
    
    // Test lou_logEnd to cleanup logging resources
    lou_logEnd();
    
    return 0;
}

} // namespace harness_001
