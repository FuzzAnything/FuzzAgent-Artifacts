#include <fuzzer/FuzzedDataProvider.h>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>
#include <algorithm>
#include "liblouis.h"

namespace harness_019 {
// liblouis compileRule deep complexity harness - targeting complex pattern matching and macro expansion
// Target: compileRule function (580 blocked branches) focusing on complex translation table 
//         compilation scenarios, especially pattern matching (CTO_Match, CTO_BackMatch) and
//         macro expansion paths that likely have many branching decisions
// Primary API chain: lou_compileString -> compileString -> compileRule -> complex opcode handling
// Based on coverage analysis guidance to improve branch coverage from current 62.07%
// Semantic differentiation from harness_017 (grouping rules), harness_008 (general opcode diversity),
// harness_010 (pattern expressions): Focuses specifically on CTO_Match/CTO_BackMatch opcodes
// and macro expansion logic which have complex internal branching


// Include liblouis public header
extern "C" {
}

// Log callback to suppress logs during fuzzing
static void avoid_log_callback(logLevels level, const char *msg) {
    (void)level;
    (void)msg;
    // Suppress all logs during fuzzing
}

// Helper to generate match pattern syntax for CTO_Match and CTO_BackMatch opcodes
// These opcodes have complex pattern compilation logic with before/after patterns
static std::string generate_match_pattern(FuzzedDataProvider& fdp, bool is_backmatch = false) {
    std::string pattern;
    
    // Match opcode
    if (is_backmatch) {
        pattern += "backmatch ";
    } else {
        pattern += "match ";
    }
    
    // Generate before pattern (could be "-" for empty or complex pattern)
    uint8_t before_type = fdp.ConsumeIntegral<uint8_t>() % 3;
    if (before_type == 0) {
        pattern += "- ";  // Empty before pattern
    } else if (before_type == 1) {
        // Simple character pattern
        pattern += fdp.ConsumeIntegralInRange<char>('a', 'z');
        pattern += " ";
    } else {
        // Complex pattern with quantifiers or character class
        pattern += "[";
        pattern += fdp.ConsumeIntegralInRange<char>('a', 'z');
        pattern += "-";
        pattern += fdp.ConsumeIntegralInRange<char>('f', 'z');
        pattern += "]* ";
    }
    
    // Generate rule characters (text to match)
    size_t rule_len = fdp.ConsumeIntegralInRange<size_t>(1, 5);
    for (size_t i = 0; i < rule_len; i++) {
        pattern += fdp.ConsumeIntegralInRange<char>('a', 'z');
    }
    pattern += " ";
    
    // Generate after pattern (similar to before)
    uint8_t after_type = fdp.ConsumeIntegral<uint8_t>() % 3;
    if (after_type == 0) {
        pattern += "- ";  // Empty after pattern
    } else if (after_type == 1) {
        // Simple character pattern
        pattern += fdp.ConsumeIntegralInRange<char>('A', 'Z');
        pattern += " ";
    } else {
        // Complex pattern
        pattern += "(";
        pattern += fdp.ConsumeIntegralInRange<char>('0', '9');
        pattern += "+) ";
    }
    
    // Generate dots pattern (braille cells)
    size_t dots_len = fdp.ConsumeIntegralInRange<size_t>(1, 3);
    for (size_t i = 0; i < dots_len; i++) {
        int dot_count = fdp.ConsumeIntegralInRange<int>(1, 3);
        for (int d = 0; d < dot_count; d++) {
            pattern += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 8));
        }
        if (i < dots_len - 1) pattern += ",";
    }
    
    return pattern + "\n";
}

// Helper to generate macro definition and expansion (complex branching in compileRule)
// Macros involve argument substitution and multi-line rule expansion
static std::string generate_macro_sequence(FuzzedDataProvider& fdp) {
    std::string content;
    
    // Only generate macros if we have enough input
    if (fdp.remaining_bytes() < 50) return "";
    
    // Macro definition
    std::string macro_name = "m";
    macro_name += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 9));
    content += "macro " + macro_name + " eom\n";
    
    // Generate macro content with potential argument references $1, $2, etc.
    size_t macro_lines = fdp.ConsumeIntegralInRange<size_t>(1, 4);
    for (size_t i = 0; i < macro_lines; i++) {
        uint8_t line_type = fdp.ConsumeIntegral<uint8_t>() % 3;
        
        if (line_type == 0) {
            // Simple character definition with argument reference
            content += "space $";
            content += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 3));
            content += "\n";
        } else if (line_type == 1) {
            // Match rule with arguments
            content += "match $1 abc $2 123\n";
        } else {
            // Plain text
            content += fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(1, 20));
            content += "\n";
        }
    }
    
    content += "eom\n";
    
    // Use the macro with arguments
    content += macro_name + " ";
    
    // Generate arguments for macro call
    size_t num_args = fdp.ConsumeIntegralInRange<size_t>(1, 3);
    for (size_t i = 0; i < num_args; i++) {
        if (i > 0) content += " ";
        uint8_t arg_type = fdp.ConsumeIntegral<uint8_t>() % 2;
        if (arg_type == 0) {
            // Character argument
            content += fdp.ConsumeIntegralInRange<char>('a', 'z');
        } else {
            // Hex escape argument
            content += "\\x";
            char hex[3];
            snprintf(hex, sizeof(hex), "%02x", fdp.ConsumeIntegralInRange<uint8_t>(0x20, 0x7e));
            content += hex;
        }
    }
    content += "\n";
    
    return content;
}

// Helper to generate emphasis-related opcodes which have complex attribute handling
static std::string generate_emphasis_opcodes(FuzzedDataProvider& fdp) {
    std::string content;
    
    const char* emph_opcodes[] = {
        "emphclass", "empletter", "begemphword", "endemphword",
        "begemph", "endemph", "begemphphrase", "endemphphrase",
        "lenemphphrase", "emphmodechars", "noemphchars"
    };
    
    size_t num_emph = fdp.ConsumeIntegralInRange<size_t>(1, 4);
    
    for (size_t i = 0; i < num_emph; i++) {
        uint8_t opcode_idx = fdp.ConsumeIntegral<uint8_t>() % (sizeof(emph_opcodes)/sizeof(emph_opcodes[0]));
        content += emph_opcodes[opcode_idx];
        content += " ";
        
        // Generate parameters based on opcode type
        if (opcode_idx < 2) { // emphclass, empletter
            // Single character or class name
            content += fdp.ConsumeIntegralInRange<char>('a', 'z');
            content += " ";
            content += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 255));
        } else if (opcode_idx < 8) { // beg/end emph word/phrase
            // Pattern
            size_t pattern_len = fdp.ConsumeIntegralInRange<size_t>(1, 5);
            for (size_t j = 0; j < pattern_len; j++) {
                content += fdp.ConsumeIntegralInRange<char>('a', 'z');
            }
        } else { // lenemphphrase, emphmodechars, noemphchars
            // Numeric value or character list
            content += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 100));
            if (fdp.ConsumeBool()) {
                content += " ";
                content += fdp.ConsumeIntegralInRange<char>('a', 'z');
            }
        }
        
        content += "\n";
    }
    
    return content;
}

// Helper to generate complex table with focus on compileRule's most branching paths
static std::string generate_complex_compileRule_table(FuzzedDataProvider& fdp) {
    std::string content;
    
    // Optional encoding line (affects line 1 handling in compileRule)
    if (fdp.ConsumeBool()) {
        uint8_t encoding_choice = fdp.ConsumeIntegral<uint8_t>() % 3;
        if (encoding_choice == 0) {
            content += "ISO\n";
        } else if (encoding_choice == 1) {
            content += "UTF-8\n";
        }
    }
    
    // Table metadata
    if (fdp.ConsumeBool()) {
        content += "table complex-compile-rule-test\n";
    }
    
    if (fdp.ConsumeBool()) {
        content += "locale ";
        content += fdp.ConsumeRandomLengthString(10);
        content += "\n";
    }
    
    // Generate mix of complex opcode sequences
    size_t num_sections = fdp.ConsumeIntegralInRange<size_t>(3, 10);
    
    for (size_t section = 0; section < num_sections && fdp.remaining_bytes() > 20; section++) {
        uint8_t section_type = fdp.ConsumeIntegral<uint8_t>() % 9;
        
        switch (section_type) {
            case 0: { // Match patterns (complex pattern compilation)
                content += generate_match_pattern(fdp, false);
                if (fdp.ConsumeBool()) {
                    content += generate_match_pattern(fdp, true); // backmatch
                }
                break;
            }
                
            case 1: { // Macro sequences (argument substitution)
                content += generate_macro_sequence(fdp);
                break;
            }
                
            case 2: { // Emphasis opcodes (complex attribute handling)
                content += generate_emphasis_opcodes(fdp);
                break;
            }
                
            case 3: { // Character class definitions with noback/nofor
                if (fdp.ConsumeBool()) content += "noback\n";
                if (fdp.ConsumeBool()) content += "nofor\n";
                content += "space ";
                content += std::to_string(fdp.ConsumeIntegralInRange<int>(32, 126));
                content += "\n";
                break;
            }
                
            case 4: { // Include directive (file handling path)
                content += "include ";
                content += fdp.ConsumeRandomLengthString(20);
                content += "\n";
                break;
            }
            case 5: { // Contraction rules
                content += "contraction ";
                size_t chars_len = fdp.ConsumeIntegralInRange<size_t>(2, 5);
                for (size_t i = 0; i < chars_len; i++) {
                    content += fdp.ConsumeIntegralInRange<char>('a', 'z');
                }
                content += " ";
                size_t dots_len = fdp.ConsumeIntegralInRange<size_t>(1, 3);
                for (size_t i = 0; i < dots_len; i++) {
                    int dot_count = fdp.ConsumeIntegralInRange<int>(1, 3);
                    for (int d = 0; d < dot_count; d++) {
                        content += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 8));
                    }
                    if (i < dots_len - 1) content += ",";
                }
                content += "\n";
                break;
            }
                
            case 6: { // Swap opcodes (character swapping)
                content += "swapcc ";
                content += fdp.ConsumeIntegralInRange<char>('a', 'z');
                content += " ";
                content += fdp.ConsumeIntegralInRange<char>('A', 'Z');
                content += "\n";
                break;
            }
                
            case 7: { // NoCont and related opcodes
                content += "nocont ";
                content += fdp.ConsumeIntegralInRange<char>('a', 'z');
                content += "\n";
                break;
            }
                
            case 8: { // Literal and compbrl
                content += "literal ";
                content += fdp.ConsumeIntegralInRange<char>('a', 'z');
                content += " ";
                content += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 8));
                content += "\n";
                break;
            }
                
            case 9: { // Correct and context opcodes
                content += "correct ";
                content += fdp.ConsumeIntegralInRange<char>('a', 'z');
                content += " ";
                content += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 8));
                content += "\n";
                break;
            }
        }
    }
    
    return content;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need substantial input for complex table generation
    if (size < 100) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize liblouis once
    static bool initialized = false;
    if (!initialized) {
        lou_registerLogCallback(avoid_log_callback);
        initialized = true;
    }
    
    // Generate complex table string targeting compileRule's branching paths
    std::string table_string = generate_complex_compileRule_table(fdp);
    
    if (table_string.empty() || fdp.remaining_bytes() < 10) {
        return 0;
    }
    
    // Use a dummy table list - the actual compilation happens on the string
    const char* table_list = "test-table";
    
    // Call lou_compileString to exercise compileRule with our complex input
    // This will trigger compileString -> compileRule -> various opcode handlers
    int compile_result = lou_compileString(table_list, table_string.c_str());
    
    // Compilation may succeed or fail - both are valid outcomes for fuzzing
    // The goal is to exercise the branching paths within compileRule
    (void)compile_result; // Result intentionally unused
    
    // If compilation succeeded, we should clean up
    // Note: lou_compileString doesn't allocate persistent tables that need freeing
    // It compiles into temporary structures that are cleaned up automatically
    
    return 0;
}

} // namespace harness_019
