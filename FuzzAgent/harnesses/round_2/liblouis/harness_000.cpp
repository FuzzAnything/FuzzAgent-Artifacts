#include <config.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <fuzzer/FuzzedDataProvider.h>
#include "liblouis.h"

namespace harness_000 {
// Comprehensive liblouis fuzzing harness
// Targets key braille translation functions with dynamic input processing
// Harness_000: First harness covering main translation APIs



// Note: internal.h may not be available in installed headers
// We'll use public API functions instead

#define LANGUAGE "en"

static int initialized = 0;
static const char* table_default = NULL;

// Log callback to suppress output during fuzzing
void avoid_log(logLevels level, const char* msg) {
    (void)level;
    (void)msg;
}

// Destructor to free liblouis resources
static void __attribute__((destructor)) free_resources(void) {
    lou_free();
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for meaningful testing
    if (size < 32) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize liblouis once
    if (!initialized) {
        lou_registerLogCallback(avoid_log);
        initialized = 1;
        
        // Try to find a default table for testing
        // First check environment variable
        table_default = getenv("FUZZ_TABLE");
        if (table_default == NULL) {
            // Try a common table if available
            table_default = "en-us-g2.ctb";
        }
    }
    
    // Consume operation type from fuzzer input
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 6;
    
    // Consume translation mode
    int mode = fdp.ConsumeIntegral<int>() % 512;
    
    // Check if we have a valid table for operations that need it
    bool use_table_file = false;
    char table_file[256] = {0};
    const char* table_to_use = table_default;
    
    // For some operations, create a temporary table file from fuzzer data
    if (operation >= 2 && operation <= 4 && fdp.remaining_bytes() >= 512) {
        static int counter = 0;
        sprintf(table_file, "/tmp/libfuzzer-table-%d.ctb", counter++);
        
        // Write table data from fuzzer input
        auto table_data = fdp.ConsumeBytes<uint8_t>(512);
        FILE* fp = fopen(table_file, "wb");
        if (fp) {
            fwrite(table_data.data(), 1, table_data.size(), fp);
            fclose(fp);
            
            // Check if it's a valid table
            if (lou_checkTable(table_file) != 0) {
                table_to_use = table_file;
                use_table_file = true;
            } else {
                unlink(table_file);
            }
        }
    }
    
    // Consume remaining bytes for text input
    std::string input_str = fdp.ConsumeRemainingBytesAsString();
    if (input_str.empty()) {
        if (use_table_file) {
            unlink(table_file);
        }
        return 0;
    }
    // Convert input string to widechar format
    // Simple conversion: copy characters directly (assumes ASCII/latin-1)
    widechar* input_text = (widechar*)malloc((input_str.size() + 1) * sizeof(widechar));
    if (!input_text) {
        if (use_table_file) {
            unlink(table_file);
        }
        return 0;
    }
    
    int input_len = (int)input_str.size();
    for (int i = 0; i < input_len; i++) {
        input_text[i] = (widechar)(unsigned char)input_str[i];
    }
    input_text[input_len] = 0;
    
    // Prepare output buffers
    int output_len = input_len * 16;
    widechar* output_text = (widechar*)malloc((output_len + 1) * sizeof(widechar));
    if (!output_text) {
        free(input_text);
        if (use_table_file) {
            unlink(table_file);
        }
        return 0;
    }
    
    // Optional arrays for advanced functions
    formtype* typeform = NULL;
    char* spacing = NULL;
    int* output_pos = NULL;
    int* input_pos = NULL;
    int cursor_pos = 0;
    char* hyphens = NULL;
    char* input_hyphens = NULL;
    char* output_hyphens = NULL;
    
    // Allocate optional arrays if needed
    // Note: typeform and spacing buffers should be sized for output_len, not input_len
    // as the library may write typeform/spacing information for output characters
    if (operation == 0 || operation == 1 || operation == 2 || operation == 3 || operation == 5) {
        typeform = (formtype*)malloc(output_len * sizeof(formtype));
        spacing = (char*)malloc(output_len * sizeof(char));
        if (typeform && spacing) {
            for (int i = 0; i < output_len; i++) {
                typeform[i] = plain_text;
                spacing[i] = 0;
            }
        } else {
            // If allocation fails, set to NULL (API allows NULL)
            free(typeform);
            free(spacing);
            typeform = NULL;
            spacing = NULL;
        }
    }
    
    if (operation == 1 || operation == 3) {
        output_pos = (int*)malloc(input_len * sizeof(int));
        input_pos = (int*)malloc(output_len * sizeof(int));
        if (!output_pos || !input_pos) {
            free(output_pos);
            free(input_pos);
            output_pos = NULL;
            input_pos = NULL;
        }
    }
    
    if (operation == 4) {
        hyphens = (char*)malloc(input_len * sizeof(char));
        if (!hyphens) {
            hyphens = NULL;
        }
    }
    
    if (operation == 5) {
        input_hyphens = (char*)malloc(input_len * sizeof(char));
        output_hyphens = (char*)malloc(output_len * sizeof(char));
        if (input_hyphens && output_hyphens) {
            memset(input_hyphens, 0, input_len);
        } else {
            free(input_hyphens);
            free(output_hyphens);
            input_hyphens = NULL;
            output_hyphens = NULL;
        }
    }
    // Execute operation based on fuzzer input
    int result = 0;
    switch (operation) {
        case 0:
            // Basic translation
            result = lou_translateString(table_to_use, input_text, &input_len, 
                                         output_text, &output_len, 
                                         typeform, spacing, mode);
            break;
            
        case 1:
            // Back translation
            result = lou_backTranslateString(table_to_use, input_text, &input_len,
                                            output_text, &output_len,
                                            typeform, spacing, mode);
            break;
            
        case 2:
            // Advanced translation with position tracking
            result = lou_translate(table_to_use, input_text, &input_len,
                                  output_text, &output_len,
                                  typeform, spacing,
                                  output_pos, input_pos, &cursor_pos, mode);
            break;
            
        case 3:
            // Advanced back translation with position tracking
            result = lou_backTranslate(table_to_use, input_text, &input_len,
                                      output_text, &output_len,
                                      typeform, spacing,
                                      output_pos, input_pos, &cursor_pos, mode);
            break;
            
        case 4:
            // Hyphenation
            if (hyphens) {
                result = lou_hyphenate(table_to_use, input_text, input_len,
                                      hyphens, mode);
            }
            break;
            
        case 5:
            // Pre-hyphenated translation
            if (input_hyphens && output_hyphens) {
                result = lou_translatePrehyphenated(table_to_use, input_text, &input_len,
                                                   output_text, &output_len,
                                                   typeform, spacing,
                                                   output_pos, input_pos, &cursor_pos,
                                                   input_hyphens, output_hyphens, mode);
            }
            break;
    }
    
    // Clean up
    free(input_text);
    free(output_text);
    
    if (typeform) free(typeform);
    if (spacing) free(spacing);
    if (output_pos) free(output_pos);
    if (input_pos) free(input_pos);
    if (hyphens) free(hyphens);
    if (input_hyphens) free(input_hyphens);
    if (output_hyphens) free(output_hyphens);
    
    if (use_table_file) {
        unlink(table_file);
    }
    
    // Always return 0 - libFuzzer expects 0 on both success and failure
    return 0;
}

} // namespace harness_000
