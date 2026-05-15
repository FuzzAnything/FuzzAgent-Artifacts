#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fuzzer/FuzzedDataProvider.h>
#include "liblouis.h"

namespace harness_005 {
// Targeted liblouis fuzzing harness for comprehensive hyphenation testing
// Harness_005: Highly focused on hyphenation operations with deep edge case testing
// Targets: lou_hyphenate (47 undiscovered branches, only 11.54% coverage), 
//          lou_translatePrehyphenated (8 undiscovered branches)
// Differentiation: Exclusively focused on hyphenation, not general translation
// Strategy: Deep testing of hyphenation logic with varied inputs and edge cases




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

// Generate hyphen positions from fuzzer data
static char* generate_hyphens(FuzzedDataProvider& fdp, int length) {
    if (length <= 0) return nullptr;
    
    char* hyphens = (char*)malloc(length * sizeof(char));
    if (!hyphens) return nullptr;
    
    for (int i = 0; i < length; i++) {
        // Generate valid hyphen characters: '0', '1', or other valid hyphen markers
        // According to liblouis documentation, hyphens array contains '0' or '1'
        hyphens[i] = fdp.ConsumeBool() ? '1' : '0';
    }
    return hyphens;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for meaningful hyphenation testing
    // We need: log level (4 bytes) + mode (4 bytes) + table choice (4 bytes) + at least 1 byte text
    if (size < 13) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // 1. Set log level - consume from valid range
    logLevels log_level = static_cast<logLevels>(
        fdp.ConsumeIntegralInRange<int>(LOU_LOG_ALL, LOU_LOG_OFF));
    lou_setLogLevel(log_level);
    
    // 2. Register log callback to suppress output
    lou_registerLogCallback(fuzz_log_callback);
    
    // 3. Select a table for hyphenation testing
    // Use common tables that support hyphenation
    const char* table_list = nullptr;
    uint8_t table_choice = fdp.ConsumeIntegral<uint8_t>() % 8;
    switch (table_choice) {
        case 0: table_list = "en-us-g2.ctb"; break;      // US English Grade 2
        case 1: table_list = "en-ueb-g2.ctb"; break;     // Unified English Braille
        case 2: table_list = "en-us-comp8.ctb"; break;   // Computer Braille 8-dot
        case 3: table_list = "en-us-comp6.ctb"; break;   // Computer Braille 6-dot
        case 4: table_list = "da-dk-g16.ctb"; break;     // Danish
        case 5: table_list = "sv-g1.ctb"; break;         // Swedish
        case 6: table_list = "de-g0.utb"; break;         // German
        case 7: table_list = "zhcn-g2.ctb"; break;       // Chinese
        default: table_list = "en-us-g2.ctb"; break;
    }
    
    // 4. Check table validity (optional but good practice)
    int table_valid = lou_checkTable(table_list);
    if (!table_valid) {
        // Table might not be available, but we'll try to load it anyway
        // Some tables might be valid but return 0 for other reasons
    }
    
    // 5. Get the table pointer (required for hyphenation operations)
    const void* table_ptr = lou_getTable(table_list);
    if (!table_ptr) {
        // If we can't get a table, we can't proceed with hyphenation
        return 0;
    }
    
    // 6. Consume operation type to test different hyphenation scenarios
    // This allows the fuzzer to focus on different code paths
    uint8_t scenario = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    // 7. Consume translation mode (deprecated but still used)
    int mode = fdp.ConsumeIntegral<int>();
    
    // 8. Consume text input for hyphenation - use part of remaining bytes
    // Determine text length (bounded to prevent excessive memory usage)
    size_t remaining = fdp.remaining_bytes();
    if (remaining < 1) return 0;
    
    size_t max_text_len = fdp.ConsumeIntegralInRange<size_t>(1, 1024);
    if (max_text_len > remaining) {
        max_text_len = remaining;
    }
    
    std::string input_text_str = fdp.ConsumeBytesAsString(max_text_len);
    if (input_text_str.empty()) {
        return 0;
    }
    
    // Convert input text to widechar format
    int input_len = 0;
    widechar* input_text = string_to_widechar(input_text_str, &input_len);
    if (!input_text || input_len == 0) {
        free(input_text);
        return 0;
    }
    
    // Execute different hyphenation scenarios
    int result = 0;
    
    switch (scenario) {
        case 0: {
            // Scenario 0: Basic hyphenation test
            // Allocate hyphen array for lou_hyphenate
            char* hyphens = (char*)malloc((input_len + 1) * sizeof(char));
            if (!hyphens) {
                free(input_text);
                return 0;
            }
            
            // Test lou_hyphenate with the input text
            result = lou_hyphenate(table_list, input_text, input_len, hyphens, mode);
            
            // If hyphenation succeeded, we could test the hyphens array
            // but just calling the function exercises the code paths
            
            free(hyphens);
            break;
        }
        
        case 1: {
            // Scenario 1: Hyphenation with generated hyphen patterns
            // Generate hyphen array from fuzzer data
            char* hyphens = generate_hyphens(fdp, input_len);
            if (!hyphens) {
                free(input_text);
                return 0;
            }
            
            // Test lou_hyphenate with input and generated hyphens
            result = lou_hyphenate(table_list, input_text, input_len, hyphens, mode);
            
            free(hyphens);
            break;
        }
        
        case 2: {
            // Scenario 2: translatePrehyphenated with generated input hyphens
            // First, allocate buffers for translation
            int max_output_len = input_len * 16; // Conservative expansion factor
            if (max_output_len < 64) max_output_len = 64; // Minimum buffer size
            
            widechar* output_text = (widechar*)malloc((max_output_len + 1) * sizeof(widechar));
            if (!output_text) {
                free(input_text);
                return 0;
            }
            
            // Generate input and output hyphen arrays
            char* input_hyphens = generate_hyphens(fdp, input_len);
            char* output_hyphens = (char*)malloc((max_output_len + 1) * sizeof(char));
            
            // Prepare other optional parameters
            formtype* typeform = nullptr;
            char* spacing = nullptr;
            int* outputPos = nullptr;
            int* inputPos = nullptr;
            int* cursorPos = nullptr;
            
            // 50% chance to allocate typeform
            if (fdp.ConsumeBool() && input_len > 0) {
                typeform = (formtype*)malloc(input_len * sizeof(formtype));
                if (typeform) {
                    for (int i = 0; i < input_len; i++) {
                        typeform[i] = static_cast<formtype>(
                            fdp.ConsumeIntegralInRange<unsigned short>(0, 0x7FF));
                    }
                }
            }
            
            // 50% chance to allocate spacing
            if (fdp.ConsumeBool() && input_len > 0) {
                spacing = (char*)malloc(input_len * sizeof(char));
                if (spacing) {
                    for (int i = 0; i < input_len; i++) {
                        spacing[i] = fdp.ConsumeIntegral<char>();
                    }
                }
            }
            
            // 50% chance to allocate position arrays
            if (fdp.ConsumeBool()) {
                outputPos = (int*)malloc(max_output_len * sizeof(int));
                inputPos = (int*)malloc(input_len * sizeof(int));
                cursorPos = (int*)malloc(sizeof(int));
            }
            
            // Make copies of lengths since they're in/out parameters
            int translate_inlen = input_len;
            int translate_outlen = max_output_len;
            
            // Test lou_translatePrehyphenated
            result = lou_translatePrehyphenated(table_list, input_text, &translate_inlen,
                                               output_text, &translate_outlen,
                                               typeform, spacing, outputPos, inputPos,
                                               cursorPos, input_hyphens, output_hyphens, mode);
            
            // Clean up
            free(output_text);
            free(input_hyphens);
            free(output_hyphens);
            free(typeform);
            free(spacing);
            free(outputPos);
            free(inputPos);
            free(cursorPos);
            break;
        }
        
        case 3: {
            // Scenario 3: Combined hyphenation and translation
            // First hyphenate the input
            char* hyphens = (char*)malloc((input_len + 1) * sizeof(char));
            if (!hyphens) {
                free(input_text);
                return 0;
            }
            
            result = lou_hyphenate(table_list, input_text, input_len, hyphens, mode);
            
            // If hyphenation succeeded, try translating the pre-hyphenated text
            if (result) {
                int max_output_len = input_len * 16;
                if (max_output_len < 64) max_output_len = 64;
                
                widechar* output_text = (widechar*)malloc((max_output_len + 1) * sizeof(widechar));
                char* output_hyphens = (char*)malloc((max_output_len + 1) * sizeof(char));
                
                if (output_text && output_hyphens) {
                    int translate_inlen = input_len;
                    int translate_outlen = max_output_len;
                    
                    // Use the generated hyphens as input hyphens
                    result = lou_translatePrehyphenated(table_list, input_text, &translate_inlen,
                                                       output_text, &translate_outlen,
                                                       nullptr, nullptr, nullptr, nullptr,
                                                       nullptr, hyphens, output_hyphens, mode);
                }
                
                free(output_text);
                free(output_hyphens);
            }
            
            free(hyphens);
            break;
        }
    }
    
    // Clean up input buffer
    free(input_text);
    
    // 9. Free liblouis resources (though destructor will handle it, doing explicitly)
    lou_free();
    
    return 0;
}

// Destructor to ensure resources are freed
static void __attribute__((destructor)) free_resources(void) {
    lou_free();
}

} // namespace harness_005
