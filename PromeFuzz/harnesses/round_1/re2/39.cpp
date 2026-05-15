// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::FilteredRE2::Compile at filtered_re2.cc:70:19 in filtered_re2.h
// re2::FilteredRE2::AllMatches at filtered_re2.cc:112:19 in filtered_re2.h
// re2::RE2::Set::Add at set.cc:64:15 in set.h
// re2::RE2::Set::Compile at set.cc:103:16 in set.h
// re2::RE2::Set::Match at set.cc:133:16 in set.h
// re2::RE2::Set::Match at set.cc:137:16 in set.h
// re2::RE2::Set::Match at set.cc:133:16 in set.h
// re2::RE2::Set::Match at set.cc:137:16 in set.h
// re2::RE2::Set::Size at set.cc:58:15 in set.h
// re2::FilteredRE2::Add at filtered_re2.cc:51:29 in filtered_re2.h
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
#include <memory>

#include "re2/set.h"
#include "re2/filtered_re2.h"

extern "C" int LLVMFuzzerTestOneInput_39(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    // Split input into parts: first byte determines number of patterns,
    // then patterns separated by null bytes, remaining data is text
    const uint8_t* ptr = Data;
    const uint8_t* end = Data + Size;
    
    if (ptr >= end) return 0;
    uint8_t num_patterns = *ptr % 10; // Limit to reasonable number of patterns
    ptr++;
    
    if (num_patterns == 0) return 0;
    
    // Extract patterns
    std::vector<std::string> patterns;
    patterns.reserve(num_patterns);
    
    for (int i = 0; i < num_patterns && ptr < end; i++) {
        const uint8_t* pattern_start = ptr;
        while (ptr < end && *ptr != 0) {
            ptr++;
        }
        if (ptr > pattern_start) {
            patterns.emplace_back(reinterpret_cast<const char*>(pattern_start), 
                                 ptr - pattern_start);
        }
        if (ptr < end) ptr++; // Skip null terminator
    }
    
    if (patterns.empty()) return 0;
    
    // Remaining data is text
    absl::string_view text(reinterpret_cast<const char*>(ptr), end - ptr);
    
    // Test RE2::Set
    {
        re2::RE2::Set set(re2::RE2::Options(), re2::RE2::UNANCHORED);
        
        // Add patterns
        for (const auto& pattern : patterns) {
            std::string error;
            set.Add(pattern, &error);
        }
        
        // Compile
        if (set.Compile()) {
            // Test both Match overloads
            std::vector<int> matches;
            
            // First overload without ErrorInfo
            set.Match(text, &matches);
            
            // Second overload with ErrorInfo
            re2::RE2::Set::ErrorInfo error_info;
            set.Match(text, &matches, &error_info);
            
            // Test without vector
            set.Match(text, nullptr);
            set.Match(text, nullptr, &error_info);
        }
        
        // Test Size()
        (void)set.Size();
    }
    
    // Test FilteredRE2
    {
        // Use first byte to determine min_atom_len
        int min_atom_len = (patterns[0].empty() ? 3 : patterns[0][0] % 10 + 1);
        re2::FilteredRE2 filtered_re2(min_atom_len);
        
        std::vector<int> pattern_ids;
        pattern_ids.reserve(patterns.size());
        
        // Add patterns to FilteredRE2 with correct signature
        for (const auto& pattern : patterns) {
            int id;
            re2::RE2::ErrorCode error_code = filtered_re2.Add(pattern, re2::RE2::Options(), &id);
            if (error_code == re2::RE2::NoError) {
                pattern_ids.push_back(id);
            }
        }
        
        if (!pattern_ids.empty()) {
            // Compile to get strings to match
            std::vector<std::string> strings_to_match;
            filtered_re2.Compile(&strings_to_match);
            
            // Create atoms vector from pattern_ids
            std::vector<int> atoms;
            for (int id : pattern_ids) {
                atoms.push_back(id);
            }
            
            // Test AllMatches
            std::vector<int> matching_regexps;
            filtered_re2.AllMatches(text, atoms, &matching_regexps);
        }
    }
    
    return 0;
}