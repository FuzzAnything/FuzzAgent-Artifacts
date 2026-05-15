#include <fuzzer/FuzzedDataProvider.h>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>
#include <algorithm>
#include "liblouis.h"

namespace harness_021 {
// liblouis compileRule custom attribute mode harness
// Target: compileRule function (580 blocked branches) focusing on custom attribute 
//         mode functionality in translation table compilation
// Primary focus: CTO_Attribute opcode for defining custom attributes, CTO_Before/CTO_After
//                attribute application, and base opcode with custom attributes
// Based on coverage analysis guidance targeting custom attribute mode functionality
// Semantic differentiation from harness_014 (emphasis/sequence), harness_003 (basic compilation):
//   Deep focus on custom attribute creation, validation, and application patterns
//   Testing reserved attribute name validation, attribute numbering (0-7), named attributes
//   Testing attribute compatibility rules (cannot mix class and attribute in same table)


// Include liblouis public header
extern "C" {
}

// Log callback to suppress logs during fuzzing
static void avoid_log_callback(logLevels level, const char *msg) {
    (void)level;
    (void)msg;
    // Suppress all logs during fuzzing
}

// Reserved attribute names that cannot be used for custom attributes
// From compileTranslationTable.c line 126-155
static const char* reserved_attributes[] = {
    "numericnocontchars",
    "numericnocontchar",
    "numericnocont",
    "midendnumericmodechars",
    "midendnumericmodechar",
    "midendnumericmode",
    "numericmodechars",
    "numericmodechar",
    "numericmode",
    "capsmodechars",
    "capsmodechar",
    "capsmode",
    "emphmodechars",
    "emphmodechar",
    "emphmode",
    "noemphchars",
    "noemphchar",
    "noemph",
    "seqdelimiter",
    "seqbeforechars",
    "seqbeforechar",
    "seqbefore",
    "seqafterchars",
    "seqafterchar",
    "seqafter",
    "noletsign",
    "noletsignbefore",
    "noletsignafter",
    NULL,
};

// Helper to check if a name is reserved (cannot be used as custom attribute)
static bool is_reserved_attribute(const std::string& name) {
    for (int i = 0; reserved_attributes[i] != NULL; i++) {
        if (name == reserved_attributes[i]) {
            return true;
        }
    }
    return false;
}

// Helper to generate valid custom attribute name (not reserved, only letters)
static std::string generate_valid_attribute_name(FuzzedDataProvider& fdp) {
    std::string name;
    
    // Choose between numbered attribute (0-7) or named attribute
    bool use_numbered = fdp.ConsumeBool();
    
    if (use_numbered) {
        // Numbered attribute: 0-7
        name = std::to_string(fdp.ConsumeIntegralInRange<int>(0, 7));
    } else {
        // Named attribute: letters only
        size_t name_len = fdp.ConsumeIntegralInRange<size_t>(1, 16);
        for (size_t i = 0; i < name_len; i++) {
            // Generate only letters (a-z, A-Z) as required by validation
            uint8_t char_type = fdp.ConsumeIntegral<uint8_t>() % 2;
            if (char_type == 0) {
                name += fdp.ConsumeIntegralInRange<char>('a', 'z');
            } else {
                name += fdp.ConsumeIntegralInRange<char>('A', 'Z');
            }
        }
        
        // Ensure it's not a reserved name by checking and regenerating if needed
        if (is_reserved_attribute(name)) {
            // Append something to make it unique
            name += "_custom";
        }
    }
    
    return name;
}

// Helper to generate attribute definition line
static std::string generate_attribute_definition(FuzzedDataProvider& fdp) {
    std::string line = "attribute ";
    
    // Choose attribute name
    line += generate_valid_attribute_name(fdp);
    line += " ";
    
    // Choose characters to apply attribute to (1-8 characters)
    size_t num_chars = fdp.ConsumeIntegralInRange<size_t>(1, 8);
    for (size_t i = 0; i < num_chars; i++) {
        // Generate printable ASCII characters
        line += fdp.ConsumeIntegralInRange<char>('!', '~');
    }
    
    return line + "\n";
}

// Helper to generate before/after attribute application
// CTO_Before and CTO_After apply attributes to characters for context matching
static std::string generate_before_after_rule(FuzzedDataProvider& fdp, bool is_before) {
    std::string line;
    
    if (is_before) {
        line = "before ";
    } else {
        line = "after ";
    }
    
    // Choose attribute name (must be defined first in table)
    line += generate_valid_attribute_name(fdp);
    
    return line + "\n";
}

// Helper to generate base opcode with custom attribute
// Base opcode sets base character for uppercase/digit mode or custom attribute
static std::string generate_base_rule(FuzzedDataProvider& fdp) {
    std::string line = "base ";
    
    // Choose attribute name - base supports "uppercase", "digit", or custom attribute
    uint8_t attr_type = fdp.ConsumeIntegral<uint8_t>() % 3;
    if (attr_type == 0) {
        line += "uppercase";
    } else if (attr_type == 1) {
        line += "digit";
    } else {
        // Custom attribute (must be valid name)
        line += generate_valid_attribute_name(fdp);
    }
    
    line += " ";
    
    // Character to apply base to (single character)
    line += fdp.ConsumeIntegralInRange<char>('a', 'z');
    line += " ";
    
    // Base character (single character)  
    line += fdp.ConsumeIntegralInRange<char>('a', 'z');
    
    return line + "\n";
}

// Helper to generate class definition (deprecated, for testing compatibility)
static std::string generate_class_definition(FuzzedDataProvider& fdp) {
    std::string line = "class ";
    
    // Class names cannot contain digits, only letters
    size_t name_len = fdp.ConsumeIntegralInRange<size_t>(1, 12);
    for (size_t i = 0; i < name_len; i++) {
        line += fdp.ConsumeIntegralInRange<char>('a', 'z');
    }
    
    line += " ";
    
    // Characters to apply class to
    size_t num_chars = fdp.ConsumeIntegralInRange<size_t>(1, 6);
    for (size_t i = 0; i < num_chars; i++) {
        line += fdp.ConsumeIntegralInRange<char>('!', '~');
    }
    
    return line + "\n";
}

// Helper to generate invalid attribute names (for testing validation)
static std::string generate_invalid_attribute_test(FuzzedDataProvider& fdp) {
    std::string line = "attribute ";
    
    // Generate invalid attribute name (contains non-letters or reserved)
    uint8_t invalid_type = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    if (invalid_type == 0) {
        // Name with digits (valid for attribute but not for class)
        line += "attr123";
    } else if (invalid_type == 1) {
        // Name with special characters
        line += "attr-name";
    } else if (invalid_type == 2) {
        // Reserved attribute name
        line += reserved_attributes[fdp.ConsumeIntegralInRange<size_t>(0, 27)];
    } else {
        // Empty or very short name
        line += "a";
    }
    
    line += " ";
    line += fdp.ConsumeIntegralInRange<char>('a', 'z');
    
    return line + "\n";
}

// Main harness for custom attribute mode compilation testing
int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough for various parameters
    if (size < 32) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize liblouis once
    static bool initialized = false;
    if (!initialized) {
        lou_registerLogCallback(avoid_log_callback);
        initialized = true;
    }
    
    // Build table content with custom attribute patterns
    std::string table_content;
    
    // 1. Table header
    table_content += "table custom-attribute-test\n";
    table_content += "locale en_US\n";
    table_content += "author fuzzer-custom-attr\n";
    
    // 2. Decide whether to test attribute or class (they cannot be mixed)
    bool test_attribute_mode = fdp.ConsumeBool();
    
    if (test_attribute_mode) {
        // Test attribute mode (CTO_Attribute opcode)
        
        // Add some standard character definitions first
        table_content += "space \\x20\n";
        table_content += "digit 0\n";
        table_content += "letter a\n";
        
        // Add custom attribute definitions
        size_t num_attributes = fdp.ConsumeIntegralInRange<size_t>(1, 8);
        for (size_t i = 0; i < num_attributes; i++) {
            table_content += generate_attribute_definition(fdp);
            
            // Occasionally add before/after rules using the attribute
            if (fdp.ConsumeBool()) {
                table_content += generate_before_after_rule(fdp, true); // before
            }
            if (fdp.ConsumeBool()) {
                table_content += generate_before_after_rule(fdp, false); // after
            }
            
            // Occasionally add base rule with custom attribute
            if (fdp.ConsumeBool()) {
                table_content += generate_base_rule(fdp);
            }
        }
        
        // Add some translation rules that might use attributes
        table_content += "always a 1\n";
        table_content += "always b 12\n";
        
    } else {
        // Test class mode (CTO_Class opcode - deprecated but still in code)
        // This tests the compatibility check that prevents mixing class and attribute
        
        table_content += "class customclass abcdef\n";
        table_content += "always x 123\n";
        
        // Try to add attribute (should fail compilation but exercise error path)
        if (fdp.ConsumeBool()) {
            table_content += "attribute test xyz\n";
        }
    }
    
    // 3. Optionally add invalid attribute tests to exercise validation paths
    if (fdp.ConsumeBool()) {
        table_content += generate_invalid_attribute_test(fdp);
    }
    
    // 4. Add some normal translation rules to ensure table has valid content
    table_content += "always c 123\n";
    table_content += "always d 124\n";
    table_content += "always e 125\n";
    
    // 5. Compile the table string
    // lou_compileString returns 1 on success, 0 on failure
    // First argument is table name, second is table content string
    int compile_result = lou_compileString("custom-attribute-test", table_content.c_str());
    
    // Even if compilation fails, we've exercised the compileRule paths
    // The fuzzer will learn which inputs cause successful vs failed compilation
    
    // 6. If compilation succeeded, get the table and perform additional checks
    if (compile_result == 1) {
        // Get the compiled table for additional testing
        const void* table = lou_getTable("custom-attribute-test");
        
        if (table) {
            // Check table validity
            int check_result = lou_checkTable("custom-attribute-test");
            // check_result meaning: 1 = valid, 0 = invalid
            
            // Free the table
            lou_free();
        }
    }
    
    return 0;
}

} // namespace harness_021
