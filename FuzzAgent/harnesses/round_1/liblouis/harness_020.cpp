#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <string>
#include <vector>
#include <algorithm>
#include <unistd.h>
#include <fuzzer/FuzzedDataProvider.h>
#include "liblouis/liblouis.h"

namespace harness_020 {
/*
 * liblouis Advanced Table Compilation and Character Attribute Testing Harness (harness_020.cpp)
 * 
 * This harness targets advanced table compilation, character attribute handling, and
 * internal pattern matching APIs that haven't been extensively covered in previous 20 harnesses.
 * Focuses on testing complex table compilation scenarios, character attribute flags,
 * user-defined attributes, and advanced pattern matching with custom classes.
 * 
 * APIs specifically targeted in this harness:
 * 1) lou_compileString: Advanced table compilation with complex rules and attributes
 * 2) Pattern matching with user-defined character attributes (CTC_UserDefined1-CTC_UserDefined45)
 * 3) Character class testing (CTC_Space, CTC_Letter, CTC_Digit, CTC_Punctuation, etc.)
 * 4) Complex rule sequences with multiple passes and conditional logic
 * 5) Table compilation with emphasis classes and custom typeforms
 * 6) Testing of internal character attribute flags from internal.h
 * 
 * Semantic differentiation from previous harnesses:
 * - harness_000-010: Basic translation, metadata, error handling, specific features
 * - harness_011-012: Integrated workflows and stress testing
 * - harness_013-014: Unicode handling and complex integration
 * - harness_015: Windows-specific and internal state testing
 * - harness_016-017: File I/O operations and RTL language functionality
 * - harness_018: Right-to-left languages and RTL cursor positioning
 * - harness_019: Mathematical braille translation and complex formula parsing
 * - This harness (020): FOCUSES EXCLUSIVELY on advanced table compilation,
 *                       character attribute testing, and complex pattern matching
 *                       with user-defined attributes and character classes
 * 
 * Testing rationale:
 * - Table compilation involves complex parsing of rule syntax and character attributes
 * - Character attribute flags control pattern matching behavior in translation tables
 * - User-defined attributes allow custom character classification for specialized rules
 * - Complex rule sequences test the pattern matching engine's ability to handle
 *   conditional logic, groups, and multi-pass transformations
 * - Edge cases with attribute combinations and rule interactions can reveal bugs
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

// Character attribute names for testing (from internal.h)
static const char* character_attributes[] = {
    "space",
    "letter",
    "digit",
    "punctuation",
    "uppercase",
    "lowercase",
    "math",
    "sign",
    "litdigit",
    "capsmode",
    "numericmode",
    "numericnocontract",
    "seqdelimiter",
    "seqbefore",
    "seqafter",
    "userdefined1",
    "userdefined2",
    "userdefined3",
    "userdefined4",
    "userdefined5",
    "userdefined6",
    "userdefined7",
    "userdefined8",
    "endofinput",
    "empmatch",
    "midendnumericmode",
    "userdefined9",
    "userdefined10",
    "userdefined11",
    "userdefined12"
};

static const size_t num_attributes = sizeof(character_attributes) / sizeof(character_attributes[0]);

// Generate complex table compilation string with character attributes
static std::string generate_table_with_attributes(FuzzedDataProvider& fdp) {
    std::string table;
    
    // Start with table header
    table += "locale en_US\n";
    table += "display-name \"Fuzzed Table with Attributes\"\n";
    table += "author \"Fuzzer\"\n";
    table += "version \"1.0\"\n\n";
    
    // Add character attribute definitions
    int num_attrs_to_add = fdp.ConsumeIntegralInRange<int>(1, 10);
    for (int i = 0; i < num_attrs_to_add && fdp.remaining_bytes() > 0; i++) {
        uint8_t attr_idx = fdp.ConsumeIntegral<uint8_t>() % num_attributes;
        std::string attr_name = character_attributes[attr_idx];
        
        // Generate character ranges for this attribute
        int num_ranges = fdp.ConsumeIntegralInRange<int>(1, 5);
        table += "attribute " + attr_name;
        
        for (int r = 0; r < num_ranges && fdp.remaining_bytes() > 2; r++) {
            char start_char = fdp.ConsumeIntegralInRange<char>('a', 'z');
            char end_char = fdp.ConsumeIntegralInRange<char>(start_char, 'z');
            table += " " + std::string(1, start_char) + "-" + std::string(1, end_char);
        }
        table += "\n";
    }
    
    // Add emphasis class definitions
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 0) {
        table += "emphclass italic 1\n";
        table += "emphclass bold 2\n";
        table += "emphclass underline 4\n";
        table += "emphclass superscript 8\n";
        table += "emphclass subscript 16\n";
    }
    
    // Add rules using character attributes
    int num_rules = fdp.ConsumeIntegralInRange<int>(1, 20);
    for (int i = 0; i < num_rules && fdp.remaining_bytes() > 10; i++) {
        uint8_t rule_type = fdp.ConsumeIntegral<uint8_t>() % 4;
        
        switch (rule_type) {
            case 0: // Character rule using attributes
                {
                    uint8_t attr_idx = fdp.ConsumeIntegral<uint8_t>() % num_attributes;
                    std::string attr_name = character_attributes[attr_idx];
                    char from_char = fdp.ConsumeIntegralInRange<char>('a', 'z');
                    std::string dots = fdp.ConsumeIntegralInRange<int>(1, 8) == 1 ? "1" : "123";
                    
                    table += "chars " + std::string(1, from_char) + " " + dots + "\n";
                    // Add attribute-based rule
                    table += "always $" + attr_name + " " + std::string(1, from_char) + " " + dots + "\n";
                }
                break;
                
            case 1: // Pattern rule with groups
                {
                    std::string pattern = fdp.ConsumeRandomLengthString(20);
                    std::string replacement = fdp.ConsumeRandomLengthString(20);
                    if (!pattern.empty() && !replacement.empty()) {
                        table += "always \"" + pattern + "\" \"" + replacement + "\"\n";
                    }
                }
                break;
                
            case 2: // Rule with conditions
                {
                    char test_char = fdp.ConsumeIntegralInRange<char>('a', 'z');
                    char result_char = fdp.ConsumeIntegralInRange<char>('a', 'z');
                    std::string dots = fdp.ConsumeIntegralInRange<int>(1, 8) == 1 ? "1" : "123";
                    
                    table += "always " + std::string(1, test_char) + " " + dots + "\n";
                    table += "if $" + std::string("letter") + " " + std::string(1, test_char) + "\n";
                    table += "then " + std::string(1, result_char) + " " + dots + "\n";
                    table += "end\n";
                }
                break;
                
            case 3: // Multi-pass rule
                {
                    char from_char = fdp.ConsumeIntegralInRange<char>('a', 'z');
                    char to_char = fdp.ConsumeIntegralInRange<char>('a', 'z');
                    std::string dots1 = "1";
                    std::string dots2 = "123";
                    
                    table += "pass0 always " + std::string(1, from_char) + " " + dots1 + "\n";
                    table += "pass1 always " + std::string(1, to_char) + " " + dots2 + "\n";
                }
                break;
        }
    }
    
    // Add include statements for real tables to test complex dependencies
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 0) {
        table += "include en-us-g1.ctb\n";
        table += "include en-us-comp8.ctb\n";
    }
    
    // Add punctuation and digit rules
    table += "punctuation . 1\n";
    table += "punctuation , 12\n";
    table += "punctuation ? 26\n";
    table += "punctuation ! 235\n";
    
    table += "digit 0 3456\n";
    table += "digit 1 1\n";
    table += "digit 2 12\n";
    table += "digit 3 14\n";
    table += "digit 4 145\n";
    table += "digit 5 15\n";
    table += "digit 6 124\n";
    table += "digit 7 1245\n";
    table += "digit 8 125\n";
    table += "digit 9 24\n";
    
    return table;
}

// Generate test strings that exercise character attributes
static std::string generate_attribute_test_string(FuzzedDataProvider& fdp) {
    std::string result;
    int length = fdp.ConsumeIntegralInRange<int>(10, 200);
    
    for (int i = 0; i < length && fdp.remaining_bytes() > 0; i++) {
        uint8_t char_type = fdp.ConsumeIntegral<uint8_t>() % 8;
        
        switch (char_type) {
            case 0: // Letters (should match CTC_Letter)
                result.push_back(fdp.ConsumeIntegralInRange<char>('a', 'z'));
                break;
            case 1: // Uppercase letters (should match CTC_UpperCase)
                result.push_back(fdp.ConsumeIntegralInRange<char>('A', 'Z'));
                break;
            case 2: // Digits (should match CTC_Digit)
                result.push_back(fdp.ConsumeIntegralInRange<char>('0', '9'));
                break;
            case 3: // Spaces (should match CTC_Space)
                result.push_back(' ');
                break;
            case 4: // Punctuation (should match CTC_Punctuation)
                {
                    const char punct[] = {'.', ',', ';', ':', '!', '?', '"', '\'', '(', ')', '[', ']', '{', '}'};
                    uint8_t punct_idx = fdp.ConsumeIntegral<uint8_t>() % (sizeof(punct)/sizeof(punct[0]));
                    result.push_back(punct[punct_idx]);
                }
                break;
            case 5: // Math symbols (should match CTC_Math)
                {
                    const char math[] = {'+', '-', '*', '/', '=', '<', '>', '^'};
                    uint8_t math_idx = fdp.ConsumeIntegral<uint8_t>() % (sizeof(math)/sizeof(math[0]));
                    result.push_back(math[math_idx]);
                }
                break;
            case 6: // Signs (should match CTC_Sign)
                {
                    const char signs[] = {'@', '#', '$', '%', '&', '|', '~', '`'};
                    uint8_t sign_idx = fdp.ConsumeIntegral<uint8_t>() % (sizeof(signs)/sizeof(signs[0]));
                    result.push_back(signs[sign_idx]);
                }
                break;
            case 7: // Mixed characters for complex testing
                result.push_back(fdp.ConsumeIntegral<uint8_t>());
                break;
        }
    }
    
    return result;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    static int initialized = 0;
    
    // Initialize liblouis once
    if (!initialized) {
        lou_registerLogCallback(fuzz_log_callback);
        lou_setLogLevel(LOU_LOG_WARN); // Show warnings for compilation errors
        initialized = 1;
    }
    
    // Need minimum input for meaningful testing
    if (size < 100) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // 1. Generate complex table with character attributes
    std::string table_content = generate_table_with_attributes(fdp);
    
    // 2. Compile the table string - use a valid table name instead of NULL
    const char* table_string = table_content.c_str();
    int compile_result = lou_compileString("fuzzed_table.ctb", table_content.c_str());
    if (compile_result == 0) {
        // Table compilation failed - this is okay, we're testing error paths too
        // Clean up any allocated memory before returning
        lou_free();
        return 0;
    }

    
    // 3. Create a temporary table file path
    static int file_counter = 0;
    int counter = __sync_fetch_and_add(&file_counter, 1);
    char table_filename[256];
    snprintf(table_filename, sizeof(table_filename), "/tmp/liblouis_compile_%d.ctb", counter);
    
    // 4. Write the compiled table to a file (simulate saving compiled table)
    FILE* table_file = fopen(table_filename, "wb");
    if (!table_file) {
        lou_free();
        return 0;
    }
    fwrite(table_string, 1, table_content.length(), table_file);
    fclose(table_file);
    
    // 5. Check if the table is valid
    if (lou_checkTable(table_filename) == 0) {
        // Table invalid, cleanup and exit
        lou_free();
        unlink(table_filename);
        return 0;
    }
    
    // 6. Generate test string that exercises character attributes
    std::string test_string = generate_attribute_test_string(fdp);
    if (test_string.empty() || fdp.remaining_bytes() < 8) {
        lou_free();
        unlink(table_filename);
        return 0;
    }
    
    // 7. Convert test string to widechar
    widechar input_buf[1024];
    int input_len = simple_string_to_widechar(test_string.c_str(), test_string.size(), input_buf);
    if (input_len <= 0) {
        lou_free();
        unlink(table_filename);
        return 0;
    }
    
    // 8. Test translation with the compiled table
    // Test forward translation
    int out_len_fwd = input_len * 16;
    widechar output_fwd[1024];
    int in_len_copy_fwd = input_len;
    
    int translate_result = lou_translateString(table_filename, input_buf, &in_len_copy_fwd,
                                               output_fwd, &out_len_fwd, NULL, NULL, 0);
    
    // Test back translation
    int out_len_rev = input_len * 16;
    widechar output_rev[1024];
    int in_len_copy_rev = input_len;
    
    lou_backTranslateString(table_filename, input_buf, &in_len_copy_rev,
                           output_rev, &out_len_rev, NULL, NULL, 0);
    
    // 9. Test with typeforms to exercise emphasis classes
    if (input_len > 0 && fdp.remaining_bytes() > sizeof(formtype)) {
        formtype* typeform_array = NULL;
        char* spacing_array = NULL;
        
        typeform_array = (formtype*)malloc(input_len * sizeof(formtype));
        spacing_array = (char*)malloc(input_len * sizeof(char));
        
        if (typeform_array && spacing_array) {
            // Initialize typeform array with various emphasis types
            for (int i = 0; i < input_len; i++) {
                uint8_t typeform_type = fdp.ConsumeIntegral<uint8_t>() % 8;
                switch (typeform_type) {
                    case 0: typeform_array[i] = plain_text; break;
                    case 1: typeform_array[i] = emph_1; break; // italic
                    case 2: typeform_array[i] = emph_2; break; // underline
                    case 3: typeform_array[i] = emph_3; break; // bold
                    case 4: typeform_array[i] = emph_4; break;
                    case 5: typeform_array[i] = emph_5; break;
                    case 6: typeform_array[i] = emph_6; break;
                    case 7: typeform_array[i] = emph_7; break;
                }
                spacing_array[i] = fdp.ConsumeBool() ? 1 : 0;
            }
            
            int out_len_type = input_len * 16;
            widechar output_type[1024];
            int in_len_copy_type = input_len;
            
            lou_translateString(table_filename, input_buf, &in_len_copy_type,
                               output_type, &out_len_type, typeform_array, spacing_array, 0);
        }
        
        free(typeform_array);
        free(spacing_array);
    }
    
    // 10. Test dot pattern conversion with the compiled table
    if (input_len > 0 && fdp.remaining_bytes() > 0) {
        widechar dots_buf[1024];
        widechar chars_buf[1024];
        
        // Test charToDots
        lou_charToDots(table_filename, input_buf, dots_buf, input_len, 0);
        
        // Test dotsToChar
        lou_dotsToChar(table_filename, dots_buf, chars_buf, input_len, 0);
    }
    
    // 11. Test hyphenation with complex patterns
    if (input_len > 0) {
        char hyphens[1024];
        lou_hyphenate(table_filename, input_buf, input_len, hyphens, 0);
    }
    
    // 12. Cleanup
    lou_free();
    unlink(table_filename);
    
    return 0;
}

} // namespace harness_020
