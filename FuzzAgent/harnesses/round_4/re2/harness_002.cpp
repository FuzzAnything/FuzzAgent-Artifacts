#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <string>
#include <vector>
#include "re2/re2.h"
#include "re2/set.h"

namespace harness_002 {
// Copyright 2024 Fuzzing Harness Generator
// Third harness for RE2 library - targeting RE2::Set operations with focus on Size() function
// Based on coverage guidance: RE2::Set module with Size() at 0% coverage and set.cc at 25.68% branch coverage




int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Require minimum input size for meaningful testing
    // Need enough for options, anchor, patterns, and text
    if (size < 32) {
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
    
    // Consume anchor type (UNANCHORED, ANCHOR_START, ANCHOR_BOTH)
    RE2::Anchor anchor = fdp.PickValueInArray<RE2::Anchor>({
        RE2::UNANCHORED,
        RE2::ANCHOR_START,
        RE2::ANCHOR_BOTH,
    });
    
    // Create RE2::Set object
    RE2::Set set(options, anchor);
    
    // Call Size() immediately after creation (should be 0)
    int initial_size = set.Size();
    (void)initial_size;  // Use to avoid unused variable warning
    
    // Determine number of patterns to add (1-8 patterns)
    int num_patterns = fdp.ConsumeIntegralInRange<int>(1, 8);
    
    std::vector<std::string> patterns;
    std::vector<int> pattern_indices;
    std::vector<std::string> error_messages;
    
    // Add patterns to the set
    for (int i = 0; i < num_patterns; i++) {
        // Consume pattern string
        std::string pattern = fdp.ConsumeRandomLengthString(128);
        if (pattern.empty() && fdp.remaining_bytes() > 0) {
            // Try to get at least 1 character for pattern
            pattern = fdp.ConsumeRandomLengthString(1);
        }
        
        patterns.push_back(pattern);
        
        // Add pattern to set, capturing error message if any
        std::string error;
        int index = set.Add(pattern, &error);
        pattern_indices.push_back(index);
        
        if (!error.empty()) {
            error_messages.push_back(error);
        }
        
        // Call Size() after each addition
        int current_size = set.Size();
        (void)current_size;
    }
    
    // Call Size() after all additions
    int size_after_add = set.Size();
    (void)size_after_add;
    
    // Try to compile the set
    bool compile_success = set.Compile();
    
    // Call Size() after compilation attempt
    int size_after_compile = set.Size();
    (void)size_after_compile;
    
    // If compilation succeeded, test matching operations
    if (compile_success) {
        // Consume text to match against
        std::string text = fdp.ConsumeRandomLengthString(256);
        
        // Test Match() without ErrorInfo
        std::vector<int> matches;
        bool match_result = set.Match(text, &matches);
        (void)match_result;
        
        // Test Match() with ErrorInfo
        RE2::Set::ErrorInfo error_info;
        std::vector<int> matches_with_error;
        bool match_with_error_result = set.Match(text, &matches_with_error, &error_info);
        (void)match_with_error_result;
        
        // Test with multiple text strings if we have enough input
        if (fdp.remaining_bytes() > 0) {
            std::string text2 = fdp.ConsumeRandomLengthString(128);
            std::vector<int> matches2;
            set.Match(text2, &matches2);
            
            // Test with empty vector
            std::vector<int> empty_matches;
            set.Match(text2, &empty_matches);
            
            // Test with NULL vector (allowed according to header)
            set.Match(text2, nullptr);
        }
        
        // Test edge cases with very short and empty text
        if (fdp.remaining_bytes() > 0) {
            std::string short_text = fdp.ConsumeRandomLengthString(1);
            std::vector<int> short_matches;
            set.Match(short_text, &short_matches);
            
            // Empty string test
            std::string empty_text = "";
            std::vector<int> empty_text_matches;
            set.Match(empty_text, &empty_text_matches);
        }
    }
    
    // Test additional edge cases
    
    // 1. Test with patterns that might cause compilation to fail
    if (fdp.remaining_bytes() > 0) {
        // Create a second set with different options to test failure scenarios
        RE2::Options options2;
        options2.set_max_mem(1024);  // Very low memory limit might cause failures
        
        RE2::Set set2(options2, anchor);
        
        // Add a few patterns
        if (fdp.remaining_bytes() > 10) {
            std::string pattern1 = fdp.ConsumeRandomLengthString(50);
            std::string pattern2 = fdp.ConsumeRandomLengthString(50);
            
            std::string error1, error2;
            set2.Add(pattern1, &error1);
            set2.Add(pattern2, &error2);
            
            // Try to compile (might fail due to memory limit)
            bool compile2_success = set2.Compile();
            (void)compile2_success;
            
            // Call Size() on second set
            int set2_size = set2.Size();
            (void)set2_size;
        }
    }
    
    // 2. Test Size() consistency
    // Size should be non-negative and equal to number of successfully added patterns
    int final_size = set.Size();
    (void)final_size;
    
    return 0;
}

} // namespace harness_002
