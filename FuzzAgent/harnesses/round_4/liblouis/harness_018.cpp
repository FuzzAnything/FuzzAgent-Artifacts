#include <fuzzer/FuzzedDataProvider.h>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>
#include <algorithm>
#include "liblouis.h"

namespace harness_018 {
// liblouis hyphenation-specific compilation harness
// Target: compileRule function with hyphenation paths (580 blocked branches) and 
//         lou_hyphenate API (80% branch coverage gap)
// Focus: Hyphenation-specific compilation paths (ISO/UTF-8 encoding lines that trigger 
//        compileHyphenation) and hyphenation API usage patterns
// Primary API chain: lou_compileString -> compileString -> compileRule -> compileHyphenation
//                   lou_hyphenate -> lou_translatePrehyphenated
// Based on coverage analysis guidance to target hyphenation functionality
// Semantic differentiation from harness_002: Deep focus on hyphenation table compilation
//   patterns (ISO/UTF-8 encoding specifiers) rather than just using lou_hyphenate API
// Semantic differentiation from harness_017: Focuses on hyphenation compilation paths
//   within compileRule rather than grouping rules


// Include liblouis public header
extern "C" {
}

// Log callback to suppress logs during fuzzing
static void avoid_log_callback(logLevels level, const char *msg) {
    (void)level;
    (void)msg;
    // Suppress all logs during fuzzing
}

// Helper to generate hyphenation pattern lines for compilation
// Hyphenation patterns follow the format: letter pattern numbers mixed
// Example: "a1b2c3" or "m2e1c2h1a2n2i1c2a2l1"
static std::string generate_hyphenation_pattern(FuzzedDataProvider& fdp) {
    std::string pattern;
    
    // Pattern length (characters + digits)
    size_t pattern_len = fdp.ConsumeIntegralInRange<size_t>(2, 32);
    
    for (size_t i = 0; i < pattern_len; i++) {
        // Alternate between letters and digits
        if (i % 2 == 0) {
            // Letter
            pattern += fdp.ConsumeIntegralInRange<char>('a', 'z');
        } else {
            // Digit (0-9 for hyphenation patterns)
            pattern += fdp.ConsumeIntegralInRange<char>('0', '9');
        }
    }
    
    return pattern;
}

// Helper to generate hyphenation exception (single word with hyphens)
static std::string generate_hyphenation_exception(FuzzedDataProvider& fdp) {
    std::string exception;
    
    // Word length
    size_t word_len = fdp.ConsumeIntegralInRange<size_t>(3, 20);
    
    for (size_t i = 0; i < word_len; i++) {
        exception += fdp.ConsumeIntegralInRange<char>('a', 'z');
        
        // Occasionally add hyphen for exception format
        if (i < word_len - 1 && fdp.ConsumeBool()) {
            exception += '-';
        }
    }
    
    return exception;
}

// Helper to generate hyphenation-specific table content
// This targets compileRule paths that handle ISO/UTF-8 encoding lines
// which trigger compileHyphenation function
static std::string generate_hyphenation_table(FuzzedDataProvider& fdp) {
    std::string content;
    
    // Start with encoding specification that triggers hyphenation compilation
    // From compileTranslationTable.c line 2882-2889: ISO or UTF-8 at line 1 triggers compileHyphenation
    uint8_t encoding_choice = fdp.ConsumeIntegral<uint8_t>() % 3;
    if (encoding_choice == 0) {
        content += "ISO\n";
    } else if (encoding_choice == 1) {
        content += "UTF-8\n";
    } else {
        // No encoding line - table won't trigger hyphenation compilation
        // but we'll still add hyphenation patterns that might be ignored
    }
    
    // Add table metadata
    if (fdp.ConsumeBool()) {
        content += "table hyphenation-test\n";
    }
    
    if (fdp.ConsumeBool()) {
        content += "locale ";
        content += fdp.ConsumeRandomLengthString(16);
        content += "\n";
    }
    
    // Generate hyphenation patterns (these are processed by compileHyphenation)
    size_t num_patterns = fdp.ConsumeIntegralInRange<size_t>(1, 10);
    
    for (size_t i = 0; i < num_patterns; i++) {
        // Hyphenation patterns are lines with mixed letters and digits
        content += generate_hyphenation_pattern(fdp);
        content += "\n";
    }
    
    // Generate hyphenation exceptions (special words with hyphen positions)
    if (fdp.ConsumeBool()) {
        size_t num_exceptions = fdp.ConsumeIntegralInRange<size_t>(1, 5);
        content += "%\n"; // Exception section marker
        
        for (size_t i = 0; i < num_exceptions; i++) {
            content += generate_hyphenation_exception(fdp);
            content += "\n";
        }
    }
    
    // Add some regular translation rules to mix with hyphenation
    // This tests interaction between hyphenation and regular compilation
    if (fdp.ConsumeBool()) {
        size_t num_rules = fdp.ConsumeIntegralInRange<size_t>(1, 5);
        
        for (size_t i = 0; i < num_rules; i++) {
            uint8_t rule_type = fdp.ConsumeIntegral<uint8_t>() % 5;
            
            switch (rule_type) {
                case 0: // Character definition
                    content += "space ";
                    content += fdp.ConsumeIntegralInRange<char>('!', '~');
                    content += " ";
                    content += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 8));
                    content += "\n";
                    break;
                    
                case 1: // Letter rule
                    content += fdp.ConsumeIntegralInRange<char>('a', 'z');
                    content += " ";
                    content += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 255));
                    content += "\n";
                    break;
                    
                case 2: // Contraction
                    if (fdp.ConsumeBool()) {
                        content += "contraction ";
                        content += fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(1, 8));
                        content += "\n";
                    }
                    break;
                    
                case 3: // Class definition
                    if (fdp.ConsumeBool()) {
                        const char* classes[] = {"digit", "uppercase", "lowercase", "punctuation"};
                        size_t class_idx = fdp.ConsumeIntegralInRange<size_t>(0, 3);
                        content += classes[class_idx];
                        content += " ";
                        content += fdp.ConsumeIntegralInRange<char>('!', '~');
                        content += "\n";
                    }
                    break;
                    
                case 4: // Multipass
                    if (fdp.ConsumeBool()) {
                        content += "multipass ";
                        content += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 3));
                        content += "\n";
                    }
                    break;
            }
        }
    }
    
    return content;
}

// Simple ASCII to widechar conversion for testing
static size_t ascii_to_widechar(const char* ascii, widechar* wide, size_t max_len) {
    size_t i;
    for (i = 0; i < max_len && ascii[i] != '\0'; i++) {
        wide[i] = ascii[i];
    }
    return i;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough for table generation and text input
    if (size < 64) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize liblouis once
    static bool initialized = false;
    if (!initialized) {
        lou_registerLogCallback(avoid_log_callback);
        initialized = true;
    }
    
    // PART 1: Hyphenation table compilation testing
    // Generate hyphenation-specific table content
    std::string table_content = generate_hyphenation_table(fdp);
    
    // Compile the table string - need table name and content
    // lou_compileString expects: (tableList, inString)
    std::string table_name_for_compile = "hyphenation-test.ctb";
    int compile_result = lou_compileString(table_name_for_compile.c_str(), table_content.c_str());
    
    // If compilation succeeded, we can test hyphenation with this table
    // but for fuzzing purposes, we'll continue with predefined tables
    
    // PART 2: Hyphenation API testing with lou_hyphenate
    // Select tables that likely support hyphenation
    const char* hyphenation_tables[] = {
        "en-us-g2.ctb",      // US English Grade 2 - likely supports hyphenation
        "en-ueb-g2.ctb",     // Unified English Braille Grade 2
        "en-us-comp8.ctb",   // US English computer braille
        "en-gb-g2.ctb",      // UK English Grade 2
        "de-de-g2.ctb",      // German Grade 2
        "fr-fr-g2.ctb",      // French Grade 2
        "es-es-g2.ctb"       // Spanish Grade 2
    };
    
    const size_t num_tables = sizeof(hyphenation_tables) / sizeof(hyphenation_tables[0]);
    size_t table_idx = fdp.ConsumeIntegralInRange<size_t>(0, num_tables - 1);
    const char* table_name = hyphenation_tables[table_idx];
    
    // Consume text input for hyphenation testing
    size_t max_text_len = fdp.ConsumeIntegralInRange<size_t>(1, 512);
    std::string text = fdp.ConsumeRandomLengthString(max_text_len);
    
    if (text.empty()) {
        // If we have no text but compiled a table, still return 0 (successful fuzzing iteration)
        return 0;
    }
    
    // Convert to widechar format required by liblouis APIs
    std::vector<widechar> inbuf(text.size() + 1);
    size_t inlen = ascii_to_widechar(text.c_str(), inbuf.data(), text.size());
    int inlen_int = static_cast<int>(inlen);
    
    // Allocate hyphenation result buffer (1 char per input character)
    std::vector<char> hyphens(inlen_int + 1);
    
    // Select mode for hyphenation based on fuzzer input
    int mode = 0;  // plain text mode
    uint8_t mode_choice = fdp.ConsumeIntegral<uint8_t>() % 8;
    
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
        case 4:
            mode = 0x0400 | 0x0800;  // computer_braille + no_translate
            break;
        case 5:
            mode = 0x0400 | 0x1000;  // computer_braille + no_contract
            break;
        case 6:
            mode = 0x0800 | 0x1000;  // no_translate + no_contract
            break;
        case 7:
            mode = 0x0400 | 0x0800 | 0x1000;  // all flags
            break;
    }
    
    // Test 1: Basic lou_hyphenate call
    int hyphen_result = lou_hyphenate(table_name, inbuf.data(), inlen_int, hyphens.data(), mode);
    
    // Test 2: If hyphenation succeeded, test lou_translatePrehyphenated
    if (hyphen_result == 1 && inlen_int > 0) {
        // Allocate buffers for translation with hyphenation
        int outlen_int = inlen_int * 16;  // Braille can expand
        std::vector<widechar> outbuf(outlen_int + 1);
        
        // Allocate arrays for position tracking
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
        int translate_result = lou_translatePrehyphenated(
            table_name, inbuf.data(), &inlen_int,
            outbuf.data(), &outlen_int,
            NULL, NULL,  // typeform and spacing (optional)
            outputPos.data(), inputPos.data(), &cursorPos,
            inputHyphens.data(), outputHyphens.data(),
            mode
        );
        
        // Test 3: Also test regular lou_translate for comparison
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
                table_name, inbuf.data(), &compare_inlen,
                compare_outbuf.data(), &compare_outlen,
                NULL, NULL,
                compare_outputPos.data(), compare_inputPos.data(), &compare_cursorPos,
                mode
            );
        }
    }
    
    // Test 4: If we compiled a table successfully, test hyphenation with it
    if (compile_result == 1 && fdp.remaining_bytes() > 20) {
        // The table compilation succeeded
        // We could try to use the compiled table, but lou_hyphenate expects a file name
        // not a table pointer. The compilation was primarily to exercise compileHyphenation paths
        
        // Try to free the compiled table file if possible
        // Note: lou_freeTableFile expects a file name
        // lou_freeTableFile(table_name_for_compile.c_str());
        
        // Since we can't easily use the compiled table with lou_hyphenate,
        // we'll just note that compilation succeeded and continue
    }
    
    // Test 5: Load a table with lou_getTable and test hyphenation
    // This exercises different code paths than just using table name
    if (fdp.ConsumeBool()) {
        const void* loaded_table = lou_getTable(table_name);
        if (loaded_table != NULL) {
            // Test hyphenation again with the same table name
            // (lou_hyphenate uses table name, not table pointer)
            std::vector<char> hyphens2(inlen_int + 1);
            int hyphen_result2 = lou_hyphenate(table_name, inbuf.data(), inlen_int, hyphens2.data(), mode);
        }
    }
    
    // Test 6: Test with different input configurations
    if (fdp.remaining_bytes() > 50) {
        // Generate a second text input
        size_t text2_len = fdp.ConsumeIntegralInRange<size_t>(1, 256);
        std::string text2 = fdp.ConsumeRandomLengthString(text2_len);
        
        if (!text2.empty()) {
            std::vector<widechar> inbuf2(text2.size() + 1);
            size_t inlen2 = ascii_to_widechar(text2.c_str(), inbuf2.data(), text2.size());
            int inlen_int2 = static_cast<int>(inlen2);
            
            std::vector<char> hyphens3(inlen_int2 + 1);
            
            // Use a different mode
            int mode2 = fdp.ConsumeIntegral<int>() & 0x1F00;
            
            int hyphen_result3 = lou_hyphenate(table_name, inbuf2.data(), inlen_int2, hyphens3.data(), mode2);
        }
    }
    
    // Clean up
    lou_free();
    
    return 0;
}

} // namespace harness_018
