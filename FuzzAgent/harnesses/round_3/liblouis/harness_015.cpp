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

namespace harness_015 {
// liblouis fuzzing harness - Macro compilation and grouping opcodes focus
// Targets: lou_compileString and compileRule function's macro compilation
// Specifically targets CTO_Macro (macro) and CTO_Grouping opcodes with 536 blocked branches
// Focus on macro definitions, macro calls, grouping rules, and other unexplored opcodes
// Differentiates from harness_014 by focusing on macro compilation scenarios


// Global initialization flag
static int initialized = 0;

// Log callback to suppress output during fuzzing
void avoid_log(logLevels level, const char *msg) {
    (void)level;
    (void)msg;
}

// Cleanup function
static void free_resources(void) {
    lou_free();
}

// External declaration for internal function
extern "C" {
    extern int _lou_compileTranslationRule(const char *tableList, const char *inString);
}

// Opcode definitions specifically targeting macro and grouping compilation
// Focus on opcodes mentioned in coverage guidance: macro, grouping, letter, upLow, etc.
const char* macro_opcodes[] = {
    // Macro-related opcodes (primary target)
    "macro",        // CTO_Macro - major coverage gap (536 blocked branches)
    "grouping",     // CTO_Grouping - specifically mentioned in guidance
    
    // Character definition opcodes that work well with macros
    "letter",       // CTO_Letter - mentioned in guidance
    "uppercase",
    "lowercase",
    "space",
    "digit",
    "punctuation",
    
    // Other opcodes that can be used in macro definitions
    "replace",
    "correct",
    "match",
    "class",
    
    // Control opcodes
    "noback",
    "nofor",
    "nocross",
    
    // Word position opcodes
    "begword",
    "midword",
    "endword",
    
    // Special opcodes mentioned in guidance
    "uplow",        // Deprecated but mentioned in guidance
    
    // Display and emphasis opcodes
    "display",
    "capsletter",
    "begcaps",
    "endcaps",
};

const size_t NUM_MACRO_OPCODES = sizeof(macro_opcodes) / sizeof(macro_opcodes[0]);

// Helper to generate random character sequence for opcode parameters
static std::string generate_char_sequence(FuzzedDataProvider& fdp, bool for_dots = false) {
    // Generate 1-4 characters
    int length = fdp.ConsumeIntegralInRange<int>(1, 4);
    std::string result;
    
    for (int i = 0; i < length; i++) {
        if (for_dots) {
            // For braille dots (1-8)
            int dot = fdp.ConsumeIntegralInRange<int>(1, 8);
            result += '0' + dot;
        } else {
            // For regular characters - using alphanumeric for macro names
            char c;
            uint8_t choice = fdp.ConsumeIntegral<uint8_t>() % 3;
            switch (choice) {
                case 0:
                    c = fdp.ConsumeIntegralInRange<char>('a', 'z');
                    break;
                case 1:
                    c = fdp.ConsumeIntegralInRange<char>('A', 'Z');
                    break;
                case 2:
                    c = fdp.ConsumeIntegralInRange<char>('0', '9');
                    break;
                default:
                    c = 'x';
            }
            result += c;
        }
    }
    
    return result;
}

// Helper to generate macro definition
static std::string generate_macro_definition(FuzzedDataProvider& fdp) {
    std::string macro_def;
    
    // Start with "macro" opcode
    macro_def += "macro ";
    
    // Generate macro name (must be alphanumeric)
    std::string macro_name;
    int name_len = fdp.ConsumeIntegralInRange<int>(1, 8);
    for (int i = 0; i < name_len; i++) {
        char c;
        uint8_t choice = fdp.ConsumeIntegral<uint8_t>() % 2;
        switch (choice) {
            case 0:
                c = fdp.ConsumeIntegralInRange<char>('a', 'z');
                break;
            case 1:
                c = fdp.ConsumeIntegralInRange<char>('A', 'Z');
                break;
            default:
                c = 'm';
        }
        macro_name += c;
    }
    macro_def += macro_name + " ";
    
    // Generate macro body with 1-3 rules
    int num_rules = fdp.ConsumeIntegralInRange<int>(1, 3);
    for (int i = 0; i < num_rules; i++) {
        if (fdp.remaining_bytes() < 10) break;
        
        // Select an opcode for macro body (not "macro" or "eom")
        size_t opcode_idx = fdp.ConsumeIntegralInRange<size_t>(2, NUM_MACRO_OPCODES - 1);
        const char* opcode = macro_opcodes[opcode_idx];
        
        macro_def += opcode;
        macro_def += " ";
        
        // Generate appropriate parameters based on opcode type
        if (strcmp(opcode, "letter") == 0 || strcmp(opcode, "uppercase") == 0 || 
            strcmp(opcode, "lowercase") == 0 || strcmp(opcode, "space") == 0 ||
            strcmp(opcode, "digit") == 0 || strcmp(opcode, "punctuation") == 0) {
            // Character definition: char dots
            macro_def += generate_char_sequence(fdp, false);
            macro_def += " ";
            macro_def += generate_char_sequence(fdp, true);
        }
        else if (strcmp(opcode, "replace") == 0 || strcmp(opcode, "correct") == 0) {
            // Replace/correct: chars dots
            macro_def += generate_char_sequence(fdp, false);
            macro_def += " ";
            macro_def += generate_char_sequence(fdp, true);
        }
        else if (strcmp(opcode, "match") == 0) {
            // Match: pattern chars
            macro_def += generate_char_sequence(fdp, false);
            macro_def += " ";
            macro_def += generate_char_sequence(fdp, false);
        }
        else if (strcmp(opcode, "grouping") == 0) {
            // Grouping: chars
            macro_def += generate_char_sequence(fdp, false);
        }
        else if (strcmp(opcode, "class") == 0) {
            // Class: name chars
            macro_def += generate_char_sequence(fdp, false);
        }
        else {
            // Default: simple char dots
            macro_def += generate_char_sequence(fdp, false);
            macro_def += " ";
            macro_def += generate_char_sequence(fdp, true);
        }
        
        if (i < num_rules - 1) {
            macro_def += "\n";
        }
    }
    
    // End macro with "eom"
    macro_def += "\neom";
    
    return macro_def;
}

// Helper to generate macro call
static std::string generate_macro_call(FuzzedDataProvider& fdp, const std::string& macro_name) {
    std::string call = macro_name + " ";
    
    // Generate 1-3 arguments for macro call
    int num_args = fdp.ConsumeIntegralInRange<int>(1, 3);
    for (int i = 0; i < num_args; i++) {
        call += generate_char_sequence(fdp, false);
        if (i < num_args - 1) {
            call += " ";
        }
    }
    
    return call;
}

// Helper to generate grouping rule
static std::string generate_grouping_rule(FuzzedDataProvider& fdp) {
    std::string rule = "grouping ";
    
    // Generate grouping characters (1-4 chars)
    int group_len = fdp.ConsumeIntegralInRange<int>(1, 4);
    for (int i = 0; i < group_len; i++) {
        char c = fdp.ConsumeIntegralInRange<char>('a', 'z');
        rule += c;
    }
    
    return rule;
}

// Helper to generate translation table with macro compilation scenarios
static std::string generate_macro_table(FuzzedDataProvider& fdp) {
    std::string table_content;
    
    // Determine table structure based on fuzzer input
    uint8_t table_type = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    switch (table_type) {
        case 0: {
            // Simple macro definition only
            table_content = generate_macro_definition(fdp);
            break;
        }
        
        case 1: {
            // Macro definition followed by macro call
            std::string macro_def = generate_macro_definition(fdp);
            table_content = macro_def + "\n";
            
            // Extract macro name from definition (first word after "macro")
            size_t macro_start = macro_def.find("macro ");
            if (macro_start != std::string::npos) {
                size_t name_start = macro_start + 6;
                size_t name_end = macro_def.find(' ', name_start);
                if (name_end != std::string::npos) {
                    std::string macro_name = macro_def.substr(name_start, name_end - name_start);
                    table_content += generate_macro_call(fdp, macro_name);
                }
            }
            break;
        }
        
        case 2: {
            // Grouping rule with other opcodes
            table_content = generate_grouping_rule(fdp) + "\n";
            
            // Add 1-2 additional rules
            int num_rules = fdp.ConsumeIntegralInRange<int>(1, 2);
            for (int i = 0; i < num_rules; i++) {
                if (fdp.remaining_bytes() < 10) break;
                
                size_t opcode_idx = fdp.ConsumeIntegralInRange<size_t>(2, NUM_MACRO_OPCODES - 1);
                const char* opcode = macro_opcodes[opcode_idx];
                
                table_content += opcode + std::string(" ") + 
                                 generate_char_sequence(fdp, false) + " " +
                                 generate_char_sequence(fdp, true);
                
                if (i < num_rules - 1) {
                    table_content += "\n";
                }
            }
            break;
        }
        
        case 3: {
            // Complex scenario: multiple macros with calls and grouping
            // Define first macro
            std::string macro1_def = generate_macro_definition(fdp);
            table_content = macro1_def + "\n";
            
            // Extract first macro name
            std::string macro1_name;
            size_t macro_start = macro1_def.find("macro ");
            if (macro_start != std::string::npos) {
                size_t name_start = macro_start + 6;
                size_t name_end = macro1_def.find(' ', name_start);
                if (name_end != std::string::npos) {
                    macro1_name = macro1_def.substr(name_start, name_end - name_start);
                }
            }
            
            // Add grouping rule
            table_content += generate_grouping_rule(fdp) + "\n";
            
            // Add call to first macro
            if (!macro1_name.empty()) {
                table_content += generate_macro_call(fdp, macro1_name) + "\n";
            }
            
            // Add regular opcode rule
            size_t opcode_idx = fdp.ConsumeIntegralInRange<size_t>(2, NUM_MACRO_OPCODES - 1);
            const char* opcode = macro_opcodes[opcode_idx];
            table_content += opcode + std::string(" ") + 
                             generate_char_sequence(fdp, false) + " " +
                             generate_char_sequence(fdp, true);
            break;
        }
    }
    
    return table_content;
}

// Select a test table
static std::string select_test_table(FuzzedDataProvider& fdp) {
    // Tables that should be available for compilation testing
    const char* test_tables[] = {
        "en-us-g1.ctb",  // Basic English grade 1
        "en-us-g2.ctb",  // English grade 2
        "tests/tables/empty.ctb",  // Empty table
        "tests/tables/large.ctb",  // Large table
    };
    
    size_t table_index = fdp.ConsumeIntegralInRange<size_t>(0, 
        sizeof(test_tables)/sizeof(test_tables[0]) - 1);
    return std::string(test_tables[table_index]);
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Minimum size check - need enough data for macro generation
    if (size < 64) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize liblouis once
    if (!initialized) {
        lou_registerLogCallback(avoid_log);
        initialized = 1;
        // Register cleanup
        atexit(free_resources);
    }
    
    // Set log level (as mentioned in guidance)
    lou_setLogLevel(LOU_LOG_WARN);
    
    // Select table for compilation
    std::string table_name = select_test_table(fdp);
    
    // Generate macro compilation scenario
    std::string table_content = generate_macro_table(fdp);
    
    if (table_content.empty()) {
        return 0;
    }
    
    // Test compilation with lou_compileString (primary target from guidance)
    int result = lou_compileString(table_name.c_str(), table_content.c_str());
    (void)result; // Result ignored for fuzzing
    
    // If enough bytes remain, also test with internal API
    if (fdp.remaining_bytes() > 32) {
        // Generate another macro scenario for internal API test
        std::string another_table = generate_macro_table(fdp);
        if (!another_table.empty()) {
            result = _lou_compileTranslationRule(table_name.c_str(), another_table.c_str());
            (void)result;
        }
    }
    
    // Try to validate the compiled table if possible
    if (fdp.remaining_bytes() > 16) {
        // Note: lou_checkTable is mentioned in guidance but may not be exported
        // We'll attempt to call it if the symbol exists
    }
    
    return 0;
}

} // namespace harness_015
