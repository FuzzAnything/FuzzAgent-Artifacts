// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::FilteredRE2::Add at filtered_re2.cc:51:29 in filtered_re2.h
// re2::FilteredRE2::Compile at filtered_re2.cc:70:19 in filtered_re2.h
// re2::FilteredRE2::FirstMatch at filtered_re2.cc:98:18 in filtered_re2.h
// re2::FilteredRE2::AllMatches at filtered_re2.cc:112:19 in filtered_re2.h
// re2::FilteredRE2::AllPotentials at filtered_re2.cc:124:19 in filtered_re2.h
// re2::RE2::Set::Add at set.cc:64:15 in set.h
// re2::RE2::Set::Compile at set.cc:103:16 in set.h
// re2::RE2::Set::Match at set.cc:133:16 in set.h
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

#include "re2/filtered_re2.h"
#include "re2/set.h"

extern "C" int LLVMFuzzerTestOneInput_52(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Split input into parts for different test cases
    size_t split_point = Size / 2;
    if (split_point == 0) split_point = 1;

    std::string pattern_str(reinterpret_cast<const char*>(Data), split_point);
    std::string text_str;
    if (Size > split_point) {
        text_str = std::string(reinterpret_cast<const char*>(Data + split_point), Size - split_point);
    }

    // Test FilteredRE2 functions
    {
        re2::FilteredRE2 filtered_re2;
        int id = 0;
        
        // Add pattern to FilteredRE2
        re2::RE2::Options options(re2::RE2::DefaultOptions);
        re2::RE2::ErrorCode error = filtered_re2.Add(pattern_str, options, &id);
        
        // Only proceed if pattern was added successfully
        if (error == re2::RE2::NoError) {
            std::vector<std::string> strings_to_match;
            filtered_re2.Compile(&strings_to_match);
            
            // Prepare atoms vector (indices of strings that match in text)
            std::vector<int> atoms;
            for (size_t i = 0; i < strings_to_match.size(); i++) {
                if (text_str.find(strings_to_match[i]) != std::string::npos) {
                    atoms.push_back(static_cast<int>(i));
                }
            }
            
            // Test FirstMatch
            if (!atoms.empty()) {
                int first_match = filtered_re2.FirstMatch(text_str, atoms);
                (void)first_match; // Use result to avoid unused variable warning
            }
            
            // Test AllMatches
            std::vector<int> matching_regexps;
            bool all_matches_result = filtered_re2.AllMatches(text_str, atoms, &matching_regexps);
            (void)all_matches_result;
            
            // Test AllPotentials
            std::vector<int> potential_regexps;
            filtered_re2.AllPotentials(atoms, &potential_regexps);
        }
    }
    
    // Test RE2::Set functions
    {
        re2::RE2::Options options(re2::RE2::DefaultOptions);
        re2::RE2::Set set(options, re2::RE2::ANCHOR_BOTH);
        
        // Add pattern to set
        std::string error;
        int pattern_index = set.Add(pattern_str, &error);
        
        // Only proceed if pattern was added successfully
        if (pattern_index >= 0) {
            if (set.Compile()) {
                std::vector<int> matches;
                bool match_result = set.Match(text_str, &matches);
                (void)match_result;
            }
        }
    }
    
    return 0;
}