#include <config.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <fuzzer/FuzzedDataProvider.h>
#include "liblouis.h"

namespace harness_001 {
// Targeted liblouis fuzzing harness for braille character encoding/decoding operations
// Harness_001: Specifically targets lou_charToDots and lou_dotsToChar functions
// Focuses on character-level braille dot pattern conversions (32+ undiscovered branches)




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
    
    // Initialize liblouis logging once
    static int initialized = 0;
    if (!initialized) {
        lou_registerLogCallback(avoid_log);
        initialized = 1;
    }
    
    // Consume operation type from fuzzer input
    // 0: charToDots only, 1: dotsToChar only, 2: round-trip (charToDots then dotsToChar)
    // 3: test with different mode flags, 4: test with invalid table
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 5;
    
    // Consume mode parameter - different modes affect charToDots behavior
    // According to source: mode & ucBrl affects output format (Unicode braille vs liblouis dots)
    int mode = fdp.ConsumeIntegral<int>();
    
    // Consume length for conversion (limited to reasonable size)
    int length = fdp.ConsumeIntegralInRange<int>(1, 256);
    
    // Select a table from available braille tables
    // Use a small set of common tables to increase chance of valid conversions
    const char* tables[] = {
        "en-us-g2.ctb",      // US English Grade 2
        "en-ueb-g2.ctb",     // Unified English Braille Grade 2  
        "en-us-comp6.ctb",   // US computer braille 6-dot
        "en-us-comp8.ctb",   // US computer braille 8-dot
        "de-g0.utb",         // German Grade 0
        "sv-g1.ctb",         // Swedish Grade 1
        NULL
    };
    
    int table_index = fdp.ConsumeIntegralInRange<int>(0, 5);
    const char* table_name = tables[table_index];
    
    // Check if table is valid before using it
    int table_valid = 0;
    if (operation != 4) {  // Skip table check for invalid table test
        table_valid = lou_checkTable(table_name);
    }
    
    // If table is not valid, try to load with lou_getTable as fallback
    const void* table_ptr = NULL;
    if (!table_valid && operation != 4) {
        table_ptr = lou_getTable(table_name);
        if (table_ptr == NULL) {
            // Table loading failed, can't proceed with most operations
            if (operation == 0 || operation == 1 || operation == 2 || operation == 3) {
                return 0;
            }
        }
    }
    
    // Consume text input for conversion
    // For charToDots: need text characters
    // For dotsToChar: need dot patterns (braille dots representation)
    std::string input_bytes = fdp.ConsumeBytesAsString(fdp.ConsumeIntegralInRange<size_t>(length, length * 2));
    if (input_bytes.empty() && length > 0) {
        return 0;
    }
    
    // Convert input to widechar format (required by liblouis API)
    widechar* input_buf = (widechar*)malloc((length + 1) * sizeof(widechar));
    widechar* output_buf = (widechar*)malloc((length + 1) * sizeof(widechar));
    widechar* intermediate_buf = NULL;
    
    if (!input_buf || !output_buf) {
        free(input_buf);
        free(output_buf);
        return 0;
    }
    
    // Initialize buffers
    memset(input_buf, 0, (length + 1) * sizeof(widechar));
    memset(output_buf, 0, (length + 1) * sizeof(widechar));
    
    // Fill input buffer based on operation type
    if (operation == 1 || operation == 2) {
        // For dotsToChar, we need dot patterns (braille dots)
        // Dot patterns in liblouis: 6-dot pattern bits 0-5, 8-dot pattern bits 0-7
        // Unicode braille: row indicator + dot pattern
        for (int i = 0; i < length && i < (int)input_bytes.size(); i++) {
            // Create dot pattern from input byte
            uint8_t dots = (uint8_t)input_bytes[i];
            // Make it a valid dot pattern (bits 0-5/7 for dots, plus LOU_DOTS flag)
            input_buf[i] = (dots & 0xFF) | LOU_DOTS;
        }
    } else {
        // For charToDots, we need text characters
        // Simple conversion: treat input bytes as characters (ASCII/latin-1)
        for (int i = 0; i < length && i < (int)input_bytes.size(); i++) {
            input_buf[i] = (widechar)(unsigned char)input_bytes[i];
        }
    }
    
    int result = 0;
    
    switch (operation) {
        case 0:
            // Test lou_charToDots only
            if (table_valid || table_ptr) {
                result = lou_charToDots(table_name, input_buf, output_buf, length, mode);
                // Result should be 1 on success, 0 on failure
                // We don't assert - just let fuzzer explore
            }
            break;
            
        case 1:
            // Test lou_dotsToChar only  
            if (table_valid || table_ptr) {
                result = lou_dotsToChar(table_name, input_buf, output_buf, length, mode);
                // Result should be 1 on success, 0 on failure
            }
            break;
            
        case 2:
            // Test round-trip: charToDots then dotsToChar
            if (table_valid || table_ptr) {
                intermediate_buf = (widechar*)malloc((length + 1) * sizeof(widechar));
                if (!intermediate_buf) break;
                
                memset(intermediate_buf, 0, (length + 1) * sizeof(widechar));
                
                // First convert characters to dots
                result = lou_charToDots(table_name, input_buf, intermediate_buf, length, mode);
                if (result) {
                    // Then convert dots back to characters
                    result = lou_dotsToChar(table_name, intermediate_buf, output_buf, length, mode);
                }
            }
            break;
            
        case 3:
            // Test with different mode flags combinations
            // Try toggling the ucBrl flag which affects Unicode braille output
            if (table_valid || table_ptr) {
                int test_mode = mode;
                // Toggle ucBrl flag (64 = 0x40)
                if (fdp.ConsumeBool()) {
                    test_mode |= 0x40;  // ucBrl flag for Unicode braille
                }
                
                result = lou_charToDots(table_name, input_buf, output_buf, length, test_mode);
                if (result) {
                    // Also test dotsToChar with same mode
                    result = lou_dotsToChar(table_name, output_buf, intermediate_buf ? intermediate_buf : input_buf, length, test_mode);
                }
            }
            break;
        case 4:
            // Test with invalid/empty table name to exercise error paths
            result = lou_charToDots("", input_buf, output_buf, length, mode);
            // Should return 0 (failure) for invalid table
            break;
    }
    
    // Clean up
    free(input_buf);
    free(output_buf);
    if (intermediate_buf) free(intermediate_buf);
    
    // Always return 0 - libFuzzer expects 0 on both success and failure
    return 0;
}

} // namespace harness_001
