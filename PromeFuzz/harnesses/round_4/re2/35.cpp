// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::RE2::Set::Match at set.cc:133:16 in set.h
// re2::FilteredRE2::AllMatches at filtered_re2.cc:112:19 in filtered_re2.h
// re2::RE2::Set::Add at set.cc:64:15 in set.h
// re2::RE2::Set::Add at set.cc:64:15 in set.h
// re2::RE2::Set::Add at set.cc:64:15 in set.h
// re2::RE2::Set::Size at set.cc:58:15 in set.h
// re2::RE2::Set::Compile at set.cc:103:16 in set.h
// re2::RE2::Set::Size at set.cc:58:15 in set.h
// re2::RE2::Set::Match at set.cc:133:16 in set.h
// re2::RE2::Set::Match at set.cc:137:16 in set.h
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
#include <vector>
#include <string>

#include "re2/set.h"
#include "re2/filtered_re2.h"

extern "C" int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size) {
    // Use the first byte to determine configuration options
    if (Size == 0) return 0;
    
    uint8_t config = Data[0];
    size_t data_offset = 1;
    size_t remaining = Size - 1;
    
    if (remaining == 0) return 0;
    
    // Split remaining data into pattern and text parts
    size_t split_point = (config % remaining) + 1;
    if (split_point > remaining) split_point = remaining / 2;
    
    // Create pattern string - limit to reasonable size and filter out null bytes
    std::string pattern_str;
    if (split_point > 0) {
        size_t pattern_len = split_point;
        if (pattern_len > 100) pattern_len = 100;
        pattern_str.reserve(pattern_len);
        for (size_t i = 0; i < pattern_len; i++) {
            char c = static_cast<char>(Data[data_offset + i]);
            if (c != 0) pattern_str.push_back(c);
        }
    }
    data_offset += split_point;
    remaining -= split_point;
    
    // Create text string - limit to reasonable size
    std::string text_str;
    if (remaining > 0) {
        size_t text_len = remaining;
        if (text_len > 100) text_len = 100;
        text_str = std::string(reinterpret_cast<const char*>(Data + data_offset), 
                              text_len);
    }
    
    // Test RE2::Set functionality
    {
        // Use default options without setting max_mem
        re2::RE2::Options options;
        re2::RE2::Set set(options, re2::RE2::UNANCHORED);
        std::string error;
        
        // Add pattern to set - only if we have non-empty pattern
        bool pattern_added = false;
        if (!pattern_str.empty()) {
            int pattern_index = set.Add(pattern_str, &error);
            if (pattern_index >= 0) {
                pattern_added = true;
                // Try to add a second simple pattern
                set.Add("[a-z]*", nullptr);
            }
        }
        
        // If no pattern was added, add a default pattern
        if (!pattern_added) {
            set.Add(".*", nullptr);
        }
        
        // Only compile if we have patterns
        if (set.Size() > 0) {
            bool compile_result = set.Compile();
            if (!compile_result) {
                // If compilation fails, skip matching
                return 0;
            }
            
            // Check size
            int set_size = set.Size();
            (void)set_size;
            
            // Test Match with vector - only if we have text
            if (!text_str.empty()) {
                std::vector<int> matches;
                bool match_result1 = set.Match(text_str, &matches);
                (void)match_result1;
                
                // Test Match with ErrorInfo
                re2::RE2::Set::ErrorInfo error_info;
                bool match_result2 = set.Match(text_str, &matches, &error_info);
                (void)match_result2;
                
                // Test Match without vector (second overload)
                bool match_result3 = set.Match(text_str, nullptr);
                (void)match_result3;
            }
        }
    } // RE2::Set destructor called here
    
    // Test FilteredRE2 functionality
    {
        re2::FilteredRE2 filtered;
        std::vector<int> atoms;
        std::vector<int> matching_regexps;
        
        // Create some dummy atom indices
        atoms.push_back(0);
        atoms.push_back(1);
        atoms.push_back(2);
        
        // Call AllMatches - only if we have text
        if (!text_str.empty()) {
            bool all_matches_result = filtered.AllMatches(text_str, atoms, &matching_regexps);
            (void)all_matches_result;
        }
    } // FilteredRE2 destructor called here
    
    return 0;
}