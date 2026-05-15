// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::FilteredRE2::AllMatches at filtered_re2.cc:112:19 in filtered_re2.h
// re2::FilteredRE2::GetRE2 at filtered_re2.h:93:14 in filtered_re2.h
// re2::RE2::Options::set_encoding at re2.h:700:10 in re2.h
// re2::RE2::Options::set_case_sensitive at re2.h:724:10 in re2.h
// re2::FilteredRE2::Add at filtered_re2.cc:51:29 in filtered_re2.h
// re2::FilteredRE2::NumRegexps at filtered_re2.h:90:7 in filtered_re2.h
// re2::FilteredRE2::GetRE2 at filtered_re2.h:93:14 in filtered_re2.h
// re2::FilteredRE2::SlowFirstMatch at filtered_re2.cc:91:18 in filtered_re2.h
// re2::FilteredRE2::AllMatches at filtered_re2.cc:112:19 in filtered_re2.h
// re2::FilteredRE2::Add at filtered_re2.cc:51:29 in filtered_re2.h
// re2::FilteredRE2::Add at filtered_re2.cc:51:29 in filtered_re2.h
// re2::FilteredRE2::NumRegexps at filtered_re2.h:90:7 in filtered_re2.h
// re2::FilteredRE2::SlowFirstMatch at filtered_re2.cc:91:18 in filtered_re2.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include "re2/filtered_re2.h"
#include "re2/re2.h"
#include <cstdint>
#include <cstddef>
#include <vector>
#include <string>

extern "C" int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    // Initialize FilteredRE2 object
    re2::FilteredRE2 filtered_re2;

    // Extract pattern string from input data
    size_t pattern_len = Size / 2;
    if (pattern_len == 0) pattern_len = 1;
    if (Size - pattern_len == 0) return 0;
    
    std::string pattern(reinterpret_cast<const char*>(Data), 
                       std::min(pattern_len, Size));
    
    // Extract text string from remaining input data
    std::string text(reinterpret_cast<const char*>(Data) + pattern_len, 
                    Size - pattern_len);
    
    // Create RE2 options
    re2::RE2::Options options;
    options.set_encoding((Data[0] & 1) ? re2::RE2::Options::EncodingUTF8 : 
                                        re2::RE2::Options::EncodingLatin1);
    options.set_case_sensitive(Data[1] & 1);
    
    // Test Add function
    int id = -1;
    re2::RE2::ErrorCode error = filtered_re2.Add(pattern, options, &id);
    
    // Test NumRegexps function
    int num_regexps = filtered_re2.NumRegexps();
    
    // If pattern was added successfully, test other functions
    if (error == re2::RE2::NoError && id >= 0) {
        // Test GetRE2 function - only if id is valid
        if (id >= 0 && id < num_regexps) {
            const re2::RE2& re2_obj = filtered_re2.GetRE2(id);
        }
        
        // Test SlowFirstMatch function (doesn't require Compile)
        // Only call if text is not empty
        if (!text.empty()) {
            int slow_match = filtered_re2.SlowFirstMatch(text);
        }
        
        // Create atoms vector for filtering functions
        std::vector<int> atoms;
        if (num_regexps > 0 && !text.empty()) {
            // Add valid atom indices
            for (int i = 0; i < num_regexps && i < 3; ++i) {
                atoms.push_back(i);
            }
            
            // Test AllMatches function
            std::vector<int> matching_regexps;
            bool all_matches_result = filtered_re2.AllMatches(text, atoms, &matching_regexps);
        }
    }
    
    // Test with multiple patterns if we have enough data
    if (Size > 10) {
        re2::FilteredRE2 filtered_re2_multi;
        
        // Add a few simple patterns
        int id1 = -1, id2 = -1;
        std::string simple_pattern1 = "a.*b";
        std::string simple_pattern2 = "[0-9]+";
        
        filtered_re2_multi.Add(simple_pattern1, options, &id1);
        filtered_re2_multi.Add(simple_pattern2, options, &id2);
        
        // Test NumRegexps with multiple patterns
        int multi_num = filtered_re2_multi.NumRegexps();
        
        // Test SlowFirstMatch with multiple patterns
        if (multi_num > 0 && !text.empty()) {
            int multi_slow_match = filtered_re2_multi.SlowFirstMatch(text);
            
            // Create atoms vector with valid indices
            std::vector<int> multi_atoms;
            for (int i = 0; i < multi_num && i < 3; ++i) {
                multi_atoms.push_back(i);
            }
            
            // Test AllMatches with multiple patterns
            std::vector<int> multi_matching;
            bool multi_all_matches = filtered_re2_multi.AllMatches(text, multi_atoms, &multi_matching);
            
            // Test GetRE2 for each valid ID
            for (int i = 0; i < multi_num; ++i) {
                const re2::RE2& re2_multi = filtered_re2_multi.GetRE2(i);
            }
        }
    }
    
    return 0;
}