#include <fuzzer/FuzzedDataProvider.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <liblouis/liblouis.h>
#include <assert.h>
#include <vector>
#include <string>

namespace harness_018 {
// liblouis fuzzing harness - Back-translation subsystem focus
// Targets: lou_backTranslateString function with 833 undiscovered branches
// Focus: Comprehensive back-translation scenarios with various modes, cursor positions, and translation tables
// Different from existing harness_000.cpp which has minimal back-translation testing
// Strategy: Exercise complex branching through multiple parameter combinations, typeform arrays, spacing arrays
// Project: $SRC, Headers: liblouis.h, internal.h for extended functionality


// Global initialization flag
static int initialized = 0;
static int file_counter = 0;

// Log callback to suppress output during fuzzing
void avoid_log(logLevels level, const char *msg) {
    (void)level;
    (void)msg;
}

// Cleanup function
static void free_resources(void) {
    lou_free();
}

// Create a temporary table file from fuzzer data for more realistic testing
static char* create_temp_table_file(const uint8_t* data, size_t size, size_t max_file_size = 1024) {
    char filename[256];
    snprintf(filename, sizeof(filename), "/tmp/liblouis-backtranslate-%d-%d.ctb", getpid(), file_counter++);
    
    // Determine how much data to write to the file
    size_t file_size = (size > 0) ? (size % max_file_size) + 1 : 1;
    if (file_size > size) file_size = size;
    
    FILE* fp = fopen(filename, "wb");
    if (!fp) {
        return NULL;
    }
    
    fwrite(data, 1, file_size, fp);
    fclose(fp);
    
    // Duplicate filename for caller to free
    return strdup(filename);
}

// Helper to create typeform array from fuzzer input
static formtype* create_typeform_array(FuzzedDataProvider& fdp, size_t length) {
    if (length == 0) return NULL;
    
    formtype* typeform = (formtype*)malloc(length * sizeof(formtype));
    if (!typeform) return NULL;
    
    for (size_t i = 0; i < length; i++) {
        // Use various typeform values from the enum
        uint8_t typeform_choice = fdp.ConsumeIntegral<uint8_t>() % 16;
        switch (typeform_choice % 8) {
            case 0: typeform[i] = plain_text; break;
            case 1: typeform[i] = emph_1; break;    // italic
            case 2: typeform[i] = emph_2; break;    // underline
            case 3: typeform[i] = emph_3; break;    // bold
            case 4: typeform[i] = emph_4; break;
            case 5: typeform[i] = emph_5; break;
            case 6: typeform[i] = emph_6; break;
            case 7: typeform[i] = emph_7; break;
            default: typeform[i] = plain_text; break;
        }
    }
    
    return typeform;
}

// Helper to create spacing array from fuzzer input
static char* create_spacing_array(FuzzedDataProvider& fdp, size_t length) {
    if (length == 0) return NULL;
    
    char* spacing = (char*)malloc(length * sizeof(char));
    if (!spacing) return NULL;
    
    for (size_t i = 0; i < length; i++) {
        spacing[i] = fdp.ConsumeIntegral<uint8_t>() % 3; // 0, 1, or 2
    }
    
    return spacing;
}

// Helper to create cursor position array
static int* create_cursor_pos_array(FuzzedDataProvider& fdp, size_t length) {
    if (length == 0) return NULL;
    
    int* cursor_pos = (int*)malloc(length * sizeof(int));
    if (!cursor_pos) return NULL;
    
    for (size_t i = 0; i < length; i++) {
        cursor_pos[i] = fdp.ConsumeIntegralInRange<int>(-1, length * 2);
    }
    
    return cursor_pos;
}

// Helper to create input/output position arrays for lou_backTranslate
static int* create_position_array(FuzzedDataProvider& fdp, size_t length) {
    if (length == 0) return NULL;
    
    int* positions = (int*)malloc(length * sizeof(int));
    if (!positions) return NULL;
    
    for (size_t i = 0; i < length; i++) {
        positions[i] = fdp.ConsumeIntegralInRange<int>(-1, length * 2);
    }
    
    return positions;
}

// Convert string to widechar array (simplified)
static std::vector<widechar> string_to_widechars(const std::string& str) {
    std::vector<widechar> widechars;
    widechars.reserve(str.length());
    for (char c : str) {
        widechars.push_back(static_cast<widechar>(c));
    }
    return widechars;
}

// Main fuzzer entry point
int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Minimum size check - need enough data for comprehensive testing
    if (size < 64) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize liblouis once
    if (!initialized) {
        lou_registerLogCallback(avoid_log);
        initialized = 1;
        // Register cleanup
        atexit(free_resources);
    }
    
    // Consume operation type to decide which back-translation API to test
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    // Consume table name or create temporary table file
    std::string table_name = fdp.ConsumeRandomLengthString(256);
    char* temp_filename = NULL;
    
    // For some operations, create a temporary table file from fuzzer data
    if (operation == 2 || operation == 3) {
        std::vector<uint8_t> file_data = fdp.ConsumeBytes<uint8_t>(fdp.ConsumeIntegralInRange<size_t>(64, 512));
        if (!file_data.empty()) {
            temp_filename = create_temp_table_file(file_data.data(), file_data.size(), 512);
        }
    }
    
    // Use either predefined table name or temporary file
    const char* table_to_use = table_name.empty() ? "en-us-g1.ctb" : table_name.c_str();
    if (temp_filename) {
        table_to_use = temp_filename;
    }
    
    // Consume input text for back-translation (braille dots input)
    size_t text_size = fdp.ConsumeIntegralInRange<size_t>(1, 512);
    std::vector<uint8_t> input_bytes = fdp.ConsumeBytes<uint8_t>(text_size);
    
    if (input_bytes.empty()) {
        if (temp_filename) {
            unlink(temp_filename);
            free(temp_filename);
        }
        return 0;
    }
    
    // Convert input to widechars (simplified - treat bytes as widechars)
    std::vector<widechar> wide_input;
    wide_input.reserve(input_bytes.size());
    for (uint8_t byte : input_bytes) {
        wide_input.push_back(static_cast<widechar>(byte));
    }
    
    int inlen = static_cast<int>(wide_input.size());
    int outlen = inlen * 16; // Conservative output buffer size
    
    // Allocate output buffer
    std::vector<widechar> output(outlen + 1);
    
    // Consume translation mode from enum values
    uint8_t mode_choice = fdp.ConsumeIntegral<uint8_t>() % 8;
    int mode = 0;
    switch (mode_choice) {
        case 0: mode = 0; break; // Default mode
        case 1: mode = noContractions; break;
        case 2: mode = compbrlAtCursor; break;
        case 3: mode = dotsIO; break;
        case 4: mode = compbrlLeftCursor; break;
        case 5: mode = ucBrl; break;
        case 6: mode = noUndefined; break;
        case 7: mode = partialTrans; break;
    }
    
    // Create typeform and spacing arrays for more comprehensive testing
    // Allocate based on outlen to match API contract (typeform/spacing buffers need at least outlen elements)
    formtype* typeform = create_typeform_array(fdp, outlen);
    char* spacing = create_spacing_array(fdp, outlen);
    
    int result = 0;
    
    switch (operation) {
        case 0: {
            // Test lou_backTranslateString with various parameter combinations
            result = lou_backTranslateString(table_to_use, wide_input.data(), &inlen,
                                           output.data(), &outlen, typeform, spacing, mode);
            (void)result; // Result ignored for fuzzing
            break;
        }
        
        case 1: {
            // Test lou_backTranslate with cursor position tracking
            int* outputPos = create_position_array(fdp, inlen);
            int* inputPos = create_position_array(fdp, outlen);
            int* cursorPos = create_cursor_pos_array(fdp, 1);
            
            result = lou_backTranslate(table_to_use, wide_input.data(), &inlen,
                                     output.data(), &outlen, typeform, spacing,
                                     outputPos, inputPos, cursorPos, mode);
            
            free(outputPos);
            free(inputPos);
            free(cursorPos);
            (void)result;
            break;
        }
        
        case 2: {
            // Test with a fuzzer-generated table file
            if (temp_filename) {
                // First check if the table is valid
                int check_result = lou_checkTable(temp_filename);
                if (check_result != 0) {
                    // Table is valid, proceed with back-translation
                    result = lou_backTranslateString(temp_filename, wide_input.data(), &inlen,
                                                   output.data(), &outlen, typeform, spacing, mode);
                    (void)result;
                }
            } else {
                // Fall back to standard table name
                result = lou_backTranslateString(table_to_use, wide_input.data(), &inlen,
                                               output.data(), &outlen, typeform, spacing, mode);
                (void)result;
            }
            break;
        }
        
        case 3: {
            // Test multiple back-translation operations in sequence
            // First try with simple parameters
            int simple_mode = fdp.ConsumeBool() ? ucBrl : 0;
            result = lou_backTranslateString(table_to_use, wide_input.data(), &inlen,
                                           output.data(), &outlen, NULL, NULL, simple_mode);
            
            // Then try with more complex parameters if there's enough data
            if (fdp.remaining_bytes() > 32) {
                int complex_mode = fdp.ConsumeIntegral<uint8_t>() % 256;
                // Try with different buffer sizes
                int alt_inlen = inlen / 2;
                if (alt_inlen > 0) {
                    result = lou_backTranslateString(table_to_use, wide_input.data(), &alt_inlen,
                                                   output.data(), &outlen, typeform, spacing, complex_mode);
                }
            }
            (void)result;
            break;
        }
    }
    
    // Clean up
    free(typeform);
    free(spacing);
    
    if (temp_filename) {
        unlink(temp_filename);
        free(temp_filename);
    }
    
    return 0;
}

} // namespace harness_018
