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

namespace harness_009 {
/*
 * Fuzzing harness for liblouis translation table compilation targeting CTO_Macro and CTO_Grouping opcodes
 * Targets: 515 blocked branches in compileRule function (compileTranslationTable.c)
 * Primary API: lou_compileString (direct entry to compileRule)
 * Strategy: Generate translation table syntax with macro features, grouping operations, and character class definitions
 *           to exercise blocked opcode cases in compileRule function
 * 
 * Specific opcodes to target:
 * 1. CTO_Macro (macro definitions and calls) - lines 2893-2909 in compileTranslationTable.c
 * 2. CTO_Grouping (grouping operations) - line 2955 in compileTranslationTable.c  
 * 3. CTO_Space, CTO_Digit, CTO_Punctuation, etc. (character class definitions)
 * 4. ISO/UTF-8 header processing - lines 2881-2890 in compileTranslationTable.c
 * 
 * Invocation sequence:
 * 1. lou_setDataPath (configure table search directory)
 * 2. lou_registerLogCallback (optional error logging)
 * 3. lou_compileString with complex table syntax containing macros, grouping, character classes
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
 * - harness_008: Complex translation rule compilation targeting compileRule
 * - harness_009: Macro features, grouping operations, character class definitions targeting specific blocked branches (current)
 */



// Log callback to avoid excessive logging during fuzzing
static void quiet_log(logLevels level, const char* msg) {
    (void)level;
    (void)msg;
}

// Global initialization flag
static int initialized = 0;

// Helper to generate macro definitions with proper syntax
static std::string generate_macro_definition(FuzzedDataProvider& fdp, int macro_id) {
    std::string macro;
    
    // Macro definition format: macro <name>
    macro += "macro macro_";
    macro += std::to_string(macro_id);
    macro += "\n";
    
    // Add some macro content (variable number of rules)
    int num_rules = fdp.ConsumeIntegralInRange<int>(1, 10);
    
    for (int i = 0; i < num_rules; i++) {
        // Choose rule type
        uint8_t rule_type = fdp.ConsumeIntegral<uint8_t>() % 6;
        
        switch (rule_type) {
            case 0: // Character definition
                macro += "letter ";
                macro += static_cast<char>('a' + (fdp.ConsumeIntegral<uint8_t>() % 26));
                macro += " @ ";
                macro += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 1234567));
                break;
                
            case 1: // Digit definition
                macro += "digit ";
                macro += static_cast<char>('0' + (fdp.ConsumeIntegral<uint8_t>() % 10));
                macro += " @ ";
                macro += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 1234567));
                break;
                
            case 2: // Punctuation definition
                macro += "punctuation ";
                macro += static_cast<char>('!' + (fdp.ConsumeIntegral<uint8_t>() % 15));
                macro += " ";
                macro += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 1234567));
                break;
                
            case 3: // Space definition
                macro += "space \\";
                macro += static_cast<char>('a' + (fdp.ConsumeIntegral<uint8_t>() % 26));
                macro += " ";
                macro += std::to_string(fdp.ConsumeIntegralInRange<int>(0, 9));
                break;
                
            case 4: // Sign definition
                macro += "sign ";
                macro += static_cast<char>('!' + (fdp.ConsumeIntegral<uint8_t>() % 15));
                macro += " ";
                macro += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 1234567));
                break;
                
            case 5: // Math symbol
                macro += "math ";
                macro += static_cast<char>('+' + (fdp.ConsumeIntegral<uint8_t>() % 10));
                macro += " ";
                macro += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 1234567));
                break;
        }
        macro += "\n";
    }
    
    // End macro with eom
    macro += "eom\n\n";
    return macro;
}

// Helper to generate grouping operations
static std::string generate_grouping_operation(FuzzedDataProvider& fdp, int group_id) {
    std::string grouping;
    
    // Grouping format: grouping (chars) @ dots
    grouping += "grouping (";
    
    // Generate character sequence for grouping
    int group_len = fdp.ConsumeIntegralInRange<int>(1, 5);
    for (int i = 0; i < group_len; i++) {
        grouping += static_cast<char>('a' + (fdp.ConsumeIntegral<uint8_t>() % 26));
    }
    
    grouping += ") @ ";
    grouping += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 1234567));
    grouping += "\n";
    
    return grouping;
}

// Helper to generate character class definitions
static std::string generate_char_class_definition(FuzzedDataProvider& fdp) {
    std::string char_class;
    
    uint8_t class_type = fdp.ConsumeIntegral<uint8_t>() % 8;
    
    switch (class_type) {
        case 0:
            char_class += "space ";
            break;
        case 1:
            char_class += "digit ";
            break;
        case 2:
            char_class += "litdigit ";
            break;
        case 3:
            char_class += "punctuation ";
            break;
        case 4:
            char_class += "math ";
            break;
        case 5:
            char_class += "sign ";
            break;
        case 6:
            char_class += "letter ";
            break;
        case 7:
            char_class += "uppercase ";
            break;
    }
    
    // Add character
    char_class += static_cast<char>('!' + (fdp.ConsumeIntegral<uint8_t>() % 90));
    
    // Add dots or class specification
    if (class_type == 0 || class_type == 1 || class_type == 2) { // space, digit, litdigit
        char_class += " ";
        char_class += std::to_string(fdp.ConsumeIntegralInRange<int>(0, 9));
    } else if (class_type == 3 || class_type == 4 || class_type == 5) { // punctuation, math, sign
        char_class += " ";
        char_class += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 1234567));
    } else if (class_type == 6 || class_type == 7) { // letter, uppercase
        char_class += " @ ";
        char_class += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 1234567));
    }
    
    char_class += "\n";
    return char_class;
}

// Helper to generate complete table compilation with targeted features
static std::string generate_table_with_targeted_features(FuzzedDataProvider& fdp) {
    std::string table_content;
    
    // Optionally add ISO/UTF-8 header on first line (to target lines 2881-2890)
    if (fdp.ConsumeBool()) {
        if (fdp.ConsumeBool()) {
            table_content += "ISO\n";
        } else {
            table_content += "UTF-8\n";
        }
    }
    
    // Add table header/comments
    table_content += "# Targeted fuzzing table for CTO_Macro and CTO_Grouping\n";
    table_content += "# Generated by harness_009\n\n";
    
    // Include a base table if needed
    if (fdp.ConsumeBool()) {
        table_content += "include en-ueb-g1.ctb\n\n";
    }
    
    // Add character class definitions (CTO_Space, CTO_Digit, CTO_Punctuation, etc.)
    if (fdp.ConsumeBool()) {
        table_content += "# Character class definitions\n";
        int num_char_classes = fdp.ConsumeIntegralInRange<int>(1, 20);
        for (int i = 0; i < num_char_classes; i++) {
            table_content += generate_char_class_definition(fdp);
        }
        table_content += "\n";
    }
    
    // Add macro definitions (CTO_Macro)
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 100) {
        table_content += "# Macro definitions\n";
        int num_macros = fdp.ConsumeIntegralInRange<int>(1, 5);
        for (int i = 0; i < num_macros; i++) {
            table_content += generate_macro_definition(fdp, i);
        }
    }
    
    // Add grouping operations (CTO_Grouping)
    if (fdp.ConsumeBool()) {
        table_content += "# Grouping operations\n";
        int num_groupings = fdp.ConsumeIntegralInRange<int>(1, 10);
        for (int i = 0; i < num_groupings; i++) {
            table_content += generate_grouping_operation(fdp, i);
        }
        table_content += "\n";
    }
    
    // Add standard translation rules for completeness
    if (fdp.ConsumeBool()) {
        table_content += "# Standard translation rules\n";
        int num_rules = fdp.ConsumeIntegralInRange<int>(5, 30);
        for (int i = 0; i < num_rules; i++) {
            // Simple character mappings
            table_content += "letter ";
            table_content += static_cast<char>('a' + (fdp.ConsumeIntegral<uint8_t>() % 26));
            table_content += " @ ";
            table_content += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 1234567));
            table_content += "\n";
        }
        table_content += "\n";
    }
    
    // Add emphasis class definitions if enabled
    if (fdp.ConsumeBool()) {
        table_content += "# Emphasis classes\n";
        table_content += "emphclass 1 bold\n";
        table_content += "emphclass 2 italic\n";
        table_content += "emphclass 3 underline\n\n";
    }
    
    // Add multipass indicator
    if (fdp.ConsumeBool()) {
        table_content += "multipass 1\n\n";
    }
    
    // Add nocont/noback/nofor indicators
    if (fdp.ConsumeBool()) {
        if (fdp.ConsumeBool()) {
            table_content += "nocont\n";
        }
        if (fdp.ConsumeBool()) {
            table_content += "noback\n";
        }
        if (fdp.ConsumeBool()) {
            table_content += "nofor\n";
        }
        table_content += "\n";
    }
    
    // Add display rules
    if (fdp.ConsumeBool()) {
        table_content += "# Display rules\n";
        table_content += "display\n";
        table_content += "dots46 @ 46\n";
        table_content += "dots123 @ 123\n\n";
    }
    
    return table_content;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size check - need enough for meaningful table generation
    if (size < 50) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize liblouis once
    if (!initialized) {
        lou_registerLogCallback(quiet_log);
        initialized = 1;
    }
    
    // Set data path to point to tables directory (required for table resolution)
    std::string data_path = "/root/src/liblouis/tables";
    char* path_result = lou_setDataPath(data_path.c_str());
    (void)path_result; // Result may be NULL, that's okay
    
    // Generate table compilation content with targeted features
    std::string table_list = "targeted_table.ctb";
    std::string compilation_string = generate_table_with_targeted_features(fdp);
    
    // Test different compilation scenarios based on input
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 3;
    
    int result = 0;
    
    switch (operation) {
        case 0: // Basic compilation test with targeted features
            result = lou_compileString(table_list.c_str(), compilation_string.c_str());
            // Clean up if compilation succeeded
            if (result == 1) {
                lou_checkTable(table_list.c_str());
            }
            break;
            
        case 1: // Multiple compilations with different content
            // First compilation
            result = lou_compileString("table_macro.ctb", compilation_string.c_str());
            
            // Generate another compilation with different features
            if (fdp.remaining_bytes() > 50) {
                std::string compilation2 = generate_table_with_targeted_features(fdp);
                int result2 = lou_compileString("table_grouping.ctb", compilation2.c_str());
                (void)result2;
            }
            break;
            
        case 2: // Edge cases and error paths
            // Test with empty string
            lou_compileString(table_list.c_str(), "");
            
            // Test with very long string (if enough input)
            if (fdp.remaining_bytes() > 500) {
                std::string long_string = fdp.ConsumeRemainingBytesAsString();
                lou_compileString(table_list.c_str(), long_string.c_str());
            }
            break;
    }
    
    // Always cleanup to free allocated tables
    lou_free();
    
    return 0;
}

} // namespace harness_009
