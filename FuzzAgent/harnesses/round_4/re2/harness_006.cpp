#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <string>
#include <vector>
#include <memory>
#include <cstring>
#include "re2/re2.h"
#include "re2/set.h"

namespace harness_006 {
// Copyright 2024 Fuzzing Harness Generator
// Seventh harness for RE2 library - targeting RE2::Set operations to penetrate blocked branches in Regexp::Parse
// Based on coverage guidance: 139 blocked branches in Regexp::Parse (86.9% blocked), reachable via RE2::Set::Add




int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need sufficient input for options, multiple patterns, text, and operation parameters
    if (size < 128) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Consume RE2::Options from fuzzer input with emphasis on flags affecting Regexp::Parse
    RE2::Options options;
    
    // Test both encodings: Latin1 and UTF-8 - line 2279-2284 in parse.cc
    options.set_encoding(fdp.ConsumeBool() ? RE2::Options::EncodingLatin1 
                                          : RE2::Options::EncodingUTF8);
    
    // Set various parsing flags that affect Regexp::Parse paths
    options.set_posix_syntax(fdp.ConsumeBool());
    options.set_longest_match(fdp.ConsumeBool());
    options.set_log_errors(false);  // Don't log during fuzzing
    options.set_max_mem(64 << 20);  // 64 MB max memory
    options.set_literal(fdp.ConsumeBool());
    options.set_never_nl(fdp.ConsumeBool());
    options.set_dot_nl(fdp.ConsumeBool());
    
    // CRITICAL: Test NeverCapture flag - line 2319 in parse.cc
    options.set_never_capture(fdp.ConsumeBool());
    
    options.set_case_sensitive(!fdp.ConsumeBool());
    options.set_perl_classes(fdp.ConsumeBool());  // Enable Perl extensions for (?i), (?m), (?s) parsing
    options.set_word_boundary(fdp.ConsumeBool());
    options.set_one_line(fdp.ConsumeBool());
    
    // Choose anchor type
    RE2::Anchor anchor = fdp.PickValueInArray<RE2::Anchor>({
        RE2::UNANCHORED,
        RE2::ANCHOR_START,
        RE2::ANCHOR_BOTH,
    });
    
    // Create RE2::Set object
    RE2::Set set(options, anchor);
    
    // Determine number of patterns to add (1-8 patterns)
    int num_patterns = fdp.ConsumeIntegralInRange<int>(1, 8);
    std::vector<std::string> patterns;
    std::vector<int> pattern_indices;
    std::string error;
    
    // Add patterns with various complexity to exercise Regexp::Parse deeply
    for (int i = 0; i < num_patterns && fdp.remaining_bytes() > 10; i++) {
        // Determine pattern complexity level
        int complexity = fdp.ConsumeIntegralInRange<int>(0, 5);
        std::string pattern;
        
        switch (complexity) {
            case 0:
                // Simple literal pattern
                pattern = fdp.ConsumeRandomLengthString(32);
                break;
                
            case 1:
                // Pattern with character classes - line 2359 in parse.cc
                {
                    std::string base = fdp.ConsumeRandomLengthString(16);
                    if (!base.empty()) {
                        pattern = "[a-zA-Z0-9" + base.substr(0, std::min(base.size(), size_t(3))) + "]";
                        if (fdp.ConsumeBool()) {
                            pattern = "[^" + pattern.substr(1);  // Negated character class
                        }
                    }
                }
                break;
                
            case 2:
                // Pattern with Perl flags - line 2313 in parse.cc: (?i), (?m), (?s)
                {
                    std::string base = fdp.ConsumeRandomLengthString(16);
                    std::string flag;
                    switch (fdp.ConsumeIntegralInRange<int>(0, 2)) {
                        case 0: flag = "i"; break;  // case-insensitive
                        case 1: flag = "m"; break;  // multi-line mode
                        case 2: flag = "s"; break;  // dot matches newline
                    }
                    pattern = "(?" + flag + ")" + base;
                    
                    // Test nested flag groups: ((?i)[a-z]+(?-i)[0-9]*)
                    if (fdp.ConsumeBool() && base.size() > 2) {
                        pattern = "((?" + flag + ")" + base.substr(0, 1) + "+(?-" + flag + ")" + 
                                 base.substr(1, 1) + "*)";
                    }
                }
                break;
                
            case 3:
                // Pattern with repetition operators and greediness modifiers
                {
                    std::string base = fdp.ConsumeRandomLengthString(16);
                    if (!base.empty()) {
                        std::string quantifier;
                        switch (fdp.ConsumeIntegralInRange<int>(0, 5)) {
                            case 0: quantifier = "*"; break;
                            case 1: quantifier = "+"; break;
                            case 2: quantifier = "?"; break;
                            case 3: quantifier = "*?"; break;  // non-greedy
                            case 4: quantifier = "+?"; break;  // non-greedy
                            case 5: quantifier = "??"; break;  // non-greedy
                        }
                        
                        // Test range repetition: {n,m}
                        if (fdp.ConsumeBool()) {
                            int min = fdp.ConsumeIntegralInRange<int>(0, 5);
                            int max = fdp.ConsumeIntegralInRange<int>(min, min + 5);
                            quantifier = "{" + std::to_string(min) + "," + std::to_string(max) + "}";
                            if (fdp.ConsumeBool()) {
                                quantifier += "?";  // non-greedy range
                            }
                        }
                        
                        pattern = base.substr(0, std::min(base.size(), size_t(3))) + quantifier;
                    }
                }
                break;
                
            case 4:
                // Pattern with nested groups and alternation
                {
                    std::string part1 = fdp.ConsumeRandomLengthString(8);
                    std::string part2 = fdp.ConsumeRandomLengthString(8);
                    std::string part3 = fdp.ConsumeRandomLengthString(8);
                    
                    if (!part1.empty() && !part2.empty() && !part3.empty()) {
                        // Create nested groups: (a(b|c)d)
                        pattern = "(" + part1.substr(0, 1) + "(" + 
                                 part2.substr(0, 1) + "|" + part3.substr(0, 1) + ")" + 
                                 part1.substr(std::min(part1.size(), size_t(2)), 1) + ")";
                        
                        // Add capturing vs non-capturing groups
                        if (fdp.ConsumeBool()) {
                            pattern = "(?:" + pattern.substr(1);  // non-capturing group
                        }
                    }
                }
                break;
                
            case 5:
                // Pattern with anchors and word boundaries
                {
                    std::string base = fdp.ConsumeRandomLengthString(16);
                    if (!base.empty()) {
                        std::string prefix = "";
                        std::string suffix = "";
                        
                        if (fdp.ConsumeBool()) prefix = "^";
                        if (fdp.ConsumeBool()) suffix = "$";
                        if (fdp.ConsumeBool()) {
                            // Word boundary: \b
                            if (fdp.ConsumeBool()) {
                                prefix = "\\b" + prefix;
                            } else {
                                suffix = suffix + "\\b";
                            }
                        }
                        
                        pattern = prefix + base + suffix;
                    }
                }
                break;
        }
        
        // Ensure we have a pattern
        if (pattern.empty() && fdp.remaining_bytes() > 0) {
            pattern = fdp.ConsumeRandomLengthString(8);
        }
        
        if (pattern.empty()) {
            continue;
        }
        
        patterns.push_back(pattern);
        
        // Add pattern to set - this calls Regexp::Parse internally
        // Use error pointer to capture parsing errors (testing error conditions)
        error.clear();
        int index = set.Add(pattern, &error);
        pattern_indices.push_back(index);
        
        // Test error conditions by occasionally adding malformed patterns
        if (i == 0 && fdp.ConsumeBool() && fdp.remaining_bytes() > 5) {
            // Add intentionally malformed patterns to test error paths
            std::string malformed;
            switch (fdp.ConsumeIntegralInRange<int>(0, 3)) {
                case 0: malformed = "("; break;  // Unclosed parenthesis
                case 1: malformed = "["; break;  // Unclosed bracket
                case 2: malformed = "\\"; break; // Trailing backslash
                case 3: malformed = "*"; break;  // Lone quantifier
            }
            
            std::string malformed_error;
            set.Add(malformed, &malformed_error);
        }
    }
    
    // Test Set::Size before compilation
    int size_before_compile = set.Size();
    
    // CRITICAL: Compile the set - exercises compilation paths
    bool compile_result = set.Compile();
    
    // Test Set::Size after compilation
    int size_after_compile = set.Size();
    
    // Only proceed with matching if compilation succeeded
    if (compile_result) {
        // Generate text to match against from fuzzer input
        std::string text;
        if (fdp.remaining_bytes() > 0) {
            text = fdp.ConsumeRandomLengthString(256);
        } else {
            // If no input left, use a small default to avoid crash
            text = "";
        }
        
        // Test Match operation with vector to capture indices
        std::vector<int> matches;
        bool match_result = set.Match(text, &matches);
        
        // Test Match operation without vector
        bool simple_match_result = set.Match(text, nullptr);
        
        // Test with various text inputs to exercise different matching paths
        if (fdp.remaining_bytes() > 0) {
            // Test with empty text
            std::vector<int> empty_matches;
            set.Match("", &empty_matches);
            
            // Test with text containing pattern fragments
            if (!patterns.empty()) {
                for (const auto& pattern : patterns) {
                    if (pattern.size() > 2) {
                        std::string fragment_text = "prefix_" + 
                            pattern.substr(0, std::min(pattern.size(), size_t(5))) + "_suffix";
                        std::vector<int> fragment_matches;
                        set.Match(fragment_text, &fragment_matches);
                    }
                }
            }
            
            // Test with text containing special characters generated from fuzzer input
            if (fdp.remaining_bytes() > 10) {
                std::string special_text = fdp.ConsumeRandomLengthString(32);
                std::vector<int> special_matches;
                set.Match(special_text, &special_matches);
            }
        }
        
        // Test the ErrorInfo version of Match
        if (fdp.remaining_bytes() > 0) {
            RE2::Set::ErrorInfo error_info;
            std::vector<int> error_info_matches;
            set.Match(text, &error_info_matches, &error_info);
        }
    }
    // Test edge cases
    if (fdp.remaining_bytes() > 10) {
        // Test with empty set
        RE2::Set empty_set(options, anchor);
        int empty_size = empty_set.Size();
        bool empty_compile = empty_set.Compile();
        if (empty_compile) {
            std::vector<int> empty_matches;
            std::string empty_test_text = fdp.ConsumeRandomLengthString(16);
            empty_set.Match(empty_test_text, &empty_matches);
        }
        
        // Test with single very simple pattern
        RE2::Set simple_set(options, anchor);
        std::string simple_error;
        std::string simple_pattern = fdp.ConsumeRandomLengthString(8);
        if (simple_pattern.empty() && fdp.remaining_bytes() > 0) {
            simple_pattern = fdp.ConsumeRandomLengthString(1);
        }
        simple_set.Add(simple_pattern, &simple_error);
        simple_set.Compile();
        
        // Test matching with the simple set
        std::vector<int> simple_matches;
        std::string match_text1 = fdp.ConsumeRandomLengthString(16);
        std::string match_text2 = fdp.ConsumeRandomLengthString(16);
        simple_set.Match(match_text1, &simple_matches);
        simple_set.Match(match_text2, &simple_matches);
    }
    // Test patterns that specifically target the 139 blocked branches in Regexp::Parse
    if (fdp.remaining_bytes() > 50) {
        // Create another set for targeted testing
        RE2::Set targeted_set(options, anchor);
        
        // Generate targeted patterns from fuzzer input instead of hardcoded values
        std::vector<std::string> targeted_patterns;
        
        // Generate Perl flag patterns from fuzzer input
        if (fdp.remaining_bytes() > 20) {
            std::string perl_base = fdp.ConsumeRandomLengthString(12);
            if (!perl_base.empty()) {
                // Generate different Perl flag patterns
                targeted_patterns.push_back("(?i)" + perl_base);
                targeted_patterns.push_back("(?m)^" + perl_base + "$");
                targeted_patterns.push_back("(?s).*" + perl_base);
            }
        }
        
        // Generate nested flag groups
        if (fdp.remaining_bytes() > 15) {
            std::string nested_base = fdp.ConsumeRandomLengthString(10);
            if (!nested_base.empty() && nested_base.size() > 2) {
                targeted_patterns.push_back("((?i)" + nested_base.substr(0, 1) + "+(?-i)" + 
                                          nested_base.substr(1, 1) + "*)");
            }
        }
        
        // Generate complex character classes
        if (fdp.remaining_bytes() > 10) {
            std::string char_class_base = fdp.ConsumeRandomLengthString(8);
            if (!char_class_base.empty()) {
                targeted_patterns.push_back("[a-zA-Z0-9_" + char_class_base.substr(0, 1) + "]+");
                targeted_patterns.push_back("[^" + char_class_base + "]");
            }
        }
        
        // Add error condition patterns (should fail parsing)
        targeted_patterns.push_back("(");    // Unclosed parenthesis
        targeted_patterns.push_back("[");    // Unclosed bracket
        targeted_patterns.push_back("\\");   // Trailing backslash
        targeted_patterns.push_back("*");    // Lone quantifier
        targeted_patterns.push_back("{}");   // Empty repetition
        
        for (const auto& targeted_pattern : targeted_patterns) {
            std::string targeted_error;
            targeted_set.Add(targeted_pattern, &targeted_error);
        }
        
        if (targeted_set.Size() > 0) {
            targeted_set.Compile();
            
            // Generate test inputs from fuzzer data
            std::vector<std::string> test_inputs;
            for (int i = 0; i < 5 && fdp.remaining_bytes() > 10; i++) {
                std::string test_input = fdp.ConsumeRandomLengthString(32);
                if (!test_input.empty()) {
                    test_inputs.push_back(test_input);
                }
            }
            
            // Add some edge case inputs
            if (fdp.remaining_bytes() > 5) {
                test_inputs.push_back("");  // Empty string
                test_inputs.push_back(fdp.ConsumeRandomLengthString(1));  // Single character
            }
            
            // Add newline-containing input for multi-line testing
            if (fdp.remaining_bytes() > 10) {
                std::string multiline_input = fdp.ConsumeRandomLengthString(16) + "\n" + 
                                             fdp.ConsumeRandomLengthString(16);
                test_inputs.push_back(multiline_input);
            }
            
            // Test matching with various inputs
            for (const auto& test_input : test_inputs) {
                std::vector<int> targeted_matches;
                targeted_set.Match(test_input, &targeted_matches);
            }
        }
    }
    
    return 0;
}

} // namespace harness_006
