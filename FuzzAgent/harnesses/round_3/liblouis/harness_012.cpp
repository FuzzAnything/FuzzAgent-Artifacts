#include <fuzzer/FuzzedDataProvider.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <liblouis/liblouis.h>
#include <assert.h>
#include <vector>
#include <string>

namespace harness_012 {
// liblouis fuzzing harness - Deep coverage of lou_hyphenate API
// Targets: lou_hyphenate (primary - 1.92% coverage, 54/60 branches undiscovered)
//          lou_getTable (table loading), lou_free (cleanup), 
//          lou_setDataPath (optional), lou_setLogLevel (optional)
// Focus: Generate valid hyphenation patterns and test boundary conditions for hyphenation rules
// Based on coverage guidance: /root/FuzzAgent/output/liblouis/coverage_guidance/harness_012_guidance.md
// Uses FuzzedDataProvider for structured input processing


// Log callback to suppress output during fuzzing
void avoid_log(logLevels level, const char *msg) {
    (void)level;
    (void)msg;
}

// Cleanup function
static void free_resources(void) {
    lou_free();
}

// Convert std::string to widechar array for liblouis input
static std::vector<widechar> string_to_widechar(const std::string& str) {
    std::vector<widechar> wide_str;
    wide_str.reserve(str.length());
    
    for (size_t i = 0; i < str.length(); i++) {
        // Simple conversion - liblouis handles various character encodings
        wide_str.push_back(static_cast<widechar>(str[i]));
    }
    
    return wide_str;
}

// Generate test string with various characteristics for hyphenation testing
static std::string generate_test_string(FuzzedDataProvider& fdp) {
    uint8_t string_type = fdp.ConsumeIntegral<uint8_t>() % 6;
    size_t max_length = fdp.ConsumeIntegralInRange<size_t>(1, 50); // HYPHSTRING limit is 100
    
    switch (string_type) {
        case 0:
            // Simple word with letters only
            return fdp.ConsumeRandomLengthString(max_length);
            
        case 1:
            // Word with mixed case
            {
                std::string word = fdp.ConsumeRandomLengthString(max_length);
                // Randomly capitalize some letters
                for (size_t i = 0; i < word.length(); i++) {
                    if (fdp.ConsumeBool() && std::isalpha(word[i])) {
                        word[i] = std::toupper(word[i]);
                    }
                }
                return word;
            }
            
        case 2:
            // Multiple words separated by spaces
            {
                int word_count = fdp.ConsumeIntegralInRange<int>(1, 5);
                std::string result;
                for (int i = 0; i < word_count; i++) {
                    if (!result.empty()) result += ' ';
                    result += fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(1, 10));
                }
                return result;
            }
            
        case 3:
            // Word with punctuation
            {
                std::string word = fdp.ConsumeRandomLengthString(max_length);
                if (word.length() > 2) {
                    // Add punctuation at beginning, middle, or end
                    size_t pos = fdp.ConsumeIntegralInRange<size_t>(0, word.length() - 1);
                    word.insert(pos, 1, fdp.PickValueInArray({'-', '\'', '.', ','}));
                }
                return word;
            }
            
        case 4:
            // Word with numbers
            {
                std::string word;
                for (size_t i = 0; i < max_length; i++) {
                    if (fdp.ConsumeBool()) {
                        word += static_cast<char>(fdp.ConsumeIntegralInRange<int>('0', '9'));
                    } else {
                        word += static_cast<char>(fdp.ConsumeIntegralInRange<int>('a', 'z'));
                    }
                }
                return word;
            }
            
        case 5:
            // Empty or single character
            if (fdp.ConsumeBool()) {
                return std::string(1, static_cast<char>(fdp.ConsumeIntegralInRange<int>('a', 'z')));
            } else {
                return "";
            }
            
        default:
            return fdp.ConsumeRandomLengthString(max_length);
    }
}

// Select a table that likely supports hyphenation
static std::string select_hyphenation_table(FuzzedDataProvider& fdp) {
    // Try tables that are more likely to have hyphenation support
    // Based on common liblouis table naming patterns
    const char* potential_tables[] = {
        "en-us-g2.ctb",      // English US grade 2 - common and likely to have hyphenation
        "en-us-g1.ctb",      // English US grade 1
        "en-ueb-g2.ctb",     // English UEB grade 2
        "afr-za-g2.ctb",     // Afrikaans grade 2 - might have hyphenation
        "de-de-g2.ctb",      // German grade 2
        "fr-fr-g2.ctb",      // French grade 2
        "es-es-g2.ctb",      // Spanish grade 2
        "pl-pl-g1.utb",      // Polish grade 1 (actually .utb in the tables dir)
        "lv-lv-g1.utb",      // Latvian grade 1
    };
    
    size_t table_index = fdp.ConsumeIntegralInRange<size_t>(0, sizeof(potential_tables)/sizeof(potential_tables[0]) - 1);
    return std::string(potential_tables[table_index]);
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Minimum size check - need enough data for meaningful testing
    // Need data for: operation type, table selection, test string generation
    if (size < 32) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize liblouis once per process
    static int initialized = 0;
    if (!initialized) {
        // Set up logging to avoid console output during fuzzing
        lou_registerLogCallback(avoid_log);
        lou_setLogLevel(LOU_LOG_WARN); // Only show warnings and errors
        
        // Optional: Set data path if needed
        // lou_setDataPath("/usr/share/liblouis/tables");
        
        initialized = 1;
        // Register cleanup
        atexit(free_resources);
    }
    
    // Consume operation type to decide testing strategy
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    // Select a table for hyphenation
    std::string table_name = select_hyphenation_table(fdp);
    
    // Load the table to check if it supports hyphenation
    const void* table = lou_getTable(table_name.c_str());
    if (table == NULL) {
        // Table failed to load, try a fallback
        table_name = "en-us-g1.ctb";
        table = lou_getTable(table_name.c_str());
        if (table == NULL) {
            // Still can't load table, skip this iteration
            return 0;
        }
    }
    
    // Generate test string for hyphenation
    std::string test_string = generate_test_string(fdp);
    if (test_string.empty()) {
        return 0;
    }
    
    // Convert to widechar format for liblouis
    std::vector<widechar> wide_input = string_to_widechar(test_string);
    if (wide_input.empty()) {
        return 0;
    }
    
    // Allocate hyphens output buffer (must be at least inlen + 1)
    std::vector<char> hyphens(wide_input.size() + 1, 0);
    
    int result = 0;
    
    switch (operation) {
        case 0:
            // Test lou_hyphenate with mode 0 (text input)
            result = lou_hyphenate(table_name.c_str(), wide_input.data(), 
                                  static_cast<int>(wide_input.size()),
                                  hyphens.data(), 0);
            // Check hyphens array for valid values (0, 1, or 2)
            for (size_t i = 0; i < wide_input.size(); i++) {
                char h = hyphens[i];
                if (h != '0' && h != '1' && h != '2') {
                    // Invalid hyphen value - but we'll continue fuzzing
                    break;
                }
            }
            break;
            
        case 1:
            // Test lou_hyphenate with mode 1 (braille input)
            // First need to translate to braille, then hyphenate
            {
                int inlen = static_cast<int>(wide_input.size());
                int outlen = inlen * 4; // Conservative buffer size for braille output
                std::vector<widechar> braille_out(outlen + 1, 0);
                std::vector<char> spacing(outlen + 1, 0);
                
                // Translate to braille
                int translate_result = lou_translateString(table_name.c_str(), 
                                                          wide_input.data(), &inlen,
                                                          braille_out.data(), &outlen,
                                                          NULL, spacing.data(), ucBrl);
                
                if (translate_result != 0 && outlen > 0 && outlen < 100) {
                    // Now hyphenate the braille output
                    std::vector<char> braille_hyphens(outlen + 1, 0);
                    result = lou_hyphenate(table_name.c_str(), braille_out.data(),
                                          outlen, braille_hyphens.data(), 1);
                }
            }
            break;
            
        case 2:
            // Test with very short input (boundary condition)
            if (wide_input.size() > 1) {
                // Use just first character
                widechar single_char[] = {wide_input[0]};
                char single_hyphen[2] = {0};
                result = lou_hyphenate(table_name.c_str(), single_char, 1,
                                      single_hyphen, 0);
            }
            break;
            
        case 3:
            // Test with input containing hyphen characters
            {
                // Create a string that includes hyphens
                std::string hyphen_string;
                for (size_t i = 0; i < std::min(wide_input.size(), static_cast<size_t>(20)); i++) {
                    hyphen_string += static_cast<char>(wide_input[i]);
                    if (i % 3 == 0 && i > 0) {
                        hyphen_string += '-';
                    }
                }
                
                std::vector<widechar> hyphen_wide = string_to_widechar(hyphen_string);
                if (!hyphen_wide.empty()) {
                    std::vector<char> hyphen_hyphens(hyphen_wide.size() + 1, 0);
                    result = lou_hyphenate(table_name.c_str(), hyphen_wide.data(),
                                          static_cast<int>(hyphen_wide.size()),
                                          hyphen_hyphens.data(), 0);
                }
            }
            break;
    }
    
    // Note: lou_free() is called via atexit handler
    
    return 0;
}

} // namespace harness_012
