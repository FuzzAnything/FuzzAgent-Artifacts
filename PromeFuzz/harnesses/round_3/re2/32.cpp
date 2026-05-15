// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::RE2::ok at re2.h:307:8 in re2.h
// re2::RE2::FullMatchN at re2.cc:418:11 in re2.h
// re2::RE2::ok at re2.h:307:8 in re2.h
// re2::RE2::Consume at re2.h:445:15 in re2.h
// re2::RE2::Arg::Parse at re2.h:920:8 in re2.h
// re2::RE2::ok at re2.h:307:8 in re2.h
// re2::RE2::Match at re2.cc:658:11 in re2.h
// re2::RE2::ok at re2.h:307:8 in re2.h
// re2::RE2::ConsumeN at re2.cc:428:11 in re2.h
// re2::RE2::ok at re2.h:307:8 in re2.h
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
#include <cstring>
#include <string>
#include <vector>

static bool dummy_parser(const char* str, size_t n, void* ptr) {
    // Simple parser that just copies the string
    if (ptr) {
        std::string* s = static_cast<std::string*>(ptr);
        s->assign(str, n);
        return true;
    }
    return false;
}

extern "C" int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;
    
    // Split input: first part for regex pattern, rest for text
    size_t pattern_len = Data[0] % (Size - 1) + 1;
    if (pattern_len > Size - 1) pattern_len = Size - 1;
    
    std::string pattern(reinterpret_cast<const char*>(Data + 1), pattern_len);
    std::string text;
    if (Size > pattern_len + 1) {
        text.assign(reinterpret_cast<const char*>(Data + 1 + pattern_len), 
                   Size - pattern_len - 1);
    }
    
    // Test 1: RE2::Arg::Parse
    {
        std::string result;
        re2::RE2::Arg arg(&result, dummy_parser);
        if (!text.empty()) {
            arg.Parse(text.c_str(), text.size());
        }
    }
    
    // Test 2: RE2::Match
    {
        re2::RE2 re(pattern, re2::RE2::Quiet);
        if (re.ok()) {
            absl::string_view sv_text(text);
            std::vector<absl::string_view> submatches(5);
            
            size_t startpos = 0;
            size_t endpos = text.size();
            if (text.size() > 0) {
                startpos = Data[0] % text.size();
                endpos = Data[1] % (text.size() + 1);
                if (endpos < startpos) std::swap(startpos, endpos);
            }
            
            re2::RE2::Anchor anchor = static_cast<re2::RE2::Anchor>(
                Data[0] % 3); // UNANCHORED, ANCHOR_START, ANCHOR_BOTH
            
            re.Match(sv_text, startpos, endpos, anchor, 
                    submatches.data(), static_cast<int>(submatches.size()));
        }
    }
    
    // Test 3: RE2::ConsumeN
    {
        re2::RE2 re(pattern, re2::RE2::Quiet);
        if (re.ok()) {
            absl::string_view input(text);
            std::string arg1, arg2;
            re2::RE2::Arg args[] = {
                re2::RE2::Arg(&arg1, dummy_parser),
                re2::RE2::Arg(&arg2, dummy_parser)
            };
            const re2::RE2::Arg* arg_ptrs[] = {&args[0], &args[1]};
            
            re2::RE2::ConsumeN(&input, re, arg_ptrs, 2);
        }
    }
    
    // Test 4: RE2::FindAndConsume
    {
        re2::RE2 re(pattern, re2::RE2::Quiet);
        if (re.ok()) {
            absl::string_view input(text);
            std::string arg1, arg2;
            
            re2::RE2::FindAndConsume(&input, re, &arg1, &arg2);
        }
    }
    
    // Test 5: RE2::FullMatchN
    {
        re2::RE2 re(pattern, re2::RE2::Quiet);
        if (re.ok()) {
            absl::string_view sv_text(text);
            std::string arg1, arg2;
            re2::RE2::Arg args[] = {
                re2::RE2::Arg(&arg1, dummy_parser),
                re2::RE2::Arg(&arg2, dummy_parser)
            };
            const re2::RE2::Arg* arg_ptrs[] = {&args[0], &args[1]};
            
            re2::RE2::FullMatchN(sv_text, re, arg_ptrs, 2);
        }
    }
    
    // Test 6: RE2::Consume
    {
        re2::RE2 re(pattern, re2::RE2::Quiet);
        if (re.ok()) {
            absl::string_view input(text);
            std::string arg1, arg2;
            
            re2::RE2::Consume(&input, re, &arg1, &arg2);
        }
    }
    
    return 0;
}