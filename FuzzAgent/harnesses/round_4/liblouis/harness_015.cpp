#include <fuzzer/FuzzedDataProvider.h>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include "liblouis.h"

namespace harness_015 {
// liblouis compileRule specialized fuzzing harness
// Target: compileRule function with focus on grouping directives, macro compilation, and hyphenation testing
// Coverage analysis shows compileRule still has 568 blocked branches
// Primary API: lou_compileString (directly exercises compileRule)
// Secondary APIs: lou_checkTable (validation), lou_freeTableFile (cleanup)
// Focus on semantic differentiation from harness_013 (macro/grouping) and harness_014 (emphasis/sequence)
// Specifically targets: CTO_Grouping, CTO_Macro, compileHyphenation, and complex opcode combinations


// Include liblouis public header
extern "C" {
}

// Log callback to suppress logs during fuzzing
static void avoid_log_callback(logLevels level, const char *msg) {
    (void)level;
    (void)msg;
}

// Helper to generate grouping directives (CTO_Grouping)
static std::string generate_grouping_directives(FuzzedDataProvider& fdp) {
    std::string content;
    
    // Generate 0-5 grouping definitions
    size_t num_groups = fdp.ConsumeIntegralInRange<size_t>(0, 5);
    for (size_t i = 0; i < num_groups; i++) {
        content += "grouping ";
        
        // Generate group name
        size_t name_len = fdp.ConsumeIntegralInRange<size_t>(1, 12);
        for (size_t j = 0; j < name_len; j++) {
            char c = fdp.ConsumeIntegralInRange<char>('a', 'z');
            content += c;
        }
        content += " ";
        
        // Generate two Unicode characters (simple ASCII range)
        char char1 = fdp.ConsumeIntegralInRange<char>('a', 'z');
        char char2 = fdp.ConsumeIntegralInRange<char>('a', 'z');
        content += char1;
        content += char2;
        content += " ";
        
        // Generate two cells separated by comma
        uint8_t cell1 = fdp.ConsumeIntegralInRange<uint8_t>(1, 8);
        uint8_t cell2 = fdp.ConsumeIntegralInRange<uint8_t>(1, 8);
        content += std::to_string(cell1) + "," + std::to_string(cell2);
        content += "\n";
    }
    
    return content;
}

// Helper to generate macro definitions (CTO_Macro)
static std::string generate_macro_definitions(FuzzedDataProvider& fdp) {
    std::string content;
    
    // Generate 0-3 macro definitions
    size_t num_macros = fdp.ConsumeIntegralInRange<size_t>(0, 3);
    for (size_t i = 0; i < num_macros; i++) {
        content += "macro ";
        
        // Generate macro name
        size_t macro_name_len = fdp.ConsumeIntegralInRange<size_t>(1, 12);
        for (size_t j = 0; j < macro_name_len; j++) {
            char c = fdp.ConsumeIntegralInRange<char>('a', 'z');
            content += c;
        }
        content += " eom\n";
        
        // Generate macro body with various opcodes (simplified)
        size_t body_lines = fdp.ConsumeIntegralInRange<size_t>(1, 5);
        for (size_t j = 0; j < body_lines; j++) {
            uint8_t line_type = fdp.ConsumeIntegral<uint8_t>() % 5;
            switch (line_type) {
                case 0:
                    content += "space ";
                    content += fdp.ConsumeIntegralInRange<char>('a', 'z');
                    content += " ";
                    content += std::to_string(fdp.ConsumeIntegralInRange<uint8_t>(1, 255));
                    content += "\n";
                    break;
                case 1:
                    content += "digit ";
                    content += fdp.ConsumeIntegralInRange<char>('0', '9');
                    content += " ";
                    content += std::to_string(fdp.ConsumeIntegralInRange<uint16_t>(1, 65535));
                    content += "\n";
                    break;
                case 2:
                    content += "letter ";
                    content += fdp.ConsumeIntegralInRange<char>('a', 'z');
                    content += " ";
                    content += std::to_string(fdp.ConsumeIntegralInRange<uint8_t>(1, 255));
                    content += "\n";
                    break;
                case 3:
                    content += "class ";
                    content += fdp.ConsumeIntegralInRange<char>('a', 'z');
                    content += " ";
                    content += fdp.ConsumeRandomLengthString(8);
                    content += "\n";
                    break;
                case 4:
                    content += "noback ";
                    content += fdp.ConsumeIntegralInRange<char>('a', 'z');
                    content += " ";
                    content += fdp.ConsumeIntegralInRange<char>('a', 'z');
                    content += " ";
                    content += std::to_string(fdp.ConsumeIntegralInRange<uint16_t>(1, 65535));
                    content += "\n";
                    break;
            }
        }
        content += "em\n";
    }
    
    return content;
}

// Helper to generate hyphenation patterns (compileHyphenation)
static std::string generate_hyphenation_patterns(FuzzedDataProvider& fdp) {
    std::string content;
    
    // Add hyphenation table header
    content += "ISO\n";
    
    // Generate 0-10 hyphenation patterns
    size_t num_patterns = fdp.ConsumeIntegralInRange<size_t>(0, 10);
    for (size_t i = 0; i < num_patterns; i++) {
        // Generate a word pattern with digit markers for hyphenation
        std::string pattern;
        size_t word_len = fdp.ConsumeIntegralInRange<size_t>(2, 10);
        
        for (size_t j = 0; j < word_len; j++) {
            // Add letters with occasional digit markers for hyphenation points
            if (fdp.ConsumeBool() && j > 0 && j < word_len - 1) {
                // Add hyphenation point digit (0-9)
                pattern += std::to_string(fdp.ConsumeIntegralInRange<uint8_t>(0, 9));
            }
            
            // Add a letter
            char letter = fdp.ConsumeIntegralInRange<char>('a', 'z');
            pattern += letter;
        }
        
        // Ensure pattern ends with at least one letter
        if (pattern.empty() || !isalpha(pattern.back())) {
            pattern += fdp.ConsumeIntegralInRange<char>('a', 'z');
        }
        
        content += pattern + "\n";
    }
    
    return content;
}

// Helper to generate complex opcode combinations
static std::string generate_complex_opcode_combinations(FuzzedDataProvider& fdp) {
    std::string content;
    
    // Add emphasis class definitions (required for emphasis opcodes)
    if (fdp.ConsumeBool()) {
        content += "emphclass italic\n";
        content += "emphclass underline\n";
        content += "emphclass bold\n";
    }
    
    // Add emphasis rules if classes are defined
    if (fdp.ConsumeBool() && !content.empty()) {
        const char* emph_opcodes[] = {"emphletter", "begemph", "endemph", "begemphword", "endemphword"};
        size_t num_emph_rules = fdp.ConsumeIntegralInRange<size_t>(0, 5);
        
        for (size_t i = 0; i < num_emph_rules; i++) {
            uint8_t opcode_idx = fdp.ConsumeIntegralInRange<uint8_t>(0, 4);
            content += emph_opcodes[opcode_idx] + std::string(" ");
            
            // Choose emphasis class
            const char* emph_classes[] = {"italic", "underline", "bold"};
            uint8_t class_idx = fdp.ConsumeIntegralInRange<uint8_t>(0, 2);
            content += emph_classes[class_idx] + std::string(" ");
            
            // Add rule pattern
            content += fdp.ConsumeIntegralInRange<char>('a', 'z');
            content += " ";
            content += std::to_string(fdp.ConsumeIntegralInRange<uint8_t>(1, 8));
            content += "\n";
        }
    }
    
    // Add sequence opcodes
    if (fdp.ConsumeBool()) {
        content += "seqdelimiter ";
        content += fdp.ConsumeIntegralInRange<char>('!', '~');
        content += "\n";
    }
    
    if (fdp.ConsumeBool()) {
        content += "seqbeforechars ";
        size_t before_len = fdp.ConsumeIntegralInRange<size_t>(1, 6);
        for (size_t i = 0; i < before_len; i++) {
            content += fdp.ConsumeIntegralInRange<char>('a', 'z');
        }
        content += "\n";
    }
    
    if (fdp.ConsumeBool()) {
        content += "seqafterchars ";
        size_t after_len = fdp.ConsumeIntegralInRange<size_t>(1, 6);
        for (size_t i = 0; i < after_len; i++) {
            content += fdp.ConsumeIntegralInRange<char>('a', 'z');
        }
        content += "\n";
    }
    
    // Add numeric mode opcodes
    if (fdp.ConsumeBool()) {
        content += "numbersign ";
        content += fdp.ConsumeIntegralInRange<char>('0', '9');
        content += " ";
        content += std::to_string(fdp.ConsumeIntegralInRange<uint8_t>(1, 8));
        content += "\n";
    }
    
    if (fdp.ConsumeBool()) {
        content += "numericmodechars ";
        size_t num_len = fdp.ConsumeIntegralInRange<size_t>(1, 6);
        for (size_t i = 0; i < num_len; i++) {
            content += fdp.ConsumeIntegralInRange<char>('0', '9');
        }
        content += "\n";
    }
    
    return content;
}

// Main table content generator
static std::string generate_specialized_table_content(FuzzedDataProvider& fdp) {
    std::string content;
    
    // Start with table metadata
    content += "table compileRule-specialized-test\n";
    content += "locale en_US\n";
    content += "author fuzzer-generated-harness_015\n";
    
    // Decide table type: regular translation table or hyphenation table
    bool is_hyphenation_table = fdp.ConsumeBool();
    
    if (is_hyphenation_table) {
        // Generate hyphenation table content
        content += generate_hyphenation_patterns(fdp);
    } else {
        // Generate regular translation table with specialized features
        content += generate_grouping_directives(fdp);
        content += generate_macro_definitions(fdp);
        content += generate_complex_opcode_combinations(fdp);
        
        // Add some basic character mappings to ensure valid table structure
        size_t num_basic_rules = fdp.ConsumeIntegralInRange<size_t>(0, 10);
        for (size_t i = 0; i < num_basic_rules; i++) {
            content += fdp.ConsumeIntegralInRange<char>('a', 'z');
            content += " ";
            content += std::to_string(fdp.ConsumeIntegralInRange<uint8_t>(1, 8));
            content += "\n";
        }
    }
    
    return content;
}
int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough for complex table generation
    if (size < 64) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize liblouis once
    static bool initialized = false;
    if (!initialized) {
        lou_registerLogCallback(avoid_log_callback);
        initialized = true;
    }
    
    // Generate specialized table content targeting compileRule blocked branches
    std::string table_content = generate_specialized_table_content(fdp);
    
    if (table_content.empty()) {
        return 0;
    }
    
    // Convert to C string for liblouis API
    const char* table_content_cstr = table_content.c_str();
    
    // Create a unique table name to avoid conflicts
    std::string table_name = "fuzz_compileRule_";
    for (int i = 0; i < 8; i++) {
        char hex_digit = "0123456789abcdef"[fdp.ConsumeIntegralInRange<uint8_t>(0, 15)];
        table_name += hex_digit;
    }
    table_name += ".ctb";

    // 1. Compile the table string using lou_compileString (primary target)
    // This directly exercises the compileRule function
    int compile_result = lou_compileString(table_name.c_str(), table_content_cstr);

    // 2. Check if table compilation was successful
    if (compile_result == 1) {
        // Table compiled successfully, perform additional validation
        
        // 3. Check table validity using lou_checkTable
        int check_result = lou_checkTable(table_name.c_str());
        
        // Note: check_result may be 0 (invalid) or 1 (valid)
        // We don't need to act on the result for fuzzing purposes
    }


    return 0;
}

} // namespace harness_015
