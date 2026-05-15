// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::RE2::PartialMatchN at re2.cc:423:11 in re2.h
// re2::RE2::FindAndConsume at re2.h:463:15 in re2.h
// re2::RE2::FindAndConsume at re2.h:463:15 in re2.h
// re2::RE2::FindAndConsume at re2.h:463:15 in re2.h
// re2::RE2::FindAndConsume at re2.h:463:15 in re2.h
// re2::RE2::Match at re2.cc:658:11 in re2.h
// re2::RE2::Match at re2.cc:658:11 in re2.h
// re2::RE2::Match at re2.cc:658:11 in re2.h
// re2::RE2::Match at re2.cc:658:11 in re2.h
// re2::RE2::Match at re2.cc:658:11 in re2.h
// re2::RE2::Match at re2.cc:658:11 in re2.h
// re2::RE2::Match at re2.cc:658:11 in re2.h
// re2::RE2::Arg::Parse at re2.h:920:8 in re2.h
// re2::RE2::Consume at re2.h:445:15 in re2.h
// re2::RE2::Consume at re2.h:445:15 in re2.h
// re2::RE2::Consume at re2.h:445:15 in re2.h
// re2::RE2::Consume at re2.h:445:15 in re2.h
// re2::RE2::ok at re2.h:307:8 in re2.h
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
#include "re2/re2.h"
#include <stdint.h>
#include <string>
#include <vector>

static re2::RE2::Arg* createArgFromData(const uint8_t* data, size_t size, size_t& offset) {
    if (offset >= size) return nullptr;
    
    // Create different types of arguments based on the data
    uint8_t arg_type = data[offset] % 3;
    offset++;
    
    switch (arg_type) {
        case 0: {
            // String argument
            if (offset >= size) return nullptr;
            size_t str_len = data[offset] % 16;
            offset++;
            if (offset + str_len > size) return nullptr;
            
            static std::string str;
            str.assign(reinterpret_cast<const char*>(data + offset), str_len);
            offset += str_len;
            return new re2::RE2::Arg(&str);
        }
        case 1: {
            // Integer argument
            static int int_val;
            int_val = static_cast<int>(data[offset % size]);
            offset = (offset + 1) % size;
            return new re2::RE2::Arg(&int_val);
        }
        case 2: {
            // Null argument
            return nullptr;
        }
        default:
            return nullptr;
    }
}

extern "C" int LLVMFuzzerTestOneInput_35(const uint8_t* data, size_t size) {
    if (size < 2) return 0;
    
    // Extract pattern from first part of data
    size_t pattern_len = data[0] % 32;
    if (pattern_len == 0) pattern_len = 1;
    if (pattern_len > size - 1) pattern_len = size - 1;
    
    std::string pattern(reinterpret_cast<const char*>(data + 1), pattern_len);
    size_t text_offset = 1 + pattern_len;
    
    // Create RE2 object
    re2::RE2 re(pattern, re2::RE2::Quiet);
    if (!re.ok()) return 0;
    
    // Prepare text for matching
    if (text_offset >= size) return 0;
    size_t text_len = size - text_offset;
    absl::string_view text(reinterpret_cast<const char*>(data + text_offset), text_len);
    
    // Test 1: RE2::ConsumeN
    {
        absl::string_view input_copy = text;
        size_t offset = 0;
        
        // Create up to 3 arguments
        std::vector<re2::RE2::Arg*> args;
        std::vector<re2::RE2::Arg*> arg_ptrs;
        
        for (int i = 0; i < 3 && offset < size; i++) {
            re2::RE2::Arg* arg = createArgFromData(data, size, offset);
            if (arg) {
                args.push_back(arg);
                arg_ptrs.push_back(arg);
            }
        }
        
        if (!arg_ptrs.empty()) {
            re2::RE2::ConsumeN(&input_copy, re, arg_ptrs.data(), static_cast<int>(arg_ptrs.size()));
        }
        
        // Cleanup
        for (auto arg : args) {
            delete arg;
        }
    }
    
    // Test 2: RE2::PartialMatchN
    {
        size_t offset = 0;
        std::vector<re2::RE2::Arg*> args;
        std::vector<re2::RE2::Arg*> arg_ptrs;
        
        for (int i = 0; i < 3 && offset < size; i++) {
            re2::RE2::Arg* arg = createArgFromData(data, size, offset);
            if (arg) {
                args.push_back(arg);
                arg_ptrs.push_back(arg);
            }
        }
        
        if (!arg_ptrs.empty()) {
            re2::RE2::PartialMatchN(text, re, arg_ptrs.data(), static_cast<int>(arg_ptrs.size()));
        }
        
        // Cleanup
        for (auto arg : args) {
            delete arg;
        }
    }
    
    // Test 3: RE2::FindAndConsume
    {
        absl::string_view input_copy = text;
        std::string s1, s2;
        int i1 = 0;
        
        // Test with different combinations of arguments
        re2::RE2::FindAndConsume(&input_copy, re, &s1);
        re2::RE2::FindAndConsume(&input_copy, re, &s1, &s2);
        re2::RE2::FindAndConsume(&input_copy, re, &i1);
        re2::RE2::FindAndConsume(&input_copy, re, &s1, &i1);
    }
    
    // Test 4: RE2::Match
    {
        std::vector<absl::string_view> submatches;
        submatches.resize(4);
        
        // Test with different anchor types and positions
        re.Match(text, 0, text.size(), re2::RE2::UNANCHORED, submatches.data(), static_cast<int>(submatches.size()));
        re.Match(text, 0, text.size(), re2::RE2::ANCHOR_START, submatches.data(), static_cast<int>(submatches.size()));
        re.Match(text, 0, text.size(), re2::RE2::ANCHOR_BOTH, submatches.data(), static_cast<int>(submatches.size()));
        
        // Test with different start positions
        if (text.size() > 2) {
            size_t start = (data[0] % (text.size() - 1));
            re.Match(text, start, text.size(), re2::RE2::UNANCHORED, submatches.data(), static_cast<int>(submatches.size()));
        }
        
        // Test with different numbers of submatches
        re.Match(text, 0, text.size(), re2::RE2::UNANCHORED, nullptr, 0);
        re.Match(text, 0, text.size(), re2::RE2::UNANCHORED, submatches.data(), 1);
        re.Match(text, 0, text.size(), re2::RE2::UNANCHORED, submatches.data(), 2);
    }
    
    // Test 5: RE2::Arg::Parse
    {
        size_t parse_offset = 0;
        re2::RE2::Arg* arg = createArgFromData(data, size, parse_offset);
        if (arg && parse_offset < size) {
            size_t parse_len = data[parse_offset] % 16;
            if (parse_len > 0 && parse_offset + parse_len <= size) {
                const char* parse_str = reinterpret_cast<const char*>(data + parse_offset);
                arg->Parse(parse_str, parse_len);
            }
        }
        delete arg;
    }
    
    // Test 6: RE2::Consume
    {
        absl::string_view input_copy = text;
        std::string s1, s2;
        int i1 = 0;
        
        // Test with different combinations of arguments
        re2::RE2::Consume(&input_copy, re, &s1);
        
        if (!input_copy.empty()) {
            re2::RE2::Consume(&input_copy, re, &s1, &s2);
        }
        
        if (!input_copy.empty()) {
            re2::RE2::Consume(&input_copy, re, &i1);
        }
        
        if (!input_copy.empty()) {
            re2::RE2::Consume(&input_copy, re, &s1, &i1);
        }
    }
    
    return 0;
}