#include <fuzzer/FuzzedDataProvider.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <liblouis/liblouis.h>
#include <assert.h>
#include <vector>
#include <string>
#include <sys/stat.h>
#include <fcntl.h>

namespace harness_010 {
// liblouis fuzzing harness - Hyphenation functionality targeting with ISO/UTF-8 headers
// Targets: lou_hyphenate (primary - 54 undiscovered branches), lou_translatePrehyphenated (secondary)
//          lou_compileString, lou_setLogLevel, lou_free
// Focus: Hyphenation functionality with ISO/UTF-8 headers, diverse opcode sequences
//        compileRule function (339 blocked branches), CTO_Letter opcode path (0 hits)
//        ISO/UTF-8 header processing (0 hits for compileHyphenation path)
// Based on coverage guidance: /root/FuzzAgent/output/liblouis/coverage_guidance/harness_010_guidance.md
// Uses FuzzedDataProvider for structured input processing


// Log callback to suppress output during fuzzing
void avoid_log(logLevels level, const char *msg) {
    (void)level;
    (void)msg;
}

// Generate a character for CTO_Letter opcode definition (Unicode-aware)
static std::string generate_unicode_char(FuzzedDataProvider& fdp) {
    // Generate different types of Unicode characters for letter definitions
    uint8_t char_type = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    switch (char_type) {
        case 0:
            // Basic ASCII letter
            return std::string(1, static_cast<char>(fdp.ConsumeIntegralInRange<int>('a', 'z')));
        case 1:
            // ASCII uppercase letter  
            return std::string(1, static_cast<char>(fdp.ConsumeIntegralInRange<int>('A', 'Z')));
        case 2:
            // Unicode character using \uXXXX escape (4 hex digits)
            {
                int unicode_val = fdp.ConsumeIntegralInRange<int>(0x0041, 0x024F);
                char hex_buf[7];
                snprintf(hex_buf, sizeof(hex_buf), "\\u%04X", unicode_val);
                return std::string(hex_buf);
            }
        case 3:
            // Hex character representation using \xXX escape (2 hex digits)
            {
                int hex_val = fdp.ConsumeIntegralInRange<int>(0x20, 0x7E);
                char hex_buf[5];
                snprintf(hex_buf, sizeof(hex_buf), "\\x%02X", hex_val);
                return std::string(hex_buf);
            }
        default:
            return "a";
    }
}

// Generate braille dots pattern with more diversity
static std::string generate_braille_dots(FuzzedDataProvider& fdp) {
    std::string dots;
    int dot_count = fdp.ConsumeIntegralInRange<int>(1, 12); // Up to 12 dots for extended braille
    
    for (int i = 0; i < dot_count && i < 8; i++) {
        int dot = fdp.ConsumeIntegralInRange<int>(1, 8);
        dots += std::to_string(dot);
    }
    
    // Add extended dots (9-16 as a-f) if we have more than 8
    if (dot_count > 8 && fdp.ConsumeBool()) {
        for (int i = 8; i < dot_count && i < 16; i++) {
            int ext_dot = fdp.ConsumeIntegralInRange<int>(0, 5);
            dots += static_cast<char>('a' + ext_dot);
        }
    }
    
    // Sometimes use hyphen for no dots
    if (dots.empty() || fdp.ConsumeBool()) {
        return "-";
    }
    
    return dots;
}

// Generate hyphenation patterns (similar to .dic files)
static std::string generate_hyphenation_pattern(FuzzedDataProvider& fdp) {
    std::string pattern;
    int pattern_len = fdp.ConsumeIntegralInRange<int>(2, 8);
    
    for (int i = 0; i < pattern_len; i++) {
        uint8_t pattern_type = fdp.ConsumeIntegral<uint8_t>() % 3;
        
        switch (pattern_type) {
            case 0:
                // Letter
                pattern += generate_unicode_char(fdp);
                break;
            case 1:
                // Digit (hyphenation priority)
                pattern += std::to_string(fdp.ConsumeIntegralInRange<int>(0, 9));
                break;
            case 2:
                // Dot for syllable boundary
                pattern += ".";
                break;
        }
    }
    
    return pattern;
}

// Generate table with hyphenation functionality focus
static std::string generate_hyphenation_table(FuzzedDataProvider& fdp) {
    std::string table;
    
    // Step 1: ISO/UTF-8 header processing (targeting compileHyphenation path)
    uint8_t header_type = fdp.ConsumeIntegral<uint8_t>() % 6;
    switch (header_type) {
        case 0:
            // ISO header with specific version - targeting ISO header parsing
            table += "ISO ";
            table += std::to_string(fdp.ConsumeIntegralInRange<int>(8859, 8859));
            table += "-";
            table += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 16));
            table += "\n";
            break;
        case 1:
            // UTF-8 header - targeting UTF-8 header path
            table += "UTF-8\n";
            break;
        case 2:
            // UTF-16 header
            table += "UTF-16\n";
            break;
        case 3:
            // UTF-32 header
            table += "UTF-32\n";
            break;
        case 4:
            // No header to test default path
            break;
        case 5:
            // Invalid header to test error paths
            table += "INVALID-ENCODING\n";
            break;
    }
    
    // Step 2: CTO_Letter opcode definitions (targeting CTO_Letter opcode path - 0 hits)
    int letter_count = fdp.ConsumeIntegralInRange<int>(1, 12); // Always at least 1 letter
    for (int i = 0; i < letter_count; i++) {
        if (fdp.remaining_bytes() < 2) break;
        
        std::string letter_char = generate_unicode_char(fdp);
        std::string dots = generate_braille_dots(fdp);
        
        table += "letter \"";
        table += letter_char;
        table += "\" ";
        table += dots;
        table += "\n";
    }
    
    // Step 3: Hyphenation patterns and rules
    int hyphen_rule_count = fdp.ConsumeIntegralInRange<int>(0, 8);
    for (int i = 0; i < hyphen_rule_count; i++) {
        if (fdp.remaining_bytes() < 3) break;
        
        // Add hyphenation rule
        table += "hyphenation ";
        table += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 4));
        table += "\n";
        
        // Add hyphenation pattern
        if (fdp.ConsumeBool()) {
            table += generate_hyphenation_pattern(fdp);
            table += "\n";
        }
    }
    
    // Step 4: Diverse opcodes for comprehensive coverage
    // CTO_Macro (if enabled)
#ifdef ENABLE_MACROS
    if (fdp.ConsumeBool()) {
        table += "macro test_macro_";
        table += std::to_string(fdp.ConsumeIntegral<uint32_t>() % 10);
        table += " \"";
        table += generate_braille_dots(fdp);
        table += "\"\n";
    }
#endif
    
    // CTO_Display opcode
    if (fdp.ConsumeBool()) {
        int display_count = fdp.ConsumeIntegralInRange<int>(0, 3);
        for (int i = 0; i < display_count; i++) {
            if (fdp.remaining_bytes() < 2) break;
            
            std::string display_char = generate_unicode_char(fdp);
            std::string display_dots = generate_braille_dots(fdp);
            
            table += "display \"";
            table += display_char;
            table += "\" ";
            table += display_dots;
            table += "\n";
        }
    }
    
    // CTO_Grouping opcode
    if (fdp.ConsumeBool()) {
        int grouping_count = fdp.ConsumeIntegralInRange<int>(0, 2);
        for (int i = 0; i < grouping_count; i++) {
            if (fdp.remaining_bytes() < 3) break;
            
            table += "grouping \"";
            int group_len = fdp.ConsumeIntegralInRange<int>(1, 4);
            for (int j = 0; j < group_len; j++) {
                table += generate_unicode_char(fdp);
            }
            
            table += "\" \"";
            table += generate_braille_dots(fdp);
            table += "\" ";
            
            if (fdp.ConsumeBool()) {
                table += "before";
            } else {
                table += "after";
            }
            table += "\n";
        }
    }
    
    // CTO_UpperCase, CTO_Digit, CTO_Punctuation for comprehensive character classes
    if (fdp.ConsumeBool()) {
        table += "uppercase \"";
        table += generate_unicode_char(fdp);
        table += "\" ";
        table += generate_braille_dots(fdp);
        table += "\n";
    }
    
    if (fdp.ConsumeBool()) {
        table += "digit \"";
        table += generate_unicode_char(fdp);
        table += "\" ";
        table += generate_braille_dots(fdp);
        table += "\n";
    }
    
    if (fdp.ConsumeBool()) {
        table += "punctuation \"";
        table += generate_unicode_char(fdp);
        table += "\" ";
        table += generate_braille_dots(fdp);
        table += "\n";
    }
    
    // Step 5: Terminator
    table += "LOU_ENDSEGMENT\n";
    
    return table;
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Minimum size check - need enough data for meaningful table generation and testing
    if (size < 32) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Step 1: Configure logging
    lou_registerLogCallback(avoid_log);
    
    // Set log level based on input
    uint8_t log_level_choice = fdp.ConsumeIntegral<uint8_t>() % 6;
    logLevels log_level = LOU_LOG_ALL;
    switch (log_level_choice) {
        case 0: log_level = LOU_LOG_ALL; break;
        case 1: log_level = LOU_LOG_DEBUG; break;
        case 2: log_level = LOU_LOG_INFO; break;
        case 3: log_level = LOU_LOG_WARN; break;
        case 4: log_level = LOU_LOG_ERROR; break;
        case 5: log_level = LOU_LOG_FATAL; break;
    }
    lou_setLogLevel(log_level);
    
    // Configure table search paths using lou_setDataPath
    // Use a fixed, deterministic temporary directory path
    const char* temp_path = "/tmp/liblouis-fuzz-hyphenation";
    mkdir(temp_path, 0755);
    lou_setDataPath(temp_path);
    
    // Generate table name from input
    std::string table_name = fdp.ConsumeRandomLengthString(48);
    if (table_name.empty() && fdp.remaining_bytes() > 0) {
        table_name = "fuzz-hyphen-table-" + std::to_string(fdp.ConsumeIntegral<uint32_t>());
    }
    
    // Generate hyphenation-focused table content
    std::string table_content = generate_hyphenation_table(fdp);
    if (table_content.empty()) {
        lou_free(); // Cleanup before returning
        return 0;
    }
    
    // Step 2: Compile table with hyphenation functionality
    int compile_result = lou_compileString(table_name.c_str(), table_content.c_str());
    (void)compile_result; // Result ignored for fuzzing
    
    // Validate compiled tables with lou_checkTable
    int check_result = lou_checkTable(table_name.c_str());
    (void)check_result;
    
    // Step 3: Test lou_hyphenate with varied inputs
    // Generate test words with different syllable structures
    int test_word_count = fdp.ConsumeIntegralInRange<int>(1, 5);
    for (int w = 0; w < test_word_count && fdp.remaining_bytes() > 4; w++) {
        // Generate test word
        std::string test_word;
        int word_len = fdp.ConsumeIntegralInRange<int>(1, 32);
        for (int i = 0; i < word_len && fdp.remaining_bytes() > 0; i++) {
            uint8_t char_type = fdp.ConsumeIntegral<uint8_t>() % 3;
            switch (char_type) {
                case 0:
                    test_word += static_cast<char>(fdp.ConsumeIntegralInRange<int>('a', 'z'));
                    break;
                case 1:
                    test_word += static_cast<char>(fdp.ConsumeIntegralInRange<int>('A', 'Z'));
                    break;
                case 2:
                    // Mixed case and special characters
                    test_word += static_cast<char>(fdp.ConsumeIntegralInRange<int>(0x20, 0x7E));
                    break;
            }
        }
        
        if (!test_word.empty()) {
            // Convert to widechar format
            size_t text_len = test_word.length();
            widechar* wide_input = (widechar*)malloc((text_len + 1) * sizeof(widechar));
            if (wide_input) {
                for (size_t i = 0; i < text_len; i++) {
                    wide_input[i] = (widechar)test_word[i];
                }
                wide_input[text_len] = 0;
                
                // Test different modes
                int mode = fdp.ConsumeIntegral<uint8_t>() % 4;
                
                // Allocate hyphenation result buffer
                char* hyphens = (char*)malloc(text_len + 1);
                if (hyphens) {
                    // Call lou_hyphenate (primary target)
                    int hyphen_result = lou_hyphenate(
                        table_name.c_str(), wide_input, (int)text_len, hyphens, mode);
                    (void)hyphen_result;
                    
                    // Also test with different table combinations
                    if (fdp.ConsumeBool()) {
                        std::string table_combo = table_name + ",tables/hyph_en_US.dic";
                        hyphen_result = lou_hyphenate(
                            table_combo.c_str(), wide_input, (int)text_len, hyphens, mode);
                        (void)hyphen_result;
                    }
                    
                    free(hyphens);
                }
                
                // Step 4: Test lou_translatePrehyphenated (secondary target)
                if (fdp.remaining_bytes() > 8) {
                    // Prepare buffers for translatePrehyphenated
                    int inlen = (int)text_len;
                    int outlen = inlen * 2; // Conservative output buffer size
                    widechar* outbuf = (widechar*)malloc((outlen + 1) * sizeof(widechar));
                    formtype* typeform = NULL;
                    char* spacing = NULL;
                    int* outputPos = NULL;
                    int* inputPos = NULL;
                    int* cursorPos = NULL;
                    
                    // Generate hyphen markers
                    char* inputHyphens = (char*)malloc(text_len + 1);
                    char* outputHyphens = (char*)malloc(outlen + 1);
                    
                    if (outbuf && inputHyphens && outputHyphens) {
                        // Fill hyphen markers
                        for (size_t i = 0; i < text_len; i++) {
                            inputHyphens[i] = fdp.ConsumeBool() ? '1' : '0';
                        }
                        inputHyphens[text_len] = '\0';
                        
                        // Call lou_translatePrehyphenated
                        int translate_result = lou_translatePrehyphenated(
                            table_name.c_str(), wide_input, &inlen, outbuf, &outlen,
                            typeform, spacing, outputPos, inputPos, cursorPos,
                            inputHyphens, outputHyphens, mode);
                        (void)translate_result;
                        
                        free(outputHyphens);
                        free(inputHyphens);
                    }
                    
                    if (outbuf) free(outbuf);
                }
                
                free(wide_input);
            }
        }
    }
    
    if (fdp.remaining_bytes() > 16) {
        std::string alt_table_name = fdp.ConsumeRandomLengthString(32);
        if (!alt_table_name.empty()) {
            // Generate alternative table with different header
            std::string alt_table;
            
            // Force different header type for more coverage
            if (fdp.ConsumeBool()) {
                alt_table += "ISO 8859-1\n";
            } else {
                alt_table += "UTF-8\n";
            }
            
            // Add more diverse letter definitions
            int alt_letter_count = fdp.ConsumeIntegralInRange<int>(1, 8);
            for (int i = 0; i < alt_letter_count && fdp.remaining_bytes() > 2; i++) {
                alt_table += "letter \"";
                alt_table += generate_unicode_char(fdp);
                alt_table += "\" ";
                alt_table += generate_braille_dots(fdp);
                alt_table += "\n";
            }
            
            // Add hyphenation rule
            alt_table += "hyphenation ";
            alt_table += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 4));
            alt_table += "\n";
            
            alt_table += "LOU_ENDSEGMENT\n";
            
            lou_compileString(alt_table_name.c_str(), alt_table.c_str());
            lou_checkTable(alt_table_name.c_str());
        }
    }
    
    // Step 6: Cleanup
    lou_free();
    
    return 0;
}

} // namespace harness_010
