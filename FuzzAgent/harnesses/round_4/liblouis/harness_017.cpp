#include <fuzzer/FuzzedDataProvider.h>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include "liblouis.h"

namespace harness_017 {
// liblouis compileRule deep coverage harness - targeting CTO_Grouping and complex opcodes
// Target: compileRule function (568 blocked branches out of 910) focusing on translation table 
//         compilation with complex opcodes, especially grouping rules (CTO_Grouping)
// Primary API chain: lou_compileString -> compileString -> compileRule -> compileGrouping
// Based on coverage analysis guidance to target highest undiscovered complexity functions
// Semantic differentiation: Deep focus on grouping rules and complex opcode compilation patterns
//                           not covered in harness_008 (general opcode diversity) or harness_016 (workflow)


// Include liblouis public header
extern "C" {
}

// Log callback to suppress logs during fuzzing
static void avoid_log_callback(logLevels level, const char *msg) {
    (void)level;
    (void)msg;
    // Suppress all logs during fuzzing
}

// Helper to generate grouping rule syntax
static std::string generate_grouping_rule(FuzzedDataProvider& fdp) {
    std::string rule;
    
    // Grouping opcode
    rule += "grouping ";
    
    // Group name
    std::string group_name = fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(1, 16));
    if (group_name.empty()) group_name = "grp";
    rule += group_name + " ";
    
    // Two Unicode characters for grouping (escaped hex or direct chars)
    uint8_t char_type = fdp.ConsumeIntegral<uint8_t>() % 3;
    if (char_type == 0) {
        // Direct ASCII characters
        rule += fdp.ConsumeIntegralInRange<char>('a', 'z');
        rule += fdp.ConsumeIntegralInRange<char>('a', 'z');
    } else if (char_type == 1) {
        // Escaped hex characters
        rule += "\\x";
        char hex1[3];
        snprintf(hex1, sizeof(hex1), "%02x", fdp.ConsumeIntegralInRange<uint8_t>(0x20, 0x7e));
        rule += hex1;
        rule += "\\x";
        char hex2[3];
        snprintf(hex2, sizeof(hex2), "%02x", fdp.ConsumeIntegralInRange<uint8_t>(0x20, 0x7e));
        rule += hex2;
    } else {
        // Mixed
        rule += fdp.ConsumeIntegralInRange<char>('A', 'Z');
        rule += "\\x";
        char hex[3];
        snprintf(hex, sizeof(hex), "%02x", fdp.ConsumeIntegralInRange<uint8_t>(0x30, 0x39));
        rule += hex;
    }
    
    rule += " ";
    
    // Two braille cells separated by comma
    for (int i = 0; i < 2; i++) {
        // Generate braille dot pattern (1-8 dots)
        int dot_count = fdp.ConsumeIntegralInRange<int>(1, 8);
        for (int d = 0; d < dot_count; d++) {
            rule += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 8));
        }
        if (i == 0) rule += ",";
    }
    
    return rule + "\n";
}

// Helper to generate complex opcode sequences for compileRule coverage
static std::string generate_complex_opcode_sequence(FuzzedDataProvider& fdp) {
    std::string content;
    
    // Generate a mix of complex opcodes that exercise compileRule paths
    size_t num_opcodes = fdp.ConsumeIntegralInRange<size_t>(5, 15);
    
    for (size_t i = 0; i < num_opcodes; i++) {
        uint8_t opcode_type = fdp.ConsumeIntegral<uint8_t>() % 15;
        
        switch (opcode_type) {
            case 0: { // CTO_Grouping - our main target
                content += generate_grouping_rule(fdp);
                break;
            }
            
            case 1: { // Character class definitions (CTC_*)
                const char* char_classes[] = {"space", "digit", "litdigit", "punctuation", 
                                            "math", "sign", "letter", "uppercase", "lowercase"};
                const size_t num_classes = sizeof(char_classes) / sizeof(char_classes[0]);
                size_t class_idx = fdp.ConsumeIntegralInRange<size_t>(0, num_classes - 1);
                
                content += char_classes[class_idx] + std::string(" ");
                content += fdp.ConsumeIntegralInRange<char>('!', '~');
                content += " ";
                content += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 255));
                content += "\n";
                break;
            }
            
            case 2: { // Emphasis class definitions
                if (fdp.ConsumeBool()) {
                    content += "emphclass ";
                    content += fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(1, 16));
                    content += "\n";
                }
                break;
            }
            
            case 3: { // Display opcode (CTO_Display)
                content += "display ";
                content += fdp.ConsumeIntegralInRange<char>('!', '~');
                content += " ";
                content += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 8));
                content += "\n";
                break;
            }
            
            case 4: { // NoBack/NoFor
                if (fdp.ConsumeBool()) content += "noback\n";
                if (fdp.ConsumeBool()) content += "nofor\n";
                break;
            }
            
            case 5: { // Class opcodes
                const char* class_ops[] = {"class", "nocross", "nocompclass"};
                const size_t num_ops = sizeof(class_ops) / sizeof(class_ops[0]);
                size_t op_idx = fdp.ConsumeIntegralInRange<size_t>(0, num_ops - 1);
                
                content += class_ops[op_idx] + std::string(" ");
                content += fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(1, 16));
                content += "\n";
                break;
            }
            
            case 6: { // Multipass opcodes
                if (fdp.ConsumeBool()) {
                    content += "multipass ";
                    content += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 4));
                    content += "\n";
                }
                break;
            }
            
            case 7: { // Always/Contraction opcodes
                if (fdp.ConsumeBool()) {
                    content += "always ";
                    content += fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(1, 8));
                    content += "\n";
                } else if (fdp.ConsumeBool()) {
                    content += "contraction ";
                    content += fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(1, 8));
                    content += "\n";
                }
                break;
            }
            
            case 8: { // Caps related opcodes
                const char* caps_ops[] = {"capsletter", "capsign", "capsdigit", 
                                         "beginuppercase", "enduppercase"};
                const size_t num_caps = sizeof(caps_ops) / sizeof(caps_ops[0]);
                size_t caps_idx = fdp.ConsumeIntegralInRange<size_t>(0, num_caps - 1);
                
                content += caps_ops[caps_idx] + std::string(" ");
                content += fdp.ConsumeIntegralInRange<char>('!', '~');
                content += " ";
                content += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 255));
                content += "\n";
                break;
            }
            
            case 9: { // Context opcodes
                if (fdp.ConsumeBool()) {
                    content += "context ";
                    content += fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(1, 32));
                    content += "\n";
                }
                break;
            }
            
            case 10: { // Math opcodes
                content += "math ";
                content += fdp.ConsumeIntegralInRange<char>('!', '~');
                content += " ";
                content += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 255));
                content += "\n";
                break;
            }
            
            case 11: { // Word opcodes
                const char* word_ops[] = {"word", "begword", "endword", "partword", 
                                         "midword", "preword", "postword"};
                const size_t num_word = sizeof(word_ops) / sizeof(word_ops[0]);
                size_t word_idx = fdp.ConsumeIntegralInRange<size_t>(0, num_word - 1);
                
                content += word_ops[word_idx] + std::string(" ");
                content += fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(1, 16));
                content += "\n";
                break;
            }
            
            case 12: { // Repeat opcodes
                if (fdp.ConsumeBool()) {
                    content += "repeat ";
                    content += std::to_string(fdp.ConsumeIntegralInRange<int>(2, 10));
                    content += "\n";
                }
                break;
            }
            
            case 13: { // Hyphenation directives
                if (fdp.ConsumeBool()) {
                    content += "hyphenation\n";
                    content += "hyphenate ";
                    content += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 3));
                    content += " ";
                    content += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 3));
                    content += "\n";
                }
                break;
            }
            
            case 14: { // Invalid/edge case opcodes to test error paths
                if (fdp.ConsumeBool()) {
                    // Deliberately malformed opcodes to test compileRule error handling
                    content += fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(1, 8));
                    content += " ";
                    content += fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(1, 32));
                    content += "\n";
                }
                break;
            }
        }
    }
    
    return content;
}

// Main harness focusing on compileRule function through lou_compileString
int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - we need enough data for meaningful table generation
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
    
    // Step 1: Generate complex table content targeting compileRule paths
    std::string table_content;
    
    // Table header
    if (fdp.ConsumeBool()) {
        table_content += "table fuzz-compilerule-";
        table_content += std::to_string(fdp.ConsumeIntegral<uint32_t>());
        table_content += "\n";
    }
    
    if (fdp.ConsumeBool()) {
        table_content += "locale ";
        const char* locales[] = {"en_US", "fr_FR", "de_DE", "es_ES", "ja_JP"};
        const size_t num_locales = sizeof(locales) / sizeof(locales[0]);
        size_t locale_idx = fdp.ConsumeIntegralInRange<size_t>(0, num_locales - 1);
        table_content += locales[locale_idx];
        table_content += "\n";
    }
    
    // Add complex opcode sequences focusing on grouping rules and compileRule coverage
    table_content += generate_complex_opcode_sequence(fdp);
    
    // Ensure we have at least some content
    if (table_content.empty()) {
        table_content = "table test\nletter a 1\n";
    }
    
    // Step 2: Select target table for compilation
    // Use an existing table that lou_compileString can work with
    const char* table_name = "en-us-comp8.ctb";  // Existing table that can be compiled
    
    // Step 3: Call lou_compileString with generated table content
    // This will trigger: lou_compileString -> compileString -> compileRule -> compileGrouping
    int compile_result = lou_compileString(table_name, table_content.c_str());
    
    // We don't care about the result - we're interested in code path coverage
    // Both successful compilations and error paths in compileRule are valuable
    
    // Step 4: Cleanup - lou_free frees all allocated tables
    // Note: We don't call lou_free here to avoid affecting other harness runs
    // The fuzzer will handle cleanup between runs
    
    return 0;
}

} // namespace harness_017
