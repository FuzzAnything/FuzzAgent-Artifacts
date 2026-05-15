// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::RE2::Options::set_encoding at re2.h:700:10 in re2.h
// re2::RE2::Options::set_log_errors at re2.h:709:10 in re2.h
// re2::FilteredRE2::Add at filtered_re2.cc:51:29 in filtered_re2.h
// re2::FilteredRE2::Add at filtered_re2.cc:51:29 in filtered_re2.h
// re2::FilteredRE2::NumRegexps at filtered_re2.h:90:7 in filtered_re2.h
// re2::FilteredRE2::SlowFirstMatch at filtered_re2.cc:91:18 in filtered_re2.h
// re2::FilteredRE2::Compile at filtered_re2.cc:70:19 in filtered_re2.h
// re2::FilteredRE2::FirstMatch at filtered_re2.cc:98:18 in filtered_re2.h
// re2::FilteredRE2::AllMatches at filtered_re2.cc:112:19 in filtered_re2.h
// re2::RE2::Options::set_case_sensitive at re2.h:724:10 in re2.h
// re2::RE2::Options::set_log_errors at re2.h:709:10 in re2.h
// re2::FilteredRE2::Add at filtered_re2.cc:51:29 in filtered_re2.h
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
#include <cstring>
#include <vector>
#include <string>
#include <algorithm>

extern "C" int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    // Split input into parts for patterns and text
    size_t split_point = std::min<size_t>(Size / 2, 128);
    std::string pattern_data(reinterpret_cast<const char*>(Data), split_point);
    std::string text_data(reinterpret_cast<const char*>(Data + split_point), Size - split_point);
    
    // Ensure text_data is not empty and has valid UTF-8
    if (text_data.empty()) {
        text_data = "test";
    }
    
    // Create FilteredRE2 object
    re2::FilteredRE2 filtered_re2(3); // min_atom_len = 3
    
    // Test Add function with error checking
    int pattern_id = -1;
    re2::RE2::Options options;
    options.set_encoding(re2::RE2::Options::EncodingUTF8);
    options.set_log_errors(false); // Disable error logging to avoid crashes
    
    re2::RE2::ErrorCode error = filtered_re2.Add(pattern_data, options, &pattern_id);
    
    // Only proceed if pattern was added successfully
    if (error != re2::RE2::NoError) {
        // Add a simple valid pattern to ensure we can test other functions
        filtered_re2.Add("test", options, &pattern_id);
    }
    
    // Test NumRegexps
    int num_regexps = filtered_re2.NumRegexps();
    
    // Test SlowFirstMatch only if we have regexps
    if (num_regexps > 0) {
        int slow_match = filtered_re2.SlowFirstMatch(text_data);
    }
    
    // Test Compile
    std::vector<std::string> strings_to_match;
    filtered_re2.Compile(&strings_to_match);
    
    // Prepare atoms for FirstMatch and AllMatches
    std::vector<int> atoms;
    if (!strings_to_match.empty()) {
        // Use valid indices from strings_to_match
        for (size_t i = 0; i < strings_to_match.size() && i < 5; ++i) {
            atoms.push_back(static_cast<int>(i));
        }
    }
    
    // Test FirstMatch only if we have atoms and regexps
    if (!atoms.empty() && num_regexps > 0) {
        int first_match = filtered_re2.FirstMatch(text_data, atoms);
    }
    
    // Test AllMatches only if we have atoms and regexps
    if (!atoms.empty() && num_regexps > 0) {
        std::vector<int> matching_regexps;
        bool all_matches_result = filtered_re2.AllMatches(text_data, atoms, &matching_regexps);
    }
    
    // Try adding another pattern with different options if we have enough data
    if (Size > 10 && num_regexps < 10) {
        std::string pattern2(reinterpret_cast<const char*>(Data), std::min<size_t>(Size, 50));
        int pattern2_id = -1;
        re2::RE2::Options options2;
        options2.set_case_sensitive(false);
        options2.set_log_errors(false);
        
        // Clean up pattern2 to avoid problematic characters
        for (char& c : pattern2) {
            if (c == 0) c = 'a'; // Replace null bytes
        }
        
        filtered_re2.Add(pattern2, options2, &pattern2_id);
        
        // Test SlowFirstMatch again with new pattern if added successfully
        if (pattern2_id >= 0) {
            filtered_re2.SlowFirstMatch(text_data);
        }
    }
    
    return 0;
}