#include <fuzzer/FuzzedDataProvider.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <vector>
#include <string>
#include <memory>
#include <liblouis.h>

namespace harness_023 {
/*
 * Fuzzing harness for liblouis targeting the compileRule function with 527 blocked branches
 * in compileTranslationTable.c (line 2879). This harness focuses on translation table
 * compilation aspects that previous harnesses may have missed, specifically targeting
 * complex rule compilation scenarios to exercise the many conditional branches in
 * compileRule.
 * 
 * Target: compileRule function - top coverage blocker with 527 blocked branches (57.9% of reachable)
 * 
 * Strategy:
 * 1. Generate diverse translation table rule definitions to exercise different opcode paths
 * 2. Focus on complex rule combinations that trigger multiple conditional branches
 * 3. Include macro definitions and expansions (ENABLE_MACROS feature)
 * 4. Test various character class definitions and attribute combinations
 * 5. Include hyphenation rules and locale specifications
 * 6. Test multi-pass operations (pass2, pass3, pass4)
 * 7. Include context, correct, and match operations
 * 
 * Primary API: lou_compileString (direct entry to compileRule via table compilation)
 * 
 * This harness is semantically unique from previous harnesses by:
 * - Focusing specifically on the compileRule function's 527 blocked branches
 * - Generating more complex rule combinations and nesting
 * - Testing macro expansion and substitution paths
 * - Including hyphenation compilation paths
 * - Testing character class attribute combinations
 */



// Log callback to avoid excessive logging during fuzzing
static void quiet_log(logLevels level, const char* msg) {
    (void)level;
    (void)msg;
}

// Global initialization flag
static int initialized = 0;

// Helper to generate a random character (ASCII printable)
static char generate_random_char(FuzzedDataProvider& fdp) {
    // Generate printable ASCII characters (32-126)
    return static_cast<char>(fdp.ConsumeIntegralInRange<uint8_t>(32, 126));
}

// Helper to generate a random string of specified length
static std::string generate_random_string(FuzzedDataProvider& fdp, size_t min_len, size_t max_len) {
    size_t length = fdp.ConsumeIntegralInRange<size_t>(min_len, max_len);
    std::string result;
    result.reserve(length);
    
    for (size_t i = 0; i < length; i++) {
        result.push_back(generate_random_char(fdp));
    }
    
    return result;
}

// Helper to generate a dot pattern (braille cell representation)
static std::string generate_dot_pattern(FuzzedDataProvider& fdp) {
    std::string pattern;
    int num_dots = fdp.ConsumeIntegralInRange<int>(1, 6);
    
    for (int i = 0; i < num_dots; i++) {
        if (i > 0 && fdp.ConsumeBool()) {
            pattern += '-';
        }
        pattern += static_cast<char>('1' + (fdp.ConsumeIntegral<uint8_t>() % 6));
    }
    
    return pattern;
}

// Helper to generate a character class name
static std::string generate_character_class(FuzzedDataProvider& fdp) {
    const char* classes[] = {
        "space", "letter", "digit", "punctuation", "uppercase", 
        "lowercase", "math", "sign", "litdigit"
    };
    size_t index = fdp.ConsumeIntegralInRange<size_t>(0, 8);
    return std::string(classes[index]);
}

// Helper to generate an opcode name
static std::string generate_opcode(FuzzedDataProvider& fdp) {
    // Sample of opcodes that compileRule handles
    const char* opcodes[] = {
        "include", "locale", "undefined", "capsletter", "begcapsword",
        "endcapsword", "begcaps", "endcaps", "begcapsphrase", "endcapsphrase",
        "lencapsphrase", "modeletter", "begmodeword", "endmodeword", "begmode",
        "endmode", "begmodephrase", "endmodephrase", "lenmodephrase", "letsign",
        "noletsignbefore", "noletsign", "noletsignafter", "numsign", "nonumsign",
        "numericmodechars", "midendnumericmodechars", "numericnocontchars",
        "seqdelimiter", "seqbeforechars", "seqafterchars", "seqafterpattern",
        "seqafterexpression", "emphclass", "emphletter", "begemphword",
        "endemphword", "begemph", "endemph", "begemphphrase", "endemphphrase",
        "lenemphphrase", "swapcd", "swapdd", "swapcc", "swapcd", "swap",
        "pass2", "pass3", "pass4", "context", "correct", "match",
        "syllable", "always", "largesign", "wholeword", "partword",
        "joinnum", "joinableword", "display", "grouping", "comp6",
        "compbrl", "noback", "nofor", "nocross", "uplow", "none"
    };
    size_t index = fdp.ConsumeIntegralInRange<size_t>(0, sizeof(opcodes)/sizeof(opcodes[0]) - 1);
    return std::string(opcodes[index]);
}

// Generate a complete translation table rule
static std::string generate_rule(FuzzedDataProvider& fdp) {
    std::string rule;
    
    // Choose rule type
    uint8_t rule_type = fdp.ConsumeIntegral<uint8_t>() % 10;
    
    switch (rule_type) {
        case 0: // Character definition
            rule += generate_character_class(fdp);
            rule += " ";
            rule += generate_random_string(fdp, 1, 3);
            rule += " ";
            rule += generate_dot_pattern(fdp);
            break;
            
        case 1: // Translation rule
            rule += generate_random_string(fdp, 1, 5);
            rule += " ";
            rule += generate_dot_pattern(fdp);
            break;
            
        case 2: // Opcode with parameters
            rule += generate_opcode(fdp);
            rule += " ";
            rule += generate_random_string(fdp, 1, 8);
            break;
            
        case 3: // Macro definition (if ENABLE_MACROS)
            rule += "macro ";
            rule += generate_random_string(fdp, 1, 10);
            rule += " ";
            rule += generate_random_string(fdp, 5, 30);
            rule += " eom";
            break;
            
        case 4: // Include directive
            rule += "include ";
            rule += generate_random_string(fdp, 1, 20);
            break;
            
        case 5: // Display rule
            rule += "display ";
            rule += generate_random_string(fdp, 1, 3);
            rule += " ";
            rule += generate_dot_pattern(fdp);
            break;
            
        case 6: // Hyphenation rule
            rule += "hyphen ";
            rule += generate_random_string(fdp, 1, 5);
            rule += " ";
            rule += generate_dot_pattern(fdp);
            break;
            
        case 7: // Context rule
            rule += "context ";
            rule += generate_random_string(fdp, 1, 5);
            rule += " ";
            rule += generate_random_string(fdp, 1, 3);
            rule += " ";
            rule += generate_dot_pattern(fdp);
            break;
            
        case 8: // Match rule
            rule += "match ";
            rule += generate_random_string(fdp, 1, 5);
            rule += " ";
            rule += generate_dot_pattern(fdp);
            break;
            
        case 9: // Complex rule with multiple parts
            rule += generate_random_string(fdp, 1, 4);
            rule += " ";
            rule += generate_dot_pattern(fdp);
            if (fdp.ConsumeBool()) {
                rule += " noback";
            }
            if (fdp.ConsumeBool()) {
                rule += " nofor";
            }
            if (fdp.ConsumeBool()) {
                rule += " nocross";
            }
            break;
    }
    
    return rule;
}

// Generate a complete translation table with multiple rules
static std::string generate_translation_table(FuzzedDataProvider& fdp) {
    std::string table;
    
    // Add locale or encoding if specified
    if (fdp.ConsumeBool()) {
        if (fdp.ConsumeBool()) {
            table += "locale ";
            table += generate_random_string(fdp, 1, 20);
        } else {
            table += "UTF-8";
        }
        table += "\n";
    }
    
    // Add undefined directive if specified
    if (fdp.ConsumeBool()) {
        table += "undefined\n";
    }
    
    // Add uplow or none if specified
    if (fdp.ConsumeBool()) {
        if (fdp.ConsumeBool()) {
            table += "uplow\n";
        } else {
            table += "none\n";
        }
    }
    
    // Generate multiple rules
    int num_rules = fdp.ConsumeIntegralInRange<int>(1, 20);
    for (int i = 0; i < num_rules; i++) {
        if (fdp.remaining_bytes() < 10) {
            break;
        }
        
        table += generate_rule(fdp);
        table += "\n";
    }
    
    return table;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size check
    if (size < 16) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Initialize liblouis once
    if (!initialized) {
        lou_registerLogCallback(quiet_log);
        initialized = 1;
    }

    // Generate translation table definition
    std::string table_def = generate_translation_table(fdp);
    
    if (table_def.empty()) {
        return 0;
    }

    // Create a unique table name for each run
    std::string table_name = "fuzz_table_";
    table_name += generate_random_string(fdp, 1, 10);
    
    // Compile the generated table definition
    const char* table_list = table_name.c_str();
    const char* table_string = table_def.c_str();
    
    int compile_result = lou_compileString(table_list, table_string);

    // Clean up any allocated resources
    lou_free();

    return 0;
}

} // namespace harness_023
