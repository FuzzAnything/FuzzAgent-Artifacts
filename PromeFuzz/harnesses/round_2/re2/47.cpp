// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::RE2::ok at re2.h:307:8 in re2.h
// re2::RE2::GlobalReplace at re2.cc:472:10 in re2.h
// re2::RE2::ok at re2.h:307:8 in re2.h
// re2::RE2::Replace at re2.cc:450:11 in re2.h
// re2::RE2::ok at re2.h:307:8 in re2.h
// re2::RE2::NumberOfCapturingGroups at re2.h:551:7 in re2.h
// re2::RE2::CheckRewriteString at re2.cc:971:11 in re2.h
// re2::RE2::MaxSubmatch at re2.cc:1011:10 in re2.h
// re2::RE2::ok at re2.h:307:8 in re2.h
// re2::RE2::Extract at re2.cc:542:11 in re2.h
// re2::RE2::ok at re2.h:307:8 in re2.h
// re2::RE2::NumberOfCapturingGroups at re2.h:551:7 in re2.h
// re2::RE2::ok at re2.h:307:8 in re2.h
// re2::RE2::CheckRewriteString at re2.cc:971:11 in re2.h
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
#include <stdint.h>
#include <string>
#include <vector>

static re2::RE2* createRE2FromData(const uint8_t* data, size_t size) {
    if (size == 0) {
        return new re2::RE2("");
    }
    // Use first 100 bytes max for pattern to avoid huge patterns
    size_t pattern_len = size > 100 ? 100 : size;
    std::string pattern(reinterpret_cast<const char*>(data), pattern_len);
    return new re2::RE2(pattern);
}

extern "C" int LLVMFuzzerTestOneInput_47(const uint8_t* Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    // Split input into parts for different parameters
    size_t split1 = Size / 3;
    size_t split2 = 2 * Size / 3;
    
    // Create pattern string from first part (limit to reasonable size)
    std::string pattern_str;
    if (split1 > 0) {
        size_t pattern_size = split1 > 100 ? 100 : split1;
        pattern_str.assign(reinterpret_cast<const char*>(Data), pattern_size);
    } else {
        pattern_str = "";
    }
    
    // Create text string from second part (limit to reasonable size)
    std::string text_str;
    if (split2 > split1) {
        size_t text_size = (split2 - split1) > 1000 ? 1000 : (split2 - split1);
        text_str.assign(reinterpret_cast<const char*>(Data + split1), text_size);
    } else {
        text_str = "";
    }
    
    // Create rewrite string from third part (limit to reasonable size)
    std::string rewrite_str;
    if (Size > split2) {
        size_t rewrite_size = (Size - split2) > 1000 ? 1000 : (Size - split2);
        rewrite_str.assign(reinterpret_cast<const char*>(Data + split2), rewrite_size);
    } else {
        rewrite_str = "";
    }
    
    // Test 1: RE2::MaxSubmatch
    try {
        int max_submatch = re2::RE2::MaxSubmatch(rewrite_str);
        (void)max_submatch;
    } catch (...) {
        // Ignore exceptions
    }
    
    // Test 2: RE2::Extract
    try {
        re2::RE2 re(pattern_str);
        if (re.ok()) {
            std::string out;
            // Ensure text and out don't alias by using separate strings
            std::string text_copy = text_str;
            bool extracted = re2::RE2::Extract(text_copy, re, rewrite_str, &out);
            (void)extracted;
        }
    } catch (...) {
        // Ignore exceptions
    }
    
    // Test 3: RE2::NumberOfCapturingGroups
    try {
        re2::RE2 re(pattern_str);
        if (re.ok()) {
            int num_groups = re.NumberOfCapturingGroups();
            (void)num_groups;
        }
    } catch (...) {
        // Ignore exceptions
    }
    
    // Test 4: RE2::CheckRewriteString
    try {
        re2::RE2 re(pattern_str);
        if (re.ok()) {
            std::string error;
            bool valid = re.CheckRewriteString(rewrite_str, &error);
            (void)valid;
        }
    } catch (...) {
        // Ignore exceptions
    }
    
    // Test 5: RE2::GlobalReplace
    try {
        re2::RE2 re(pattern_str);
        if (re.ok()) {
            std::string str_to_replace = text_str;
            int replacements = re2::RE2::GlobalReplace(&str_to_replace, re, rewrite_str);
            (void)replacements;
        }
    } catch (...) {
        // Ignore exceptions
    }
    
    // Test 6: RE2::Replace
    try {
        re2::RE2 re(pattern_str);
        if (re.ok()) {
            std::string str_to_replace = text_str;
            bool replaced = re2::RE2::Replace(&str_to_replace, re, rewrite_str);
            (void)replaced;
        }
    } catch (...) {
        // Ignore exceptions
    }
    
    // Additional test: Create multiple RE2 objects with different patterns
    for (size_t i = 0; i < 3 && i * 20 < Size; i++) {
        size_t start = i * 20;
        size_t end = start + 20;
        if (end > Size) end = Size;
        if (start < end) {
            std::string alt_pattern(reinterpret_cast<const char*>(Data + start), end - start);
            re2::RE2 alt_re(alt_pattern);
            if (alt_re.ok()) {
                // Test various functions with this alternate RE2 object
                int groups = alt_re.NumberOfCapturingGroups();
                (void)groups;
                
                std::string error;
                bool check = alt_re.CheckRewriteString(rewrite_str, &error);
                (void)check;
            }
        }
    }
    
    return 0;
}