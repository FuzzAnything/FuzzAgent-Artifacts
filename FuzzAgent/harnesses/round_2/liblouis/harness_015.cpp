#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <vector>
#include <string>
#include <algorithm>
#include <fuzzer/FuzzedDataProvider.h>
#include "liblouis.h"

namespace harness_015 {
// Targeted liblouis fuzzing harness for compileRule function with 568 blocked branches
// Harness_015: Specifically targets remaining blocked branches in compileRule function
// Focus: Semantic diversity from harness_012 - targets complex nested rules, macro expansions,
//        fall-through opcodes (CTO_Syllable, CTO_Always, etc.), error conditions, and display table interactions
// Strategy: Generate tables with diverse opcode combinations, nested macros, nocross/nofor/noback modifiers,
//           and test both valid and invalid rule syntax to explore error paths
// Differentiation: Goes deeper into macro expansion, fall-through opcodes, and error handling than harness_012




// Log callback to suppress output during fuzzing
static void fuzz_log_callback(logLevels level, const char* message) {
    (void)level;
    (void)message;
    // Suppress all logging during fuzzing
}

// Generate opcode from the fall-through group (CTO_Syllable through CTO_RepWord)
static std::string generate_fallthrough_opcode(FuzzedDataProvider& fdp) {
    const char* fallthrough_opcodes[] = {
        "syllable", "always", "largesign", "wholeword", "partword", "joinnum",
        "joinableword", "lowword", "suffixableword", "prefixableword", "begword",
        "begmidword", "midword", "midendword", "endword", "prepunc", "postpunc",
        "begnum", "midnum", "endnum", "repeated", "repword"
    };
    
    uint8_t idx = fdp.ConsumeIntegral<uint8_t>() % (sizeof(fallthrough_opcodes)/sizeof(fallthrough_opcodes[0]));
    return std::string(fallthrough_opcodes[idx]);
}

// Generate opcode with special handling (different from harness_012)
static std::string generate_special_opcode(FuzzedDataProvider& fdp) {
    const char* special_opcodes[] = {
        "locale", "uplow", "correct", "pass2", "pass3", "pass4", "context",
        "contraction", "nocont", "compbrl", "literal", "multind", "attribute",
        "base", "hyphen", "decpoint", "swapcc", "swapcd", "swapdd"
    };
    
    uint8_t idx = fdp.ConsumeIntegral<uint8_t>() % (sizeof(special_opcodes)/sizeof(special_opcodes[0]));
    return std::string(special_opcodes[idx]);
}

// Generate character pattern (for rule chars)
static std::string generate_char_pattern(FuzzedDataProvider& fdp, int max_len = 10) {
    std::string pattern;
    int length = fdp.ConsumeIntegralInRange<int>(1, max_len);
    
    for (int i = 0; i < length; i++) {
        uint8_t c = fdp.ConsumeIntegral<uint8_t>();
        // Convert to printable ASCII range (32-126)
        c = 32 + (c % 95);
        pattern += static_cast<char>(c);
    }
    
    return pattern;
}

// Generate dots pattern (0-255 values, possibly with hyphens)
static std::string generate_dots_pattern(FuzzedDataProvider& fdp, bool allow_empty = false) {
    if (allow_empty && fdp.ConsumeBool()) {
        return "";  // Empty dots pattern for testing "=" operand
    }
    
    std::string pattern;
    int length = fdp.ConsumeIntegralInRange<int>(1, 5);
    
    for (int i = 0; i < length; i++) {
        uint8_t dot_value = fdp.ConsumeIntegral<uint8_t>() % 256;
        pattern += std::to_string(dot_value);
        if (i < length - 1) {
            pattern += "-";
        }
    }
    
    return pattern;
}

// Generate macro definition with parameters
static std::string generate_macro_definition(FuzzedDataProvider& fdp) {
    std::string macro_def;
    
    // Macro name
    std::string macro_name = "macro";
    macro_name += std::to_string(fdp.ConsumeIntegral<uint8_t>() % 10);
    macro_def += "macro " + macro_name;
    
    // Parameters ($1, $2, etc.)
    int param_count = fdp.ConsumeIntegralInRange<int>(1, 3);
    for (int i = 0; i < param_count; i++) {
        macro_def += " $" + std::to_string(i + 1);
    }
    macro_def += "\n";
    
    // Macro body with rules
    int rule_count = fdp.ConsumeIntegralInRange<int>(1, 3);
    for (int i = 0; i < rule_count; i++) {
        if (fdp.remaining_bytes() < 10) break;
        
        // Simple rule in macro
        std::string rule_chars = generate_char_pattern(fdp, 3);
        std::string rule_dots = generate_dots_pattern(fdp, false);
        
        if (!rule_chars.empty() && !rule_dots.empty()) {
            macro_def += "letter " + rule_chars + " " + rule_dots + "\n";
        }
    }
    
    macro_def += "eom\n";
    return macro_def;
}

// Generate a rule with potential modifiers (nocross, nofor, noback)
static std::string generate_rule_with_modifiers(FuzzedDataProvider& fdp, const std::string& opcode) {
    std::string rule;
    
    // Add modifiers before the rule (testing modifier stacking)
    if (fdp.ConsumeBool()) {
        rule += "nocross\n";
    }
    if (fdp.ConsumeBool()) {
        rule += "nofor\n";
    }
    if (fdp.ConsumeBool()) {
        rule += "noback\n";
    }
    
    // Add the actual rule
    rule += opcode + " ";
    
    // For fall-through opcodes, need chars and dots
    std::string rule_chars = generate_char_pattern(fdp, 5);
    std::string rule_dots = generate_dots_pattern(fdp, true);  // Allow empty for "=" testing
    
    rule += rule_chars + " " + rule_dots + "\n";
    
    return rule;
}

// Generate complex table with error testing
static std::string generate_complex_table(FuzzedDataProvider& fdp) {
    std::string table;
    
    // Add locale (triggers warning path)
    if (fdp.ConsumeBool()) {
        table += "locale en_US.UTF-8\n";
    }
    
    // Add charset
    if (fdp.ConsumeBool()) {
        table += "charset UTF-8\n";
    }
    
    // Add macros (if enabled in build)
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 100) {
        table += generate_macro_definition(fdp);
    }
    
    // Add some basic character definitions first (for testing characters that are "not defined" error)
    if (fdp.ConsumeBool()) {
        // Generate random character definitions from fuzzer data
        for (int i = 0; i < 3 && fdp.remaining_bytes() > 5; i++) {
            std::string char_def = generate_char_pattern(fdp, 1);
            std::string dots_def = generate_dots_pattern(fdp, false);
            if (!char_def.empty() && !dots_def.empty()) {
                table += "letter " + char_def + " " + dots_def + "\n";
            }
        }
    }
    
    // Generate rules with fall-through opcodes
    int rule_count = fdp.ConsumeIntegralInRange<int>(1, 8);
    for (int i = 0; i < rule_count; i++) {
        if (fdp.remaining_bytes() < 20) break;
        
        uint8_t rule_type = fdp.ConsumeIntegral<uint8_t>() % 3;
        
        switch (rule_type) {
            case 0:
                // Fall-through opcode rule
                table += generate_rule_with_modifiers(fdp, generate_fallthrough_opcode(fdp));
                break;
            case 1:
                // Special opcode rule
                table += generate_rule_with_modifiers(fdp, generate_special_opcode(fdp));
                break;
            case 2:
                // Error-testing rule (potentially invalid)
                if (fdp.ConsumeBool()) {
                    // Rule with empty chars or dots
                    std::string opcode = generate_fallthrough_opcode(fdp);
                    table += opcode + " ";
                    
                    if (fdp.ConsumeBool()) {
                        table += "";  // Empty chars
                    } else {
                        table += generate_char_pattern(fdp, 3);
                    }
                    
                    table += " ";
                    
                    if (fdp.ConsumeBool()) {
                        table += "";  // Empty dots
                    } else {
                        table += generate_dots_pattern(fdp, false);
                    }
                    table += "\n";
                }
                break;
        }
        
        // Add blank line between rules sometimes
        if (i < rule_count - 1 && fdp.ConsumeBool()) {
            table += "\n";
        }
    }
    
    // Test duplicate modifier error paths
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 10) {
        table += "nocross\n";
        table += "nocross\n";  // Duplicate - should trigger error
        // Generate a random rule to follow the modifiers
        std::string error_char = generate_char_pattern(fdp, 1);
        std::string error_dots = generate_dots_pattern(fdp, false);
        if (!error_char.empty() && !error_dots.empty()) {
            table += "letter " + error_char + " " + error_dots + "\n";
        }
    }
    
    // Test nofor/noback conflict
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 10) {
        table += "nofor\n";
        table += "noback\n";  // Conflict - should trigger error
        // Generate a random rule to follow the modifiers
        std::string conflict_char = generate_char_pattern(fdp, 1);
        std::string conflict_dots = generate_dots_pattern(fdp, false);
        if (!conflict_char.empty() && !conflict_dots.empty()) {
            table += "letter " + conflict_char + " " + conflict_dots + "\n";
        }
    }
    
    return table;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for complex table generation
    if (size < 100) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize liblouis logging
    static int logging_initialized = 0;
    if (!logging_initialized) {
        lou_registerLogCallback(fuzz_log_callback);
        logging_initialized = 1;
    }
    
    // Select base table - use different tables than harness_012 for diversity
    const char* base_tables[] = {
        "en-us-comp6.ctb",
        "en-us-comp8.ctb", 
        "de-g0.utb",
        "sv-g1.ctb",
        "fr-bfu-g2.ctb",
        "es-g2.ctb",
        NULL
    };
    
    uint8_t table_idx = fdp.ConsumeIntegral<uint8_t>() % 6;
    const char* base_table = base_tables[table_idx];
    
    // Generate complex table compilation string
    std::string compile_string = generate_complex_table(fdp);
    
    // Try compilation - this will exercise compileRule with our generated rules
    // Note: Some rules may be invalid and cause compilation errors, which is fine
    // as we want to explore those error paths in compileRule
    int compile_result = lou_compileString(base_table, compile_string.c_str());
    
    // If compilation succeeded, test some operations with the table
    if (compile_result) {
        const void* table_ptr = lou_getTable(base_table);
        
        if (table_ptr && fdp.remaining_bytes() > 20) {
            // Test simple translation with the potentially modified table
            std::string test_text = fdp.ConsumeRandomLengthString(30);
            if (!test_text.empty()) {
                widechar* inbuf = (widechar*)malloc((test_text.length() + 1) * sizeof(widechar));
                widechar* outbuf = (widechar*)malloc((test_text.length() * 10 + 1) * sizeof(widechar));
                
                if (inbuf && outbuf) {
                    for (size_t i = 0; i < test_text.length(); i++) {
                        inbuf[i] = (widechar)(unsigned char)test_text[i];
                    }
                    inbuf[test_text.length()] = 0;
                    
                    int inlen = test_text.length();
                    int outlen = test_text.length() * 10;
                    int result = lou_translateString(base_table, inbuf, &inlen, outbuf, &outlen, NULL, NULL, 0);
                    (void)result; // Use result to avoid unused variable warning
                }
                
                free(inbuf);
                free(outbuf);
            }
        }
    }
    
    // Clean up - lou_free will handle resources from lou_compileString
    // Don't call lou_free() here as it would affect other harnesses running in same process
    
    return 0;
}

} // namespace harness_015
