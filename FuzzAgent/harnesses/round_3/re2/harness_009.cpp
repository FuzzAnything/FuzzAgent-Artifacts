#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <string>
#include <vector>
#include <memory>
#include "re2/re2.h"
#include "re2/set.h"

namespace harness_009 {
// Fuzzing harness for RE2::Set operations targeting deep regex parsing paths
// Primary target: RE2::Set::Add as entry point to Regexp::Parse (139 blocked branches)
// Secondary targets: RE2::Set::Compile, RE2::Set::Match (with ErrorInfo), RE2::Set::Size
// Focus: Exercise diverse regex patterns (character classes, repetitions, flags) through batch compilation



// Entry point for libFuzzer
int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check: need enough data for regex patterns and text
    if (size < 64) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Consume operation type to test different aspects of RE2::Set
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 4;

    // Consume RE2::Options settings
    RE2::Options options;
    uint8_t options_bits = fdp.ConsumeIntegral<uint8_t>();
    
    // Configure options based on fuzzed bits
    options.set_encoding((options_bits & 0x01) == 0 ? RE2::Options::EncodingUTF8 : RE2::Options::EncodingLatin1);
    options.set_posix_syntax((options_bits & 0x02) != 0);
    options.set_longest_match((options_bits & 0x04) != 0);
    options.set_log_errors((options_bits & 0x08) != 0);
    options.set_max_mem(fdp.ConsumeIntegralInRange<int>(1000, 1000000));
    options.set_literal((options_bits & 0x10) != 0);
    options.set_never_nl((options_bits & 0x20) != 0);
    options.set_dot_nl((options_bits & 0x40) != 0);
    options.set_never_capture((options_bits & 0x80) != 0);

    // Consume anchor type
    RE2::Anchor anchor = static_cast<RE2::Anchor>(fdp.ConsumeIntegralInRange<int>(0, 2));

    // Create RE2::Set instance
    RE2::Set set(options, anchor);

    // Determine number of patterns to add (1-10)
    int num_patterns = fdp.ConsumeIntegralInRange<int>(1, 10);
    std::vector<int> pattern_indices;
    
    // Add diverse regex patterns to exercise Regexp::Parse deeply
    for (int i = 0; i < num_patterns; i++) {
        if (fdp.remaining_bytes() < 4) break;
        
        // Consume pattern length (1-256 bytes)
        size_t pattern_len = fdp.ConsumeIntegralInRange<size_t>(1, std::min<size_t>(256, fdp.remaining_bytes()));
        std::string pattern = fdp.ConsumeBytesAsString(pattern_len);
        
        // Add some regex syntax diversity based on operation type
        std::string error;
        int index = set.Add(pattern, &error);
        if (index != -1) {
            pattern_indices.push_back(index);
        }
    }

    // Check set size
    int set_size = set.Size();
    
    // Only compile if we have at least one valid pattern
    if (set_size > 0) {
        bool compiled = set.Compile();
        
        if (compiled) {
            // Test matching with various input strings
            for (int match_test = 0; match_test < 3; match_test++) {
                if (fdp.remaining_bytes() < 4) break;
                
                size_t text_len = fdp.ConsumeIntegralInRange<size_t>(0, std::min<size_t>(512, fdp.remaining_bytes()));
                std::string text = fdp.ConsumeBytesAsString(text_len);
                
                std::vector<int> matches;
                RE2::Set::ErrorInfo error_info;
                
                // Test both Match overloads (with and without ErrorInfo)
                if (operation % 2 == 0) {
                    set.Match(text, &matches);
                } else {
                    set.Match(text, &matches, &error_info);
                }
                

            }
        }
    }

    // Test different operation types
    switch (operation) {
        case 0: {
            // Test with Latin1 encoding
            RE2::Options latin1_options(RE2::Latin1);
            RE2::Set latin1_set(latin1_options, RE2::UNANCHORED);
            
            if (fdp.remaining_bytes() > 10) {
                size_t pattern_len = fdp.ConsumeIntegralInRange<size_t>(1, 100);
                std::string pattern = fdp.ConsumeBytesAsString(pattern_len);
                std::string error;
                latin1_set.Add(pattern, &error);
                latin1_set.Compile();
            }
            break;
        }
        case 1: {
            // Test with POSIX syntax
            RE2::Options posix_options(RE2::POSIX);
            RE2::Set posix_set(posix_options, RE2::ANCHOR_BOTH);
            if (fdp.remaining_bytes() > 10) {
                size_t pattern_len = fdp.ConsumeIntegralInRange<size_t>(1, 100);
                std::string pattern = fdp.ConsumeBytesAsString(pattern_len);
                std::string error;
                posix_set.Add(pattern, &error);
                posix_set.Compile();
                
                if (fdp.remaining_bytes() > 5) {
                    size_t text_len = fdp.ConsumeIntegralInRange<size_t>(0, 50);
                    std::string text = fdp.ConsumeBytesAsString(text_len);
                    std::vector<int> matches;
                    posix_set.Match(text, &matches);
                }
            }
            break;
        }
        case 2: {
            // Test error cases with malformed patterns
            RE2::Set error_set(options, anchor);
            
            // Try adding potentially problematic patterns
            for (int i = 0; i < 3 && fdp.remaining_bytes() > 2; i++) {
                size_t pattern_len = fdp.ConsumeIntegralInRange<size_t>(0, 50);
                std::string pattern = fdp.ConsumeBytesAsString(pattern_len);
                std::string error;
                error_set.Add(pattern, &error);
            }
            
            // Try to compile even with potentially invalid patterns
            error_set.Compile();
            break;
        }
        case 3: {
            // Test with quiet option
            RE2::Options quiet_options(RE2::Quiet);
            RE2::Set quiet_set(quiet_options, RE2::ANCHOR_START);
            
            // Add multiple patterns
            for (int i = 0; i < 5 && fdp.remaining_bytes() > 5; i++) {
                size_t pattern_len = fdp.ConsumeIntegralInRange<size_t>(1, 30);
                std::string pattern = fdp.ConsumeBytesAsString(pattern_len);
                std::string error;
                quiet_set.Add(pattern, &error);
            }
            
            quiet_set.Compile();
            quiet_set.Size();
            
            if (fdp.remaining_bytes() > 10) {
                size_t text_len = fdp.ConsumeIntegralInRange<size_t>(1, 100);
                std::string text = fdp.ConsumeBytesAsString(text_len);
                std::vector<int> matches;
                RE2::Set::ErrorInfo error_info;
                quiet_set.Match(text, &matches, &error_info);
            }
            break;
        }
    }

    // Test move operations (available per set.h)
    if (set.Size() > 0) {
        RE2::Set moved_set(std::move(set));
        // Original set should be in valid but unspecified state
        // moved_set should have the patterns
    }

    return 0;
}

} // namespace harness_009
