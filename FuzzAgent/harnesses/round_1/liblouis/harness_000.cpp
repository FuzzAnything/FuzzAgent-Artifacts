#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <string>
#include <vector>
#include <unistd.h>  // for unlink()
#include <algorithm> // for std::min
#include <fuzzer/FuzzedDataProvider.h>
#include "liblouis/liblouis.h"

namespace harness_000 {
/*
 * liblouis Braille Translation and Back-Translation Library Fuzzing Harness
 * 
 * This harness tests multiple liblouis APIs with fuzzer-generated input:
 * - lou_checkTable: Validate braille table files
 * - lou_translateString: Translate text to braille
 * - lou_backTranslateString: Translate braille back to text
 * - lou_translate: Translation with cursor position tracking
 * - lou_hyphenate: Hyphenation functionality
 * - lou_dotsToChar/lou_charToDots: Character conversion
 * 
 * Uses FuzzedDataProvider for structured input splitting to maximize coverage.
 */


extern "C" {
}


// For fuzzing purposes, we just copy bytes to widechars
static int simple_string_to_widechar(const char* input, size_t input_len, widechar* output) {
    for (size_t i = 0; i < input_len && i < 1024; i++) { // Limit to 1024 chars
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

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    static int initialized = 0;
    static int file_counter = 0;
    
    // Initialize liblouis once
    if (!initialized) {
        lou_registerLogCallback(fuzz_log_callback);
        initialized = 1;
    }
    
    // Need minimum input for meaningful testing
    // We need: table data (512 bytes) + operation byte + at least some text
    if (size < 520) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // 1. Create a temporary table file from first part of input
    std::vector<uint8_t> table_data = fdp.ConsumeBytes<uint8_t>(512);
    char table_filename[256];
    int counter = __sync_fetch_and_add(&file_counter, 1);
    snprintf(table_filename, sizeof(table_filename), "/tmp/liblouis_fuzz_%d.ctb", counter);
    
    // Write table data to file
    FILE* table_file = fopen(table_filename, "wb");
    if (!table_file) {
        return 0;
    }
    fwrite(table_data.data(), 1, table_data.size(), table_file);
    fclose(table_file);
    
    // 2. Check if table is valid
    if (lou_checkTable(table_filename) == 0) {
        // Invalid table, cleanup and exit
        unlink(table_filename);
        return 0;
    }
    
    // 3. Get remaining input for text operations
    size_t remaining_bytes = fdp.remaining_bytes();
    if (remaining_bytes < 8) {
        lou_free();
        unlink(table_filename);
        return 0;
    }
    
    // 4. Determine which operations to test based on input
    uint8_t operation_mask = fdp.ConsumeIntegral<uint8_t>();
    
    // 5. Prepare text input - convert remaining bytes to widechar string
    std::string text_input = fdp.ConsumeRemainingBytesAsString();
    if (text_input.empty()) {
        lou_free();
        unlink(table_filename);
        return 0;
    }
    
    // Convert to widechar using our simple conversion function
    // Limit input size to prevent excessive memory allocation
    size_t max_input_len = std::min(text_input.size(), (size_t)1024);
    widechar* input_text = (widechar*)malloc((max_input_len + 1) * sizeof(widechar));
    if (!input_text) {
        lou_free();
        unlink(table_filename);
        return 0;
    }
    
    int input_len = simple_string_to_widechar(text_input.c_str(), text_input.size(), input_text);
    
    if (input_len <= 0) {
        free(input_text);
        lou_free();
        unlink(table_filename);
        return 0;
    }
    // 6. Test different operations based on operation_mask
    
    // Operation 1: lou_translateString
    if (operation_mask & 0x01) {
        int out_len = input_len * 16;
        widechar* output_text = (widechar*)malloc((out_len + 1) * sizeof(widechar));
        if (output_text) {
            int in_len_copy = input_len;
            lou_translateString(table_filename, input_text, &in_len_copy, 
                               output_text, &out_len, NULL, NULL, ucBrl);
            free(output_text);
        }
    }
    
    // Operation 2: lou_backTranslateString
    if (operation_mask & 0x02) {
        int out_len = input_len * 16;
        widechar* output_text = (widechar*)malloc((out_len + 1) * sizeof(widechar));
        if (output_text) {
            int in_len_copy = input_len;
            lou_backTranslateString(table_filename, input_text, &in_len_copy,
                                   output_text, &out_len, NULL, NULL, ucBrl);
            free(output_text);
        }
    }
    
    // Operation 3: lou_translate (with cursor position tracking)
    if (operation_mask & 0x04 && input_len > 0) {
        int out_len = input_len * 16;
        widechar* output_text = (widechar*)malloc((out_len + 1) * sizeof(widechar));
        int* output_pos = (int*)malloc(input_len * sizeof(int));
        int* input_pos = (int*)malloc((out_len + 1) * sizeof(int));
        int cursor_pos = 0;
        
        if (output_text && output_pos && input_pos) {
            int in_len_copy = input_len;
            lou_translate(table_filename, input_text, &in_len_copy, output_text, &out_len,
                         NULL, NULL, output_pos, input_pos, &cursor_pos, ucBrl);
        }
        
        free(output_text);
        free(output_pos);
        free(input_pos);
    }
    
    // Operation 4: lou_hyphenate
    if (operation_mask & 0x08 && input_len > 0) {
        char* hyphens = (char*)malloc((input_len + 1) * sizeof(char));
        if (hyphens) {
            lou_hyphenate(table_filename, input_text, input_len, hyphens, ucBrl);
            free(hyphens);
        }
    }
    
    // Operation 5: lou_dotsToChar and lou_charToDots
    if (operation_mask & 0x10 && input_len > 0) {
        widechar* dots_output = (widechar*)malloc((input_len + 1) * sizeof(widechar));
        widechar* chars_output = (widechar*)malloc((input_len + 1) * sizeof(widechar));
        
        if (dots_output && chars_output) {
            // Convert chars to dots
            lou_charToDots(table_filename, input_text, dots_output, input_len, ucBrl);
            
            // Convert dots back to chars
            lou_dotsToChar(table_filename, dots_output, chars_output, input_len, ucBrl);
        }
        
        free(dots_output);
        free(chars_output);
    }
    
    // Operation 6: Test lou_getTable
    if (operation_mask & 0x20) {
        const void* table = lou_getTable(table_filename);
        (void)table; // Table pointer not used, just checking it loads
    }
    
    // Operation 7: Test lou_compileString with random string
    if (operation_mask & 0x40 && text_input.size() > 10) {
        // Use a substring as a table entry to compile
        std::string compile_str = text_input.substr(0, std::min(text_input.size(), size_t(100)));
        lou_compileString(table_filename, compile_str.c_str());
    }
    
    // Cleanup
    free(input_text);
    lou_free();
    unlink(table_filename);
    
    return 0;
}

} // namespace harness_000
