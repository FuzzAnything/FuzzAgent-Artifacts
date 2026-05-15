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

namespace harness_009 {
// liblouis fuzzing harness - Deep compileRule function targeting with uncovered APIs
// Targets: lou_compileString, lou_checkTable, lou_charSize, lou_freeTableInfo, lou_setDataPath
// Focus: compileRule function with 339 blocked branches, ISO/UTF-8 header processing (0 hits),
//        CTO_Letter opcode handling (0 hits), CTO_Macro if enabled
// Based on coverage guidance: /root/FuzzAgent/output/liblouis/coverage_guidance/harness_009_guidance.md
// Uses FuzzedDataProvider for structured input processing
// FIXED VERSION: Removes state dependency issues, improves determinism


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

// Generate table with specific focus on compileRule blocked branches
static std::string generate_compileRule_focused_table(FuzzedDataProvider& fdp) {
    std::string table;
    
    // Step 1: ISO/UTF-8 header processing (targeting lines 2884-2893 in compileRule)
    uint8_t header_type = fdp.ConsumeIntegral<uint8_t>() % 5;
    switch (header_type) {
        case 0:
            // ISO header with version - targeting ISO header parsing
            table += "ISO ";
            table += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 16));
            table += "\n";
            break;
        case 1:
            // UTF-8 header - targeting UTF-8 header path
            table += "UTF-8\n";
            break;
        case 2:
            // UTF-16 or UTF-32 header - test other UTF variants
            table += "UTF-";
            table += std::to_string(fdp.ConsumeIntegralInRange<int>(16, 32));
            table += "\n";
            break;
        case 3:
            // No header - test default path
            break;
        case 4:
            // Invalid header to test error paths
            table += "INVALID-HEADER\n";
            break;
    }
    
    // Step 2: CTO_Letter opcode definitions with diverse Unicode characters
    int letter_count = fdp.ConsumeIntegralInRange<int>(1, 8); // Always at least 1 letter
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
    
    // Step 3: Include hyphenation compilation paths (lines 2884-2888)
    if (fdp.ConsumeBool()) {
        table += "hyphenation 2\n"; // Simple hyphenation rule
    }
    
    // Step 4: Include other untested opcodes: CTO_Display, CTO_Grouping
    if (fdp.ConsumeBool()) {
        int display_count = fdp.ConsumeIntegralInRange<int>(0, 2);
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
    
    if (fdp.ConsumeBool()) {
        int grouping_count = fdp.ConsumeIntegralInRange<int>(0, 1);
        for (int i = 0; i < grouping_count; i++) {
            if (fdp.remaining_bytes() < 3) break;
            
            table += "grouping \"";
            int group_len = fdp.ConsumeIntegralInRange<int>(1, 3);
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
    
    // Step 5: Test CTO_Macro usage if enabled (compileRule lines 2896-2911)
#ifdef ENABLE_MACROS
    if (fdp.ConsumeBool()) {
        table += "macro test_macro_";
        table += std::to_string(fdp.ConsumeIntegral<uint32_t>() % 10);
        table += "\n";
        
        int macro_lines = fdp.ConsumeIntegralInRange<int>(1, 3);
        for (int j = 0; j < macro_lines; j++) {
            table += "letter \"";
            table += generate_unicode_char(fdp);
            table += "\" ";
            table += generate_braille_dots(fdp);
            table += "\n";
        }
        
        table += "eom\n";
    }
#endif
    
    // Step 6: Include diverse character class definitions (CTC_Letter, CTC_Space, CTC_Digit)
    // to test related compilation paths
    if (fdp.ConsumeBool()) {
        table += "space \\x";
        char hex_chars[] = "0123456789abcdef";
        for (int i = 0; i < 4; i++) {
            table += hex_chars[fdp.ConsumeIntegralInRange<int>(0, 15)];
        }
        table += " ";
        table += generate_braille_dots(fdp);
        table += "\n";
    }
    
    if (fdp.ConsumeBool()) {
        table += "digit \\x";
        char hex_chars[] = "0123456789abcdef";
        for (int i = 0; i < 4; i++) {
            table += hex_chars[fdp.ConsumeIntegralInRange<int>(0, 15)];
        }
        table += " ";
        table += generate_braille_dots(fdp);
        table += "\n";
    }
    
    if (fdp.ConsumeBool()) {
        table += "punctuation \\x";
        char hex_chars[] = "0123456789abcdef";
        for (int i = 0; i < 4; i++) {
            table += hex_chars[fdp.ConsumeIntegralInRange<int>(0, 15)];
        }
        table += " ";
        table += generate_braille_dots(fdp);
        table += "\n";
    }
    
    // Add terminator
    table += "LOU_ENDSEGMENT\n";
    
    return table;
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Minimum size check - need enough data for meaningful table generation
    if (size < 24) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize liblouis for this iteration (deterministic, isolated)
    lou_registerLogCallback(avoid_log);
    
    // Configure table search paths using lou_setDataPath
    // Use a fixed, deterministic temporary directory path
    const char* temp_path = "/tmp/liblouis-fuzz-deterministic";
    
    // Create the directory if it doesn't exist
    mkdir(temp_path, 0755);
    lou_setDataPath(temp_path);
    
    // Test lou_charSize API (uncovered API)
    int char_size = lou_charSize();
    (void)char_size; // Result used for coverage
    
    // Generate table name from input
    std::string table_name = fdp.ConsumeRandomLengthString(48);
    if (table_name.empty() && fdp.remaining_bytes() > 0) {
        table_name = "fuzz-table-" + std::to_string(fdp.ConsumeIntegral<uint32_t>());
    }
    
    // Generate compileRule-focused table content
    std::string table_content = generate_compileRule_focused_table(fdp);
    if (table_content.empty()) {
        lou_free(); // Cleanup before returning
        return 0;
    }
    
    // Step 2 & 3: Test ISO and UTF-8 headers by generating different tables
    // We already generate different headers in generate_compileRule_focused_table
    // Compile the table
    int compile_result = lou_compileString(table_name.c_str(), table_content.c_str());
    (void)compile_result; // Result ignored for fuzzing
    
    // Step 5: Validate compiled tables with lou_checkTable
    int check_result = lou_checkTable(table_name.c_str());
    (void)check_result;
    
    // Test lou_getTableInfo and lou_freeTableInfo APIs
    if (fdp.remaining_bytes() > 4) {
        // Try to get table metadata
        std::string metadata_key;
        uint8_t key_choice = fdp.ConsumeIntegral<uint8_t>() % 4;
        switch (key_choice) {
            case 0: metadata_key = "language"; break;
            case 1: metadata_key = "locale"; break;
            case 2: metadata_key = "type"; break;
            case 3: metadata_key = "description"; break;
        }
        
        char *table_info = lou_getTableInfo(table_name.c_str(), metadata_key.c_str());
        if (table_info != NULL) {
            // Free the memory with lou_freeTableInfo
            lou_freeTableInfo(table_info);
        }
    }
    
    // Test with alternative table names and content
    if (fdp.remaining_bytes() > 16) {
        std::string alt_table_name = fdp.ConsumeRandomLengthString(32);
        if (!alt_table_name.empty()) {
            // Generate alternative table with different header
            std::string alt_table;
            
            // Force different header type
            if (fdp.ConsumeBool()) {
                alt_table += "ISO 8859-1\n";
            } else {
                alt_table += "UTF-8\n";
            }
            
            // Add letter definitions
            int alt_letter_count = fdp.ConsumeIntegralInRange<int>(1, 4);
            for (int i = 0; i < alt_letter_count && fdp.remaining_bytes() > 2; i++) {
                alt_table += "letter \"";
                alt_table += generate_unicode_char(fdp);
                alt_table += "\" ";
                alt_table += generate_braille_dots(fdp);
                alt_table += "\n";
            }
            
            alt_table += "LOU_ENDSEGMENT\n";
            
            lou_compileString(alt_table_name.c_str(), alt_table.c_str());
            lou_checkTable(alt_table_name.c_str());
            
            // Test table info API again
            char *alt_info = lou_getTableInfo(alt_table_name.c_str(), "language");
            if (alt_info != NULL) {
                lou_freeTableInfo(alt_info);
            }
        }
    }
    
    // Test lou_charSize again after table operations
    int final_char_size = lou_charSize();
    (void)final_char_size;
    
    // Clean up all resources for this iteration
    lou_free();
    
    return 0;
}

} // namespace harness_009
