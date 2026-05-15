// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::FilteredRE2::AllMatches at filtered_re2.cc:112:19 in filtered_re2.h
// re2::FilteredRE2::SlowFirstMatch at filtered_re2.cc:91:18 in filtered_re2.h
// re2::FilteredRE2::AllPotentials at filtered_re2.cc:124:19 in filtered_re2.h
// re2::FilteredRE2::FirstMatch at filtered_re2.cc:98:18 in filtered_re2.h
// re2::FilteredRE2::AllMatches at filtered_re2.cc:112:19 in filtered_re2.h
// re2::FilteredRE2::Add at filtered_re2.cc:51:29 in filtered_re2.h
// re2::FilteredRE2::Compile at filtered_re2.cc:70:19 in filtered_re2.h
// re2::FilteredRE2::AllPotentials at filtered_re2.cc:124:19 in filtered_re2.h
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
#include <cstdint>
#include <cstddef>
#include <vector>
#include <string>
#include <cstring>
#include <cstdlib>

extern "C" int LLVMFuzzerTestOneInput_49(const uint8_t *Data, size_t Size) {
    // Early exit for very small inputs
    if (Size < 2) return 0;
    
    // Initialize FilteredRE2 object
    re2::FilteredRE2 filtered_re2;
    
    // Parse input data to create patterns and text
    // First byte: number of patterns to add (max 5 to avoid explosion)
    size_t num_patterns = Data[0] % 5;
    if (num_patterns == 0) num_patterns = 1;
    
    size_t offset = 1;
    std::vector<int> pattern_ids;
    std::vector<std::string> patterns;
    
    // Add patterns to FilteredRE2
    for (size_t i = 0; i < num_patterns && offset < Size; i++) {
        // Next byte: pattern length (max 50 bytes)
        size_t pattern_len = Data[offset] % 50;
        offset++;
        
        if (offset + pattern_len > Size) break;
        
        std::string pattern(reinterpret_cast<const char*>(Data + offset), pattern_len);
        offset += pattern_len;
        
        // Add pattern to FilteredRE2
        int id = -1;
        re2::RE2::Options options;
        re2::RE2::ErrorCode error = filtered_re2.Add(pattern, options, &id);
        
        if (error == re2::RE2::NoError && id >= 0) {
            pattern_ids.push_back(id);
            patterns.push_back(pattern);
        }
        
        // If we're running out of data, break
        if (offset >= Size) break;
    }
    
    // If we have no valid patterns, return early
    if (pattern_ids.empty()) return 0;
    
    // Get remaining data for text
    size_t text_len = Size - offset;
    if (text_len > 500) text_len = 500; // Limit text size
    
    absl::string_view text;
    if (text_len > 0) {
        text = absl::string_view(reinterpret_cast<const char*>(Data + offset), text_len);
    } else {
        text = absl::string_view("");
    }
    
    // Compile the FilteredRE2
    std::vector<std::string> strings_to_match;
    filtered_re2.Compile(&strings_to_match);
    
    // Prepare atoms vector - use indices from 0 to strings_to_match.size()-1
    std::vector<int> atoms;
    if (!strings_to_match.empty()) {
        // Create some atom indices based on the data
        size_t num_atoms = Data[0] % (strings_to_match.size() + 1);
        for (size_t i = 0; i < num_atoms && i < strings_to_match.size(); i++) {
            atoms.push_back(i);
        }
    }
    
    // Test AllPotentials
    std::vector<int> potential_regexps;
    filtered_re2.AllPotentials(atoms, &potential_regexps);
    
    // Test FirstMatch - only if Compile was successful and atoms is valid
    if (!strings_to_match.empty()) {
        int first_match = filtered_re2.FirstMatch(text, atoms);
        (void)first_match; // Mark as used
    }
    
    // Test AllMatches - only if Compile was successful and atoms is valid
    if (!strings_to_match.empty()) {
        std::vector<int> matching_regexps;
        bool any_matches = filtered_re2.AllMatches(text, atoms, &matching_regexps);
        (void)any_matches; // Mark as used
    }
    
    // Test SlowFirstMatch - this doesn't require Compile or atoms
    int slow_first_match = filtered_re2.SlowFirstMatch(text);
    (void)slow_first_match; // Mark as used
    
    // Also test with empty atoms vector
    std::vector<int> empty_atoms;
    std::vector<int> empty_potential_regexps;
    filtered_re2.AllPotentials(empty_atoms, &empty_potential_regexps);
    
    // Only call FirstMatch and AllMatches with empty atoms if we have compiled
    if (!strings_to_match.empty()) {
        int empty_first_match = filtered_re2.FirstMatch(text, empty_atoms);
        (void)empty_first_match;
        
        std::vector<int> empty_matching_regexps;
        bool empty_any_matches = filtered_re2.AllMatches(text, empty_atoms, &empty_matching_regexps);
        (void)empty_any_matches;
    }
    
    return 0;
}