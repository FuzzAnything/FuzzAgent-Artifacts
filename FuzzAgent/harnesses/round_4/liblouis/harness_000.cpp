#include <fuzzer/FuzzedDataProvider.h>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>
#include <iostream>
#include "liblouis.h"

namespace harness_000 {
// liblouis braille translation fuzzing harness
// Targets core translation APIs: lou_translateString, lou_translate, lou_backTranslateString
// Based on existing fuzz_translate.c but using FuzzedDataProvider


// Include liblouis public header
extern "C" {
}

// Log callback to suppress logs during fuzzing
static void avoid_log_callback(logLevels level, const char *msg) {
    (void)level;
    (void)msg;
}

// Mode constants - extracted from liblouis usage patterns
// These are based on observed usage in the source code
const int MODE_PLAIN = 0;
const int MODE_COMPUTER_BRAILLE = 0x0400;  // computer_braille from typeforms enum
const int MODE_NO_TRANSLATE = 0x0800;      // no_translate from typeforms enum
const int MODE_NO_CONTRACT = 0x1000;       // no_contract from typeforms enum

// Simple ASCII to widechar conversion
static size_t ascii_to_widechar(const char* ascii, widechar* wide, size_t max_len) {
    size_t i;
    for (i = 0; i < max_len && ascii[i] != '\0'; i++) {
        wide[i] = ascii[i];
    }
    return i;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check
    if (size < 16) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize liblouis once
    static bool initialized = false;
    if (!initialized) {
        lou_registerLogCallback(avoid_log_callback);
        initialized = true;
    }
    
    // Select a table - use a few common tables
    const char* tables[] = {
        "en-us-comp8.ctb",
        "en-ueb-g2.ctb", 
        "en-us-comp6.ctb"
    };
    const size_t num_tables = sizeof(tables) / sizeof(tables[0]);
    size_t table_idx = fdp.ConsumeIntegralInRange<size_t>(0, num_tables - 1);
    const char* table = tables[table_idx];
    
    // Check if table is valid (this might fail if tables aren't in expected location)
    // We'll continue anyway as lou_translateString handles invalid tables
    
    // Consume text to translate
    size_t max_text_len = fdp.ConsumeIntegralInRange<size_t>(1, 256);
    std::string text = fdp.ConsumeRandomLengthString(max_text_len);
    
    if (text.empty()) {
        return 0;
    }
    
    // Convert to widechar
    std::vector<widechar> inbuf(text.size() + 1);
    size_t inlen = ascii_to_widechar(text.c_str(), inbuf.data(), text.size());
    int inlen_int = static_cast<int>(inlen);
    
    // Allocate output buffer - braille can expand, so allocate more space
    int outlen_int = inlen_int * 16;
    std::vector<widechar> outbuf(outlen_int + 1);
    
    // Select mode based on fuzzed input
    int mode = MODE_PLAIN;
    uint8_t mode_choice = fdp.ConsumeIntegral<uint8_t>() % 4;
    switch (mode_choice) {
        case 0:
            mode = MODE_PLAIN;
            break;
        case 1:
            mode = MODE_COMPUTER_BRAILLE;
            break;
        case 2:
            mode = MODE_NO_TRANSLATE;
            break;
        case 3:
            mode = MODE_NO_CONTRACT;
            break;
    }
    
    // Select which API to test
    uint8_t api_choice = fdp.ConsumeIntegral<uint8_t>() % 3;
    int result = 0;
    
    switch (api_choice) {
        case 0: {
            // Test lou_translateString
            int actual_inlen = inlen_int;
            int actual_outlen = outlen_int;
            result = lou_translateString(table, inbuf.data(), &actual_inlen, 
                                        outbuf.data(), &actual_outlen, 
                                        NULL, NULL, mode);
            break;
        }
            
        case 1: {
            // Test lou_translate with position tracking
            int actual_inlen = inlen_int;
            int actual_outlen = outlen_int;
            std::vector<int> outputPos(inlen_int);
            std::vector<int> inputPos(outlen_int);
            int cursorPos = -1;
            
            result = lou_translate(table, inbuf.data(), &actual_inlen,
                                  outbuf.data(), &actual_outlen,
                                  NULL, NULL,
                                  outputPos.data(), inputPos.data(), &cursorPos,
                                  mode);
            break;
        }
            
        case 2: {
            // Test lou_backTranslateString
            int actual_inlen = inlen_int;
            int actual_outlen = outlen_int;
            result = lou_backTranslateString(table, inbuf.data(), &actual_inlen,
                                            outbuf.data(), &actual_outlen,
                                            NULL, NULL, mode);
            break;
        }
    }
    
    // Ignore result - we're testing for crashes
    
    // If we have remaining bytes, test charToDots and dotsToChar
    if (fdp.remaining_bytes() > 0) {
        size_t extra_len = fdp.ConsumeIntegralInRange<size_t>(1, 100);
        if (extra_len > fdp.remaining_bytes()) {
            extra_len = fdp.remaining_bytes();
        }
        
        std::string extra_text = fdp.ConsumeBytesAsString(extra_len);
        if (!extra_text.empty()) {
            std::vector<widechar> extra_inbuf(extra_text.size() + 1);
            std::vector<widechar> extra_outbuf(extra_text.size() + 1);
            
            size_t extra_conv_len = ascii_to_widechar(extra_text.c_str(), 
                                                     extra_inbuf.data(), 
                                                     extra_text.size());
            int extra_len_int = static_cast<int>(extra_conv_len);
            
            // Test charToDots
            lou_charToDots(table, extra_inbuf.data(), extra_outbuf.data(), 
                          extra_len_int, mode);
            
            // Test dotsToChar (using output as input)
            lou_dotsToChar(table, extra_outbuf.data(), extra_inbuf.data(), 
                          extra_len_int, mode);
        }
    }
    
    return 0;
}

} // namespace harness_000
