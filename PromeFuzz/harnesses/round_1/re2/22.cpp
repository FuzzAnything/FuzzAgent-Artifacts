// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::FilteredRE2::SlowFirstMatch at filtered_re2.cc:91:18 in filtered_re2.h
// re2::FilteredRE2::Compile at filtered_re2.cc:70:19 in filtered_re2.h
// re2::FilteredRE2::FirstMatch at filtered_re2.cc:98:18 in filtered_re2.h
// re2::FilteredRE2::Add at filtered_re2.cc:51:29 in filtered_re2.h
// re2::FilteredRE2::GetRE2 at filtered_re2.h:93:14 in filtered_re2.h
// re2::FilteredRE2::NumRegexps at filtered_re2.h:90:7 in filtered_re2.h
// re2::LazyRE2::get at re2.h:1000:8 in re2.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include "re2/re2.h"
#include "re2/filtered_re2.h"
#include <cstdint>
#include <cstring>
#include <vector>
#include <string>

extern "C" int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    // Create FilteredRE2 object
    re2::FilteredRE2 filtered_re2(3); // min_atom_len = 3

    // Use first byte to determine number of patterns to add (max 5)
    size_t num_patterns = (Data[0] % 5) + 1;
    size_t offset = 1;

    std::vector<int> pattern_ids;
    std::vector<re2::RE2::CannedOptions> option_values = {
        re2::RE2::DefaultOptions,
        re2::RE2::Latin1,
        re2::RE2::POSIX,
        re2::RE2::Quiet
    };

    // Add patterns to FilteredRE2
    for (size_t i = 0; i < num_patterns && offset < Size; i++) {
        // Determine pattern length (max 100 bytes)
        size_t pattern_len = Data[offset] % 100;
        offset++;
        
        if (offset + pattern_len > Size) {
            pattern_len = Size - offset;
        }
        
        if (pattern_len == 0) {
            // Skip if no data for pattern
            continue;
        }

        // Create pattern string
        std::string pattern(reinterpret_cast<const char*>(Data + offset), pattern_len);
        offset += pattern_len;

        // Choose random option
        re2::RE2::CannedOptions option = option_values[Data[offset % Size] % option_values.size()];
        re2::RE2::Options options(option);

        int id = -1;
        re2::RE2::ErrorCode error = filtered_re2.Add(pattern, options, &id);
        
        if (error == re2::RE2::NoError && id != -1) {
            pattern_ids.push_back(id);
        }

        // Test GetRE2 for valid IDs
        if (!pattern_ids.empty()) {
            int test_id = pattern_ids[Data[offset % Size] % pattern_ids.size()];
            try {
                const re2::RE2& re2_obj = filtered_re2.GetRE2(test_id);
                (void)re2_obj; // Use to avoid unused variable warning
            } catch (...) {
                // Ignore exceptions
            }
        }

        // Test NumRegexps
        int num_regexps = filtered_re2.NumRegexps();
        (void)num_regexps; // Use to avoid unused variable warning
    }

    // Test LazyRE2
    if (offset < Size) {
        size_t lazy_pattern_len = Data[offset] % 50;
        offset++;
        
        if (offset + lazy_pattern_len <= Size && lazy_pattern_len > 0) {
            std::string lazy_pattern(reinterpret_cast<const char*>(Data + offset), lazy_pattern_len);
            offset += lazy_pattern_len;
            
            static re2::LazyRE2 lazy_re2 = {lazy_pattern.c_str()};
            try {
                re2::RE2* re2_ptr = lazy_re2.get();
                (void)re2_ptr; // Use to avoid unused variable warning
            } catch (...) {
                // Ignore exceptions
            }
        }
    }

    // Test SlowFirstMatch
    if (offset < Size) {
        size_t text_len = Size - offset;
        if (text_len > 0) {
            std::string text(reinterpret_cast<const char*>(Data + offset), text_len);
            int slow_match = filtered_re2.SlowFirstMatch(text);
            (void)slow_match; // Use to avoid unused variable warning
        }
    }

    // Test FirstMatch (requires Compile first)
    if (!pattern_ids.empty() && offset < Size) {
        // Create strings_to_match vector for Compile
        std::vector<std::string> strings_to_match;
        
        // Use some data to create strings for matching
        size_t num_strings = (Data[offset] % 3) + 1; // 1-3 strings
        offset++;
        
        for (size_t i = 0; i < num_strings && offset < Size; i++) {
            size_t str_len = Data[offset] % 50;
            offset++;
            
            if (offset + str_len <= Size && str_len > 0) {
                std::string str(reinterpret_cast<const char*>(Data + offset), str_len);
                strings_to_match.push_back(str);
                offset += str_len;
            }
        }
        
        // Call Compile with the strings_to_match vector
        filtered_re2.Compile(&strings_to_match);
        
        // Use remaining data for text
        if (offset < Size) {
            size_t text_len = Size - offset;
            if (text_len > 0) {
                std::string text(reinterpret_cast<const char*>(Data + offset), text_len);
                
                // Create dummy atoms vector
                std::vector<int> atoms;
                if (text_len > 10) {
                    for (size_t i = 0; i < 5 && i < text_len; i++) {
                        atoms.push_back(static_cast<int>(Data[offset + i] % 256));
                    }
                }
                
                int first_match = filtered_re2.FirstMatch(text, atoms);
                (void)first_match; // Use to avoid unused variable warning
            }
        }
    }

    return 0;
}