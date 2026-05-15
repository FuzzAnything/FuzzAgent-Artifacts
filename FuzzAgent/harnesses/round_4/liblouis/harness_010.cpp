#include <fuzzer/FuzzedDataProvider.h>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>
#include <algorithm>
#include "liblouis.h"

namespace harness_010 {
// liblouis complex pattern compilation fuzzing harness
// Target APIs: lou_compileString (Primary), lou_free (Secondary), lou_getTable (Optional)
// Based on coverage analysis guidance: pattern_compile_expression has 115 blocked branches (36.1% uncovered)
// This function handles complex pattern matching expressions including nested parentheses, negation operators,
// character classes, and quantifiers in translation table patterns.
// Invocation sequence: 1. Generate complex pattern expressions with diverse pattern syntax
//                      2. Call lou_compileString with generated pattern strings
//                      3. Validate compilation success and store table pointer
//                      4. Optionally call lou_getTable to retrieve and validate compiled table
//                      5. Call lou_free to clean up allocated memory
// Semantic differentiation from previous compilation harnesses (harness_003, harness_005, harness_008):
// Focuses specifically on pattern compilation logic with complex pattern expressions rather than
// general table compilation or opcode diversity.


// Include liblouis public header
extern "C" {
}

// Log callback to suppress logs during fuzzing
static void avoid_log_callback(logLevels level, const char *msg) {
    (void)level;
    (void)msg;
}

// Helper to generate complex pattern expressions based on fuzzer input
static std::string generate_pattern_expression(FuzzedDataProvider& fdp) {
    std::string pattern;
    
    // Determine pattern complexity based on remaining bytes
    size_t complexity = fdp.ConsumeIntegralInRange<size_t>(1, 10);
    
    for (size_t i = 0; i < complexity && fdp.remaining_bytes() > 10; i++) {
        uint8_t pattern_type = fdp.ConsumeIntegral<uint8_t>() % 20;
        
        switch (pattern_type) {
            case 0: { // Nested parentheses: (a(b(c)))
                pattern += "(";
                size_t depth = fdp.ConsumeIntegralInRange<size_t>(1, 5);
                for (size_t d = 0; d < depth; d++) {
                    pattern += fdp.ConsumeRandomLengthString(1);
                    if (d < depth - 1) pattern += "(";
                }
                for (size_t d = 0; d < depth; d++) {
                    pattern += ")";
                }
                break;
            }
            case 1: { // Negation operators: !abc
                pattern += "!";
                pattern += fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(1, 10));
                break;
            }
            case 2: { // Negation with parentheses: !(abc|def)
                pattern += "!(";
                pattern += fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(1, 5));
                pattern += "|";
                pattern += fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(1, 5));
                pattern += ")";
                break;
            }
            case 3: { // Character classes: [a-z]
                pattern += "[";
                char start = fdp.ConsumeIntegralInRange<char>('a', 'z');
                char end = fdp.ConsumeIntegralInRange<char>(start, 'z');
                pattern += start;
                pattern += '-';
                pattern += end;
                pattern += "]";
                break;
            }
            case 4: { // Negated character classes: [^0-9]
                pattern += "[^";
                char start = fdp.ConsumeIntegralInRange<char>('0', '9');
                char end = fdp.ConsumeIntegralInRange<char>(start, '9');
                pattern += start;
                pattern += '-';
                pattern += end;
                pattern += "]";
                break;
            }
            case 5: { // Quantifiers: a*
                pattern += fdp.ConsumeRandomLengthString(1);
                pattern += "*";
                break;
            }
            case 6: { // Quantifiers: b+
                pattern += fdp.ConsumeRandomLengthString(1);
                pattern += "+";
                break;
            }
            case 7: { // Quantifiers: c?
                pattern += fdp.ConsumeRandomLengthString(1);
                pattern += "?";
                break;
            }
            case 8: { // Alternation: a|b|c
                size_t alt_count = fdp.ConsumeIntegralInRange<size_t>(2, 5);
                for (size_t a = 0; a < alt_count; a++) {
                    pattern += fdp.ConsumeRandomLengthString(1);
                    if (a < alt_count - 1) pattern += "|";
                }
                break;
            }
            case 9: { // Complex combinations: !([a-z]*|(test+)?)
                pattern += "!([";
                char start = fdp.ConsumeIntegralInRange<char>('a', 'z');
                char end = fdp.ConsumeIntegralInRange<char>(start, 'z');
                pattern += start;
                pattern += '-';
                pattern += end;
                pattern += "]*|(";
                pattern += fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(1, 5));
                pattern += "+)?)";
                break;
            }
            case 10: { // Dot operator (any character): .
                pattern += ".";
                break;
            }
            case 11: { // Escape sequences
                pattern += "\\";
                pattern += fdp.ConsumeRandomLengthString(1);
                break;
            }
            case 12: { // Character class with multiple ranges
                pattern += "[";
                pattern += fdp.ConsumeIntegralInRange<char>('a', 'z');
                pattern += '-';
                pattern += fdp.ConsumeIntegralInRange<char>('a', 'z');
                pattern += fdp.ConsumeIntegralInRange<char>('A', 'Z');
                pattern += '-';
                pattern += fdp.ConsumeIntegralInRange<char>('A', 'Z');
                pattern += "]";
                break;
            }
            case 13: { // Attribute patterns: %[a-z] (character attributes)
                pattern += "%";
                if (fdp.ConsumeBool()) pattern += "[";
                pattern += fdp.ConsumeRandomLengthString(1);
                if (pattern.back() == '[') pattern += "]";
                break;
            }
            case 14: { // Hook patterns: @[a-z] (translation hooks)
                pattern += "@";
                if (fdp.ConsumeBool()) pattern += "[";
                pattern += fdp.ConsumeRandomLengthString(1);
                if (pattern.back() == '[') pattern += "]";
                break;
            }
            case 15: { // Start/end markers: ^ and $
                if (fdp.ConsumeBool()) {
                    pattern += "^";
                } else {
                    pattern += "$";
                }
                break;
            }
            case 16: { // Mixed quantifiers
                std::string base = fdp.ConsumeRandomLengthString(1);
                uint8_t quant = fdp.ConsumeIntegral<uint8_t>() % 3;
                pattern += base;
                if (quant == 0) pattern += "*";
                else if (quant == 1) pattern += "+";
                else pattern += "?";
                break;
            }
            case 17: { // Nested alternation
                pattern += "(";
                pattern += fdp.ConsumeRandomLengthString(1);
                pattern += "|";
                pattern += fdp.ConsumeRandomLengthString(1);
                pattern += "|";
                pattern += fdp.ConsumeRandomLengthString(1);
                pattern += ")";
                break;
            }
            case 18: { // Complex nested with quantifiers
                pattern += "(";
                pattern += fdp.ConsumeRandomLengthString(1);
                pattern += "(";
                pattern += fdp.ConsumeRandomLengthString(1);
                pattern += "+";
                pattern += ")?";
                pattern += ")*";
                break;
            }
            case 19: { // Simple character
                pattern += fdp.ConsumeRandomLengthString(1);
                break;
            }
        }
        
        // Add separator or concatenation
        if (i < complexity - 1 && fdp.ConsumeBool()) {
            pattern += " ";
        }
    }
    
    return pattern;
}

// Helper to create complete table entry with pattern expression
static std::string create_table_with_pattern(FuzzedDataProvider& fdp, const std::string& pattern_expr) {
    std::string table_content;
    
    // Basic table metadata
    if (fdp.ConsumeBool()) {
        table_content += "table test-pattern\n";
    }
    
    if (fdp.ConsumeBool()) {
        table_content += "locale en\n";
    }
    
    // Add a rule using the pattern expression
    // Pattern rules in liblouis tables typically look like: pattern <pattern-expr> <translation>
    table_content += "pattern ";
    table_content += pattern_expr;
    table_content += " ";
    
    // Add translation (braille dots)
    // Generate random braille dot pattern (1-8 dots)
    uint8_t dot_pattern = fdp.ConsumeIntegral<uint8_t>() % 255;
    table_content += std::to_string(static_cast<int>(dot_pattern));
    table_content += "\n";
    
    // Add more rules to increase complexity
    size_t extra_rules = fdp.ConsumeIntegralInRange<size_t>(0, 5);
    for (size_t i = 0; i < extra_rules; i++) {
        if (fdp.ConsumeBool()) {
            table_content += "always ";
        } else {
            table_content += "pattern ";
            table_content += generate_pattern_expression(fdp);
            table_content += " ";
        }
        table_content += std::to_string(fdp.ConsumeIntegral<uint8_t>() % 255);
        table_content += "\n";
    }
    
    return table_content;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough for pattern generation
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
    
    // Generate complex pattern expression
    std::string pattern_expr = generate_pattern_expression(fdp);
    if (pattern_expr.empty()) {
        return 0;
    }
    
    // Create complete table content with pattern
    std::string table_content = create_table_with_pattern(fdp, pattern_expr);
    
    // Choose a table name for compilation
    const char* table_names[] = {
        "test-pattern.ctb",
        "pattern-test.utb",
        "complex-pattern.ctb",
        "expr-test.ctb"
    };
    const size_t num_tables = sizeof(table_names) / sizeof(table_names[0]);
    size_t table_idx = fdp.ConsumeIntegralInRange<size_t>(0, num_tables - 1);
    const char* table_name = table_names[table_idx];
    
    // 1. Call lou_compileString with complex pattern expressions
    // This is the primary target API that exercises pattern_compile_expression
    int compile_result = lou_compileString(table_name, table_content.c_str());
    
    // 2. Validate compilation success and store table pointer
    // Note: lou_compileString returns 1 on success, 0 on failure
    // Some pattern expressions will be invalid by design - that's OK for fuzzing
    
    // 3. Optionally call lou_getTable to retrieve and validate compiled table
    if (compile_result == 1 && fdp.ConsumeBool()) {
        // Try to get the compiled table
        // Note: lou_getTable may return NULL if table wasn't successfully compiled
        const void* table_ptr = lou_getTable(table_name);
        // We don't need to do anything with the pointer, just calling it exercises the code
        (void)table_ptr;
    }
    // 4. Call lou_free to clean up allocated memory
    // This is important to prevent memory leaks during fuzzing
    lou_free();
    
    return 0;
}

} // namespace harness_010
