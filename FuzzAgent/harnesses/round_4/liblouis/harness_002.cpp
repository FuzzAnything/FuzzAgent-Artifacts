#include <fuzzer/FuzzedDataProvider.h>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>
#include <iostream>
#include "liblouis.h"

namespace harness_002 {
// liblouis hyphenation fuzzing harness
// Targets hyphenation APIs: lou_hyphenate, lou_translatePrehyphenated
// With required helper functions: lou_getTable, lou_translate, lou_free
// Based on coverage analysis guidance to target completely uncovered hyphenation functionality


// Include liblouis public header
extern "C" {
}

// Log callback to suppress logs during fuzzing
static void avoid_log_callback(logLevels level, const char *msg) {
    (void)level;
    (void)msg;
}

// Simple ASCII to widechar conversion
static size_t ascii_to_widechar(const char* ascii, widechar* wide, size_t max_len) {
    size_t i;
    for (i = 0; i < max_len && ascii[i] != '\0'; i++) {
        wide[i] = ascii[i];
    }
    return i;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough for table selection, text strings, and parameters
    if (size < 32) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize liblouis once
    static bool initialized = false;
    if (!initialized) {
        lou_registerLogCallback(avoid_log_callback);
        initialized = true;
    }
    
    // 1. Select a translation table for hyphenation testing
    // Use tables that support hyphenation (English tables are good candidates)
    const char* tables[] = {
        "en-us-g2.ctb",      // US English Grade 2 braille - likely supports hyphenation
        "en-ueb-g2.ctb",     // Unified English Braille Grade 2
        "en-us-comp8.ctb",   // US English computer braille
        "en-gb-g2.ctb",      // UK English Grade 2 braille
        "afr-za-g2.ctb",     // Afrikaans Grade 2 braille
        "de-de-g2.ctb"       // German Grade 2 braille
    };
    const size_t num_tables = sizeof(tables) / sizeof(tables[0]);
    size_t table_idx = fdp.ConsumeIntegralInRange<size_t>(0, num_tables - 1);
    const char* table = tables[table_idx];
    
    // 2. Load the table using lou_getTable
    // This compiles and loads the translation table into memory
    const void* loaded_table = lou_getTable(table);
    if (loaded_table == NULL) {
        // Table loading failed, which is OK for fuzzing - still continue
        // Some tables might not exist in the current environment
    }
    
    // 3. Consume text input for hyphenation testing
    // Determine text length and content from fuzzer input
    size_t max_text_len = fdp.ConsumeIntegralInRange<size_t>(1, 512);
    std::string text = fdp.ConsumeRandomLengthString(max_text_len);
    
    if (text.empty()) {
        return 0;
    }
    
    // Convert to widechar format required by liblouis APIs
    std::vector<widechar> inbuf(text.size() + 1);
    size_t inlen = ascii_to_widechar(text.c_str(), inbuf.data(), text.size());
    int inlen_int = static_cast<int>(inlen);
    
    // 4. Test lou_hyphenate API
    // Allocate hyphenation result buffer (1 char per input character)
    std::vector<char> hyphens(inlen_int + 1);
    
    // Select mode for hyphenation
    int mode = 0;  // plain text mode
    uint8_t mode_choice = fdp.ConsumeIntegral<uint8_t>() % 4;
    switch (mode_choice) {
        case 0:
            mode = 0;  // plain_text
            break;
        case 1:
            mode = 0x0400;  // computer_braille
            break;
        case 2:
            mode = 0x0800;  // no_translate
            break;
        case 3:
            mode = 0x1000;  // no_contract
            break;
    }
    
    // Call lou_hyphenate to get hyphenation positions
    int hyphen_result = lou_hyphenate(table, inbuf.data(), inlen_int, hyphens.data(), mode);
    
    // 5. Test lou_translatePrehyphenated API
    // This API performs translation with pre-existing hyphenation information
    if (hyphen_result == 1 && inlen_int > 0) {
        // Allocate buffers for translation with hyphenation
        int outlen_int = inlen_int * 16;  // Braille can expand
        std::vector<widechar> outbuf(outlen_int + 1);
        
        // Allocate arrays for position tracking (optional)
        std::vector<int> outputPos(inlen_int);
        std::vector<int> inputPos(outlen_int);
        int cursorPos = -1;
        
        // Allocate arrays for hyphenation information
        std::vector<char> inputHyphens(inlen_int + 1);
        std::vector<char> outputHyphens(outlen_int + 1);
        
        // Copy the hyphenation results to inputHyphens
        for (int i = 0; i < inlen_int; i++) {
            inputHyphens[i] = hyphens[i];
        }
        
        // Call lou_translatePrehyphenated
        // This translates the text while considering hyphenation positions
        int translate_result = lou_translatePrehyphenated(
            table, inbuf.data(), &inlen_int,
            outbuf.data(), &outlen_int,
            NULL, NULL,  // typeform and spacing (optional)
            outputPos.data(), inputPos.data(), &cursorPos,
            inputHyphens.data(), outputHyphens.data(),
            mode
        );
        
        // 6. Also test regular lou_translate for comparison
        if (fdp.remaining_bytes() > 10) {
            // Reset input length
            int compare_inlen = static_cast<int>(inlen);
            int compare_outlen = compare_inlen * 16;
            std::vector<widechar> compare_outbuf(compare_outlen + 1);
            std::vector<int> compare_outputPos(compare_inlen);
            std::vector<int> compare_inputPos(compare_outlen);
            int compare_cursorPos = -1;
            
            // Regular translation without hyphenation
            int regular_translate = lou_translate(
                table, inbuf.data(), &compare_inlen,
                compare_outbuf.data(), &compare_outlen,
                NULL, NULL,
                compare_outputPos.data(), compare_inputPos.data(), &compare_cursorPos,
                mode
            );
        }
    }
    
    // 7. Clean up using lou_free
    // This frees all memory allocated by liblouis
    lou_free();
    
    return 0;
}

} // namespace harness_002
