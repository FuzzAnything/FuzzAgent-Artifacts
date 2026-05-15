// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::FilteredRE2::FirstMatch at filtered_re2.cc:98:18 in filtered_re2.h
// re2::FilteredRE2::SlowFirstMatch at filtered_re2.cc:91:18 in filtered_re2.h
// re2::FilteredRE2::FirstMatch at filtered_re2.cc:98:18 in filtered_re2.h
// re2::RE2::ok at re2.h:307:8 in re2.h
// re2::RE2::Match at re2.cc:658:11 in re2.h
// re2::RE2::Options::set_encoding at re2.h:700:10 in re2.h
// re2::FilteredRE2::Add at filtered_re2.cc:51:29 in filtered_re2.h
// re2::RE2::Options::set_encoding at re2.h:700:10 in re2.h
// re2::FilteredRE2::Add at filtered_re2.cc:51:29 in filtered_re2.h
// re2::FilteredRE2::NumRegexps at filtered_re2.h:90:7 in filtered_re2.h
// re2::FilteredRE2::SlowFirstMatch at filtered_re2.cc:91:18 in filtered_re2.h
// re2::FilteredRE2::Compile at filtered_re2.cc:70:19 in filtered_re2.h
// re2::FilteredRE2::GetRE2 at filtered_re2.h:93:14 in filtered_re2.h
// re2::RE2::ok at re2.h:307:8 in re2.h
// re2::RE2::error at re2.h:316:22 in re2.h
// re2::RE2::error_code at re2.h:320:13 in re2.h
// re2::FilteredRE2::FirstMatch at filtered_re2.cc:98:18 in filtered_re2.h
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
#include <cstdint>
#include <vector>
#include <string>
#include <fstream>

#include "re2/filtered_re2.h"
#include "re2/re2.h"

static void WriteToDummyFile(const uint8_t* data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data), size);
        file.close();
    }
}

extern "C" int LLVMFuzzerTestOneInput_42(const uint8_t* Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Split input for different purposes
    size_t split = Size / 3;
    std::string pattern(reinterpret_cast<const char*>(Data), std::min(split, Size));
    std::string text(reinterpret_cast<const char*>(Data + split), std::min(split, Size - split));
    std::string dummy_content(reinterpret_cast<const char*>(Data + 2*split), Size - 2*split);
    
    // Write to dummy file if needed
    WriteToDummyFile(Data, Size);
    
    // Initialize RE2::Options
    re2::RE2::Options options;
    options.set_encoding(re2::RE2::Options::EncodingUTF8);
    
    // Test FilteredRE2 functions
    re2::FilteredRE2 filtered;
    
    // Add patterns to FilteredRE2
    int id1 = -1, id2 = -1;
    re2::RE2::ErrorCode error1 = filtered.Add(pattern, options, &id1);
    
    // Add another pattern with modified options
    re2::RE2::Options options2;
    options2.set_encoding(re2::RE2::Options::EncodingLatin1);
    std::string pattern2 = pattern + ".*";
    re2::RE2::ErrorCode error2 = filtered.Add(pattern2, options2, &id2);
    
    // Test NumRegexps
    int num_regexps = filtered.NumRegexps();
    
    // Test SlowFirstMatch (can be called before Compile)
    // Only call if we have valid regexps and non-empty text
    int slow_match = -1;
    if (num_regexps > 0 && !text.empty()) {
        slow_match = filtered.SlowFirstMatch(text);
    }
    
    // Only compile if we have valid regexps
    if (num_regexps > 0) {
        // Compile the filtered regexps with a vector of strings
        std::vector<std::string> strings_to_match;
        strings_to_match.push_back(text);
        if (Size > 10) {
            strings_to_match.push_back(pattern);
        }
        
        // Compile returns void, not bool
        filtered.Compile(&strings_to_match);
        
        // Test GetRE2 - only access valid indices
        for (int i = 0; i < num_regexps; ++i) {
            try {
                const re2::RE2& re2_obj = filtered.GetRE2(i);
                // Access some properties to explore more states
                bool ok = re2_obj.ok();
                std::string error = re2_obj.error();
                re2::RE2::ErrorCode error_code = re2_obj.error_code();
            } catch (...) {
                // Handle exceptions silently
            }
        }
        
        // Test FirstMatch with different atom vectors
        // Only call if text is not empty to avoid issues
        if (!text.empty()) {
            std::vector<int> atoms_empty;
            int first_match_empty = filtered.FirstMatch(text, atoms_empty);
            
            std::vector<int> atoms_single;
            if (num_regexps > 0) {
                atoms_single.push_back(0);
                int first_match_single = filtered.FirstMatch(text, atoms_single);
            }
            
            std::vector<int> atoms_all;
            for (int i = 0; i < num_regexps; ++i) {
                atoms_all.push_back(i);
            }
            int first_match_all = filtered.FirstMatch(text, atoms_all);
            
            // Test with different text variations
            std::string text_reversed(text.rbegin(), text.rend());
            int match_reversed = filtered.SlowFirstMatch(text_reversed);
            
            std::string text_uppercase = text;
            for (auto& c : text_uppercase) c = toupper(c);
            int match_uppercase = filtered.FirstMatch(text_uppercase, atoms_all);
        }
    }
    
    // Additional test: Create standalone RE2 objects
    try {
        re2::RE2 standalone_re(pattern, options);
        if (standalone_re.ok() && !text.empty()) {
            bool standalone_match = standalone_re.Match(text, 0, text.size(), 
                                                       re2::RE2::UNANCHORED, nullptr, 0);
        }
    } catch (...) {
        // Handle exceptions silently
    }
    
    return 0;
}