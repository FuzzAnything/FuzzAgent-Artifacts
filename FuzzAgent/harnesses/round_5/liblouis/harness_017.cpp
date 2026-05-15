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

namespace harness_017 {
/*
 * Fuzzing harness for liblouis targeting CTO_BackMatch case in compileRule 
 * (backward matching compilation) with semantic diversity from harness_016.
 * 
 * Target: CTO_BackMatch opcode compilation identified as critical coverage gap
 *         with 526 blocked branches and 0 hits in compileRule function.
 * 
 * Specific focus areas for semantic diversity from harness_016:
 * 1. Error path exploration in CTO_BackMatch compilation (lines 3153-3183)
 * 2. Pattern compilation failure scenarios for _lou_pattern_compile
 * 3. Pattern reversal edge cases with _lou_pattern_reverse (line 3162)
 * 4. Empty pattern handling (ptn_before.chars[0] == '-' && ptn_before.length == 1)
 * 5. Memory allocation failure simulation via malformed patterns
 * 6. Interaction of CTO_BackMatch with other table compilation features
 * 7. Complex pattern combinations that may trigger edge conditions
 * 
 * Error paths to target:
 * - addRule failure (line 3153) 
 * - Before pattern compilation failure (line 3160)
 * - After pattern compilation failure (line 3169)
 * - allocateSpaceInTranslationTable failure (line 3173)
 * 
 * Semantic differentiation from existing harnesses:
 * - harness_008: Simple backmatch rule among many rule types
 * - harness_016: Comprehensive backmatch pattern generation, complex patterns
 * - harness_017: Error path exploration, pattern reversal edge cases,
 *                malformed pattern testing, allocation edge cases (current)
 * 
 * Primary API: lou_compileString (direct entry to compileRule)
 * 
 * Strategy: Generate backward matching rules focusing on error conditions
 * and edge cases not covered by harness_016's pattern generation approach.
 */



// Log callback to avoid excessive logging during fuzzing
static void quiet_log(logLevels level, const char* msg) {
    (void)level;
    (void)msg;
}

// Global initialization flag
static int initialized = 0;

// Generate patterns specifically designed to test error paths and edge cases
static std::string generate_error_path_pattern(FuzzedDataProvider& fdp, int pattern_type, bool is_before) {
    std::string pattern;
    
    switch (pattern_type % 12) {
        case 0: { // Empty pattern (represented as "-")
            pattern = "-";
            break;
        }
            
        case 1: { // Single character pattern
            pattern = static_cast<char>('a' + (fdp.ConsumeIntegral<uint8_t>() % 26));
            break;
        }
            
        case 2: { // Pattern with only wildcard
            pattern = "*";
            break;
        }
            
        case 3: { // Pattern with multiple wildcards (potentially problematic)
            int num_wildcards = fdp.ConsumeIntegralInRange<int>(1, 5);
            for (int i = 0; i < num_wildcards; i++) {
                pattern += "*";
                if (i < num_wildcards - 1 && fdp.ConsumeBool()) {
                    pattern += static_cast<char>('a' + (fdp.ConsumeIntegral<uint8_t>() % 26));
                }
            }
            break;
        }
            
        case 4: { // Pattern with special characters that might cause compilation issues
            int length = fdp.ConsumeIntegralInRange<int>(1, 10);
            for (int i = 0; i < length; i++) {
                uint8_t char_type = fdp.ConsumeIntegral<uint8_t>() % 8;
                switch (char_type) {
                    case 0: // Control characters
                        pattern += static_cast<char>(fdp.ConsumeIntegralInRange<uint8_t>(1, 31));
                        break;
                    case 1: // Unicode-like high bytes
                        pattern += static_cast<char>(fdp.ConsumeIntegralInRange<uint8_t>(128, 255));
                        break;
                    case 2: // Pattern special chars
                        pattern += '[';
                        break;
                    case 3:
                        pattern += ']';
                        break;
                    case 4:
                        pattern += '(';
                        break;
                    case 5:
                        pattern += ')';
                        break;
                    case 6:
                        pattern += '\\';
                        break;
                    case 7: // Normal char
                        pattern += static_cast<char>('a' + (fdp.ConsumeIntegral<uint8_t>() % 26));
                        break;
                }
            }
            break;
        }
            
        case 5: { // Very long pattern (potential memory/compilation stress)
            int length = fdp.ConsumeIntegralInRange<int>(20, 100);
            for (int i = 0; i < length; i++) {
                pattern += static_cast<char>('a' + (fdp.ConsumeIntegral<uint8_t>() % 26));
                if (fdp.ConsumeBool() && i < length - 1) {
                    pattern += '*';
                }
            }
            break;
        }
            
        case 6: { // Pattern designed for interesting reversal
            // Patterns that when reversed might be interesting
            std::string base = "abc";
            if (fdp.ConsumeBool()) base = "123";
            if (fdp.ConsumeBool()) base = "!@#";
            
            if (fdp.ConsumeBool()) {
                pattern = base + "*" + base;
            } else {
                pattern = base;
            }
            break;
        }
            
        case 7: { // Pattern with hyphens (special in pattern compilation)
            pattern = "test-";
            if (fdp.ConsumeBool()) pattern += "pattern";
            if (fdp.ConsumeBool()) pattern += "-end";
            break;
        }
            
        case 8: { // Pattern with digits only
            int length = fdp.ConsumeIntegralInRange<int>(1, 10);
            for (int i = 0; i < length; i++) {
                pattern += static_cast<char>('0' + (fdp.ConsumeIntegral<uint8_t>() % 10));
            }
            break;
        }
            
        case 9: { // Mixed pattern with varied character types
            int length = fdp.ConsumeIntegralInRange<int>(3, 15);
            for (int i = 0; i < length; i++) {
                switch (i % 4) {
                    case 0: pattern += static_cast<char>('a' + (fdp.ConsumeIntegral<uint8_t>() % 26)); break;
                    case 1: pattern += static_cast<char>('A' + (fdp.ConsumeIntegral<uint8_t>() % 26)); break;
                    case 2: pattern += static_cast<char>('0' + (fdp.ConsumeIntegral<uint8_t>() % 10)); break;
                    case 3: pattern += static_cast<char>('!' + (fdp.ConsumeIntegral<uint8_t>() % 15)); break;
                }
            }
            break;
        }
            
        case 10: { // Pattern with escape sequences
            pattern = "\\";
            pattern += static_cast<char>('a' + (fdp.ConsumeIntegral<uint8_t>() % 26));
            if (fdp.ConsumeBool()) {
                pattern += "\\";
                pattern += static_cast<char>('0' + (fdp.ConsumeIntegral<uint8_t>() % 10));
            }
            break;
        }
            
        case 11: { // Pattern designed to test allocation boundaries
            // Create pattern that might stress pattern compilation
            pattern = "";
            for (int i = 0; i < fdp.ConsumeIntegralInRange<int>(0, 5); i++) {
                pattern += "xyz";
                if (fdp.ConsumeBool()) pattern += "*";
            }
            break;
        }
    }
    
    return pattern;
}

// Generate backward matching rule focusing on error conditions
static std::string generate_error_focused_backmatch_rule(FuzzedDataProvider& fdp, int rule_type) {
    std::string rule;
    
    // Determine rule structure based on type
    switch (rule_type % 10) {
        case 0: { // Basic backmatch with potential error patterns
            std::string before = generate_error_path_pattern(fdp, fdp.ConsumeIntegralInRange<int>(0, 11), true);
            std::string after = generate_error_path_pattern(fdp, fdp.ConsumeIntegralInRange<int>(0, 11), false);
            rule = "backmatch ";
            rule += before;
            rule += " ";
            rule += fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<int>(1, 10));  // Rule chars from fuzzer input
            rule += " ";
            rule += after;
            rule += " ";
            rule += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 1234567));  // Dots pattern
            break;
        }
            
        case 1: { // Backmatch with missing components (potential parse errors)
            uint8_t missing_part = fdp.ConsumeIntegral<uint8_t>() % 4;
            rule = "backmatch ";
            
            if (missing_part != 0) {
                rule += generate_error_path_pattern(fdp, fdp.ConsumeIntegralInRange<int>(0, 5), true);
            }
            rule += " ";
            
            if (missing_part != 1) {
                rule += fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<int>(1, 5));
            }
            rule += " ";
            
            if (missing_part != 2) {
                rule += generate_error_path_pattern(fdp, fdp.ConsumeIntegralInRange<int>(0, 5), false);
            }
            rule += " ";
            
            if (missing_part != 3) {
                rule += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 99999));
            }
            break;
        }
            
        case 2: { // Backmatch with noback/nofor modifiers and error patterns
            if (fdp.ConsumeBool()) {
                rule = "noback ";
            }
            if (fdp.ConsumeBool()) {
                rule += "nofor ";
            }
            rule += "backmatch ";
            
            // Use potentially problematic patterns
            rule += generate_error_path_pattern(fdp, fdp.ConsumeIntegralInRange<int>(6, 11), true);
            rule += " ";
            rule += fdp.ConsumeRandomLengthString(3);
            rule += " ";
            rule += generate_error_path_pattern(fdp, fdp.ConsumeIntegralInRange<int>(6, 11), false);
            rule += " ";
            rule += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 65535));
            break;
        }
            
        case 3: { // Backmatch with very long patterns (stress test)
            rule = "backmatch ";
            rule += fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<int>(10, 50));
            rule += " ";
            rule += fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<int>(1, 10));
            rule += " ";
            rule += fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<int>(10, 50));
            rule += " ";
            rule += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 9999999));
            break;
        }
            
        case 4: { // Backmatch with special character patterns only
            rule = "backmatch ";
            rule += "!@#$%^";
            rule += " ";
            rule += fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<int>(1, 10));  // Rule chars from fuzzer input
            rule += " ";
            rule += "&*()_+";
            rule += " ";
            rule += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 9999));
            break;
        }
            
        case 5: { // Backmatch with pattern designed for reversal testing
            std::string before;
            std::string after;
            
            // Create patterns that might behave differently when reversed
            if (fdp.ConsumeBool()) {
                before = "abc*def";
                after = "xyz*uvw";
            } else {
                before = "123*456";
                after = "789*012";
            }
            
            rule = "backmatch ";
            rule += before;
            rule += " ";
            rule += fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<int>(1, 10));  // Rule chars from fuzzer input
            rule += " ";
            rule += after;
            rule += " ";
            rule += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 88888));
            break;
        }
            
        case 6: { // Backmatch with minimal valid pattern
            rule = "backmatch - ";
            rule += fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<int>(1, 10));  // Rule chars from fuzzer input
            rule += " - 1";
            break;
        }
            
        case 7: { // Backmatch with only wildcards
            rule = "backmatch ";
            if (fdp.ConsumeBool()) {
                rule += "*";
            } else {
                rule += "**";
            }
            rule += " ";
            rule += "w";
            rule += " ";
            if (fdp.ConsumeBool()) {
                rule += "*";
            } else {
                rule += "**";
            }
            rule += " ";
            rule += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 77));
            break;
        }

            
        case 8: { // Backmatch with numeric patterns
            rule = "backmatch ";
            rule += "123";
            if (fdp.ConsumeBool()) rule += "*";
            rule += " ";
            rule += "num";
            rule += " ";
            rule += "456";
            if (fdp.ConsumeBool()) rule += "*";
            rule += " ";
            rule += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 999));
            break;
        }
            
        case 9: { // Backmatch with mixed pattern types
            rule = "backmatch ";
            
            // Mix pattern types
            uint8_t before_type = fdp.ConsumeIntegral<uint8_t>() % 3;
            if (before_type == 0) {
                rule += generate_error_path_pattern(fdp, 0, true); // Empty
            } else if (before_type == 1) {
                rule += generate_error_path_pattern(fdp, 1, true); // Single char
            } else {
                rule += generate_error_path_pattern(fdp, 2, true); // Wildcard
            }
            
            rule += " ";
            rule += fdp.ConsumeRandomLengthString(4);
            rule += " ";
            
            uint8_t after_type = fdp.ConsumeIntegral<uint8_t>() % 3;
            if (after_type == 0) {
                rule += generate_error_path_pattern(fdp, 0, false); // Empty
            } else if (after_type == 1) {
                rule += generate_error_path_pattern(fdp, 1, false); // Single char
            } else {
                rule += generate_error_path_pattern(fdp, 2, false); // Wildcard
            }
            
            rule += " ";
            rule += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 55555));
            break;
        }
    }
    
    return rule;
}

// Generate table with error-focused backward matching compilation scenarios
static std::string generate_error_focused_backmatch_table(FuzzedDataProvider& fdp) {
    std::string table;
    
    // Start with table header
    table += "# Error-focused backward matching compilation test table\n";
    table += "table error_backmatch_test\n\n";
    
    // Add locale
    table += "locale en-us\n\n";
    
    // Add minimal character definitions for context
    table += "# Basic character definitions\n";
    for (int i = 0; i < 2; i++) {
        table += "letter ";
        table += static_cast<char>('a' + i);
        table += " @ ";
        table += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 99));
        table += "\n";
    }
    table += "\n";
    
    // Add error-focused backmatch rules
    int num_backmatch_rules = fdp.ConsumeIntegralInRange<int>(1, 15);
    table += "# Error-focused backward matching rules\n";
    
    for (int i = 0; i < num_backmatch_rules; i++) {
        std::string rule = generate_error_focused_backmatch_rule(fdp, i);
        table += rule;
        table += "\n";
        
        // Occasionally add error-inducing comments
        if (fdp.ConsumeBool() && fdp.remaining_bytes() > 5) {
            table += "# Error test: ";
            table += fdp.ConsumeRandomLengthString(15);
            table += "\n";
        }
    }
    
    // Add some regular rules for comparison and context
    table += "\n# Regular rules for context\n";
    int num_regular_rules = fdp.ConsumeIntegralInRange<int>(0, 3);
    for (int i = 0; i < num_regular_rules; i++) {
        table += "letter ";
        table += static_cast<char>('c' + i);
        table += " @ ";
        table += std::to_string(fdp.ConsumeIntegralInRange<int>(100, 999));
        table += "\n";
    }
    
    // Add a forward match rule to test interaction
    if (fdp.ConsumeBool()) {
        table += "\n# Forward match rule for interaction test\n";
        table += "match abc test xyz 123\n";
    }
    
    return table;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size check - error patterns need some data
    if (size < 50) {
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
    
    // Generate table content with error-focused backward matching scenarios
    std::string table_name = "error_backmatch_compilation_test.ctb";
    std::string table_content = generate_error_focused_backmatch_table(fdp);
    
    // Compile the table - focus is on exercising CTO_BackMatch error paths
    int compilation_result = lou_compileString(table_name.c_str(), table_content.c_str());
    (void)compilation_result; // We're testing compilation, not result
    
    // Clean up
    lou_free();
    
    return 0;
}

} // namespace harness_017
