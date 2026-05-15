// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::FilteredRE2::Add at filtered_re2.cc:51:29 in filtered_re2.h
// re2::FilteredRE2::Compile at filtered_re2.cc:70:19 in filtered_re2.h
// re2::FilteredRE2::AllPotentials at filtered_re2.cc:124:19 in filtered_re2.h
// re2::FilteredRE2::Add at filtered_re2.cc:51:29 in filtered_re2.h
// re2::FilteredRE2::Compile at filtered_re2.cc:70:19 in filtered_re2.h
// re2::FilteredRE2::AllMatches at filtered_re2.cc:112:19 in filtered_re2.h
// re2::RE2::Set::Add at set.cc:64:15 in set.h
// re2::RE2::Set::Compile at set.cc:103:16 in set.h
// re2::RE2::Set::Match at set.cc:133:16 in set.h
// re2::RE2::Set::Match at set.cc:133:16 in set.h
// re2::RE2::Set::Add at set.cc:64:15 in set.h
// re2::RE2::Set::Compile at set.cc:103:16 in set.h
// re2::RE2::Set::Match at set.cc:133:16 in set.h
// re2::FilteredRE2::Add at filtered_re2.cc:51:29 in filtered_re2.h
// re2::FilteredRE2::Compile at filtered_re2.cc:70:19 in filtered_re2.h
// re2::FilteredRE2::AllPotentials at filtered_re2.cc:124:19 in filtered_re2.h
// re2::FilteredRE2::FirstMatch at filtered_re2.cc:98:18 in filtered_re2.h
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
#include "re2/filtered_re2.h"
#include "re2/set.h"

#include <cstdint>
#include <cstring>
#include <string>
#include <vector>

static std::vector<int> parseAtomsFromData(const uint8_t *Data, size_t Size, size_t max_atoms) {
    std::vector<int> atoms;
    if (Size == 0) return atoms;
    
    size_t atom_count = std::min(Size, max_atoms);
    atoms.reserve(atom_count);
    
    for (size_t i = 0; i < atom_count; ++i) {
        atoms.push_back(static_cast<int>(Data[i]));
    }
    
    return atoms;
}

static std::string extractString(const uint8_t *Data, size_t Size, size_t offset) {
    if (offset >= Size) return "";
    size_t remaining = Size - offset;
    size_t str_len = std::min(remaining, static_cast<size_t>(256));
    return std::string(reinterpret_cast<const char*>(Data + offset), str_len);
}

extern "C" int LLVMFuzzerTestOneInput_39(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    // Split data for different purposes
    size_t pattern_offset = Size / 3;
    size_t text_offset = 2 * Size / 3;
    
    std::string pattern = extractString(Data, Size, 0);
    std::string text = extractString(Data, Size, text_offset);
    
    // Test RE2::Set::Match
    {
        re2::RE2::Options options(re2::RE2::DefaultOptions);
        re2::RE2::Set set(options, re2::RE2::UNANCHORED);
        std::string error;
        int pattern_id = set.Add(pattern, &error);
        if (pattern_id >= 0) {
            if (set.Compile()) {
                std::vector<int> matches;
                set.Match(text, &matches);
            }
        }
    }
    
    // Test FilteredRE2 functions
    {
        re2::FilteredRE2 filtered_re2(1); // min_atom_len = 1
        
        // Add multiple patterns
        std::vector<int> pattern_ids;
        for (int i = 0; i < 3 && i * pattern_offset < Size; ++i) {
            std::string pat = extractString(Data, Size, i * pattern_offset);
            int id;
            re2::RE2::ErrorCode err = filtered_re2.Add(pat, re2::RE2::DefaultOptions, &id);
            if (err == re2::RE2::NoError) {
                pattern_ids.push_back(id);
            }
        }
        
        if (!pattern_ids.empty()) {
            std::vector<std::string> strings_to_match;
            filtered_re2.Compile(&strings_to_match);
            
            // Generate atoms from data
            std::vector<int> atoms = parseAtomsFromData(Data, Size, 10);
            
            if (!atoms.empty()) {
                std::vector<int> potential_regexps;
                filtered_re2.AllPotentials(atoms, &potential_regexps);
                
                int first_match = filtered_re2.FirstMatch(text, atoms);
                
                std::vector<int> matching_regexps;
                filtered_re2.AllMatches(text, atoms, &matching_regexps);
            }
        }
    }
    
    // Test with different options
    {
        re2::FilteredRE2 filtered_re2_latin1(2);
        re2::RE2::Options latin1_options(re2::RE2::Latin1);
        
        int id;
        re2::RE2::ErrorCode err = filtered_re2_latin1.Add(pattern, latin1_options, &id);
        if (err == re2::RE2::NoError) {
            std::vector<std::string> strings_to_match;
            filtered_re2_latin1.Compile(&strings_to_match);
            
            std::vector<int> atoms = parseAtomsFromData(Data, Size, 5);
            if (!atoms.empty()) {
                std::vector<int> potential_regexps;
                filtered_re2_latin1.AllPotentials(atoms, &potential_regexps);
            }
        }
    }
    
    // Test with POSIX options
    {
        re2::FilteredRE2 filtered_re2_posix(3);
        re2::RE2::Options posix_options(re2::RE2::POSIX);
        
        int id;
        re2::RE2::ErrorCode err = filtered_re2_posix.Add(pattern, posix_options, &id);
        if (err == re2::RE2::NoError) {
            std::vector<std::string> strings_to_match;
            filtered_re2_posix.Compile(&strings_to_match);
            
            std::vector<int> atoms = parseAtomsFromData(Data, Size, 8);
            if (!atoms.empty()) {
                std::vector<int> matching_regexps;
                filtered_re2_posix.AllMatches(text, atoms, &matching_regexps);
            }
        }
    }
    
    // Test RE2::Set with multiple patterns
    {
        re2::RE2::Options quiet_options(re2::RE2::Quiet);
        re2::RE2::Set set2(quiet_options, re2::RE2::UNANCHORED);
        std::vector<int> ids;
        
        for (int i = 0; i < 2 && i * pattern_offset < Size; ++i) {
            std::string pat = extractString(Data, Size, i * pattern_offset);
            std::string error;
            int id = set2.Add(pat, &error);
            if (id >= 0) {
                ids.push_back(id);
            }
        }
        
        if (!ids.empty() && set2.Compile()) {
            std::vector<int> matches;
            set2.Match(text, &matches);
            
            // Test with empty vector
            set2.Match(text, nullptr);
        }
    }
    
    return 0;
}