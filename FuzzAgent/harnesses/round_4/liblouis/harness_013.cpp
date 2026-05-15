#include <fuzzer/FuzzedDataProvider.h>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>
#include <iostream>
#include "liblouis.h"

namespace harness_013 {
// liblouis compileRule opcode parsing fuzzing harness
// Targets compileRule function's unexercised opcode parsing paths (568 blocked branches, 62.4% uncovered)
// Specifically targets: CTO_Macro opcode (currently 0 hits), CTO_Grouping opcode (currently 0 hits)
// Based on coverage analysis guidance: compileRule has the largest single blocker with many switch cases showing 0 hits
// Primary target: lou_compileString (directly exercises compileRule)
// Secondary: lou_getTable (table initialization), lou_checkTable (validation after compilation)
// Focus on semantic differentiation from previous compilation harnesses


// Include liblouis public header
extern "C" {
}

// Log callback to suppress logs during fuzzing
static void avoid_log_callback(logLevels level, const char *msg) {
    (void)level;
    (void)msg;
}

// Helper to generate translation table rules with specific opcodes
static std::string generate_table_rule(FuzzedDataProvider& fdp, uint8_t rule_type) {
    std::string rule;
    
    switch (rule_type % 8) {
        case 0: { // Macro definition (CTO_Macro)
            rule = "macro ";
            // Generate macro name
            size_t macro_name_len = fdp.ConsumeIntegralInRange<size_t>(1, 16);
            for (size_t i = 0; i < macro_name_len; i++) {
                char c = fdp.ConsumeIntegralInRange<char>('a', 'z');
                rule += c;
            }
            rule += " eom\n";
            
            // Generate macro body with various opcodes
            size_t macro_body_lines = fdp.ConsumeIntegralInRange<size_t>(1, 5);
            for (size_t i = 0; i < macro_body_lines; i++) {
                uint8_t line_type = fdp.ConsumeIntegral<uint8_t>() % 4;
                switch (line_type) {
                    case 0:
                        rule += "space ";
                        rule += fdp.ConsumeIntegralInRange<char>('a', 'z');
                        rule += " 46\n";
                        break;
                    case 1:
                        rule += "digit ";
                        rule += fdp.ConsumeIntegralInRange<char>('0', '9');
                        rule += " 3456\n";
                        break;
                    case 2:
                        rule += "punctuation ";
                        rule += fdp.ConsumeIntegralInRange<char>('!', '/');
                        rule += " 46\n";
                        break;
                    case 3:
                        rule += "letter ";
                        rule += fdp.ConsumeIntegralInRange<char>('a', 'z');
                        rule += " 1\n";
                        break;
                }
            }
            rule += "em\n";
            break;
        }
            
        case 1: { // Grouping operation (CTO_Grouping)
            rule = "grouping ";
            // Generate grouping name
            size_t group_name_len = fdp.ConsumeIntegralInRange<size_t>(1, 16);
            for (size_t i = 0; i < group_name_len; i++) {
                char c = fdp.ConsumeIntegralInRange<char>('a', 'z');
                rule += c;
            }
            rule += " ";
            
            // Generate two Unicode characters
            rule += fdp.ConsumeIntegralInRange<char>('a', 'z');
            rule += fdp.ConsumeIntegralInRange<char>('a', 'z');
            rule += " ";
            
            // Generate two cells separated by comma
            uint8_t cell1 = fdp.ConsumeIntegralInRange<uint8_t>(1, 8);
            uint8_t cell2 = fdp.ConsumeIntegralInRange<uint8_t>(1, 8);
            rule += std::to_string(cell1) + "," + std::to_string(cell2);
            rule += "\n";
            break;
        }
            
        case 2: { // Character class definitions
            uint8_t char_class = fdp.ConsumeIntegral<uint8_t>() % 8;
            switch (char_class) {
                case 0: rule += "space "; break;
                case 1: rule += "digit "; break;
                case 2: rule += "punctuation "; break;
                case 3: rule += "math "; break;
                case 4: rule += "sign "; break;
                case 5: rule += "letter "; break;
                case 6: rule += "uppercase "; break;
                case 7: rule += "lowercase "; break;
            }
            rule += fdp.ConsumeIntegralInRange<char>('!', '~');
            rule += " ";
            rule += std::to_string(fdp.ConsumeIntegralInRange<uint8_t>(1, 123456));
            rule += "\n";
            break;
        }
            
        case 3: { // Include file directive (CTO_IncludeFile)
            rule = "include ";
            size_t include_len = fdp.ConsumeIntegralInRange<size_t>(1, 32);
            for (size_t i = 0; i < include_len; i++) {
                char c = fdp.ConsumeIntegralInRange<char>('a', 'z');
                rule += c;
            }
            rule += ".uti\n";
            break;
        }
            
        case 4: { // Complex pass opcode configurations
            uint8_t pass_num = fdp.ConsumeIntegralInRange<uint8_t>(2, 4);
            rule += "pass" + std::to_string(pass_num) + " ";
            
            // Generate pass rule
            uint8_t pass_rule = fdp.ConsumeIntegral<uint8_t>() % 4;
            switch (pass_rule) {
                case 0:
                    rule += "always ";
                    rule += fdp.ConsumeIntegralInRange<char>('a', 'z');
                    rule += " 1\n";
                    break;
                case 1:
                    rule += "context ";
                    rule += fdp.ConsumeIntegralInRange<char>('a', 'z');
                    rule += " 1 ";
                    rule += fdp.ConsumeIntegralInRange<char>('a', 'z');
                    rule += "\n";
                    break;
                case 2:
                    rule += "correct ";
                    rule += fdp.ConsumeIntegralInRange<char>('a', 'z');
                    rule += " 1\n";
                    break;
                case 3:
                    rule += "repeated ";
                    rule += fdp.ConsumeIntegralInRange<char>('a', 'z');
                    rule += " 1\n";
                    break;
            }
            break;
        }
            
        case 5: { // Emphasis class definitions
            rule = "class ";
            size_t emph_name_len = fdp.ConsumeIntegralInRange<size_t>(1, 16);
            for (size_t i = 0; i < emph_name_len; i++) {
                char c = fdp.ConsumeIntegralInRange<char>('a', 'z');
                rule += c;
            }
            rule += " ";
            rule += fdp.ConsumeIntegralInRange<char>('a', 'z');
            rule += " ";
            rule += std::to_string(fdp.ConsumeIntegralInRange<uint8_t>(1, 123456));
            rule += "\n";
            break;
        }
            
        case 6: { // After/Before constraints
            uint8_t constraint = fdp.ConsumeIntegral<uint8_t>() % 2;
            if (constraint == 0) {
                rule += "after ";
            } else {
                rule += "before ";
            }
            rule += fdp.ConsumeIntegralInRange<char>('a', 'z');
            rule += " ";
            rule += fdp.ConsumeIntegralInRange<char>('a', 'z');
            rule += " 1\n";
            break;
        }
            
        case 7: { // Multi-pass rules
            rule = "multipass ";
            rule += std::to_string(fdp.ConsumeIntegralInRange<uint8_t>(1, 3));
            rule += "\n";
            
            // Add some additional rules
            size_t extra_rules = fdp.ConsumeIntegralInRange<size_t>(1, 3);
            for (size_t i = 0; i < extra_rules; i++) {
                rule += "space ";
                rule += fdp.ConsumeIntegralInRange<char>('a', 'z');
                rule += " ";
                rule += std::to_string(fdp.ConsumeIntegralInRange<uint8_t>(1, 123456));
                rule += "\n";
            }
            break;
        }
    }
    
    return rule;
}


// Simple ASCII to widechar conversion
static size_t ascii_to_widechar(const char* ascii, widechar* wide, size_t max_len) {
    size_t i;
    for (i = 0; i < max_len && ascii[i] != '\0'; i++) {
        wide[i] = ascii[i];
    }
    return i;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough for table rules generation
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
    
    // Use a simple base table
    const char* base_table = "empty.ctb";
    
    // 1. Generate complex translation table rules with diverse opcodes
    std::string table_rules;
    
    // Start with table version/encoding if needed
    if (fdp.ConsumeBool()) {
        table_rules += "utf-8\n";
    }
    
    // Generate multiple rules to exercise different opcode paths
    size_t num_rules = fdp.ConsumeIntegralInRange<size_t>(3, 15);
    for (size_t i = 0; i < num_rules; i++) {
        if (fdp.remaining_bytes() < 10) break;
        
        uint8_t rule_type = fdp.ConsumeIntegral<uint8_t>();
        std::string rule = generate_table_rule(fdp, rule_type);
        table_rules += rule;
        
        // Add comments occasionally
        if (fdp.ConsumeBool()) {
            table_rules += "# Comment: generated rule " + std::to_string(i) + "\n";
        }
    }
    
    if (table_rules.empty()) {
        return 0;
    }
    
    // 2. Call lou_compileString with the generated rules
    // This directly exercises compileRule with the complex opcodes
    int compile_result = lou_compileString(base_table, table_rules.c_str());
    
    // 3. Call lou_getTable to retrieve and validate the compiled table
    // Note: lou_getTable doesn't exist in the public API, we need to use lou_findTable
    // or lou_checkTable instead. Let's use lou_checkTable for validation.
    
    // 4. Call lou_checkTable to perform comprehensive table validation
    int check_result = lou_checkTable(base_table);
    
    // 5. If compilation was successful, test translation with the compiled rules
    if (compile_result == 1) {
        // Generate some text to translate
        size_t text_len = fdp.ConsumeIntegralInRange<size_t>(1, 128);
        std::string text = fdp.ConsumeRandomLengthString(text_len);
        
        if (!text.empty()) {
            // Convert to widechar
            std::vector<widechar> inbuf(text.size() + 1);
            size_t inlen = ascii_to_widechar(text.c_str(), inbuf.data(), text.size());
            int inlen_int = static_cast<int>(inlen);
            
            // Allocate output buffer
            int outlen_int = inlen_int * 16;
            std::vector<widechar> outbuf(outlen_int + 1);
            
            // Allocate typeform buffer
            std::vector<formtype> typeform_buf(inlen_int + 1);
            
            // Call lou_translateString to test the compiled rules
            int trans_result = lou_translateString(
                base_table,
                inbuf.data(),
                &inlen_int,
                outbuf.data(),
                &outlen_int,
                typeform_buf.data(),
                NULL,
                0
            );
            // Also test back translation
            if (trans_result != 0 && outlen_int > 0) {
                int back_inlen_int = outlen_int;
                int back_outlen_int = inlen_int * 16;
                std::vector<widechar> back_outbuf(back_outlen_int + 1);
                
                std::vector<formtype> back_typeform_buf(back_inlen_int + 1);
                
                int back_trans_result = lou_backTranslateString(
                    base_table,
                    outbuf.data(),
                    &back_inlen_int,
                    back_outbuf.data(),
                    &back_outlen_int,
                    back_typeform_buf.data(),
                    NULL,
                    0
                );
            }
        }
    }
    
    // Clean up
    lou_free();
    
    return 0;
}


} // namespace harness_013
