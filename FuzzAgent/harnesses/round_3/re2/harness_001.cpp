#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <string>
#include <vector>
#include "re2/re2.h"
#include "re2/set.h"
#include "re2/filtered_re2.h"

namespace harness_001 {
// Fuzzing harness for RE2 advanced features and multi-pattern matching
// Targets RE2::Set, FilteredRE2, and complex matching scenarios



// Entry point for libFuzzer
int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check
    if (size < 8) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Test 1: RE2::Set with multiple patterns
    RE2::Options options;
    options.set_encoding(fdp.ConsumeBool() ? RE2::Options::EncodingLatin1 
                                           : RE2::Options::EncodingUTF8);
    options.set_log_errors(false);
    
    RE2::Anchor anchor = fdp.PickValueInArray<RE2::Anchor>({
        RE2::UNANCHORED,
        RE2::ANCHOR_START,
        RE2::ANCHOR_BOTH,
    });
    
    RE2::Set set(options, anchor);
    
    // Add multiple patterns to the set
    int num_patterns = fdp.ConsumeIntegralInRange<int>(0, 5);
    for (int i = 0; i < num_patterns && fdp.remaining_bytes() > 10; i++) {
        std::string set_pattern = fdp.ConsumeRandomLengthString(100);
        set.Add(set_pattern, /*error=*/nullptr);
    }
    
    // Compile the set if we added patterns
    if (num_patterns > 0) {
        if (set.Compile()) {
            std::string set_text = fdp.ConsumeRandomLengthString(200);
            std::vector<int> matches;
            set.Match(set_text, &matches);
        }
    }
    
    // Test 2: FilteredRE2 with pattern filtering
    re2::FilteredRE2 filter;
    
    int num_filter_patterns = fdp.ConsumeIntegralInRange<int>(0, 5);
    for (int i = 0; i < num_filter_patterns && fdp.remaining_bytes() > 10; i++) {
        std::string filter_pattern = fdp.ConsumeRandomLengthString(100);
        int index = -1;
        filter.Add(filter_pattern, options, &index);
    }
    
    // Compile the filter if we added patterns
    if (num_filter_patterns > 0) {
        std::vector<std::string> atoms;
        filter.Compile(&atoms);
        
        if (!atoms.empty()) {
            std::string filter_text = fdp.ConsumeRandomLengthString(200);
            
            // Create matched atoms (simulate some matches)
            std::vector<int> matched_atoms;
            for (size_t i = 0; i < atoms.size() && i < 3; i++) {
                if (fdp.ConsumeBool()) {
                    matched_atoms.push_back(static_cast<int>(i));
                }
            }
            
            if (!matched_atoms.empty()) {
                std::vector<int> filter_matches;
                filter.AllMatches(filter_text, matched_atoms, &filter_matches);
            }
        }
    }
    
    // Test 3: Complex RE2 matching with multiple capture groups
    if (fdp.remaining_bytes() > 100) {
        std::string pattern = fdp.ConsumeRandomLengthString(100);
        std::string text = fdp.ConsumeRemainingBytesAsString();
        
        RE2::Options match_options;
        match_options.set_encoding(fdp.ConsumeBool() ? RE2::Options::EncodingLatin1 
                                                     : RE2::Options::EncodingUTF8);
        match_options.set_log_errors(false);
        match_options.set_posix_syntax(fdp.ConsumeBool());
        match_options.set_longest_match(fdp.ConsumeBool());
        match_options.set_literal(fdp.ConsumeBool());
        match_options.set_never_nl(fdp.ConsumeBool());
        match_options.set_dot_nl(fdp.ConsumeBool());
        match_options.set_never_capture(fdp.ConsumeBool());
        match_options.set_case_sensitive(!fdp.ConsumeBool());
        match_options.set_perl_classes(fdp.ConsumeBool());
        match_options.set_word_boundary(fdp.ConsumeBool());
        match_options.set_one_line(fdp.ConsumeBool());
        
        RE2 re(pattern, match_options);
        if (re.ok()) {
            // Test various matching functions
            RE2::FullMatch(text, re);
            RE2::PartialMatch(text, re);
            
            absl::string_view text_view = text;
            RE2::Consume(&text_view, re);
            
            text_view = text;  // Reset
            RE2::FindAndConsume(&text_view, re);
            
            // Test with capture groups if present
            if (re.NumberOfCapturingGroups() > 0) {
                // Try different output types
                std::string str_result;
                int int_result;
                float float_result;
                double double_result;
                
                RE2::FullMatch(text, re, &str_result);
                RE2::PartialMatch(text, re, &int_result);
                
                text_view = text;
                RE2::Consume(&text_view, re, &float_result);
                
                text_view = text;
                RE2::FindAndConsume(&text_view, re, &double_result);
            }
            
            // Test replace operations
            std::string replace_text = text;
            RE2::Replace(&replace_text, re, "X");
            
            std::string global_replace_text = text;
            RE2::GlobalReplace(&global_replace_text, re, "Y");
            
            // Test possible match range
            std::string min_match, max_match;
            re.PossibleMatchRange(&min_match, &max_match, 50);
            
            // Test other RE2 methods
            re.NamedCapturingGroups();
            re.CapturingGroupNames();
            
            // Quote meta characters
            std::string quoted = RE2::QuoteMeta(pattern);
        }
    }
    
    // Test 4: Multiple RE2 objects with different options
    if (fdp.remaining_bytes() > 50) {
        std::string simple_pattern = fdp.ConsumeRandomLengthString(50);
        std::string simple_text = fdp.ConsumeRemainingBytesAsString();
        
        // Test with default options
        RE2 re1(simple_pattern);
        if (re1.ok()) {
            RE2::PartialMatch(simple_text, re1);
        }
        
        // Test with Latin1 encoding
        RE2::Options latin1_options;
        latin1_options.set_encoding(RE2::Options::EncodingLatin1);
        RE2 re2(simple_pattern, latin1_options);
        if (re2.ok()) {
            RE2::PartialMatch(simple_text, re2);
        }
        
        // Test with never_capture
        RE2::Options no_capture_options;
        no_capture_options.set_never_capture(true);
        RE2 re3(simple_pattern, no_capture_options);
        if (re3.ok()) {
            RE2::PartialMatch(simple_text, re3);
        }
    }
    
    return 0;
}

} // namespace harness_001
