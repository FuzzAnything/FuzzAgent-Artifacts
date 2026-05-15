#include <fuzzer/FuzzedDataProvider.h>
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
#include <algorithm>
#include "re2/re2.h"

namespace harness_020 {
// RE2 complex regex pattern parsing and validation fuzzing harness
// Targets Regexp::Parse with 140/160 branches blocked (87.5% blocker rate)
// Focuses on complex regex parsing edge cases to unlock blocked branches in core parsing logic
// Semantically unique from existing harnesses by targeting deep parser state machine paths



// Helper function to generate complex regex patterns with edge cases
std::string GenerateComplexPattern(FuzzedDataProvider& fdp) {
    std::string pattern;
    
    // Consume pattern complexity level
    int complexity = fdp.ConsumeIntegralInRange<int>(1, 5);
    
    for (int i = 0; i < complexity; i++) {
        // Choose pattern component type
        int component_type = fdp.ConsumeIntegralInRange<int>(0, 12);
        
        switch (component_type) {
            case 0: // Unicode character classes and properties
                pattern += "\\p{";
                pattern += fdp.ConsumeRandomLengthString(10);
                pattern += "}";
                break;
                
            case 1: // Unicode negated character classes
                pattern += "\\P{";
                pattern += fdp.ConsumeRandomLengthString(10);
                pattern += "}";
                break;
                
            case 2: // Nested repetition operators with limits
                pattern += "(a{";
                pattern += std::to_string(fdp.ConsumeIntegralInRange<int>(0, 10));
                pattern += ",";
                pattern += std::to_string(fdp.ConsumeIntegralInRange<int>(5, 20));
                pattern += "}){";
                pattern += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 5));
                pattern += "}";
                break;
                
            case 3: // Complex character classes
                pattern += "[";
                if (fdp.ConsumeBool()) pattern += "^";
                pattern += "a-z";
                if (fdp.ConsumeBool()) pattern += "A-Z";
                if (fdp.ConsumeBool()) pattern += "0-9";
                pattern += "]";
                break;
                
            case 4: // Grouping with alternation
                pattern += "(";
                pattern += fdp.ConsumeRandomLengthString(5);
                pattern += "|";
                pattern += fdp.ConsumeRandomLengthString(5);
                pattern += ")";
                break;
                
            case 5: // Quantified groups
                pattern += "(";
                pattern += fdp.ConsumeRandomLengthString(3);
                pattern += "){";
                pattern += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 10));
                pattern += "}";
                break;
                
            case 6: // Escape sequences
                pattern += "\\";
                pattern += fdp.ConsumeRandomLengthString(1);
                break;
                
            case 7: // Anchors
                if (fdp.ConsumeBool()) pattern += "^";
                pattern += fdp.ConsumeRandomLengthString(3);
                if (fdp.ConsumeBool()) pattern += "$";
                break;
                
            case 8: // Dot and wildcards
                pattern += ".";
                if (fdp.ConsumeBool()) pattern += "*";
                if (fdp.ConsumeBool()) pattern += "+";
                if (fdp.ConsumeBool()) pattern += "?";
                break;
                
            case 9: // Perl character classes
                pattern += "\\";
                if (fdp.ConsumeBool()) pattern += "d";
                else if (fdp.ConsumeBool()) pattern += "w";
                else pattern += "s";
                break;
                
            case 10: // Negated Perl character classes
                pattern += "\\";
                if (fdp.ConsumeBool()) pattern += "D";
                else if (fdp.ConsumeBool()) pattern += "W";
                else pattern += "S";
                break;
                
            case 11: // Complex nested structures
                pattern += "(((";
                pattern += fdp.ConsumeRandomLengthString(2);
                pattern += ")*)?)+";
                break;
                
            case 12: // Invalid patterns (should fail parsing gracefully)
                if (fdp.ConsumeBool()) pattern += "[";
                if (fdp.ConsumeBool()) pattern += "{";
                if (fdp.ConsumeBool()) pattern += "(";
                pattern += fdp.ConsumeRandomLengthString(3);
                break;
        }
        
        // Add separator between components
        if (i < complexity - 1 && fdp.ConsumeBool()) {
            pattern += fdp.ConsumeRandomLengthString(1);
        }
    }
    
    return pattern;
}

// Entry point for libFuzzer
int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size: need enough for pattern generation and options
    if (size < 16) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Configure RE2 options from fuzzer input
    RE2::Options options;
    
    // Set encoding: UTF-8 (default) or Latin1 - important for Unicode parsing
    options.set_encoding(fdp.ConsumeBool() ? RE2::Options::EncodingLatin1 
                                         : RE2::Options::EncodingUTF8);
    
    // Set various options from fuzzer input to explore different parsing paths
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
    
    // Generate multiple complex patterns to stress parser state machine
    int num_patterns = fdp.ConsumeIntegralInRange<int>(1, 5);
    
    for (int i = 0; i < num_patterns && fdp.remaining_bytes() > 10; i++) {
        // Generate complex pattern with edge cases
        std::string pattern = GenerateComplexPattern(fdp);
        
        // Skip empty patterns
        if (pattern.empty()) {
            continue;
        }
        
        // Create RE2 object with the pattern and options - triggers Regexp::Parse
        RE2 re(pattern, options);
        
        // Test 1: Basic validation of parsing result
        bool ok = re.ok();
        RE2::ErrorCode error_code = re.error_code();
        std::string error_msg = re.error();
        
        // Test 2: If parsing succeeded, test additional functionality
        if (ok) {
            // Consume text to match against
            std::string text = fdp.ConsumeRandomLengthString(50);
            
            // Test various matching operations to exercise parsed pattern
            RE2::FullMatch(text, re);
            RE2::PartialMatch(text, re);
            
            // Test pattern metadata
            re.ProgramSize();
            re.ReverseProgramSize();
            re.NumberOfCapturingGroups();
            
            // Test possible match range
            std::string min_match, max_match;
            re.PossibleMatchRange(&min_match, &max_match, 10);
            
            // Test quote meta
            std::string quoted = RE2::QuoteMeta(pattern);
            
            // Test replacement with backreferences if pattern has capture groups
            if (re.NumberOfCapturingGroups() > 0) {
                std::string replace_text = text;
                RE2::Replace(&replace_text, re, "\\1");
            }
        } else {
            // Test error handling paths - important for invalid pattern parsing
            // The error should be one of the defined error codes
            if (error_code != RE2::NoError) {
                // Valid error path - parser correctly rejected invalid pattern
                // This exercises error handling branches in parse.cc
            }
        }
        
        // Test 3: Create another RE2 object with different options
        if (fdp.remaining_bytes() > 20) {
            RE2::Options options2 = options;
            options2.set_case_sensitive(!options.case_sensitive());
            options2.set_longest_match(!options.longest_match());
            
            RE2 re2(pattern, options2);
            bool ok2 = re2.ok();
            
            // Compare parsing results with different options
            if (ok != ok2) {
                // Different options caused different parsing outcomes
                // This exercises option-dependent parsing paths
            }
        }
        
        // Test 4: Generate and test a modified version of the pattern
        if (fdp.remaining_bytes() > 30 && pattern.size() > 3) {
            std::string modified_pattern = pattern;
            
            // Modify pattern in various ways
            int modification = fdp.ConsumeIntegralInRange<int>(0, 3);
            switch (modification) {
                case 0: // Add quantifier
                    modified_pattern += "*";
                    break;
                case 1: // Add grouping
                    modified_pattern = "(" + modified_pattern + ")";
                    break;
                case 2: // Add alternation
                    modified_pattern += "|" + fdp.ConsumeRandomLengthString(3);
                    break;
                case 3: // Add character class
                    modified_pattern += "[a-z]";
                    break;
            }
            
            RE2 re_modified(modified_pattern, options);
            re_modified.ok();
        }
        
        // Test 5: Test pattern combinations that stress the parser state machine
        if (fdp.remaining_bytes() > 40 && i < num_patterns - 1) {
            // Combine current pattern with next pattern using alternation
            std::string next_pattern = GenerateComplexPattern(fdp);
            if (!next_pattern.empty()) {
                std::string combined_pattern = "(" + pattern + ")|(" + next_pattern + ")";
                RE2 re_combined(combined_pattern, options);
                re_combined.ok();
            }
        }
    }
    
    // Test 6: Special edge cases for parser state machine
    if (fdp.remaining_bytes() > 50) {
        // Generate patterns specifically designed to stress the parser
        
        // Deeply nested parentheses
        std::string deep_nested;
        int depth = fdp.ConsumeIntegralInRange<int>(1, 8);
        for (int j = 0; j < depth; j++) {
            deep_nested += "(";
        }
        deep_nested += "a";
        for (int j = 0; j < depth; j++) {
            deep_nested += ")";
        }
        RE2 re_deep(deep_nested, options);
        re_deep.ok();
        
        // Complex character class with many ranges
        std::string complex_class = "[";
        if (fdp.ConsumeBool()) complex_class += "^";
        complex_class += "a-zA-Z0-9";
        if (fdp.ConsumeBool()) complex_class += "\\[\\]";
        if (fdp.ConsumeBool()) complex_class += "\\p{Alpha}";
        complex_class += "]";
        RE2 re_class(complex_class, options);
        re_class.ok();
        
        // Pattern with many alternations
        std::string many_alternations;
        int alt_count = fdp.ConsumeIntegralInRange<int>(2, 6);
        for (int j = 0; j < alt_count; j++) {
            if (j > 0) many_alternations += "|";
            many_alternations += fdp.ConsumeRandomLengthString(2);
        }
        RE2 re_alt(many_alternations, options);
        re_alt.ok();
    }
    
    return 0;
}

} // namespace harness_020
