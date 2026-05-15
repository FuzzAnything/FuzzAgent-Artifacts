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

namespace harness_022 {
/*
 * Fuzzing harness for liblouis targeting translation table compilation for compileRule's 527 blocked branches
 * based on coverage analysis recommendations. Focus on swap operations and before/after attribute handling.
 * 
 * Target: compileRule function identified as top blocker with 527 blocked branches
 * 
 * Specific focus areas for semantic diversity:
 * 1. Swap operations: CTO_SwapCc, CTO_SwapCd, CTO_SwapDd (lines 4294-4297)
 * 2. Before/After attribute handling: CTO_Before, CTO_After (lines 4200-4218)
 * 3. Hyphen and decimal point operations: CTO_Hyphen, CTO_DecPoint (lines 4298-4311)
 * 4. Complex rule combinations with character class interactions
 * 5. Edge cases in swap compilation logic (compileSwap function lines 1652-1678)
 * 
 * Semantic differentiation from existing harnesses:
 * - harness_011: General compilation edge cases and error handling
 * - harness_014: Macro compilation features
 * - harness_016: CTO_BackMatch compilation
 * - harness_017: CTO_BackMatch error paths
 * - harness_018: CTO_Grouping compilation
 * - harness_019: Complex group scenarios
 * - harness_020: Emphasis and mode operations
 * - harness_021: Character class and attribute definitions
 * - harness_022: Swap operations, Before/After attribute handling, 
 *                hyphen/decimal point rules (current)
 * 
 * Primary API: lou_compileString (direct entry to compileRule)
 * 
 * Strategy: Generate diverse translation rules targeting:
 * 1. Swap operations with character/dot pattern combinations
 * 2. Before/After attribute assignment with character classes
 * 3. Hyphen and decimal point rule definitions
 * 4. Complex interactions between swap rules and character attributes
 * 5. Error scenarios: invalid swap patterns, missing character classes
 */



// Log callback to avoid excessive logging during fuzzing
static void quiet_log(logLevels level, const char* msg) {
    (void)level;
    (void)msg;
}

// Global initialization flag
static int initialized = 0;

// Helper to generate valid swap operation name
static std::string generate_swap_name(FuzzedDataProvider& fdp) {
    std::string name;
    int name_length = fdp.ConsumeIntegralInRange<int>(1, 15);
    
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

// Helper to generate character string for swap/attribute operations
static std::string generate_character_string(FuzzedDataProvider& fdp, bool for_chars) {
    std::string chars;
    int length = fdp.ConsumeIntegralInRange<int>(1, 5);
    
    for (int i = 0; i < length; i++) {
        if (for_chars) {
            // For character strings (text representation)
            uint8_t char_type = fdp.ConsumeIntegral<uint8_t>() % 4;
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
                case 3: // punctuation (for testing edge cases)
                    chars += static_cast<char>('.' + (fdp.ConsumeIntegral<uint8_t>() % 5));
                    break;
            }
        } else {
            // For dot patterns (braille cell representation)
            // Generate valid dot patterns like "123", "14", "25-36", etc.
            int num_dots = fdp.ConsumeIntegralInRange<int>(1, 3);
            for (int j = 0; j < num_dots; j++) {
                if (j > 0 && fdp.ConsumeBool()) {
                    chars += '-';
                }
                chars += static_cast<char>('1' + (fdp.ConsumeIntegral<uint8_t>() % 6));
                if (fdp.ConsumeBool() && j < num_dots - 1) {
                    chars += static_cast<char>('1' + (fdp.ConsumeIntegral<uint8_t>() % 6));
                }
            }
        }
    }
    
    return chars;
}

// Helper to generate character class name for before/after attributes
static std::string generate_character_class_name(FuzzedDataProvider& fdp) {
    std::string name;
    int name_length = fdp.ConsumeIntegralInRange<int>(3, 12);
    
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

// Generate a complete translation table definition targeting swap and attribute operations
static std::string generate_table_definition(FuzzedDataProvider& fdp, int definition_type) {
    std::string table_def;
    
    switch (definition_type % 8) {
        case 0: // SwapCc operation (character to character swap)
            table_def += "swapCc ";
            table_def += generate_swap_name(fdp);
            table_def += " ";
            table_def += generate_character_string(fdp, true); // matches (characters)
            table_def += " ";
            table_def += generate_character_string(fdp, true); // replacements (characters)
            break;
            
        case 1: // SwapCd operation (character to dots swap)
            table_def += "swapCd ";
            table_def += generate_swap_name(fdp);
            table_def += " ";
            table_def += generate_character_string(fdp, true); // matches (characters)
            table_def += " ";
            table_def += generate_character_string(fdp, false); // replacements (dots)
            break;
            
        case 2: // SwapDd operation (dots to dots swap)
            table_def += "swapDd ";
            table_def += generate_swap_name(fdp);
            table_def += " ";
            table_def += generate_character_string(fdp, false); // matches (dots)
            table_def += " ";
            table_def += generate_character_string(fdp, false); // replacements (dots)
            break;
            
        case 3: // Before attribute with character class
            {
                std::string class_name = generate_character_class_name(fdp);
                table_def += "class " + class_name + "\n";
                table_def += "before " + class_name + "\n";
                table_def += "always a 123";
            }
            break;
            
        case 4: // After attribute with character class
            {
                std::string class_name = generate_character_class_name(fdp);
                table_def += "class " + class_name + "\n";
                table_def += "after " + class_name + "\n";
                table_def += "always b 14";
            }
            break;
            
        case 5: // Hyphen rule
            table_def += "hyphen ";
            table_def += generate_character_string(fdp, true).substr(0, 1); // single character
            table_def += " ";
            table_def += generate_character_string(fdp, false).substr(0, 3); // dot pattern
            break;
            
        case 6: // Decimal point rule
            table_def += "decpoint ";
            table_def += generate_character_string(fdp, true).substr(0, 1); // single character
            table_def += " ";
            table_def += generate_character_string(fdp, false).substr(0, 3); // dot pattern
            break;
            
        case 7: // Complex combination: swap with before/after attributes
            {
                std::string class_name = generate_character_class_name(fdp);
                table_def += "class " + class_name + "\n";
                table_def += "before " + class_name + "\n";
                table_def += "swapCc ";
                table_def += generate_swap_name(fdp);
                table_def += " ";
                table_def += generate_character_string(fdp, true);
                table_def += " ";
                table_def += generate_character_string(fdp, true);
                table_def += "\n";
                table_def += "after " + class_name + "\n";
                table_def += "always c 25";
            }
            break;
    }
    
    return table_def;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Initialize library once
    if (!initialized) {
        lou_registerLogCallback(quiet_log);
        initialized = 1;
    }
    
    if (size < 10) return 0;  // Need minimum input for meaningful testing
    
    FuzzedDataProvider fdp(data, size);
    
    // Generate diverse table definitions based on fuzzer input
    int num_definitions = fdp.ConsumeIntegralInRange<int>(1, 5);
    std::string table_content;
    
    // Add basic character class definitions first (needed for before/after attributes)
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 10) {
        int num_classes = fdp.ConsumeIntegralInRange<int>(0, 3);
        for (int i = 0; i < num_classes; i++) {
            std::string class_name = generate_character_class_name(fdp);
            table_content += "class " + class_name + "\n";
        }
    }
    
    // Generate main rule definitions
    for (int i = 0; i < num_definitions && fdp.remaining_bytes() > 10; i++) {
        int def_type = fdp.ConsumeIntegral<uint8_t>() % 8;
        table_content += generate_table_definition(fdp, def_type);
        table_content += "\n";
        
        // Add some basic rules to ensure valid table structure
        if (fdp.ConsumeBool() && fdp.remaining_bytes() > 5) {
            table_content += "always ";
            table_content += generate_character_string(fdp, true).substr(0, 1);
            table_content += " ";
            table_content += generate_character_string(fdp, false).substr(0, 3);
            table_content += "\n";
        }
    }
    
    // Ensure we have at least one valid rule if table is empty
    if (table_content.empty() && fdp.remaining_bytes() > 5) {
        table_content = "always a 123\n";
    }
    
    // Try to compile the generated table
    if (!table_content.empty()) {
        // Use a fixed table name to avoid file system dependencies
        const char* table_list = "test_table";
        
        // Compile the table string
        int result = lou_compileString(table_list, table_content.c_str());
        
        // Clean up if compilation succeeded
        if (result == 1) {
            lou_free();
        }
    }
    
    return 0;
}

} // namespace harness_022
