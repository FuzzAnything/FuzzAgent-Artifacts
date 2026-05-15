#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <string>
#include <vector>
#include <memory>
#include "re2/re2.h"
#include "re2/set.h"

namespace harness_010 {
// Fuzzing harness for RE2 library targeting deep regex parsing and matching scenarios
// Primary target: Regexp::Parse (139 blocked branches) via RE2::Set::Add
// Secondary target: RE2::Match (114 blocked branches) via RE2::Set::Match
// Focus: Exercise blocked branches in Parse function with complex regex patterns and various flags



// Helper function to generate complex regex patterns with specific syntax elements
std::string GenerateComplexPattern(FuzzedDataProvider& fdp, uint8_t pattern_type) {
    std::string pattern;
    
    switch (pattern_type % 12) {
        case 0: // Latin1 patterns (single-byte characters)
            pattern = fdp.ConsumeRandomLengthString(50);
            break;
            
        case 1: // Nested parentheses with Perl flags
            pattern = "(?i)" + fdp.ConsumeRandomLengthString(20) + 
                      "((?:[a-z]+)?(?:" + fdp.ConsumeRandomLengthString(10) + "))";
            break;
            
        case 2: // Character classes with ranges
            pattern = "[A-Za-z0-9_" + fdp.ConsumeRandomLengthString(5) + 
                      "]{1," + std::to_string(fdp.ConsumeIntegralInRange<int>(1, 10)) + "}";
            break;
            
        case 3: // Anchors with various positions
            pattern = "^" + fdp.ConsumeRandomLengthString(10) + 
                      "$|\\b" + fdp.ConsumeRandomLengthString(10) + "\\b";
            break;
            
        case 4: // Alternation operators
            pattern = fdp.ConsumeRandomLengthString(8) + "|" + 
                      fdp.ConsumeRandomLengthString(8) + "|" + 
                      fdp.ConsumeRandomLengthString(8);
            break;
            
        case 5: // Quantifiers with edge cases
            pattern = fdp.ConsumeRandomLengthString(5) + "+?" + 
                      fdp.ConsumeRandomLengthString(5) + "*?" + 
                      fdp.ConsumeRandomLengthString(5) + "??";
            break;
            
        case 6: // Non-capturing groups with NeverCapture flag
            pattern = "(?:" + fdp.ConsumeRandomLengthString(15) + ")" + 
                      "(?!" + fdp.ConsumeRandomLengthString(10) + ")";
            break;
            
        case 7: // Complex character class combinations
            pattern = "[[:alnum:]]+" + fdp.ConsumeRandomLengthString(5) + 
                      "[^A-Z]+" + fdp.ConsumeRandomLengthString(5);
            break;
            
        case 8: // Backslash escapes
            pattern = "\\d+\\s+" + fdp.ConsumeRandomLengthString(10) + 
                      "\\w*\\b" + fdp.ConsumeRandomLengthString(5);
            break;
            
        case 9: // Repetition with bounds
            pattern = fdp.ConsumeRandomLengthString(6) + "{" + 
                      std::to_string(fdp.ConsumeIntegralInRange<int>(0, 5)) + "," + 
                      std::to_string(fdp.ConsumeIntegralInRange<int>(5, 20)) + "}";
            break;
            
        case 10: // Dot patterns with newline flags
            pattern = "." + fdp.ConsumeRandomLengthString(8) + 
                      ".*?" + fdp.ConsumeRandomLengthString(8);
            break;
            
        case 11: // Mixed syntax
            pattern = "^(" + fdp.ConsumeRandomLengthString(5) + 
                      "){2,5}[abc]|" + fdp.ConsumeRandomLengthString(8);
            break;
    }
    
    return pattern;
}

// Entry point for libFuzzer
int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check: need enough data for patterns, flags, and text
    if (size < 100) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Phase 1: Configure RE2::Options with various flag combinations
    // Targeting specific Parse paths mentioned in coverage guidance
    
    RE2::Options options;
    
    // Consume option flags from fuzzer input to target specific Parse paths
    uint16_t flags = fdp.ConsumeIntegral<uint16_t>();
    
    // Target: Latin1 vs UTF-8 encoding paths (line 2279)
    options.set_encoding((flags & 0x0001) ? RE2::Options::EncodingLatin1 : RE2::Options::EncodingUTF8);
    
    // Target: Literal flag paths (line 2286)
    options.set_literal((flags & 0x0002) != 0);
    
    // Target: NeverCapture flag (line 2319)
    options.set_never_capture((flags & 0x0004) != 0);
    
    // Other flags to explore different parsing paths
    options.set_posix_syntax((flags & 0x0008) != 0);
    options.set_longest_match((flags & 0x0010) != 0);
    options.set_log_errors((flags & 0x0020) != 0);
    options.set_never_nl((flags & 0x0040) != 0);
    options.set_dot_nl((flags & 0x0080) != 0);
    options.set_case_sensitive((flags & 0x0100) == 0); // Inverted: default is true
    options.set_perl_classes((flags & 0x0200) != 0);
    options.set_word_boundary((flags & 0x0400) != 0);
    options.set_one_line((flags & 0x0800) != 0);
    
    // Set memory limit
    options.set_max_mem(fdp.ConsumeIntegralInRange<int64_t>(1000, 10000000));
    
    // Consume anchor type
    RE2::Anchor anchor = static_cast<RE2::Anchor>(fdp.ConsumeIntegralInRange<int>(0, 2));
    
    // Create RE2::Set instance
    RE2::Set set(options, anchor);
    
    // Phase 2: Add complex regex patterns to exercise Parse blocked branches
    
    // Determine number of patterns to add (1-20)
    int num_patterns = fdp.ConsumeIntegralInRange<int>(1, 20);
    std::vector<int> valid_pattern_indices;
    std::vector<std::string> pattern_errors;
    
    for (int i = 0; i < num_patterns; i++) {
        if (fdp.remaining_bytes() < 10) break;
        
        // Generate pattern type to target specific syntax combinations
        uint8_t pattern_type = fdp.ConsumeIntegral<uint8_t>();
        
        // Generate complex pattern
        std::string pattern = GenerateComplexPattern(fdp, pattern_type);
        
        // Ensure pattern is not empty
        if (pattern.empty()) {
            pattern = ".*";
        }
        
        // Add pattern to set (triggers Regexp::Parse)
        std::string error;
        int index = set.Add(pattern, &error);
        
        if (index != -1) {
            valid_pattern_indices.push_back(index);
        } else if (!error.empty()) {
            pattern_errors.push_back(error);
        }
    }
    
    // Check set size
    int set_size = set.Size();
    
    // Phase 3: Compile set if we have valid patterns
    bool compiled = false;
    if (set_size > 0) {
        compiled = set.Compile();
    }
    
    // Phase 4: Test matching to exercise RE2::Match blocked branches
    if (compiled) {
        // Test with various text inputs
        int num_match_tests = fdp.ConsumeIntegralInRange<int>(1, 10);
        
        for (int test = 0; test < num_match_tests; test++) {
            if (fdp.remaining_bytes() < 5) break;
            
            // Generate text for matching
            size_t text_len = fdp.ConsumeIntegralInRange<size_t>(0, std::min<size_t>(1024, fdp.remaining_bytes()));
            std::string text = fdp.ConsumeBytesAsString(text_len);
            
            if (text.empty()) {
                text = "test";
            }
            
            // Test both Match overloads
            std::vector<int> matches;
            RE2::Set::ErrorInfo error_info;
            
            // Alternate between Match with and without ErrorInfo
            if (test % 2 == 0) {
                set.Match(text, &matches);
            } else {
                set.Match(text, &matches, &error_info);
            }
            
            // Also test with different anchor types if we have enough patterns
            if (!matches.empty() && fdp.remaining_bytes() > 10) {
                // Test partial matching by using substring
                size_t start_pos = fdp.ConsumeIntegralInRange<size_t>(0, text.length() / 2);
                size_t end_pos = fdp.ConsumeIntegralInRange<size_t>(start_pos, text.length());
                
                if (end_pos > start_pos) {
                    std::string substring = text.substr(start_pos, end_pos - start_pos);
                    std::vector<int> substring_matches;
                    set.Match(substring, &substring_matches);
                }
            }
        }
    }
    
    // Phase 5: Test edge cases and error conditions
    
    // Test with empty text
    if (compiled) {
        std::vector<int> empty_matches;
        set.Match("", &empty_matches);
    }
    
    // Test with very long text (if we have enough input)
    if (compiled && fdp.remaining_bytes() > 100) {
        std::string long_text = fdp.ConsumeRemainingBytesAsString();
        if (!long_text.empty()) {
            std::vector<int> long_matches;
            set.Match(long_text, &long_matches);
        }
    }
    
    // Phase 6: Test move semantics of RE2::Set (if we have another set)
    if (fdp.remaining_bytes() > 50) {
        RE2::Options options2;
        options2.set_encoding(fdp.ConsumeBool() ? RE2::Options::EncodingLatin1 : RE2::Options::EncodingUTF8);
        options2.set_never_capture(fdp.ConsumeBool());
        
        RE2::Set set2(options2, RE2::ANCHOR_BOTH);
        
        // Add a simple pattern
        std::string simple_pattern = fdp.ConsumeRandomLengthString(20);
        if (simple_pattern.empty()) simple_pattern = ".*";
        
        std::string error2;
        set2.Add(simple_pattern, &error2);
        
        if (set2.Size() > 0) {
            set2.Compile();
            
            // Test move assignment
            RE2::Set moved_set = std::move(set2);
            
            // Test matching on moved set
            std::vector<int> moved_matches;
            moved_set.Match("test", &moved_matches);
        }
    }
    
    return 0;
}

} // namespace harness_010
