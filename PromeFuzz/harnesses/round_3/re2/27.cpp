// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::RE2::Options::set_encoding at re2.h:700:10 in re2.h
// re2::FilteredRE2::Add at filtered_re2.cc:51:29 in filtered_re2.h
// re2::FilteredRE2::NumRegexps at filtered_re2.h:90:7 in filtered_re2.h
// re2::FilteredRE2::SlowFirstMatch at filtered_re2.cc:91:18 in filtered_re2.h
// re2::FilteredRE2::Compile at filtered_re2.cc:70:19 in filtered_re2.h
// re2::FilteredRE2::FirstMatch at filtered_re2.cc:98:18 in filtered_re2.h
// re2::FilteredRE2::AllMatches at filtered_re2.cc:112:19 in filtered_re2.h
// re2::FilteredRE2::SlowFirstMatch at filtered_re2.cc:91:18 in filtered_re2.h
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

#include "re2/filtered_re2.h"
#include "re2/re2.h"

extern "C" int LLVMFuzzerTestOneInput_27(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    // Initialize FilteredRE2 object
    re2::FilteredRE2 filtered_re2;

    // Parse input to get patterns and text
    // Format: [num_patterns:1 byte] [pattern1_len:1 byte] [pattern1] ... [patternN] [text]
    const uint8_t* ptr = Data;
    const uint8_t* end = Data + Size;

    if (ptr >= end) return 0;
    uint8_t num_patterns = *ptr++;
    if (num_patterns == 0) num_patterns = 1; // Ensure at least one pattern

    std::vector<std::string> patterns;
    std::vector<int> pattern_ids;

    // Read patterns
    for (int i = 0; i < num_patterns && ptr < end; ++i) {
        if (ptr >= end) break;
        uint8_t pattern_len = *ptr++;
        if (pattern_len == 0) pattern_len = 1;

        size_t actual_len = std::min<size_t>(pattern_len, end - ptr);
        if (actual_len == 0) {
            patterns.push_back(".");
            continue;
        }

        std::string pattern(reinterpret_cast<const char*>(ptr), actual_len);
        patterns.push_back(pattern);
        ptr += actual_len;
    }

    // Remaining data is the text
    absl::string_view text(reinterpret_cast<const char*>(ptr), end - ptr);

    // Add patterns to FilteredRE2
    std::vector<int> added_ids;
    for (const auto& pattern : patterns) {
        int id = -1;
        re2::RE2::Options options;
        options.set_encoding(re2::RE2::Options::EncodingUTF8);
        re2::RE2::ErrorCode error = filtered_re2.Add(pattern, options, &id);
        if (error == re2::RE2::NoError && id != -1) {
            added_ids.push_back(id);
        }
    }

    // Test NumRegexps
    int num_regexps = filtered_re2.NumRegexps();

    // Test SlowFirstMatch (can be called before Compile)
    int slow_match = filtered_re2.SlowFirstMatch(text);

    // Compile and test other functions
    std::vector<std::string> strings_to_match;
    filtered_re2.Compile(&strings_to_match);

    // Create atoms vector from strings_to_match indices
    std::vector<int> atoms;
    for (size_t i = 0; i < strings_to_match.size(); ++i) {
        atoms.push_back(static_cast<int>(i));
    }

    // Test FirstMatch
    int first_match = filtered_re2.FirstMatch(text, atoms);

    // Test AllMatches
    std::vector<int> matching_regexps;
    bool all_matches_result = filtered_re2.AllMatches(text, atoms, &matching_regexps);

    // Test SlowFirstMatch again after Compile
    int slow_match_after = filtered_re2.SlowFirstMatch(text);

    return 0;
}