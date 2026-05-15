#include <fuzzer/FuzzedDataProvider.h>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>
#include <iostream>
#include "liblouis.h"

namespace harness_016 {
// liblouis comprehensive table compilation and hyphenation fuzzing harness
// Target: loadTable (894 undiscovered branches) via lou_getTable, lou_compileString (exercises _lou_compileTranslationRule internally), lou_hyphenate (48 undiscovered branches with 9.62% coverage)
// Implement comprehensive workflow: setDataPath -> compileString -> getTable -> hyphenate -> free
// Based on coverage analysis guidance to target highest undiscovered complexity functions
// Semantic differentiation: Comprehensive workflow targeting table loading/compilation and hyphenation


// Include liblouis public header
extern "C" {
}

// Log callback to suppress logs during fuzzing
static void avoid_log_callback(logLevels level, const char *msg) {
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


// Generate simple table content for compilation testing
static std::string generate_table_content(FuzzedDataProvider& fdp) {
    std::string content;
    
    // Basic table header
    content += "table fuzz-generated-table\n";
    content += "locale en_US\n";
    content += "author fuzzer-harness-016\n";
    
    // Add some rules based on fuzzer input
    size_t num_rules = fdp.ConsumeIntegralInRange<size_t>(0, 10);
    for (size_t i = 0; i < num_rules; i++) {
        // Generate rule type
        uint8_t rule_type = fdp.ConsumeIntegral<uint8_t>() % 6;
        switch (rule_type) {
            case 0:
                content += "space ";
                content += fdp.ConsumeIntegralInRange<char>('a', 'z');
                content += " ";
                content += std::to_string(fdp.ConsumeIntegralInRange<uint8_t>(1, 8));
                content += "\n";
                break;
            case 1:
                content += "digit ";
                content += fdp.ConsumeIntegralInRange<char>('0', '9');
                content += " ";
                content += std::to_string(fdp.ConsumeIntegralInRange<uint16_t>(1, 255));
                content += "\n";
                break;
            case 2:
                content += "letter ";
                content += fdp.ConsumeIntegralInRange<char>('a', 'z');
                content += " ";
                content += std::to_string(fdp.ConsumeIntegralInRange<uint8_t>(1, 8));
                content += "\n";
                break;
            case 3:
                content += "punctuation ";
                content += fdp.ConsumeIntegralInRange<char>('!', '/');
                content += " ";
                content += std::to_string(fdp.ConsumeIntegralInRange<uint8_t>(1, 8));
                content += "\n";
                break;
            case 4:
                content += "always ";
                content += fdp.ConsumeIntegralInRange<char>('a', 'z');
                content += "\n";
                break;
            case 5:
                content += "nocontract ";
                content += fdp.ConsumeIntegralInRange<char>('a', 'z');
                content += "\n";
                break;
        }
    }
    
    // Add hyphenation directive if fuzzer decides
    if (fdp.ConsumeBool()) {
        content += "hyphenation\n";
        content += "hyphenate 2 2\n";  // Allow 2 chars at start and end
    }
    
    return content;
}

// Generate hyphenation patterns for testing
static void generate_hyphenation_pattern(FuzzedDataProvider& fdp, char* hyphens, int len) {
    for (int i = 0; i < len; i++) {
        // Generate hyphen pattern: '0' for no hyphen, '1' for hyphen
        // Some hyphenation implementations may use other chars
        uint8_t pattern = fdp.ConsumeIntegral<uint8_t>() % 4;
        switch (pattern) {
            case 0:
                hyphens[i] = '0';  // No hyphen
                break;
            case 1:
                hyphens[i] = '1';  // Hyphen
                break;
            case 2:
                hyphens[i] = '.';  // Alternative hyphen marker
                break;
            case 3:
                hyphens[i] = '-';  // Dash as hyphen marker
                break;
        }
    }
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough for all operations
    if (size < 64) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize liblouis once with log suppression
    static bool initialized = false;
    if (!initialized) {
        lou_registerLogCallback(avoid_log_callback);
        lou_setLogLevel(LOU_LOG_FATAL);  // Only show fatal errors
        initialized = true;
    }
    
    // 1. Set data path (lou_setDataPath)
    // Consume path string from fuzzer input
    size_t max_path_len = fdp.ConsumeIntegralInRange<size_t>(1, 256);
    std::string data_path = fdp.ConsumeRandomLengthString(max_path_len);
    
    char* set_path_result = lou_setDataPath(data_path.c_str());
    // Note: lou_setDataPath returns NULL if path is NULL or too long
    
    // 2. Generate and compile table string (lou_compileString)
    std::string table_content = generate_table_content(fdp);
    const char* table_name = "fuzz-compiled-table";
    
    // Compile the generated table content
    int compile_result = lou_compileString(table_name, table_content.c_str());
    
    // 3. Get table handle (lou_getTable) - this exercises loadTable internally
    const void* table_handle = lou_getTable(table_name);
    
    // Also test with some known table names
    const char* known_tables[] = {
        "en-us-g2.ctb",
        "en-ueb-g2.ctb", 
        "en-us-comp8.ctb",
        "de-de-g2.ctb"
    };
    const size_t num_known_tables = sizeof(known_tables) / sizeof(known_tables[0]);
    size_t known_table_idx = fdp.ConsumeIntegralInRange<size_t>(0, num_known_tables - 1);
    const char* known_table = known_tables[known_table_idx];
    
    const void* known_table_handle = lou_getTable(known_table);
    // 4. Test compilation again with different content (exercises _lou_compileTranslationRule internally)
    // Generate another table string for compilation
    std::string second_table_content = generate_table_content(fdp);
    
    // Compile second table string (lou_compileString exercises _lou_compileTranslationRule internally)
    int second_compile_result = lou_compileString(table_name, second_table_content.c_str());
    size_t max_text_len = fdp.ConsumeIntegralInRange<size_t>(1, 512);
    std::string text = fdp.ConsumeRandomLengthString(max_text_len);
    
    if (!text.empty()) {
        // Convert to widechar format
        std::vector<widechar> inbuf(text.size() + 1);
        size_t inlen = ascii_to_widechar(text.c_str(), inbuf.data(), text.size());
        int inlen_int = static_cast<int>(inlen);
        
        if (inlen_int > 0) {
            // Allocate hyphenation result buffer
            std::vector<char> hyphens(inlen_int + 1);
            
            // Generate hyphenation pattern
            generate_hyphenation_pattern(fdp, hyphens.data(), inlen_int);
            
            // Select mode for hyphenation
            int mode = 0;  // plain text mode
            uint8_t mode_choice = fdp.ConsumeIntegral<uint8_t>() % 8;
            switch (mode_choice) {
                case 0:
                    mode = 0;  // plain_text
                    break;
                case 1:
                    mode = 0x0001;  // emph_1 (italic)
                    break;
                case 2:
                    mode = 0x0002;  // emph_2 (underline)
                    break;
                case 3:
                    mode = 0x0004;  // emph_3 (bold)
                    break;
                case 4:
                    mode = 0x0400;  // computer_braille
                    break;
                case 5:
                    mode = 0x0800;  // no_translate
                    break;
                case 6:
                    mode = 0x1000;  // no_contract
                    break;
                case 7:
                    mode = 0x0001 | 0x0002;  // Combined emphasis
                    break;
            }
            
            // Test hyphenate with compiled table
            int hyphen_result = lou_hyphenate(table_name, inbuf.data(), inlen_int, hyphens.data(), mode);
            
            // Test hyphenate with known table
            if (known_table_handle != NULL) {
                std::vector<char> known_hyphens(inlen_int + 1);
                generate_hyphenation_pattern(fdp, known_hyphens.data(), inlen_int);
                int known_hyphen_result = lou_hyphenate(known_table, inbuf.data(), inlen_int, known_hyphens.data(), mode);
            }
            
            // 6. Test translation with hyphenation (lou_translatePrehyphenated)
            if (hyphen_result == 1 && inlen_int > 0) {
                // Allocate translation buffers
                int outlen_int = inlen_int * 16;
                std::vector<widechar> outbuf(outlen_int + 1);
                
                // Allocate position tracking arrays
                std::vector<int> outputPos(inlen_int);
                std::vector<int> inputPos(outlen_int);
                int cursorPos = -1;
                
                // Allocate hyphen arrays
                std::vector<char> inputHyphens(inlen_int + 1);
                std::vector<char> outputHyphens(outlen_int + 1);
                
                // Copy generated hyphens
                for (int i = 0; i < inlen_int; i++) {
                    inputHyphens[i] = hyphens[i];
                }
                
                // Test pre-hyphenated translation
                int prehyphen_result = lou_translatePrehyphenated(
                    table_name, inbuf.data(), &inlen_int,
                    outbuf.data(), &outlen_int,
                    NULL, NULL,  // typeform and spacing
                    outputPos.data(), inputPos.data(), &cursorPos,
                    inputHyphens.data(), outputHyphens.data(),
                    mode
                );
            }
        }
    }
    
    // 7. Cleanup (lou_free) - free all allocated resources
    lou_free();
    
    return 0;
}

} // namespace harness_016
