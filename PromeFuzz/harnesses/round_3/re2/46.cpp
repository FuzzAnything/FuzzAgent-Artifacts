// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::RE2::Options::set_encoding at re2.h:700:10 in re2.h
// re2::FilteredRE2::Add at filtered_re2.cc:51:29 in filtered_re2.h
// re2::FilteredRE2::NumRegexps at filtered_re2.h:90:7 in filtered_re2.h
// re2::FilteredRE2::GetRE2 at filtered_re2.h:93:14 in filtered_re2.h
// re2::RE2::pattern at re2.h:312:22 in re2.h
// re2::FilteredRE2::SlowFirstMatch at filtered_re2.cc:91:18 in filtered_re2.h
// re2::FilteredRE2::Compile at filtered_re2.cc:70:19 in filtered_re2.h
// re2::FilteredRE2::FirstMatch at filtered_re2.cc:98:18 in filtered_re2.h
// re2::LazyRE2::get at re2.h:1000:8 in re2.h
// re2::RE2::pattern at re2.h:312:22 in re2.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <cstring>
#include <vector>
#include <string>

#include "re2/filtered_re2.h"
#include "re2/re2.h"

static std::vector<std::string> parsePatterns(const uint8_t *data, size_t size) {
    std::vector<std::string> patterns;
    if (size == 0) return patterns;
    
    const uint8_t *end = data + size;
    const uint8_t *current = data;
    
    while (current < end) {
        // Find next null terminator or end
        const uint8_t *pattern_end = current;
        while (pattern_end < end && *pattern_end != 0) {
            ++pattern_end;
        }
        
        if (pattern_end > current) {
            // Limit pattern size to avoid excessive memory usage
            size_t pattern_size = pattern_end - current;
            if (pattern_size > 1000) pattern_size = 1000;
            patterns.emplace_back(reinterpret_cast<const char*>(current), pattern_size);
        }
        
        if (pattern_end >= end) break;
        current = pattern_end + 1;
        
        // Limit number of patterns
        if (patterns.size() >= 10) break;
    }
    
    return patterns;
}

static absl::string_view getText(const uint8_t *data, size_t size) {
    if (size == 0) return absl::string_view("");
    
    // Use first non-empty null-terminated string as text
    const uint8_t *end = data + size;
    const uint8_t *current = data;
    
    while (current < end) {
        const uint8_t *text_end = current;
        while (text_end < end && *text_end != 0) {
            ++text_end;
        }
        
        if (text_end > current) {
            // Limit text size
            size_t text_size = text_end - current;
            if (text_size > 1000) text_size = 1000;
            return absl::string_view(reinterpret_cast<const char*>(current), text_size);
        }
        
        if (text_end >= end) break;
        current = text_end + 1;
    }
    
    // If no proper string found, use limited data as text
    size_t text_size = size;
    if (text_size > 1000) text_size = 1000;
    return absl::string_view(reinterpret_cast<const char*>(data), text_size);
}

extern "C" int LLVMFuzzerTestOneInput_46(const uint8_t *data, size_t size) {
    // Parse patterns from input data
    std::vector<std::string> patterns = parsePatterns(data, size);
    absl::string_view text = getText(data, size);
    
    // Create FilteredRE2 object
    re2::FilteredRE2 filtered_re2;
    
    // Add patterns to FilteredRE2
    std::vector<int> pattern_ids;
    for (const auto& pattern : patterns) {
        int id = -1;
        re2::RE2::Options options;
        
        // Use default options to avoid complex states that might crash
        // Only use Latin1 encoding if pattern is simple ASCII
        bool is_ascii = true;
        for (char c : pattern) {
            if (static_cast<unsigned char>(c) > 127) {
                is_ascii = false;
                break;
            }
        }
        if (is_ascii && size > 0 && (data[0] & 0x01)) {
            options.set_encoding(re2::RE2::Options::EncodingLatin1);
        }
        
        re2::RE2::ErrorCode error = filtered_re2.Add(pattern, options, &id);
        if (error == re2::RE2::NoError && id >= 0) {
            pattern_ids.push_back(id);
        }
    }
    
    // Test NumRegexps
    int num_regexps = filtered_re2.NumRegexps();
    
    // Test GetRE2 for valid indices
    for (int i = 0; i < num_regexps && i < 5; ++i) {
        try {
            const re2::RE2& re2_obj = filtered_re2.GetRE2(i);
            // Only access safe properties
            (void)re2_obj.pattern();
        } catch (...) {
            // Ignore exceptions
        }
    }
    
    // Test SlowFirstMatch only if we have patterns and text is not empty
    if (num_regexps > 0 && !text.empty()) {
        try {
            int slow_match = filtered_re2.SlowFirstMatch(text);
        } catch (...) {
            // Catch any exceptions from SlowFirstMatch
        }
    }
    
    // Try to compile if we have patterns
    if (!pattern_ids.empty()) {
        std::vector<int> atoms;
        // Create some dummy atoms based on pattern ids
        for (int id : pattern_ids) {
            if (id >= 0 && id < num_regexps) {
                atoms.push_back(id);
            }
        }
        
        // Try to compile - Compile expects vector<string>*
        std::vector<std::string> strings_to_match;
        // Only compile if we have atoms
        if (!atoms.empty()) {
            try {
                filtered_re2.Compile(&strings_to_match);
                
                // Test FirstMatch after compilation
                if (!text.empty()) {
                    int first_match = filtered_re2.FirstMatch(text, atoms);
                }
            } catch (...) {
                // Ignore compilation or matching exceptions
            }
        }
    }
    
    // Test LazyRE2 with a simple, safe pattern
    try {
        static re2::LazyRE2 lazy_re2 = {"[a-zA-Z0-9]+"};
        re2::RE2* re2_ptr = lazy_re2.get();
        if (re2_ptr) {
            (void)re2_ptr->pattern();
        }
    } catch (...) {
        // Ignore exceptions
    }
    
    return 0;
}