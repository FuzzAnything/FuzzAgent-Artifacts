#include <fuzzer/FuzzedDataProvider.h>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>
#include <iostream>
#include "liblouis.h"

namespace harness_011 {
// liblouis multipass rule compilation fuzzing harness
// Targets multipass rule compilation via lou_compileString to exercise compilePassOpcode function
// Based on coverage analysis guidance: compilePassOpcode has 165 blocked branches (3rd highest overall)
// Focuses on testing multipass rules: pass, pass2, pass3, pass4 opcodes with various operands


// Include liblouis public header
extern "C" {
}

// Log callback to suppress logs during fuzzing
static void avoid_log_callback(logLevels level, const char *msg) {
    (void)level;
    (void)msg;
}

// Simple ASCII to widechar conversion
static size_t ascii_to_widechar(const char* ascii, widechar* wide, size_t max_len) {
    size_t i;
    for (i = 0; i < max_len && ascii[i] != '\0'; i++) {
        wide[i] = ascii[i];
    }
    return i;
}

// Function to generate multipass rule from fuzzer input
static std::string generate_multipass_rule(FuzzedDataProvider& fdp) {
    std::string rule;
    
    // Choose pass opcode type: pass, pass2, pass3, pass4
    uint8_t pass_type = fdp.ConsumeIntegral<uint8_t>() % 4;
    uint8_t direction = fdp.ConsumeBool() ? 1 : 0; // noback or nofor
    
    // Build rule prefix
    if (direction == 0) {
        rule += "noback ";
    } else {
        rule += "nofor ";
    }
    
    switch (pass_type) {
        case 0: rule += "pass "; break;
        case 1: rule += "pass2 "; break;
        case 2: rule += "pass3 "; break;
        case 3: rule += "pass4 "; break;
    }
    
    // Generate test part with various multipass operations
    // Based on compilePassOpcode switch cases: lookback, not, first, last, search, string, dots
    uint8_t test_op_count = fdp.ConsumeIntegralInRange<uint8_t>(1, 5);
    
    for (uint8_t i = 0; i < test_op_count; i++) {
        uint8_t test_op = fdp.ConsumeIntegral<uint8_t>() % 7;
        
        switch (test_op) {
            case 0: // lookback
                rule += "_";
                if (fdp.ConsumeBool()) {
                    uint8_t lookback_num = fdp.ConsumeIntegralInRange<uint8_t>(1, 9);
                    rule += std::to_string(lookback_num);
                }
                break;
                
            case 1: // not
                rule += "!";
                break;
                
            case 2: // first
                rule += "^";
                break;
                
            case 3: // last
                rule += "$";
                break;
                
            case 4: // search - create search pattern
                if (fdp.ConsumeBool()) {
                    rule += "[";
                    uint8_t pattern_len = fdp.ConsumeIntegralInRange<uint8_t>(1, 5);
                    for (uint8_t j = 0; j < pattern_len; j++) {
                        char c = fdp.ConsumeIntegralInRange<char>('a', 'z');
                        rule += c;
                    }
                    rule += "]";
                }
                break;
                
            case 5: { // string operand
                if (fdp.ConsumeBool()) {
                    rule += "@";
                    uint8_t dots = fdp.ConsumeIntegralInRange<uint8_t>(1, 8);
                    rule += std::to_string(dots);
                }
                break;
            }
                
            case 6: { // dots operand
                if (fdp.ConsumeBool()) {
                    rule += "@";
                    uint8_t dot_pattern = fdp.ConsumeIntegral<uint8_t>() % 256;
                    std::string dot_str;
                    if (dot_pattern & 0x01) dot_str += "1";
                    if (dot_pattern & 0x02) dot_str += "2";
                    if (dot_pattern & 0x04) dot_str += "3";
                    if (dot_pattern & 0x08) dot_str += "4";
                    if (dot_pattern & 0x10) dot_str += "5";
                    if (dot_pattern & 0x20) dot_str += "6";
                    if (dot_pattern & 0x40) dot_str += "7";
                    if (dot_pattern & 0x80) dot_str += "8";
                    if (!dot_str.empty()) {
                        rule += dot_str;
                    } else {
                        rule += "0";
                    }
                }
                break;
            }
        }
        
        // Add separator if not last operation
        if (i < test_op_count - 1) {
            rule += " ";
        }
    }
    
    // Add separator between test and action parts
    rule += " @";
    
    // Generate action part
    uint8_t action_op_count = fdp.ConsumeIntegralInRange<uint8_t>(1, 3);
    
    for (uint8_t i = 0; i < action_op_count; i++) {
        uint8_t action_type = fdp.ConsumeIntegral<uint8_t>() % 3;
        
        switch (action_type) {
            case 0: { // string output
                rule += "@";
                uint8_t out_dots = fdp.ConsumeIntegralInRange<uint8_t>(1, 8);
                rule += std::to_string(out_dots);
                break;
            }
                
            case 1: { // character output
                if (fdp.ConsumeBool()) {
                    char out_char = fdp.ConsumeIntegralInRange<char>('a', 'z');
                    rule += out_char;
                }
                break;
            }
                
            case 2: { // dots pattern
                rule += "@";
                uint8_t out_pattern = fdp.ConsumeIntegral<uint8_t>() % 256;
                std::string out_dot_str;
                if (out_pattern & 0x01) out_dot_str += "1";
                if (out_pattern & 0x02) out_dot_str += "2";
                if (out_pattern & 0x04) out_dot_str += "3";
                if (out_pattern & 0x08) out_dot_str += "4";
                if (out_pattern & 0x10) out_dot_str += "5";
                if (out_pattern & 0x20) out_dot_str += "6";
                if (out_pattern & 0x40) out_dot_str += "7";
                if (out_pattern & 0x80) out_dot_str += "8";
                if (!out_dot_str.empty()) {
                    rule += out_dot_str;
                } else {
                    rule += "0";
                }
                break;
            }
        }
        
        // Add separator if not last operation
        if (i < action_op_count - 1) {
            rule += "-";
        }
    }
    
    return rule;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough for rule generation and text
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
    
    // Select a base table - use common tables that support multipass
    const char* tables[] = {
        "en-us-comp8.ctb",
        "en-ueb-g2.ctb", 
        "da-dk-g28.ctb",  // Has multipass rules
        "en-g3.ctb"       // Has multipass rules
    };
    const size_t num_tables = sizeof(tables) / sizeof(tables[0]);
    size_t table_idx = fdp.ConsumeIntegralInRange<size_t>(0, num_tables - 1);
    const char* base_table = tables[table_idx];
    
    // Generate multipass rules from fuzzer input
    uint8_t rule_count = fdp.ConsumeIntegralInRange<uint8_t>(1, 5);
    std::vector<std::string> multipass_rules;
    
    for (uint8_t i = 0; i < rule_count; i++) {
        if (fdp.remaining_bytes() < 10) break;
        multipass_rules.push_back(generate_multipass_rule(fdp));
    }
    
    if (multipass_rules.empty()) {
        return 0;
    }
    
    // Create a complete table string with multipass rules
    std::string table_content;
    
    // Start with base table
    table_content = "include ";
    table_content += base_table;
    table_content += "\n";
    
    // Add multipass rules
    for (const auto& rule : multipass_rules) {
        table_content += rule;
        table_content += "\n";
    }
    
    // 1. Call lou_compileString to compile multipass rules
    int compile_result = lou_compileString(base_table, table_content.c_str());
    
    // Even if compilation fails, we've exercised compilePassOpcode path
    // Failed compilation still tests error handling paths in compilePassOpcode
    
    if (compile_result == 1) {
        // 2. Call lou_translate using the compiled tables to exercise multipass execution path
        // Consume text to translate
        size_t max_text_len = fdp.ConsumeIntegralInRange<size_t>(1, 128);
        std::string text = fdp.ConsumeRandomLengthString(max_text_len);
        
        if (!text.empty()) {
            // Convert to widechar
            std::vector<widechar> inbuf(text.size() + 1);
            size_t inlen = ascii_to_widechar(text.c_str(), inbuf.data(), text.size());
            int inlen_int = static_cast<int>(inlen);
            
            // Allocate output buffer
            int outlen_int = inlen_int * 16;
            std::vector<widechar> outbuf(outlen_int + 1);
            
            // Select mode
            int mode = 0;
            uint8_t mode_choice = fdp.ConsumeIntegral<uint8_t>() % 3;
            switch (mode_choice) {
                case 0:
                    mode = 0;  // plain_text
                    break;
                case 1:
                    mode = 0x0400;  // computer_braille
                    break;
                case 2:
                    mode = 0x1000;  // no_contract
                    break;
            }
            
            // Call lou_translate with position tracking (can be NULL)
            int actual_inlen = inlen_int;
            int actual_outlen = outlen_int;
            std::vector<int> outputPos(inlen_int);
            std::vector<int> inputPos(outlen_int);
            int cursorPos = -1;
            
            int translate_result = lou_translate(
                base_table, 
                inbuf.data(), 
                &actual_inlen,
                outbuf.data(), 
                &actual_outlen,
                NULL,  // typeform
                NULL,  // spacing
                outputPos.data(),  // outputPos
                inputPos.data(),   // inputPos
                &cursorPos,        // cursorPos
                mode
            );
            
            // Also test lou_translateString
            if (fdp.remaining_bytes() > 10) {
                size_t max_text2_len = fdp.ConsumeIntegralInRange<size_t>(1, 64);
                std::string text2 = fdp.ConsumeRandomLengthString(max_text2_len);
                
                if (!text2.empty()) {
                    // Convert to widechar
                    std::vector<widechar> inbuf2(text2.size() + 1);
                    size_t inlen2 = ascii_to_widechar(text2.c_str(), inbuf2.data(), text2.size());
                    int inlen2_int = static_cast<int>(inlen2);
                    
                    // Allocate output buffer
                    int outlen2_int = inlen2_int * 16;
                    std::vector<widechar> outbuf2(outlen2_int + 1);
                    
                    // Call lou_translateString
                    int actual_inlen2 = inlen2_int;
                    int actual_outlen2 = outlen2_int;
                    
                    lou_translateString(
                        base_table,
                        inbuf2.data(),
                        &actual_inlen2,
                        outbuf2.data(),
                        &actual_outlen2,
                        NULL,  // typeform
                        NULL,  // spacing
                        mode
                    );
                }
            }
        }  // End of if (!text.empty())
        
        // 3. Call lou_free to clean up compiled tables
        lou_free();
    } else {
        // Compilation failed, still need to clean up if table was loaded
        lou_free();
    }
    
    return 0;
}

} // namespace harness_011
