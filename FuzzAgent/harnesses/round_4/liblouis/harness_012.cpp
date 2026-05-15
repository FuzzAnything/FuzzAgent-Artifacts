#include <fuzzer/FuzzedDataProvider.h>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>
#include <iostream>
#include "liblouis.h"

namespace harness_012 {
// liblouis translateString blocked branches fuzzing harness
// Targets translateString function's blocked branches (146 blocked)
// Focus on computer braille, context rules, numeric mode, and emphasis handling
// Based on coverage analysis guidance: translateString has 146 blocked branches (54% of undiscovered complexity)
// Specifically targets: computer braille translation, CTO_CompBrl opcode handling, 
// context rule processing, numeric mode transitions, and emphasis insertion logic


// Include liblouis public header
extern "C" {
}

// Log callback to suppress logs during fuzzing
static void avoid_log_callback(logLevels level, const char *msg) {
    (void)level;
    (void)msg;
}

// Typeform constants from liblouis.h.in
const formtype PLAIN_TEXT = 0x0000;
const formtype EMPH_1 = 0x0001;      // italic
const formtype EMPH_2 = 0x0002;      // underline  
const formtype EMPH_3 = 0x0004;      // bold
const formtype EMPH_4 = 0x0008;
const formtype EMPH_5 = 0x0010;
const formtype EMPH_6 = 0x0020;
const formtype EMPH_7 = 0x0040;
const formtype EMPH_8 = 0x0080;
const formtype EMPH_9 = 0x0100;
const formtype EMPH_10 = 0x0200;
const formtype COMPUTER_BRAILLE = 0x0400;
const formtype NO_TRANSLATE = 0x0800;
const formtype NO_CONTRACT = 0x1000;

// Mode flag constants observed in source code
// These are used to trigger specific code paths in translateString
const int COMPBRL_AT_CURSOR = 0x2000;    // compbrlAtCursor flag
const int COMPBRL_LEFT_CURSOR = 0x4000;  // compbrlLeftCursor flag
const int PASS_AT_CURSOR = 0x8000;       // passAtCursor flag

// Simple ASCII to widechar conversion
static size_t ascii_to_widechar(const char* ascii, widechar* wide, size_t max_len) {
    size_t i;
    for (i = 0; i < max_len && ascii[i] != '\0'; i++) {
        wide[i] = ascii[i];
    }
    return i;
}
// Generate input with computer braille markers (compbrlStart/compbrlEnd)
static std::string generate_input_with_markers(FuzzedDataProvider& fdp, bool include_compbrl) {
    std::string input;
    
    // Add some prefix text
    size_t prefix_len = fdp.ConsumeIntegralInRange<size_t>(0, 10);
    for (size_t i = 0; i < prefix_len; i++) {
        char c = fdp.ConsumeIntegralInRange<char>('a', 'z');
        input += c;
    }
    
    // Add computer braille section if requested
    if (include_compbrl) {
        // Add compbrlStart marker (could be specific character or pattern)
        // In practice, computer braille is triggered by typeform flag, not input characters
        // But we'll add some computer-braille-like content
        input += "comp";
        size_t comp_len = fdp.ConsumeIntegralInRange<size_t>(1, 20);
        for (size_t i = 0; i < comp_len; i++) {
            // Computer braille often includes symbols like @ # $ % etc.
            uint8_t char_type = fdp.ConsumeIntegral<uint8_t>() % 4;
            switch (char_type) {
                case 0: input += fdp.ConsumeIntegralInRange<char>('0', '9'); break;
                case 1: input += fdp.ConsumeIntegralInRange<char>('A', 'Z'); break;
                case 2: input += fdp.ConsumeIntegralInRange<char>('!', '/'); break; // symbols
                case 3: input += fdp.ConsumeIntegralInRange<char>('a', 'z'); break;
            }
        }
        input += "brl";
    }
    
    // Add context rule triggers (common patterns that might trigger context rules)
    // Context rules often look for specific patterns like "ing", "ed", etc.
    const char* context_patterns[] = {"ing", "ed", "tion", "ment", "ness", "able"};
    if (fdp.ConsumeBool()) {
        size_t pattern_idx = fdp.ConsumeIntegralInRange<size_t>(0, 5);
        input += context_patterns[pattern_idx];
    }
    
    // Add numeric mode characters
    if (fdp.ConsumeBool()) {
        input += "#"; // Number indicator
        size_t num_len = fdp.ConsumeIntegralInRange<size_t>(1, 10);
        for (size_t i = 0; i < num_len; i++) {
            input += fdp.ConsumeIntegralInRange<char>('0', '9');
        }
    }
    
    // Add suffix
    size_t suffix_len = fdp.ConsumeIntegralInRange<size_t>(0, 10);
    for (size_t i = 0; i < suffix_len; i++) {
        input += fdp.ConsumeIntegralInRange<char>('a', 'z');
    }
    
    return input;
}

// Generate typeform array matching input length
static std::vector<formtype> generate_typeform_array(size_t length, FuzzedDataProvider& fdp) {
    std::vector<formtype> typeforms(length);
    
    for (size_t i = 0; i < length; i++) {
        formtype typeform = PLAIN_TEXT;
        
        // Randomly add emphasis types
        if (fdp.ConsumeBool()) typeform |= EMPH_1;  // italic
        if (fdp.ConsumeBool()) typeform |= EMPH_2;  // underline
        if (fdp.ConsumeBool()) typeform |= EMPH_3;  // bold
        
        // Add computer braille typeform for some sections
        if (fdp.ConsumeBool()) typeform |= COMPUTER_BRAILLE;
        
        // Add no-translate sections (deprecated but still in code)
        if (fdp.ConsumeBool() && fdp.ConsumeProbability<float>() < 0.1) {
            typeform |= NO_TRANSLATE;
        }
        
        // Add no-contract sections
        if (fdp.ConsumeBool()) typeform |= NO_CONTRACT;
        
        typeforms[i] = typeform;
    }
    
    return typeforms;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough for configuration
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
    
    // Select translation tables that support computer braille, context rules, numeric mode
    const char* tables[] = {
        "en-us-comp8.ctb",    // Computer braille table (8-dot) - supports CTO_CompBrl
        "en-us-comp6.ctb",    // Computer braille table (6-dot)
        "en-ueb-g2.ctb",      // UEB Grade 2 with various features including context rules
        "en-us-g1.ctb",       // Grade 1 braille
        "da-dk-g16.ctb",      // Danish 16-dot computer braille
        "zhcn-g2.ctb"         // Chinese grade 2
    };
    const size_t num_tables = sizeof(tables) / sizeof(tables[0]);
    
    // Perform multiple iterations with different tables and input patterns
    // to exercise various blocked branches in translateString
    size_t iterations = fdp.ConsumeIntegralInRange<size_t>(1, 3);
    
    for (size_t iter = 0; iter < iterations && fdp.remaining_bytes() > 16; iter++) {
        // 1. Call lou_getTable to load a translation table
        size_t table_idx = fdp.ConsumeIntegralInRange<size_t>(0, num_tables - 1);
        const char* table = tables[table_idx];
        
        // Load the table - this exercises table loading and compilation
        const void* loaded_table = lou_getTable(table);
        // Note: lou_getTable returns NULL if table has errors, but we continue anyway
        
        // Generate input text with various patterns targeting blocked branches
        bool include_compbrl = fdp.ConsumeBool();
        std::string text = generate_input_with_markers(fdp, include_compbrl);
        
        if (text.empty() && fdp.remaining_bytes() > 0) {
            // Generate minimal input if empty
            text = "test";
        }
        
        if (text.empty()) {
            continue;
        }
        
        // Convert to widechar
        std::vector<widechar> inbuf(text.size() + 1);
        size_t inlen = ascii_to_widechar(text.c_str(), inbuf.data(), text.size());
        int inlen_int = static_cast<int>(inlen);
        
        // Allocate output buffer - braille can expand significantly
        int outlen_int = inlen_int * 16;
        std::vector<widechar> outbuf(outlen_int + 1);
        
        // Generate typeform array for emphasis and special typeforms
        // This targets emphasis insertion logic and no-translate typeform sections
        std::vector<formtype> typeform_array = generate_typeform_array(inlen, fdp);
        formtype* typeform_ptr = typeform_array.empty() ? NULL : typeform_array.data();
        
        // Generate spacing array (optional)
        std::vector<char> spacing_array(inlen);
        for (int i = 0; i < inlen_int; i++) {
            spacing_array[i] = fdp.ConsumeIntegralInRange<char>(0, 1);
        }
        char* spacing_ptr = spacing_array.empty() ? NULL : spacing_array.data();
        
        // Select mode based on fuzzed input
        // Use specific flags to target computer braille and cursor-related code paths
        int mode = 0;
        uint8_t mode_bits = fdp.ConsumeIntegral<uint8_t>();
        
        // Set various mode flags
        if (mode_bits & 0x01) mode |= COMPUTER_BRAILLE;
        if (mode_bits & 0x02) mode |= NO_TRANSLATE;
        if (mode_bits & 0x04) mode |= NO_CONTRACT;
        
        // Set cursor-related flags to target compbrlAtCursor, compbrlLeftCursor paths
        if (fdp.ConsumeBool()) mode |= COMPBRL_AT_CURSOR;
        if (fdp.ConsumeBool()) mode |= COMPBRL_LEFT_CURSOR;
        if (fdp.ConsumeBool()) mode |= PASS_AT_CURSOR;
        
        // 2. Call lou_translateString with input containing computer braille indicators,
        // context rule triggers, numeric mode characters, emphasis markers, etc.
        // This exercises the blocked branches in translateString
        int result = lou_translateString(
            table,                    // tableList
            inbuf.data(),            // inbuf
            &inlen_int,              // inlen  
            outbuf.data(),           // outbuf
            &outlen_int,             // outlen
            typeform_ptr,            // typeform array (emphasis markers)
            spacing_ptr,             // spacing array
            mode                     // mode with computer braille flags
        );
        
        // Also test lou_translate (alternative entry point with position tracking)
        if (result && fdp.ConsumeBool() && inlen_int > 0 && fdp.remaining_bytes() > 8) {
            std::vector<int> outputPos(inlen_int);
            std::vector<int> inputPos(outlen_int);
            int cursorPos = fdp.ConsumeIntegralInRange<int>(-1, inlen_int);
            
            int alt_result = lou_translate(
                table,                    // tableList
                inbuf.data(),            // inbuf
                &inlen_int,              // inlen  
                outbuf.data(),           // outbuf
                &outlen_int,             // outlen
                typeform_ptr,            // typeform array
                spacing_ptr,             // spacing array
                outputPos.data(),        // outputPos
                inputPos.data(),         // inputPos
                &cursorPos,              // cursorPos
                mode                     // mode
            );
            
            // Test lou_translatePrehyphenated if we have hyphen arrays
            if (alt_result && fdp.ConsumeBool() && fdp.remaining_bytes() > inlen_int) {
                std::vector<char> inputHyphens(inlen_int);
                std::vector<char> outputHyphens(outlen_int);
                for (int i = 0; i < inlen_int; i++) {
                    inputHyphens[i] = fdp.ConsumeIntegralInRange<char>(0, 1);
                }
                
                int prehyphen_result = lou_translatePrehyphenated(
                    table,                    // tableList
                    inbuf.data(),            // inbuf
                    &inlen_int,              // inlen  
                    outbuf.data(),           // outbuf
                    &outlen_int,             // outlen
                    typeform_ptr,            // typeform array
                    spacing_ptr,             // spacing array
                    outputPos.data(),        // outputPos
                    inputPos.data(),         // inputPos
                    &cursorPos,              // cursorPos
                    inputHyphens.data(),     // inputHyphens
                    outputHyphens.data(),    // outputHyphens
                    mode                     // mode
                );
            }
        }
        
        // Test with table combinations (comma-separated table lists)
        if (fdp.ConsumeBool() && num_tables > 1 && fdp.remaining_bytes() > 8) {
            // Create table list with multiple tables
            std::string table_list = tables[0];
            size_t num_combined = fdp.ConsumeIntegralInRange<size_t>(1, 3);
            for (size_t i = 1; i < num_combined && i < num_tables; i++) {
                table_list += ",";
                table_list += tables[i];
            }
            
            // Load the combined table list
            const void* combined_table = lou_getTable(table_list.c_str());
            
            // Reset lengths for new translation
            int new_inlen = static_cast<int>(inlen);
            int new_outlen = inlen_int * 16;
            std::vector<widechar> new_outbuf(new_outlen + 1);
            
            lou_translateString(
                table_list.c_str(),    // tableList with multiple tables
                inbuf.data(),          // inbuf
                &new_inlen,            // inlen  
                new_outbuf.data(),     // outbuf
                &new_outlen,           // outlen
                typeform_ptr,          // typeform array
                spacing_ptr,           // spacing array
                mode                   // mode
            );
        }
        
        // 3. Call lou_free to release the table(s)
        // This cleans up internal table cache and exercises cleanup paths
        lou_free();
    }
    
    return 0;
}

} // namespace harness_012
