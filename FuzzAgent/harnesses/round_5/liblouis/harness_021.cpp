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

namespace harness_021 {
/*
 * Fuzzing harness for liblouis targeting compileRule function with 526 blocked branches
 * via lou_compileString API with diverse table definitions based on coverage analysis recommendations.
 * 
 * Focus on: 
 * 1) compileRule function identified as largest coverage gap with 526 blocked branches
 * 2) Use lou_compileString API with diverse table definitions to penetrate deep logical paths 
 *    in table compilation system
 * 3) Create semantically unique harness focusing on character class and attribute definitions
 *    (CTO_Class and CTO_Attribute opcodes) which represent untested logical paths
 * 4) Target specific aspects of compileRule that previous harnesses (011, 014, 016, 017, 018, 019, 020) 
 *    may have missed: character class definitions, attribute assignments, numbered attributes (0-7),
 *    and character class lookup/creation logic
 * 5) Address the significant branch coverage gap (currently 58.43% branch coverage) by exploring
 *    untested logical paths in the character class and attribute system
 * 
 * Specific target areas in compileRule:
 * - CTO_Class opcode (line 4096, deprecated but still functional)
 * - CTO_Attribute opcode (line 4098, modern replacement for class)
 * - Character class creation and lookup (findCharacterClass, addCharacterClass)
 * - Numbered attribute handling (attributes 0-7)
 * - Character class attribute assignment to characters
 * - usesAttributeOrClass table flag management
 * 
 * Semantic differentiation from existing harnesses:
 * - harness_011: General compilation edge cases and error handling
 * - harness_014: Macro compilation features
 * - harness_016: CTO_BackMatch compilation
 * - harness_017: CTO_BackMatch error paths
 * - harness_018: CTO_Grouping compilation
 * - harness_019: Complex group scenarios
 * - harness_020: Emphasis and mode operations
 * - harness_021: Character class and attribute definitions, numbered attributes,
 *                class/attribute interaction logic (current)
 * 
 * Primary API: lou_compileString (direct entry to compileRule)
 * 
 * Strategy: Generate diverse character class and attribute definitions targeting:
 * 1. Named character classes with various naming patterns
 * 2. Numbered attributes (0-7) with digit-based names
 * 3. Character class lookup scenarios (existing vs new classes)
 * 4. Attribute assignment to diverse character sets
 * 5. Error scenarios: mixing class and attribute rules, invalid names
 * 6. Boundary conditions for character class system
 */



// Log callback to avoid excessive logging during fuzzing
static void quiet_log(logLevels level, const char* msg) {
    (void)level;
    (void)msg;
}

// Global initialization flag
static int initialized = 0;

// Helper to generate valid class/attribute name (letters only for class, digits 0-7 or letters for attribute)
static std::string generate_class_attribute_name(FuzzedDataProvider& fdp, bool is_attribute) {
    std::string name;
    
    if (is_attribute) {
        // Attribute can be digit 0-7 or word with letters only
        uint8_t name_type = fdp.ConsumeIntegral<uint8_t>() % 3;
        switch (name_type) {
            case 0: // Single digit 0-7
                name = std::to_string(fdp.ConsumeIntegralInRange<int>(0, 7));
                break;
            case 1: // Letter-only word
                {
                    int name_length = fdp.ConsumeIntegralInRange<int>(1, 15);
                    for (int i = 0; i < name_length; i++) {
                        name += static_cast<char>('a' + (fdp.ConsumeIntegral<uint8_t>() % 26));
                    }
                }
                break;
            case 2: // Mixed letters (uppercase/lowercase)
                {
                    int name_length = fdp.ConsumeIntegralInRange<int>(1, 10);
                    for (int i = 0; i < name_length; i++) {
                        if (fdp.ConsumeBool()) {
                            name += static_cast<char>('a' + (fdp.ConsumeIntegral<uint8_t>() % 26));
                        } else {
                            name += static_cast<char>('A' + (fdp.ConsumeIntegral<uint8_t>() % 26));
                        }
                    }
                }
                break;
        }
    } else {
        // Class must be letters only (no digits)
        int name_length = fdp.ConsumeIntegralInRange<int>(1, 15);
        for (int i = 0; i < name_length; i++) {
            uint8_t char_type = fdp.ConsumeIntegral<uint8_t>() % 2;
            switch (char_type) {
                case 0: // lowercase letter
                    name += static_cast<char>('a' + (fdp.ConsumeIntegral<uint8_t>() % 26));
                    break;
                case 1: // uppercase letter
                    name += static_cast<char>('A' + (fdp.ConsumeIntegral<uint8_t>() % 26));
                    break;
            }
        }
    }
    
    return name;
}

// Helper to generate characters for class/attribute assignment
static std::string generate_characters_for_assignment(FuzzedDataProvider& fdp) {
    std::string chars;
    int num_chars = fdp.ConsumeIntegralInRange<int>(1, 20);
    
    for (int i = 0; i < num_chars; i++) {
        uint8_t char_type = fdp.ConsumeIntegral<uint8_t>() % 6;
        switch (char_type) {
            case 0: // lowercase letter
                chars += static_cast<char>('a' + (fdp.ConsumeIntegral<uint8_t>() % 26));
                break;
            case 1: // uppercase letter
                chars += static_cast<char>('A' + (fdp.ConsumeIntegral<uint8_t>() % 26));
                break;
            case 2: // digit
                chars += static_cast<char>('0' + (fdp.ConsumeIntegral<uint8_t>() % 10));
                break;
            case 3: // punctuation
                chars += static_cast<char>('!' + (fdp.ConsumeIntegral<uint8_t>() % 15));
                break;
            case 4: // space (special handling)
                chars += ' ';
                break;
            case 5: // extended ASCII
                chars += static_cast<char>(128 + (fdp.ConsumeIntegral<uint8_t>() % 128));
                break;
        }
    }
    
    return chars;
}

// Generate character class or attribute definition
static std::string generate_class_attribute_definition(FuzzedDataProvider& fdp, int definition_type) {
    std::string definition;
    
    switch (definition_type % 8) {
        case 0: // Simple class definition
            definition = "class ";
            definition += generate_class_attribute_name(fdp, false);
            definition += " ";
            definition += generate_characters_for_assignment(fdp);
            break;
            
        case 1: // Simple attribute definition
            definition = "attribute ";
            definition += generate_class_attribute_name(fdp, true);
            definition += " ";
            definition += generate_characters_for_assignment(fdp);
            break;
            
        case 2: // Attribute with digit name (0-7)
            definition = "attribute ";
            definition += std::to_string(fdp.ConsumeIntegralInRange<int>(0, 7));
            definition += " ";
            definition += generate_characters_for_assignment(fdp);
            break;
            
        case 3: // Multiple class definitions in sequence
            {
                int num_classes = fdp.ConsumeIntegralInRange<int>(1, 3);
                for (int i = 0; i < num_classes; i++) {
                    if (i > 0) definition += "\n";
                    definition += "class ";
                    definition += generate_class_attribute_name(fdp, false);
                    definition += " ";
                    definition += generate_characters_for_assignment(fdp);
                }
            }
            break;
            
        case 4: // Mixed class and attribute definitions (testing usesAttributeOrClass flag)
            definition = "class ";
            definition += generate_class_attribute_name(fdp, false);
            definition += " ";
            definition += generate_characters_for_assignment(fdp);
            definition += "\n";
            definition += "attribute ";
            definition += generate_class_attribute_name(fdp, true);
            definition += " ";
            definition += generate_characters_for_assignment(fdp);
            break;
            
        case 5: // Class with invalid digit in name (should trigger error)
            definition = "class ";
            definition += "test";
            definition += std::to_string(fdp.ConsumeIntegralInRange<int>(0, 9));
            definition += " ";
            definition += generate_characters_for_assignment(fdp);
            break;
            
        case 6: // Attribute with invalid multi-digit number (should trigger error)
            definition = "attribute ";
            definition += std::to_string(fdp.ConsumeIntegralInRange<int>(10, 99));
            definition += " ";
            definition += generate_characters_for_assignment(fdp);
            break;
            
        case 7: // Complex attribute with uppercase/lowercase mixing
            definition = "attribute ";
            definition += "MixedCaseName";
            definition += " ";
            definition += generate_characters_for_assignment(fdp);
            break;
    }
    
    return definition;
}

// Generate a complete table with character class/attribute definitions
static std::string generate_class_attribute_table(FuzzedDataProvider& fdp) {
    std::string table;
    
    // Start with some basic rules to ensure table is valid
    table += "locale en_US.UTF-8\n";
    table += "space \\s\n";
    table += "digit \\d\n";
    table += "punctuation \\p\n";
    
    // Add character class and attribute definitions
    int num_definitions = fdp.ConsumeIntegralInRange<int>(1, 10);
    for (int i = 0; i < num_definitions; i++) {
        table += "\n";
        table += generate_class_attribute_definition(fdp, i);
    }
    
    // Add some translation rules that might use the defined classes/attributes
    if (fdp.ConsumeBool()) {
        table += "\n";
        table += "a 1\n";
        table += "b 12\n";
        table += "c 123\n";
    }
    
    return table;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum input for meaningful testing
    if (size < 10) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize liblouis once
    if (!initialized) {
        lou_registerLogCallback(quiet_log);
        initialized = 1;
    }
    
    // Generate table list (use a standard table as base)
    std::string table_list = "en-us-g2.ctb";
    
    // Generate character class/attribute table definition
    std::string table_def = generate_class_attribute_table(fdp);
    
    // Compile the table definition
    int result = lou_compileString(table_list.c_str(), table_def.c_str());
    
    // Clean up any compiled tables to prevent memory leak
    lou_free();
    
    return 0;
}

} // namespace harness_021
