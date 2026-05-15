// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::FilteredRE2::AllMatches at filtered_re2.cc:112:19 in filtered_re2.h
// re2::RE2::Options::set_log_errors at re2.h:709:10 in re2.h
// re2::RE2::Set::Add at set.cc:64:15 in set.h
// re2::RE2::Set::Compile at set.cc:103:16 in set.h
// re2::RE2::Set::Size at set.cc:58:15 in set.h
// re2::RE2::Set::Match at set.cc:133:16 in set.h
// re2::RE2::Set::Match at set.cc:137:16 in set.h
// re2::RE2::Options::set_log_errors at re2.h:709:10 in re2.h
// re2::FilteredRE2::Add at filtered_re2.cc:51:29 in filtered_re2.h
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

extern "C" int LLVMFuzzerTestOneInput_37(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    // Split input into two parts: patterns and text
    size_t split_point = Size / 2;
    if (split_point == 0) split_point = 1;
    
    const uint8_t* patterns_data = Data;
    size_t patterns_size = split_point;
    const uint8_t* text_data = Data + split_point;
    size_t text_size = Size - split_point;

    // Create RE2::Set with default options
    re2::RE2::Options options;
    options.set_log_errors(false);
    re2::RE2::Set set(options, re2::RE2::UNANCHORED);

    // Add patterns to the set
    std::vector<std::string> pattern_strings;
    const uint8_t* current = patterns_data;
    size_t remaining = patterns_size;
    
    while (remaining > 0) {
        // Find null terminator or use remaining bytes
        size_t pattern_len = 0;
        while (pattern_len < remaining && current[pattern_len] != 0) {
            pattern_len++;
        }
        
        if (pattern_len > 0) {
            std::string pattern(reinterpret_cast<const char*>(current), pattern_len);
            pattern_strings.push_back(pattern);
            
            std::string error;
            set.Add(pattern, &error);
        }
        
        // Move to next pattern (skip null terminator if present)
        size_t advance = pattern_len + (pattern_len < remaining ? 1 : 0);
        if (advance > remaining) {
            break;
        }
        current += advance;
        remaining -= advance;
    }

    // Compile the set
    if (!set.Compile()) {
        return 0;
    }

    // Check size
    set.Size();

    // Create text string
    std::string text(reinterpret_cast<const char*>(text_data), text_size);

    // Test first Match overload
    std::vector<int> matches1;
    set.Match(text, &matches1);

    // Test second Match overload with ErrorInfo
    std::vector<int> matches2;
    re2::RE2::Set::ErrorInfo error_info;
    set.Match(text, &matches2, &error_info);

    // Test FilteredRE2::AllMatches
    re2::FilteredRE2 filtered;
    std::vector<int> atoms;
    std::vector<int> matching_regexps;
    
    // Add some patterns to FilteredRE2
    for (const auto& pattern : pattern_strings) {
        if (!pattern.empty()) {
            int id;
            re2::RE2::Options filter_options;
            filter_options.set_log_errors(false);
            filtered.Add(pattern, filter_options, &id);
        }
    }
    
    // Build atoms vector (indices of patterns to check)
    for (int i = 0; i < static_cast<int>(pattern_strings.size()) && i < 10; ++i) {
        atoms.push_back(i);
    }
    
    if (!atoms.empty()) {
        filtered.AllMatches(text, atoms, &matching_regexps);
    }

    return 0;
}