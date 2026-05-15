#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <string>
#include <vector>
#include "re2/re2.h"
#include "re2/set.h"

namespace harness_000 {
// Copyright 2024 Fuzzing Harness Generator
// First harness for RE2 library - targeting core regex operations
// Based on patterns from existing re2_fuzzer.cc




int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Require minimum input size for meaningful testing
    if (size < 4) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Consume basic options from fuzzer input
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
    
    // Consume pattern and text from remaining input
    std::string pattern = fdp.ConsumeRandomLengthString(256);
    std::string text = fdp.ConsumeRemainingBytesAsString();
    
    // Skip if pattern is empty
    if (pattern.empty()) {
        return 0;
    }
    
    // Create RE2 object with the pattern
    RE2 re(pattern, options);
    
    // If RE2 construction failed, return early
    if (!re.ok()) {
        return 0;
    }
    
    // Test basic matching operations
    
    // 1. FullMatch - exact match of entire text
    bool full_match_result = RE2::FullMatch(text, re);
    (void)full_match_result;  // Use result to avoid unused variable warning
    
    // 2. PartialMatch - match anywhere in text
    bool partial_match_result = RE2::PartialMatch(text, re);
    (void)partial_match_result;
    
    // 3. Consume - match at beginning of text and advance
    absl::string_view consume_view(text);
    bool consume_result = RE2::Consume(&consume_view, re);
    (void)consume_result;
    
    // 4. FindAndConsume - find match anywhere and advance
    absl::string_view find_consume_view(text);
    bool find_consume_result = RE2::FindAndConsume(&find_consume_view, re);
    (void)find_consume_result;
    
    // Test with capturing groups if pattern has them
    if (re.NumberOfCapturingGroups() > 0) {
        // Try to extract different types of values
        absl::string_view capture_view(text);
        
        // Try extracting a string
        std::string extracted_string;
        RE2::FullMatch(capture_view, re, &extracted_string);
        
        // Try extracting integers
        int extracted_int;
        RE2::PartialMatch(text, re, &extracted_int);
        
        // Try extracting other numeric types
        short extracted_short;
        long extracted_long;
        float extracted_float;
        double extracted_double;
        
        // Reset view for each operation
        capture_view = text;
        RE2::Consume(&capture_view, re, &extracted_short);
        
        capture_view = text;
        RE2::FindAndConsume(&capture_view, re, &extracted_long);
        
        capture_view = text;
        RE2::FullMatch(capture_view, re, &extracted_float);
        
        capture_view = text;
        RE2::PartialMatch(capture_view, re, &extracted_double);
    }
    
    // Test replacement operations
    std::string replace_text = text;
    RE2::Replace(&replace_text, re, "");
    
    std::string global_replace_text = text;
    RE2::GlobalReplace(&global_replace_text, re, "");
    
    // Test PossibleMatchRange API
    std::string min_match, max_match;
    re.PossibleMatchRange(&min_match, &max_match, 50);
    
    // Test QuoteMeta API
    std::string quoted = RE2::QuoteMeta(pattern);
    
    // Test Set API - regex set matching
    RE2::Set set(options, RE2::UNANCHORED);
    int index = set.Add(pattern, nullptr);  // -1 on error
    if (index != -1 && set.Compile()) {
        std::vector<int> matches;
        set.Match(text, &matches);
    }
    
    return 0;
}

} // namespace harness_000
