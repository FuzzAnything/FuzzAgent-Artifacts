// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::RE2::Options::set_log_errors at re2.h:709:10 in re2.h
// re2::FilteredRE2::Add at filtered_re2.cc:51:29 in filtered_re2.h
// re2::FilteredRE2::NumRegexps at filtered_re2.h:90:7 in filtered_re2.h
// re2::FilteredRE2::GetRE2 at filtered_re2.h:93:14 in filtered_re2.h
// re2::FilteredRE2::SlowFirstMatch at filtered_re2.cc:91:18 in filtered_re2.h
// re2::FilteredRE2::Compile at filtered_re2.cc:70:19 in filtered_re2.h
// re2::FilteredRE2::FirstMatch at filtered_re2.cc:98:18 in filtered_re2.h
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

extern "C" int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    // Split input into pattern and text parts
    size_t split_point = Size / 2;
    std::string pattern(reinterpret_cast<const char*>(Data), split_point);
    std::string text(reinterpret_cast<const char*>(Data + split_point), Size - split_point);

    // Test FilteredRE2 functions
    {
        re2::FilteredRE2 filtered_re2(3); // min_atom_len = 3
        
        // Test Add function
        int id = -1;
        re2::RE2::Options options;
        options.set_log_errors(false);
        re2::RE2::ErrorCode error = filtered_re2.Add(pattern, options, &id);
        
        // Test NumRegexps
        int num_regexps = filtered_re2.NumRegexps();
        
        // Test GetRE2 if pattern was added successfully
        if (error == re2::RE2::NoError && id >= 0 && id < num_regexps) {
            const re2::RE2& re2_obj = filtered_re2.GetRE2(id);
            (void)re2_obj; // Use to avoid unused variable warning
        }
        
        // Test SlowFirstMatch (can be called before Compile)
        int slow_match = filtered_re2.SlowFirstMatch(text);
        (void)slow_match;
        
        // Try to compile and test FirstMatch
        if (error == re2::RE2::NoError) {
            // Create strings_to_match vector for Compile
            std::vector<std::string> strings_to_match;
            // Add some strings to match - use text and pattern parts
            if (!text.empty()) {
                strings_to_match.push_back(text);
            }
            if (!pattern.empty() && pattern != text) {
                strings_to_match.push_back(pattern);
            }
            // Add a few more strings if we have enough data
            if (Size > 10) {
                strings_to_match.push_back("test");
                strings_to_match.push_back("match");
            }
            
            filtered_re2.Compile(&strings_to_match);
            
            // Create atoms vector for FirstMatch
            std::vector<int> atoms;
            // Add some atom indices based on the strings_to_match size
            for (int i = 0; i < static_cast<int>(strings_to_match.size()) && i < 5; ++i) {
                atoms.push_back(i);
            }
            
            int first_match = filtered_re2.FirstMatch(text, atoms);
            (void)first_match;
        }
    }

    return 0;
}