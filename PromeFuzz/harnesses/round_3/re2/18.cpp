// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::RE2::FullMatch at re2.h:411:15 in re2.h
// re2::RE2::Consume at re2.h:445:15 in re2.h
// re2::RE2::FindAndConsume at re2.h:463:15 in re2.h
// re2::RE2::Consume at re2.h:445:15 in re2.h
// re2::RE2::FindAndConsume at re2.h:463:15 in re2.h
// re2::RE2::Consume at re2.h:445:15 in re2.h
// re2::RE2::FindAndConsume at re2.h:463:15 in re2.h
// re2::RE2::Options::set_never_capture at re2.h:721:10 in re2.h
// re2::RE2::Options::set_case_sensitive at re2.h:724:10 in re2.h
// re2::RE2::Options::set_log_errors at re2.h:709:10 in re2.h
// re2::RE2::ok at re2.h:307:8 in re2.h
// re2::RE2::NumberOfCapturingGroups at re2.h:551:7 in re2.h
// re2::RE2::Match at re2.cc:658:11 in re2.h
// re2::RE2::FullMatch at re2.h:411:15 in re2.h
// re2::RE2::FullMatch at re2.h:411:15 in re2.h
// re2::RE2::FullMatch at re2.h:411:15 in re2.h
// re2::RE2::Consume at re2.h:445:15 in re2.h
// re2::RE2::FindAndConsume at re2.h:463:15 in re2.h
// re2::RE2::Consume at re2.h:445:15 in re2.h
// re2::RE2::FindAndConsume at re2.h:463:15 in re2.h
// re2::RE2::FullMatch at re2.h:411:15 in re2.h
// re2::RE2::Consume at re2.h:445:15 in re2.h
// re2::RE2::FindAndConsume at re2.h:463:15 in re2.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include "re2/re2.h"
#include <cstdint>
#include <cstddef>
#include <cstring>
#include <string>
#include <vector>

static uint8_t dummy_buffer[4096];

extern "C" int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;
    
    // Split input into pattern and text parts
    size_t pattern_len = Data[0] % (Size - 1) + 1;
    if (pattern_len > Size - 1) pattern_len = Size - 1;
    
    std::string pattern(reinterpret_cast<const char*>(Data + 1), pattern_len);
    std::string text(reinterpret_cast<const char*>(Data + 1 + pattern_len), Size - 1 - pattern_len);
    
    // Create RE2 object with various options
    re2::RE2::Options options;
    options.set_never_capture((pattern_len % 2) == 0);
    options.set_case_sensitive((pattern_len % 3) != 0);
    options.set_log_errors(true);
    
    re2::RE2 re(pattern, options);
    if (!re.ok()) {
        // Invalid pattern, skip further processing
        return 0;
    }
    
    // Test NumberOfCapturingGroups
    int num_captures = re.NumberOfCapturingGroups();
    
    // Test Match with various parameters
    size_t startpos = (pattern_len % 4) ? 0 : (text.size() / 3);
    size_t endpos = (pattern_len % 5) ? text.size() : (text.size() * 2 / 3);
    if (endpos > text.size()) endpos = text.size();
    if (startpos > endpos) startpos = endpos;
    
    re2::RE2::Anchor anchor = (pattern_len % 6) ? re2::RE2::UNANCHORED : 
                             (pattern_len % 7) ? re2::RE2::ANCHOR_START : 
                             re2::RE2::ANCHOR_BOTH;
    
    // Prepare submatch array
    int nsubmatch = (num_captures >= 0) ? (num_captures + 1) : 0;
    if (nsubmatch > 10) nsubmatch = 10; // Limit for performance
    
    std::vector<absl::string_view> submatch(nsubmatch);
    bool match_result = re.Match(text, startpos, endpos, anchor, 
                                 submatch.data(), nsubmatch);
    
    // Test FullMatch
    std::string full_match_str1, full_match_str2;
    absl::string_view full_match_view;
    re2::RE2::FullMatch(text, re, &full_match_str1, &full_match_view, &full_match_str2);
    re2::RE2::FullMatch(text, re, static_cast<void*>(nullptr));
    re2::RE2::FullMatch(text, re);
    
    // Test Consume
    absl::string_view consume_input = text;
    std::string consume_str;
    absl::string_view consume_view;
    re2::RE2::Consume(&consume_input, re, &consume_str, &consume_view);
    
    // Reset for FindAndConsume
    absl::string_view find_input = text;
    std::string find_str;
    absl::string_view find_view;
    re2::RE2::FindAndConsume(&find_input, re, &find_str, &find_view);
    
    // Test with different argument counts based on capture groups
    if (num_captures >= 1) {
        absl::string_view input1 = text;
        std::string arg1;
        re2::RE2::Consume(&input1, re, &arg1);
        
        absl::string_view input2 = text;
        re2::RE2::FindAndConsume(&input2, re, &arg1);
        
        re2::RE2::FullMatch(text, re, &arg1);
    }
    
    if (num_captures >= 2) {
        absl::string_view input1 = text;
        std::string arg1, arg2;
        re2::RE2::Consume(&input1, re, &arg1, &arg2);
        
        absl::string_view input2 = text;
        re2::RE2::FindAndConsume(&input2, re, &arg1, &arg2);
        
        re2::RE2::FullMatch(text, re, &arg1, &arg2);
    }
    
    // Test with no capture arguments
    absl::string_view input_no_args = text;
    re2::RE2::Consume(&input_no_args, re);
    
    absl::string_view find_no_args = text;
    re2::RE2::FindAndConsume(&find_no_args, re);
    
    // Test with NULL arguments
    absl::string_view input_null = text;
    re2::RE2::Consume(&input_null, re, static_cast<void*>(nullptr));
    
    absl::string_view find_null = text;
    re2::RE2::FindAndConsume(&find_null, re, static_cast<void*>(nullptr));
    
    // Test with mixed NULL and valid arguments
    if (num_captures >= 1) {
        absl::string_view input_mixed = text;
        std::string mixed_str;
        re2::RE2::Consume(&input_mixed, re, &mixed_str, static_cast<void*>(nullptr));
        
        absl::string_view find_mixed = text;
        re2::RE2::FindAndConsume(&find_mixed, re, &mixed_str, static_cast<void*>(nullptr));
    }
    
    return 0;
}