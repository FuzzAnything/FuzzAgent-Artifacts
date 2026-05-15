// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::FilteredRE2::AllMatches at filtered_re2.cc:112:19 in filtered_re2.h
// re2::RE2::Set::Add at set.cc:64:15 in set.h
// re2::RE2::Set::Compile at set.cc:103:16 in set.h
// re2::RE2::Set::Match at set.cc:133:16 in set.h
// re2::FilteredRE2::Add at filtered_re2.cc:51:29 in filtered_re2.h
// re2::FilteredRE2::Compile at filtered_re2.cc:70:19 in filtered_re2.h
// re2::FilteredRE2::AllMatches at filtered_re2.cc:112:19 in filtered_re2.h
// re2::RE2::Options::set_encoding at re2.h:700:10 in re2.h
// re2::FilteredRE2::Add at filtered_re2.cc:51:29 in filtered_re2.h
// re2::FilteredRE2::Compile at filtered_re2.cc:70:19 in filtered_re2.h
// re2::FilteredRE2::AllMatches at filtered_re2.cc:112:19 in filtered_re2.h
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
#include <vector>
#include <string>

#include "re2/set.h"
#include "re2/filtered_re2.h"

extern "C" int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    // Split input for different purposes
    size_t pattern_size = Size / 2;
    size_t text_size = Size - pattern_size;
    
    std::string pattern(reinterpret_cast<const char*>(Data), pattern_size);
    std::string text(reinterpret_cast<const char*>(Data + pattern_size), text_size);

    // 1. Test RE2::Set::Compile and RE2::Set::Match
    {
        re2::RE2::Options options;
        re2::RE2::Set set(options, re2::RE2::ANCHOR_BOTH);
        
        std::string error;
        int pattern_id = set.Add(pattern, &error);
        
        if (set.Compile()) {
            std::vector<int> matches;
            set.Match(text, &matches);
        }
    }

    // 2. Test FilteredRE2 functions
    {
        re2::FilteredRE2 filtered_re2;
        re2::RE2::Options options;
        int id;
        
        // Add pattern to FilteredRE2
        re2::RE2::ErrorCode add_error = filtered_re2.Add(pattern, options, &id);
        
        if (add_error == re2::RE2::NoError) {
            std::vector<std::string> strings_to_match;
            filtered_re2.Compile(&strings_to_match);
            
            std::vector<int> atoms;
            std::vector<int> matching_regexps;
            
            // Add the pattern ID to atoms if we got a valid ID
            if (id >= 0) {
                atoms.push_back(id);
            }
            
            filtered_re2.AllMatches(text, atoms, &matching_regexps);
        }
    }

    // 3. Test with different options
    {
        re2::RE2::Options options_latin1;
        options_latin1.set_encoding(re2::RE2::Options::EncodingLatin1);
        
        re2::FilteredRE2 filtered_re2_latin1;
        int id_latin1;
        filtered_re2_latin1.Add(pattern, options_latin1, &id_latin1);
        
        std::vector<std::string> strings_latin1;
        filtered_re2_latin1.Compile(&strings_latin1);
        
        std::vector<int> atoms_latin1;
        std::vector<int> matches_latin1;
        if (id_latin1 >= 0) {
            atoms_latin1.push_back(id_latin1);
        }
        filtered_re2_latin1.AllMatches(text, atoms_latin1, &matches_latin1);
    }

    // 4. Test with empty strings
    {
        re2::FilteredRE2 filtered_empty;
        re2::RE2::Options options;
        int empty_id;
        
        // Try with empty pattern
        filtered_empty.Add("", options, &empty_id);
        
        std::vector<std::string> empty_strings;
        filtered_empty.Compile(&empty_strings);
        
        std::vector<int> empty_atoms;
        std::vector<int> empty_matches;
        filtered_empty.AllMatches("", empty_atoms, &empty_matches);
    }

    return 0;
}