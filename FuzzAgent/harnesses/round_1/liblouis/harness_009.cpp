#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <string>
#include <vector>
#include <algorithm>
#include <fuzzer/FuzzedDataProvider.h>
#include "liblouis/liblouis.h"

namespace harness_009 {
/*
 * liblouis Table Chaining and Comprehensive Mode Combinations Fuzzing Harness
 * 
 * This harness specifically targets table chaining (multiple tables in comma-separated tableList)
 * and exhaustive testing of mode flag combinations. Focuses on workflows where multiple tables
 * are specified and their interaction, and all possible mode bit combinations.
 * 
 * APIs targeted:
 * 1. lou_translateString: Forward translation with table chaining and mode combinations
 * 2. lou_translate: Translation with cursor position tracking and table chaining
 * 3. lou_backTranslateString: Back translation with table chaining and mode combinations
 * 4. lou_backTranslate: Back translation with cursor tracking and table chaining
 * 5. lou_hyphenate: Hyphenation with table chaining
 * 6. lou_dotsToChar/lou_charToDots: Dot pattern conversion with table chaining
 * 
 * Specific focus areas:
 * - Testing translation with multiple tables in tableList (e.g., "en-us-g1.ctb,en-us-g2.ctb")
 * - Testing all mode flag combinations (computer_braille, no_contract, no_translate, emphasis combinations)
 * - Testing translation with typeform arrays using all emphasis types
 * - Testing back translation with multiple tables and mode combinations
 * - Testing hyphenation with table chaining
 * - Testing dot pattern conversion with table chaining
 * 
 * Semantic differentiation from existing harnesses:
 * - harness_000.cpp: Basic translation with single temporary tables
 * - harness_001.cpp: Comprehensive translation with single real tables
 * - harness_004.cpp: Basic mode testing with single tables, limited combinations
 * - harness_005.cpp: Dot conversion with single tables
 * - harness_006.cpp: Emphasis classes with single tables
 * - harness_008.cpp: Pre-hyphenated with single tables
 * - This harness: Focuses EXCLUSIVELY on table chaining and exhaustive mode combination testing
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

// Available table files for testing (use full paths for reliability)
static const char* available_tables[] = {
    "/root/src/liblouis/tables/en-us-g2.ctb",
    "/root/src/liblouis/tables/en-us-g1.ctb", 
    "/root/src/liblouis/tables/en-us-comp8.ctb",
    "/root/src/liblouis/tables/afr-za-g1.ctb",
    "/root/src/liblouis/tables/afr-za-g2.ctb",
    "/root/src/liblouis/tables/da-dk-g16.ctb",
    "/root/src/liblouis/tables/da-dk-g18.ctb",
    "/root/src/liblouis/tables/da-dk-g26.ctb",
    "/root/src/liblouis/tables/da-dk-g28.ctb",
    "/root/src/liblouis/tables/Es-Es-G0.utb",
    "/root/src/liblouis/tables/IPA.utb",
    "/root/src/liblouis/tables/Lv-Lv-g1.utb",
    "/root/src/liblouis/tables/Pl-Pl-g1.utb"
};
static const size_t num_tables = sizeof(available_tables) / sizeof(available_tables[0]);

// All emphasis typeform values for comprehensive testing
static const formtype all_emphasis_values[] = {
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
    emph_1 | emph_2,          // italic + underline
    emph_1 | emph_3,          // italic + bold
    emph_2 | emph_3,          // underline + bold
    emph_1 | emph_2 | emph_3  // italic + underline + bold
};
static const size_t num_emphasis_values = sizeof(all_emphasis_values) / sizeof(all_emphasis_values[0]);

// All mode flags for comprehensive testing (from typeforms enum)
static const int typeform_mode_flags[] = {
    0,  // plain text
    computer_braille,
    no_translate,
    no_contract,
    computer_braille | no_translate,
    computer_braille | no_contract,
    no_translate | no_contract,
    computer_braille | no_translate | no_contract
};
static const size_t num_typeform_modes = sizeof(typeform_mode_flags) / sizeof(typeform_mode_flags[0]);

// All translation mode flags (from translationModes enum)
static const int translation_mode_flags[] = {
    0,
    noContractions,
    compbrlAtCursor,
    dotsIO,
    compbrlLeftCursor,
    ucBrl,
    noUndefined,
    partialTrans,
    noContractions | compbrlAtCursor,
    noContractions | dotsIO,
    noContractions | compbrlLeftCursor,
    noContractions | ucBrl,
    noContractions | noUndefined,
    noContractions | partialTrans,
    compbrlAtCursor | dotsIO | compbrlLeftCursor | ucBrl | noUndefined | partialTrans  // combined
};
static const size_t num_translation_modes = sizeof(translation_mode_flags) / sizeof(translation_mode_flags[0]);

// Build a table list with multiple tables (2-4 tables)
static std::string build_table_list(FuzzedDataProvider& fdp) {
    std::string table_list;
    
    // Determine how many tables to chain (2-4 tables)
    size_t num_tables_to_chain = fdp.ConsumeIntegralInRange<size_t>(2, 4);
    
    // Select unique tables for the chain
    std::vector<size_t> selected_indices;
    for (size_t i = 0; i < num_tables_to_chain; i++) {
        size_t table_idx;
        bool unique;
        
        // Ensure we get unique tables (retry up to 10 times)
        int attempts = 0;
        do {
            table_idx = fdp.ConsumeIntegralInRange<size_t>(0, num_tables - 1);
            unique = true;
            for (size_t existing : selected_indices) {
                if (existing == table_idx) {
                    unique = false;
                    break;
                }
            }
            attempts++;
        } while (!unique && attempts < 10);
        
        selected_indices.push_back(table_idx);
        
        // Add to table list
        if (!table_list.empty()) {
            table_list += ",";
        }
        table_list += available_tables[table_idx];
    }
    
    return table_list;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Initialize logging once
    static int initialized = 0;
    if (!initialized) {
        lou_registerLogCallback(fuzz_log_callback);
        initialized = 1;
    }
    
    // Need minimum input for meaningful testing
    if (size < 32) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Determine which test scenario to run
    uint8_t test_scenario = fdp.ConsumeIntegral<uint8_t>() % 6;
    
    // Build table list with multiple tables
    std::string table_list = build_table_list(fdp);
    if (table_list.empty()) {
        return 0;
    }
    
    // Get remaining input for text
    size_t remaining_bytes = fdp.remaining_bytes();
    if (remaining_bytes < 16) {
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
    
    switch (test_scenario) {
        case 0: {
            // Test 1: Forward translation with table chaining and typeform mode combinations
            
            // Select typeform mode combination
            uint8_t mode_idx = fdp.ConsumeIntegralInRange<uint8_t>(0, num_typeform_modes - 1);
            int mode = typeform_mode_flags[mode_idx];
            
            // Select emphasis typeform
            uint8_t emphasis_idx = fdp.ConsumeIntegralInRange<uint8_t>(0, num_emphasis_values - 1);
            formtype selected_emphasis = all_emphasis_values[emphasis_idx];
            
            // Create typeform array
            formtype typeform_array[1024];
            for (int i = 0; i < inlen; i++) {
                typeform_array[i] = selected_emphasis;
            }
            
            // Test lou_translateString with table chaining
            int inlen_copy = inlen;
            int outlen_copy = outlen;
            lou_translateString(table_list.c_str(), inbuf, &inlen_copy, outbuf, &outlen_copy, 
                               typeform_array, NULL, mode);
            
            break;
        }
        
        case 1: {
            // Test 2: Translation with cursor tracking, table chaining, and translation mode combinations
            
            // Select translation mode combination
            uint8_t trans_mode_idx = fdp.ConsumeIntegralInRange<uint8_t>(0, num_translation_modes - 1);
            int mode = translation_mode_flags[trans_mode_idx];
            
            // Also add typeform mode flags with 50% probability
            if (fdp.ConsumeBool()) {
                uint8_t typeform_mode_idx = fdp.ConsumeIntegralInRange<uint8_t>(0, num_typeform_modes - 1);
                mode |= typeform_mode_flags[typeform_mode_idx];
            }
            
            // Prepare position tracking arrays
            int outputPos[1024];
            int inputPos[2048];
            int cursorPos = 0;
            char spacing[1024];
            
            // Select emphasis typeform
            uint8_t emphasis_idx = fdp.ConsumeIntegralInRange<uint8_t>(0, num_emphasis_values - 1);
            formtype selected_emphasis = all_emphasis_values[emphasis_idx];
            
            // Create typeform array
            formtype typeform_array[1024];
            for (int i = 0; i < inlen; i++) {
                typeform_array[i] = selected_emphasis;
            }
            
            // Test lou_translate with table chaining
            int inlen_copy = inlen;
            int outlen_copy = outlen;
            lou_translate(table_list.c_str(), inbuf, &inlen_copy, outbuf, &outlen_copy,
                         typeform_array, spacing, outputPos, inputPos, &cursorPos, mode);
            
            break;
        }
        
        case 2: {
            // Test 3: Back translation with table chaining and mode combinations
            
            // First do forward translation to get braille output
            uint8_t mode_idx = fdp.ConsumeIntegralInRange<uint8_t>(0, num_typeform_modes - 1);
            int mode = typeform_mode_flags[mode_idx];
            
            int inlen_copy = inlen;
            int outlen_copy = outlen;
            lou_translateString(table_list.c_str(), inbuf, &inlen_copy, outbuf, &outlen_copy, 
                               NULL, NULL, mode);
            
            // Now test back translation with the same table chaining
            if (outlen_copy > 0) {
                widechar back_inbuf[2048];
                widechar back_outbuf[1024];
                int back_inlen = outlen_copy;
                int back_outlen = sizeof(back_outbuf) / sizeof(back_outbuf[0]);
                
                // Copy output to input for back translation
                for (int i = 0; i < back_inlen && i < 2048; i++) {
                    back_inbuf[i] = outbuf[i];
                }
                
                // Test lou_backTranslateString with table chaining
                lou_backTranslateString(table_list.c_str(), back_inbuf, &back_inlen, back_outbuf, &back_outlen, 
                                       NULL, NULL, mode);
                
                // Test lou_backTranslate with cursor tracking if we have enough input
                if (fdp.remaining_bytes() > 4) {
                    int back_outputPos[2048];
                    int back_inputPos[1024];
                    int back_cursorPos = 0;
                    char back_spacing[2048];
                    
                    back_inlen = outlen_copy;
                    back_outlen = sizeof(back_outbuf) / sizeof(back_outbuf[0]);
                    
                    // Reset back buffer
                    for (int i = 0; i < back_inlen && i < 2048; i++) {
                        back_inbuf[i] = outbuf[i];
                    }
                    
                    lou_backTranslate(table_list.c_str(), back_inbuf, &back_inlen, back_outbuf, &back_outlen,
                                     NULL, back_spacing, back_outputPos, back_inputPos, &back_cursorPos, mode);
                }
            }
            
            break;
        }
        
        case 3: {
            // Test 4: Hyphenation with table chaining
            
            // Select mode
            uint8_t mode_idx = fdp.ConsumeIntegralInRange<uint8_t>(0, num_typeform_modes - 1);
            int mode = typeform_mode_flags[mode_idx];
            
            // Prepare hyphen array
            char hyphens[1024];
            
            // Test lou_hyphenate with table chaining
            lou_hyphenate(table_list.c_str(), inbuf, inlen, hyphens, mode);
            
            break;
        }
        
        case 4: {
            // Test 5: Dot pattern conversion with table chaining
            
            // Select mode for dot conversion
            uint8_t mode_idx = fdp.ConsumeIntegralInRange<uint8_t>(0, num_typeform_modes - 1);
            int mode = typeform_mode_flags[mode_idx];
            
            // Test lou_charToDots with table chaining
            widechar dots_outbuf[2048];
            int conv_length = std::min(inlen, 512);
            lou_charToDots(table_list.c_str(), inbuf, dots_outbuf, conv_length, mode);
            
            // If we have remaining input, test lou_dotsToChar
            if (fdp.remaining_bytes() > 4 && conv_length > 0) {
                widechar chars_outbuf[1024];
                lou_dotsToChar(table_list.c_str(), dots_outbuf, chars_outbuf, conv_length, mode);
            }
            
            break;
        }
        
        case 5: {
            // Test 6: Exhaustive mode combination testing with table chaining
            
            // Test ALL possible mode combinations in a single run
            // This tests edge cases where multiple mode bits are set
            
            // Get random mode bits for comprehensive testing
            int mode = 0;
            
            // Add typeform mode flags
            uint8_t typeform_bits = fdp.ConsumeIntegral<uint8_t>();
            if (typeform_bits & 0x01) mode |= computer_braille;
            if (typeform_bits & 0x02) mode |= no_translate;
            if (typeform_bits & 0x04) mode |= no_contract;
            
            // Add emphasis flags
            uint8_t emphasis_bits = fdp.ConsumeIntegral<uint8_t>();
            if (emphasis_bits & 0x01) mode |= emph_1;
            if (emphasis_bits & 0x02) mode |= emph_2;
            if (emphasis_bits & 0x04) mode |= emph_3;
            if (emphasis_bits & 0x08) mode |= emph_4;
            if (emphasis_bits & 0x10) mode |= emph_5;
            
            // Add translation mode flags
            uint8_t trans_bits = fdp.ConsumeIntegral<uint8_t>();
            if (trans_bits & 0x01) mode |= noContractions;
            if (trans_bits & 0x02) mode |= compbrlAtCursor;
            if (trans_bits & 0x04) mode |= dotsIO;
            if (trans_bits & 0x08) mode |= compbrlLeftCursor;
            if (trans_bits & 0x10) mode |= ucBrl;
            if (trans_bits & 0x20) mode |= noUndefined;
            if (trans_bits & 0x40) mode |= partialTrans;
            
            // Test all APIs with this comprehensive mode combination
            
            // 1. Forward translation
            int inlen_copy = inlen;
            int outlen_copy = outlen;
            lou_translateString(table_list.c_str(), inbuf, &inlen_copy, outbuf, &outlen_copy, 
                               NULL, NULL, mode);
            
            // 2. Back translation if we got output
            if (outlen_copy > 0 && fdp.remaining_bytes() > 4) {
                widechar back_inbuf[2048];
                widechar back_outbuf[1024];
                int back_inlen = outlen_copy;
                int back_outlen = sizeof(back_outbuf) / sizeof(back_outbuf[0]);
                
                for (int i = 0; i < back_inlen && i < 2048; i++) {
                    back_inbuf[i] = outbuf[i];
                }
                
                lou_backTranslateString(table_list.c_str(), back_inbuf, &back_inlen, back_outbuf, &back_outlen, 
                                       NULL, NULL, mode);
            }
            
            // 3. Hyphenation
            if (fdp.remaining_bytes() > 4) {
                char hyphens[1024];
                lou_hyphenate(table_list.c_str(), inbuf, inlen, hyphens, mode);
            }
            
            break;
        }
    }
    
    // Clean up
    lou_free();
    
    return 0;
}

} // namespace harness_009
