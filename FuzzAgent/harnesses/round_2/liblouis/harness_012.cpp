#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <vector>
#include <string>
#include <algorithm>
#include <fuzzer/FuzzedDataProvider.h>
#include "liblouis.h"

namespace harness_012 {
// Targeted liblouis fuzzing harness for deep table compilation scenarios
// Harness_012: Specifically targets compileRule function with 558 blocked branches
// Focus: Complex table compilation with varied opcodes, match patterns, and rule structures
// Strategy: Generate semantically rich table entries with opcode combinations, pattern matching,
//           emphasis classes, and edge case compilation scenarios
// Differentiation: Goes beyond simple lou_compileString testing to target internal compileRule logic




// Log callback to suppress output during fuzzing
static void fuzz_log_callback(logLevels level, const char* message) {
    (void)level;
    (void)message;
    // Suppress all logging during fuzzing
}

// Generate a valid table opcode string from fuzzer data
static std::string generate_opcode(FuzzedDataProvider& fdp) {
    // Common opcodes from compileTranslationTable.c that trigger different compileRule paths
    const char* opcodes[] = {
        "space", "digit", "litdigit", "punctuation", "math", "sign", "letter",
        "uppercase", "lowercase", "undefined", "match", "backmatch", "capsletter",
        "begcapsword", "endcapsword", "begcaps", "endcaps", "begcapsphrase", 
        "endcapsphrase", "lencapsphrase", "modeletter", "begmodeword", "endmodeword",
        "begmode", "endmode", "begmodephrase", "endmodephrase", "lenmodephrase",
        "letsign", "noletsign", "noletsignbefore", "noletsignafter", "numsign",
        "nonumsign", "emphclass", "emphletter", "begemphword", "endemphword",
        "begemph", "endemph", "begemphphrase", "endemphphrase", "lenemphphrase",
        "class", "nocross", "nofor", "noback", "display", "uplow", "grouping",
        "multind", "repeated", "always", "context", "swapcd", "swapdd", "comp6",
        "compbrl", "capsmode", "capsnocross", "capsnofor", "capsnoback", "comp",
        "syllable", "correct", "contextn", "compup", "compmid", "compcont",
        "compfinal"
    };
    
    uint8_t opcode_idx = fdp.ConsumeIntegral<uint8_t>() % (sizeof(opcodes)/sizeof(opcodes[0]));
    return std::string(opcodes[opcode_idx]);
}

// Generate character pattern for table rules
static std::string generate_char_pattern(FuzzedDataProvider& fdp, bool for_dots = false) {
    std::string pattern;
    int length = fdp.ConsumeIntegralInRange<int>(1, 10);
    
    if (for_dots) {
        // For braille dot patterns (1-8, 0-255)
        for (int i = 0; i < length; i++) {
            uint8_t dot_pattern = fdp.ConsumeIntegral<uint8_t>();
            pattern += std::to_string(dot_pattern % 256);
            if (i < length - 1) pattern += "-";
        }
    } else {
        // For character patterns (letters, numbers, symbols)
        for (int i = 0; i < length; i++) {
            uint8_t c = fdp.ConsumeIntegral<uint8_t>();
            // Convert to printable ASCII range (32-126)
            c = 32 + (c % 95);
            pattern += static_cast<char>(c);
        }
    }
    
    return pattern;
}

// Generate match pattern characters (before/after patterns)
static std::string generate_match_pattern(FuzzedDataProvider& fdp) {
    std::string pattern;
    uint8_t pattern_type = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    switch (pattern_type) {
        case 0:
            // Single dash (wildcard)
            pattern = "-";
            break;
        case 1:
            // Character class pattern
            pattern = "[";
            pattern += generate_char_pattern(fdp, false);
            pattern += "]";
            break;
        case 2:
            // Negated character class
            pattern = "[^";
            pattern += generate_char_pattern(fdp, false);
            pattern += "]";
            break;
        case 3:
            // Simple character sequence
            pattern = generate_char_pattern(fdp, false);
            break;
    }
    
    return pattern;
}

// Generate a complete table rule with opcode and patterns
static std::string generate_table_rule(FuzzedDataProvider& fdp) {
    std::string rule;
    std::string opcode = generate_opcode(fdp);
    
    rule += opcode;
    rule += " ";
    
    if (opcode == "match" || opcode == "backmatch") {
        // match/backmatch opcodes have special syntax: match <before> <chars> <after> <dots>
        std::string before_pattern = generate_match_pattern(fdp);
        std::string char_pattern = generate_char_pattern(fdp, false);
        std::string after_pattern = generate_match_pattern(fdp);
        std::string dot_pattern = generate_char_pattern(fdp, true);
        
        rule += before_pattern + " " + char_pattern + " " + after_pattern + " " + dot_pattern;
    } else if (opcode == "emphclass") {
        // emphclass needs a name
        std::string class_name = "emph";
        class_name += std::to_string(fdp.ConsumeIntegral<uint8_t>() % 10);
        rule += class_name;
    } else if (opcode == "class") {
        // class opcode needs character class definition
        std::string class_name = "class";
        class_name += std::to_string(fdp.ConsumeIntegral<uint8_t>() % 10);
        std::string char_pattern = generate_char_pattern(fdp, false);
        
        rule += class_name + " " + char_pattern;
    } else if (opcode == "display") {
        // display opcode: character to dots mapping
        std::string char_pattern = generate_char_pattern(fdp, false);
        std::string dot_pattern = generate_char_pattern(fdp, true);
        
        if (char_pattern.length() == 1 && dot_pattern.length() > 0) {
            rule += char_pattern + " " + dot_pattern;
        } else {
            // Fallback to simple rule
            rule += "a 1";
        }
    } else if (opcode == "include") {
        // include opcode needs a filename
        std::string filename = "test";
        filename += std::to_string(fdp.ConsumeIntegral<uint8_t>() % 10);
        filename += ".uti";
        rule += filename;
    } else {
        // Most opcodes follow: <chars> <dots> pattern
        std::string char_pattern = generate_char_pattern(fdp, false);
        std::string dot_pattern = generate_char_pattern(fdp, true);
        
        rule += char_pattern + " " + dot_pattern;
    }
    
    return rule;
}

// Generate emphasis class definition
static std::string generate_emph_class(FuzzedDataProvider& fdp) {
    std::string emph_def;
    uint8_t emph_type = fdp.ConsumeIntegral<uint8_t>() % 3;
    
    switch (emph_type) {
        case 0:
            emph_def = "emphclass emph1\n";
            emph_def += "emphletter a b\n";
            break;
        case 1:
            emph_def = "emphclass emph2\n";
            emph_def += "begemphword [ a\n";
            emph_def += "endemphword a ]\n";
            break;
        case 2:
            emph_def = "emphclass emph3\n";
            emph_def += "begemph [\n";
            emph_def += "endemph ]\n";
            break;
    }
    
    return emph_def;
}

// Generate macro definition
static std::string generate_macro(FuzzedDataProvider& fdp) {
    std::string macro_def;
    if (fdp.ConsumeBool()) {
        macro_def = "macro testmacro $1 $2\n";
        macro_def += "letter a 1\n";
        macro_def += "letter b 12\n";
        macro_def += "eom\n";
    }
    return macro_def;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for complex table generation
    if (size < 100) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize liblouis logging
    static int logging_initialized = 0;
    if (!logging_initialized) {
        lou_registerLogCallback(fuzz_log_callback);
        logging_initialized = 1;
    }
    
    // Select a base table for compilation
    const char* base_tables[] = {
        "en-us-g2.ctb",
        "en-ueb-g2.ctb", 
        "en-us-comp6.ctb",
        "en-us-comp8.ctb",
        "de-g0.utb",
        "sv-g1.ctb",
        NULL
    };
    
    uint8_t table_idx = fdp.ConsumeIntegral<uint8_t>() % 6;
    const char* base_table = base_tables[table_idx];
    
    // Generate complex table compilation string
    std::string compile_string;
    
    // Add table header/metadata
    if (fdp.ConsumeBool()) {
        compile_string += "locale en_US.UTF-8\n";
    }
    
    if (fdp.ConsumeBool()) {
        compile_string += "charset UTF-8\n";
    }
    
    // Add emphasis classes (triggers emphasis-related code paths)
    if (fdp.ConsumeBool()) {
        compile_string += generate_emph_class(fdp);
    }
    
    // Add macros (if enabled in build)
    if (fdp.ConsumeBool()) {
        compile_string += generate_macro(fdp);
    }
    
    // Add character class definitions
    if (fdp.ConsumeBool()) {
        compile_string += "class numbers 0123456789\n";
        compile_string += "class letters abcdefghijklmnopqrstuvwxyz\n";
    }
    
    // Generate multiple table rules to test different opcode combinations
    int rule_count = fdp.ConsumeIntegralInRange<int>(1, 10);
    for (int i = 0; i < rule_count; i++) {
        if (fdp.remaining_bytes() < 10) break;
        
        // Add nocross/nofor/noback modifiers to test conditional compilation
        if (fdp.ConsumeBool()) {
            compile_string += "nocross\n";
        }
        if (fdp.ConsumeBool()) {
            compile_string += "nofor\n";
        }
        if (fdp.ConsumeBool()) {
            compile_string += "noback\n";
        }
        
        std::string rule = generate_table_rule(fdp);
        compile_string += rule + "\n";
        
        // Reset modifiers after each rule
        if (i < rule_count - 1) {
            compile_string += "\n";
        }
    }
    
    // Add some edge case rules
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 20) {
        // Test with grouping opcode
        compile_string += "grouping ()\n";
        compile_string += "letter ( 2345\n";
        compile_string += "letter ) 1236\n";
    }
    
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 20) {
        // Test with repeated opcode
        compile_string += "repeated\n";
        compile_string += "letter a 1\n";
    }
    
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 20) {
        // Test with always opcode
        compile_string += "always\n";
        compile_string += "letter b 12\n";
    }
    
    // Call lou_compileString with the generated complex table
    // This will trigger compileRule internally with various opcodes and patterns
    int compile_result = lou_compileString(base_table, compile_string.c_str());
    
    // Test lou_getTable to see if table was loaded successfully
    const void* table_ptr = lou_getTable(base_table);
    
    // If compilation succeeded and table was loaded, test some operations
    if (compile_result && table_ptr) {
        // Test getting emphasis classes (if any were defined)
        char const** emph_classes = lou_getEmphClasses(base_table);
        
        // Test emphasis class to typeform conversion
        if (emph_classes && emph_classes[0]) {
            formtype typeform = lou_getTypeformForEmphClass(base_table, emph_classes[0]);
            (void)typeform; // Use result to avoid unused variable warning
        }
        
        // Test simple translation with the compiled table
        if (fdp.remaining_bytes() > 10) {
            std::string test_text = fdp.ConsumeRandomLengthString(50);
            if (!test_text.empty()) {
                // Convert to widechar
                widechar* inbuf = (widechar*)malloc((test_text.length() + 1) * sizeof(widechar));
                widechar* outbuf = (widechar*)malloc((test_text.length() * 10 + 1) * sizeof(widechar));
                int* inpos = (int*)malloc((test_text.length() + 1) * sizeof(int));
                int* outpos = (int*)malloc((test_text.length() * 10 + 1) * sizeof(int));
                
                if (inbuf && outbuf && inpos && outpos) {
                    for (size_t i = 0; i < test_text.length(); i++) {
                        inbuf[i] = (widechar)(unsigned char)test_text[i];
                    }
                    inbuf[test_text.length()] = 0;
                    
                    int inlen = test_text.length();
                    int outlen = test_text.length() * 10;
                    int result = lou_translate(base_table, inbuf, &inlen, outbuf, &outlen, 
                                              NULL, NULL, NULL, NULL, NULL, 0);
                    (void)result; // Use result to avoid unused variable warning
                }
                
                free(inbuf);
                free(outbuf);
                free(inpos);
                free(outpos);
            }
        }
    }

    // Clean up - lou_free will handle resources from lou_compileString
    // Don't call lou_free() here as it would affect other harnesses running in same process
    
    return 0;
}

} // namespace harness_012
