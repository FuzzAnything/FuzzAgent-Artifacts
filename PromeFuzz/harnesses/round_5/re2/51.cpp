// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::FilteredRE2::SlowFirstMatch at filtered_re2.cc:91:18 in filtered_re2.h
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
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>

#include "re2/filtered_re2.h"

extern "C" int LLVMFuzzerTestOneInput_51(const uint8_t *Data, size_t Size) {
    // Early exit if no data
    if (Size == 0) {
        return 0;
    }

    // Create FilteredRE2 instance
    re2::FilteredRE2 filtered_re2;

    // Parse input to get patterns and text
    // Simple format: first byte = number of patterns (n), then n null-terminated patterns,
    // then remaining bytes are text
    if (Size < 1) {
        return 0;
    }

    uint8_t num_patterns = Data[0];
    size_t offset = 1;
    std::vector<std::string> patterns;
    std::vector<int> pattern_ids;

    // Extract patterns
    for (int i = 0; i < num_patterns && offset < Size; i++) {
        // Find null terminator
        size_t pattern_start = offset;
        while (offset < Size && Data[offset] != 0) {
            offset++;
        }
        
        if (offset >= Size) {
            break; // No null terminator found
        }
        
        size_t pattern_len = offset - pattern_start;
        if (pattern_len > 0) {
            patterns.push_back(std::string(reinterpret_cast<const char*>(Data + pattern_start), pattern_len));
        }
        offset++; // Skip null terminator
    }

    // Remaining data is text
    size_t text_len = Size - offset;
    std::string text;
    if (text_len > 0) {
        text = std::string(reinterpret_cast<const char*>(Data + offset), text_len);
    }

    // Add patterns to FilteredRE2
    re2::RE2::Options options;
    for (const auto& pattern : patterns) {
        int id = -1;
        re2::RE2::ErrorCode error = filtered_re2.Add(pattern, options, &id);
        if (error == re2::RE2::NoError && id >= 0) {
            pattern_ids.push_back(id);
        }
    }

    // Compile to get strings to match
    std::vector<std::string> strings_to_match;
    filtered_re2.Compile(&strings_to_match);

    // Prepare atoms vector (use pattern IDs as atoms)
    std::vector<int> atoms;
    for (int id : pattern_ids) {
        atoms.push_back(id);
    }

    // Test AllPotentials
    std::vector<int> potential_regexps;
    filtered_re2.AllPotentials(atoms, &potential_regexps);

    // Test FirstMatch
    if (!text.empty()) {
        int first_match = filtered_re2.FirstMatch(text, atoms);
        (void)first_match; // Use result to avoid unused variable warning
    }

    // Test AllMatches
    if (!text.empty()) {
        std::vector<int> matching_regexps;
        bool has_matches = filtered_re2.AllMatches(text, atoms, &matching_regexps);
        (void)has_matches; // Use result to avoid unused variable warning
    }

    // Test SlowFirstMatch
    if (!text.empty()) {
        int slow_first_match = filtered_re2.SlowFirstMatch(text);
        (void)slow_first_match; // Use result to avoid unused variable warning
    }

    return 0;
}