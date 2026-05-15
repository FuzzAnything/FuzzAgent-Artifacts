// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::FilteredRE2::Add at filtered_re2.cc:51:29 in filtered_re2.h
// re2::FilteredRE2::Compile at filtered_re2.cc:70:19 in filtered_re2.h
// re2::FilteredRE2::AllMatches at filtered_re2.cc:112:19 in filtered_re2.h
// re2::RE2::Set::Add at set.cc:64:15 in set.h
// re2::RE2::Set::Compile at set.cc:103:16 in set.h
// re2::RE2::Set::Match at set.cc:133:16 in set.h
// re2::RE2::Set::Match at set.cc:137:16 in set.h
// re2::RE2::Set::Size at set.cc:58:15 in set.h
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
#include <algorithm>

#include "re2/set.h"
#include "re2/filtered_re2.h"

extern "C" int LLVMFuzzerTestOneInput_47(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    // Parse input: first byte determines number of patterns (0-10)
    size_t num_patterns = std::min<size_t>((Data[0] % 11), 10);
    if (num_patterns == 0) num_patterns = 1;
    
    const uint8_t* pattern_data = Data + 1;
    size_t pattern_data_size = Size - 1;
    
    // Split remaining data into patterns and text
    std::vector<std::string> patterns;
    std::string text;
    
    size_t pos = 0;
    for (size_t i = 0; i < num_patterns && pos < pattern_data_size; ++i) {
        // Find next null byte or end of data
        size_t pattern_len = 0;
        while (pos + pattern_len < pattern_data_size && 
               pattern_data[pos + pattern_len] != 0) {
            ++pattern_len;
        }
        
        if (pattern_len > 0) {
            patterns.emplace_back(
                reinterpret_cast<const char*>(pattern_data + pos),
                pattern_len
            );
        } else {
            patterns.emplace_back(".*");  // Default pattern if empty
        }
        
        pos += pattern_len;
        if (pos < pattern_data_size && pattern_data[pos] == 0) {
            ++pos;  // Skip null terminator
        }
    }
    
    // Remaining data is text
    if (pos < pattern_data_size) {
        text.assign(
            reinterpret_cast<const char*>(pattern_data + pos),
            pattern_data_size - pos
        );
    }
    
    // Test RE2::Set
    {
        re2::RE2::Set set(re2::RE2::DefaultOptions, re2::RE2::ANCHOR_BOTH);
        
        // Add patterns
        for (const auto& pattern : patterns) {
            std::string error;
            set.Add(pattern, &error);
        }
        
        // Compile
        bool compiled = set.Compile();
        
        if (compiled) {
            // Test Match overload without ErrorInfo
            std::vector<int> matches1;
            set.Match(text, &matches1);
            
            // Test Match overload with ErrorInfo
            std::vector<int> matches2;
            re2::RE2::Set::ErrorInfo error_info;
            set.Match(text, &matches2, &error_info);
            
            // Test Size
            (void)set.Size();
        }
    }
    
    // Test FilteredRE2
    {
        // Use second byte for min_atom_len (1-10)
        int min_atom_len = (Data[1] % 10) + 1;
        re2::FilteredRE2 filtered_re2(min_atom_len);
        
        // Add patterns to FilteredRE2
        std::vector<int> pattern_ids;
        for (const auto& pattern : patterns) {
            int id;
            re2::RE2::ErrorCode error = filtered_re2.Add(pattern, re2::RE2::DefaultOptions, &id);
            if (error == re2::RE2::NoError) {
                pattern_ids.push_back(id);
            }
        }
        
        // Compile the prefilter tree
        std::vector<std::string> atoms;
        filtered_re2.Compile(&atoms);
        
        if (!pattern_ids.empty()) {
            // Create atoms vector for AllMatches
            // Use a subset of pattern indices as atoms
            std::vector<int> atoms_for_match;
            for (size_t i = 0; i < std::min(pattern_ids.size(), size_t(5)); ++i) {
                atoms_for_match.push_back(pattern_ids[i % pattern_ids.size()]);
            }
            
            // Test AllMatches
            std::vector<int> matching_regexps;
            filtered_re2.AllMatches(text, atoms_for_match, &matching_regexps);
        }
    }
    
    return 0;
}