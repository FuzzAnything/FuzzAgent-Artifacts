// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::RE2::Options::set_encoding at re2.h:700:10 in re2.h
// re2::RE2::Set::Add at set.cc:64:15 in set.h
// re2::RE2::Set::Add at set.cc:64:15 in set.h
// re2::RE2::Set::Compile at set.cc:103:16 in set.h
// re2::RE2::Set::Match at set.cc:133:16 in set.h
// re2::RE2::Options::set_encoding at re2.h:700:10 in re2.h
// re2::FilteredRE2::Add at filtered_re2.cc:51:29 in filtered_re2.h
// re2::FilteredRE2::Add at filtered_re2.cc:51:29 in filtered_re2.h
// re2::FilteredRE2::Compile at filtered_re2.cc:70:19 in filtered_re2.h
// re2::FilteredRE2::AllMatches at filtered_re2.cc:112:19 in filtered_re2.h
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
#include <cstddef>
#include <vector>
#include <string>
#include <cstring>

#include "re2/set.h"
#include "re2/filtered_re2.h"

static void WriteDummyFile(const uint8_t* data, size_t size) {
    FILE* f = fopen("./dummy_file", "wb");
    if (!f) return;
    fwrite(data, 1, size, f);
    fclose(f);
}

extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t* Data, size_t Size) {
    if (Size < 2) return 0;

    // Split input for different purposes
    size_t split1 = Size / 3;
    size_t split2 = 2 * Size / 3;
    
    // Create patterns from first part
    std::string pattern1(reinterpret_cast<const char*>(Data), split1);
    std::string pattern2(reinterpret_cast<const char*>(Data + split1), split2 - split1);
    
    // Text to match against
    absl::string_view text(reinterpret_cast<const char*>(Data + split2), Size - split2);
    
    // Test RE2::Set
    {
        re2::RE2::Options options;
        options.set_encoding(re2::RE2::Options::EncodingUTF8);
        
        // Create RE2::Set using move constructor
        re2::RE2::Set set1(options, re2::RE2::UNANCHORED);
        re2::RE2::Set set(std::move(set1));
        
        std::string error1, error2;
        // Add patterns - RE2::Set::Add returns index or -1 on error
        int id1 = set.Add(pattern1, &error1);
        int id2 = set.Add(pattern2, &error2);
        
        if (id1 != -1 && id2 != -1) {
            // Compile the set
            if (set.Compile()) {
                std::vector<int> matches;
                // Match against text
                bool matched = set.Match(text, &matches);
                (void)matched; // Use result to avoid optimization
            }
        }
    }
    
    // Test FilteredRE2
    {
        // Create FilteredRE2 with minimum atom length
        re2::FilteredRE2 filtered_re2(3);
        
        re2::RE2::Options options;
        options.set_encoding(re2::RE2::Options::EncodingUTF8);
        
        int id1, id2;
        // Add patterns to FilteredRE2
        re2::RE2::ErrorCode err1 = filtered_re2.Add(pattern1, options, &id1);
        re2::RE2::ErrorCode err2 = filtered_re2.Add(pattern2, options, &id2);
        
        // Only proceed if at least one pattern was added successfully
        if (err1 == re2::RE2::NoError || err2 == re2::RE2::NoError) {
            std::vector<std::string> strings_to_match;
            // Compile FilteredRE2
            filtered_re2.Compile(&strings_to_match);
            
            // Prepare atoms (indices of successfully added patterns)
            std::vector<int> atoms;
            if (err1 == re2::RE2::NoError) atoms.push_back(id1);
            if (err2 == re2::RE2::NoError) atoms.push_back(id2);
            
            if (!atoms.empty()) {
                std::vector<int> matching_regexps;
                // Test AllMatches
                bool all_matched = filtered_re2.AllMatches(text, atoms, &matching_regexps);
                (void)all_matched; // Use result to avoid optimization
            }
        }
    }
    
    // Test with file input
    WriteDummyFile(Data, Size);
    
    // Additional test: create RE2 directly
    try {
        re2::RE2 re2_direct(pattern1);
        (void)re2_direct; // Use to avoid optimization
    } catch (...) {
        // Ignore exceptions
    }
    
    return 0;
}