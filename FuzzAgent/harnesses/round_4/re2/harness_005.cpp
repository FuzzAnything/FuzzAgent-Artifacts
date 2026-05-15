#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <string>
#include <vector>
#include <memory>
#include <cstring>
#include "re2/re2.h"

namespace harness_005 {
// Copyright 2024 Fuzzing Harness Generator
// Sixth harness for RE2 library - targeting regex replacement operations with array-based matching functions
// Based on coverage guidance: GlobalReplace (165 undiscovered branches), Replace (159 undiscovered branches),
// ConsumeN (132 undiscovered branches), PartialMatchN, and Extract operations




int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Require minimum input size for meaningful testing
    // Need enough for options, patterns, text, rewrite strings, and operation parameters
    if (size < 64) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Consume RE2::Options from fuzzer input
    RE2::Options options;
    options.set_encoding(fdp.ConsumeBool() ? RE2::Options::EncodingLatin1
                                          : RE2::Options::EncodingUTF8);
    options.set_posix_syntax(fdp.ConsumeBool());
    options.set_longest_match(fdp.ConsumeBool());
    options.set_log_errors(false);  // Don't log during fuzzing
    options.set_max_mem(64 << 20);  // 64 MB max memory
    options.set_literal(fdp.ConsumeBool());
    options.set_never_nl(fdp.ConsumeBool());
    options.set_dot_nl(fdp.ConsumeBool());
    options.set_never_capture(fdp.ConsumeBool());
    options.set_case_sensitive(!fdp.ConsumeBool());
    options.set_perl_classes(fdp.ConsumeBool());
    options.set_word_boundary(fdp.ConsumeBool());
    options.set_one_line(fdp.ConsumeBool());
    
    // Determine operation mode from fuzzer input
    // 0: Test with simple pattern
    // 1: Test with pattern containing capture groups
    // 2: Test with pattern containing named capture groups
    // 3: Test with multiple patterns in sequence
    int operation_mode = fdp.ConsumeIntegralInRange<int>(0, 3);
    
    // Consume pattern string(s)
    std::vector<std::string> patterns;
    std::vector<std::unique_ptr<RE2>> re2_objects;
    
    if (operation_mode == 3) {
        // Multiple patterns mode
        int num_patterns = fdp.ConsumeIntegralInRange<int>(1, 4);
        for (int i = 0; i < num_patterns; i++) {
            std::string pattern = fdp.ConsumeRandomLengthString(128);
            if (pattern.empty() && fdp.remaining_bytes() > 0) {
                pattern = fdp.ConsumeRandomLengthString(1);
            }
            if (!pattern.empty()) {
                patterns.push_back(pattern);
                auto re2_ptr = std::make_unique<RE2>(pattern, options);
                if (re2_ptr->ok()) {
                    re2_objects.push_back(std::move(re2_ptr));
                }
            }
        }
    } else {
        // Single pattern mode
        std::string pattern = fdp.ConsumeRandomLengthString(128);
        
        // Modify pattern based on operation mode
        if (operation_mode == 1 && pattern.size() > 2) {
            // Add capture groups: convert "abc" to "(a)(b)(c)" or similar
            pattern = "(" + pattern.substr(0, 1) + ")+(" + 
                     (pattern.size() > 1 ? pattern.substr(1, 1) : "x") + ")*(" + 
                     (pattern.size() > 2 ? pattern.substr(2, 1) : "y") + ")?";
        } else if (operation_mode == 2 && pattern.size() > 2) {
            // Add named capture groups
            pattern = "(?P<name1>" + pattern.substr(0, 1) + "+)(?P<name2>" + 
                     (pattern.size() > 1 ? pattern.substr(1, 1) : "x") + "*)";
        }
        
        if (pattern.empty() && fdp.remaining_bytes() > 0) {
            pattern = fdp.ConsumeRandomLengthString(1);
        }
        
        if (pattern.empty()) {
            return 0;
        }
        
        patterns.push_back(pattern);
        auto re2_ptr = std::make_unique<RE2>(pattern, options);
        if (re2_ptr->ok()) {
            re2_objects.push_back(std::move(re2_ptr));
        }
    }
    
    // If no valid RE2 objects were created, return early
    if (re2_objects.empty()) {
        return 0;
    }
    
    // Consume text to operate on
    std::string text = fdp.ConsumeRandomLengthString(512);
    if (text.empty() && fdp.remaining_bytes() > 0) {
        text = fdp.ConsumeRandomLengthString(1);
    }
    
    // Test each RE2 object
    for (size_t re_idx = 0; re_idx < re2_objects.size(); re_idx++) {
        RE2* re = re2_objects[re_idx].get();
        
        if (!re->ok()) {
            continue;  // Skip invalid patterns
        }
        
        // Test PartialMatchN - array-based matching interface
        // This is a lower-level API that the regular PartialMatch uses internally
        {
            absl::string_view text_view(text);
            
            // We'll test the public PartialMatch with multiple arguments
            // which internally calls PartialMatchN
            if (re->NumberOfCapturingGroups() > 0) {
                // Test with different combinations of argument types
                // This exercises the array-based matching infrastructure
                
                // Single string argument
                std::string out1;
                RE2::PartialMatch(text, *re, &out1);
                
                // Multiple arguments of different types
                if (re->NumberOfCapturingGroups() >= 2) {
                    std::string out2;
                    int out3;
                    RE2::PartialMatch(text, *re, &out2, &out3);
                }
                
                if (re->NumberOfCapturingGroups() >= 3) {
                    std::string out4;
                    short out5;
                    long out6;
                    RE2::PartialMatch(text, *re, &out4, &out5, &out6);
                }
            } else {
                // Test without capture groups
                bool match_result = RE2::PartialMatch(text, *re);
                (void)match_result;
            }
        }
        
        // Test ConsumeN - array-based consume interface
        {
            absl::string_view consume_view(text);
            
            if (re->NumberOfCapturingGroups() > 0) {
                // Test with different combinations of argument types
                std::string consumed1;
                RE2::Consume(&consume_view, *re, &consumed1);
                
                // Reset view and test with different arguments
                consume_view = text;
                if (re->NumberOfCapturingGroups() >= 2) {
                    std::string consumed2;
                    int consumed3;
                    RE2::Consume(&consume_view, *re, &consumed2, &consumed3);
                }
            } else {
                // Test without capture groups
                absl::string_view simple_consume_view(text);
                RE2::Consume(&simple_consume_view, *re);
            }
        }
        
        // Test RE2::Extract - string extraction with rewrite
        if (fdp.remaining_bytes() > 0) {
            std::string rewrite_for_extract = fdp.ConsumeRandomLengthString(64);
            std::string extracted;
            
            // Test Extract with the rewrite string
            bool extract_result = RE2::Extract(text, *re, rewrite_for_extract, &extracted);
            (void)extract_result;
            
            // Test with empty rewrite
            std::string extracted_empty;
            RE2::Extract(text, *re, "", &extracted_empty);
            
            // Test with rewrite containing backreferences
            if (re->NumberOfCapturingGroups() > 0) {
                std::string rewrite_with_backref = "\\1-test-\\2";
                std::string extracted_backref;
                RE2::Extract(text, *re, rewrite_with_backref, &extracted_backref);
            }
        }
        
        // Test RE2::Replace - single replacement operation
        if (fdp.remaining_bytes() > 0) {
            std::string text_for_replace = text;
            std::string rewrite_for_replace = fdp.ConsumeRandomLengthString(64);
            
            // Test Replace with various rewrite strings
            bool replace_result = RE2::Replace(&text_for_replace, *re, rewrite_for_replace);
            (void)replace_result;
            
            // Test with empty rewrite
            std::string text_for_empty_replace = text;
            RE2::Replace(&text_for_empty_replace, *re, "");
            
            // Test with rewrite containing backreferences
            if (re->NumberOfCapturingGroups() > 0) {
                std::string text_for_backref_replace = text;
                std::string rewrite_with_backref = "matched-\\1-\\2";
                RE2::Replace(&text_for_backref_replace, *re, rewrite_with_backref);
            }
        }
        
        // Test RE2::GlobalReplace - multiple replacement operations
        // Primary target: 165 undiscovered branches
        if (fdp.remaining_bytes() > 0) {
            std::string text_for_global = text;
            std::string rewrite_for_global = fdp.ConsumeRandomLengthString(64);
            
            // Test GlobalReplace with various rewrite strings
            int global_replace_count = RE2::GlobalReplace(&text_for_global, *re, rewrite_for_global);
            (void)global_replace_count;
            
            // Test with empty rewrite
            std::string text_for_empty_global = text;
            RE2::GlobalReplace(&text_for_empty_global, *re, "");
            
            // Test with rewrite containing backreferences
            if (re->NumberOfCapturingGroups() > 0) {
                std::string text_for_backref_global = text;
                std::string rewrite_with_backref = "\\1-\\2";
                RE2::GlobalReplace(&text_for_backref_global, *re, rewrite_with_backref);
            }
            
            // Test with text containing multiple matches
            std::string repetitive_text = "test test test " + text + " test test";
            std::string repetitive_text_copy = repetitive_text;
            RE2::GlobalReplace(&repetitive_text_copy, *re, rewrite_for_global);
        }
        
        // Test edge cases for replacement operations
        {
            // Test with text that's likely to match
            std::string matching_text;
            if (!patterns[re_idx].empty()) {
                // Create text that contains the pattern
                matching_text = "prefix_" + patterns[re_idx].substr(0, std::min(patterns[re_idx].size(), size_t(10))) + "_suffix";
                
                std::string matching_text_for_replace = matching_text;
                RE2::Replace(&matching_text_for_replace, *re, "replaced");
                
                std::string matching_text_for_global = matching_text;
                RE2::GlobalReplace(&matching_text_for_global, *re, "global_replaced");
            }
            
            // Test with very short text
            std::string short_text = "a";
            std::string short_text_copy = short_text;
            RE2::Replace(&short_text_copy, *re, "x");
            
            // Test with text containing special regex characters
            std::string special_text = ".*+?^${}()|[]\\";
            std::string special_text_copy = special_text;
            RE2::GlobalReplace(&special_text_copy, *re, "escaped");
        }
    }
    
    return 0;
}

} // namespace harness_005
