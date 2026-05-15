#include <fuzzer/FuzzedDataProvider.h>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>
#include <iostream>
#include "liblouis.h"

namespace harness_006 {
// liblouis hyphenation and pre-hyphenated translation fuzzing harness
// Targets: lou_translatePrehyphenated (0% coverage, 20 undiscovered branches) and lou_hyphenate (9.62% coverage, 48 undiscovered branches)
// Based on coverage analysis: previous harness_002 failed to trigger hyphenation code paths due to missing preconditions
// Focus: Proper hyphen array handling for lou_translatePrehyphenated
// Invocation sequence: 1. lou_setLogLevel (optional), 2. lou_getTable/lou_compileString, 3. lou_hyphenate, 
//                      4. lou_translatePrehyphenated, 5. lou_free for cleanup
// Semantic differentiation from harness_002: Focus on correct hyphen array creation and passing


// Include liblouis public header
extern "C" {
}

// Log callback to suppress logs during fuzzing
static void fuzz_log_callback(logLevels level, const char *msg) {
    (void)level;
    (void)msg;
    // Suppress all logs during fuzzing
}

// Simple ASCII to widechar conversion for testing
static size_t ascii_to_widechar(const char* ascii, widechar* wide, size_t max_len) {
    size_t i;
    for (i = 0; i < max_len && ascii[i] != '\0'; i++) {
        wide[i] = ascii[i];
    }
    return i;
}

// Generate hyphen pattern from fuzzer input
static void generate_hyphen_pattern(FuzzedDataProvider& fdp, char* hyphens, int len) {
    for (int i = 0; i < len; i++) {
        // Hyphen patterns are typically '0' (no hyphen) or '1' (hyphen)
        // Could also be other characters based on liblouis implementation
        if (fdp.ConsumeBool()) {
            hyphens[i] = '1';  // Hyphen at this position
        } else {
            hyphens[i] = '0';  // No hyphen at this position
        }
    }
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough for table selection, text, and parameters
    if (size < 64) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // 1. Optional: Set log level for debugging (suppress during fuzzing)
    // Use a low log level to minimize output
    lou_setLogLevel(LOU_LOG_FATAL);  // Only show fatal errors
    
    // Register custom log callback to suppress output
    lou_registerLogCallback(fuzz_log_callback);
    
    // 2. Select and load hyphenation-enabled translation tables
    // Tables known to support hyphenation based on liblouis documentation
    const char* hyphenation_tables[] = {
        "en-us-g2.ctb",       // US English Grade 2 - likely supports hyphenation
        "en-ueb-g2.ctb",      // Unified English Braille Grade 2
        "en-gb-g2.ctb",       // UK English Grade 2
        "de-de-g2.ctb",       // German Grade 2
        "fr-bfu-g2.ctb",      // French Grade 2
        "es-g2.ctb",          // Spanish Grade 2
        "nl-NL-g2.ctb"        // Dutch Grade 2
    };
    const size_t num_tables = sizeof(hyphenation_tables) / sizeof(hyphenation_tables[0]);
    size_t table_idx = fdp.ConsumeIntegralInRange<size_t>(0, num_tables - 1);
    const char* table = hyphenation_tables[table_idx];
    
    // Try to load table using lou_getTable
    const void* loaded_table = lou_getTable(table);
    if (loaded_table == NULL) {
        // Table loading failed, try lou_compileString with a simple table entry
        // This ensures we have a valid table for hyphenation testing
        const char* simple_table_entry = "include en-us-g2.ctb\n";
        lou_compileString(table, simple_table_entry);
        
        // Try loading again
        loaded_table = lou_getTable(table);
        if (loaded_table == NULL) {
            // Still failed, but continue anyway - some paths might handle NULL tables
        }
    }
    
    // 3. Consume text input for hyphenation testing
    size_t max_text_len = fdp.ConsumeIntegralInRange<size_t>(1, 256);
    std::string text = fdp.ConsumeRandomLengthString(max_text_len);
    
    if (text.empty()) {
        // Clean up and return
        lou_free();
        return 0;
    }
    
    // Convert to widechar format required by liblouis APIs
    std::vector<widechar> inbuf(text.size() + 1);
    size_t inlen = ascii_to_widechar(text.c_str(), inbuf.data(), text.size());
    int inlen_int = static_cast<int>(inlen);
    
    // 4. Test lou_hyphenate API first to get proper hyphenation patterns
    // Allocate hyphenation result buffer (1 char per input character + null terminator)
    std::vector<char> hyphens(inlen_int + 1);
    
    // Select mode for hyphenation
    int mode = 0;  // plain text mode
    uint8_t mode_choice = fdp.ConsumeIntegral<uint8_t>() % 8;
    switch (mode_choice) {
        case 0:
            mode = 0;           // plain_text
            break;
        case 1:
            mode = 0x0001;      // emph_1
            break;
        case 2:
            mode = 0x0002;      // emph_2 (underline)
            break;
        case 3:
            mode = 0x0004;      // emph_3 (bold)
            break;
        case 4:
            mode = 0x0400;      // computer_braille
            break;
        case 5:
            mode = 0x0800;      // no_translate
            break;
        case 6:
            mode = 0x1000;      // no_contract
            break;
        case 7:
            mode = 0;           // plain_text with possible emphasis bits
            if (fdp.ConsumeBool()) mode |= 0x0020;  // emph_6
            if (fdp.ConsumeBool()) mode |= 0x0040;  // emph_7
            break;
    }
    
    // Call lou_hyphenate to get actual hyphenation positions
    int hyphen_result = lou_hyphenate(table, inbuf.data(), inlen_int, hyphens.data(), mode);
    
    // 5. Test lou_translatePrehyphenated API - PRIMARY TARGET
    // This is the stubborn API with 0% coverage that we need to hit
    
    // Allocate buffers for translation with hyphenation
    // Braille translation can expand text, so allocate generous output buffer
    int outlen_int = inlen_int * 16;
    if (outlen_int < 16) outlen_int = 16;  // Minimum buffer size
    std::vector<widechar> outbuf(outlen_int + 1);
    
    // Allocate arrays for position tracking (required by lou_translatePrehyphenated)
    std::vector<int> outputPos(inlen_int);
    std::vector<int> inputPos(outlen_int);
    int cursorPos = -1;
    
    // Allocate arrays for hyphenation information
    // CRITICAL: inputHyphens and outputHyphens must be properly allocated
    std::vector<char> inputHyphens(inlen_int + 1);
    std::vector<char> outputHyphens(outlen_int + 1);
    
    // Prepare hyphen arrays based on lou_hyphenate results or generate our own
    if (hyphen_result == 1 && inlen_int > 0) {
        // Use hyphenation results from lou_hyphenate
        for (int i = 0; i < inlen_int; i++) {
            inputHyphens[i] = hyphens[i];
        }
    } else {
        // lou_hyphenate failed or returned no results, generate our own hyphen pattern
        generate_hyphen_pattern(fdp, inputHyphens.data(), inlen_int);
        
        // Also try with all zeros (no hyphens) to test that code path
        if (fdp.ConsumeBool()) {
            for (int i = 0; i < inlen_int; i++) {
                inputHyphens[i] = '0';
            }
        }
    }
    
    // Null-terminate the hyphen arrays
    inputHyphens[inlen_int] = '\0';
    outputHyphens[outlen_int] = '\0';
    
    // Test multiple scenarios with lou_translatePrehyphenated
    int translate_result = 0;
    
    // Scenario 1: With hyphen arrays (main test case)
    int test_inlen = inlen_int;
    int test_outlen = outlen_int;
    translate_result = lou_translatePrehyphenated(
        table, inbuf.data(), &test_inlen,
        outbuf.data(), &test_outlen,
        NULL, NULL,  // typeform and spacing (optional)
        outputPos.data(), inputPos.data(), &cursorPos,
        inputHyphens.data(), outputHyphens.data(),
        mode
    );
    
    // Scenario 2: With NULL inputHyphens (should skip hyphen processing)
    if (fdp.remaining_bytes() > 16 && inlen_int > 0) {
        test_inlen = inlen_int;
        test_outlen = outlen_int;
        std::vector<widechar> outbuf2(outlen_int + 1);
        std::vector<char> outputHyphens2(outlen_int + 1);
        
        // According to source code, when inputHyphens is NULL, 
        // lou_translatePrehyphenated should skip hyphen array processing
        int result2 = lou_translatePrehyphenated(
            table, inbuf.data(), &test_inlen,
            outbuf2.data(), &test_outlen,
            NULL, NULL,
            outputPos.data(), inputPos.data(), &cursorPos,
            NULL, outputHyphens2.data(),  // inputHyphens = NULL
            mode
        );
    }
    
    // Scenario 3: With different mode settings
    if (fdp.remaining_bytes() > 8 && inlen_int > 0) {
        test_inlen = inlen_int;
        test_outlen = outlen_int;
        std::vector<widechar> outbuf3(outlen_int + 1);
        std::vector<char> inputHyphens3(inlen_int + 1);
        std::vector<char> outputHyphens3(outlen_int + 1);
        
        // Generate different hyphen pattern
        generate_hyphen_pattern(fdp, inputHyphens3.data(), inlen_int);
        inputHyphens3[inlen_int] = '\0';
        
        // Try with computer braille mode specifically
        int result3 = lou_translatePrehyphenated(
            table, inbuf.data(), &test_inlen,
            outbuf3.data(), &test_outlen,
            NULL, NULL,
            NULL, NULL, NULL,  // No position tracking
            inputHyphens3.data(), outputHyphens3.data(),
            0x0400  // computer_braille mode
        );
    }
    
    // 6. Additional test: Verify outputHyphens array was filled
    if (translate_result == 1 && test_outlen > 0 && test_outlen <= outlen_int) {
        // Check that outputHyphens array has valid data
        // According to source, outputHyphens should contain hyphen info for output positions
        for (int i = 0; i < test_outlen; i++) {
            // outputHyphens[i] should be '0' or '1' or other valid hyphen char
            char h = outputHyphens[i];
            (void)h;  // Just access to ensure array was written to
        }
    }
    
    // 7. Test edge cases with very short input
    if (fdp.remaining_bytes() > 4) {
        // Single character input
        std::vector<widechar> single_inbuf(2);
        single_inbuf[0] = 'a';
        int single_inlen = 1;
        int single_outlen = 16;
        std::vector<widechar> single_outbuf(single_outlen + 1);
        std::vector<char> single_inputHyphens(2);
        std::vector<char> single_outputHyphens(single_outlen + 1);
        single_inputHyphens[0] = '0';
        single_inputHyphens[1] = '\0';
        
        int single_result = lou_translatePrehyphenated(
            table, single_inbuf.data(), &single_inlen,
            single_outbuf.data(), &single_outlen,
            NULL, NULL,
            NULL, NULL, NULL,
            single_inputHyphens.data(), single_outputHyphens.data(),
            mode
        );
    }
    
    // 8. Test with empty input (should handle gracefully)
    if (fdp.remaining_bytes() > 4) {
        std::vector<widechar> empty_inbuf(1);
        int empty_inlen = 0;
        int empty_outlen = 16;
        std::vector<widechar> empty_outbuf(empty_outlen + 1);
        std::vector<char> empty_inputHyphens(1);
        std::vector<char> empty_outputHyphens(empty_outlen + 1);
        empty_inputHyphens[0] = '\0';
        
        int empty_result = lou_translatePrehyphenated(
            table, empty_inbuf.data(), &empty_inlen,
            empty_outbuf.data(), &empty_outlen,
            NULL, NULL,
            NULL, NULL, NULL,
            empty_inputHyphens.data(), empty_outputHyphens.data(),
            mode
        );
    }
    
    // 9. Clean up using lou_free
    // This frees all memory allocated by liblouis
    lou_free();
    
    return 0;
}

} // namespace harness_006
