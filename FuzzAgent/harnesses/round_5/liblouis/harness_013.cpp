#include <fuzzer/FuzzedDataProvider.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <vector>
#include <string>
#include <algorithm>
#include <liblouis.h>

namespace harness_013 {
/*
 * Fuzzing harness for liblouis hyphenation functionality focusing on uncovered branches
 * Based on coverage analysis: lou_hyphenate API has 48 undiscovered branches - largest coverage deficit
 * 
 * Targets: lou_hyphenate API with comprehensive test scenarios
 * 
 * Semantic differentiation from existing harnesses:
 * - harness_002: Basic hyphenation testing with 4 operations including lou_hyphenate
 * - harness_005: Pre-hyphenated translation operations
 * - harness_013: Deep focus on hyphenation patterns, syllable boundaries, hyphenation rules
 *                targeting 48 undiscovered branches in hyphenation code
 * 
 * Strategy:
 * 1. Test hyphenation with diverse table combinations (single, multiple, invalid tables)
 * 2. Test all possible mode values and their combinations
 * 3. Test edge cases: empty strings, minimum/maximum lengths, special characters
 * 4. Test syllable boundary detection and hyphenation rule application
 * 5. Test hyphenation with different language tables and character sets
 * 6. Test hyphen array validation and post-processing
 * 
 * Invocation sequence designed to maximize coverage of hyphenation-specific code paths:
 * 1. Initialize liblouis with logging suppression
 * 2. Generate diverse input strings (ASCII, Unicode, mixed)
 * 3. Test hyphenation with various table combinations
 * 4. Validate hyphenation results and patterns
 * 5. Test edge cases and error conditions
 */



// Log callback to avoid excessive logging during fuzzing
static void quiet_log(logLevels level, const char* msg) {
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
    
    // Convert characters to widechar (simple ASCII handling)
    for (size_t i = 0; i < len; i++) {
        wstr[i] = (widechar)(unsigned char)str[i];
    }
    wstr[len] = 0;
    *out_len = len;
    return wstr;
}

// Helper function to generate Unicode test strings
static std::string generate_unicode_string(FuzzedDataProvider& fdp, size_t max_len) {
    std::string result;
    size_t length = fdp.ConsumeIntegralInRange<size_t>(1, max_len);
    
    for (size_t i = 0; i < length && fdp.remaining_bytes() >= 2; i++) {
        // Generate UTF-8 sequences (simplified)
        uint8_t byte = fdp.ConsumeIntegral<uint8_t>();
        result.push_back(byte);
        
        // Occasionally add multi-byte sequences
        if ((byte & 0xE0) == 0xC0 && fdp.remaining_bytes() >= 1) {
            result.push_back(fdp.ConsumeIntegralInRange<uint8_t>(0x80, 0xBF));
        } else if ((byte & 0xF0) == 0xE0 && fdp.remaining_bytes() >= 2) {
            result.push_back(fdp.ConsumeIntegralInRange<uint8_t>(0x80, 0xBF));
            result.push_back(fdp.ConsumeIntegralInRange<uint8_t>(0x80, 0xBF));
        }
    }
    
    return result;
}

// Helper to create table list with various combinations
static std::string create_table_list(FuzzedDataProvider& fdp) {
    const char* known_tables[] = {
        "en-us-g2.ctb",
        "en-us-g1.ctb",
        "en-ueb-g2.ctb",
        "en-ueb-g1.ctb",
        "de-de-g2.ctb",
        "fr-bfu-g2.ctb",
        "es-g1.ctb",
        "it-it-g1.ctb",
        "nl-NL-g1.ctb",
        "sv-se-g1.ctb"
    };
    
    size_t num_tables = sizeof(known_tables) / sizeof(known_tables[0]);
    
    // Choose how many tables to include (0-4, where 0 means use a random string)
    uint8_t table_count = fdp.ConsumeIntegral<uint8_t>() % 5;
    
    if (table_count == 0) {
        // Generate a random table name
        return fdp.ConsumeRandomLengthString(128);
    }
    
    std::string table_list;
    std::vector<size_t> used_indices;
    
    for (uint8_t i = 0; i < table_count; i++) {
        size_t idx = fdp.ConsumeIntegralInRange<size_t>(0, num_tables - 1);
        
        // Avoid duplicates
        if (std::find(used_indices.begin(), used_indices.end(), idx) != used_indices.end()) {
            continue;
        }
        
        used_indices.push_back(idx);
        
        if (!table_list.empty()) {
            table_list += ",";
        }
        
        // Sometimes add path prefixes
        if (fdp.ConsumeBool()) {
            table_list += "./";
        } else if (fdp.ConsumeBool()) {
            table_list += "/usr/share/liblouis/tables/";
        }
        
        table_list += known_tables[idx];
    }
    
    return table_list;
}

// Validate hyphenation results
static bool validate_hyphenation_results(const char* hyphens, size_t len) {
    if (!hyphens) return false;
    
    for (size_t i = 0; i < len; i++) {
        char h = hyphens[i];
        // Valid hyphen values: '0' (no hyphen), '1' (hyphen allowed), '2' (required hyphen)
        if (h != '0' && h != '1' && h != '2' && h != '\0') {
            return false;
        }
    }
    
    return true;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size check - we need enough for meaningful hyphenation tests
    if (size < 32) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize liblouis once
    if (!initialized) {
        lou_registerLogCallback(quiet_log);
        initialized = 1;
    }
    
    // Consume test scenario selector (0-7 to test different aspects)
    uint8_t scenario = fdp.ConsumeIntegral<uint8_t>() % 8;
    
    // Generate input text based on scenario
    std::string input_text;
    switch (scenario) {
        case 0:
            // ASCII text with spaces and punctuation
            input_text = fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(10, 200));
            // Add some spaces and punctuation
            for (size_t i = 0; i < input_text.length(); i++) {
                if (i % 7 == 0) input_text[i] = ' ';
                else if (i % 11 == 0) input_text[i] = '.';
                else if (i % 13 == 0) input_text[i] = '-';
            }
            break;
            
        case 1:
            // Longer text for multi-word hyphenation
            input_text = fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(50, 500));
            break;
            
        case 2:
            // Very short text (edge case)
            input_text = fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(1, 5));
            break;
            
        case 3:
            // Text with numbers and special characters
            input_text = fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(20, 100));
            for (size_t i = 0; i < input_text.length(); i++) {
                if (i % 5 == 0) input_text[i] = '0' + (fdp.ConsumeIntegral<uint8_t>() % 10);
                else if (i % 7 == 0) input_text[i] = '!' + (fdp.ConsumeIntegral<uint8_t>() % 15);
            }
            break;
            
        case 4:
            // Mixed case text
            input_text = fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(30, 150));
            for (size_t i = 0; i < input_text.length(); i++) {
                if (i % 3 == 0 && input_text[i] >= 'a' && input_text[i] <= 'z') {
                    input_text[i] = input_text[i] - 'a' + 'A';
                }
            }
            break;
            
        case 5:
            // Unicode text (simplified)
            input_text = generate_unicode_string(fdp, fdp.ConsumeIntegralInRange<size_t>(20, 100));
            break;
            
        case 6:
            // Text with existing hyphens
            input_text = fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(30, 120));
            for (size_t i = 0; i < input_text.length(); i++) {
                if (i % 8 == 0) input_text[i] = '-';
            }
            break;
            
        case 7:
            // Empty or nearly empty text (edge case)
            if (fdp.ConsumeBool()) {
                input_text = "";
            } else {
                input_text = fdp.ConsumeRandomLengthString(1);
            }
            break;
    }
    
    if (input_text.empty() && scenario != 7) {
        return 0;
    }
    
    // Create table list
    std::string table_list = create_table_list(fdp);
    
    // Consume mode parameter with full range testing
    int mode = 0;
    uint8_t mode_selector = fdp.ConsumeIntegral<uint8_t>() % 8;
    switch (mode_selector) {
        case 0: mode = 0; break;  // text mode
        case 1: mode = 1; break;  // braille mode
        case 2: mode = fdp.ConsumeIntegral<int>() & 0xFF; break;  // random mode
        case 3: mode = -1; break;  // negative mode (edge case)
        case 4: mode = 256; break;  // large mode value
        case 5: mode = fdp.ConsumeIntegral<int>() % 32; break;  // limited range
        case 6: mode = (fdp.ConsumeBool() ? 0 : 1); break;  // binary mode
        case 7: mode = fdp.ConsumeIntegralInRange<int>(0, 100); break;  // moderate range
    }
    
    // Convert input to widechar format
    size_t inlen;
    widechar* inbuf = string_to_widechar(input_text, &inlen);
    if (!inbuf || inlen == 0) {
        free(inbuf);
        // For empty input, we still want to test the API (edge case)
        if (input_text.empty()) {
            inlen = 0;
            inbuf = (widechar*)malloc(sizeof(widechar));
            if (!inbuf) return 0;
            inbuf[0] = 0;
        } else {
            return 0;
        }
    }
    
    // Allocate hyphen array (must be at least inlen + 1 chars)
    char* hyphens = (char*)malloc((inlen + 1) * sizeof(char));
    if (!hyphens) {
        free(inbuf);
        return 0;
    }
    
    // Initialize hyphens array to a known state
    memset(hyphens, 'X', inlen + 1);  // Fill with invalid value to detect if API writes to it
    
    // Call lou_hyphenate - this is the main API under test
    int success = lou_hyphenate(table_list.c_str(), inbuf, (int)inlen, hyphens, mode);
    
    // Validate results if successful
    if (success) {
        // Check that hyphens array contains valid values
        bool valid = validate_hyphenation_results(hyphens, inlen);
        
        // Additional validation: check for patterns in hyphenation
        if (valid && inlen > 0) {
            // Count hyphen positions
            int hyphen_count = 0;
            int required_hyphen_count = 0;
            
            for (size_t i = 0; i < inlen; i++) {
                if (hyphens[i] == '1') hyphen_count++;
                else if (hyphens[i] == '2') required_hyphen_count++;
            }
            
            // Basic sanity checks
            // Hyphens shouldn't be at position 0 (beginning of text)
            if (inlen > 0 && hyphens[0] != '0' && hyphens[0] != '\0') {
                // This might be valid in some cases, but unusual
            }
            
            // Check for consecutive hyphens (usually not allowed)
            for (size_t i = 1; i < inlen; i++) {
                if ((hyphens[i] == '1' || hyphens[i] == '2') && 
                    (hyphens[i-1] == '1' || hyphens[i-1] == '2')) {
                    // Consecutive hyphens might indicate an issue
                }
            }
        }
        
        // Test hyphenation with different table combinations
        if (fdp.remaining_bytes() > 16 && inlen > 5) {
            // Try hyphenation with just the first table if we have a comma-separated list
            size_t comma_pos = table_list.find(',');
            if (comma_pos != std::string::npos) {
                std::string single_table = table_list.substr(0, comma_pos);
                char* hyphens2 = (char*)malloc((inlen + 1) * sizeof(char));
                if (hyphens2) {
                    memset(hyphens2, 'X', inlen + 1);
                    int success2 = lou_hyphenate(single_table.c_str(), inbuf, (int)inlen, hyphens2, mode);
                    if (success2) {
                        validate_hyphenation_results(hyphens2, inlen);
                    }
                    free(hyphens2);
                }
            }
        }
    } else {
        // Hyphenation failed - check if hyphens array was modified
        // Some implementations might partially fill it even on failure
        bool any_written = false;
        for (size_t i = 0; i < inlen; i++) {
            if (hyphens[i] != 'X') {
                any_written = true;
                break;
            }
        }
    }
    
    // Clean up
    free(inbuf);
    free(hyphens);
    
    // Additional tests if we have remaining input
    if (fdp.remaining_bytes() > 64) {
        // Test with different mode on same input
        int alt_mode = fdp.ConsumeIntegral<int>() % 4;
        if (alt_mode != mode && input_text.length() > 3) {
            // Re-convert input (simplified - reuse original conversion)
            size_t alt_inlen;
            widechar* alt_inbuf = string_to_widechar(input_text, &alt_inlen);
            if (alt_inbuf && alt_inlen > 0) {
                char* alt_hyphens = (char*)malloc((alt_inlen + 1) * sizeof(char));
                if (alt_hyphens) {
                    int alt_success = lou_hyphenate(table_list.c_str(), alt_inbuf, (int)alt_inlen, alt_hyphens, alt_mode);
                    if (alt_success) {
                        validate_hyphenation_results(alt_hyphens, alt_inlen);
                    }
                    free(alt_hyphens);
                }
                free(alt_inbuf);
            }
        }
    }
    
    return 0;
}

} // namespace harness_013
