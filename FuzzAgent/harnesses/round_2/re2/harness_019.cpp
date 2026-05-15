#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <string>
#include <vector>
#include "re2/re2.h"
#include "re2/set.h"

namespace harness_019 {
// Fuzzing harness for RE2::Set operations targeting coverage gaps
// RE2::Set has 84+ accumulated branch complexity with 45+ undiscovered branches
// Targets: RE2::Set constructor, Set::Add() patterns, Set::Compile(), 
// Set::Match() (both variants), and Set::Size()
// Focus on edge cases, error paths, and security-sensitive scenarios



int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 4) return 0;  // Need minimum input
    
    FuzzedDataProvider fdp(data, size);
    
    // Configure RE2::Options from fuzzer input
    RE2::Options options;
    
    // Consume booleans for various options
    options.set_encoding(fdp.ConsumeBool() ? RE2::Options::EncodingLatin1 
                                          : RE2::Options::EncodingUTF8);
    options.set_posix_syntax(fdp.ConsumeBool());
    options.set_longest_match(fdp.ConsumeBool());
    options.set_log_errors(false);  // Don't log errors during fuzzing
    options.set_max_mem(64 << 20);  // 64 MB max memory
    options.set_literal(fdp.ConsumeBool());
    options.set_never_nl(fdp.ConsumeBool());
    options.set_dot_nl(fdp.ConsumeBool());
    options.set_never_capture(fdp.ConsumeBool());
    options.set_case_sensitive(!fdp.ConsumeBool());  // Invert for case-insensitive
    options.set_perl_classes(fdp.ConsumeBool());
    options.set_word_boundary(fdp.ConsumeBool());
    options.set_one_line(fdp.ConsumeBool());
    
    // Consume anchor type
    RE2::Anchor anchor = fdp.PickValueInArray<RE2::Anchor>({
        RE2::UNANCHORED,
        RE2::ANCHOR_START,
        RE2::ANCHOR_BOTH,
    });
    
    // Create RE2::Set instance
    RE2::Set set(options, anchor);
    
    // Consume number of patterns to add (0-20)
    int num_patterns = fdp.ConsumeIntegralInRange<int>(0, 20);
    std::vector<int> pattern_indices;
    
    for (int i = 0; i < num_patterns && fdp.remaining_bytes() > 0; ++i) {
        // Consume pattern string
        size_t pattern_len = fdp.ConsumeIntegralInRange<size_t>(0, 256);
        std::string pattern = fdp.ConsumeBytesAsString(pattern_len);
        
        // Decide whether to provide error string pointer
        std::string* error_ptr = nullptr;
        std::string error_str;
        if (fdp.ConsumeBool() && fdp.remaining_bytes() > 0) {
            error_ptr = &error_str;
        }
        
        // Add pattern to set
        int index = set.Add(pattern, error_ptr);
        pattern_indices.push_back(index);
        
        // Test Size() after each addition
        int current_size = set.Size();
        (void)current_size;  // Prevent unused variable warning
    }
    
    // Test Size() before compilation
    int size_before_compile = set.Size();
    (void)size_before_compile;
    
    // Try to compile the set (may fail due to memory or other issues)
    bool compile_success = false;
    if (fdp.ConsumeBool()) {
        compile_success = set.Compile();
    } else {
        // Sometimes don't compile to test error paths in Match()
        compile_success = false;
    }
    
    if (compile_success) {
        // Test Size() after compilation
        int size_after_compile = set.Size();
        (void)size_after_compile;
        
        // Consume text to match against
        size_t text_len = fdp.ConsumeIntegralInRange<size_t>(0, 1024);
        std::string text;
        if (text_len > 0 && fdp.remaining_bytes() >= text_len) {
            text = fdp.ConsumeBytesAsString(text_len);
        } else {
            text = fdp.ConsumeRemainingBytesAsString();
        }
        
        // Test both Match() variants
        
        // Variant 1: Match without ErrorInfo
        if (fdp.ConsumeBool()) {
            std::vector<int> matches1;
            bool match_result1 = set.Match(text, &matches1);
            (void)match_result1;
            
            // Also test with NULL vector
            bool match_result2 = set.Match(text, nullptr);
            (void)match_result2;
        }
        
        // Variant 2: Match with ErrorInfo
        if (fdp.ConsumeBool()) {
            RE2::Set::ErrorInfo error_info;
            std::vector<int> matches3;
            bool match_result3 = set.Match(text, &matches3, &error_info);
            (void)match_result3;
            (void)error_info.kind;
            
            // Test with NULL vector but valid ErrorInfo
            RE2::Set::ErrorInfo error_info2;
            bool match_result4 = set.Match(text, nullptr, &error_info2);
            (void)match_result4;
            (void)error_info2.kind;
            
            // Test with NULL ErrorInfo (should work like variant 1)
            std::vector<int> matches5;
            bool match_result5 = set.Match(text, &matches5, nullptr);
            (void)match_result5;
        }
        
        // Test edge cases with empty text
        if (fdp.ConsumeBool()) {
            std::vector<int> empty_matches;
            set.Match("", &empty_matches);
        }
        
        // Test with very long text (potential buffer overflow scenarios)
        if (fdp.ConsumeBool() && fdp.remaining_bytes() > 0) {
            size_t long_text_len = fdp.ConsumeIntegralInRange<size_t>(1024, 4096);
            if (fdp.remaining_bytes() >= long_text_len) {
                std::string long_text = fdp.ConsumeBytesAsString(long_text_len);
                std::vector<int> long_matches;
                set.Match(long_text, &long_matches);
            }
        }
    } else {
        // Test error paths when set is not compiled
        if (fdp.ConsumeBool() && fdp.remaining_bytes() > 0) {
            std::string error_text = fdp.ConsumeRandomLengthString(100);
            std::vector<int> error_matches;
            
            // These should fail or trigger asserts (in debug mode)
            // but we need to handle gracefully
            #ifndef NDEBUG
            // In debug builds, these might assert
            #else
            // In release builds, try to call anyway
            set.Match(error_text, &error_matches);
            #endif
        }
    }
    
    // Test adding patterns after compilation (should fail)
    if (compile_success && fdp.ConsumeBool() && fdp.remaining_bytes() > 0) {
        std::string post_compile_pattern = fdp.ConsumeRandomLengthString(50);
        std::string post_compile_error;
        
        // This should return -1 or trigger debug assert
        int post_index = set.Add(post_compile_pattern, &post_compile_error);
        (void)post_index;
    }
    // Test with special patterns that might trigger security issues
    // based on the known bug in RE2::Set::Compile
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 0) {
        // Create a new set for security testing
        RE2::Set security_set(options, anchor);
        
        // Consume number of special patterns to add
        int num_special_patterns = fdp.ConsumeIntegralInRange<int>(0, 10);
        
        for (int i = 0; i < num_special_patterns && fdp.remaining_bytes() > 0; ++i) {
            // Consume pattern type to generate different problematic patterns
            uint8_t pattern_type = fdp.ConsumeIntegral<uint8_t>() % 10;
            std::string pattern;
            
            switch (pattern_type) {
                case 0:
                    pattern = ".*";  // Wildcard
                    break;
                case 1:
                    pattern = ".+";  // One or more wildcard
                    break;
                case 2:
                    pattern = ".?";  // Optional wildcard
                    break;
                case 3:
                    // Variable repetition
                    pattern = ".{" + std::to_string(fdp.ConsumeIntegralInRange<int>(0, 100)) + "}";
                    break;
                case 4:
                    // Unicode property class (simplified)
                    pattern = fdp.ConsumeBool() ? "\\p{Any}" : "\\P{Any}";
                    break;
                case 5:
                    // Large repetitions
                    pattern = "\\d{" + std::to_string(fdp.ConsumeIntegralInRange<int>(10, 200)) + ",}";
                    break;
                case 6:
                    // Mode modifiers
                    pattern = fdp.ConsumeBool() ? "(?i).*" : "(?s).*";
                    break;
                case 7:
                    // Backreference
                    pattern = "\\" + std::to_string(fdp.ConsumeIntegralInRange<int>(1, 9));
                    break;
                case 8:
                    // Incomplete character class
                    pattern = fdp.PickValueInArray<std::string>({"[[:", "[=", "[."});
                    break;
                case 9:
                default:
                    // Random pattern from input
                    size_t pattern_len = fdp.ConsumeIntegralInRange<size_t>(1, 100);
                    pattern = fdp.ConsumeBytesAsString(pattern_len);
                    break;
            }
            
            if (fdp.ConsumeBool()) {
                security_set.Add(pattern, nullptr);
            }
        }
        
        // Try to compile (might trigger the known bug)
        if (fdp.ConsumeBool()) {
            security_set.Compile();
        }
    }
    
    // Test with patterns that have null bytes
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 0) {
        RE2::Set null_set(options, anchor);
        
        // Create pattern with null bytes
        std::string null_pattern;
        size_t pattern_size = fdp.ConsumeIntegralInRange<size_t>(1, 100);
        for (size_t i = 0; i < pattern_size && fdp.remaining_bytes() > 0; ++i) {
            uint8_t byte = fdp.ConsumeIntegral<uint8_t>();
            null_pattern.push_back(static_cast<char>(byte));
        }
        
        null_set.Add(null_pattern, nullptr);
        
        if (fdp.ConsumeBool()) {
            null_set.Compile();
            
            // Test matching with text containing null bytes
            std::string null_text;
            size_t text_size = fdp.ConsumeIntegralInRange<size_t>(1, 100);
            for (size_t i = 0; i < text_size && fdp.remaining_bytes() > 0; ++i) {
                uint8_t byte = fdp.ConsumeIntegral<uint8_t>();
                null_text.push_back(static_cast<char>(byte));
            }
            
            std::vector<int> null_matches;
            null_set.Match(null_text, &null_matches);
        }
    }
    
    return 0;
}

} // namespace harness_019
