#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <vector>
#include <string>
#include <algorithm>
#include <fuzzer/FuzzedDataProvider.h>
#include "liblouis.h"

namespace harness_016 {
// Targeted liblouis fuzzing harness for compileRule function with 581 blocked branches (63.8% blocked)
// Harness_016: Specifically targets remaining blocked branches in compileRule with focus on deep table compilation
// Focus: Translation table compilation with various opcode types, complex pattern matching, emphasis classes,
//        character attributes, exact dots, capsnocont, multind, swap operations, and complex translation scenarios
// Strategy: Generate semantically diverse table entries targeting opcode combinations not well-covered by harnesses 012 and 015
// Differentiation: Deep exploration of pattern compilation, character classes, emphasis logic, and special opcode handling
// Target: Unlock 815+ accumulated branches across related compilation functions




// Log callback to suppress output during fuzzing
static void fuzz_log_callback(logLevels level, const char* message) {
    (void)level;
    (void)message;
    // Suppress all logging during fuzzing
}

// Generate complex pattern for match/backmatch opcodes (different from previous harnesses)
static std::string generate_complex_pattern(FuzzedDataProvider& fdp) {
    std::string pattern;
    uint8_t pattern_type = fdp.ConsumeIntegral<uint8_t>() % 6;
    
    switch (pattern_type) {
        case 0: {
            // Single dash (wildcard)
            pattern = "-";
            break;
        }
        case 1: {
            // Character class with range
            pattern = "[";
            pattern += static_cast<char>(32 + (fdp.ConsumeIntegral<uint8_t>() % 26)); // A-Z
            pattern += "-";
            pattern += static_cast<char>(32 + (fdp.ConsumeIntegral<uint8_t>() % 26) + 26); // a-z
            pattern += "]";
            break;
        }
        case 2: {
            // Negated character class with multiple chars
            pattern = "[^";
            int char_count = fdp.ConsumeIntegralInRange<int>(1, 5);
            for (int i = 0; i < char_count; i++) {
                pattern += static_cast<char>(32 + (fdp.ConsumeIntegral<uint8_t>() % 95));
            }
            pattern += "]";
            break;
        }
        case 3: {
            // Complex pattern with escapes
            pattern = "\\[";
            pattern += static_cast<char>(32 + (fdp.ConsumeIntegral<uint8_t>() % 95));
            pattern += "\\]";
            break;
        }
        case 4: {
            // Mixed pattern with special chars
            pattern = "[:alpha:]";
            break;
        }
        case 5: {
            // Empty pattern (edge case)
            pattern = "";
            break;
        }
    }
    
    return pattern;
}

// Generate emphasis class name (for emphasis-related opcodes)
static std::string generate_emphasis_class(FuzzedDataProvider& fdp) {
    const char* emphasis_classes[] = {
        "emphasis", "italic", "bold", "underline", "strikethrough",
        "superscript", "subscript", "outline", "shadow", "blink"
    };
    
    uint8_t idx = fdp.ConsumeIntegral<uint8_t>() % (sizeof(emphasis_classes)/sizeof(emphasis_classes[0]));
    return std::string(emphasis_classes[idx]);
}

// Generate character class definition
static std::string generate_character_class(FuzzedDataProvider& fdp) {
    std::string class_def;
    uint8_t class_type = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    switch (class_type) {
        case 0: {
            // Simple character class
            class_def = "class ";
            class_def += static_cast<char>('A' + (fdp.ConsumeIntegral<uint8_t>() % 26));
            class_def += " ";
            break;
        }
        case 1: {
            // Character class with multiple chars
            class_def = "class ";
            int char_count = fdp.ConsumeIntegralInRange<int>(2, 6);
            for (int i = 0; i < char_count; i++) {
                class_def += static_cast<char>(32 + (fdp.ConsumeIntegral<uint8_t>() % 95));
                if (i < char_count - 1) class_def += " ";
            }
            class_def += " ";
            break;
        }
        case 2: {
            // Character class with attribute
            class_def = "class attr_";
            class_def += std::to_string(fdp.ConsumeIntegral<uint8_t>() % 10);
            class_def += " ";
            class_def += static_cast<char>(32 + (fdp.ConsumeIntegral<uint8_t>() % 95));
            class_def += " ";
            break;
        }
        case 3: {
            // Empty class (edge case)
            class_def = "class";
            break;
        }
    }
    
    return class_def;
}

// Generate attribute definition
static std::string generate_attribute(FuzzedDataProvider& fdp) {
    std::string attr_def = "attribute ";
    attr_def += static_cast<char>('A' + (fdp.ConsumeIntegral<uint8_t>() % 26));
    attr_def += " ";
    
    uint8_t attr_type = fdp.ConsumeIntegral<uint8_t>() % 3;
    switch (attr_type) {
        case 0: {
            attr_def += "before";
            break;
        }
        case 1: {
            attr_def += "after";
            break;
        }
        case 2: {
            attr_def += "both";
            break;
        }
    }

    
    return attr_def;
}

// Generate exact dots pattern (starts with @)
static std::string generate_exact_dots(FuzzedDataProvider& fdp) {
    std::string dots = "@";
    int dot_count = fdp.ConsumeIntegralInRange<int>(1, 4);
    
    for (int i = 0; i < dot_count; i++) {
        uint8_t dot_value = fdp.ConsumeIntegral<uint8_t>() % 256;
        dots += std::to_string(dot_value);
        if (i < dot_count - 1) {
            dots += "-";
        }
    }
    
    return dots;
}

// Generate swap operation
static std::string generate_swap_op(FuzzedDataProvider& fdp) {
    const char* swap_ops[] = {
        "swapcc", "swapcd", "swapdd"
    };
    
    uint8_t idx = fdp.ConsumeIntegral<uint8_t>() % (sizeof(swap_ops)/sizeof(swap_ops[0]));
    return std::string(swap_ops[idx]);
}

// Generate multind (multiple braille indicators) pattern
static std::string generate_multind(FuzzedDataProvider& fdp) {
    std::string multind = "multind ";
    
    // Generate dots pattern
    int dot_count = fdp.ConsumeIntegralInRange<int>(1, 3);
    for (int i = 0; i < dot_count; i++) {
        uint8_t dot_value = fdp.ConsumeIntegral<uint8_t>() % 256;
        multind += std::to_string(dot_value);
        if (i < dot_count - 1) {
            multind += "-";
        }
    }
    multind += " ";
    
    // Generate opcodes
    int opcode_count = fdp.ConsumeIntegralInRange<int>(1, 3);
    const char* multind_opcodes[] = {
        "space", "digit", "letter", "punctuation", "sign", "math"
    };
    
    for (int i = 0; i < opcode_count; i++) {
        uint8_t op_idx = fdp.ConsumeIntegral<uint8_t>() % (sizeof(multind_opcodes)/sizeof(multind_opcodes[0]));
        multind += multind_opcodes[op_idx];
        if (i < opcode_count - 1) {
            multind += " ";
        }
    }
    
    return multind;
}

// Generate complex table rule with various opcodes
static std::string generate_complex_rule(FuzzedDataProvider& fdp) {
    std::string rule;
    uint8_t rule_type = fdp.ConsumeIntegral<uint8_t>() % 8;
    
    switch (rule_type) {
        case 0: {
            // Match rule with complex patterns
            rule = "match ";
            rule += generate_complex_pattern(fdp);
            rule += " ";
            rule += static_cast<char>(32 + (fdp.ConsumeIntegral<uint8_t>() % 95));
            rule += " ";
            rule += generate_complex_pattern(fdp);
            rule += " ";
            rule += std::to_string(fdp.ConsumeIntegral<uint8_t>() % 256);
            break;
        }
        case 1: {
            // Backmatch rule
            rule = "backmatch ";
            rule += generate_complex_pattern(fdp);
            rule += " ";
            rule += static_cast<char>(32 + (fdp.ConsumeIntegral<uint8_t>() % 95));
            rule += " ";
            rule += generate_complex_pattern(fdp);
            rule += " ";
            rule += std::to_string(fdp.ConsumeIntegral<uint8_t>() % 256);
            break;
        }
        case 2: {
            // Emphasis class definition
            rule = "emphclass ";
            rule += generate_emphasis_class(fdp);
            break;
        }
        case 3: {
            // Character class definition
            rule = generate_character_class(fdp);
            break;
        }
        case 4: {
            // Attribute definition
            rule = generate_attribute(fdp);
            break;
        }
        case 5: {
            // Exact dots
            rule = "exactdots ";
            rule += static_cast<char>(32 + (fdp.ConsumeIntegral<uint8_t>() % 95));
            rule += " ";
            rule += generate_exact_dots(fdp);
            break;
        }
        case 6: {
            // Capsnocont
            rule = "capsnocont";
            break;
        }
        case 7: {
            // Multind
            rule = generate_multind(fdp);
            break;
        }
    }
    
    return rule;
}

// Generate table with complex compilation scenarios
static std::string generate_complex_table(FuzzedDataProvider& fdp) {
    std::string table_content;
    
    // Table header
    table_content += "## Complex translation table for deep compilation testing\n\n";
    
    // Add some character definitions first (required for some opcodes)
    table_content += "space 32\n";
    table_content += "digit 48-57\n";
    table_content += "letter a-zA-Z\n";
    table_content += "punctuation !\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~\n";
    
    // Generate multiple complex rules
    int rule_count = fdp.ConsumeIntegralInRange<int>(5, 15);
    for (int i = 0; i < rule_count; i++) {
        table_content += generate_complex_rule(fdp);
        table_content += "\n";
        
        // Occasionally add swap operations
        if (fdp.ConsumeBool() && fdp.remaining_bytes() > 10) {
            table_content += generate_swap_op(fdp);
            table_content += " ";
            table_content += static_cast<char>(32 + (fdp.ConsumeIntegral<uint8_t>() % 95));
            table_content += " ";
            table_content += std::to_string(fdp.ConsumeIntegral<uint8_t>() % 256);
            table_content += "\n";
        }
        
        // Occasionally add hyphen or decpoint
        if (fdp.ConsumeBool() && fdp.remaining_bytes() > 5) {
            if (fdp.ConsumeBool()) {
                table_content += "hyphen ";
            } else {
                table_content += "decpoint ";
            }
            table_content += static_cast<char>(32 + (fdp.ConsumeIntegral<uint8_t>() % 95));
            table_content += "\n";
        }
    }
    
    // Add some pass opcodes with nofor/noback
    if (fdp.ConsumeBool()) {
        table_content += "nofor\n";
        table_content += "pass2\n";
        table_content += "context a 1\n";
    } else if (fdp.ConsumeBool()) {
        table_content += "noback\n";
        table_content += "pass3\n";
        table_content += "correct\n";
    }
    
    // Add contraction/nocont/compbrl/literal rules
    if (fdp.remaining_bytes() > 20) {
        const char* cont_ops[] = {"contraction", "nocont", "compbrl", "literal"};
        uint8_t cont_idx = fdp.ConsumeIntegral<uint8_t>() % (sizeof(cont_ops)/sizeof(cont_ops[0]));
        table_content += cont_ops[cont_idx];
        table_content += " ";
        
        int char_count = fdp.ConsumeIntegralInRange<int>(1, 3);
        for (int i = 0; i < char_count; i++) {
            table_content += static_cast<char>(97 + (fdp.ConsumeIntegral<uint8_t>() % 26)); // a-z
        }
        table_content += "\n";
    }
    
    // Add replace rule
    if (fdp.remaining_bytes() > 10) {
        table_content += "replace ";
        table_content += static_cast<char>(32 + (fdp.ConsumeIntegral<uint8_t>() % 95));
        table_content += " ";
        table_content += std::to_string(fdp.ConsumeIntegral<uint8_t>() % 256);
        table_content += "\n";
    }
    
    // Add empmatchbefore/empmatchafter
    if (fdp.ConsumeBool()) {
        if (fdp.ConsumeBool()) {
            table_content += "empmatchbefore ";
        } else {
            table_content += "empmatchafter ";
        }
        table_content += static_cast<char>(32 + (fdp.ConsumeIntegral<uint8_t>() % 95));
        table_content += " ";
        table_content += std::to_string(fdp.ConsumeIntegral<uint8_t>() % 256);
        table_content += "\n";
    }
    
    return table_content;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 64) {
        return 0;  // Need enough data for meaningful table generation
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Set log callback to suppress output
    lou_registerLogCallback(fuzz_log_callback);
    
    // Generate complex table content
    std::string table_content = generate_complex_table(fdp);
    
    if (table_content.empty()) {
        return 0;
    }
    
    // Create a temporary file for the table
    char temp_filename[] = "/tmp/liblouis_fuzz_table_XXXXXX";
    int fd = mkstemp(temp_filename);
    if (fd == -1) {
        return 0;
    }
    
    // Write table content to temporary file
    ssize_t written = write(fd, table_content.c_str(), table_content.length());
    close(fd);
    
    if (written != (ssize_t)table_content.length()) {
        unlink(temp_filename);
        return 0;
    }
    
    // Try to compile the table
    int result = 0;
    const char* table_list[] = {temp_filename, NULL};
    
    // Test compilation with lou_getTable - returns const void*
    const void* table_ptr = lou_getTable(temp_filename);
    if (table_ptr) {
        result = 1;
    }
    
    // Test compilation with lou_compileString
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 10) {
        // Create another table with different content
        std::string table2 = generate_complex_table(fdp);
        if (!table2.empty()) {
            // lou_compileString takes tableList and inString parameters
            lou_compileString(temp_filename, table2.c_str());
        }
    }
    
    // Clean up
    unlink(temp_filename);
    
    return result;
}

} // namespace harness_016
