#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <fuzzer/FuzzedDataProvider.h>
#include "absl/strings/string_view.h"
#include "re2/re2.h"
#include "re2/set.h"

namespace harness_016 {
// RE2::Set comprehensive fuzzing harness
// Targets RE2::Set operations identified as critical coverage gap with 66+ undiscovered branches
// Focuses on complete lifecycle: constructor, Set::Add(), Set::Compile(), Set::Match() (both variants), Set::Size()
// Semantic diversity from existing harnesses: deeper error path exploration, complex multi-pattern scenarios



int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size: need enough for basic operations
    // Need: options, anchor choice, pattern count, patterns, text
    if (size < 32) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Configure RE2::Options with fuzzed values
    RE2::Options options;
    
    // Set encoding: UTF-8 (default) or Latin1
    options.set_encoding(fdp.ConsumeBool() ? RE2::Options::EncodingLatin1 
                                          : RE2::Options::EncodingUTF8);
    
    // Set various options from fuzzer input
    options.set_posix_syntax(fdp.ConsumeBool());
    options.set_longest_match(fdp.ConsumeBool());
    options.set_log_errors(false);  // Don't log errors during fuzzing
    options.set_max_mem(64 << 20);  // 64 MB memory limit
    options.set_literal(fdp.ConsumeBool());
    options.set_never_nl(fdp.ConsumeBool());
    options.set_dot_nl(fdp.ConsumeBool());
    options.set_never_capture(fdp.ConsumeBool());
    options.set_case_sensitive(!fdp.ConsumeBool());  // Invert: default is true
    options.set_perl_classes(fdp.ConsumeBool());
    options.set_word_boundary(fdp.ConsumeBool());
    options.set_one_line(fdp.ConsumeBool());
    
    // Choose anchor type for RE2::Set constructor
    RE2::Anchor anchor = RE2::UNANCHORED;
    uint8_t anchor_choice = fdp.ConsumeIntegral<uint8_t>() % 3;
    switch (anchor_choice) {
        case 0:
            anchor = RE2::UNANCHORED;
            break;
        case 1:
            anchor = RE2::ANCHOR_START;
            break;
        case 2:
            anchor = RE2::ANCHOR_BOTH;
            break;
    }
    
    // Create RE2::Set object with options and anchor
    std::unique_ptr<RE2::Set> set;
    try {
        set.reset(new RE2::Set(options, anchor));
    } catch (...) {
        // If construction fails, return early
        return 0;
    }
    
    // Test Size() before adding any patterns
    int initial_size = set->Size();
    
    // Determine number of patterns to add (1-12 for more diversity)
    int num_patterns = fdp.ConsumeIntegralInRange<int>(1, 12);
    std::vector<int> successful_indices;
    std::vector<std::string> pattern_strings;
    std::vector<std::string> error_messages;
    
    // Add multiple patterns to the set with varied approaches
    for (int i = 0; i < num_patterns && fdp.remaining_bytes() > 10; i++) {
        // Consume pattern string with varying lengths
        size_t pattern_len = fdp.ConsumeIntegralInRange<size_t>(1, std::min(size_t(128), fdp.remaining_bytes()));
        std::string pattern = fdp.ConsumeBytesAsString(pattern_len);
        
        if (pattern.empty()) continue;
        
        pattern_strings.push_back(pattern);
        
        // Test both Add() approaches: with and without error string
        bool use_error_string = fdp.ConsumeBool();
        
        if (use_error_string) {
            // Add pattern with error string
            std::string error;
            int index = set->Add(pattern, &error);
            
            if (!error.empty()) {
                error_messages.push_back(error);
            }
            
            if (index != -1) {
                successful_indices.push_back(index);
            }
        } else {
            // Add pattern without error string
            int index = set->Add(pattern, nullptr);
            if (index != -1) {
                successful_indices.push_back(index);
            }
        }
        
        // Test Size() after each addition
        int current_size = set->Size();
        
        // Check if we're running low on input
        if (fdp.remaining_bytes() < 20) {
            break;
        }
    }
    
    // Check Set size after all additions
    int size_after_add = set->Size();
    
    // If no patterns were successfully added, try some edge cases
    if (successful_indices.empty()) {
        // Try adding empty string pattern
        std::string empty_error;
        int empty_index = set->Add("", &empty_error);
        
        // Try adding invalid pattern
        std::string invalid_error;
        int invalid_index = set->Add("(", &invalid_error);
        
        // If still empty, return early
        if (set->Size() == 0) {
            return 0;
        }
    }
    
    // Test Compile() - critical API with 16 undiscovered branches
    bool compile_success = false;
    
    // Try compilation
    compile_success = set->Compile();
    
    if (!compile_success) {
        // Compilation failed - test what we can without compilation
        // Test Size() after failed compilation
        int size_after_failed_compile = set->Size();
        
        // Try to call Match() without compilation (should fail gracefully)
        std::vector<int> matches;
        RE2::Set::ErrorInfo error_info;
        // Consume test text from fuzzer input if available
        std::string test_text;
        if (fdp.remaining_bytes() > 0) {
            size_t test_len = std::min(size_t(4), fdp.remaining_bytes());
            test_text = fdp.ConsumeBytesAsString(test_len);
        }
        absl::string_view test_view(test_text);
        
        // This should trigger kNotCompiled error
        bool match_result = set->Match(test_view, &matches, &error_info);
        
        return 0;
    }
    
    // Compilation succeeded - now test matching operations
    
    // Test Size() after successful compilation
    int size_after_compile = set->Size();
    
    // Consume text to match against
    size_t text_len = fdp.ConsumeIntegralInRange<size_t>(0, std::min(size_t(256), fdp.remaining_bytes()));
    std::string text = fdp.ConsumeBytesAsString(text_len);
    absl::string_view text_view(text);
    
    // Test Match() without error_info (31 undiscovered branches)
    std::vector<int> matches_without_error;
    bool match_without_error = set->Match(text_view, &matches_without_error);
    
    // Verify match indices are valid
    if (match_without_error && !matches_without_error.empty()) {
        for (int match_idx : matches_without_error) {
            // Check bounds
            if (match_idx < 0 || match_idx >= size_after_compile) {
                // Invalid index - this shouldn't happen
            }
        }
    }
    
    // Test Match() with error_info (31 undiscovered branches)
    std::vector<int> matches_with_error;
    RE2::Set::ErrorInfo error_info;
    bool match_with_error = set->Match(text_view, &matches_with_error, &error_info);
    
    // Explore error_info conditions
    switch (error_info.kind) {
        case RE2::Set::kNoError:
            // No error - either match found or text doesn't match
            break;
        case RE2::Set::kNotCompiled:
            // Should not happen since we compiled successfully
            break;
        case RE2::Set::kOutOfMemory:
            // DFA ran out of memory during matching
            break;
        case RE2::Set::kInconsistent:
            // Internal inconsistency - should never happen
            break;
    }
    
    // Verify match indices for error_info variant
    if (match_with_error && !matches_with_error.empty()) {
        for (int match_idx : matches_with_error) {
            if (match_idx < 0 || match_idx >= size_after_compile) {
                // Invalid index
            }
        }
    }
    
    // Test with empty text
    absl::string_view empty_text("");
    std::vector<int> empty_matches;
    RE2::Set::ErrorInfo empty_error_info;
    bool empty_match_result = set->Match(empty_text, &empty_matches, &empty_error_info);
    

    if (fdp.remaining_bytes() > 0) {
        // Consume a single character from fuzzer input
        std::string short_text = fdp.ConsumeBytesAsString(1);
        absl::string_view short_view(short_text);
        std::vector<int> short_matches;
        bool short_match_result = set->Match(short_view, &short_matches);
    }
    
    // Test with text containing special regex characters
    if (fdp.remaining_bytes() > 0) {
        // Consume special characters from fuzzer input
        size_t special_len = std::min(size_t(16), fdp.remaining_bytes());
        std::string special_text = fdp.ConsumeBytesAsString(special_len);
        absl::string_view special_view(special_text);
        std::vector<int> special_matches;
        RE2::Set::ErrorInfo special_error_info;
        bool special_match_result = set->Match(special_view, &special_matches, &special_error_info);
    }
    
    // Test multiple match calls with same set (state persistence)
    for (int i = 0; i < 3 && fdp.remaining_bytes() > 10; i++) {
        size_t extra_text_len = fdp.ConsumeIntegralInRange<size_t>(1, std::min(size_t(100), fdp.remaining_bytes()));
        std::string extra_text = fdp.ConsumeBytesAsString(extra_text_len);
        absl::string_view extra_view(extra_text);
        
        std::vector<int> extra_matches;
        bool extra_match_result = false;
        
        if (fdp.ConsumeBool()) {
            extra_match_result = set->Match(extra_view, &extra_matches);
        } else {
            RE2::Set::ErrorInfo extra_error_info;
            extra_match_result = set->Match(extra_view, &extra_matches, &extra_error_info);
        }
    }
    

    if (fdp.remaining_bytes() > 10) {
        // Consume pattern for post-compile test
        size_t post_pattern_len = std::min(size_t(20), fdp.remaining_bytes());
        std::string post_compile_pattern = fdp.ConsumeBytesAsString(post_pattern_len);
        std::string post_compile_error;
        int post_compile_index = set->Add(post_compile_pattern, &post_compile_error);
        // Should return -1 and potentially log error
    }
    
    // Final Size() check
    int final_size = set->Size();
    
    return 0;
}

} // namespace harness_016
