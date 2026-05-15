// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::RE2::FindAndConsume at re2.h:463:15 in re2.h
// re2::RE2::FindAndConsume at re2.h:463:15 in re2.h
// re2::RE2::FindAndConsume at re2.h:463:15 in re2.h
// re2::RE2::FullMatchN at re2.cc:418:11 in re2.h
// re2::RE2::FullMatchN at re2.cc:418:11 in re2.h
// re2::RE2::FullMatchN at re2.cc:418:11 in re2.h
// re2::RE2::Arg::Parse at re2.h:920:8 in re2.h
// re2::RE2::Arg::Parse at re2.h:920:8 in re2.h
// re2::RE2::Arg::Parse at re2.h:920:8 in re2.h
// re2::RE2::Options::set_encoding at re2.h:700:10 in re2.h
// re2::RE2::ok at re2.h:307:8 in re2.h
// re2::RE2::Consume at re2.h:445:15 in re2.h
// re2::RE2::Consume at re2.h:445:15 in re2.h
// re2::RE2::Consume at re2.h:445:15 in re2.h
// re2::RE2::Consume at re2.h:445:15 in re2.h
// re2::RE2::Consume at re2.h:445:15 in re2.h
// re2::RE2::FullMatch at re2.h:411:15 in re2.h
// re2::RE2::FullMatch at re2.h:411:15 in re2.h
// re2::RE2::FullMatch at re2.h:411:15 in re2.h
// re2::RE2::FullMatch at re2.h:411:15 in re2.h
// re2::RE2::FullMatch at re2.h:411:15 in re2.h
// re2::RE2::FullMatch at re2.h:411:15 in re2.h
// re2::RE2::Match at re2.cc:658:11 in re2.h
// re2::RE2::Match at re2.cc:658:11 in re2.h
// re2::RE2::Match at re2.cc:658:11 in re2.h
// re2::RE2::Match at re2.cc:658:11 in re2.h
// re2::RE2::Match at re2.cc:658:11 in re2.h
// re2::RE2::Match at re2.cc:658:11 in re2.h
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
#include <string>
#include <optional>

extern "C" int LLVMFuzzerTestOneInput_43(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    // Split input into pattern and text parts
    size_t pattern_len = Size / 2;
    size_t text_len = Size - pattern_len;
    
    std::string pattern_str(reinterpret_cast<const char*>(Data), pattern_len);
    std::string text_str(reinterpret_cast<const char*>(Data + pattern_len), text_len);
    
    absl::string_view text_view(text_str);
    
    // Try to create RE2 object with default options
    re2::RE2::Options options;
    options.set_encoding(re2::RE2::Options::EncodingUTF8);
    
    re2::RE2 re(pattern_str, options);
    if (!re.ok()) {
        return 0;
    }
    
    // Test RE2::Consume
    {
        absl::string_view input_consume = text_view;
        std::string captured_str;
        absl::string_view captured_view;
        int captured_int = 0;
        std::optional<int> captured_opt;
        
        // Try different combinations of capture arguments
        re2::RE2::Consume(&input_consume, re, &captured_str);
        re2::RE2::Consume(&input_consume, re, &captured_view);
        re2::RE2::Consume(&input_consume, re, &captured_int);
        re2::RE2::Consume(&input_consume, re, &captured_opt);
        re2::RE2::Consume(&input_consume, re, nullptr);
    }
    
    // Test RE2::FullMatch
    {
        std::string full_captured_str;
        absl::string_view full_captured_view;
        int full_captured_int = 0;
        double full_captured_double = 0.0;
        std::optional<std::string> full_captured_opt_str;
        
        re2::RE2::FullMatch(text_view, re, &full_captured_str);
        re2::RE2::FullMatch(text_view, re, &full_captured_view);
        re2::RE2::FullMatch(text_view, re, &full_captured_int);
        re2::RE2::FullMatch(text_view, re, &full_captured_double);
        re2::RE2::FullMatch(text_view, re, &full_captured_opt_str);
        re2::RE2::FullMatch(text_view, re, nullptr);
    }
    
    // Test RE2::Match
    {
        const int max_submatches = 4;
        absl::string_view submatches[max_submatches];
        
        // Test different anchor types and positions
        re.Match(text_view, 0, text_view.size(), re2::RE2::UNANCHORED, submatches, max_submatches);
        re.Match(text_view, 0, text_view.size(), re2::RE2::ANCHOR_START, submatches, max_submatches);
        re.Match(text_view, 0, text_view.size(), re2::RE2::ANCHOR_BOTH, submatches, max_submatches);
        
        // Test with different start/end positions
        if (text_view.size() > 1) {
            size_t start = text_view.size() / 2;
            size_t end = text_view.size();
            re.Match(text_view, start, end, re2::RE2::UNANCHORED, submatches, max_submatches);
        }
        
        // Test with fewer submatches
        re.Match(text_view, 0, text_view.size(), re2::RE2::UNANCHORED, submatches, 1);
        re.Match(text_view, 0, text_view.size(), re2::RE2::UNANCHORED, submatches, 0);
    }
    
    // Test RE2::FindAndConsume
    {
        absl::string_view input_find = text_view;
        std::string find_captured_str;
        absl::string_view find_captured_view;
        int find_captured_int = 0;
        
        // Try multiple times to find all occurrences
        while (re2::RE2::FindAndConsume(&input_find, re, &find_captured_str)) {
            // Continue consuming
        }
        
        input_find = text_view;
        while (re2::RE2::FindAndConsume(&input_find, re, &find_captured_view)) {
            // Continue consuming
        }
        
        input_find = text_view;
        while (re2::RE2::FindAndConsume(&input_find, re, &find_captured_int)) {
            // Continue consuming
        }
    }
    
    // Test RE2::FullMatchN
    {
        std::string arg1_str;
        int arg2_int = 0;
        
        re2::RE2::Arg args[] = {
            re2::RE2::Arg(&arg1_str),
            re2::RE2::Arg(&arg2_int)
        };
        
        const re2::RE2::Arg* arg_ptrs[] = {&args[0], &args[1]};
        
        re2::RE2::FullMatchN(text_view, re, arg_ptrs, 2);
        re2::RE2::FullMatchN(text_view, re, arg_ptrs, 1);
        re2::RE2::FullMatchN(text_view, re, nullptr, 0);
    }
    
    // Test RE2::Arg::Parse
    {
        // Create Arg objects with different parsers
        std::string parse_str;
        int parse_int = 0;
        double parse_double = 0.0;
        
        // Use the template constructor for Arg
        re2::RE2::Arg arg_str(&parse_str);
        re2::RE2::Arg arg_int(&parse_int);
        re2::RE2::Arg arg_double(&parse_double);
        
        // Try parsing different parts of the text
        if (text_view.size() > 0) {
            size_t parse_len = text_view.size() / 2;
            if (parse_len > 0) {
                arg_str.Parse(text_view.data(), parse_len);
                arg_int.Parse(text_view.data(), parse_len);
                arg_double.Parse(text_view.data(), parse_len);
            }
        }
    }
    
    return 0;
}