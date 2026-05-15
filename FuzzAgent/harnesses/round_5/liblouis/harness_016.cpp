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

namespace harness_016 {
/*
 * Fuzzing harness for liblouis targeting CTO_BackMatch opcode compilation
 * (backward matching rule compilation) based on coverage analysis recommendations.
 * 
 * Target: CTO_BackMatch opcode compilation identified as critical coverage gap
 *         with 526 blocked branches in compileRule function.
 * 
 * Specific focus areas:
 * 1. CTO_BackMatch opcode compilation (lines 3134-3183 in compileTranslationTable.c)
 * 2. Backward matching-specific pattern compilation with _lou_pattern_compile
 * 3. Pattern reversal logic with _lou_pattern_reverse (line 3162)
 * 4. Complex before/after pattern handling for backward matching
 * 5. Edge cases: empty patterns, single character patterns, wildcards
 * 6. Error handling paths in CTO_BackMatch_cleanup
 * 
 * Semantic differentiation from existing harnesses:
 * - harness_008: General translation rule compilation with basic backmatch rule
 * - harness_011: General compilation edge cases and error handling
 * - harness_014: Macro compilation features
 * - harness_016: Deep CTO_BackMatch compilation logic, pattern reversal,
 *                backward-specific pattern handling (current)
 * 
 * Primary API: lou_compileString (direct entry to compileRule)
 * 
 * Strategy: Generate diverse backward matching rule syntax focusing on:
 * 1. Complex before/after pattern combinations
 * 2. Pattern reversal edge cases
 * 3. Error scenarios (invalid patterns, insufficient pattern data)
 * 4. Boundary conditions for pattern compilation
 * 5. Special characters and wildcards in backward matching context
 */



// Log callback to avoid excessive logging during fuzzing
static void quiet_log(logLevels level, const char* msg) {
    (void)level;
    (void)msg;
}

// Global initialization flag
static int initialized = 0;

// Helper to generate pattern for backward matching (can include special chars)
static std::string generate_backmatch_pattern(FuzzedDataProvider& fdp, bool is_before_pattern) {
    std::string pattern;
    int pattern_length = fdp.ConsumeIntegralInRange<int>(0, 10);
    
    if (pattern_length == 0) {
        // Empty pattern (special case)
        return "";
    }
    
    for (int i = 0; i < pattern_length; i++) {
        uint8_t char_type = fdp.ConsumeIntegral<uint8_t>() % 6;
        switch (char_type) {
            case 0: // lowercase letter
                pattern += static_cast<char>('a' + (fdp.ConsumeIntegral<uint8_t>() % 26));
                break;
            case 1: // uppercase letter
                pattern += static_cast<char>('A' + (fdp.ConsumeIntegral<uint8_t>() % 26));
                break;
            case 2: // digit
                pattern += static_cast<char>('0' + (fdp.ConsumeIntegral<uint8_t>() % 10));
                break;
            case 3: // special character (valid in patterns)
                pattern += static_cast<char>('!' + (fdp.ConsumeIntegral<uint8_t>() % 15));
                break;
            case 4: // wildcard (*)
                if (fdp.ConsumeBool()) {
                    pattern += '*';
                } else {
                    pattern += static_cast<char>('a' + (fdp.ConsumeIntegral<uint8_t>() % 26));
                }
                break;
            case 5: // hyphen (special case in pattern compilation)
                if (fdp.ConsumeBool()) {
                    pattern += '-';
                } else {
                    pattern += static_cast<char>('a' + (fdp.ConsumeIntegral<uint8_t>() % 26));
                }
                break;
        }
    }
    
    return pattern;
}

// Generate a complete backward matching rule
static std::string generate_backmatch_rule(FuzzedDataProvider& fdp, int rule_type) {
    std::string rule;
    
    switch (rule_type % 8) {
        case 0: { // Basic backmatch with simple patterns
            std::string before = generate_backmatch_pattern(fdp, true);
            std::string after = generate_backmatch_pattern(fdp, false);
            
            rule = "backmatch ";
            if (before.empty()) {
                rule += "-";
            } else {
                rule += before;
            }
            rule += " ";
            rule += "test";  // Rule chars (simple)
            rule += " ";
            if (after.empty()) {
                rule += "-";
            } else {
                rule += after;
            }
            rule += " ";
            rule += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 1234567));  // Dots pattern
            break;
        }
            
        case 1: { // Backmatch with wildcard in before pattern
            rule = "backmatch ";
            if (fdp.ConsumeBool()) {
                rule += "*";
            } else {
                rule += "abc*";
            }
            rule += " ";
            rule += fdp.ConsumeRandomLengthString(5);
            rule += " ";
            rule += generate_backmatch_pattern(fdp, false);
            rule += " ";
            rule += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 99999));
            break;
        }
            
        case 2: { // Backmatch with wildcard in after pattern
            rule = "backmatch ";
            rule += generate_backmatch_pattern(fdp, true);
            rule += " ";
            rule += fdp.ConsumeRandomLengthString(3);
            rule += " ";
            if (fdp.ConsumeBool()) {
                rule += "*";
            } else {
                rule += "xyz*";
            }
            rule += " ";
            rule += std::to_string(fdp.ConsumeIntegralInRange<uint8_t>(0, 255));
            break;
        }
        case 3: { // Backmatch with complex patterns (long)
            rule = "backmatch ";
            rule += fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<int>(1, 20));
            rule += " ";
            rule += fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<int>(1, 10));
            rule += " ";
            rule += fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<int>(1, 20));
            rule += " ";
            rule += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 999999));
            break;
        }
            
        case 4: { // Backmatch with special characters
            rule = "backmatch ";
            // Pattern with special chars
            rule += "!@#$%";
            rule += " ";
            rule += "test";
            rule += " ";
            rule += "^&*()";
            rule += " ";
            rule += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 9999));
            break;
        }
            
        case 5: { // Backmatch with noback/nofor modifiers
            if (fdp.ConsumeBool()) {
                rule = "noback ";
            } else {
                rule = "nofor ";
            }
            rule += "backmatch ";
            rule += generate_backmatch_pattern(fdp, true);
            rule += " ";
            rule += fdp.ConsumeRandomLengthString(4);
            rule += " ";
            rule += generate_backmatch_pattern(fdp, false);
            rule += " ";
            rule += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 65535));
            break;
        }
            
        case 6: { // Backmatch with empty before pattern (hyphen)
            rule = "backmatch - ";
            rule += fdp.ConsumeRandomLengthString(6);
            rule += " ";
            rule += generate_backmatch_pattern(fdp, false);
            rule += " ";
            rule += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 999999));
            break;
        }
            
        case 7: { // Backmatch with empty after pattern (hyphen)
            rule = "backmatch ";
            rule += generate_backmatch_pattern(fdp, true);
            rule += " ";
            rule += fdp.ConsumeRandomLengthString(6);
            rule += " - ";
            rule += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 999999));
            break;
        }
    }
    
    return rule;
}

// Generate table with backward matching compilation scenarios
static std::string generate_backmatch_compilation_table(FuzzedDataProvider& fdp) {
    std::string table;
    
    // Start with table header
    table += "# Backward matching compilation test table\n";
    table += "table backmatch_test\n\n";
    
    // Add locale
    table += "locale en-us\n\n";
    
    // Add some basic character definitions first
    table += "# Basic character definitions\n";
    for (int i = 0; i < 3; i++) {
        table += "letter ";
        table += static_cast<char>('a' + i);
        table += " @ ";
        table += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 999));
        table += "\n";
    }
    table += "\n";
    
    // Add multiple backmatch rules with different scenarios
    int num_backmatch_rules = fdp.ConsumeIntegralInRange<int>(1, 10);
    table += "# Backward matching rules\n";
    
    for (int i = 0; i < num_backmatch_rules; i++) {
        std::string rule = generate_backmatch_rule(fdp, i);
        table += rule;
        table += "\n";
        
        // Occasionally add comments
        if (fdp.ConsumeBool() && fdp.remaining_bytes() > 5) {
            table += "# ";
            table += fdp.ConsumeRandomLengthString(20);
            table += "\n";
        }
    }
    
    // Add some regular rules for context
    table += "\n# Regular rules for context\n";
    int num_regular_rules = fdp.ConsumeIntegralInRange<int>(0, 5);
    for (int i = 0; i < num_regular_rules; i++) {
        table += "letter ";
        table += static_cast<char>('d' + i);
        table += " @ ";
        table += std::to_string(fdp.ConsumeIntegralInRange<int>(1000, 9999));
        table += "\n";
    }
    
    // Add match rules for comparison
    if (fdp.ConsumeBool()) {
        table += "\n# Forward match rules for comparison\n";
        table += "match abc test xyz 123456\n";
        table += "match * wildcard * 789\n";
    }
    
    return table;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size check - backmatch rules need meaningful data
    if (size < 100) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize liblouis once
    if (!initialized) {
        lou_registerLogCallback(quiet_log);
        initialized = 1;
    }
    
    // Set data path to point to tables directory
    std::string data_path = "/root/src/liblouis/tables";
    char* path_result = lou_setDataPath(data_path.c_str());
    (void)path_result; // Result may be NULL, that's okay
    
    // Generate table content with backward matching compilation scenarios
    std::string table_name = "backmatch_compilation_test.ctb";
    std::string table_content = generate_backmatch_compilation_table(fdp);
    
    // Compile the table - focus is on exercising CTO_BackMatch compilation paths
    int compilation_result = lou_compileString(table_name.c_str(), table_content.c_str());
    (void)compilation_result;
    
    // Clean up
    lou_free();
    
    return 0;
}

} // namespace harness_016
