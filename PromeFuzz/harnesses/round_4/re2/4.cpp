// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::FilteredRE2::AllMatches at filtered_re2.cc:112:19 in filtered_re2.h
// re2::FilteredRE2::AllMatches at filtered_re2.cc:112:19 in filtered_re2.h
// re2::FilteredRE2::AllMatches at filtered_re2.cc:112:19 in filtered_re2.h
// re2::FilteredRE2::AllMatches at filtered_re2.cc:112:19 in filtered_re2.h
// re2::FilteredRE2::AllMatches at filtered_re2.cc:112:19 in filtered_re2.h
// re2::FilteredRE2::AllMatches at filtered_re2.cc:112:19 in filtered_re2.h
// re2::RE2::Options::set_encoding at re2.h:700:10 in re2.h
// re2::RE2::Set::Add at set.cc:64:15 in set.h
// re2::RE2::Set::Compile at set.cc:103:16 in set.h
// re2::RE2::Set::Match at set.cc:133:16 in set.h
// re2::RE2::Set::Match at set.cc:133:16 in set.h
// re2::RE2::Set::Match at set.cc:133:16 in set.h
// re2::RE2::Set::Match at set.cc:133:16 in set.h
// re2::RE2::Options::set_encoding at re2.h:700:10 in re2.h
// re2::FilteredRE2::Add at filtered_re2.cc:51:29 in filtered_re2.h
// re2::RE2::Options::set_encoding at re2.h:700:10 in re2.h
// re2::FilteredRE2::Add at filtered_re2.cc:51:29 in filtered_re2.h
// re2::FilteredRE2::Add at filtered_re2.cc:51:29 in filtered_re2.h
// re2::FilteredRE2::Add at filtered_re2.cc:51:29 in filtered_re2.h
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
#include <cstdint>
#include <cstring>
#include <vector>
#include <string>

#include "re2/set.h"
#include "re2/filtered_re2.h"

extern "C" int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Split input into parts for different purposes
    // First part: pattern for RE2::Set (up to half of input)
    size_t set_pattern_size = Size / 2;
    std::string set_pattern(reinterpret_cast<const char*>(Data), set_pattern_size);
    
    // Second part: text for matching (remaining input)
    size_t text_size = Size - set_pattern_size;
    absl::string_view match_text(reinterpret_cast<const char*>(Data + set_pattern_size), text_size);
    
    // Third part: pattern for FilteredRE2 (use first 256 bytes or available data)
    size_t filtered_pattern_size = (Size > set_pattern_size + 256) ? 256 : (Size > set_pattern_size ? Size - set_pattern_size : 0);
    std::string filtered_pattern;
    if (filtered_pattern_size > 0) {
        filtered_pattern = std::string(reinterpret_cast<const char*>(Data + set_pattern_size), filtered_pattern_size);
    } else {
        filtered_pattern = "test.*pattern";
    }

    // 1. Create and use RE2::Set
    {
        re2::RE2::Options options;
        options.set_encoding(re2::RE2::Options::EncodingUTF8);
        
        re2::RE2::Set set(options, re2::RE2::Anchor::UNANCHORED);
        
        // Add pattern to set
        std::string error;
        int pattern_id = set.Add(set_pattern, &error);
        
        // Compile the set (target function 1)
        if (set.Compile()) {
            std::vector<int> matches;
            
            // Match against text (target function 2)
            bool matched = set.Match(match_text, &matches);
            
            // Try with NULL vector
            set.Match(match_text, nullptr);
            
            // Try with empty string
            set.Match("", &matches);
            
            // Try with various string views
            if (text_size > 0) {
                // Test with substring
                absl::string_view sub_text = match_text.substr(0, text_size / 2);
                set.Match(sub_text, &matches);
            }
        }
    }

    // 2. Create and use FilteredRE2
    {
        // Create FilteredRE2 with different min_atom_len values
        re2::FilteredRE2 filtered_re2(3);
        
        int id;
        re2::RE2::Options options;
        options.set_encoding(re2::RE2::Options::EncodingUTF8);
        
        // Add pattern (target function 3)
        re2::RE2::ErrorCode err = filtered_re2.Add(filtered_pattern, options, &id);
        
        // Add some additional patterns with different options
        re2::RE2::Options options2;
        options2.set_encoding(re2::RE2::Options::EncodingLatin1);
        filtered_re2.Add("another.*pattern", options2, &id);
        
        filtered_re2.Add("[a-z]+", options, &id);
        filtered_re2.Add("\\d{3,5}", options, &id);
        
        // Compile (target function 4)
        std::vector<std::string> strings_to_match;
        filtered_re2.Compile(&strings_to_match);
        
        // Prepare atoms vector (indices of patterns to check)
        std::vector<int> atoms;
        for (int i = 0; i < 3; ++i) {
            atoms.push_back(i);
        }
        
        // AllMatches (target function 5)
        std::vector<int> matching_regexps;
        bool any_matches = filtered_re2.AllMatches(match_text, atoms, &matching_regexps);
        
        // Test with empty atoms vector
        std::vector<int> empty_atoms;
        filtered_re2.AllMatches(match_text, empty_atoms, &matching_regexps);
        
        // Test with empty text
        filtered_re2.AllMatches("", atoms, &matching_regexps);
        
        // Test with NULL matching_regexps
        filtered_re2.AllMatches(match_text, atoms, nullptr);
        
        // Test with various atom combinations
        if (atoms.size() > 1) {
            std::vector<int> single_atom = {0};
            filtered_re2.AllMatches(match_text, single_atom, &matching_regexps);
            
            std::vector<int> reversed_atoms = {2, 1, 0};
            filtered_re2.AllMatches(match_text, reversed_atoms, &matching_regexps);
        }
    }

    return 0;
}