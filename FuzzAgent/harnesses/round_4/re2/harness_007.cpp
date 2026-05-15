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

namespace harness_007 {
// Copyright 2024 Fuzzing Harness Generator
// Harness 007 for RE2 library - targeting RE2::Set operations to penetrate blocked parsing paths
// Based on coverage guidance: Regexp::Parse has 139/160 blocked branches, reachable via RE2::Set::Add




int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need substantial input for complex regex patterns and operations
    if (size < 256) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Create multiple RE2::Set objects with different options to test various parsing paths
    std::vector<std::unique_ptr<RE2::Set>> sets;
    
    // Test different option combinations that affect Regexp::Parse
    for (int set_idx = 0; set_idx < 3 && fdp.remaining_bytes() > 100; set_idx++) {
        RE2::Options options;
        
        // Test different encoding combinations
        switch (fdp.ConsumeIntegralInRange<int>(0, 2)) {
            case 0: options.set_encoding(RE2::Options::EncodingLatin1); break;
            case 1: options.set_encoding(RE2::Options::EncodingUTF8); break;
            case 2: options.set_encoding(fdp.ConsumeBool() ? RE2::Options::EncodingLatin1 
                                                          : RE2::Options::EncodingUTF8); break;
        }
        
        // Set flags that directly affect parsing
        options.set_posix_syntax(fdp.ConsumeBool());
        options.set_literal(fdp.ConsumeBool());
        options.set_never_nl(fdp.ConsumeBool());
        options.set_dot_nl(fdp.ConsumeBool());
        options.set_never_capture(fdp.ConsumeBool());  // CRITICAL for parsing paths
        options.set_case_sensitive(!fdp.ConsumeBool());
        options.set_perl_classes(fdp.ConsumeBool());  // Enables (?i), (?m), (?s) parsing
        options.set_word_boundary(fdp.ConsumeBool());
        options.set_one_line(fdp.ConsumeBool());
        options.set_longest_match(fdp.ConsumeBool());
        options.set_log_errors(false);
        options.set_max_mem(64 << 20);
        
        RE2::Anchor anchor = fdp.PickValueInArray<RE2::Anchor>({
            RE2::UNANCHORED,
            RE2::ANCHOR_START,
            RE2::ANCHOR_BOTH,
        });
        
        auto set = std::make_unique<RE2::Set>(options, anchor);
        
        // Generate diverse regex patterns targeting blocked branches in Regexp::Parse
        int num_patterns = fdp.ConsumeIntegralInRange<int>(1, 12);
        std::vector<std::string> patterns_added;
        
        for (int i = 0; i < num_patterns && fdp.remaining_bytes() > 50; i++) {
            // Use fuzzer input to determine pattern complexity and type
            int pattern_type = fdp.ConsumeIntegralInRange<int>(0, 8);
            std::string pattern;
            
            switch (pattern_type) {
                case 0: // Simple literal patterns
                    pattern = fdp.ConsumeRandomLengthString(32);
                    break;
                    
                case 1: // Character classes - line 2359 in parse.cc
                    {
                        std::string chars = fdp.ConsumeRandomLengthString(16);
                        if (!chars.empty()) {
                            std::string class_content;
                            bool negated = fdp.ConsumeBool();
                            bool has_range = fdp.ConsumeBool();
                            
                            if (negated) class_content = "[^";
                            else class_content = "[";
                            
                            if (has_range && chars.size() >= 2) {
                                class_content += chars[0];
                                class_content += '-';
                                class_content += chars[1];
                                if (chars.size() >= 3) {
                                    class_content += chars[2];
                                }
                            } else {
                                class_content += chars.substr(0, std::min(chars.size(), size_t(8)));
                            }
                            class_content += "]";
                            pattern = class_content;
                        }
                    }
                    break;
                    
                case 2: // Perl-style flags - line 2313 in parse.cc
                    {
                        std::string base = fdp.ConsumeRandomLengthString(24);
                        if (!base.empty()) {
                            std::string flags;
                            int flag_count = fdp.ConsumeIntegralInRange<int>(1, 3);
                            for (int f = 0; f < flag_count; f++) {
                                switch (fdp.ConsumeIntegralInRange<int>(0, 3)) {
                                    case 0: flags += "i"; break;  // case-insensitive
                                    case 1: flags += "m"; break;  // multi-line
                                    case 2: flags += "s"; break;  // dot matches newline
                                    case 3: flags += "U"; break;  // ungreedy
                                }
                            }
                            pattern = "(?" + flags + ")" + base;
                            
                            // Test nested flag groups
                            if (fdp.ConsumeBool() && base.size() >= 4) {
                                pattern = "((?" + flags.substr(0, 1) + ")" + 
                                         base.substr(0, 2) + "(?-" + flags.substr(0, 1) + ")" +
                                         base.substr(2, 2) + ")";
                            }
                        }
                    }
                    break;
                    
                case 3: // Quantifiers and repetition
                    {
                        std::string base = fdp.ConsumeRandomLengthString(16);
                        if (!base.empty()) {
                            std::string quantifier;
                            int quant_type = fdp.ConsumeIntegralInRange<int>(0, 6);
                            
                            switch (quant_type) {
                                case 0: quantifier = "*"; break;
                                case 1: quantifier = "+"; break;
                                case 2: quantifier = "?"; break;
                                case 3: quantifier = "*?"; break;  // non-greedy
                                case 4: quantifier = "+?"; break;  // non-greedy
                                case 5: quantifier = "??"; break;  // non-greedy
                                case 6: // Range repetition {n,m}
                                    {
                                        int min_val = fdp.ConsumeIntegralInRange<int>(0, 10);
                                        int max_val = fdp.ConsumeIntegralInRange<int>(min_val, min_val + 10);
                                        quantifier = "{" + std::to_string(min_val) + "," + std::to_string(max_val) + "}";
                                        if (fdp.ConsumeBool()) quantifier += "?";
                                    }
                                    break;
                            }
                            
                            pattern = base.substr(0, std::min(base.size(), size_t(4))) + quantifier;
                        }
                    }
                    break;
                    
                case 4: // Groups and alternation
                    {
                        std::string part1 = fdp.ConsumeRandomLengthString(12);
                        std::string part2 = fdp.ConsumeRandomLengthString(12);
                        std::string part3 = fdp.ConsumeRandomLengthString(12);
                        
                        if (!part1.empty() && !part2.empty() && !part3.empty()) {
                            bool capturing = fdp.ConsumeBool();
                            bool has_alternation = fdp.ConsumeBool();
                            
                            if (has_alternation) {
                                pattern = (capturing ? "(" : "(?:") + 
                                         part1.substr(0, std::min(part1.size(), size_t(3))) + "|" +
                                         part2.substr(0, std::min(part2.size(), size_t(3))) + "|" +
                                         part3.substr(0, std::min(part3.size(), size_t(3))) + ")";
                            } else {
                                pattern = (capturing ? "(" : "(?:") + 
                                         part1.substr(0, std::min(part1.size(), size_t(3))) +
                                         part2.substr(0, std::min(part2.size(), size_t(3))) +
                                         part3.substr(0, std::min(part3.size(), size_t(3))) + ")";
                            }
                        }
                    }
                    break;
                    
                case 5: // Anchors and word boundaries
                    {
                        std::string base = fdp.ConsumeRandomLengthString(20);
                        if (!base.empty()) {
                            std::string prefix = "";
                            std::string suffix = "";
                            
                            if (fdp.ConsumeBool()) prefix = "^";
                            if (fdp.ConsumeBool()) suffix = "$";
                            if (fdp.ConsumeBool()) {
                                if (fdp.ConsumeBool()) prefix = "\\b" + prefix;
                                else suffix = suffix + "\\b";
                            }
                            if (fdp.ConsumeBool()) {
                                if (fdp.ConsumeBool()) prefix = "\\B" + prefix;
                                else suffix = suffix + "\\B";
                            }
                            
                            pattern = prefix + base + suffix;
                        }
                    }
                    break;
                    
                case 6: // Escape sequences
                    {
                        std::string base = fdp.ConsumeRandomLengthString(16);
                        if (!base.empty()) {
                            std::string escaped;
                            int escape_type = fdp.ConsumeIntegralInRange<int>(0, 7);
                            
                            switch (escape_type) {
                                case 0: escaped = "\\d"; break;  // digit
                                case 1: escaped = "\\D"; break;  // non-digit
                                case 2: escaped = "\\s"; break;  // whitespace
                                case 3: escaped = "\\S"; break;  // non-whitespace
                                case 4: escaped = "\\w"; break;  // word character
                                case 5: escaped = "\\W"; break;  // non-word character
                                case 6: escaped = "\\n"; break;  // newline
                                case 7: escaped = "\\t"; break;  // tab
                            }
                            
                            pattern = escaped + base;
                        }
                    }
                    break;
                    
                case 7: // Complex nested expressions
                    {
                        std::string chars = fdp.ConsumeRandomLengthString(8);
                        std::string digits = fdp.ConsumeRandomLengthString(8);
                        
                        if (!chars.empty() && !digits.empty()) {
                            // Create deeply nested patterns
                            pattern = "((" + chars.substr(0, 1) + "(" + digits.substr(0, 1) + 
                                     "|" + chars.substr(std::min(chars.size(), size_t(2)), 1) + 
                                     ")*)" + digits.substr(std::min(digits.size(), size_t(2)), 1) + ")";
                            
                            // Add quantifiers
                            if (fdp.ConsumeBool()) {
                                pattern += "+";
                            }
                        }
                    }
                    break;
                    
                case 8: // Error cases (malformed patterns to test error paths)
                    {
                        int error_type = fdp.ConsumeIntegralInRange<int>(0, 6);
                        switch (error_type) {
                            case 0: pattern = "("; break;     // unclosed parenthesis
                            case 1: pattern = "["; break;     // unclosed bracket
                            case 2: pattern = "{"; break;     // unclosed brace
                            case 3: pattern = "\\"; break;    // trailing backslash
                            case 4: pattern = "*"; break;     // lone quantifier
                            case 5: pattern = "|"; break;     // lone alternation
                            case 6: pattern = ")"; break;     // unmatched close paren
                        }
                    }
                    break;
            }
            
            // Fallback if pattern is still empty
            if (pattern.empty() && fdp.remaining_bytes() > 0) {
                pattern = fdp.ConsumeRandomLengthString(16);
            }
            
            if (!pattern.empty()) {
                patterns_added.push_back(pattern);
                
                // Add pattern to set - this calls Regexp::Parse internally
                std::string error;
                int index = set->Add(pattern, &error);
                
                // Test adding duplicate patterns
                if (fdp.ConsumeBool() && !patterns_added.empty()) {
                    std::string dup_error;
                    set->Add(patterns_added[fdp.ConsumeIntegralInRange<size_t>(0, patterns_added.size()-1)], &dup_error);
                }
            }
        }
        
        // Test Set::Size before compilation
        int size_before = set->Size();
        
        // Compile the set
        bool compile_success = set->Compile();
        
        // Test Set::Size after compilation
        int size_after = set->Size();
        
        // If compilation succeeded, test matching operations
        if (compile_success) {
            // Generate various test strings from fuzzer input
            std::vector<std::string> test_strings;
            for (int t = 0; t < 5 && fdp.remaining_bytes() > 20; t++) {
                test_strings.push_back(fdp.ConsumeRandomLengthString(64));
            }
            
            // Add edge cases
            test_strings.push_back("");  // empty string
            test_strings.push_back("\n");  // newline
            test_strings.push_back("\t");  // tab
            
            // Test matching with each string
            for (const auto& test_str : test_strings) {
                std::vector<int> matches;
                bool match_result = set->Match(test_str, &matches);
                
                // Test alternative Match signature with ErrorInfo
                RE2::Set::ErrorInfo error_info;
                std::vector<int> matches_with_error;
                set->Match(test_str, &matches_with_error, &error_info);
                
                // Test without output vector
                set->Match(test_str, nullptr);
            }
            
            // Test with strings containing pattern fragments
            for (const auto& pattern : patterns_added) {
                if (pattern.size() > 3 && fdp.remaining_bytes() > 10) {
                    std::string fragment = fdp.ConsumeRandomLengthString(8) + 
                                          pattern.substr(0, std::min(pattern.size(), size_t(4))) + 
                                          fdp.ConsumeRandomLengthString(8);
                    std::vector<int> fragment_matches;
                    set->Match(fragment, &fragment_matches);
                }
            }
        }
        
        sets.push_back(std::move(set));
    }
    
    // Test additional edge cases with remaining input
    if (fdp.remaining_bytes() > 100) {
        // Test empty set
        RE2::Options default_options;
        RE2::Set empty_set(default_options, RE2::UNANCHORED);
        int empty_size = empty_set.Size();
        bool empty_compile = empty_set.Compile();
        
        if (empty_compile) {
            std::vector<int> empty_matches;
            std::string test_empty = fdp.ConsumeRandomLengthString(32);
            empty_set.Match(test_empty, &empty_matches);
        }
        
        // Test set with single very complex pattern
        RE2::Set complex_set(default_options, RE2::UNANCHORED);
        
        // Generate a complex pattern using remaining bytes
        std::string complex_pattern;
        if (fdp.remaining_bytes() > 50) {
            complex_pattern = fdp.ConsumeRandomLengthString(48);
            if (!complex_pattern.empty()) {
                // Add various regex elements
                complex_pattern = "(?i)" + complex_pattern + "[a-z]*\\d{1,3}(?:x|y|z)";
            }
        }
        
        if (!complex_pattern.empty()) {
            std::string complex_error;
            complex_set.Add(complex_pattern, &complex_error);
            complex_set.Compile();
            
            if (complex_set.Size() > 0) {
                std::vector<int> complex_matches;
                std::string complex_test = fdp.ConsumeRemainingBytesAsString();
                complex_set.Match(complex_test, &complex_matches);
            }
        }
    }
    
    return 0;
}

} // namespace harness_007
