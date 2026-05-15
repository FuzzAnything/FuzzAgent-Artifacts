#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <string>
#include <vector>
#include <algorithm>
#include <fuzzer/FuzzedDataProvider.h>
#include "liblouis/liblouis.h"

namespace harness_019 {
/*
 * liblouis Mathematical Braille Translation and Complex Formula Parsing Fuzzing Harness (harness_019.cpp)
 * 
 * This harness specifically targets mathematical braille translation tables and complex
 * formula parsing functionality in liblouis. Focuses on testing mathematical notation,
 * specialized math braille tables, formula translation, and edge cases with mathematical
 * expressions that haven't been extensively covered in previous harnesses.
 * 
 * Primary focus areas:
 * 1) Mathematical braille tables (ar-ar-math.uti*, en-ueb-math.ctb, en-us-mathtext.ctb, etc.)
 * 2) Complex mathematical expression parsing and translation
 * 3) Unicode mathematical symbols and operators
 * 4) Formula translation with specialized math rules
 * 5) Interaction between math and regular text in mixed expressions
 * 6) Edge cases with mathematical notation and parentheses
 * 7) Testing both forward and back translation of mathematical formulas
 * 
 * APIs specifically targeted in this harness:
 * - lou_translateString: Forward translation of mathematical expressions
 * - lou_translate: Translation with cursor tracking for formula navigation
 * - lou_backTranslateString: Back translation of braille math back to text
 * - lou_backTranslate: Back translation with cursor tracking
 * - lou_hyphenate: Hyphenation with mathematical content (edge case)
 * - lou_dotsToChar/lou_charToDots: Dot pattern conversion with math symbols
 * - lou_checkTable/lou_getTable: Loading and validation of math tables
 * 
 * Semantic differentiation from previous harnesses:
 * - harness_000-017: General translation, error handling, specific features
 * - harness_018: Right-to-left languages and RTL-specific cursor positioning
 * - This harness (019): FOCUSES EXCLUSIVELY on mathematical braille translation,
 *                      complex formula parsing, and mathematical notation handling
 * 
 * Mathematical testing rationale:
 * - Math tables contain specialized translation rules for mathematical notation
 * - Formula parsing involves complex pattern matching and expression trees
 * - Mathematical symbols have unique Unicode representations and braille mappings
 * - Edge cases with nested parentheses, operators, and mathematical functions
 * - Potential for parser vulnerabilities in complex expression handling
 */


extern "C" {
}

// Simple string to widechar conversion for fuzzing
static int simple_string_to_widechar(const char* input, size_t input_len, widechar* output) {
    for (size_t i = 0; i < input_len && i < 1024; i++) {
        output[i] = (widechar)(unsigned char)input[i];
    }
    return (int)std::min(input_len, (size_t)1024);
}

// Log callback to suppress output during fuzzing
static void fuzz_log_callback(logLevels level, const char* msg) {
    (void)level;
    (void)msg;
    // Suppress all logging during fuzzing
}

// Mathematical braille tables for testing
static const char* math_tables[] = {
    // Arabic math table (RTL math)
    "/root/src/liblouis/tables/ar-ar-math.uti*",
    
    // Unified English Braille math table
    "/root/src/liblouis/tables/en-ueb-math.ctb",
    
    // US math text table
    "/root/src/liblouis/tables/en-us-mathtext.ctb",
    
    // UK math definitions (inclusion files)
    "/root/src/liblouis/tables/ukmaths_single_cell_defs.cti",
    "/root/src/liblouis/tables/ukmaths_unicode_defs.cti",
    
    // Regular tables for comparison and mixed testing
    "/root/src/liblouis/tables/en-us-g2.ctb",
    "/root/src/liblouis/tables/en-us-g1.ctb",
    
    // Tables that might handle math symbols
    "/root/src/liblouis/tables/IPA.utb",  // International Phonetic Alphabet
    "/root/src/liblouis/tables/Es-Es-G0.utb",  // Spanish
};

static const size_t num_math_tables = sizeof(math_tables) / sizeof(math_tables[0]);

// Mathematical symbols and operators for testing
static const char* math_symbols[] = {
    // Basic operators
    "+", "-", "*", "/", "=", "≠", "<", ">", "≤", "≥",
    
    // Advanced operators
    "±", "×", "÷", "√", "∛", "∜", "∑", "∏", "∫", "∂",
    
    // Greek letters (common in math)
    "α", "β", "γ", "δ", "ε", "ζ", "η", "θ", "ι", "κ", "λ", "μ", "ν",
    "ξ", "π", "ρ", "σ", "τ", "υ", "φ", "χ", "ψ", "ω",
    
    // Mathematical constants
    "π", "e", "i", "∞",
    
    // Set theory symbols
    "∈", "∉", "⊂", "⊃", "⊆", "⊇", "∪", "∩", "∅",
    
    // Logical operators
    "∧", "∨", "¬", "→", "↔", "∀", "∃",
    
    // Parentheses and brackets
    "(", ")", "[", "]", "{", "}", "⟨", "⟩",
    
    // Fractions and superscripts/subscripts
    "½", "¼", "¾", "¹", "²", "³", "₀", "₁", "₂", "₃",
    
    // Arrows
    "→", "←", "↑", "↓", "↔", "⇒", "⇐", "⇔",
    
    // Other mathematical symbols
    "°", "′", "″", "‰", "‱", "∇", "∆", "≈", "≅", "∼", "∝",
};

static const size_t num_math_symbols = sizeof(math_symbols) / sizeof(math_symbols[0]);

// Mathematical function names
static const char* math_functions[] = {
    "sin", "cos", "tan", "cot", "sec", "csc",
    "asin", "acos", "atan", "acot", "asec", "acsc",
    "sinh", "cosh", "tanh", "coth", "sech", "csch",
    "log", "ln", "exp", "sqrt", "abs", "floor", "ceil",
    "max", "min", "sum", "prod", "lim", "det", "tr",
    "grad", "div", "curl", "laplace",
};

static const size_t num_math_functions = sizeof(math_functions) / sizeof(math_functions[0]);

// Generate mathematical expression from fuzzer input
static std::string generate_math_expression(FuzzedDataProvider& fdp, size_t max_len) {
    std::string expression;
    size_t target_len = fdp.ConsumeIntegralInRange<size_t>(1, max_len);
    
    for (size_t i = 0; i < target_len && fdp.remaining_bytes() > 0; i++) {
        uint8_t element_type = fdp.ConsumeIntegral<uint8_t>() % 6;
        
        switch (element_type) {
            case 0: // Digits
                expression.push_back(fdp.ConsumeIntegralInRange<char>('0', '9'));
                break;
            case 1: // Variables (letters)
                expression.push_back(fdp.ConsumeIntegralInRange<char>('a', 'z'));
                break;
            case 2: // Basic operators
                expression.push_back(fdp.ConsumeIntegralInRange<char>('+', '/'));
                break;
            case 3: // Parentheses
                if (fdp.ConsumeBool()) {
                    expression.push_back('(');
                } else {
                    expression.push_back(')');
                }
                break;
            case 4: // Mathematical symbols (from predefined list)
                if (num_math_symbols > 0 && fdp.remaining_bytes() > 0) {
                    uint8_t symbol_idx = fdp.ConsumeIntegralInRange<uint8_t>(0, num_math_symbols - 1);
                    expression.append(math_symbols[symbol_idx]);
                }
                break;
            case 5: // Function names
                if (num_math_functions > 0 && fdp.remaining_bytes() > 0) {
                    uint8_t func_idx = fdp.ConsumeIntegralInRange<uint8_t>(0, num_math_functions - 1);
                    expression.append(math_functions[func_idx]);
                    expression.push_back('(');
                }
                break;
        }
        
        // Add spaces occasionally for readability (though math often omits them)
        if (fdp.ConsumeBool() && fdp.ConsumeIntegral<uint8_t>() < 64) {
            expression.push_back(' ');
        }
    }
    
    // Ensure parentheses are somewhat balanced (not perfect, but helps)
    size_t open_paren = std::count(expression.begin(), expression.end(), '(');
    size_t close_paren = std::count(expression.begin(), expression.end(), ')');
    
    if (open_paren > close_paren) {
        for (size_t i = 0; i < open_paren - close_paren && expression.length() < max_len * 2; i++) {
            expression.push_back(')');
        }
    }
    
    return expression;
}

// Generate mixed expression with math and regular text
static std::string generate_mixed_expression(FuzzedDataProvider& fdp, size_t max_len) {
    std::string result;
    size_t target_len = fdp.ConsumeIntegralInRange<size_t>(1, max_len);
    
    bool in_math_mode = fdp.ConsumeBool();
    
    for (size_t i = 0; i < target_len && fdp.remaining_bytes() > 0; i++) {
        if (in_math_mode) {
            // Generate math content
            uint8_t math_type = fdp.ConsumeIntegral<uint8_t>() % 4;
            switch (math_type) {
                case 0:
                    result.push_back(fdp.ConsumeIntegralInRange<char>('0', '9'));
                    break;
                case 1:
                    result.push_back(fdp.ConsumeIntegralInRange<char>('+', '/'));
                    break;
                case 2:
                    if (num_math_symbols > 0) {
                        uint8_t idx = fdp.ConsumeIntegralInRange<uint8_t>(0, num_math_symbols - 1);
                        result.append(math_symbols[idx]);
                    }
                    break;
                case 3:
                    result.push_back(fdp.ConsumeIntegralInRange<char>('a', 'z'));
                    break;
            }
        } else {
            // Generate regular text
            result.push_back(fdp.ConsumeIntegralInRange<char>('A', 'z'));
        }
        
        // Occasionally switch modes
        if (fdp.ConsumeIntegral<uint8_t>() < 32) {
            in_math_mode = !in_math_mode;
            result.push_back(' '); // Add space when switching
        }
    }
    
    return result;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Initialize logging once
    static int initialized = 0;
    if (!initialized) {
        lou_registerLogCallback(fuzz_log_callback);
        initialized = 1;
    }
    
    // Need minimum input for meaningful testing
    if (size < 16) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // 1. Select math table to use
    uint8_t table_index = fdp.ConsumeIntegralInRange<uint8_t>(0, num_math_tables - 1);
    const char* table_name = math_tables[table_index];
    
    // 2. Check if table is valid (some might be inclusion files, which is OK)
    if (lou_checkTable(table_name) == 0) {
        // Try to get table directly
        const void* table = lou_getTable(table_name);
        if (table == NULL) {
            // Try a fallback table
            table_name = "/root/src/liblouis/tables/en-us-g2.ctb";
            if (lou_checkTable(table_name) == 0) {
                return 0; // No valid table available
            }
        }
    }
    
    // 3. Determine test type based on input
    uint8_t test_type = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    // 4. Prepare input buffers
    widechar inbuf[1024];
    widechar outbuf[2048]; // Math translation might produce more output
    int inlen = 0;
    int outlen = 0;
    
    // 5. Generate appropriate input based on test type
    std::string input_text;
    
    switch (test_type) {
        case 0: // Pure mathematical expression
            input_text = generate_math_expression(fdp, 256);
            break;
        case 1: // Mixed math and text
            input_text = generate_mixed_expression(fdp, 256);
            break;
        case 2: // Mathematical function with parameters
            if (num_math_functions > 0) {
                uint8_t func_idx = fdp.ConsumeIntegralInRange<uint8_t>(0, num_math_functions - 1);
                input_text = math_functions[func_idx];
                input_text += "(";
                
                // Add parameters
                size_t num_params = fdp.ConsumeIntegralInRange<size_t>(1, 5);
                for (size_t i = 0; i < num_params; i++) {
                    if (i > 0) input_text += ",";
                    input_text += generate_math_expression(fdp, 50);
                }
                input_text += ")";
            } else {
                input_text = generate_math_expression(fdp, 256);
            }
            break;
        case 3: // Complex expression with nested parentheses
            input_text = "(" + generate_math_expression(fdp, 100) + ")";
            // Add outer operators
            input_text = generate_math_expression(fdp, 50) + "+" + input_text;
            input_text = "sqrt(" + input_text + ")";
            break;
    }
    
    if (input_text.empty()) {
        return 0;
    }
    
    // 6. Convert to widechar for liblouis
    inlen = simple_string_to_widechar(input_text.c_str(), input_text.length(), inbuf);
    if (inlen <= 0) {
        return 0;
    }
    
    // 7. Test different translation operations based on remaining input
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 5;
    
    switch (operation) {
        case 0: // Forward translation with lou_translateString
            {
                int actual_inlen = inlen;
                outlen = sizeof(outbuf) / sizeof(outbuf[0]);
                lou_translateString(table_name, inbuf, &actual_inlen, outbuf, &outlen, NULL, NULL, 0);
            }
            break;
            
        case 1: // Forward translation with lou_translate (with cursor positions)
            {
                int actual_inlen = inlen;
                outlen = sizeof(outbuf) / sizeof(outbuf[0]);
                int cursorPos = fdp.ConsumeIntegralInRange<int>(0, inlen);
                int* outputPos = NULL;
                int* inputPos = NULL;
                
                // Sometimes test with position arrays
                if (fdp.ConsumeBool()) {
                    static int outputPosArray[1024];
                    static int inputPosArray[1024];
                    outputPos = outputPosArray;
                    inputPos = inputPosArray;
                }
                
                lou_translate(table_name, inbuf, &actual_inlen, outbuf, &outlen, 
                             NULL, NULL, outputPos, inputPos, &cursorPos, 0);
            }
            break;
        case 2: // Back translation
            {
                // First do forward translation
                int actual_inlen = inlen;
                outlen = sizeof(outbuf) / sizeof(outbuf[0]);
                int forward_result = lou_translateString(table_name, inbuf, &actual_inlen, outbuf, &outlen, NULL, NULL, 0);
                
                if (forward_result) {
                    // Now try back translation
                    int back_inlen = outlen;
                    int back_outlen = sizeof(inbuf) / sizeof(inbuf[0]);
                    lou_backTranslateString(table_name, outbuf, &back_inlen, inbuf, &back_outlen, NULL, NULL, 0);
                }
            }
            break;
            
        case 3: // Dot pattern conversion
            {
                // Test charToDots with math input
                outlen = sizeof(outbuf) / sizeof(outbuf[0]);
                lou_charToDots(table_name, inbuf, outbuf, inlen, 0);
                
                // Test dotsToChar with the result
                if (inlen > 0) {
                    widechar dots_back[1024];
                    lou_dotsToChar(table_name, outbuf, dots_back, inlen, 0);
                }
            }
            break;
            
        case 4: // Hyphenation with mathematical content (edge case)
            {
                char hyphen_buffer[1024];
                int hyphen_result = lou_hyphenate(table_name, inbuf, inlen, hyphen_buffer, 0);
                (void)hyphen_result;
            }
            break;
    }
    
    // 8. Test with different modes if there's remaining input
    if (fdp.remaining_bytes() > 0) {
        uint8_t mode_test = fdp.ConsumeIntegral<uint8_t>() % 3;
        
        int actual_inlen = inlen;
        outlen = sizeof(outbuf) / sizeof(outbuf[0]);
        
        switch (mode_test) {
            case 0: // Computer braille mode (relevant for math)
                lou_translateString(table_name, inbuf, &actual_inlen, outbuf, &outlen, 
                                   NULL, NULL, computer_braille);
                break;
            case 1: // No contractions mode
                lou_translateString(table_name, inbuf, &actual_inlen, outbuf, &outlen, 
                                   NULL, NULL, no_contract);
                break;
            case 2: // Combined modes
                lou_translateString(table_name, inbuf, &actual_inlen, outbuf, &outlen, 
                                   NULL, NULL, computer_braille | no_contract);
                break;
        }
    }
    
    // 9. Test table chaining with math and regular tables
    if (fdp.remaining_bytes() > 0 && fdp.ConsumeBool()) {
        std::string table_chain = table_name;
        table_chain += ",/root/src/liblouis/tables/en-us-g2.ctb";
        
        int actual_inlen = inlen;
        outlen = sizeof(outbuf) / sizeof(outbuf[0]);
        
        lou_translateString(table_chain.c_str(), inbuf, &actual_inlen, outbuf, &outlen, 
                           NULL, NULL, 0);
    }
    
    // Cleanup
    lou_free();
    
    return 0;
}

} // namespace harness_019
