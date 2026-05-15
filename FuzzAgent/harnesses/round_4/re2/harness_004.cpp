#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <string>
#include <vector>
#include <map>
#include "re2/re2.h"

namespace harness_004 {
// Copyright 2024 Fuzzing Harness Generator
// Fifth harness for RE2 library - targeting RE2 internal state inspection operations
// Based on coverage guidance: 9 completely uncovered RE2 inspection APIs with significant undiscovered branch complexity




int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Require minimum input size for meaningful testing
    // Need enough for options, valid pattern, invalid pattern, and operation selection
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
    
    // Determine pattern type to generate: 0 = pattern with capturing groups, 1 = pattern with named capturing groups
    // 2 = simple pattern, 3 = invalid pattern for error testing
    int pattern_type = fdp.ConsumeIntegralInRange<int>(0, 3);
    
    std::string valid_pattern;
    
    // Generate appropriate pattern based on type
    switch (pattern_type) {
        case 0:
            // Pattern with capturing groups: e.g., "(a+)(b+)(c+)"
            valid_pattern = "(" + fdp.ConsumeRandomLengthString(8) + ")+(" + 
                           fdp.ConsumeRandomLengthString(8) + ")*(" + 
                           fdp.ConsumeRandomLengthString(8) + ")?";
            break;
        case 1:
            // Pattern with named capturing groups: e.g., "(?P<name1>a+)(?P<name2>b+)"
            valid_pattern = "(?P<" + fdp.ConsumeRandomLengthString(6) + ">" + 
                           fdp.ConsumeRandomLengthString(8) + "+)(?P<" + 
                           fdp.ConsumeRandomLengthString(6) + ">" + 
                           fdp.ConsumeRandomLengthString(8) + "*)";
            break;
        case 2:
            // Simple pattern without capturing groups
            valid_pattern = fdp.ConsumeRandomLengthString(64);
            break;
        case 3:
            // Invalid pattern (unclosed parenthesis)
            valid_pattern = "(" + fdp.ConsumeRandomLengthString(32);
            break;
    }
    
    // If pattern is empty, try to get at least something
    if (valid_pattern.empty() && fdp.remaining_bytes() > 0) {
        valid_pattern = fdp.ConsumeRandomLengthString(1);
    }
    
    // Skip if pattern is still empty
    if (valid_pattern.empty()) {
        return 0;
    }
    
    // Create RE2 object with the valid/invalid pattern
    RE2 re(valid_pattern, options);
    
    // TEST 1: Call pattern() to retrieve the pattern string
    const std::string& retrieved_pattern = re.pattern();
    (void)retrieved_pattern;  // Use to avoid unused variable warning
    
    // TEST 2: If RE2 construction succeeded, test introspection APIs
    if (re.ok()) {
        // TEST 3: Call CapturingGroupNames() to get capturing group names
        const std::map<int, std::string>& group_names = re.CapturingGroupNames();
        (void)group_names;
        
        // TEST 4: Call NamedCapturingGroups() to get named capturing groups map
        const std::map<std::string, int>& named_groups = re.NamedCapturingGroups();
        (void)named_groups;
        
        // TEST 5: Call ProgramSize() to get program size
        int program_size = re.ProgramSize();
        (void)program_size;
        
        // TEST 6: Call ProgramFanout() with vector to get program fanout
        std::vector<int> fanout_histogram;
        int fanout_result = re.ProgramFanout(&fanout_histogram);
        (void)fanout_result;
        
        // TEST 7: Call ReverseProgramSize() to get reverse program size
        int reverse_program_size = re.ReverseProgramSize();
        (void)reverse_program_size;
        
        // TEST 8: Call ReverseProgramFanout() with vector to get reverse program fanout
        std::vector<int> reverse_fanout_histogram;
        int reverse_fanout_result = re.ReverseProgramFanout(&reverse_fanout_histogram);
        (void)reverse_fanout_result;
        
        // Also test NumberOfCapturingGroups() which is related
        int num_capturing_groups = re.NumberOfCapturingGroups();
        (void)num_capturing_groups;
    } else {
        // TEST 9: Call error() to get error code (only when RE2 construction fails)
        RE2::ErrorCode error_code = re.error_code();
        (void)error_code;
        
        // TEST 10: Call error() to get error string
        const std::string& error_string = re.error();
        (void)error_string;
        
        // TEST 11: Call error_arg() to get error argument
        const std::string& error_argument = re.error_arg();
        (void)error_argument;
    }
    
    // Now test with explicitly invalid pattern if we have enough input remaining
    if (fdp.remaining_bytes() > 10) {
        // Create a known invalid pattern (unclosed parenthesis)
        std::string invalid_pattern = "(" + fdp.ConsumeRandomLengthString(16);
        RE2 invalid_re(invalid_pattern, options);
        
        // Test error APIs with the invalid RE2 object
        if (!invalid_re.ok()) {
            RE2::ErrorCode invalid_error_code = invalid_re.error_code();
            (void)invalid_error_code;
            
            const std::string& invalid_error_string = invalid_re.error();
            (void)invalid_error_string;
            
            const std::string& invalid_error_argument = invalid_re.error_arg();
            (void)invalid_error_argument;
        }
    }
    
    // Test with multiple different pattern variations if we have remaining input
    if (fdp.remaining_bytes() > 20) {
        // Test pattern with alternation
        std::string alt_pattern = fdp.ConsumeRandomLengthString(8) + "|" + 
                                  fdp.ConsumeRandomLengthString(8);
        RE2 alt_re(alt_pattern, options);
        
        if (alt_re.ok()) {
            // Test all inspection APIs on this alternate pattern
            const std::string& alt_retrieved_pattern = alt_re.pattern();
            (void)alt_retrieved_pattern;
            
            int alt_program_size = alt_re.ProgramSize();
            (void)alt_program_size;
            
            int alt_reverse_program_size = alt_re.ReverseProgramSize();
            (void)alt_reverse_program_size;
            
            std::vector<int> alt_fanout_histogram;
            alt_re.ProgramFanout(&alt_fanout_histogram);
            
            std::vector<int> alt_reverse_fanout_histogram;
            alt_re.ReverseProgramFanout(&alt_reverse_fanout_histogram);
        }
    }
    
    return 0;
}

} // namespace harness_004
