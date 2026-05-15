#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <vector>
#include <string>
#include <algorithm>
#include <fuzzer/FuzzedDataProvider.h>
#include "liblouis.h"

namespace harness_013 {
// Targeted liblouis fuzzing harness for compileGrouping function
// Harness_013: Specifically targets compileGrouping function with 154 blocked branches (0% coverage)
// Focus: Comprehensive grouping functionality via lou_compileString with grouping rules in table definitions
// Strategy: Generate complex grouping scenarios including nested groups, varied delimiter pairs,
//           combined with translation operations to exercise grouping logic during translation
// Differentiation: Deep focus on grouping-specific syntax and semantics unlike existing harnesses




// Log callback to suppress output during fuzzing
static void fuzz_log_callback(logLevels level, const char* message) {
    (void)level;
    (void)message;
    // Suppress all logging during fuzzing
}

// Destructor to free liblouis resources
static void __attribute__((destructor)) free_resources(void) {
    lou_free();
}

// Generate valid grouping pairs according to compileGrouping requirements
// From analysis: grouping requires name operand, two Unicode characters, and two cells separated by a comma
// Format: grouping <name> <char1><char2> <dots1>,<dots2>
static std::string generate_grouping_pair(FuzzedDataProvider& fdp) {
    std::string grouping = "grouping ";
    
    // Grouping name (required token)
    uint8_t name_type = fdp.ConsumeIntegral<uint8_t>() % 4;
    switch (name_type) {
        case 0:
            grouping += "test";
            break;
        case 1:
            grouping += "pair";
            break;
        case 2:
            if (fdp.remaining_bytes() > 1) {
                char c = fdp.ConsumeIntegral<char>();
                c = 32 + (c % 95);
                grouping += std::string(1, c);
            } else {
                grouping += "g";
            }
            break;
        case 3:
            // Empty name (invalid but tests error path)
            grouping += "";
            break;
    }
    
    // Space after name
    if (!grouping.empty() && grouping.back() != ' ') {
        grouping += " ";
    }
    
    // First character of the pair (must be printable for meaningful testing)
    char char1 = '(';
    if (fdp.remaining_bytes() > 0) {
        char1 = fdp.ConsumeIntegral<char>();
        // Ensure printable ASCII
        char1 = 32 + (char1 % 95);
    }
    
    // Second character of the pair
    char char2 = ')';
    if (fdp.remaining_bytes() > 0) {
        char2 = fdp.ConsumeIntegral<char>();
        char2 = 32 + (char2 % 95);
    }
    
    grouping += std::string(1, char1) + std::string(1, char2);
    grouping += " ";
    
    // Dots operand - must be two cells separated by a comma
    // According to compileGrouping: "Dots operand must consist of two cells separated by a comma"
    
    // First braille cell (0-255)
    uint8_t cell1 = fdp.ConsumeIntegral<uint8_t>() % 256;
    grouping += std::to_string(cell1);
    
    // Comma separator (required)
    grouping += ",";
    
    // Second braille cell
    uint8_t cell2 = fdp.ConsumeIntegral<uint8_t>() % 256;
    grouping += std::to_string(cell2);
    
    return grouping;
}

// Generate nested grouping structure (multiple grouping rules)
static std::string generate_nested_grouping(FuzzedDataProvider& fdp) {
    std::string nested;
    
    int level_count = fdp.ConsumeIntegralInRange<int>(1, 4);
    for (int level = 0; level < level_count; level++) {
        if (fdp.remaining_bytes() < 10) break;
        
        // Opening delimiter
        char open_char;
        uint8_t open_choice = fdp.ConsumeIntegral<uint8_t>() % 6;
        switch (open_choice) {
            case 0: open_char = '('; break;
            case 1: open_char = '['; break;
            case 2: open_char = '{'; break;
            case 3: open_char = '<'; break;
            case 4: open_char = '"'; break;
            case 5: 
                if (fdp.remaining_bytes() > 0) {
                    open_char = fdp.ConsumeIntegral<char>();
                    open_char = 32 + (open_char % 95);
                } else {
                    open_char = '(';
                }
                break;
        }
        
        // Closing delimiter (should match or be different)
        char close_char;
        uint8_t close_choice = fdp.ConsumeIntegral<uint8_t>() % 3;
        switch (close_choice) {
            case 0:
                // Matching pair
                if (open_char == '(') close_char = ')';
                else if (open_char == '[') close_char = ']';
                else if (open_char == '{') close_char = '}';
                else if (open_char == '<') close_char = '>';
                else if (open_char == '"') close_char = '"';
                else close_char = open_char + 1; // Simple offset
                break;
            case 1:
                // Different character
                if (fdp.remaining_bytes() > 0) {
                    close_char = fdp.ConsumeIntegral<char>();
                    close_char = 32 + (close_char % 95);
                } else {
                    close_char = ')';
                }
                break;
            case 2:
                // Same character (invalid but tests error handling)
                close_char = open_char;
                break;
        }
        
        // Generate grouping rule
        std::string open_str(1, open_char);
        std::string close_str(1, close_char);
        
        // First cell dots (1-256)
        uint8_t dots1 = fdp.ConsumeIntegral<uint8_t>() % 256;
        uint8_t dots2 = fdp.ConsumeIntegral<uint8_t>() % 256;
        
        nested += "grouping " + open_str + close_str + " " + 
                  std::to_string(dots1) + "," + std::to_string(dots2) + "\n";
        
        // Add corresponding braille mappings for the delimiters
        // This helps test grouping during translation
        nested += "letter " + open_str + " " + std::to_string(dots1) + "\n";
        nested += "letter " + close_str + " " + std::to_string(dots2) + "\n";
    }
    
    return nested;
}

// Generate complex table with grouping and translation test patterns
static std::string generate_grouping_table(FuzzedDataProvider& fdp, bool include_translation_test = true) {
    std::string table;
    
    // Table metadata
    if (fdp.ConsumeBool()) {
        table += "locale en_US.UTF-8\n";
        table += "charset UTF-8\n";
    }
    
    // Add grouping rules
    int grouping_count = fdp.ConsumeIntegralInRange<int>(1, 8);
    for (int i = 0; i < grouping_count; i++) {
        if (fdp.remaining_bytes() < 20) break;
        
        // Choose grouping type
        uint8_t group_type = fdp.ConsumeIntegral<uint8_t>() % 3;
        switch (group_type) {
            case 0:
                // Simple pair
                table += generate_grouping_pair(fdp) + "\n";
                break;
            case 1:
                // Nested structure
                table += generate_nested_grouping(fdp);
                break;
            case 2:
                // Edge case: invalid grouping syntax (tests error paths)
                table += "grouping abc\n";  // Missing dots operand
                if (fdp.ConsumeBool()) {
                    table += "grouping a 1\n";  // Missing comma in dots
                }
                if (fdp.ConsumeBool()) {
                    table += "grouping ab 1,2,3\n";  // Too many dots
                }
                break;
        }
    }
    
    // Add character mappings to test grouping during translation
    if (include_translation_test && fdp.remaining_bytes() > 10) {
        // Add some letter mappings that might interact with grouping
        table += "\n# Character mappings\n";
        
        int mapping_count = fdp.ConsumeIntegralInRange<int>(1, 6);
        for (int i = 0; i < mapping_count; i++) {
            if (fdp.remaining_bytes() < 5) break;
            
            char letter = 'a' + (fdp.ConsumeIntegral<uint8_t>() % 26);
            uint8_t dots = fdp.ConsumeIntegral<uint8_t>() % 8 + 1;
            table += "letter " + std::string(1, letter) + " " + std::to_string(dots) + "\n";
        }
        
        // Add punctuation that might be used as grouping delimiters
        if (fdp.ConsumeBool()) {
            const char* punctuation[] = {".", ",", ";", ":", "!", "?", "'", "\""};
            int punct_idx = fdp.ConsumeIntegral<uint8_t>() % 8;
            uint8_t punct_dots = fdp.ConsumeIntegral<uint8_t>() % 8 + 1;
            table += "punctuation " + std::string(punctuation[punct_idx]) + " " + 
                     std::to_string(punct_dots) + "\n";
        }
    }
    
    // Add conditional modifiers (affect grouping compilation)
    if (fdp.ConsumeBool()) {
        table += "noback\n";
    }
    if (fdp.ConsumeBool()) {
        table += "nofor\n";
    }
    
    return table;
}

// Generate test text that includes potential grouping delimiters
static std::string generate_grouping_test_text(FuzzedDataProvider& fdp) {
    std::string text;
    
    int length = fdp.ConsumeIntegralInRange<int>(1, 100);
    for (int i = 0; i < length; i++) {
        if (fdp.remaining_bytes() < 1) break;
        
        uint8_t char_type = fdp.ConsumeIntegral<uint8_t>() % 10;
        switch (char_type) {
            case 0: text += '('; break;
            case 1: text += ')'; break;
            case 2: text += '['; break;
            case 3: text += ']'; break;
            case 4: text += '{'; break;
            case 5: text += '}'; break;
            case 6: text += '<'; break;
            case 7: text += '>'; break;
            case 8: text += '"'; break;
            case 9:
                // Regular letter
                text += 'a' + (fdp.ConsumeIntegral<uint8_t>() % 26);
                break;
        }
    }
    
    return text;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - grouping requires significant input
    if (size < 50) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize liblouis logging
    static int logging_initialized = 0;
    if (!logging_initialized) {
        lou_registerLogCallback(fuzz_log_callback);
        logging_initialized = 1;
    }
    
    // Select operation mode
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    // Choose base table (some may have existing grouping definitions)
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
    
    // Generate grouping-focused compilation string
    std::string compile_string;
    
    switch (operation) {
        case 0:
            // Focus: Basic grouping compilation
            compile_string = generate_grouping_table(fdp, false);
            break;
            
        case 1:
            // Focus: Nested grouping with translation mappings
            compile_string = generate_grouping_table(fdp, true);
            break;
            
        case 2:
            // Focus: Edge cases and error paths in grouping
            compile_string = generate_grouping_table(fdp, false);
            // Add intentionally problematic grouping rules
            if (fdp.ConsumeBool()) {
                compile_string += "grouping a 1\n";  // Missing second char and comma
            }
            if (fdp.ConsumeBool()) {
                compile_string += "grouping abc 1,2\n";  // Three chars, should fail
            }
            if (fdp.ConsumeBool()) {
                compile_string += "grouping ab 1\n";  // Missing comma
            }
            break;
            
        case 3:
            // Focus: Multiple grouping rules with varied syntax
            compile_string = "locale en_US.UTF-8\n";
            compile_string += "charset UTF-8\n";
            
            // Add standard mathematical grouping
            compile_string += "grouping () 1234,2345\n";
            compile_string += "letter ( 1234\n";
            compile_string += "letter ) 2345\n";
            
            // Add bracket grouping
            if (fdp.ConsumeBool()) {
                compile_string += "grouping [] 123,234\n";
                compile_string += "letter [ 123\n";
                compile_string += "letter ] 234\n";
            }
            
            // Add brace grouping
            if (fdp.ConsumeBool()) {
                compile_string += "grouping {} 134,124\n";
                compile_string += "letter { 134\n";
                compile_string += "letter } 124\n";
            }
            
            // Add custom grouping
            if (fdp.remaining_bytes() > 10) {
                char c1 = fdp.ConsumeIntegral<char>();
                char c2 = fdp.ConsumeIntegral<char>();
                c1 = 32 + (c1 % 95);
                c2 = 32 + (c2 % 95);
                uint8_t d1 = fdp.ConsumeIntegral<uint8_t>() % 256;
                uint8_t d2 = fdp.ConsumeIntegral<uint8_t>() % 256;
                
                compile_string += "grouping " + std::string(1, c1) + std::string(1, c2) + " " +
                                 std::to_string(d1) + "," + std::to_string(d2) + "\n";
                compile_string += "letter " + std::string(1, c1) + " " + std::to_string(d1) + "\n";
                compile_string += "letter " + std::string(1, c2) + " " + std::to_string(d2) + "\n";
            }
            break;
    }
    
    // Call lou_compileString with grouping-focused table
    int compile_result = lou_compileString(base_table, compile_string.c_str());
    
    // If compilation succeeded, test grouping during translation
    if (compile_result) {
        const void* table_ptr = lou_getTable(base_table);
        
        if (table_ptr && fdp.remaining_bytes() > 20) {
            // Generate test text with grouping delimiters
            std::string test_text = generate_grouping_test_text(fdp);
            
            if (!test_text.empty()) {
                // Convert to widechar for translation
                widechar* inbuf = (widechar*)malloc((test_text.length() + 1) * sizeof(widechar));
                widechar* outbuf = (widechar*)malloc((test_text.length() * 10 + 1) * sizeof(widechar));
                int* inpos = (int*)malloc((test_text.length() + 1) * sizeof(int));
                int* outpos = (int*)malloc((test_text.length() * 10 + 1) * sizeof(int));
                
                if (inbuf && outbuf && inpos && outpos) {
                    // Simple ASCII to widechar conversion
                    for (size_t i = 0; i < test_text.length(); i++) {
                        inbuf[i] = (widechar)(unsigned char)test_text[i];
                    }
                    inbuf[test_text.length()] = 0;
                    
                    int inlen = test_text.length();
                    int outlen = test_text.length() * 10;
                    
                    // Test translation which should exercise grouping logic
                    int trans_result = lou_translate(base_table, inbuf, &inlen, outbuf, &outlen,
                                                     NULL, NULL, NULL, NULL, NULL, 0);
                    (void)trans_result; // Use to avoid unused variable warning
                    
                    // Also test backtranslation to exercise grouping in reverse
                    if (fdp.ConsumeBool() && outlen > 0) {
                        widechar* back_in = (widechar*)malloc((outlen + 1) * sizeof(widechar));
                        widechar* back_out = (widechar*)malloc((outlen * 10 + 1) * sizeof(widechar));
                        int back_inlen = outlen;
                        int back_outlen = outlen * 10;
                        
                        if (back_in && back_out) {
                            for (int i = 0; i < outlen; i++) {
                                back_in[i] = outbuf[i];
                            }
                            back_in[outlen] = 0;
                            
                            lou_backTranslate(base_table, back_in, &back_inlen, back_out, &back_outlen,
                                            NULL, NULL, NULL, NULL, NULL, 0);
                            
                            free(back_in);
                            free(back_out);
                        }
                    }
                }
                
                free(inbuf);
                free(outbuf);
                free(inpos);
                free(outpos);
            }
        }
    }
    
    // Test emphasis classes if they might interact with grouping
    if (fdp.ConsumeBool()) {
        char const** emph_classes = lou_getEmphClasses(base_table);
        if (emph_classes && emph_classes[0]) {
            formtype typeform = lou_getTypeformForEmphClass(base_table, emph_classes[0]);
            (void)typeform;
        }
    }
    
    // Cleanup handled by destructor
    return 0;
}

} // namespace harness_013
