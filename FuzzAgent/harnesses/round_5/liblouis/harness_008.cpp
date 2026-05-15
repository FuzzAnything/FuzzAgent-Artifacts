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

namespace harness_008 {
/*
 * Fuzzing harness for liblouis translation rule compilation targeting compileRule function
 * Targets: 515 blocked branches in compileRule function (compileTranslationTable.c)
 * Primary API: lou_compileString (direct entry to compileRule)
 * Strategy: Generate diverse translation rule patterns to exercise complex compilation logic
 * 
 * Rule patterns to include:
 * 1. Wildcards, ranges, repetitions
 * 2. Character class definitions  
 * 3. Pass operations with conditions
 * 4. Include/subtable references
 * 5. Hyphenation and swap rules
 * 6. Before/after context rules
 * 7. Complex pattern matching with operators
 * 
 * Invocation sequence:
 * 1. lou_setDataPath (configure table search directory)
 * 2. lou_compileString with diverse translation rule patterns
 * 3. lou_checkTable (validate compilation results)
 * 4. lou_free (cleanup allocated resources)
 * 
 * Semantic differentiation from existing harnesses:
 * - harness_000: Basic translation APIs
 * - harness_001: Table metadata discovery
 * - harness_002: Hyphenation functionality
 * - harness_003: Table compilation and validation (basic)
 * - harness_004: Logging functionality
 * - harness_005: Pre-hyphenated translation
 * - harness_006: Table resolver system
 * - harness_007: Deep compilation functions and cleanup paths
 * - harness_008: Complex translation rule compilation targeting compileRule (current)
 */



// Log callback to avoid excessive logging during fuzzing
static void quiet_log(logLevels level, const char* msg) {
    (void)level;
    (void)msg;
}

// Global initialization flag
static int initialized = 0;

// Helper to generate complex rule patterns based on opcode types
static std::string generate_rule_pattern(FuzzedDataProvider& fdp, int pattern_type) {
    std::string rule;
    
    switch (pattern_type % 20) {
        case 0: { // Character definition with dots
            rule = "letter ";
            rule += static_cast<char>('a' + (fdp.ConsumeIntegral<uint8_t>() % 26));
            rule += " @ ";
            rule += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 1234567));
            break;
        }
            
        case 1: { // Sign definition
            rule = "sign ";
            rule += static_cast<char>('!' + (fdp.ConsumeIntegral<uint8_t>() % 15));
            rule += " ";
            rule += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 1234567));
            break;
        }
            
        case 2: { // Punctuation with specific dots
            rule = "punctuation ";
            rule += static_cast<char>('!' + (fdp.ConsumeIntegral<uint8_t>() % 15));
            rule += " ";
            rule += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 1234567));
            break;
        }
            
        case 3: { // Space definition with escape sequences
            rule = "space \\";
            rule += static_cast<char>('a' + (fdp.ConsumeIntegral<uint8_t>() % 26));
            rule += " ";
            rule += std::to_string(fdp.ConsumeIntegralInRange<int>(0, 9));
            rule += " ";
            rule += fdp.ConsumeRandomLengthString(10);
            break;
        }
            
        case 4: { // Math symbol
            rule = "math ";
            rule += static_cast<char>('+' + (fdp.ConsumeIntegral<uint8_t>() % 10));
            rule += " ";
            rule += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 1234567));
            break;
        }
            
        case 5: { // Digit definition
            rule = "digit ";
            rule += static_cast<char>('0' + (fdp.ConsumeIntegral<uint8_t>() % 10));
            rule += " @ ";
            rule += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 1234567));
            break;
        }
            
        case 6: { // Literal digit
            rule = "litdigit ";
            rule += static_cast<char>('0' + (fdp.ConsumeIntegral<uint8_t>() % 10));
            rule += " ";
            rule += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 1234567));
            break;
        }
            
        case 7: { // Character class definition
            rule = "class ";
            rule += fdp.ConsumeRandomLengthString(8);
            rule += " ";
            rule += static_cast<char>('a' + (fdp.ConsumeIntegral<uint8_t>() % 26));
            rule += "-";
            rule += static_cast<char>('z' - (fdp.ConsumeIntegral<uint8_t>() % 26));
            break;
        }
            
        case 8: { // Attribute definition
            rule = "attribute ";
            rule += fdp.ConsumeRandomLengthString(8);
            rule += " ";
            rule += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 1000));
            break;
        }
            
        case 9: { // Emphasis class
            rule = "emphclass ";
            rule += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 10));
            rule += " ";
            rule += fdp.ConsumeRandomLengthString(10);
            break;
        }
            
        case 10: { // Contraction rule
            rule = "contraction ";
            rule += fdp.ConsumeRandomLengthString(3);
            rule += " ";
            rule += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 1234567));
            break;
        }
            
        case 11: { // No contraction rule
            rule = "nocont ";
            rule += fdp.ConsumeRandomLengthString(3);
            rule += " ";
            rule += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 1234567));
            break;
        }
            
        case 12: { // Computer braille dots
            rule = "comp";
            rule += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 8));
            rule += " ";
            rule += static_cast<char>('a' + (fdp.ConsumeIntegral<uint8_t>() % 26));
            rule += " @ ";
            rule += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 1234567));
            break;
        }
            
        case 13: { // Hyphenation rule
            rule = "hyphen ";
            rule += static_cast<char>('a' + (fdp.ConsumeIntegral<uint8_t>() % 26));
            rule += " ";
            rule += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 1234567));
            break;
        }
            
        case 14: { // Swap rule
            rule = "swap";
            const char* swap_types[] = {"cc", "cd", "dd"};
            rule += swap_types[fdp.ConsumeIntegral<uint8_t>() % 3];
            rule += " ";
            rule += fdp.ConsumeRandomLengthString(2);
            rule += " ";
            rule += fdp.ConsumeRandomLengthString(2);
            break;
        }
            
        case 15: { // Context rule with before/after
            rule = "context ";
            rule += fdp.ConsumeRandomLengthString(2);
            rule += " ";
            rule += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 1234567));
            if (fdp.ConsumeBool()) {
                rule += " before ";
                rule += fdp.ConsumeRandomLengthString(2);
            }
            if (fdp.ConsumeBool()) {
                rule += " after ";
                rule += fdp.ConsumeRandomLengthString(2);
            }
            break;
        }
            
        case 16: { // Pass operation
            rule = "pass";
            rule += std::to_string(fdp.ConsumeIntegralInRange<int>(2, 4));
            rule += " ";
            rule += fdp.ConsumeRandomLengthString(3);
            rule += " ";
            rule += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 1234567));
            break;
        }
            
        case 17: { // Correct rule
            rule = "correct ";
            rule += fdp.ConsumeRandomLengthString(3);
            rule += " ";
            rule += fdp.ConsumeRandomLengthString(3);
            break;
        }
            
        case 18: { // Match rule with wildcard
            rule = "match ";
            rule += fdp.ConsumeRandomLengthString(3);
            if (fdp.ConsumeBool()) {
                rule += "*";
            }
            rule += " ";
            rule += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 1234567));
            break;
        }
            
        case 19: { // Backmatch rule
            rule = "backmatch ";
            rule += fdp.ConsumeRandomLengthString(3);
            if (fdp.ConsumeBool()) {
                rule += "*";
            }
            rule += " ";
            rule += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 1234567));
            break;
        }
    }
    
    // Add noback/nofor modifiers
    if (fdp.ConsumeBool()) {
        rule = "noback " + rule;
    }
    if (fdp.ConsumeBool()) {
        rule = "nofor " + rule;
    }
    
    return rule;
}
// Generate a complete table compilation string with various rule patterns
static std::string generate_table_compilation(FuzzedDataProvider& fdp) {
    std::string compilation;
    
    // Start with table declaration
    compilation += "table ";
    compilation += fdp.ConsumeRandomLengthString(10);
    compilation += "\n\n";
    
    // Add locale if needed
    if (fdp.ConsumeBool()) {
        const char* locales[] = {"en-us", "de-de", "fr-fr", "es-es", "nl-nl", "zh-cn", "ja-jp"};
        compilation += "locale ";
        compilation += locales[fdp.ConsumeIntegralInRange<size_t>(0, 6)];
        compilation += "\n\n";
    }
    
    // Add include statements
    size_t num_includes = fdp.ConsumeIntegralInRange<size_t>(0, 3);
    for (size_t i = 0; i < num_includes; i++) {
        compilation += "include ";
        compilation += fdp.ConsumeRandomLengthString(15);
        compilation += ".uti\n";
    }
    if (num_includes > 0) {
        compilation += "\n";
    }
    
    // Add emphasis class definitions
    if (fdp.ConsumeBool()) {
        compilation += "emphclass 1 bold\n";
        compilation += "emphclass 2 italic\n";
        compilation += "emphclass 3 underline\n\n";
    }
    
    // Add display rules
    if (fdp.ConsumeBool()) {
        compilation += "display\n";
        compilation += "dots46 @ 46\n";
        compilation += "dots123 @ 123\n\n";
    }
    
    // Add multiple rules with different patterns
    size_t num_rules = fdp.ConsumeIntegralInRange<size_t>(5, 50);
    for (size_t i = 0; i < num_rules; i++) {
        std::string rule = generate_rule_pattern(fdp, i);
        compilation += rule;
        compilation += "\n";
        
        // Occasionally add comments
        if (fdp.ConsumeBool() && fdp.remaining_bytes() > 10) {
            compilation += "# ";
            compilation += fdp.ConsumeRandomLengthString(20);
            compilation += "\n";
        }
    }
    
    // Add macros if enabled
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 50) {
        compilation += "\nmacro testMacro\n";
        compilation += "letter a @ 1\n";
        compilation += "letter b @ 12\n";
        compilation += "eom\n\n";
    }
    
    // Add multipass indicator
    if (fdp.ConsumeBool()) {
        compilation += "\nmultipass 1\n";
    }
    
    // Add grouping rules
    if (fdp.ConsumeBool()) {
        compilation += "\ngrouping (ab) @ 12\n";
        compilation += "grouping (cd) @ 34\n";
    }
    
    return compilation;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size check - need enough for meaningful rule generation
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
    
    // Consume operation type to decide testing strategy
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    // Generate table compilation content
    std::string table_list = "test_table.ctb";
    std::string compilation_string = generate_table_compilation(fdp);
    
    // Test different compilation scenarios
    int result = 0;
    
    switch (operation) {
        case 0: // Basic compilation test
            result = lou_compileString(table_list.c_str(), compilation_string.c_str());
            // Clean up if compilation succeeded
            if (result == 1) {
                lou_checkTable(table_list.c_str());
            }
            break;
            
        case 1: // Compilation with validation
            result = lou_compileString(table_list.c_str(), compilation_string.c_str());
            if (result == 1) {
                // Test emphasis class retrieval
                formtype emph_type = lou_getTypeformForEmphClass(table_list.c_str(), "bold");
                (void)emph_type;
                
                // Test table validation
                int check_result = lou_checkTable(table_list.c_str());
                (void)check_result;
            }
            break;
            
        case 2: // Multiple compilations
            // First compilation
            result = lou_compileString("table1.ctb", compilation_string.c_str());
            
            // Generate another compilation string
            if (fdp.remaining_bytes() > 50) {
                std::string compilation2 = generate_table_compilation(fdp);
                int result2 = lou_compileString("table2.ctb", compilation2.c_str());
                (void)result2;
            }
            break;
            
        case 3: // Edge case testing
            // Test with empty string
            lou_compileString(table_list.c_str(), "");
            
            // Test with very long string
            if (fdp.remaining_bytes() > 1000) {
                std::string long_string = fdp.ConsumeRemainingBytesAsString();
                lou_compileString(table_list.c_str(), long_string.c_str());
            }
            break;
    }
    
    // Always cleanup
    lou_free();
    
    return 0;
}

} // namespace harness_008
