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

namespace harness_010 {
/*
 * Fuzzing harness for liblouis translation table compilation targeting CTO_Macro and CTO_Grouping opcodes
 * Following harness_010_guidance.md specification
 * 
 * Primary API: lou_compileString
 * Specific targets: 
 * 1. CTO_Macro definitions (lines 2893-2909 in compileTranslationTable.c)
 * 2. CTO_Grouping opcodes (line 2955 in compileTranslationTable.c)
 * 3. Nested macro invocations
 * 
 * Invocation sequence per guidance:
 * 1. Call lou_compileString with translation table rules containing CTO_Macro definitions
 * 2. Call lou_compileString with translation table rules containing CTO_Grouping opcodes
 * 3. Call lou_compileString with nested macro invocations
 * 4. Call lou_free to clean up compiled table
 * 
 * Semantic differentiation from harness_009:
 * - More focused on exact invocation sequence from guidance
 * - Explicit testing of macro definition syntax ("macro name" format)
 * - Explicit testing of grouping opcode syntax for character class relationships
 * - Separate compilation calls for each operation type
 * - Testing nested macro invocations specifically
 */



// Log callback to avoid excessive logging during fuzzing
static void quiet_log(logLevels level, const char* msg) {
    (void)level;
    (void)msg;
}

// Global initialization flag
static int initialized = 0;

// Helper to generate macro definition with proper syntax
static std::string generate_macro_definition(FuzzedDataProvider& fdp, const std::string& name) {
    std::string macro;
    
    // Macro definition format: macro <name>
    macro += "macro ";
    macro += name;
    macro += "\n";
    
    // Add macro body content
    int body_lines = fdp.ConsumeIntegralInRange<int>(1, 5);
    for (int i = 0; i < body_lines; i++) {
        // Generate different types of macro content
        uint8_t content_type = fdp.ConsumeIntegral<uint8_t>() % 4;
        
        switch (content_type) {
            case 0: // Character mapping
                macro += "letter ";
                macro += static_cast<char>('a' + (fdp.ConsumeIntegral<uint8_t>() % 26));
                macro += " @ ";
                macro += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 1234567));
                break;
                
            case 1: // Digit mapping  
                macro += "digit ";
                macro += static_cast<char>('0' + (fdp.ConsumeIntegral<uint8_t>() % 10));
                macro += " ";
                macro += std::to_string(fdp.ConsumeIntegralInRange<int>(0, 9));
                break;
                
            case 2: // Space definition
                macro += "space \\";
                macro += static_cast<char>('a' + (fdp.ConsumeIntegral<uint8_t>() % 26));
                macro += " ";
                macro += std::to_string(fdp.ConsumeIntegralInRange<int>(0, 9));
                break;
                
            case 3: // Variable substitution with $
                macro += "letter a @ $";
                macro += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 9));
                break;
        }
        macro += "\n";
    }
    
    // End macro with eom
    macro += "eom\n";
    return macro;
}

// Helper to generate grouping operation with proper syntax
static std::string generate_grouping_operation(FuzzedDataProvider& fdp) {
    std::string grouping;
    
    // Grouping format: grouping (characters) @ dots
    grouping += "grouping (";
    
    // Generate character sequence
    int group_len = fdp.ConsumeIntegralInRange<int>(1, 4);
    for (int i = 0; i < group_len; i++) {
        uint8_t char_type = fdp.ConsumeIntegral<uint8_t>() % 3;
        switch (char_type) {
            case 0: // lowercase letter
                grouping += static_cast<char>('a' + (fdp.ConsumeIntegral<uint8_t>() % 26));
                break;
            case 1: // digit
                grouping += static_cast<char>('0' + (fdp.ConsumeIntegral<uint8_t>() % 10));
                break;
            case 2: // special character
                grouping += static_cast<char>('!' + (fdp.ConsumeIntegral<uint8_t>() % 15));
                break;
        }
    }
    
    grouping += ") @ ";
    grouping += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 1234567));
    grouping += "\n";
    
    return grouping;
}

// Helper to generate table with macro definitions
static std::string generate_table_with_macros(FuzzedDataProvider& fdp) {
    std::string table;
    
    // Add header
    table += "# Table with macro definitions\n";
    table += "include en-ueb-g1.ctb\n\n";
    
    // Add macro definitions
    int num_macros = fdp.ConsumeIntegralInRange<int>(1, 3);
    for (int i = 0; i < num_macros; i++) {
        std::string macro_name = "macro_";
        macro_name += std::to_string(i);
        table += generate_macro_definition(fdp, macro_name);
        table += "\n";
    }
    
    // Add some regular rules that might use macros
    table += "# Regular rules\n";
    int num_rules = fdp.ConsumeIntegralInRange<int>(2, 5);
    for (int i = 0; i < num_rules; i++) {
        table += "letter ";
        table += static_cast<char>('a' + (fdp.ConsumeIntegral<uint8_t>() % 26));
        table += " @ ";
        table += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 1234567));
        table += "\n";
    }
    
    return table;
}

// Helper to generate table with grouping operations
static std::string generate_table_with_grouping(FuzzedDataProvider& fdp) {
    std::string table;
    
    // Add header
    table += "# Table with grouping operations\n";
    table += "include en-ueb-g1.ctb\n\n";
    
    // Add character class definitions first
    table += "# Character classes\n";
    int num_classes = fdp.ConsumeIntegralInRange<int>(1, 3);
    for (int i = 0; i < num_classes; i++) {
        uint8_t class_type = fdp.ConsumeIntegral<uint8_t>() % 4;
        switch (class_type) {
            case 0:
                table += "space ";
                table += static_cast<char>('a' + (fdp.ConsumeIntegral<uint8_t>() % 26));
                table += " ";
                table += std::to_string(fdp.ConsumeIntegralInRange<int>(0, 9));
                break;
            case 1:
                table += "digit ";
                table += static_cast<char>('0' + (fdp.ConsumeIntegral<uint8_t>() % 10));
                table += " ";
                table += std::to_string(fdp.ConsumeIntegralInRange<int>(0, 9));
                break;
            case 2:
                table += "punctuation ";
                table += static_cast<char>('!' + (fdp.ConsumeIntegral<uint8_t>() % 15));
                table += " ";
                table += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 1234567));
                break;
            case 3:
                table += "letter ";
                table += static_cast<char>('a' + (fdp.ConsumeIntegral<uint8_t>() % 26));
                table += " @ ";
                table += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 1234567));
                break;
        }
        table += "\n";
    }
    table += "\n";
    
    // Add grouping operations
    table += "# Grouping operations\n";
    int num_groupings = fdp.ConsumeIntegralInRange<int>(1, 4);
    for (int i = 0; i < num_groupings; i++) {
        table += generate_grouping_operation(fdp);
    }
    
    return table;
}

// Helper to generate table with nested macro invocations
static std::string generate_table_with_nested_macros(FuzzedDataProvider& fdp) {
    std::string table;
    
    // Add header
    table += "# Table with nested macro invocations\n";
    table += "include en-ueb-g1.ctb\n\n";
    
    // Define base macros
    table += "# Base macros\n";
    table += "macro base_macro\n";
    table += "letter x @ 123456\n";
    table += "letter y @ 234567\n";
    table += "eom\n\n";
    
    // Define macro that references base macro
    table += "macro nested_macro\n";
    table += "letter a @ 111111\n";
    table += "# Reference to base macro content\n";
    table += "include base_macro\n";
    table += "letter z @ 333333\n";
    table += "eom\n\n";
    
    // Use the nested macro in rules
    table += "# Rules using nested macro\n";
    table += "include nested_macro\n";
    
    // Add some additional rules
    int num_rules = fdp.ConsumeIntegralInRange<int>(2, 4);
    for (int i = 0; i < num_rules; i++) {
        table += "letter ";
        table += static_cast<char>('a' + (fdp.ConsumeIntegral<uint8_t>() % 26));
        table += " @ ";
        table += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 1234567));
        table += "\n";
    }
    
    return table;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size check
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
    
    // Determine which operation to test based on input
    uint8_t test_scenario = fdp.ConsumeIntegral<uint8_t>() % 4;
    int compilation_result = 0;
    
    switch (test_scenario) {
        case 0: // Test 1: Macro definitions
            {
                std::string table_name = "test_macros.ctb";
                std::string table_content = generate_table_with_macros(fdp);
                compilation_result = lou_compileString(table_name.c_str(), table_content.c_str());
            }
            break;
            
        case 1: // Test 2: Grouping operations
            {
                std::string table_name = "test_grouping.ctb";
                std::string table_content = generate_table_with_grouping(fdp);
                compilation_result = lou_compileString(table_name.c_str(), table_content.c_str());
            }
            break;
            
        case 2: // Test 3: Nested macro invocations
            {
                std::string table_name = "test_nested.ctb";
                std::string table_content = generate_table_with_nested_macros(fdp);
                compilation_result = lou_compileString(table_name.c_str(), table_content.c_str());
            }
            break;
            
        case 3: // Test all three in sequence (as per guidance)
            {
                // 1. Call lou_compileString with macro definitions
                std::string table1_name = "seq_macros.ctb";
                std::string table1_content = generate_table_with_macros(fdp);
                int result1 = lou_compileString(table1_name.c_str(), table1_content.c_str());
                (void)result1;
                
                // 2. Call lou_compileString with grouping opcodes
                if (fdp.remaining_bytes() > 50) {
                    std::string table2_name = "seq_grouping.ctb";
                    std::string table2_content = generate_table_with_grouping(fdp);
                    int result2 = lou_compileString(table2_name.c_str(), table2_content.c_str());
                    (void)result2;
                }
                
                // 3. Call lou_compileString with nested macro invocations
                if (fdp.remaining_bytes() > 50) {
                    std::string table3_name = "seq_nested.ctb";
                    std::string table3_content = generate_table_with_nested_macros(fdp);
                    int result3 = lou_compileString(table3_name.c_str(), table3_content.c_str());
                    (void)result3;
                }
                
                compilation_result = 1; // Mark as success for cleanup
            }
            break;
    }
    
    // 4. Always call lou_free to clean up compiled table (as per guidance)
    lou_free();
    
    return 0;
}

} // namespace harness_010
