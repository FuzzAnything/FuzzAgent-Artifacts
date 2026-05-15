#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <string>
#include <vector>
#include <algorithm>
#include <unistd.h>
#include <fuzzer/FuzzedDataProvider.h>
#include "liblouis/liblouis.h"

namespace harness_004 {
/*
 * liblouis Logging, Configuration and Typeform Fuzzing Harness
 * 
 * This harness targets logging, configuration, and typeform/mode APIs in liblouis
 * that haven't been extensively covered by previous harnesses.
 * 
 * APIs targeted:
 * - lou_setLogLevel: Set logging level with various logLevels
 * - lou_registerLogCallback: Register custom logging callback
 * - lou_logFile: Specify log file for deprecated logging
 * - lou_logPrint: Print error messages to file (deprecated API)
 * - lou_logEnd: Close log file
 * - lou_setDataPath: Set data path for table discovery
 * - lou_registerTableResolver: Register custom table resolver
 * - lou_readCharFromFile: Read character from file with different endianness
 * 
 * Translation testing with:
 * - Different typeforms: plain_text, emphasis, italic, underline, bold
 * - Different modes: computer_braille, no_contract
 * 
 * Semantic differentiation from existing harnesses:
 * - harness_000.cpp: Tests translation with temporary fuzzer-created tables
 * - harness_001.cpp: Tests translation with real project tables and comprehensive APIs
 * - harness_002.cpp: Focuses on table discovery/metadata operations
 * - harness_003.cpp: Specifically targets complex pattern expression matching
 * - This harness: Focuses on logging/configuration APIs and translation with various typeforms/modes
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

// Custom logging callback for testing
static void test_log_callback(logLevels level, const char* message) {
    (void)level;
    (void)message;
    // Just consume the callback for testing
}

// Custom table resolver for testing
static char **test_table_resolver(const char *table, const char *base) {
    // Return NULL to indicate no custom resolution
    // This allows testing resolver registration
    (void)table;
    (void)base;
    return NULL;
}

// Available table files for testing
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

// Log levels for testing
static const logLevels log_levels[] = {
    LOU_LOG_ALL,
    LOU_LOG_DEBUG,
    LOU_LOG_INFO,
    LOU_LOG_WARN,
    LOU_LOG_ERROR,
    LOU_LOG_FATAL,
    LOU_LOG_OFF
};
static const size_t num_log_levels = sizeof(log_levels) / sizeof(log_levels[0]);

// Typeform values for testing
static const formtype typeform_values[] = {
    plain_text,
    emph_1,      // italic
    emph_2,      // underline
    emph_3,      // bold
    emph_4,
    emph_5,
    emph_6,
    emph_7,
    emph_8,
    emph_9,
    emph_10,
    computer_braille,
    no_contract
};
static const size_t num_typeform_values = sizeof(typeform_values) / sizeof(typeform_values[0]);
int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum input for meaningful testing
    if (size < 32) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // 1. Test logging configuration
    // Set log level from fuzzer input
    uint8_t log_level_idx = fdp.ConsumeIntegralInRange<uint8_t>(0, num_log_levels - 1);
    lou_setLogLevel(log_levels[log_level_idx]);
    
    // Register custom logging callback
    lou_registerLogCallback(test_log_callback);
    
    // Test deprecated logging APIs with temporary log file
    char log_filename[] = "/tmp/liblouis_fuzz_log_XXXXXX";
    int log_fd = mkstemp(log_filename);
    if (log_fd >= 0) {
        close(log_fd);
        lou_logFile(log_filename);
        
        // Test lou_logPrint with fuzzer-generated format string
        if (fdp.remaining_bytes() > 16) {
            std::string log_message = fdp.ConsumeRandomLengthString(64);
            lou_logPrint("Fuzzer log: %s", log_message.c_str());
        }
        
        // Close log file
        lou_logEnd();
        
        // Clean up log file
        unlink(log_filename);
    }
    
    // 2. Test configuration APIs
    // Set data path from fuzzer input
    if (fdp.remaining_bytes() > 8) {
        std::string data_path = fdp.ConsumeRandomLengthString(128);
        char* old_path = lou_setDataPath(data_path.c_str());
        // Note: lou_setDataPath returns a pointer to a static buffer, do NOT free it
        (void)old_path; // Mark as used to avoid warnings
    }
    
    // Register custom table resolver
    lou_registerTableResolver(test_table_resolver);
    
    // 3. Test file operations with lou_readCharFromFile
    // Create a temporary test file with fuzzer data
    char test_filename[] = "/tmp/liblouis_fuzz_file_XXXXXX";
    int test_fd = mkstemp(test_filename);
    if (test_fd >= 0) {
        // Write some fuzzer data to the file
        if (fdp.remaining_bytes() > 16) {
            size_t file_size = fdp.ConsumeIntegralInRange<size_t>(1, 256);
            std::vector<uint8_t> file_data = fdp.ConsumeBytes<uint8_t>(file_size);
            write(test_fd, file_data.data(), file_data.size());
            close(test_fd);
            
            // Test reading characters from the file
            int mode = 1;
            while (lou_readCharFromFile(test_filename, &mode) != EOF) {
                mode = 0;  // Set mode to 0 after first call as per documentation
            }
        } else {
            close(test_fd);
        }
        
        // Clean up test file
        unlink(test_filename);
    }
    
    // 4. Test translation with various typeforms and modes
    // Select a table for translation testing
    if (fdp.remaining_bytes() < 8) {
        lou_free();
        return 0;
    }
    
    uint8_t table_index = fdp.ConsumeIntegralInRange<uint8_t>(0, num_tables - 1);
    const char* table_name = available_tables[table_index];
    
    // Check if table is valid
    if (lou_checkTable(table_name) == 0) {
        lou_free();
        return 0;
    }
    
    // Get remaining input for text
    size_t remaining_bytes = fdp.remaining_bytes();
    if (remaining_bytes < 8) {
        lou_free();
        return 0;
    }
    
    // Convert input text to widechar
    std::string text_input = fdp.ConsumeRandomLengthString(512);
    if (text_input.empty()) {
        lou_free();
        return 0;
    }
    
    widechar inbuf[1024];
    int inlen = simple_string_to_widechar(text_input.c_str(), text_input.size(), inbuf);
    if (inlen <= 0) {
        lou_free();
        return 0;
    }
    
    // Prepare output buffers
    widechar outbuf[2048];
    int outlen = sizeof(outbuf) / sizeof(outbuf[0]);
    
    // Test different typeforms
    uint8_t typeform_idx = fdp.ConsumeIntegralInRange<uint8_t>(0, num_typeform_values - 1);
    formtype selected_typeform = typeform_values[typeform_idx];
    
    // Create typeform array (same typeform for all characters)
    formtype typeform_array[1024];
    for (int i = 0; i < inlen; i++) {
        typeform_array[i] = selected_typeform;
    }
    
    // Select translation mode
    int mode = 0;
    uint8_t mode_selector = fdp.ConsumeIntegral<uint8_t>();
    if (mode_selector & 0x01) {
        mode |= computer_braille;
    }
    if (mode_selector & 0x02) {
        mode |= no_contract;
    }
    if (mode_selector & 0x04) {
        mode |= noContractions;
    }
    if (mode_selector & 0x08) {
        mode |= compbrlAtCursor;
    }
    
    // Test lou_translateString with typeforms
    int inlen_copy = inlen;
    int outlen_copy = outlen;
    lou_translateString(table_name, inbuf, &inlen_copy, outbuf, &outlen_copy, 
                       typeform_array, NULL, mode);
    
    // Test lou_translate with cursor position tracking
    if (fdp.remaining_bytes() > 4) {
        int outputPos[1024];
        int inputPos[2048];
        int cursorPos = 0;
        char spacing[1024];
        
        inlen_copy = inlen;
        outlen_copy = outlen;
        lou_translate(table_name, inbuf, &inlen_copy, outbuf, &outlen_copy,
                     typeform_array, spacing, outputPos, inputPos, &cursorPos, mode);
    }
    
    // Test back translation
    if (outlen_copy > 0 && fdp.remaining_bytes() > 4) {
        widechar back_inbuf[2048];
        widechar back_outbuf[1024];
        int back_inlen = outlen_copy;
        int back_outlen = sizeof(back_outbuf) / sizeof(back_outbuf[0]);
        
        // Copy output to input for back translation
        for (int i = 0; i < back_inlen && i < 2048; i++) {
            back_inbuf[i] = outbuf[i];
        }
        
        lou_backTranslateString(table_name, back_inbuf, &back_inlen, back_outbuf, &back_outlen, 
                               NULL, NULL, mode);
    }
    
    // Clean up
    lou_free();
    
    return 0;
}

} // namespace harness_004
