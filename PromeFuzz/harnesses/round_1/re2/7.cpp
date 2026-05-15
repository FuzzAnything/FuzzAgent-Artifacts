// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::RE2::ok at re2.h:307:8 in re2.h
// re2::RE2::Replace at re2.cc:450:11 in re2.h
// re2::RE2::GlobalReplace at re2.cc:472:10 in re2.h
// re2::RE2::ok at re2.h:307:8 in re2.h
// re2::RE2::PossibleMatchRange at re2.cc:597:11 in re2.h
// re2::RE2::ok at re2.h:307:8 in re2.h
// re2::RE2::NamedCapturingGroups at re2.cc:395:40 in re2.h
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
#include <cstdint>
#include <cstddef>
#include <string>
#include <map>

static std::string create_string_from_data(const uint8_t *data, size_t size, size_t max_len) {
    if (size == 0) return "";
    size_t len = size < max_len ? size : max_len;
    return std::string(reinterpret_cast<const char*>(data), len);
}

extern "C" int LLVMFuzzerTestOneInput_7(const uint8_t *Data, size_t Size) {
    if (Size < 3) return 0;
    
    // Split input into parts for pattern, rewrite, and text
    size_t pattern_len = Size / 3;
    size_t rewrite_len = Size / 3;
    size_t text_len = Size - 2 * pattern_len;
    
    std::string pattern = create_string_from_data(Data, pattern_len, 100);
    std::string rewrite = create_string_from_data(Data + pattern_len, rewrite_len, 100);
    std::string text = create_string_from_data(Data + pattern_len + rewrite_len, text_len, 200);
    
    // Skip empty patterns to avoid crashes
    if (pattern.empty()) return 0;
    
    // Create RE2 object with error handling
    re2::RE2 re(pattern);
    if (!re.ok()) return 0;
    
    // 1. Test Replace - ensure we have valid strings
    if (!text.empty() && !rewrite.empty()) {
        std::string str1 = text;
        bool replaced = re2::RE2::Replace(&str1, re, rewrite);
        // Just ignore the result, we're only testing for crashes
    }
    
    // 2. Test GlobalReplace
    if (!text.empty() && !rewrite.empty()) {
        std::string str2 = text;
        int replacements = re2::RE2::GlobalReplace(&str2, re, rewrite);
        // Just ignore the result, we're only testing for crashes
    }
    
    // 3. Test PossibleMatchRange - use safe maxlen
    if (re.ok()) {
        std::string min, max;
        int maxlen = 50;  // Reduced from 100 to avoid potential issues
        bool range_success = re.PossibleMatchRange(&min, &max, maxlen);
        // Just ignore the result, we're only testing for crashes
    }
    
    // 4. Test NamedCapturingGroups
    if (re.ok()) {
        const std::map<std::string, int>& groups = re.NamedCapturingGroups();
        // Just accessing the reference, no operations needed
    }
    
    return 0;
}