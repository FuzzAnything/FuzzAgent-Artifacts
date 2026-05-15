#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fuzzer/FuzzedDataProvider.h>
#include "liblouis.h"

namespace harness_004 {
// Targeted liblouis fuzzing harness for translation and hyphenation operations
// Harness_004: Deep focus on hyphenation and translation APIs with extensive input variations
// Targets: lou_hyphenate (47 undiscovered branches), lou_translatePrehyphenated (20),
//          lou_translateString (0 hits), lou_translate (0 hits)




// Log callback to suppress output during fuzzing
static void fuzz_log_callback(logLevels level, const char* message) {
    (void)level;
    (void)message;
    // Suppress all logging during fuzzing
}

// Destructor to free liblouis resources
static void __attribute__((destructor)) free_resources(void) {
    lou_free();
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
        hyphens[i] = fdp.ConsumeBool() ? '1' : '0';
    }
    return hyphens;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for meaningful testing
    // We need: log level (4 bytes) + mode (4 bytes) + text length (4 bytes) + at least 1 byte text
    if (size < 13) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // 1. Set log level
    logLevels log_level = static_cast<logLevels>(
        fdp.ConsumeIntegralInRange<int>(LOU_LOG_ALL, LOU_LOG_OFF));
    lou_setLogLevel(log_level);
    
    // 2. Optionally register log callback (do it always for consistency)
    lou_registerLogCallback(fuzz_log_callback);
    
    // 3. Get table - use a common table for testing
    // Try multiple tables to increase coverage
    const char* table_list = nullptr;
    uint8_t table_choice = fdp.ConsumeIntegral<uint8_t>() % 4;
    switch (table_choice) {
        case 0: table_list = "en-us-g2.ctb"; break;
        case 1: table_list = "en-ueb-g2.ctb"; break;
        case 2: table_list = "en-us-comp8.ctb"; break;
        case 3: table_list = "en-us-comp6.ctb"; break;
        default: table_list = "en-us-g2.ctb"; break;
    }
    
    // 4. Check table validity
    int table_valid = lou_checkTable(table_list);
    if (!table_valid) {
        // Table might not be available, try to load it anyway
        // Some tables might be valid but return 0 for other reasons
    }
    
    // Get the table pointer
    const void* table_ptr = lou_getTable(table_list);
    if (!table_ptr) {
        // If we can't get a table, we can't proceed with translation
        return 0;
    }
    
    // Consume operation type to determine which functions to test
    // This allows the fuzzer to focus on different code paths
    uint8_t operation_type = fdp.ConsumeIntegral<uint8_t>() % 8;
    
    // Consume translation mode
    int mode = fdp.ConsumeIntegral<int>();
    
    // Consume text input - use remaining bytes for text
    std::string input_text_str = fdp.ConsumeRemainingBytesAsString();
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
    
    // Prepare output buffer for translation functions
    // Output buffer needs to be larger than input for braille expansion
    int max_output_len = input_len * 16; // Conservative expansion factor
    if (max_output_len < 64) max_output_len = 64; // Minimum buffer size
    
    widechar* output_text = (widechar*)malloc((max_output_len + 1) * sizeof(widechar));
    if (!output_text) {
        free(input_text);
        return 0;
    }
    
    // Initialize output length variable (in/out parameter)
    int output_len = max_output_len;
    
    // Result variable for API calls
    int result = 0;
    
    // Execute based on operation type
    switch (operation_type) {
        case 0: {
            // 5. Test lou_translateString with varied inputs
            // Allocate optional typeform and spacing arrays
            formtype* typeform = nullptr;
            char* spacing = nullptr;
            
            // 50% chance to use typeform
            if (fdp.ConsumeBool() && input_len > 0) {
                typeform = (formtype*)malloc(input_len * sizeof(formtype));
                if (typeform) {
                    for (int i = 0; i < input_len; i++) {
                        // Random typeform from fuzzer data
                        typeform[i] = static_cast<formtype>(
                            fdp.ConsumeIntegralInRange<unsigned short>(0, 0x7FF));
                    }
                }
            }
            
            // 50% chance to use spacing
            if (fdp.ConsumeBool() && input_len > 0) {
                spacing = (char*)malloc(input_len * sizeof(char));
                if (spacing) {
                    for (int i = 0; i < input_len; i++) {
                        spacing[i] = fdp.ConsumeIntegral<char>();
                    }
                }
            }
            
            // Make a copy of input_len since it's an in/out parameter
            int translate_inlen = input_len;
            int translate_outlen = output_len;
            
            result = lou_translateString(table_list, input_text, &translate_inlen,
                                       output_text, &translate_outlen,
                                       typeform, spacing, mode);
            
            free(typeform);
            free(spacing);
            break;
        }
        
        case 1: {
            // 6. Test lou_translate with different modes and position tracking
            formtype* typeform = nullptr;
            char* spacing = nullptr;
            int* output_pos = nullptr;
            int* input_pos = nullptr;
            int cursor_pos = 0;
            
            // Randomly allocate optional parameters
            if (fdp.ConsumeBool() && input_len > 0) {
                typeform = (formtype*)malloc(input_len * sizeof(formtype));
                if (typeform) {
                    for (int i = 0; i < input_len; i++) {
                        typeform[i] = plain_text;
                    }
                }
            }
            
            if (fdp.ConsumeBool() && input_len > 0) {
                spacing = (char*)malloc(input_len * sizeof(char));
                if (spacing) {
                    memset(spacing, 0, input_len);
                }
            }
            
            if (fdp.ConsumeBool() && input_len > 0) {
                output_pos = (int*)malloc(input_len * sizeof(int));
                if (output_pos) {
                    for (int i = 0; i < input_len; i++) {
                        output_pos[i] = -1;
                    }
                }
            }
            
            if (fdp.ConsumeBool() && output_len > 0) {
                input_pos = (int*)malloc(output_len * sizeof(int));
                if (input_pos) {
                    for (int i = 0; i < output_len; i++) {
                        input_pos[i] = -1;
                    }
                }
            }
            
            cursor_pos = fdp.ConsumeIntegralInRange<int>(0, input_len);
            
            int translate_inlen = input_len;
            int translate_outlen = output_len;
            
            result = lou_translate(table_list, input_text, &translate_inlen,
                                 output_text, &translate_outlen,
                                 typeform, spacing, output_pos, input_pos,
                                 &cursor_pos, mode);
            
            free(typeform);
            free(spacing);
            free(output_pos);
            free(input_pos);
            break;
        }
        
        case 2:
        case 3: {
            // 7. Test lou_translatePrehyphenated with hyphen positions
            formtype* typeform = nullptr;
            char* spacing = nullptr;
            int* output_pos = nullptr;
            int* input_pos = nullptr;
            int cursor_pos = 0;
            char* input_hyphens = nullptr;
            char* output_hyphens = nullptr;
            
            // Allocate hyphen arrays
            if (input_len > 0) {
                input_hyphens = generate_hyphens(fdp, input_len);
                output_hyphens = (char*)malloc(output_len * sizeof(char));
                if (output_hyphens) {
                    memset(output_hyphens, 0, output_len);
                }
            }
            
            // Randomly allocate other optional parameters
            if (fdp.ConsumeBool() && input_len > 0) {
                typeform = (formtype*)malloc(input_len * sizeof(formtype));
                if (typeform) {
                    for (int i = 0; i < input_len; i++) {
                        typeform[i] = plain_text;
                    }
                }
            }
            
            if (fdp.ConsumeBool() && input_len > 0) {
                spacing = (char*)malloc(input_len * sizeof(char));
                if (spacing) {
                    memset(spacing, 0, input_len);
                }
            }
            
            if (fdp.ConsumeBool() && input_len > 0) {
                output_pos = (int*)malloc(input_len * sizeof(int));
                if (output_pos) {
                    for (int i = 0; i < input_len; i++) {
                        output_pos[i] = -1;
                    }
                }
            }
            
            if (fdp.ConsumeBool() && output_len > 0) {
                input_pos = (int*)malloc(output_len * sizeof(int));
                if (input_pos) {
                    for (int i = 0; i < output_len; i++) {
                        input_pos[i] = -1;
                    }
                }
            }
            
            cursor_pos = fdp.ConsumeIntegralInRange<int>(0, input_len);
            
            int translate_inlen = input_len;
            int translate_outlen = output_len;
            
            if (input_hyphens && output_hyphens) {
                result = lou_translatePrehyphenated(table_list, input_text, &translate_inlen,
                                                  output_text, &translate_outlen,
                                                  typeform, spacing, output_pos, input_pos,
                                                  &cursor_pos, input_hyphens, output_hyphens,
                                                  mode);
            }
            
            free(typeform);
            free(spacing);
            free(output_pos);
            free(input_pos);
            free(input_hyphens);
            free(output_hyphens);
            break;
        }
        
        case 4:
        case 5: {
            // 8. Test lou_hyphenate with text inputs
            if (input_len > 0) {
                char* hyphens = (char*)malloc(input_len * sizeof(char));
                if (hyphens) {
                    result = lou_hyphenate(table_list, input_text, input_len, hyphens, mode);
                    free(hyphens);
                }
            }
            break;
        }
        
        case 6:
        case 7: {
            // 9. Optionally test lou_backTranslateString
            formtype* typeform = nullptr;
            char* spacing = nullptr;
            
            if (fdp.ConsumeBool() && input_len > 0) {
                typeform = (formtype*)malloc(input_len * sizeof(formtype));
                if (typeform) {
                    for (int i = 0; i < input_len; i++) {
                        typeform[i] = plain_text;
                    }
                }
            }
            
            if (fdp.ConsumeBool() && input_len > 0) {
                spacing = (char*)malloc(input_len * sizeof(char));
                if (spacing) {
                    memset(spacing, 0, input_len);
                }
            }
            
            int back_inlen = input_len;
            int back_outlen = output_len;
            
            result = lou_backTranslateString(table_list, input_text, &back_inlen,
                                           output_text, &back_outlen,
                                           typeform, spacing, mode);
            
            free(typeform);
            free(spacing);
            break;
        }
    }
    
    // 10. lou_free is called automatically by the destructor
    
    // Clean up allocated buffers
    free(input_text);
    free(output_text);
    
    // Always return 0 - libFuzzer expects 0 on both success and failure
    return 0;
}

} // namespace harness_004
