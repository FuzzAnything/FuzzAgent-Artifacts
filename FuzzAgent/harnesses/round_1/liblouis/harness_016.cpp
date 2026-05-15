#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <string>
#include <vector>
#include <algorithm>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <fuzzer/FuzzedDataProvider.h>
#include "liblouis/liblouis.h"

namespace harness_016 {
/*
 * liblouis Comprehensive Translation Modes and Library Information Fuzzing Harness (harness_016.cpp)
 * 
 * This harness focuses on comprehensive testing of translation modes enum values,
 * library information queries, and file-based operations that haven't been
 * thoroughly covered in previous harnesses.
 * 
 * Primary focus areas:
 * 1) Comprehensive testing of all translationModes enum values and combinations:
 *    - noContractions, compbrlAtCursor, dotsIO, compbrlLeftCursor, ucBrl, noUndefined, partialTrans
 *    - All possible bitwise combinations of translation modes
 *    - Mode interaction with different table types and input scenarios
 * 
 * 2) Library information and configuration queries:
 *    - lou_version(): Library version information testing
 *    - lou_charSize(): Wide character size queries
 *    - Testing library state queries during active translation sessions
 * 
 * 3) File-based operations and advanced testing:
 *    - lou_readCharFromFile: File character reading with different modes
 *    - Testing file operations alongside translation functions
 *    - Simulating real-world file-based translation workflows
 * 
 * 4) Mode-specific edge cases:
 *    - noContractions mode with contraction-heavy tables
 *    - compbrlAtCursor and compbrlLeftCursor cursor positioning
 *    - dotsIO mode for dot pattern preservation
 *    - ucBrl mode for Unicode braille handling
 *    - noUndefined mode for error handling
 *    - partialTrans mode for partial translation scenarios
 * 
 * Semantic differentiation from previous harnesses:
 * - harness_004.cpp: Basic logging/configuration, limited mode testing
 * - harness_009.cpp: Typeform mode combinations, not translationModes enum
 * - harness_010.cpp: Spacing patterns, not translation mode bitwise combinations
 * - harness_015.cpp: Windows-specific APIs, not translation modes
 * - This harness: FOCUSES EXCLUSIVELY on translationModes enum, library info queries,
 *                  and comprehensive mode combination testing
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

// Available table files for testing with different characteristics
static const char* available_tables[] = {
    // English tables (good for contraction testing)
    "/root/src/liblouis/tables/en-us-g2.ctb",      // Grade 2 with contractions
    "/root/src/liblouis/tables/en-us-g1.ctb",      // Grade 1 without contractions
    "/root/src/liblouis/tables/en-us-comp8.ctb",   // Computer braille
    
    // Tables with Unicode support
    "/root/src/liblouis/tables/Es-Es-G0.utb",      // Unicode braille
    "/root/src/liblouis/tables/IPA.utb",           // IPA Unicode
    
    // International tables for diverse testing
    "/root/src/liblouis/tables/afr-za-g2.ctb",     // African language with patterns
    "/root/src/liblouis/tables/Lv-Lv-g1.utb",      // Latvian Unicode
    "/root/src/liblouis/tables/Pl-Pl-g1.utb",      // Polish Unicode
    
    // Tables for specific mode testing
    "/root/src/liblouis/tables/ar-ar-g2.ctb",      // Arabic with right-to-left
    "/root/src/liblouis/tables/zh-tw.ctb",         // Chinese for complex patterns
};
static const size_t num_tables = sizeof(available_tables) / sizeof(available_tables[0]);

// Individual translation modes for comprehensive testing
static const int translation_modes[] = {
    0,  // Default/no mode
    
    // Individual mode flags
    noContractions,
    compbrlAtCursor,
    dotsIO,
    compbrlLeftCursor,
    ucBrl,
    noUndefined,
    partialTrans,
    
    // Common combinations
    noContractions | compbrlAtCursor,
    noContractions | dotsIO,
    compbrlAtCursor | compbrlLeftCursor,
    ucBrl | noUndefined,
    noContractions | partialTrans,
    dotsIO | ucBrl,
    
    // Complex combinations
    noContractions | compbrlAtCursor | dotsIO,
    compbrlAtCursor | ucBrl | noUndefined,
    noContractions | ucBrl | partialTrans,
    dotsIO | compbrlLeftCursor | noUndefined,
    
    // All modes combined (maximum testing)
    noContractions | compbrlAtCursor | dotsIO | compbrlLeftCursor | ucBrl | noUndefined | partialTrans,
};
static const size_t num_modes = sizeof(translation_modes) / sizeof(translation_modes[0]);

// Create a temporary file with test data for lou_readCharFromFile
static std::string create_temp_file(FuzzedDataProvider& fdp, size_t max_size = 1024) {
    char temp_filename[] = "/tmp/liblouis_fuzz_XXXXXX";
    int fd = mkstemp(temp_filename);
    if (fd < 0) {
        return "";
    }
    
    // Write some test data to the file
    size_t file_size = fdp.ConsumeIntegralInRange<size_t>(1, max_size);
    std::vector<uint8_t> file_data = fdp.ConsumeBytes<uint8_t>(file_size);
    
    if (write(fd, file_data.data(), file_data.size()) != (ssize_t)file_data.size()) {
        close(fd);
        unlink(temp_filename);
        return "";
    }
    
    close(fd);
    return std::string(temp_filename);
}

// Test a specific translation mode with comprehensive parameters
static void test_translation_mode(const char* table_name, const widechar* inbuf, int inlen,
                                  int mode, FuzzedDataProvider& fdp) {
    // Allocate buffers
    widechar outbuf[2048];
    formtype typeform[2048];
    char spacing[2048];
    int outputPos[2048];
    int inputPos[2048];
    int cursorPos = 0;
    
    int outlen = 2048;
    int actual_inlen = inlen;
    
    // Test lou_translateString with the mode
    int result = lou_translateString(table_name, inbuf, &actual_inlen, 
                                     outbuf, &outlen, typeform, spacing, mode);
    (void)result; // Result may be 0 or 1 depending on input
    
    // Test lou_translate with the mode (cursor position tracking)
    outlen = 2048;
    actual_inlen = inlen;
    result = lou_translate(table_name, inbuf, &actual_inlen, outbuf, &outlen,
                          typeform, spacing, outputPos, inputPos, &cursorPos, mode);
    
    // Test lou_backTranslateString with the mode
    outlen = 2048;
    actual_inlen = inlen;
    result = lou_backTranslateString(table_name, inbuf, &actual_inlen,
                                    outbuf, &outlen, typeform, spacing, mode);
    
    // Test lou_backTranslate with the mode
    outlen = 2048;
    actual_inlen = inlen;
    result = lou_backTranslate(table_name, inbuf, &actual_inlen, outbuf, &outlen,
                              typeform, spacing, outputPos, inputPos, &cursorPos, mode);
    
    // Test hyphenation with the mode
    if (inlen > 0 && inlen < 2048) {
        char hyphens[2048];
        result = lou_hyphenate(table_name, inbuf, inlen, hyphens, mode);
    }
    
    // Test dot conversion with the mode if applicable
    if (mode & dotsIO) {
        widechar dot_buffer[2048];
        int conv_length = std::min(inlen, 2048);
        
        // Test lou_charToDots
        result = lou_charToDots(table_name, inbuf, dot_buffer, conv_length, mode);
        
        // Test lou_dotsToChar (if we have dot patterns)
        if (result) {
            widechar char_buffer[2048];
            result = lou_dotsToChar(table_name, dot_buffer, char_buffer, conv_length, mode);
        }
    }
}

// Test file reading operations
static void test_file_operations(FuzzedDataProvider& fdp) {
    // Create a temporary file for testing
    std::string temp_file = create_temp_file(fdp, 512);
    if (temp_file.empty()) {
        return;
    }
    
    // Test lou_readCharFromFile with different modes
    int mode = 1; // First call mode
    int char_result = 0;
    
    // Read several characters from the file
    for (int i = 0; i < 10; i++) {
        char_result = lou_readCharFromFile(temp_file.c_str(), &mode);
        mode = 0; // Subsequent calls
        if (char_result == EOF) {
            break;
        }
    }
    
    // Clean up temporary file
    unlink(temp_file.c_str());
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Initialize logging once
    static int initialized = 0;
    if (!initialized) {
        lou_registerLogCallback(fuzz_log_callback);
        initialized = 1;
    }
    
    // Need minimum input for meaningful testing
    if (size < 16) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // 1. Query library information (always safe)
    const char* version = lou_version();
    (void)version;
    
    int char_size = lou_charSize();
    (void)char_size;
    
    // 2. Select a table for testing
    uint8_t table_index = fdp.ConsumeIntegralInRange<uint8_t>(0, num_tables - 1);
    const char* table_name = available_tables[table_index];
    
    // Check if table is valid
    if (lou_checkTable(table_name) == 0) {
        // Try to get the table directly
        const void* table = lou_getTable(table_name);
        if (table == NULL) {
            return 0; // No valid table available
        }
    }
    
    // 3. Prepare text input from remaining bytes
    size_t remaining_bytes = fdp.remaining_bytes();
    if (remaining_bytes < 8) {
        lou_free();
        return 0;
    }
    
    std::string text_input = fdp.ConsumeRandomLengthString(512);
    if (text_input.empty()) {
        lou_free();
        return 0;
    }
    
    // Convert to widechar for translation
    widechar inbuf[1024];
    int inlen = simple_string_to_widechar(text_input.c_str(), text_input.size(), inbuf);
    if (inlen <= 0) {
        lou_free();
        return 0;
    }
    
    // 4. Test different aspects based on input
    uint8_t test_type = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    switch (test_type) {
        case 0: {
            // Test comprehensive translation modes
            uint8_t mode_index = fdp.ConsumeIntegralInRange<uint8_t>(0, num_modes - 1);
            int test_mode = translation_modes[mode_index];
            
            // Also test with typeform modes combined
            uint8_t typeform_mode = fdp.ConsumeIntegral<uint8_t>() % 8;
            switch (typeform_mode) {
                case 0: test_mode |= computer_braille; break;
                case 1: test_mode |= no_translate; break;
                case 2: test_mode |= no_contract; break;
                case 3: test_mode |= computer_braille | no_translate; break;
                case 4: test_mode |= computer_braille | no_contract; break;
                case 5: test_mode |= no_translate | no_contract; break;
                // cases 6-7: keep base mode only
            }
            
            test_translation_mode(table_name, inbuf, inlen, test_mode, fdp);
            break;
        }
            
        case 1: {
            // Test all translation modes systematically
            for (size_t i = 0; i < num_modes && fdp.remaining_bytes() > 0; i++) {
                int test_mode = translation_modes[i];
                
                // Test with a subset of input
                int test_inlen = std::min(inlen, 64);
                test_translation_mode(table_name, inbuf, test_inlen, test_mode, fdp);
                
                // Also test with computer_braille mode added
                test_translation_mode(table_name, inbuf, test_inlen, 
                                     test_mode | computer_braille, fdp);
            }
            break;
        }
            
        case 2: {
            // Test mode combinations with file operations
            test_file_operations(fdp);
            
            // Also test translation with a mode
            uint8_t mode_index = fdp.ConsumeIntegralInRange<uint8_t>(0, num_modes - 1);
            int test_mode = translation_modes[mode_index];
            test_translation_mode(table_name, inbuf, inlen, test_mode, fdp);
            break;
        }
            
        case 3: {
            // Test specific mode behaviors
            
            // Test noContractions mode (should work with any table)
            test_translation_mode(table_name, inbuf, inlen, noContractions, fdp);
            
            // Test ucBrl mode (Unicode braille)
            test_translation_mode(table_name, inbuf, inlen, ucBrl, fdp);
            
            // Test noUndefined mode (error handling)
            test_translation_mode(table_name, inbuf, inlen, noUndefined, fdp);
            
            // Test partialTrans mode
            test_translation_mode(table_name, inbuf, inlen, partialTrans, fdp);
            
            // Test cursor modes
            test_translation_mode(table_name, inbuf, inlen, compbrlAtCursor, fdp);
            test_translation_mode(table_name, inbuf, inlen, compbrlLeftCursor, fdp);
            test_translation_mode(table_name, inbuf, inlen, 
                                 compbrlAtCursor | compbrlLeftCursor, fdp);
            
            // Test dotsIO mode
            test_translation_mode(table_name, inbuf, inlen, dotsIO, fdp);
            break;
        }
    }
    
    // 5. Final library information queries
    version = lou_version(); // Query again after operations
    char_size = lou_charSize();
    
    // 6. Cleanup
    lou_free();
    
    return 0;
}

} // namespace harness_016
