// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::FilteredRE2::FirstMatch at filtered_re2.cc:98:18 in filtered_re2.h
// re2::FilteredRE2::SlowFirstMatch at filtered_re2.cc:91:18 in filtered_re2.h
// re2::FilteredRE2::GetRE2 at filtered_re2.h:93:14 in filtered_re2.h
// re2::RE2::Options::set_max_mem at re2.h:697:10 in re2.h
// re2::RE2::Options::set_log_errors at re2.h:709:10 in re2.h
// re2::FilteredRE2::Add at filtered_re2.cc:51:29 in filtered_re2.h
// re2::FilteredRE2::NumRegexps at filtered_re2.h:90:7 in filtered_re2.h
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
#include <cstring>

#include "re2/filtered_re2.h"

extern "C" int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create FilteredRE2 object
    re2::FilteredRE2 filtered_re2;

    // Create RE2 options with conservative settings
    re2::RE2::Options options;
    options.set_max_mem(64 * 1024);  // Limit memory usage
    options.set_log_errors(false);   // Disable error logging

    // Parse fuzzer input to create patterns
    // Use first byte to determine number of patterns (max 5 to avoid explosion)
    size_t num_patterns = (Data[0] % 5) + 1;
    if (num_patterns * 2 > Size) {
        return 0;
    }

    std::vector<int> pattern_ids;
    size_t data_offset = 1;

    // Add patterns to FilteredRE2
    for (size_t i = 0; i < num_patterns && data_offset < Size; i++) {
        // Create pattern from fuzzer data
        size_t pattern_len = Data[data_offset] % 64;  // Limit pattern length
        if (pattern_len == 0) {
            pattern_len = 1;
        }
        
        // Ensure we don't read beyond bounds
        if (data_offset + pattern_len > Size) {
            pattern_len = Size - data_offset;
            if (pattern_len == 0) {
                break;
            }
        }
        
        std::string pattern(reinterpret_cast<const char*>(Data + data_offset), pattern_len);
        
        int pattern_id = -1;
        re2::RE2::ErrorCode error = filtered_re2.Add(pattern, options, &pattern_id);
        
        if (error == re2::RE2::NoError && pattern_id >= 0) {
            pattern_ids.push_back(pattern_id);
        }
        
        data_offset += pattern_len;
        if (data_offset >= Size) {
            break;
        }
    }

    // Create text from remaining data (limit text size)
    std::string text;
    if (data_offset < Size) {
        size_t text_len = Size - data_offset;
        if (text_len > 1024) {  // Limit text size
            text_len = 1024;
        }
        text.assign(reinterpret_cast<const char*>(Data + data_offset), text_len);
    } else {
        text = "test";
    }

    // Call NumRegexps
    int num_regexps = filtered_re2.NumRegexps();

    // Try to compile if we have patterns
    if (!pattern_ids.empty()) {
        std::vector<std::string> strings_to_match;
        
        try {
            filtered_re2.Compile(&strings_to_match);
        } catch (...) {
            // Ignore compilation exceptions
            return 0;
        }

        // Create atoms vector for FirstMatch
        std::vector<int> atoms;
        if (!strings_to_match.empty()) {
            // Use some indices from pattern_ids as atoms
            for (size_t i = 0; i < std::min(pattern_ids.size(), strings_to_match.size()); i++) {
                atoms.push_back(i % strings_to_match.size());
            }
        }

        // Call FirstMatch if we have atoms
        if (!atoms.empty()) {
            try {
                int first_match = filtered_re2.FirstMatch(text, atoms);
                (void)first_match;
            } catch (...) {
                // Ignore exceptions from FirstMatch
            }
        }
    }

    // Call SlowFirstMatch with try-catch
    try {
        int slow_first_match = filtered_re2.SlowFirstMatch(text);
        (void)slow_first_match;
    } catch (...) {
        // Ignore exceptions from SlowFirstMatch
    }

    // Try to get RE2 objects for valid pattern IDs
    for (int id : pattern_ids) {
        if (id >= 0 && id < num_regexps) {
            try {
                const re2::RE2& re2_obj = filtered_re2.GetRE2(id);
                (void)re2_obj;
            } catch (...) {
                // Ignore exceptions
            }
        }
    }

    return 0;
}