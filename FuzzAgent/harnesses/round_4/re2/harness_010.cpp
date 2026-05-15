#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <string>
#include <vector>
#include <memory>
#include <cstring>
#include <algorithm>
#include "re2/re2.h"
#include "re2/set.h"

namespace harness_010 {
// Copyright 2024 Fuzzing Harness Generator
// Harness 010 for RE2 library - targeting complex regex parsing through RE2::Set
// Based on coverage guidance: Critical branch blocker is Regexp::Parse with 139 blocked branches
// Focus: Complex regex patterns to stress parser, RE2::Set operations with error handling
// APIs: RE2::Set::Add (with error strings), RE2::Set::Compile, RE2::Set::Match (with ErrorInfo)




int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need substantial input for complex regex patterns and test strings
    if (size < 256) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Step 1: Configure RE2::Options to test different parsing paths
    RE2::Options options;
    
    // Test encoding (affects character handling)
    if (fdp.ConsumeBool()) {
        options.set_encoding(RE2::Options::EncodingLatin1);
    } else {
        options.set_encoding(RE2::Options::EncodingUTF8);
    }
    
    // Set various parsing flags that affect regex compilation
    options.set_case_sensitive(fdp.ConsumeBool());
    options.set_posix_syntax(fdp.ConsumeBool());
    options.set_literal(fdp.ConsumeBool());
    options.set_never_nl(fdp.ConsumeBool());
    options.set_dot_nl(fdp.ConsumeBool());
    options.set_never_capture(fdp.ConsumeBool());
    options.set_perl_classes(fdp.ConsumeBool());
    options.set_word_boundary(fdp.ConsumeBool());
    options.set_one_line(fdp.ConsumeBool());
    options.set_longest_match(fdp.ConsumeBool());
    options.set_log_errors(false); // Disable error logging for fuzzing
    options.set_max_mem(128 << 20); // 128MB memory limit for complex patterns
    
    // Step 2: Choose anchor type
    RE2::Anchor anchor = fdp.PickValueInArray<RE2::Anchor>({
        RE2::UNANCHORED,
        RE2::ANCHOR_START,
        RE2::ANCHOR_BOTH,
    });
    
    // Step 3: Create RE2::Set object
    RE2::Set set(options, anchor);
    
    // Step 4: Determine number of patterns to add (0-20 for variety)
    int num_patterns = fdp.ConsumeIntegralInRange<int>(0, 20);
    std::vector<std::string> patterns;
    std::vector<int> pattern_indices;
    std::vector<std::string> error_messages;
    
    // Step 5: Add complex regex patterns designed to stress Regexp::Parse
    for (int i = 0; i < num_patterns && fdp.remaining_bytes() > 50; i++) {
        // Generate pattern complexity level (0-9, higher = more complex)
        int complexity = fdp.ConsumeIntegralInRange<int>(0, 9);
        std::string pattern;
        std::string error;
        
        // Base pattern component (simple string to build upon)
        std::string base = fdp.ConsumeRandomLengthString(16);
        if (base.empty() && fdp.ConsumeBool()) {
            base = "a"; // Ensure we have something to work with
        }
        
        switch (complexity) {
            case 0:
                // Simple literal pattern (may be empty for edge case)
                pattern = fdp.ConsumeRandomLengthString(32);
                break;
                
            case 1:
                // Alternations: a|b|c
                {
                    int num_alternatives = fdp.ConsumeIntegralInRange<int>(2, 5);
                    pattern = "";
                    for (int j = 0; j < num_alternatives; j++) {
                        if (j > 0) pattern += "|";
                        pattern += fdp.ConsumeRandomLengthString(4);
                    }
                }
                break;
                
            case 2:
                // Quantifiers: *, +, ?, {n,m}
                {
                    std::string quant_base = fdp.ConsumeRandomLengthString(8);
                    if (!quant_base.empty()) {
                        int quant_type = fdp.ConsumeIntegralInRange<int>(0, 4);
                        switch (quant_type) {
                            case 0: pattern = quant_base + "*"; break;
                            case 1: pattern = quant_base + "+"; break;
                            case 2: pattern = quant_base + "?"; break;
                            case 3: 
                                {
                                    int min_q = fdp.ConsumeIntegralInRange<int>(0, 5);
                                    int max_q = fdp.ConsumeIntegralInRange<int>(min_q, min_q + 5);
                                    pattern = quant_base + "{" + std::to_string(min_q) + "," + std::to_string(max_q) + "}";
                                    if (fdp.ConsumeBool()) pattern += "?"; // non-greedy
                                }
                                break;
                            case 4:
                                // Nested quantifiers
                                pattern = "(" + quant_base + "+)*";
                                break;
                        }
                    }
                }
                break;
                
            case 3:
                // Character classes and escape sequences
                {
                    int class_type = fdp.ConsumeIntegralInRange<int>(0, 6);
                    switch (class_type) {
                        case 0: pattern = "[a-z" + base + "]"; break;
                        case 1: pattern = "[^0-9" + base + "]"; break;
                        case 2: pattern = "\\d+"; break;
                        case 3: pattern = "\\w*"; break;
                        case 4: pattern = "\\s+" + base + "\\S*"; break;
                        case 5: pattern = "\\." + base + "\\."; break; // escaped dot
                        case 6: pattern = "\\[\\]\\{\\}\\(\\)\\|\\*\\+\\?"; // escaped special chars
                                break;
                    }
                }
                break;
                
            case 4:
                // Groups: capturing, non-capturing, atomic
                {
                    int group_type = fdp.ConsumeIntegralInRange<int>(0, 3);
                    std::string group_content = fdp.ConsumeRandomLengthString(12);
                    switch (group_type) {
                        case 0: pattern = "(" + group_content + ")"; break;
                        case 1: pattern = "(?:" + group_content + ")"; break;
                        case 2: pattern = "(?>" + group_content + ")"; break;
                        case 3: // Multiple nested groups
                            pattern = "((?:" + group_content + ")+)";
                            break;
                    }
                }
                break;
                
            case 5:
                // Anchors and boundaries
                {
                    std::string content = fdp.ConsumeRandomLengthString(10);
                    if (!content.empty()) {
                        std::string prefix = "";
                        std::string suffix = "";
                        
                        if (fdp.ConsumeBool()) prefix = "^";
                        if (fdp.ConsumeBool()) suffix = "$";
                        if (fdp.ConsumeBool()) prefix = "\\b" + prefix;
                        if (fdp.ConsumeBool()) suffix = suffix + "\\B";
                        
                        pattern = prefix + content + suffix;
                    }
                }
                break;
                
            case 6:
                // Lookahead and lookbehind assertions
                {
                    std::string main = fdp.ConsumeRandomLengthString(6);
                    std::string assertion = fdp.ConsumeRandomLengthString(4);
                    
                    if (!main.empty() && !assertion.empty()) {
                        int assertion_type = fdp.ConsumeIntegralInRange<int>(0, 3);
                        switch (assertion_type) {
                            case 0: pattern = main + "(?=" + assertion + ")"; break;
                            case 1: pattern = main + "(?!" + assertion + ")"; break;
                            case 2: pattern = "(?<=" + assertion + ")" + main; break;
                            case 3: pattern = "(?<!" + assertion + ")" + main; break;
                        }
                    }
                }
                break;
                
            case 7:
                // Backreferences
                {
                    std::string group_content = fdp.ConsumeRandomLengthString(4);
                    if (!group_content.empty()) {
                        pattern = "(" + group_content + ")\\1";
                        if (fdp.ConsumeBool()) pattern += "+"; // repeated backreference
                    }
                }
                break;
                
            case 8:
                // Complex combination with multiple features
                {
                    std::string part1 = fdp.ConsumeRandomLengthString(3);
                    std::string part2 = fdp.ConsumeRandomLengthString(3);
                    std::string part3 = fdp.ConsumeRandomLengthString(3);
                    
                    if (!part1.empty() && !part2.empty() && !part3.empty()) {
                        pattern = "^(?=.*" + part1 + ")(?=.*" + part2 + ").*" + part3 + "$";
                    }
                }
                break;
                
            case 9:
                // Edge cases: empty, very long, or malformed patterns
                {
                    int edge_type = fdp.ConsumeIntegralInRange<int>(0, 3);
                    switch (edge_type) {
                        case 0: pattern = ""; break; // empty pattern
                        case 1: pattern = fdp.ConsumeRandomLengthString(256); // very long
                                break;
                        case 2: pattern = "["; // malformed (unclosed bracket)
                                break;
                        case 3: pattern = "("; // malformed (unclosed paren)
                                break;
                    }
                }
                break;
        }
        
        // Add pattern to set and capture error if any
        std::string add_error;
        int index = set.Add(pattern, &add_error);
        
        patterns.push_back(pattern);
        pattern_indices.push_back(index);
        error_messages.push_back(add_error);
    }
    
    // Step 6: Get set size before compilation
    int set_size = set.Size();
    
    // Step 7: Compile the set (may fail for certain pattern combinations)
    bool compile_success = set.Compile();
    
    // Step 8: Test matching if compilation succeeded
    if (compile_success && fdp.remaining_bytes() > 20) {
        // Generate various test strings
        int num_test_strings = fdp.ConsumeIntegralInRange<int>(1, 10);
        
        for (int i = 0; i < num_test_strings && fdp.remaining_bytes() > 10; i++) {
            // Generate test string with various characteristics
            std::string test_string;
            int string_type = fdp.ConsumeIntegralInRange<int>(0, 5);
            
            switch (string_type) {
                case 0:
                    test_string = fdp.ConsumeRandomLengthString(64);
                    break;
                case 1:
                    test_string = ""; // empty string
                    break;
                case 2:
                    test_string = fdp.ConsumeRandomLengthString(256); // long string
                    break;
                case 3:
                    // String with special characters
                    test_string = "test\x00null" + fdp.ConsumeRandomLengthString(20);
                    break;
                case 4:
                    // String likely to match some patterns
                    test_string = "abc123" + fdp.ConsumeRandomLengthString(30);
                    break;
                case 5:
                    // Unicode/Latin1 test string
                    test_string = "café" + fdp.ConsumeRandomLengthString(20);
                    break;
            }
            
            // Test Match without ErrorInfo
            std::vector<int> matches;
            bool match_result = set.Match(test_string, &matches);
            
            // Test Match with ErrorInfo
            RE2::Set::ErrorInfo error_info;
            std::vector<int> matches_with_error;
            bool match_with_error_result = set.Match(test_string, &matches_with_error, &error_info);
            
            // Validate consistency between two Match calls
            // (match_result should equal match_with_error_result when matches are found)
        }
    }
    
    // Step 9: Test edge cases for RE2::Set
    
    // Test with empty set
    if (fdp.remaining_bytes() > 30) {
        RE2::Set empty_set(options, anchor);
        bool empty_compile = empty_set.Compile();
        
        if (empty_compile) {
            std::string test_text = fdp.ConsumeRandomLengthString(32);
            std::vector<int> empty_matches;
            empty_set.Match(test_text, &empty_matches);
            
            // Should not match anything
        }
    }
    
    // Test with single very complex pattern
    if (fdp.remaining_bytes() > 100) {
        RE2::Set complex_set(options, anchor);
        
        // Create a highly complex pattern combining many features
        std::string complex_pattern;
        if (fdp.ConsumeBool()) {
            complex_pattern = "^(?=(.*[a-z]))(?=(.*[A-Z]))(?=(.*\\d)).{8,}$"; // password validator
        } else {
            complex_pattern = "\\b(\\w+)\\s+\\1\\b"; // repeated words
        }
        
        std::string complex_error;
        complex_set.Add(complex_pattern, &complex_error);
        complex_set.Compile();
        
        if (complex_set.Size() > 0) {
            std::string complex_test = fdp.ConsumeRandomLengthString(100);
            std::vector<int> complex_matches;
            complex_set.Match(complex_test, &complex_matches);
        }
    }
    
    // Step 10: Test with different anchor type
    if (fdp.remaining_bytes() > 50) {
        RE2::Anchor alt_anchor = (anchor == RE2::UNANCHORED) ? RE2::ANCHOR_BOTH : RE2::UNANCHORED;
        RE2::Set alt_set(options, alt_anchor);
        
        // Add a few patterns
        int alt_patterns = fdp.ConsumeIntegralInRange<int>(1, 5);
        for (int i = 0; i < alt_patterns && fdp.remaining_bytes() > 20; i++) {
            std::string alt_pattern = fdp.ConsumeRandomLengthString(24);
            std::string alt_error;
            alt_set.Add(alt_pattern, &alt_error);
        }
        
        if (alt_set.Size() > 0) {
            alt_set.Compile();
            
            std::string alt_text = fdp.ConsumeRandomLengthString(80);
            std::vector<int> alt_matches;
            alt_set.Match(alt_text, &alt_matches);
        }
    }
    
    return 0;
}

} // namespace harness_010
