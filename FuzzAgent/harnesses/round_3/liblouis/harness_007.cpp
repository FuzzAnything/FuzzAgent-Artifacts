#include <fuzzer/FuzzedDataProvider.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <liblouis/liblouis.h>
#include <assert.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <vector>
#include <string>

namespace harness_007 {
// liblouis fuzzing harness - Advanced table compilation with diverse opcode sequences
// Targets: lou_compileString and compilation functions that call compileRule
// Focus: Advanced opcode sequences to target compileRule function with 339 blocked branches
// Uses FuzzedDataProvider for structured input processing
// Strategy: Generate diverse opcode combinations and table rules from fuzzer input


// Global initialization flag
static int initialized = 0;

// Log callback to suppress output during fuzzing
void avoid_log(logLevels level, const char *msg) {
    (void)level;
    (void)msg;
}

// Cleanup function
static void free_resources(void) {
    lou_free();
}

// List of common opcodes for generating diverse sequences
static const char* opcode_list[] = {
    "space", "digit", "punctuation", "math", "sign", "letter", "uppercase", "lowercase",
    "litdigit", "display", "replace", "context", "correct", "pass2", "pass3", "pass4",
    "repeated", "repword", "repmidword", "rependword", "capsnocont", "always", "exactdots",
    "nocross", "syllable", "nocont", "compbrl", "literal", "largesign", "wholeword",
    "partword", "joinnum", "joinableword", "lowword", "contraction", "suffixableword",
    "prefixableword", "begword", "begmidword", "midword", "midendword", "endword",
    "prepunc", "postpunc", "begnum", "midnum", "endnum", "decpoint", "hyphen", "nobreak",
    "match", "backmatch", "attribute", "base", "class", "after", "before", "noback",
    "nofor", "empmatchbefore", "empmatchafter", "swapcc", "swapcd", "swapdd", "grouping",
    "uplow", "undefined", "include", "locale", "capsletter", "begcapsword", "endcapsword",
    "begcaps", "endcaps", "begcapsphrase", "endcapsphrase", "lencapsphrase", "modeletter",
    "begmodeword", "endmodeword", "begmode", "endmode", "begmodephrase", "endmodephrase",
    "lenmodephrase", "letsign", "noletsignbefore", "noletsign", "noletsignafter", "numsign",
    "nonumsign", "numericmodechars", "midendnumericmodechars", "numericnocontchars",
    "seqdelimiter", "seqbeforechars", "seqafterchars", "seqafterpattern", "seqafterexpression",
    "emphclass", "empletter", "begemphword", "endemphword", "begemph", "endemph",
    "begemphpfrase", "endemphpfrase", "lenemphpfrase", "capsmodechars", "emphmodechars",
    "noemphchars", "begcomp", "endcomp", "nocontractsign", "multind", "compdots", "comp6"
};

static const int opcode_count = sizeof(opcode_list) / sizeof(opcode_list[0]);

// Generate a random opcode rule from fuzzer data
static std::string generate_opcode_rule(FuzzedDataProvider& fdp) {
    std::string rule;
    
    // Choose random opcode
    int opcode_idx = fdp.ConsumeIntegralInRange<int>(0, opcode_count - 1);
    rule = opcode_list[opcode_idx];
    rule += " ";
    
    // Generate parameters based on opcode type
    uint8_t param_type = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    switch (param_type) {
        case 0: {
            // Simple character to dots mapping
            rule += "\\x";
            rule += "0123456789abcdef"[fdp.ConsumeIntegralInRange<int>(0, 15)];
            rule += "0123456789abcdef"[fdp.ConsumeIntegralInRange<int>(0, 15)];
            rule += "0123456789abcdef"[fdp.ConsumeIntegralInRange<int>(0, 15)];
            rule += "0123456789abcdef"[fdp.ConsumeIntegralInRange<int>(0, 15)];
            rule += " ";
            rule += "123456789abcdef"[fdp.ConsumeIntegralInRange<int>(0, 14)];
            break;
        }
            
        case 1: {
            // String to dots mapping
            rule += "\"";
            int str_len = fdp.ConsumeIntegralInRange<int>(1, 8);
            for (int i = 0; i < str_len; i++) {
                char c = fdp.ConsumeIntegralInRange<char>('a', 'z');
                rule += c;
            }
            rule += "\" ";
            
            int dots_len = fdp.ConsumeIntegralInRange<int>(1, 4);
            for (int i = 0; i < dots_len; i++) {
                rule += "123456789abcdef"[fdp.ConsumeIntegralInRange<int>(0, 14)];
            }
            break;
        }
            
        case 2: {
            // Character class definition
            rule += "\"";
            char c = fdp.ConsumeIntegralInRange<char>('a', 'z');
            rule += c;
            rule += "\" ";
            rule += "classname";
            break;
        }
            
        case 3: {
            // Match pattern with before/after patterns
            rule += "\"";
            rule += "test";
            rule += "\" ";
            rule += "123456 ";
            rule += "\"";
            rule += "before";
            rule += "\" \"";
            rule += "after";
            rule += "\"";
            break;
        }
    }
    
    return rule;
}

// Generate a complete table with multiple opcode rules
static std::string generate_table_rules(FuzzedDataProvider& fdp) {
    std::string table_rules;
    int rule_count = fdp.ConsumeIntegralInRange<int>(1, 10);
    
    for (int i = 0; i < rule_count; i++) {
        if (fdp.remaining_bytes() < 10) break;
        table_rules += generate_opcode_rule(fdp);
        table_rules += "\n";
    }
    
    return table_rules;
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Minimum size check
    if (size < 32) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize liblouis once
    if (!initialized) {
        lou_registerLogCallback(avoid_log);
        initialized = 1;
        // Register cleanup
        atexit(free_resources);
    }
    
    // Consume operation type
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    // Generate table name from input
    std::string table_name = fdp.ConsumeRandomLengthString(64);
    if (table_name.empty()) {
        table_name = "test-table";
    }
    
    // Generate rule string from input
    std::string rule_string;
    
    if (operation == 0) {
        // For compilation operations, generate diverse opcode sequences
        rule_string = generate_table_rules(fdp);
        
        // Add terminator
        rule_string += "LOU_ENDSEGMENT";
    } else {
        // For other operations, use random string
        rule_string = fdp.ConsumeRandomLengthString(512);
    }
    
    if (rule_string.empty()) {
        return 0;
    }
    
    int result = 0;
    
    switch (operation) {
        case 0:
            // Test lou_compileString with diverse opcode sequences
            result = lou_compileString(table_name.c_str(), rule_string.c_str());
            (void)result;
            break;
            
        case 1:
            // Test lou_checkTable with random table names
            result = lou_checkTable(table_name.c_str());
            (void)result;
            break;
            
        case 2: {
            // Test compilation and then translation
            // First compile a rule
            std::string simple_rule = "space \\x0020 123456 LOU_ENDSEGMENT";
            result = lou_compileString(table_name.c_str(), simple_rule.c_str());
            
            // Then try to get the table and use it
            if (result) {
                const void* table = lou_getTable(table_name.c_str());
                (void)table;
                
                // Try to translate something with it
                if (fdp.remaining_bytes() > 0) {
                    std::string text = fdp.ConsumeRandomLengthString(64);
                    if (!text.empty()) {
                        // Convert to widechar
                        size_t text_len = text.length();
                        std::vector<widechar> wide_input(text_len + 1);
                        for (size_t i = 0; i < text_len; i++) {
                            wide_input[i] = (widechar)text[i];
                        }
                        wide_input[text_len] = 0;
                        
                        int inlen = (int)text_len;
                        int outlen = inlen * 16;
                        std::vector<widechar> output(outlen + 1);
                        
                        int translate_result = lou_translateString(
                            table_name.c_str(), 
                            wide_input.data(), 
                            &inlen,
                            output.data(), 
                            &outlen, 
                            NULL, 
                            NULL, 
                            noContractions
                        );
                        (void)translate_result;
                    }
                }
            }
            break;
        }
            
        case 3: {
            // Test with temporary file creation
            // Create a temporary file with fuzzer data
            char filename[256];
            static int file_counter = 0;
            snprintf(filename, sizeof(filename), "/tmp/liblouis-fuzz-adv-%d-%d.ctb", getpid(), file_counter++);
            
            // Write some fuzzer data to the file
            std::vector<uint8_t> file_data = fdp.ConsumeRemainingBytes<uint8_t>();
            if (!file_data.empty()) {
                FILE* fp = fopen(filename, "wb");
                if (fp) {
                    fwrite(file_data.data(), 1, file_data.size(), fp);
                    fclose(fp);
                    
                    // Test table checking
                    result = lou_checkTable(filename);
                    (void)result;
                    
                    // Try to compile string with this table name
                    if (result) {
                        lou_compileString(filename, "space \\x0020 123456 LOU_ENDSEGMENT");
                    }
                    
                    // Clean up
                    unlink(filename);
                }
            }
            break;
        }
    }
    
    return 0;
}

} // namespace harness_007
