#include <fuzzer/FuzzedDataProvider.h>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>
#include <algorithm>
#include "liblouis.h"

namespace harness_020 {
// liblouis compileRule sequence opcode deep coverage harness
// Target: compileRule function (580 blocked branches) focusing on complex translation table 
//         compilation scenarios, specifically sequence-related opcodes that involve
//         pattern matching, boundary checking, and expression parsing
// Primary API chain: lou_compileString -> compileString -> compileRule -> sequence opcode handling
// Based on coverage analysis guidance to improve branch coverage from current 62.07%
// Targeting sequence opcodes: CTO_SeqDelimiter, CTO_SeqBeforeChars, CTO_SeqAfterChars,
//                             CTO_SeqAfterPattern, CTO_SeqAfterExpression
// Semantic differentiation from harness_019 (match/backmatch), harness_017 (grouping),
// harness_010 (pattern expressions): Focuses specifically on sequence handling logic
// which has complex boundary conditions and pattern compilation


// Include liblouis public header
extern "C" {
}

// Log callback to suppress logs during fuzzing
static void avoid_log_callback(logLevels level, const char *msg) {
    (void)level;
    (void)msg;
    // Suppress all logs during fuzzing
}

// Helper to generate sequence delimiter definition
// CTO_SeqDelimiter: Marks characters as sequence delimiters
static std::string generate_seq_delimiter_rule(FuzzedDataProvider& fdp) {
    std::string rule = "seqdelimiter ";
    
    // Generate delimiter characters (1-3 characters)
    size_t num_chars = fdp.ConsumeIntegralInRange<size_t>(1, 3);
    for (size_t i = 0; i < num_chars; i++) {
        // Choose between regular characters, escaped hex, or Unicode
        uint8_t char_type = fdp.ConsumeIntegral<uint8_t>() % 3;
        
        if (char_type == 0) {
            // Regular ASCII character
            rule += fdp.ConsumeIntegralInRange<char>('!', '~');
        } else if (char_type == 1) {
            // Escaped hex character
            rule += "\\x";
            char hex[3];
            snprintf(hex, sizeof(hex), "%02x", fdp.ConsumeIntegralInRange<uint8_t>(0x20, 0x7e));
            rule += hex;
        } else {
            // Unicode character (escaped)
            rule += "\\u";
            char hex[5];
            snprintf(hex, sizeof(hex), "%04x", fdp.ConsumeIntegralInRange<uint16_t>(0x0041, 0x007a));
            rule += hex;
        }
    }
    
    return rule + "\n";
}

// Helper to generate sequence before characters definition
// CTO_SeqBeforeChars: Marks characters that can appear before sequences
static std::string generate_seq_before_chars_rule(FuzzedDataProvider& fdp) {
    std::string rule = "seqbeforechars ";
    
    // Generate 1-5 characters that can appear before sequences
    size_t num_chars = fdp.ConsumeIntegralInRange<size_t>(1, 5);
    for (size_t i = 0; i < num_chars; i++) {
        uint8_t char_type = fdp.ConsumeIntegral<uint8_t>() % 3;
        
        if (char_type == 0) {
            rule += fdp.ConsumeIntegralInRange<char>('a', 'z');
        } else if (char_type == 1) {
            rule += fdp.ConsumeIntegralInRange<char>('A', 'Z');
        } else {
            rule += fdp.ConsumeIntegralInRange<char>('0', '9');
        }
    }
    
    return rule + "\n";
}

// Helper to generate sequence after characters definition
// CTO_SeqAfterChars: Marks characters that can appear after sequences
static std::string generate_seq_after_chars_rule(FuzzedDataProvider& fdp) {
    std::string rule = "seqafterchars ";
    
    // Generate 1-5 characters that can appear after sequences
    size_t num_chars = fdp.ConsumeIntegralInRange<size_t>(1, 5);
    for (size_t i = 0; i < num_chars; i++) {
        uint8_t char_type = fdp.ConsumeIntegral<uint8_t>() % 4;
        
        if (char_type == 0) {
            rule += fdp.ConsumeIntegralInRange<char>('a', 'z');
        } else if (char_type == 1) {
            rule += fdp.ConsumeIntegralInRange<char>('A', 'Z');
        } else if (char_type == 2) {
            rule += fdp.ConsumeIntegralInRange<char>('0', '9');
        } else {
            // Punctuation that might appear after sequences
            const char* punct = ".,;:!?";
            size_t punct_idx = fdp.ConsumeIntegralInRange<size_t>(0, strlen(punct) - 1);
            rule += punct[punct_idx];
        }
    }
    
    return rule + "\n";
}

// Helper to generate sequence after pattern
// CTO_SeqAfterPattern: Defines patterns that can follow sequences
static std::string generate_seq_after_pattern_rule(FuzzedDataProvider& fdp) {
    std::string rule = "seqafterpattern ";
    
    // Generate a pattern that can appear after sequences
    // Could be simple character sequence or more complex pattern
    uint8_t pattern_type = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    if (pattern_type == 0) {
        // Simple character sequence
        size_t seq_len = fdp.ConsumeIntegralInRange<size_t>(1, 8);
        for (size_t i = 0; i < seq_len; i++) {
            rule += fdp.ConsumeIntegralInRange<char>('a', 'z');
        }
    } else if (pattern_type == 1) {
        // Character class pattern
        rule += "[";
        char start = fdp.ConsumeIntegralInRange<char>('a', 'z');
        char end = fdp.ConsumeIntegralInRange<char>(start, 'z');
        rule += start;
        rule += '-';
        rule += end;
        rule += "]";
    } else if (pattern_type == 2) {
        // Pattern with quantifier
        rule += fdp.ConsumeIntegralInRange<char>('a', 'z');
        rule += "*";  // Zero or more
    } else {
        // Pattern with alternation
        rule += "(";
        rule += fdp.ConsumeIntegralInRange<char>('a', 'z');
        rule += "|";
        rule += fdp.ConsumeIntegralInRange<char>('A', 'Z');
        rule += ")";
    }
    
    return rule + "\n";
}

// Helper to generate sequence after expression
// CTO_SeqAfterExpression: Defines expression that can follow sequences
static std::string generate_seq_after_expression_rule(FuzzedDataProvider& fdp) {
    std::string rule = "seqafterexpression ";
    
    // Generate an expression that can appear after sequences
    // More complex than simple patterns, may include operators
    uint8_t expr_type = fdp.ConsumeIntegral<uint8_t>() % 3;
    
    if (expr_type == 0) {
        // Character range expression
        rule += fdp.ConsumeIntegralInRange<char>('a', 'z');
        rule += "-";
        rule += fdp.ConsumeIntegralInRange<char>('f', 'z');
    } else if (expr_type == 1) {
        // Character class expression with negation
        rule += "[^";
        rule += fdp.ConsumeIntegralInRange<char>('0', '9');
        rule += "-";
        rule += fdp.ConsumeIntegralInRange<char>('5', '9');
        rule += "]";
    } else {
        // Complex expression with grouping
        rule += "(";
        rule += fdp.ConsumeIntegralInRange<char>('a', 'z');
        rule += fdp.ConsumeIntegralInRange<char>('a', 'z');
        rule += ")+";
    }
    
    return rule + "\n";
}

// Helper to generate character definitions needed for sequence testing
static std::string generate_character_definitions(FuzzedDataProvider& fdp) {
    std::string content;
    
    // Define some characters that will be used in sequence rules
    size_t num_chars = fdp.ConsumeIntegralInRange<size_t>(3, 10);
    
    for (size_t i = 0; i < num_chars; i++) {
        // Character to define
        char c = fdp.ConsumeIntegralInRange<char>('a', 'z');
        
        // Braille cell pattern (1-8 dots)
        int dot_count = fdp.ConsumeIntegralInRange<int>(1, 8);
        std::string dots;
        for (int d = 0; d < dot_count; d++) {
            dots += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 8));
        }
        
        content += std::string(1, c) + " " + dots + "\n";
    }
    
    return content;
}

// Helper to generate a complete table with sequence opcodes
static std::string generate_sequence_table(FuzzedDataProvider& fdp) {
    std::string content;
    
    // Table header
    content += "table sequence_test\n";
    content += "locale en_US\n";
    content += "author fuzzer\n\n";
    
    // Generate character definitions first (needed for sequence rules)
    content += generate_character_definitions(fdp);
    content += "\n";
    
    // Generate sequence-related rules based on fuzzer input
    size_t num_sequence_rules = fdp.ConsumeIntegralInRange<size_t>(2, 8);
    
    for (size_t i = 0; i < num_sequence_rules; i++) {
        uint8_t rule_type = fdp.ConsumeIntegral<uint8_t>() % 6;
        
        switch (rule_type) {
            case 0:
                content += generate_seq_delimiter_rule(fdp);
                break;
            case 1:
                content += generate_seq_before_chars_rule(fdp);
                break;
            case 2:
                content += generate_seq_after_chars_rule(fdp);
                break;
            case 3:
                content += generate_seq_after_pattern_rule(fdp);
                break;
            case 4:
                content += generate_seq_after_expression_rule(fdp);
                break;
            case 5:
                // Regular character rule to complement sequence rules
                content += std::string(1, fdp.ConsumeIntegralInRange<char>('a', 'z')) + " " +
                          std::to_string(fdp.ConsumeIntegralInRange<int>(1, 8)) + "\n";
                break;
        }
    }
    
    // Add some regular rules that might interact with sequence logic
    content += "\n";
    content += "space 0\n";
    content += "digit 3456\n";
    content += "letter 1\n";
    
    return content;
}


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - sequence compilation needs substantial input
    if (size < 128) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize liblouis logging once
    static bool initialized = false;
    if (!initialized) {
        lou_registerLogCallback(avoid_log_callback);
        initialized = true;
    }
    
    // Set data path (optional but helps with table resolution)
    size_t path_len = fdp.ConsumeIntegralInRange<size_t>(1, 128);
    std::string data_path = fdp.ConsumeBytesAsString(path_len);
    lou_setDataPath(data_path.c_str());
    
    // Generate table content with sequence opcodes
    std::string table_content = generate_sequence_table(fdp);
    
    if (table_content.empty() || fdp.remaining_bytes() < 10) {
        return 0;
    }
    
    // Use a dummy table list - the actual compilation happens on the string
    const char* table_list = "sequence_test";
    
    // Call lou_compileString to exercise compileRule with our sequence opcodes
    // This will trigger compileString -> compileRule -> sequence opcode handlers
    int compile_result = lou_compileString(table_list, table_content.c_str());
    
    // Compilation may succeed or fail - both are valid outcomes for fuzzing
    // The goal is to exercise the branching paths within compileRule
    (void)compile_result; // Result intentionally unused
    
    // If we want to test table retrieval, we can try lou_getTable
    // but only if we have enough remaining input
    if (fdp.remaining_bytes() > 20) {
        const void* retrieved_table = lou_getTable(table_list);
        (void)retrieved_table; // Result intentionally unused
    }
    
    // Always call lou_free to clean up any allocated resources
    lou_free();
    
    return 0;
}

} // namespace harness_020
