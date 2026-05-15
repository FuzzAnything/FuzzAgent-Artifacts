// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::FilteredRE2::AllPotentials at filtered_re2.cc:124:19 in filtered_re2.h
// re2::FilteredRE2::AllMatches at filtered_re2.cc:112:19 in filtered_re2.h
// re2::FilteredRE2::FirstMatch at filtered_re2.cc:98:18 in filtered_re2.h
// re2::FilteredRE2::SlowFirstMatch at filtered_re2.cc:91:18 in filtered_re2.h
// re2::RE2::Options::set_encoding at re2.h:700:10 in re2.h
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
#include <vector>
#include <string>
#include <algorithm>

#include "re2/filtered_re2.h"

extern "C" int LLVMFuzzerTestOneInput_27(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Initialize FilteredRE2 with min_atom_len derived from input
    int min_atom_len = (Data[0] % 10) + 1; // Range 1-10
    re2::FilteredRE2 filtered_re2(min_atom_len);

    // Split input data into potential patterns and text
    // Use first half for patterns, second half for text
    size_t split_point = Size / 2;
    const uint8_t* pattern_data = Data;
    size_t pattern_size = split_point;
    const uint8_t* text_data = Data + split_point;
    size_t text_size = Size - split_point;

    // Create patterns from pattern_data (split by null bytes if available)
    std::vector<std::string> patterns;
    const uint8_t* start = pattern_data;
    const uint8_t* end = pattern_data + pattern_size;
    
    for (const uint8_t* ptr = start; ptr < end; ++ptr) {
        if (*ptr == 0 || ptr == end - 1) {
            size_t len = ptr - start;
            if (len > 0 && len <= 1000) {  // Limit pattern length
                patterns.emplace_back(reinterpret_cast<const char*>(start), len);
            }
            start = ptr + 1;
        }
    }

    // Add patterns to FilteredRE2
    std::vector<int> pattern_ids;
    re2::RE2::Options options;
    options.set_encoding(Size % 2 == 0 ? re2::RE2::Options::EncodingUTF8 : 
                                        re2::RE2::Options::EncodingLatin1);
    
    for (const auto& pattern : patterns) {
        int id = -1;
        re2::RE2::ErrorCode error = filtered_re2.Add(pattern, options, &id);
        if (error == re2::RE2::NoError && id >= 0) {
            pattern_ids.push_back(id);
        }
    }

    // Only compile if we have added at least one pattern
    bool has_compiled = false;
    std::vector<std::string> strings_to_match;
    if (!pattern_ids.empty()) {
        try {
            filtered_re2.Compile(&strings_to_match);
            has_compiled = true;
        } catch (...) {
            // Catch any exceptions during compilation
            return 0;
        }
    }

    // Create text for matching
    std::string text;
    if (text_size > 0) {
        text.assign(reinterpret_cast<const char*>(text_data), 
                   std::min(text_size, static_cast<size_t>(1000)));
    }

    // Generate atoms from strings_to_match indices, not pattern_ids
    std::vector<int> atoms;
    if (has_compiled && !strings_to_match.empty()) {
        // Use indices into strings_to_match as atoms
        // Limit to valid indices to avoid out-of-bounds access
        size_t num_atoms = std::min(strings_to_match.size(), static_cast<size_t>(10));
        for (size_t i = 0; i < num_atoms; ++i) {
            atoms.push_back(i);
        }
    }

    // Test AllPotentials only if we have compiled and atoms
    if (has_compiled && !atoms.empty()) {
        std::vector<int> potential_regexps;
        try {
            // Ensure atoms are valid indices
            for (int atom : atoms) {
                if (atom < 0 || static_cast<size_t>(atom) >= strings_to_match.size()) {
                    return 0; // Invalid atom index, skip this input
                }
            }
            filtered_re2.AllPotentials(atoms, &potential_regexps);
        } catch (...) {
            // Ignore exceptions
        }
    }

    // Test AllMatches only if we have compiled, text, and atoms
    if (has_compiled && !text.empty() && !atoms.empty()) {
        std::vector<int> matching_regexps;
        try {
            // Ensure atoms are valid indices
            for (int atom : atoms) {
                if (atom < 0 || static_cast<size_t>(atom) >= strings_to_match.size()) {
                    return 0; // Invalid atom index, skip this input
                }
            }
            filtered_re2.AllMatches(text, atoms, &matching_regexps);
        } catch (...) {
            // Ignore exceptions
        }
    }

    // Test FirstMatch only if we have compiled, text, and atoms
    if (has_compiled && !text.empty() && !atoms.empty()) {
        try {
            // Ensure atoms are valid indices
            for (int atom : atoms) {
                if (atom < 0 || static_cast<size_t>(atom) >= strings_to_match.size()) {
                    return 0; // Invalid atom index, skip this input
                }
            }
            int first_match = filtered_re2.FirstMatch(text, atoms);
            (void)first_match;
        } catch (...) {
            // Ignore exceptions
        }
    }

    // Test SlowFirstMatch (can be called before or after Compile)
    if (!text.empty()) {
        try {
            int slow_first_match = filtered_re2.SlowFirstMatch(text);
            (void)slow_first_match;
        } catch (...) {
            // Ignore exceptions
        }
    }

    return 0;
}