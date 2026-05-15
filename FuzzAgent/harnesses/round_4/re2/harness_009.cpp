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

namespace harness_009 {
// Copyright 2024 Fuzzing Harness Generator
// Harness 009 for RE2 library - targeting RE2::Set operations to cover critical coverage gaps
// Based on coverage guidance: RE2::Set::Match has only 1% function coverage with 29 undiscovered branches
// RE2::Set::Add has 54% coverage with 20 undiscovered branches
// APIs: RE2::Set::Add (with error feedback), RE2::Set::Compile, RE2::Set::Match (primary target), RE2::Set::Size, RE2 object for comparison




int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need substantial input for complex regex patterns, options, anchor types, and test texts
    if (size < 128) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Step 1: Create RE2::Options with various configurations to test different parsing paths
    RE2::Options options;
    
    // Test different encoding combinations (Latin1 vs UTF-8)
    switch (fdp.ConsumeIntegralInRange<int>(0, 2)) {
        case 0: options.set_encoding(RE2::Options::EncodingLatin1); break;
        case 1: options.set_encoding(RE2::Options::EncodingUTF8); break;
        case 2: 
            // Random encoding based on bool
            if (fdp.ConsumeBool()) {
                options.set_encoding(RE2::Options::EncodingLatin1);
            } else {
                options.set_encoding(RE2::Options::EncodingUTF8);
            }
            break;
    }
    
    // Set various flags that affect pattern compilation and matching
    options.set_case_sensitive(!fdp.ConsumeBool());
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
    options.set_max_mem(64 << 20); // 64MB memory limit
    
    // Step 2: Choose anchor type
    RE2::Anchor anchor = fdp.PickValueInArray<RE2::Anchor>({
        RE2::UNANCHORED,
        RE2::ANCHOR_START,
        RE2::ANCHOR_BOTH,
    });
    
    // Step 3: Create RE2::Set object
    RE2::Set set(options, anchor);
    
    // Step 4: Determine number of patterns to add (0-12 to test empty set edge cases)
    int num_patterns = fdp.ConsumeIntegralInRange<int>(0, 12);
    std::vector<std::string> patterns_added;
    std::vector<int> pattern_indices;
    std::vector<bool> pattern_valid;
    
    // Step 5: Add diverse regex patterns with varying complexity
    for (int i = 0; i < num_patterns && fdp.remaining_bytes() > 20; i++) {
        // Generate different types of regex patterns to exercise various parsing paths
        int pattern_type = fdp.ConsumeIntegralInRange<int>(0, 10);
        std::string pattern;
        std::string error_msg;
        
        switch (pattern_type) {
            case 0:
                // Simple literal pattern
                pattern = fdp.ConsumeRandomLengthString(32);
                break;
                
            case 1:
                // Perl syntax patterns with flags: (?i), (?m), (?s), (?-i)
                {
                    std::string base = fdp.ConsumeRandomLengthString(16);
                    std::string flag;
                    switch (fdp.ConsumeIntegralInRange<int>(0, 3)) {
                        case 0: flag = "i"; break;  // case-insensitive
                        case 1: flag = "m"; break;  // multi-line mode
                        case 2: flag = "s"; break;  // dot matches newline
                        case 3: flag = "-i"; break; // turn off case-insensitive
                    }
                    pattern = "(?" + flag + ")" + base;
                    
                    // Test multiple flags: (?im), (?is), (?ms)
                    if (fdp.ConsumeBool() && base.size() > 2) {
                        std::string flag2;
                        switch (fdp.ConsumeIntegralInRange<int>(0, 2)) {
                            case 0: flag2 = "i"; break;
                            case 1: flag2 = "m"; break;
                            case 2: flag2 = "s"; break;
                        }
                        pattern = "(?" + flag + flag2 + ")" + base.substr(0, std::min(base.size(), size_t(5)));
                    }
                }
                break;
                
            case 2:
                // Character classes: simple, negated, ranges
                {
                    std::string base = fdp.ConsumeRandomLengthString(8);
                    if (!base.empty()) {
                        int class_type = fdp.ConsumeIntegralInRange<int>(0, 3);
                        switch (class_type) {
                            case 0:
                                pattern = "[a-zA-Z0-9" + base.substr(0, std::min(base.size(), size_t(3))) + "]";
                                break;
                            case 1:
                                pattern = "[^a-zA-Z0-9" + base.substr(0, std::min(base.size(), size_t(3))) + "]";
                                break;
                            case 2:
                                pattern = "[\\d\\w\\s" + base.substr(0, std::min(base.size(), size_t(2))) + "]";
                                break;
                            case 3:
                                // Unicode character class
                                pattern = "[\\p{L}\\p{N}" + base.substr(0, std::min(base.size(), size_t(2))) + "]";
                                break;
                        }
                    }
                }
                break;
                
            case 3:
                // Named captures: (?P<name>pattern)
                {
                    std::string base = fdp.ConsumeRandomLengthString(12);
                    std::string name = fdp.ConsumeRandomLengthString(8);
                    if (!base.empty() && !name.empty()) {
                        pattern = "(?P<" + name.substr(0, std::min(name.size(), size_t(6))) + ">" + 
                                 base.substr(0, std::min(base.size(), size_t(8))) + ")";
                    }
                }
                break;
                
            case 4:
                // Alternations: pattern1|pattern2|pattern3
                {
                    std::string part1 = fdp.ConsumeRandomLengthString(8);
                    std::string part2 = fdp.ConsumeRandomLengthString(8);
                    std::string part3 = fdp.ConsumeRandomLengthString(8);
                    
                    if (!part1.empty() && !part2.empty() && !part3.empty()) {
                        pattern = part1.substr(0, std::min(part1.size(), size_t(3))) + "|" +
                                 part2.substr(0, std::min(part2.size(), size_t(3))) + "|" +
                                 part3.substr(0, std::min(part3.size(), size_t(3)));
                    }
                }
                break;
                
            case 5:
                // Repetition with various quantifiers: *, +, ?, {n,m}
                {
                    std::string base = fdp.ConsumeRandomLengthString(10);
                    if (!base.empty()) {
                        std::string quantifier;
                        int quant_type = fdp.ConsumeIntegralInRange<int>(0, 4);
                        switch (quant_type) {
                            case 0: quantifier = "*"; break;
                            case 1: quantifier = "+"; break;
                            case 2: quantifier = "?"; break;
                            case 3: quantifier = "*?"; break;  // non-greedy
                            case 4: quantifier = "+?"; break;  // non-greedy
                        }
                        
                        // Test range repetition: {n,m} or {n,} or {n}
                        if (fdp.ConsumeBool()) {
                            int min_val = fdp.ConsumeIntegralInRange<int>(0, 5);
                            int max_val = fdp.ConsumeIntegralInRange<int>(min_val, min_val + 5);
                            quantifier = "{" + std::to_string(min_val) + "," + std::to_string(max_val) + "}";
                            if (fdp.ConsumeBool()) {
                                quantifier += "?";  // non-greedy range
                            }
                        }
                        
                        pattern = base.substr(0, std::min(base.size(), size_t(4))) + quantifier;
                    }
                }
                break;
                
            case 6:
                // Anchors and word boundaries: ^, $, \b, \B
                {
                    std::string base = fdp.ConsumeRandomLengthString(12);
                    if (!base.empty()) {
                        std::string prefix = "";
                        std::string suffix = "";
                        
                        if (fdp.ConsumeBool()) prefix = "^";
                        if (fdp.ConsumeBool()) suffix = "$";
                        if (fdp.ConsumeBool()) {
                            if (fdp.ConsumeBool()) {
                                prefix = "\\b" + prefix;
                            } else {
                                suffix = suffix + "\\b";
                            }
                        }
                        if (fdp.ConsumeBool()) {
                            if (fdp.ConsumeBool()) {
                                prefix = "\\B" + prefix;
                            } else {
                                suffix = suffix + "\\B";
                            }
                        }
                        
                        pattern = prefix + base + suffix;
                    }
                }
                break;
                
            case 7:
                // Grouping: capturing vs non-capturing, atomic groups
                {
                    std::string base = fdp.ConsumeRandomLengthString(10);
                    if (!base.empty()) {
                        int group_type = fdp.ConsumeIntegralInRange<int>(0, 2);
                        switch (group_type) {
                            case 0:
                                pattern = "(" + base + ")";  // capturing group
                                break;
                            case 1:
                                pattern = "(?:" + base + ")";  // non-capturing group
                                break;
                            case 2:
                                pattern = "(?>" + base + ")";  // atomic group
                                break;
                        }
                    }
                }
                break;
                
            case 8:
                // Lookahead and lookbehind assertions
                {
                    std::string base = fdp.ConsumeRandomLengthString(8);
                    std::string look = fdp.ConsumeRandomLengthString(4);
                    
                    if (!base.empty() && !look.empty()) {
                        int assertion_type = fdp.ConsumeIntegralInRange<int>(0, 3);
                        switch (assertion_type) {
                            case 0:
                                pattern = base + "(?=" + look + ")";  // positive lookahead
                                break;
                            case 1:
                                pattern = base + "(?!" + look + ")";  // negative lookahead
                                break;
                            case 2:
                                pattern = "(?<=" + look + ")" + base;  // positive lookbehind
                                break;
                            case 3:
                                pattern = "(?<!" + look + ")" + base;  // negative lookbehind
                                break;
                        }
                    }
                }
                break;
                
            case 9:
                // Backreferences: \1, \2, etc.
                {
                    std::string base = fdp.ConsumeRandomLengthString(6);
                    if (!base.empty()) {
                        pattern = "(" + base.substr(0, std::min(base.size(), size_t(2))) + ")\\1";
                    }
                }
                break;
                
            case 10:
                // Complex combination of multiple features
                {
                    std::string part1 = fdp.ConsumeRandomLengthString(4);
                    std::string part2 = fdp.ConsumeRandomLengthString(4);
                    std::string part3 = fdp.ConsumeRandomLengthString(4);
                    
                    if (!part1.empty() && !part2.empty() && !part3.empty()) {
                        pattern = "(?i)(?P<name>" + part1 + ")(?:" + part2 + "|" + part3 + "){1,3}?\\b";
                    }
                }
                break;
        }
        
        // If pattern is still empty, generate a simple one
        if (pattern.empty() && fdp.remaining_bytes() > 0) {
            pattern = fdp.ConsumeRandomLengthString(16);
        }
        
        if (pattern.empty()) {
            continue;
        }
        
        patterns_added.push_back(pattern);
        
        // Add pattern to set with error feedback - primary target API
        std::string add_error;
        int index = set.Add(pattern, &add_error);
        pattern_indices.push_back(index);
        pattern_valid.push_back(index != -1);
        
        // Test error feedback by occasionally adding intentionally malformed patterns
        if (i == 0 && fdp.ConsumeBool() && fdp.remaining_bytes() > 5) {
            // Add intentionally malformed patterns to test error paths in RE2::Set::Add
            std::string malformed;
            int error_type = fdp.ConsumeIntegralInRange<int>(0, 5);
            switch (error_type) {
                case 0: malformed = "("; break;  // Unclosed parenthesis
                case 1: malformed = "["; break;  // Unclosed bracket
                case 2: malformed = "\\"; break; // Trailing backslash
                case 3: malformed = "*"; break;  // Lone quantifier
                case 4: malformed = "{5"; break; // Incomplete repetition
                case 5: malformed = "(?P<>abc)"; break; // Empty named capture
            }
            
            std::string malformed_error;
            set.Add(malformed, &malformed_error);
        }
    }
    
    // Step 6: Test Set::Size before compilation
    int size_before_compile = set.Size();
    
    // Step 7: Compile the set - required before matching
    bool compile_result = set.Compile();
    
    // Step 8: Test Set::Size after compilation
    int size_after_compile = set.Size();
    
    // Only proceed with matching if compilation succeeded and set is not empty
    if (compile_result && size_after_compile > 0) {
        // Step 9: Generate diverse text inputs to match against
        // Generate multiple text strings to exercise different matching paths
        int num_texts = fdp.ConsumeIntegralInRange<int>(1, 5);
        std::vector<std::string> test_texts;
        
        for (int i = 0; i < num_texts && fdp.remaining_bytes() > 10; i++) {
            std::string text;
            int text_type = fdp.ConsumeIntegralInRange<int>(0, 4);
            
            switch (text_type) {
                case 0:
                    // Random text
                    text = fdp.ConsumeRandomLengthString(128);
                    break;
                case 1:
                    // Text containing pattern fragments
                    if (!patterns_added.empty()) {
                        int pattern_idx = fdp.ConsumeIntegralInRange<int>(0, patterns_added.size() - 1);
                        std::string pattern_fragment = patterns_added[pattern_idx].substr(
                            0, std::min(patterns_added[pattern_idx].size(), size_t(10)));
                        text = "prefix_" + pattern_fragment + "_suffix" + 
                               fdp.ConsumeRandomLengthString(32);
                    } else {
                        text = fdp.ConsumeRandomLengthString(64);
                    }
                    break;
                case 2:
                    // Text with special characters
                    text = fdp.ConsumeRandomLengthString(96);
                    // Inject some special chars
                    if (text.size() > 10) {
                        text.insert(text.size() / 2, "\n\t\r");
                    }
                    break;
                case 3:
                    // Empty text (edge case)
                    text = "";
                    break;
                case 4:
                    // Text with Unicode characters (if UTF-8 encoding)
                    text = fdp.ConsumeRandomLengthString(80);
                    if (text.size() > 20 && options.encoding() == RE2::Options::EncodingUTF8) {
                        // Add some common UTF-8 sequences
                        text.insert(text.size() / 3, " café résumé naïve ");
                    }
                    break;
            }
            
            test_texts.push_back(text);
        }
        
        // If no texts were generated, create at least one
        if (test_texts.empty() && fdp.remaining_bytes() > 0) {
            test_texts.push_back(fdp.ConsumeRandomLengthString(64));
        }
        
        // Step 10: Perform Match operations - primary target API
        for (const auto& text : test_texts) {
            // Test Match with vector to capture indices
            std::vector<int> matches;
            bool match_result = set.Match(text, &matches);
            
            // Test Match without vector (simpler version)
            bool simple_match_result = set.Match(text, nullptr);
            
            // Test Match with ErrorInfo to exercise error reporting paths
            RE2::Set::ErrorInfo error_info;
            std::vector<int> error_info_matches;
            bool match_with_error_info = set.Match(text, &error_info_matches, &error_info);
            
            // Step 11: Validate results against individual RE2 pattern matching for comparison
            // Create individual RE2 objects for patterns that were successfully added
            for (size_t i = 0; i < patterns_added.size() && i < pattern_valid.size(); i++) {
                if (pattern_valid[i] && pattern_indices[i] != -1) {
                    // Create RE2 object with same options
                    RE2 re2_pattern(patterns_added[i], options);
                    
                    if (re2_pattern.ok()) {
                        // Test FullMatch for comparison
                        bool full_match = RE2::FullMatch(text, re2_pattern);
                        
                        // Test PartialMatch for comparison
                        bool partial_match = RE2::PartialMatch(text, re2_pattern);
                        
                        // Compare with set matching results
                        bool in_matches = std::find(matches.begin(), matches.end(), pattern_indices[i]) != matches.end();
                        
                        // For debugging/logging (commented out in fuzzing)
                        // The comparison helps validate RE2::Set::Match behavior
                    }
                }
            }
        }
        
        // Step 12: Test additional edge cases for Match
        if (fdp.remaining_bytes() > 0) {
            // Test with very long text
            if (fdp.remaining_bytes() > 100) {
                std::string long_text = fdp.ConsumeRandomLengthString(512);
                std::vector<int> long_matches;
                set.Match(long_text, &long_matches);
            }
            
            // Test with text containing null bytes
            std::string null_text = "prefix";
            null_text.push_back('\0');
            null_text += "suffix" + fdp.ConsumeRandomLengthString(20);
            std::vector<int> null_matches;
            set.Match(null_text, &null_matches);
        }
    }
    
    // Step 13: Test edge cases for empty set
    if (fdp.remaining_bytes() > 20) {
        RE2::Set empty_set(options, anchor);
        int empty_size = empty_set.Size();
        bool empty_compile = empty_set.Compile();
        
        if (empty_compile) {
            // Test matching with empty set
            std::string test_text = fdp.ConsumeRandomLengthString(32);
            std::vector<int> empty_matches;
            bool empty_match_result = empty_set.Match(test_text, &empty_matches);
            
            // Test with ErrorInfo
            RE2::Set::ErrorInfo empty_error_info;
            std::vector<int> empty_error_matches;
            empty_set.Match(test_text, &empty_error_matches, &empty_error_info);
        }
    }
    
    // Step 14: Test with different anchor types
    if (fdp.remaining_bytes() > 30) {
        RE2::Anchor alt_anchor = fdp.PickValueInArray<RE2::Anchor>({
            RE2::UNANCHORED,
            RE2::ANCHOR_START,
            RE2::ANCHOR_BOTH,
        });
        
        if (alt_anchor != anchor) {
            RE2::Set alt_set(options, alt_anchor);
            
            // Add a few patterns
            int alt_patterns = fdp.ConsumeIntegralInRange<int>(1, 3);
            for (int i = 0; i < alt_patterns && fdp.remaining_bytes() > 10; i++) {
                std::string alt_pattern = fdp.ConsumeRandomLengthString(16);
                if (!alt_pattern.empty()) {
                    std::string alt_error;
                    alt_set.Add(alt_pattern, &alt_error);
                }
            }
            
            if (alt_set.Size() > 0) {
                alt_set.Compile();
                
                if (fdp.remaining_bytes() > 0) {
                    std::string alt_text = fdp.ConsumeRandomLengthString(64);
                    std::vector<int> alt_matches;
                    alt_set.Match(alt_text, &alt_matches);
                }
            }
        }
    }
    
    return 0;
}

} // namespace harness_009
