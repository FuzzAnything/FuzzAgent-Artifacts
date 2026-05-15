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

namespace harness_020 {
/*
 * Fuzzing harness for liblouis targeting compileRule function with specific focus on
 * emphasis-related opcodes and mode operations based on coverage analysis recommendations.
 * 
 * Target: compileRule function with 526 blocked branches, focusing on untested
 * emphasis and mode operation branches identified in coverage analysis.
 * 
 * Specific focus areas:
 * 1. Emphasis class operations (CTO_EmphClass, CTO_EmphLetter, CTO_BegEmphWord, CTO_EndEmphWord)
 * 2. Mode operations (CTO_ModeLetter, CTO_BegModeWord, CTO_EndModeWord, CTO_BegMode, CTO_EndMode)
 * 3. Character class definitions and interactions with emphasis/mode operations
 * 4. Edge cases in emphasis mode character handling (CTO_EmphModeChars, CTO_NoEmphChars)
 * 5. Emphasis class validation and error handling paths
 * 
 * Semantic differentiation from existing harnesses:
 * - harness_011: General compilation edge cases and error handling
 * - harness_014: Macro compilation features  
 * - harness_016: CTO_BackMatch compilation
 * - harness_017: CTO_BackMatch error paths
 * - harness_018: CTO_Grouping compilation
 * - harness_019: Complex group scenarios
 * - harness_020: Deep emphasis and mode operation testing, character class interactions,
 *                emphasis class validation edge cases (current)
 * 
 * Primary API: lou_compileString (direct entry to compileRule)
 * 
 * Strategy: Generate diverse emphasis and mode operation scenarios targeting:
 * 1. Emphasis class declaration and usage sequences
 * 2. Mode operations with custom character class attributes
 * 3. Edge cases in emphasis mode character arrays
 * 4. Error scenarios: duplicate emphasis classes, invalid emphasis class usage
 * 5. Complex emphasis rule combinations with character class dependencies
 */



// Log callback to avoid excessive logging during fuzzing
static void quiet_log(logLevels level, const char* msg) {
    (void)level;
    (void)msg;
}

// Global initialization flag
static int initialized = 0;

// Helper to generate emphasis class name
static std::string generate_emphasis_class_name(FuzzedDataProvider& fdp, int class_id) {
    std::string name;
    
    // First 3 classes must be italic, underline, bold (for backwards compatibility)
    switch (class_id) {
        case 0:
            return "italic";
        case 1:
            return "underline";
        case 2:
            return "bold";
        default:
            // Generate custom emphasis class name
            int name_length = fdp.ConsumeIntegralInRange<int>(3, 15);
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
}

// Helper to generate character class name for mode operations
static std::string generate_character_class_name(FuzzedDataProvider& fdp) {
    std::string name;
    int name_length = fdp.ConsumeIntegralInRange<int>(3, 15);
    
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

// Generate emphasis class definition
static std::string generate_emphasis_class_definition(FuzzedDataProvider& fdp, int class_id) {
    std::string emphasis_def;
    
    emphasis_def += "emphclass ";
    emphasis_def += generate_emphasis_class_name(fdp, class_id);
    emphasis_def += "\n";
    
    return emphasis_def;
}

// Generate emphasis operation rule (emphletter, begemphword, endemphword, etc.)
static std::string generate_emphasis_operation(FuzzedDataProvider& fdp, const std::string& emphasis_class, int opcode_type) {
    std::string rule;
    
    switch (opcode_type % 8) {
        case 0: // emphletter
            rule += "emphletter ";
            rule += emphasis_class;
            rule += " a @ ";
            rule += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 123456));
            break;
            
        case 1: // begemphword
            rule += "begemphword ";
            rule += emphasis_class;
            rule += " test @ ";
            rule += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 123456));
            break;
            
        case 2: // endemphword
            rule += "endemphword ";
            rule += emphasis_class;
            rule += " test @ ";
            rule += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 123456));
            break;
            
        case 3: // begemph
            rule += "begemph ";
            rule += emphasis_class;
            rule += " a @ ";
            rule += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 123456));
            break;
            
        case 4: // endemph
            rule += "endemph ";
            rule += emphasis_class;
            rule += " a @ ";
            rule += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 123456));
            break;
            
        case 5: // begemphphrase
            rule += "begemphphrase ";
            rule += emphasis_class;
            rule += " test @ ";
            rule += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 123456));
            break;
            
        case 6: // endemphphrase (with before/after location)
            rule += "endemphphrase ";
            rule += emphasis_class;
            if (fdp.ConsumeBool()) {
                rule += " before";
            } else {
                rule += " after";
            }
            rule += " test @ ";
            rule += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 123456));
            break;
            
        case 7: // lenemphphrase
            rule += "lenemphphrase ";
            rule += emphasis_class;
            rule += " ";
            rule += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 10));
            break;
    }
    
    rule += "\n";
    return rule;
}

// Generate mode operation rule (modeletter, begmodeword, etc.)
static std::string generate_mode_operation(FuzzedDataProvider& fdp, const std::string& mode_name, int opcode_type) {
    std::string rule;
    
    switch (opcode_type % 9) {
        case 0: // capsletter (deprecated but still in switch)
            rule += "capsletter a @ ";
            rule += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 123456));
            break;
            
        case 1: // begcapsword
            rule += "begcapsword test @ ";
            rule += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 123456));
            break;
            
        case 2: // endcapsword
            rule += "endcapsword test @ ";
            rule += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 123456));
            break;
            
        case 3: // begcaps
            rule += "begcaps a @ ";
            rule += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 123456));
            break;
            
        case 4: // endcaps
            rule += "endcaps a @ ";
            rule += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 123456));
            break;
            
        case 5: // begcapsphrase
            rule += "begcapsphrase test @ ";
            rule += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 123456));
            break;
            
        case 6: // endcapsphrase (with before/after location)
            rule += "endcapsphrase ";
            if (fdp.ConsumeBool()) {
                rule += "before";
            } else {
                rule += "after";
            }
            rule += " test @ ";
            rule += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 123456));
            break;
            
        case 7: // lencapsphrase
            rule += "lencapsphrase ";
            rule += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 10));
            break;
            
        case 8: // mode operation with custom character class
            rule += "modeletter ";
            rule += mode_name;
            rule += " a @ ";
            rule += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 123456));
            break;
    }
    
    rule += "\n";
    return rule;
}

// Generate character class definition
static std::string generate_character_class_definition(FuzzedDataProvider& fdp, const std::string& class_name) {
    std::string class_def;
    
    class_def += "class ";
    class_def += class_name;
    class_def += "\n";
    
    // Add character definitions to the class
    int num_chars = fdp.ConsumeIntegralInRange<int>(1, 5);
    for (int i = 0; i < num_chars; i++) {
        char ch = static_cast<char>('a' + (fdp.ConsumeIntegral<uint8_t>() % 26));
        class_def += "letter ";
        class_def += ch;
        class_def += " @ ";
        class_def += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 123456));
        class_def += "\n";
    }
    
    return class_def;
}

// Generate emphasis mode character rule
static std::string generate_emph_mode_chars(FuzzedDataProvider& fdp, const std::string& emphasis_class) {
    std::string rule;
    
    rule += "emphmodechars ";
    rule += emphasis_class;
    
    // Add 1-5 characters
    int num_chars = fdp.ConsumeIntegralInRange<int>(1, 5);
    for (int i = 0; i < num_chars; i++) {
        char ch = static_cast<char>('a' + (fdp.ConsumeIntegral<uint8_t>() % 26));
        rule += " ";
        rule += ch;
    }
    
    rule += "\n";
    return rule;
}

// Generate no emphasis character rule
static std::string generate_no_emph_chars(FuzzedDataProvider& fdp, const std::string& emphasis_class) {
    std::string rule;
    
    rule += "noemphchars ";
    rule += emphasis_class;
    
    // Add 1-5 characters
    int num_chars = fdp.ConsumeIntegralInRange<int>(1, 5);
    for (int i = 0; i < num_chars; i++) {
        char ch = static_cast<char>('a' + (fdp.ConsumeIntegral<uint8_t>() % 26));
        rule += " ";
        rule += ch;
    }
    
    rule += "\n";
    return rule;
}

// Generate comprehensive table with emphasis and mode operations
static std::string generate_emphasis_mode_table(FuzzedDataProvider& fdp) {
    std::string table;
    
    // Add basic character definitions first
    table += "letter a @ 1\n";
    table += "letter b @ 12\n";
    table += "letter c @ 14\n";
    table += "letter d @ 145\n";
    table += "letter e @ 15\n";
    table += "letter f @ 124\n";
    table += "letter g @ 1245\n";
    table += "letter h @ 125\n";
    table += "letter i @ 24\n";
    table += "letter j @ 245\n";
    table += "letter k @ 13\n";
    table += "letter l @ 123\n";
    table += "letter m @ 134\n";
    table += "letter n @ 1345\n";
    table += "letter o @ 135\n";
    table += "letter p @ 1234\n";
    table += "letter q @ 12345\n";
    table += "letter r @ 1235\n";
    table += "letter s @ 234\n";
    table += "letter t @ 2345\n";
    table += "letter u @ 136\n";
    table += "letter v @ 1236\n";
    table += "letter w @ 2456\n";
    table += "letter x @ 1346\n";
    table += "letter y @ 13456\n";
    table += "letter z @ 1356\n";
    table += "digit 0 @ 356\n";
    table += "digit 1 @ 2\n";
    table += "digit 2 @ 23\n";
    table += "digit 3 @ 25\n";
    table += "digit 4 @ 256\n";
    table += "digit 5 @ 26\n";
    table += "digit 6 @ 235\n";
    table += "digit 7 @ 2356\n";
    table += "digit 8 @ 236\n";
    table += "digit 9 @ 35\n";
    
    // Determine how many emphasis classes to define (1-6, max is MAX_EMPH_CLASSES)
    int num_emphasis_classes = fdp.ConsumeIntegralInRange<int>(1, 6);
    
    // Define emphasis classes (first 3 must be italic, underline, bold)
    for (int i = 0; i < num_emphasis_classes; i++) {
        table += generate_emphasis_class_definition(fdp, i);
    }
    
    // Define a character class for mode operations (optional)
    if (fdp.ConsumeBool()) {
        std::string char_class_name = generate_character_class_name(fdp);
        table += generate_character_class_definition(fdp, char_class_name);
        
        // Add mode operations using this character class
        int num_mode_ops = fdp.ConsumeIntegralInRange<int>(1, 3);
        for (int i = 0; i < num_mode_ops; i++) {
            table += generate_mode_operation(fdp, char_class_name, i);
        }
    } else {
        // Add basic caps operations (uppercase mode)
        int num_caps_ops = fdp.ConsumeIntegralInRange<int>(1, 4);
        for (int i = 0; i < num_caps_ops; i++) {
            table += generate_mode_operation(fdp, "", i);
        }
    }
    
    // Add emphasis operations for each defined emphasis class
    for (int i = 0; i < num_emphasis_classes; i++) {
        std::string emphasis_class = generate_emphasis_class_name(fdp, i);
        
        // Add emphasis operations
        int num_emphasis_ops = fdp.ConsumeIntegralInRange<int>(1, 4);
        for (int j = 0; j < num_emphasis_ops; j++) {
            table += generate_emphasis_operation(fdp, emphasis_class, j);
        }
        
        // Possibly add emphasis mode characters
        if (fdp.ConsumeBool() && fdp.remaining_bytes() > 10) {
            table += generate_emph_mode_chars(fdp, emphasis_class);
        }
        
        // Possibly add no emphasis characters
        if (fdp.ConsumeBool() && fdp.remaining_bytes() > 10) {
            table += generate_no_emph_chars(fdp, emphasis_class);
        }
    }
    
    // Add some regular translation rules to ensure table is usable
    table += "always test @ 2345\n";
    table += "always example @ 12346\n";
    
    return table;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 16) return 0;  // Need minimum data for meaningful fuzzing
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize liblouis once
    if (!initialized) {
        lou_registerLogCallback(quiet_log);
        initialized = 1;
    }
    
    // Generate table content with emphasis and mode operations
    std::string table_content = generate_emphasis_mode_table(fdp);
    
    // Compile the table string
    int result = lou_compileString("fuzz_table", table_content.c_str());
    
    // Clean up if compilation succeeded
    if (result) {
        lou_free();
    }
    
    return 0;
}

} // namespace harness_020
