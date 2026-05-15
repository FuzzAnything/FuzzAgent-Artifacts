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

namespace harness_014 {
/*
 * Fuzzing harness for liblouis targeting macro definition and expansion features 
 * within compileRule function based on coverage analysis recommendations
 * 
 * Target: compileRule function which still has 526 blocked branches primarily 
 *         due to untested macro compilation features
 * 
 * Specific focus areas:
 * 1. Macro compilation features (compileMacro function lines 2779-2862)
 * 2. Macro expansion and invocation (lines 2970-3054 in compileRule)
 * 3. Variable substitution with $ syntax and MAX_MACRO_VAR limits
 * 4. Macro name validation and error cases
 * 5. Edge cases around MAXSTRING limits for macro definitions
 * 
 * Semantic differentiation from existing harnesses:
 * - harness_009: Basic macro definitions and grouping operations
 * - harness_010: Macro definitions with $ substitutions and grouping
 * - harness_011: General translation table compilation edge cases
 * - harness_014: Deep macro compilation features, validation, error paths,
 *                variable substitution limits, complex expansion scenarios
 * 
 * Primary API: lou_compileString (direct entry to compileRule)
 * 
 * Strategy: Generate diverse macro syntax with focus on:
 * 1. Complex macro names (validation paths)
 * 2. Variable substitutions with $0-$9 syntax
 * 3. Macro argument count variations (0-9 arguments)
 * 4. Nested macro definitions and invocations
 * 5. Edge cases: missing eom, too many substitutions, invalid names
 * 6. MAXSTRING boundary testing for macro definitions
 */



// Log callback to avoid excessive logging during fuzzing
static void quiet_log(logLevels level, const char* msg) {
    (void)level;
    (void)msg;
}

// Global initialization flag
static int initialized = 0;

// Helper to generate valid macro name (letters and digits only)
static std::string generate_macro_name(FuzzedDataProvider& fdp) {
    std::string name;
    int name_length = fdp.ConsumeIntegralInRange<int>(1, 20);
    
    for (int i = 0; i < name_length; i++) {
        uint8_t char_type = fdp.ConsumeIntegral<uint8_t>() % 3;
        switch (char_type) {
            case 0: // lowercase letter
                name += static_cast<char>('a' + (fdp.ConsumeIntegral<uint8_t>() % 26));
                break;
            case 1: // uppercase letter
                name += static_cast<char>('A' + (fdp.ConsumeIntegral<uint8_t>() % 26));
                break;
            case 2: // digit (but not as first character)
                if (i > 0) {
                    name += static_cast<char>('0' + (fdp.ConsumeIntegral<uint8_t>() % 10));
                } else {
                    name += static_cast<char>('a' + (fdp.ConsumeIntegral<uint8_t>() % 26));
                }
                break;
        }
    }
    
    return name;
}

// Helper to generate invalid macro name (with special characters)
static std::string generate_invalid_macro_name(FuzzedDataProvider& fdp) {
    std::string name;
    int name_length = fdp.ConsumeIntegralInRange<int>(1, 10);
    
    for (int i = 0; i < name_length; i++) {
        uint8_t char_type = fdp.ConsumeIntegral<uint8_t>() % 4;
        switch (char_type) {
            case 0: // letter
                name += static_cast<char>('a' + (fdp.ConsumeIntegral<uint8_t>() % 26));
                break;
            case 1: // digit
                name += static_cast<char>('0' + (fdp.ConsumeIntegral<uint8_t>() % 10));
                break;
            case 2: // underscore (invalid)
                name += '_';
                break;
            case 3: // punctuation (invalid)
                name += static_cast<char>('!' + (fdp.ConsumeIntegral<uint8_t>() % 15));
                break;
        }
    }
    
    return name;
}

// Helper to generate macro body with variable substitutions
static std::string generate_macro_body(FuzzedDataProvider& fdp, bool include_variable_subs) {
    std::string body;
    int num_lines = fdp.ConsumeIntegralInRange<int>(1, 10);
    
    for (int line = 0; line < num_lines; line++) {
        uint8_t line_type = fdp.ConsumeIntegral<uint8_t>() % 5;
        
        switch (line_type) {
            case 0: // Character definition
                body += "letter ";
                body += static_cast<char>('a' + (fdp.ConsumeIntegral<uint8_t>() % 26));
                body += " @ ";
                body += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 1234567));
                break;
                
            case 1: // Digit definition
                body += "digit ";
                body += static_cast<char>('0' + (fdp.ConsumeIntegral<uint8_t>() % 10));
                body += " ";
                body += std::to_string(fdp.ConsumeIntegralInRange<int>(0, 9));
                break;
                
            case 2: // Space definition
                body += "space \\";
                body += static_cast<char>('a' + (fdp.ConsumeIntegral<uint8_t>() % 26));
                body += " ";
                body += std::to_string(fdp.ConsumeIntegralInRange<int>(0, 9));
                break;
                
            case 3: // Variable substitution with $
                if (include_variable_subs && fdp.ConsumeBool()) {
                    body += "letter ";
                    body += static_cast<char>('a' + (fdp.ConsumeIntegral<uint8_t>() % 26));
                    body += " @ $";
                    body += std::to_string(fdp.ConsumeIntegralInRange<int>(0, 9));
                } else {
                    // Regular rule without substitution
                    body += "punctuation ";
                    body += static_cast<char>('!' + (fdp.ConsumeIntegral<uint8_t>() % 15));
                    body += " ";
                    body += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 1234567));
                }
                break;
                
            case 4: // Empty line or comment
                if (fdp.ConsumeBool()) {
                    body += "# Comment line ";
                    body += std::to_string(line);
                }
                // else empty line
                break;
        }
        
        body += "\n";
    }
    
    return body;
}

// Generate macro definition with varying complexity
static std::string generate_macro_definition(FuzzedDataProvider& fdp, int macro_id) {
    std::string macro;
    
    // Determine macro characteristics
    bool valid_name = fdp.ConsumeBool();
    bool include_variable_subs = fdp.ConsumeBool();
    bool include_eom = fdp.ConsumeBool();  // Test missing eom case
    
    // Macro header
    macro += "macro ";
    if (valid_name) {
        macro += generate_macro_name(fdp);
    } else {
        macro += generate_invalid_macro_name(fdp);
    }
    macro += "\n";
    
    // Macro body
    macro += generate_macro_body(fdp, include_variable_subs);
    
    // Macro termination
    if (include_eom) {
        macro += "eom\n";
    }
    // else: test missing eom (error case)
    
    return macro;
}

// Generate table with complex macro scenarios
static std::string generate_macro_scenario_table(FuzzedDataProvider& fdp) {
    std::string table;
    
    // Add table header
    table += "### Macro compilation test table\n";
    table += "# Generated by fuzzing harness\n\n";
    
    // Determine scenario type
    uint8_t scenario_type = fdp.ConsumeIntegral<uint8_t>() % 6;
    
    switch (scenario_type) {
        case 0: // Single macro with many variable substitutions
            {
                table += "# Test: Macro with max variable substitutions\n";
                table += generate_macro_definition(fdp, 0);
                if (fdp.ConsumeBool()) {
                    table += "\n# Some rules after macro\n";
                    table += "letter a @ 1\n";
                    table += "digit 0 0\n";
                }
            }
            break;
            
        case 1: // Nested macros (macro calling other macros)
            {
                table += "# Test: Nested macro definitions\n";
                // Generate macro names from fuzzer input
                std::string outer_name = generate_macro_name(fdp);
                std::string inner_name = generate_macro_name(fdp);
                
                // Define first macro
                table += "macro ";
                table += outer_name;
                table += "\n";
                table += "letter a @ $0\n";
                table += "include ";
                table += inner_name;
                table += "\n";
                table += "eom\n";
                
                // Define second macro
                table += "\nmacro ";
                table += inner_name;
                table += "\n";
                table += "digit $1 0\n";
                table += "eom\n";
                
                // Use the macros
                table += "\n# Use the macros\n";
                table += outer_name;
                table += " 1 2\n";
            }
            break;
        case 2: // Multiple macros with different argument counts
            {
                table += "# Test: Multiple macros with different args\n";
                int num_macros = fdp.ConsumeIntegralInRange<int>(1, 5);
                for (int i = 0; i < num_macros; i++) {
                    table += generate_macro_definition(fdp, i);
                    table += "\n";
                }
                
                // Invoke some macros
                if (num_macros > 0 && fdp.ConsumeBool()) {
                    table += "# Macro invocations\n";
                    for (int i = 0; i < std::min(num_macros, 3); i++) {
                        table += "macro_";
                        table += std::to_string(i);
                        int num_args = fdp.ConsumeIntegralInRange<int>(0, 3);
                        for (int j = 0; j < num_args; j++) {
                            table += " ";
                            table += std::to_string(fdp.ConsumeIntegralInRange<int>(0, 9));
                        }
                        table += "\n";
                    }
                }
            }
            break;
            
        case 3: // Macro with long body (testing MAXSTRING limits)
            {
                table += "# Test: Macro with long body\n";
                table += "macro long_macro\n";
                // Generate many lines to approach MAXSTRING
                int num_lines = fdp.ConsumeIntegralInRange<int>(10, 50);
                for (int i = 0; i < num_lines; i++) {
                    table += "letter ";
                    table += static_cast<char>('a' + (i % 26));
                    table += " @ ";
                    table += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 999999));
                    table += "\n";
                }
                table += "eom\n";
            }
            break;
            
        case 4: // Error case testing (invalid macro syntax)
            {
                table += "# Test: Invalid macro syntax cases\n";
                if (fdp.ConsumeBool()) {
                    // Macro without eom
                    table += "macro no_eom_macro\n";
                    table += "letter a @ 1\n";
                    table += "digit 0 0\n";
                    // intentionally missing eom
                } else {
                    // Macro with opcode as name
                    table += "macro space\n";  // "space" is a CTO opcode
                    table += "letter a @ 1\n";
                    table += "eom\n";
                }
            }
            break;
            
        case 5: // Mixed macro and regular rules
            {
                table += "# Test: Mixed macro and regular rules\n";
                // Add some regular rules first
                int num_regular = fdp.ConsumeIntegralInRange<int>(1, 5);
                for (int i = 0; i < num_regular; i++) {
                    table += "letter ";
                    table += static_cast<char>('a' + (i % 26));
                    table += " @ ";
                    table += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 1234567));
                    table += "\n";
                }
                
                // Add macro
                table += "\n";
                table += generate_macro_definition(fdp, 0);
                
                // Add more rules
                if (fdp.ConsumeBool()) {
                    table += "\n";
                    int num_more = fdp.ConsumeIntegralInRange<int>(1, 3);
                    for (int i = 0; i < num_more; i++) {
                        table += "digit ";
                        table += static_cast<char>('0' + (i % 10));
                        table += " ";
                        table += std::to_string(fdp.ConsumeIntegralInRange<int>(0, 9));
                        table += "\n";
                    }
                }
            }
            break;
    }
    
    return table;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size check - macros need some data
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
    
    // Generate table content with macro compilation scenarios
    std::string table_name = "macro_compilation_test.ctb";
    std::string table_content = generate_macro_scenario_table(fdp);
    
    // Compile the table - we don't care about success/failure,
    // just want to exercise the macro compilation paths
    int compilation_result = lou_compileString(table_name.c_str(), table_content.c_str());
    (void)compilation_result;
    
    // Clean up
    lou_free();
    
    return 0;
}

} // namespace harness_014
