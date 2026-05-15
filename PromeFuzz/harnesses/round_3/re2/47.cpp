// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::FilteredRE2::Compile at filtered_re2.cc:70:19 in filtered_re2.h
// re2::FilteredRE2::FirstMatch at filtered_re2.cc:98:18 in filtered_re2.h
// re2::LazyRE2::get at re2.h:1000:8 in re2.h
// re2::RE2::Options::set_encoding at re2.h:700:10 in re2.h
// re2::FilteredRE2::Add at filtered_re2.cc:51:29 in filtered_re2.h
// re2::FilteredRE2::NumRegexps at filtered_re2.h:90:7 in filtered_re2.h
// re2::FilteredRE2::NumRegexps at filtered_re2.h:90:7 in filtered_re2.h
// re2::FilteredRE2::GetRE2 at filtered_re2.h:93:14 in filtered_re2.h
// re2::FilteredRE2::NumRegexps at filtered_re2.h:90:7 in filtered_re2.h
// re2::FilteredRE2::GetRE2 at filtered_re2.h:93:14 in filtered_re2.h
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
#include "re2/filtered_re2.h"
#include "re2/re2.h"
#include <cstdint>
#include <cstdlib>
#include <string>
#include <vector>
#include <memory>

extern "C" int LLVMFuzzerTestOneInput_47(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    // Create FilteredRE2 instance
    re2::FilteredRE2 filtered_re2;

    // Add some regex patterns using fuzzer data
    int num_patterns = (Data[0] % 5) + 1;  // 1-5 patterns
    std::vector<int> pattern_ids;
    
    size_t data_offset = 1;
    for (int i = 0; i < num_patterns && data_offset < Size; i++) {
        // Extract pattern string from fuzzer data
        size_t pattern_len = (Data[data_offset] % 32) + 1;  // 1-32 bytes
        if (data_offset + pattern_len >= Size) break;
        
        std::string pattern(reinterpret_cast<const char*>(Data + data_offset), pattern_len);
        data_offset += pattern_len;
        
        // Add pattern to FilteredRE2
        int pattern_id = -1;
        re2::RE2::Options options;
        options.set_encoding(re2::RE2::Options::EncodingUTF8);
        
        re2::RE2::ErrorCode error = filtered_re2.Add(pattern, options, &pattern_id);
        if (error == re2::RE2::NoError) {
            pattern_ids.push_back(pattern_id);
        }
    }

    // Test NumRegexps
    int num_regexps = filtered_re2.NumRegexps();
    (void)num_regexps;  // Use result to avoid unused variable warning

    // Test GetRE2 for valid indices
    for (int id : pattern_ids) {
        if (id >= 0 && id < filtered_re2.NumRegexps()) {
            try {
                const re2::RE2& re2_obj = filtered_re2.GetRE2(id);
                (void)re2_obj;  // Use result
            } catch (...) {
                // Ignore exceptions
            }
        }
    }

    // Test GetRE2 for potentially invalid indices
    if (Size > 0) {
        int invalid_id = Data[0];
        if (invalid_id >= 0 && invalid_id < filtered_re2.NumRegexps()) {
            try {
                const re2::RE2& re2_obj = filtered_re2.GetRE2(invalid_id);
                (void)re2_obj;
            } catch (...) {
                // Ignore exceptions
            }
        }
    }

    // Test SlowFirstMatch
    if (data_offset < Size) {
        size_t text_len = std::min(Size - data_offset, (size_t)64);
        absl::string_view text(reinterpret_cast<const char*>(Data + data_offset), text_len);
        data_offset += text_len;
        
        int slow_match_result = filtered_re2.SlowFirstMatch(text);
        (void)slow_match_result;
    }

    // Test FirstMatch (requires Compile first)
    if (pattern_ids.size() > 0) {
        // Create strings_to_match vector for Compile
        std::vector<std::string> strings_to_match;
        
        // Add some strings from fuzzer data for matching
        int num_strings = (Data[0] % 3) + 1;  // 1-3 strings
        for (int i = 0; i < num_strings && data_offset < Size; i++) {
            size_t str_len = (Data[data_offset] % 16) + 1;  // 1-16 bytes
            if (data_offset + str_len >= Size) break;
            
            std::string str(reinterpret_cast<const char*>(Data + data_offset), str_len);
            data_offset += str_len;
            strings_to_match.push_back(str);
        }
        
        filtered_re2.Compile(&strings_to_match);
        
        if (data_offset < Size) {
            size_t text_len = std::min(Size - data_offset, (size_t)64);
            absl::string_view text(reinterpret_cast<const char*>(Data + data_offset), text_len);
            data_offset += text_len;
            
            // Create atoms vector from fuzzer data
            std::vector<int> atoms;
            size_t atoms_count = std::min(Size - data_offset, (size_t)10);
            for (size_t i = 0; i < atoms_count && data_offset < Size; i++) {
                atoms.push_back(Data[data_offset] % 256);
                data_offset++;
            }
            
            int first_match_result = filtered_re2.FirstMatch(text, atoms);
            (void)first_match_result;
        }
    }

    // Test LazyRE2::get
    if (Size > 10) {
        // Create a pattern from fuzzer data
        size_t lazy_pattern_len = std::min(Size, (size_t)50);
        std::string lazy_pattern(reinterpret_cast<const char*>(Data), lazy_pattern_len);
        
        // LazyRE2 is typically used as a static variable, but we can create one locally
        static re2::LazyRE2 lazy_re2 = {lazy_pattern.c_str()};
        
        try {
            re2::RE2* re2_ptr = lazy_re2.get();
            (void)re2_ptr;
        } catch (...) {
            // Ignore exceptions
        }
    }

    return 0;
}