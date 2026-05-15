// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::RE2::ConsumeN at re2.cc:428:11 in re2.h
// re2::RE2::ConsumeN at re2.cc:428:11 in re2.h
// re2::RE2::FindAndConsume at re2.h:463:15 in re2.h
// re2::RE2::FindAndConsume at re2.h:463:15 in re2.h
// re2::RE2::FullMatchN at re2.cc:418:11 in re2.h
// re2::RE2::FullMatchN at re2.cc:418:11 in re2.h
// re2::RE2::Consume at re2.h:445:15 in re2.h
// re2::RE2::Consume at re2.h:445:15 in re2.h
// re2::RE2::Match at re2.cc:658:11 in re2.h
// re2::RE2::Match at re2.cc:658:11 in re2.h
// re2::RE2::Match at re2.cc:658:11 in re2.h
// re2::RE2::Match at re2.cc:658:11 in re2.h
// re2::RE2::Match at re2.cc:658:11 in re2.h
// re2::RE2::Match at re2.cc:658:11 in re2.h
// re2::RE2::ConsumeN at re2.cc:428:11 in re2.h
// re2::RE2::ConsumeN at re2.cc:428:11 in re2.h
// re2::RE2::ConsumeN at re2.cc:428:11 in re2.h
// re2::RE2::FindAndConsume at re2.h:463:15 in re2.h
// re2::RE2::FindAndConsume at re2.h:463:15 in re2.h
// re2::RE2::FindAndConsume at re2.h:463:15 in re2.h
// re2::RE2::FullMatchN at re2.cc:418:11 in re2.h
// re2::RE2::FullMatchN at re2.cc:418:11 in re2.h
// re2::RE2::FullMatchN at re2.cc:418:11 in re2.h
// re2::RE2::Consume at re2.h:445:15 in re2.h
// re2::RE2::Consume at re2.h:445:15 in re2.h
// re2::RE2::Consume at re2.h:445:15 in re2.h
// re2::RE2::Arg::Parse at re2.h:920:8 in re2.h
// re2::RE2::Arg::Parse at re2.h:920:8 in re2.h
// re2::RE2::Arg::Parse at re2.h:920:8 in re2.h
// re2::RE2::ok at re2.h:307:8 in re2.h
// re2::RE2::ok at re2.h:307:8 in re2.h
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
#include <cstdint>
#include <cstring>
#include <string>
#include <vector>

#include "re2/re2.h"

static uint8_t dummy_file_content[4096];
static size_t dummy_file_size = 0;

static void write_to_dummy_file(const uint8_t* data, size_t size) {
    if (size > sizeof(dummy_file_content)) {
        size = sizeof(dummy_file_content);
    }
    memcpy(dummy_file_content, data, size);
    dummy_file_size = size;
}

extern "C" int LLVMFuzzerTestOneInput_37(const uint8_t* Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Write data to dummy file for any potential file operations
    write_to_dummy_file(Data, Size);
    
    // Split input for different purposes
    size_t split1 = Size / 3;
    size_t split2 = 2 * Size / 3;
    
    // Create strings from input data
    std::string pattern_str(reinterpret_cast<const char*>(Data), 
                           std::min(split1, Size));
    std::string text_str(reinterpret_cast<const char*>(Data + split1), 
                        std::min(split2 - split1, Size - split1));
    std::string consume_str(reinterpret_cast<const char*>(Data + split2),
                           Size - split2);
    
    // Test 1: RE2::Arg::Parse
    {
        int int_val = 0;
        std::string str_val;
        double double_val = 0.0;
        
        re2::RE2::Arg int_arg(&int_val);
        re2::RE2::Arg str_arg(&str_val);
        re2::RE2::Arg double_arg(&double_val);
        
        // Try parsing different parts of the input
        if (text_str.size() > 0) {
            int_arg.Parse(text_str.c_str(), text_str.size());
        }
        if (consume_str.size() > 0) {
            str_arg.Parse(consume_str.c_str(), consume_str.size());
        }
        if (pattern_str.size() > 0) {
            double_arg.Parse(pattern_str.c_str(), pattern_str.size());
        }
    }
    
    // Initialize RE2 pattern (might fail if pattern is invalid)
    // Use a simple pattern if the input pattern is invalid to avoid crashes
    re2::RE2 re(pattern_str);
    if (!re.ok()) {
        // Use a simple default pattern that won't crash
        re2::RE2 default_re("a");
        if (!default_re.ok()) {
            return 0;
        }
        
        // Test 2: RE2::Match with default pattern
        {
            absl::string_view text(text_str);
            std::vector<absl::string_view> submatches(2);  // Smaller size
            
            // Try different anchor types with safe parameters
            if (!text.empty()) {
                default_re.Match(text, 0, std::min(text.size(), (size_t)1000), 
                               re2::RE2::UNANCHORED, 
                               submatches.data(), static_cast<int>(submatches.size()));
                
                default_re.Match(text, 0, std::min(text.size(), (size_t)1000), 
                               re2::RE2::ANCHOR_START, 
                               submatches.data(), static_cast<int>(submatches.size()));
            }
        }
        
        // Test 3: RE2::ConsumeN with default pattern
        {
            absl::string_view input(consume_str);
            
            // Create arguments for ConsumeN
            std::string arg1_str;
            int arg2_int = 0;
            
            re2::RE2::Arg arg1(&arg1_str);
            re2::RE2::Arg arg2(&arg2_int);
            
            const re2::RE2::Arg* args1[] = {&arg1, &arg2};
            const re2::RE2::Arg* args2[] = {&arg1};
            
            // Try ConsumeN with different argument counts
            if (!input.empty()) {
                re2::RE2::ConsumeN(&input, default_re, args1, 2);
                
                // Reset input and try again
                input = absl::string_view(consume_str);
                re2::RE2::ConsumeN(&input, default_re, args2, 1);
            }
        }
        
        // Test 4: RE2::FindAndConsume with default pattern
        {
            absl::string_view input(consume_str);
            std::string arg1_str;
            int arg2_int = 0;
            
            // Try FindAndConsume with different argument combinations
            if (!input.empty()) {
                re2::RE2::FindAndConsume(&input, default_re, &arg1_str);
                
                input = absl::string_view(consume_str);
                re2::RE2::FindAndConsume(&input, default_re, &arg1_str, &arg2_int);
            }
        }
        
        // Test 5: RE2::FullMatchN with default pattern
        {
            absl::string_view text(text_str);
            
            // Create arguments for FullMatchN
            std::string arg1_str;
            int arg2_int = 0;
            
            re2::RE2::Arg arg1(&arg1_str);
            re2::RE2::Arg arg2(&arg2_int);
            
            const re2::RE2::Arg* args1[] = {&arg1, &arg2};
            const re2::RE2::Arg* args2[] = {&arg1};
            
            // Try FullMatchN with different argument counts
            if (!text.empty()) {
                re2::RE2::FullMatchN(text, default_re, args1, 2);
                re2::RE2::FullMatchN(text, default_re, args2, 1);
            }
        }
        
        // Test 6: RE2::Consume with default pattern
        {
            absl::string_view input(consume_str);
            std::string arg1_str;
            int arg2_int = 0;
            
            // Try Consume with different argument combinations
            if (!input.empty()) {
                re2::RE2::Consume(&input, default_re, &arg1_str);
                
                input = absl::string_view(consume_str);
                re2::RE2::Consume(&input, default_re, &arg1_str, &arg2_int);
            }
        }
        
        return 0;
    }
    
    // If we have a valid RE2 object, run all tests with it
    
    // Test 2: RE2::Match
    {
        absl::string_view text(text_str);
        std::vector<absl::string_view> submatches(2);  // Reduced from 4 to avoid excessive allocation
        
        // Try different anchor types with safe bounds
        if (!text.empty()) {
            re.Match(text, 0, std::min(text.size(), (size_t)1000), 
                    re2::RE2::UNANCHORED, 
                    submatches.data(), static_cast<int>(submatches.size()));
            
            re.Match(text, 0, std::min(text.size(), (size_t)1000), 
                    re2::RE2::ANCHOR_START, 
                    submatches.data(), static_cast<int>(submatches.size()));
            
            re.Match(text, 0, std::min(text.size(), (size_t)1000), 
                    re2::RE2::ANCHOR_BOTH, 
                    submatches.data(), static_cast<int>(submatches.size()));
            
            // Try with different start/end positions
            if (text.size() > 2) {
                size_t start = std::min((size_t)1, text.size() - 1);
                size_t end = std::min(text.size() - 1, (size_t)1000);
                if (start < end) {
                    re.Match(text, start, end, re2::RE2::UNANCHORED,
                            submatches.data(), static_cast<int>(submatches.size()));
                }
            }
            
            // Try with different number of submatches
            re.Match(text, 0, std::min(text.size(), (size_t)1000), 
                    re2::RE2::UNANCHORED, nullptr, 0);
            re.Match(text, 0, std::min(text.size(), (size_t)1000), 
                    re2::RE2::UNANCHORED, 
                    submatches.data(), 1);
        }
    }
    
    // Test 3: RE2::ConsumeN
    {
        absl::string_view input(consume_str);
        
        // Create arguments for ConsumeN
        std::string arg1_str;
        int arg2_int = 0;
        
        re2::RE2::Arg arg1(&arg1_str);
        re2::RE2::Arg arg2(&arg2_int);
        
        const re2::RE2::Arg* args1[] = {&arg1, &arg2};
        const re2::RE2::Arg* args2[] = {&arg1};
        
        // Try ConsumeN with different argument counts
        if (!input.empty()) {
            re2::RE2::ConsumeN(&input, re, args1, 2);
            
            // Reset input and try again
            input = absl::string_view(consume_str);
            re2::RE2::ConsumeN(&input, re, args2, 1);
            
            // Try with null args
            input = absl::string_view(consume_str);
            re2::RE2::ConsumeN(&input, re, nullptr, 0);
        }
    }
    
    // Test 4: RE2::FindAndConsume
    {
        absl::string_view input(consume_str);
        std::string arg1_str;
        int arg2_int = 0;
        
        // Try FindAndConsume with different argument combinations
        if (!input.empty()) {
            re2::RE2::FindAndConsume(&input, re, &arg1_str);
            
            input = absl::string_view(consume_str);
            re2::RE2::FindAndConsume(&input, re, &arg1_str, &arg2_int);
            
            // Try with null arguments
            input = absl::string_view(consume_str);
            re2::RE2::FindAndConsume(&input, re, static_cast<std::string*>(nullptr));
        }
    }
    
    // Test 5: RE2::FullMatchN
    {
        absl::string_view text(text_str);
        
        // Create arguments for FullMatchN
        std::string arg1_str;
        int arg2_int = 0;
        
        re2::RE2::Arg arg1(&arg1_str);
        re2::RE2::Arg arg2(&arg2_int);
        
        const re2::RE2::Arg* args1[] = {&arg1, &arg2};
        const re2::RE2::Arg* args2[] = {&arg1};
        
        // Try FullMatchN with different argument counts
        if (!text.empty()) {
            re2::RE2::FullMatchN(text, re, args1, 2);
            re2::RE2::FullMatchN(text, re, args2, 1);
            re2::RE2::FullMatchN(text, re, nullptr, 0);
        }
    }
    
    // Test 6: RE2::Consume
    {
        absl::string_view input(consume_str);
        std::string arg1_str;
        int arg2_int = 0;
        
        // Try Consume with different argument combinations
        if (!input.empty()) {
            re2::RE2::Consume(&input, re, &arg1_str);
            
            input = absl::string_view(consume_str);
            re2::RE2::Consume(&input, re, &arg1_str, &arg2_int);
            
            // Try with null arguments
            input = absl::string_view(consume_str);
            re2::RE2::Consume(&input, re, static_cast<std::string*>(nullptr));
        }
    }
    
    return 0;
}