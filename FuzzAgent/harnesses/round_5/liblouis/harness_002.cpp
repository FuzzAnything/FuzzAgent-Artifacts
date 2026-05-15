#include <fuzzer/FuzzedDataProvider.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <vector>
#include <string>
#include <liblouis.h>

namespace harness_002 {
/*
 * Fuzzing harness for liblouis hyphenation functionality
 * Targets: lou_hyphenate, lou_translatePrehyphenated, lou_charToDots, lou_dotsToChar
 * Required helpers: lou_getTable, lou_backTranslate (for braille mode), lou_free
 */



// Log callback to avoid excessive logging during fuzzing
void avoid_log(logLevels level, const char* msg) {
    (void)level;
    (void)msg;
}

// Global initialization flag
static int initialized = 0;

// Helper function to convert std::string to widechar array
static widechar* string_to_widechar(const std::string& str, size_t* out_len) {
    if (str.empty()) {
        *out_len = 0;
        return nullptr;
    }
    
    size_t len = str.length();
    widechar* wstr = (widechar*)malloc((len + 1) * sizeof(widechar));
    if (!wstr) {
        *out_len = 0;
        return nullptr;
    }
    
    // Simple conversion: assuming ASCII/UTF-8 single-byte chars
    for (size_t i = 0; i < len; i++) {
        wstr[i] = (widechar)(unsigned char)str[i];
    }
    wstr[len] = 0;
    *out_len = len;
    return wstr;
}

// Helper to check if table likely exists
static bool is_likely_valid_table(const std::string& table_name) {
    if (table_name.empty() || table_name.length() > 256) {
        return false;
    }
    
    // Check for common table extensions
    return table_name.find(".ctb") != std::string::npos || 
           table_name.find(".utb") != std::string::npos;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size check - we need enough for basic operations
    if (size < 64) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Initialize liblouis once
    if (!initialized) {
        lou_registerLogCallback(avoid_log);
        initialized = 1;
    }

    // Consume operation type to decide which hyphenation flow to test
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    // Consume mode parameter (0 = text, 1 = braille)
    int mode = fdp.ConsumeIntegral<uint8_t>() % 2;
    
    // Consume table name or list
    std::string table_name = fdp.ConsumeRandomLengthString(128);
    
    // If table doesn't look valid, use a known valid one
    if (!is_likely_valid_table(table_name)) {
        const char* default_tables[] = {
            "en-us-g2.ctb",
            "en-us-g1.ctb",
            "de-de-g2.ctb",
            "fr-bfu-g2.ctb"
        };
        uint8_t table_idx = fdp.ConsumeIntegral<uint8_t>() % 4;
        table_name = default_tables[table_idx];
    }
    
    // Consume input text (limited to reasonable size for hyphenation)
    size_t max_input_len = fdp.ConsumeIntegralInRange<size_t>(1, 100);
    std::string input_text = fdp.ConsumeRandomLengthString(max_input_len);
    
    if (input_text.empty()) {
        return 0;
    }
    
    // Convert input to widechar format
    size_t inlen;
    widechar* inbuf = string_to_widechar(input_text, &inlen);
    if (!inbuf || inlen == 0) {
        free(inbuf);
        return 0;
    }
    
    // Variables for hyphenation results
    char* hyphens = nullptr;
    int* actual_inlen = nullptr;
    widechar* outbuf = nullptr;
    int outlen = 0;
    char* inputHyphens = nullptr;
    char* outputHyphens = nullptr;
    
    // Track success/failure
    int success = 0;
    
    // Execute the selected hyphenation operation
    switch (operation) {
        case 0: {
            // Test lou_hyphenate - generate hyphenation patterns
            hyphens = (char*)malloc((inlen + 1) * sizeof(char));
            if (!hyphens) break;
            
            success = lou_hyphenate(table_name.c_str(), inbuf, (int)inlen, hyphens, mode);
            
            if (success) {
                // Verify hyphens array contains valid values (0, 1, or 2)
                for (size_t i = 0; i < inlen; i++) {
                    char h = hyphens[i];
                    assert(h == '0' || h == '1' || h == '2' || h == 0);
                }
            }
            break;
        }
        
        case 1: {
            // Test lou_hyphenate followed by lou_translatePrehyphenated
            // First generate hyphenation patterns
            hyphens = (char*)malloc((inlen + 1) * sizeof(char));
            if (!hyphens) break;
            
            int hyphen_result = lou_hyphenate(table_name.c_str(), inbuf, (int)inlen, hyphens, mode);
            if (!hyphen_result) break;
            
            // Now prepare for translation with hyphenation info
            actual_inlen = (int*)malloc(sizeof(int));
            if (!actual_inlen) break;
            *actual_inlen = (int)inlen;
            
            // Allocate output buffer (typically larger than input)
            outlen = (int)inlen * 4;  // Conservative estimate
            outbuf = (widechar*)malloc((outlen + 1) * sizeof(widechar));
            if (!outbuf) break;
            
            // Allocate hyphen arrays for translation
            inputHyphens = (char*)malloc((inlen + 1) * sizeof(char));
            outputHyphens = (char*)malloc((outlen + 1) * sizeof(char));
            if (!inputHyphens || !outputHyphens) break;
            
            // Copy hyphens to inputHyphens
            memcpy(inputHyphens, hyphens, inlen);
            inputHyphens[inlen] = 0;
            
            success = lou_translatePrehyphenated(table_name.c_str(), inbuf, actual_inlen,
                                                outbuf, &outlen, NULL, NULL, NULL, NULL, NULL,
                                                inputHyphens, outputHyphens, mode);
            break;
        }
        
        case 2: {
            // Test lou_charToDots - convert characters to braille dots
            // Note: inbuf for charToDots should be text characters
            outlen = (int)inlen;
            outbuf = (widechar*)malloc((outlen + 1) * sizeof(widechar));
            if (!outbuf) break;
            
            success = lou_charToDots(table_name.c_str(), inbuf, outbuf, (int)inlen, mode);
            break;
        }
        
        case 3: {
            // Test lou_dotsToChar - convert braille dots to characters
            // Note: inbuf for dotsToChar should be braille dot patterns
            outlen = (int)inlen;
            outbuf = (widechar*)malloc((outlen + 1) * sizeof(widechar));
            if (!outbuf) break;
            
            success = lou_dotsToChar(table_name.c_str(), inbuf, outbuf, (int)inlen, mode);
            break;
        }
    }
    
    // Cleanup
    free(inbuf);
    free(hyphens);
    free(actual_inlen);
    free(outbuf);
    free(inputHyphens);
    free(outputHyphens);
    
    // Note: lou_free() is called automatically via __attribute__((destructor))
    // in harness_000.cpp, so we don't need to call it here
    
    return 0;
}

} // namespace harness_002
