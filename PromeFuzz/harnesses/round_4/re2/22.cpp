// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::RE2::ok at re2.h:307:8 in re2.h
// re2::RE2::PartialMatchN at re2.cc:423:11 in re2.h
// re2::RE2::FullMatchN at re2.cc:418:11 in re2.h
// re2::RE2::ConsumeN at re2.cc:428:11 in re2.h
// re2::RE2::FindAndConsumeN at re2.cc:439:11 in re2.h
// re2::RE2::Match at re2.cc:658:11 in re2.h
// re2::RE2::Match at re2.cc:658:11 in re2.h
// re2::RE2::Match at re2.cc:658:11 in re2.h
// re2::RE2::Arg::Parse at re2.h:920:8 in re2.h
// re2::RE2::Match at re2.cc:658:11 in re2.h
// re2::RE2::Match at re2.cc:658:11 in re2.h
// re2::RE2::Match at re2.cc:658:11 in re2.h
// re2::RE2::PartialMatchN at re2.cc:423:11 in re2.h
// re2::RE2::FullMatchN at re2.cc:418:11 in re2.h
// re2::RE2::ConsumeN at re2.cc:428:11 in re2.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <string>
#include <vector>

#include "re2/re2.h"

extern "C" int LLVMFuzzerTestOneInput_22(const uint8_t* Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    // Split input into pattern and text
    size_t pattern_len = Data[0] % (Size - 1) + 1;
    if (pattern_len >= Size) {
        pattern_len = Size / 2;
    }
    
    std::string pattern(reinterpret_cast<const char*>(Data + 1), pattern_len);
    std::string text(reinterpret_cast<const char*>(Data + 1 + pattern_len), 
                     Size - 1 - pattern_len);
    
    // Create RE2 object
    re2::RE2 re(pattern);
    if (!re.ok()) {
        return 0;
    }
    
    // Prepare arguments for capture groups
    const int max_captures = 10;
    std::vector<std::string> capture_storage(max_captures);
    std::vector<re2::RE2::Arg> args;
    std::vector<const re2::RE2::Arg*> arg_ptrs;
    
    for (int i = 0; i < max_captures; ++i) {
        args.emplace_back(&capture_storage[i]);
        arg_ptrs.push_back(&args.back());
    }
    
    // Test PartialMatchN
    int n_captures = Data[0] % (max_captures + 1);
    re2::RE2::PartialMatchN(text, re, arg_ptrs.data(), n_captures);
    
    // Test FullMatchN
    re2::RE2::FullMatchN(text, re, arg_ptrs.data(), n_captures);
    
    // Test ConsumeN
    absl::string_view consume_input = text;
    re2::RE2::ConsumeN(&consume_input, re, arg_ptrs.data(), n_captures);
    
    // Test FindAndConsumeN
    absl::string_view find_consume_input = text;
    re2::RE2::FindAndConsumeN(&find_consume_input, re, arg_ptrs.data(), n_captures);
    
    // Test Match with various parameters
    absl::string_view* submatch = new absl::string_view[max_captures];
    size_t startpos = Data[0] % (text.size() + 1);
    size_t endpos = text.size();
    if (startpos < endpos) {
        re.Match(text, startpos, endpos, re2::RE2::UNANCHORED, submatch, n_captures);
        re.Match(text, startpos, endpos, re2::RE2::ANCHOR_START, submatch, n_captures);
        re.Match(text, startpos, endpos, re2::RE2::ANCHOR_BOTH, submatch, n_captures);
    }
    delete[] submatch;
    
    // Test Arg::Parse with string argument
    if (!text.empty()) {
        std::string parse_dest;
        re2::RE2::Arg string_arg(&parse_dest);
        string_arg.Parse(text.data(), text.size());
    }
    
    // Test with different anchor types
    absl::string_view* submatch2 = new absl::string_view[1];
    if (!text.empty()) {
        re.Match(text, 0, text.size(), re2::RE2::UNANCHORED, submatch2, 1);
        re.Match(text, 0, text.size(), re2::RE2::ANCHOR_START, submatch2, 1);
        re.Match(text, 0, text.size(), re2::RE2::ANCHOR_BOTH, submatch2, 1);
    }
    delete[] submatch2;
    
    // Test with empty text
    absl::string_view empty_text;
    re2::RE2::PartialMatchN(empty_text, re, arg_ptrs.data(), 0);
    re2::RE2::FullMatchN(empty_text, re, arg_ptrs.data(), 0);
    
    // Test ConsumeN with empty input
    absl::string_view empty_consume;
    re2::RE2::ConsumeN(&empty_consume, re, arg_ptrs.data(), 0);
    
    return 0;
}