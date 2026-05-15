// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::FilteredRE2::FirstMatch at filtered_re2.cc:98:18 in filtered_re2.h
// re2::FilteredRE2::AllMatches at filtered_re2.cc:112:19 in filtered_re2.h
// re2::RE2::Options::set_max_mem at re2.h:697:10 in re2.h
// re2::RE2::Options::set_log_errors at re2.h:709:10 in re2.h
// re2::FilteredRE2::Add at filtered_re2.cc:51:29 in filtered_re2.h
// re2::FilteredRE2::NumRegexps at filtered_re2.h:90:7 in filtered_re2.h
// re2::FilteredRE2::SlowFirstMatch at filtered_re2.cc:91:18 in filtered_re2.h
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
#include <cstddef>
#include <vector>
#include <string>

#include "re2/filtered_re2.h"
#include "re2/re2.h"

extern "C" int LLVMFuzzerTestOneInput_38(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Initialize FilteredRE2 object
    re2::FilteredRE2 filtered_re2;

    // Parse input data to create patterns and text
    // Use first byte to determine number of patterns (max 3 to avoid explosion)
    size_t num_patterns = (Data[0] % 3) + 1;
    size_t offset = 1;

    std::vector<int> pattern_ids;
    std::vector<std::string> patterns;

    // Extract patterns from input data
    for (size_t i = 0; i < num_patterns && offset < Size; i++) {
        // Pattern length determined by next byte (max 30 chars)
        size_t pattern_len = (Data[offset] % 30) + 1;
        offset++;
        
        if (offset + pattern_len > Size) {
            pattern_len = Size - offset;
        }
        
        if (pattern_len > 0) {
            std::string pattern(reinterpret_cast<const char*>(Data + offset), pattern_len);
            patterns.push_back(pattern);
            offset += pattern_len;
        }
    }

    // Add patterns to FilteredRE2 with safe options
    re2::RE2::Options options;
    options.set_max_mem(64 * 1024); // Limit memory to 64KB
    options.set_log_errors(false); // Disable error logging
    
    for (const auto& pattern : patterns) {
        int id = -1;
        re2::RE2::ErrorCode error = filtered_re2.Add(pattern, options, &id);
        if (error == re2::RE2::NoError && id >= 0) {
            pattern_ids.push_back(id);
        }
    }

    // Get remaining data as text - ensure non-empty string_view
    absl::string_view text;
    if (offset < Size) {
        size_t text_len = Size - offset;
        // Ensure we have at least 1 byte for text
        if (text_len > 0) {
            text = absl::string_view(reinterpret_cast<const char*>(Data + offset), text_len);
        } else {
            text = "a"; // Default minimal text
        }
    } else {
        text = "a"; // Default minimal text
    }

    // Test NumRegexps
    int num_regexps = filtered_re2.NumRegexps();

    // Test SlowFirstMatch only if we have patterns AND the text is not empty
    if (!pattern_ids.empty() && !text.empty()) {
        // Wrap in try-catch to handle any potential exceptions
        try {
            int first_match = filtered_re2.SlowFirstMatch(text);
        } catch (...) {
            // Ignore exceptions
        }
    }

    // Test Compile and related functions if we have patterns
    if (!pattern_ids.empty()) {
        std::vector<std::string> strings_to_match;
        try {
            filtered_re2.Compile(&strings_to_match);
        } catch (...) {
            // Ignore exceptions, continue without compiled state
        }

        // Create atoms vector (use pattern indices)
        std::vector<int> atoms;
        for (size_t i = 0; i < pattern_ids.size() && i < 2; i++) {
            atoms.push_back(pattern_ids[i]);
        }

        // Test FirstMatch (requires Compile first and non-empty atoms/text)
        if (!atoms.empty() && !text.empty()) {
            try {
                int compiled_first_match = filtered_re2.FirstMatch(text, atoms);
            } catch (...) {
                // Ignore exceptions
            }
        }

        // Test AllMatches
        if (!atoms.empty() && !text.empty()) {
            std::vector<int> matching_regexps;
            try {
                bool has_matches = filtered_re2.AllMatches(text, atoms, &matching_regexps);
            } catch (...) {
                // Ignore exceptions
            }
        }
    }

    return 0;
}