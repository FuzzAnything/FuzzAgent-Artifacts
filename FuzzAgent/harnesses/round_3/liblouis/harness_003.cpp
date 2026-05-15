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

namespace harness_003 {
// liblouis fuzzing harness - Character/Dots Conversion APIs focus
// Targets: lou_charToDots, lou_dotsToChar, lou_translatePrehyphenated
// Uses FuzzedDataProvider for structured input processing
// Focus on low-level character-to-braille conversion with semantic diversity from existing harnesses


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

// Helper function to create a widechar buffer from fuzzer input
static widechar* create_widechar_buffer(FuzzedDataProvider& fdp, size_t max_length, size_t* actual_length) {
    // Consume length of buffer (1 to max_length)
    size_t length = fdp.ConsumeIntegralInRange<size_t>(1, max_length);
    if (length == 0) length = 1;
    
    // Consume bytes for the buffer
    std::vector<uint8_t> bytes = fdp.ConsumeBytes<uint8_t>(length * sizeof(widechar));
    if (bytes.size() < sizeof(widechar)) {
        // Not enough data for even one widechar
        return NULL;
    }
    
    // Convert to widechar buffer
    widechar* buffer = (widechar*)malloc(length * sizeof(widechar));
    if (!buffer) return NULL;
    
    // Fill buffer with consumed data
    size_t num_wchars = bytes.size() / sizeof(widechar);
    memcpy(buffer, bytes.data(), num_wchars * sizeof(widechar));
    
    // Fill remaining with random data if needed
    if (num_wchars < length) {
        std::vector<uint8_t> remaining = fdp.ConsumeBytes<uint8_t>((length - num_wchars) * sizeof(widechar));
        if (remaining.size() >= sizeof(widechar)) {
            memcpy(buffer + num_wchars, remaining.data(), 
                   std::min(remaining.size(), (length - num_wchars) * sizeof(widechar)));
        }
    }
    
    *actual_length = length;
    return buffer;
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Minimum size check - need enough data for character/dots operations
    if (size < 64) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize liblouis once
    if (!initialized) {
        lou_registerLogCallback(avoid_log);
        initialized = 1;
        // Register cleanup
        atexit(free_resources);
    }
    
    // Consume operation type to decide which API to test
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 6;
    
    // Consume table name from input (limited to 256 chars)
    std::string table_name = fdp.ConsumeRandomLengthString(256);
    if (table_name.empty()) {
        table_name = "en-us-g1.ctb"; // Default table if no name provided
    }
    
    // Consume mode flags
    int mode = 0;
    if (fdp.ConsumeBool()) mode |= ucBrl;
    if (fdp.ConsumeBool()) mode |= dotsIO;
    if (fdp.ConsumeBool()) mode |= noContractions;
    if (fdp.ConsumeBool()) mode |= compbrlAtCursor;
    
    // For charToDots and dotsToChar operations
    size_t buffer_length = 0;
    widechar* input_buffer = NULL;
    widechar* output_buffer = NULL;
    
    if (operation < 4) { // charToDots and dotsToChar operations
        // Create input buffer
        input_buffer = create_widechar_buffer(fdp, 256, &buffer_length);
        if (!input_buffer || buffer_length == 0) {
            if (input_buffer) free(input_buffer);
            return 0;
        }
        
        // Allocate output buffer (same size as input)
        output_buffer = (widechar*)malloc(buffer_length * sizeof(widechar));
        if (!output_buffer) {
            free(input_buffer);
            return 0;
        }
    }
    
    int result = 0;
    
    switch (operation) {
        case 0: {
            // Test lou_charToDots - basic conversion
            result = lou_charToDots(table_name.c_str(), input_buffer, output_buffer, 
                                   (int)buffer_length, mode);
            (void)result; // Result ignored for fuzzing
            break;
        }
        
        case 1: {
            // Test lou_dotsToChar - reverse conversion
            result = lou_dotsToChar(table_name.c_str(), input_buffer, output_buffer,
                                   (int)buffer_length, mode);
            (void)result;
            break;
        }
        
        case 2: {
            // Test lou_charToDots with ucBrl mode
            result = lou_charToDots(table_name.c_str(), input_buffer, output_buffer,
                                   (int)buffer_length, mode | ucBrl);
            (void)result;
            break;
        }
        
        case 3: {
            // Test lou_dotsToChar with dotsIO mode
            result = lou_dotsToChar(table_name.c_str(), input_buffer, output_buffer,
                                   (int)buffer_length, mode | dotsIO);
            (void)result;
            break;
        }
        
        case 4: {
            // Test lou_translatePrehyphenated with hyphenation data
            // Create input buffer for translation
            size_t trans_length = 0;
            widechar* trans_input = create_widechar_buffer(fdp, 128, &trans_length);
            if (!trans_input || trans_length == 0) {
                if (trans_input) free(trans_input);
                break;
            }
            
            // Allocate output buffer (larger for expansion)
            int inlen = (int)trans_length;
            int outlen = inlen * 4;
            widechar* trans_output = (widechar*)malloc(outlen * sizeof(widechar));
            if (!trans_output) {
                free(trans_input);
                break;
            }
            
            // Create hyphen arrays
            char* input_hyphens = (char*)malloc(trans_length + 1);
            char* output_hyphens = (char*)malloc(outlen + 1);
            if (!input_hyphens || !output_hyphens) {
                free(trans_input);
                free(trans_output);
                if (input_hyphens) free(input_hyphens);
                if (output_hyphens) free(output_hyphens);
                break;
            }
            
            // Fill hyphen arrays with fuzzer data
            std::vector<uint8_t> hyphen_data = fdp.ConsumeBytes<uint8_t>(trans_length);
            for (size_t i = 0; i < trans_length && i < hyphen_data.size(); i++) {
                input_hyphens[i] = (hyphen_data[i] % 3) + '0'; // '0', '1', or '2'
            }
            input_hyphens[trans_length] = '\0';
            
            // Position arrays (optional)
            int* output_pos = NULL;
            int* input_pos = NULL;
            int cursor_pos = -1;
            
            // Test the API
            result = lou_translatePrehyphenated(table_name.c_str(), trans_input, &inlen,
                                               trans_output, &outlen, NULL, NULL,
                                               output_pos, input_pos, &cursor_pos,
                                               input_hyphens, output_hyphens, mode);
            (void)result;
            
            // Cleanup
            free(trans_input);
            free(trans_output);
            free(input_hyphens);
            free(output_hyphens);
            break;
        }
        
        case 5: {
            // Test combination: charToDots followed by dotsToChar (round-trip)
            // First convert chars to dots
            result = lou_charToDots(table_name.c_str(), input_buffer, output_buffer,
                                   (int)buffer_length, mode);
            if (result) {
                // Then convert dots back to chars (using output as new input)
                widechar* roundtrip_buffer = (widechar*)malloc(buffer_length * sizeof(widechar));
                if (roundtrip_buffer) {
                    result = lou_dotsToChar(table_name.c_str(), output_buffer, roundtrip_buffer,
                                           (int)buffer_length, mode);
                    free(roundtrip_buffer);
                }
            }
            (void)result;
            break;
        }
    }
    
    // Cleanup
    if (input_buffer) free(input_buffer);
    if (output_buffer) free(output_buffer);
    
    return 0;
}

} // namespace harness_003
