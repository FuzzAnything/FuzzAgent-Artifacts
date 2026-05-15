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

namespace harness_017 {
// liblouis fuzzing harness - Pattern Compilation Subsystem
// Targets: pattern_compile_expression function with 115 blocked branches
// Focus: Complex pattern expressions, different from existing harness targets
// Functions: _lou_pattern_compile, _lou_pattern_check, _lou_pattern_reverse
// Project: $SRC, Headers: appropriate headers for pattern compilation functions


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

// External declaration for pattern compilation functions (internal API)
// Note: These functions use internal types not exposed in public header
// We'll declare them with void pointers to avoid type issues
extern "C" {
    // Pattern compilation functions - using void* for internal types
    extern int _lou_pattern_compile(const widechar *input, const int input_max, 
                                   widechar *expr_data, const int expr_max,
                                   void *table, const void *nested);
    
    extern void _lou_pattern_reverse(widechar *expr_data);
    
    extern int _lou_pattern_check(const widechar *input, const int input_start,
                                 const int input_minmax, const int input_dir,
                                 const widechar *expr_data, const void *table);
    
    // Helper functions for table operations
    extern const void *_lou_getTranslationTable(const char *tableList);
    extern int _lou_compileTranslationRule(const char *tableList, const char *inString);
}

// Helper function to generate random pattern characters based on pattern syntax
static char get_pattern_char(FuzzedDataProvider& fdp) {
    // Pattern syntax characters based on pattern_compile_expression switch cases
    char pattern_chars[] = {
        '(', ')', '!', '+', '*', '?', '|', '.', '%', '[', ']', '@',
        'l', 'd', 'p', 'u', 'L', 's', '<', '>', '^',
        '0', '1', '2', '3', '4', '5', '6', '7',
        'a', 'b', 'c', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'm', 'n',
        'o', 'q', 'r', 't', 'v', 'w', 'x', 'y', 'z',
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'M',
        'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
        '\\', '-', '_', '=', '+', '{', '}', ':', ';', ',', '.', '/', '?',
        '~', '`', '!', '@', '#', '$', '%', '&', '*', '(', ')', '-', '+'
    };
    
    size_t index = fdp.ConsumeIntegralInRange<size_t>(0, sizeof(pattern_chars) - 1);
    return pattern_chars[index];
}

// Convert string to widechar array
static std::vector<widechar> string_to_widechars(const std::string& str) {
    std::vector<widechar> widechars;
    widechars.reserve(str.length());
    for (char c : str) {
        widechars.push_back(static_cast<widechar>(c));
    }
    return widechars;
}

// Generate a simple pattern for testing
static std::string generate_simple_pattern(FuzzedDataProvider& fdp) {
    std::string pattern;
    int length = fdp.ConsumeIntegralInRange<int>(1, 100);
    
    for (int i = 0; i < length; i++) {
        // Simple pattern generation without complex nesting to avoid syntax issues
        uint8_t choice = fdp.ConsumeIntegral<uint8_t>() % 10;
        switch (choice) {
            case 0:
                pattern += '(';
                pattern += get_pattern_char(fdp);
                pattern += ')';
                break;
            case 1:
                pattern += '!';
                pattern += get_pattern_char(fdp);
                break;
            case 2:
                pattern += get_pattern_char(fdp);
                pattern += '+';
                break;
            case 3:
                pattern += get_pattern_char(fdp);
                pattern += '*';
                break;
            case 4:
                pattern += get_pattern_char(fdp);
                pattern += '?';
                break;
            case 5:
                pattern += get_pattern_char(fdp);
                pattern += '|';
                pattern += get_pattern_char(fdp);
                break;
            case 6:
                pattern += '.';
                break;
            case 7: {
                pattern += '%';
                // Add simple attribute
                char attrs[] = {'l', 'd', 'p', 'u', 'L', 's', '<', '>', '^', '0'};
                size_t attr_idx = fdp.ConsumeIntegralInRange<size_t>(0, sizeof(attrs) - 1);
                pattern += attrs[attr_idx];
                break;
            }
            case 8:
                pattern += '[';
                pattern += get_pattern_char(fdp);
                pattern += get_pattern_char(fdp);
                pattern += ']';
                break;
            case 9:
                pattern += '@';
                pattern += get_pattern_char(fdp);
                break;
            default:
                pattern += get_pattern_char(fdp);
                break;
        }
    }
    
    return pattern;
}

// Main fuzzer entry point
int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Minimum size check - need enough data for meaningful pattern testing
    if (size < 32) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize liblouis once
    if (!initialized) {
        lou_registerLogCallback(avoid_log);
        initialized = 1;
        // Register cleanup
        atexit(free_resources);
    }
    
    // Consume operation type to decide which pattern API to test
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    // Generate pattern string
    std::string pattern_str = generate_simple_pattern(fdp);
    std::vector<widechar> pattern_wchars = string_to_widechars(pattern_str);
    
    // Allocate expression buffer
    const int expr_max = 4096;
    std::vector<widechar> expr_data(expr_max, 0);
    
    // Get a translation table for testing
    // Use a simple built-in table
    const char* test_table = "en-us-g2.ctb";
    
    // Try to get a translation table
    const void* table = _lou_getTranslationTable(test_table);
    if (!table) {
        // If table not found, try to compile a simple rule to create one
        const char* simple_rule = "include en-us-g2.ctb";
        _lou_compileTranslationRule(test_table, simple_rule);
        table = _lou_getTranslationTable(test_table);
    }
    
    if (!table) {
        // Still no table, can't test pattern compilation
        return 0;
    }
    
    int result = 0;
    
    switch (operation) {
        case 0: {
            // Test _lou_pattern_compile with generated pattern
            result = _lou_pattern_compile(
                pattern_wchars.data(),
                pattern_wchars.size(),
                expr_data.data(),
                expr_max,
                const_cast<void*>(table),
                NULL  // nested FileInfo
            );
            
            if (result > 0) {
                // If compilation succeeded, test pattern reverse
                _lou_pattern_reverse(expr_data.data());
            }
            break;
        }
        
        case 1: {
            // Test pattern compilation with multiple patterns
            int num_patterns = fdp.ConsumeIntegralInRange<int>(1, 3);
            
            for (int i = 0; i < num_patterns && fdp.remaining_bytes() > 10; i++) {
                std::string pat = generate_simple_pattern(fdp);
                std::vector<widechar> pat_wchars = string_to_widechars(pat);
                _lou_pattern_compile(
                    pat_wchars.data(),
                    pat_wchars.size(),
                    expr_data.data(),
                    expr_max,
                    const_cast<void*>(table),
                    NULL
                );
            }
            break;
        }
        
        case 2: {
            // Test pattern check with various inputs
            // Generate test input
            std::string test_input_str = generate_simple_pattern(fdp);
            std::vector<widechar> test_input = string_to_widechars(test_input_str);
            
            // Compile a pattern first
            result = _lou_pattern_compile(
                pattern_wchars.data(),
                pattern_wchars.size(),
                expr_data.data(),
                expr_max,
                const_cast<void*>(table),
                NULL
            );
            
            if (result > 0) {
                // Test pattern check with random parameters
                if (test_input.size() > 0) {
                    int input_start = fdp.ConsumeIntegralInRange<int>(0, test_input.size() - 1);
                    int input_minmax = fdp.ConsumeIntegralInRange<int>(input_start, test_input.size());
                    int input_dir = fdp.ConsumeIntegralInRange<int>(-1, 1);
                    
                    _lou_pattern_check(
                        test_input.data(),
                        input_start,
                        input_minmax,
                        input_dir,
                        expr_data.data(),
                        table
                    );
                }
            }
            break;
        }
        
        case 3: {
            // Test edge cases
            // Test with empty pattern
            std::vector<widechar> empty_pattern;
            _lou_pattern_compile(
                empty_pattern.data(),
                0,
                expr_data.data(),
                expr_max,
                const_cast<void*>(table),
                NULL
            );
            
            // Test pattern reverse
            _lou_pattern_reverse(expr_data.data());
            
            break;
        }
    }
    
    return 0;
}

} // namespace harness_017
