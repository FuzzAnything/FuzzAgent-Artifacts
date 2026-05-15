// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::FilteredRE2::FirstMatch at filtered_re2.cc:98:18 in filtered_re2.h
// re2::FilteredRE2::AllMatches at filtered_re2.cc:112:19 in filtered_re2.h
// re2::FilteredRE2::Add at filtered_re2.cc:51:29 in filtered_re2.h
// re2::FilteredRE2::NumRegexps at filtered_re2.h:90:7 in filtered_re2.h
// re2::FilteredRE2::GetRE2 at filtered_re2.h:93:14 in filtered_re2.h
// re2::FilteredRE2::SlowFirstMatch at filtered_re2.cc:91:18 in filtered_re2.h
// re2::FilteredRE2::Compile at filtered_re2.cc:70:19 in filtered_re2.h
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
#include <stdint.h>
#include <stddef.h>
#include <vector>
#include <string>

extern "C" int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Initialize FilteredRE2 object
    re2::FilteredRE2 filtered_re2;
    
    // Parse input data: first byte determines number of patterns to add
    size_t pos = 0;
    int num_patterns = Data[pos] % 8 + 1; // 1-8 patterns
    pos = (pos + 1) % Size;
    
    std::vector<int> pattern_ids;
    std::vector<std::string> patterns;
    
    // Add patterns to FilteredRE2
    for (int i = 0; i < num_patterns && pos < Size; i++) {
        // Extract pattern string from data
        size_t pattern_len = Data[pos] % 16 + 1; // 1-16 chars
        pos = (pos + 1) % Size;
        
        std::string pattern;
        for (size_t j = 0; j < pattern_len && pos < Size; j++) {
            pattern.push_back(static_cast<char>(Data[pos]));
            pos = (pos + 1) % Size;
        }
        
        patterns.push_back(pattern);
        
        // Add pattern with default options
        int id = -1;
        re2::RE2::Options options(re2::RE2::DefaultOptions);
        re2::RE2::ErrorCode error = filtered_re2.Add(pattern, options, &id);
        
        if (error == re2::RE2::NoError && id >= 0) {
            pattern_ids.push_back(id);
        }
    }
    
    // Call NumRegexps
    int num_regexps = filtered_re2.NumRegexps();
    
    // Call GetRE2 for valid indices
    for (int i = 0; i < num_regexps && i < 10; i++) {
        if (i < pattern_ids.size()) {
            const re2::RE2& re2_obj = filtered_re2.GetRE2(i);
            (void)re2_obj; // Use to avoid unused variable warning
        }
    }
    
    // Extract text for matching functions
    std::string text;
    if (pos < Size) {
        size_t text_len = Size - pos;
        if (text_len > 100) text_len = 100; // Limit text size
        text.assign(reinterpret_cast<const char*>(Data + pos), text_len);
    } else {
        text = "test";
    }
    
    // Call SlowFirstMatch (doesn't require Compile)
    int slow_match = filtered_re2.SlowFirstMatch(text);
    (void)slow_match;
    
    // Try to compile if we have patterns
    if (!pattern_ids.empty()) {
        // Create strings_to_match vector for Compile
        std::vector<std::string> strings_to_match;
        // Use the patterns we added as strings to match
        for (const auto& pattern : patterns) {
            if (!pattern.empty()) {
                strings_to_match.push_back(pattern);
            }
        }
        // Add the text as well
        if (!text.empty()) {
            strings_to_match.push_back(text);
        }
        
        // Call Compile with the strings_to_match vector
        filtered_re2.Compile(&strings_to_match);
        
        // Create atoms vector for filtering
        std::vector<int> atoms;
        for (size_t i = 0; i < pattern_ids.size() && i < 5; i++) {
            atoms.push_back(pattern_ids[i]);
        }
        
        // Call FirstMatch
        int first_match = filtered_re2.FirstMatch(text, atoms);
        (void)first_match;
        
        // Call AllMatches
        std::vector<int> matching_regexps;
        bool all_matches_result = filtered_re2.AllMatches(text, atoms, &matching_regexps);
        (void)all_matches_result;
    }
    
    return 0;
}