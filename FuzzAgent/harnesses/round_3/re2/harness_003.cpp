#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <string>
#include <vector>
#include "re2/re2.h"
#include "re2/set.h"

namespace harness_003 {
// Comprehensive fuzzing harness for RE2::Set interface
// Targets RE2::Set with full error reporting and both Match variants
// Focuses on aspects not covered by harness_001: error reporting in Add(),
// both Match variants (with/without ErrorInfo), edge cases in Compile()



int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough for basic operations
    if (size < 16) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Create RE2::Options with diverse settings
    RE2::Options options;
    
    // Consume encoding setting
    uint8_t encoding_choice = fdp.ConsumeIntegral<uint8_t>() % 3;
    switch (encoding_choice) {
        case 0:
            options.set_encoding(RE2::Options::EncodingUTF8);
            break;
        case 1:
            options.set_encoding(RE2::Options::EncodingLatin1);
            break;
        case 2:
            // Leave as default (UTF8)
            break;
    }
    
    // Consume various boolean options
    options.set_log_errors(fdp.ConsumeBool());
    options.set_posix_syntax(fdp.ConsumeBool());
    options.set_longest_match(fdp.ConsumeBool());
    options.set_literal(fdp.ConsumeBool());
    options.set_never_nl(fdp.ConsumeBool());
    options.set_dot_nl(fdp.ConsumeBool());
    options.set_never_capture(fdp.ConsumeBool());
    options.set_case_sensitive(fdp.ConsumeBool());
    options.set_perl_classes(fdp.ConsumeBool());
    options.set_word_boundary(fdp.ConsumeBool());
    options.set_one_line(fdp.ConsumeBool());
    
    // Consume max_mem setting
    int64_t max_mem = fdp.ConsumeIntegralInRange<int64_t>(0, 1024 * 1024); // 0-1MB
    options.set_max_mem(max_mem);

    // Consume anchor setting
    RE2::Anchor anchor = fdp.PickValueInArray<RE2::Anchor>({
        RE2::UNANCHORED,
        RE2::ANCHOR_START,
        RE2::ANCHOR_BOTH,
    });

    // Create RE2::Set instance
    RE2::Set set(options, anchor);

    // Add patterns with error reporting (not nullptr like in harness_001)
    int num_patterns = fdp.ConsumeIntegralInRange<int>(0, 10);
    std::vector<std::string> pattern_errors;
    
    for (int i = 0; i < num_patterns && fdp.remaining_bytes() > 5; i++) {
        // Determine pattern type/complexity
        uint8_t pattern_type = fdp.ConsumeIntegral<uint8_t>() % 5;
        std::string pattern;
        
        switch (pattern_type) {
            case 0:
                // Simple literal pattern
                pattern = fdp.ConsumeRandomLengthString(50);
                break;
            case 1:
                // Pattern with character classes
                pattern = "[" + fdp.ConsumeRandomLengthString(20) + "]";
                break;
            case 2:
                // Pattern with repetition
                pattern = fdp.ConsumeRandomLengthString(20) + "+";
                break;
            case 3:
                // Pattern with alternation
                pattern = fdp.ConsumeRandomLengthString(15) + "|" + 
                          fdp.ConsumeRandomLengthString(15);
                break;
            case 4:
                // Pattern with grouping
                pattern = "(" + fdp.ConsumeRandomLengthString(20) + ")";
                break;
        }
        
        // Add pattern with error reporting
        std::string error;
        int index = set.Add(pattern, &error);
        
        // Store error if any
        if (!error.empty()) {
            pattern_errors.push_back(error);
        }
    }

    // Check Size() method
    int set_size = set.Size();
    
    // Test Compile() with various scenarios
    bool compile_success = false;
    
    if (set_size > 0) {
        // Test Compile() - this can fail on memory exhaustion or invalid patterns
        compile_success = set.Compile();
        
        if (compile_success) {
            // Test both Match variants
            
            // Consume test text
            std::string test_text = fdp.ConsumeRandomLengthString(200);
            
            // Variant 1: Match without ErrorInfo (basic variant)
            std::vector<int> matches_basic;
            bool match_basic_result = set.Match(test_text, &matches_basic);
            
            // Variant 2: Match with ErrorInfo (advanced variant - not tested in harness_001)
            std::vector<int> matches_with_error;
            RE2::Set::ErrorInfo error_info;
            bool match_with_error_result = set.Match(test_text, &matches_with_error, &error_info);
            
            // Both variants should return the same result when compilation succeeded
            // (though this is implementation detail we're testing)
            
            // Test with empty vector pointer (nullptr behavior)
            bool match_no_vector = set.Match(test_text, nullptr);
            
            // Test with different text variations
            if (fdp.remaining_bytes() > 50) {
                // Test with empty string
                std::vector<int> empty_matches;
                set.Match("", &empty_matches);
                
                // Test with very short string
                set.Match("a", &empty_matches);
                
                // Test with string containing special regex characters
                set.Match(".*+?[]{}()|^$\\", &empty_matches);
            }
            
            // Test edge cases: match after compilation with no patterns added
            // (handled by set_size > 0 check above)
        }
        
        // Test re-compilation attempt (should fail or have no effect)
        if (fdp.ConsumeBool()) {
            // Try to compile again - behavior depends on implementation
            bool recompile_attempt = set.Compile();
        }
    } else {
        // Empty set tests
        // Compile should succeed on empty set
        compile_success = set.Compile();
        
        if (compile_success) {
            std::string test_text = fdp.ConsumeRandomLengthString(100);
            std::vector<int> empty_matches;
            bool empty_match = set.Match(test_text, &empty_matches);
            
            // With ErrorInfo
            RE2::Set::ErrorInfo empty_error_info;
            bool empty_match_with_error = set.Match(test_text, &empty_matches, &empty_error_info);
        }
    }

    // Test with insufficient memory scenario (via options)
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 100) {
        // Create a new set with very low memory limit
        RE2::Options low_mem_options = options;
        low_mem_options.set_max_mem(1); // Extremely low memory
        
        RE2::Set low_mem_set(low_mem_options, anchor);
        
        // Add a few simple patterns
        int low_mem_patterns = fdp.ConsumeIntegralInRange<int>(1, 3);
        for (int i = 0; i < low_mem_patterns && fdp.remaining_bytes() > 10; i++) {
            std::string simple_pattern = fdp.ConsumeRandomLengthString(20);
            std::string error;
            low_mem_set.Add(simple_pattern, &error);
        }
        
        // Try to compile - may fail due to memory constraints
        bool low_mem_compile = low_mem_set.Compile();
        
        if (low_mem_compile) {
            std::string test_text = fdp.ConsumeRandomLengthString(50);
            std::vector<int> low_mem_matches;
            low_mem_set.Match(test_text, &low_mem_matches);
        }
    }

    // Test pattern error accumulation with fuzzer-generated invalid patterns
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 50) {
        // Create a set and add patterns that may be invalid
        RE2::Set error_set(options, anchor);
        
        // Add some potentially invalid patterns generated from fuzzer input
        int invalid_pattern_count = fdp.ConsumeIntegralInRange<int>(1, 5);
        for (int i = 0; i < invalid_pattern_count && fdp.remaining_bytes() > 10; i++) {
            // Generate pattern with various structures that could be invalid
            uint8_t pattern_structure = fdp.ConsumeIntegral<uint8_t>() % 6;
            std::string pattern;
            
            switch (pattern_structure) {
                case 0:
                    // Unclosed character class
                    pattern = "[" + fdp.ConsumeRandomLengthString(10);
                    break;
                case 1:
                    // Invalid group syntax
                    pattern = "(?" + fdp.ConsumeRandomLengthString(5);
                    break;
                case 2:
                    // Quantifier without preceding expression
                    pattern = "*" + fdp.ConsumeRandomLengthString(5);
                    break;
                case 3:
                    // Invalid range (min > max)
                    pattern = fdp.ConsumeRandomLengthString(3) + 
                             "{100," + std::to_string(fdp.ConsumeIntegralInRange<int>(1, 99)) + "}";
                    break;
                case 4:
                    // Trailing backslash
                    pattern = fdp.ConsumeRandomLengthString(5) + "\\";
                    break;
                case 5:
                    // Complex potentially invalid pattern
                    pattern = fdp.ConsumeRandomLengthString(10) + 
                             "[" + fdp.ConsumeRandomLengthString(5) + 
                             "|" + fdp.ConsumeRandomLengthString(5) + 
                             "+?";
                    break;
            }
            
            std::string error;
            error_set.Add(pattern, &error);
            // Error may or may not be set depending on pattern validity
        }
        
        // Try to compile even with potential errors
        error_set.Compile();
    }

    // Test move operations (if supported by implementation)
    // Note: Move constructor/assignment are declared in set.h
    
    // Test with different anchor settings
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 100) {
        RE2::Anchor alternate_anchor = (anchor == RE2::UNANCHORED) ? 
                                      RE2::ANCHOR_START : RE2::UNANCHORED;
        
        RE2::Set alt_set(options, alternate_anchor);
        
        // Add some patterns
        int alt_patterns = fdp.ConsumeIntegralInRange<int>(1, 3);
        for (int i = 0; i < alt_patterns && fdp.remaining_bytes() > 10; i++) {
            std::string alt_pattern = fdp.ConsumeRandomLengthString(30);
            std::string error;
            alt_set.Add(alt_pattern, &error);
        }
        
        if (alt_set.Compile()) {
            std::string alt_text = fdp.ConsumeRandomLengthString(100);
            std::vector<int> alt_matches;
            alt_set.Match(alt_text, &alt_matches);
        }
    }

    // Set destruction happens automatically when leaving scope
    return 0;
}

} // namespace harness_003
