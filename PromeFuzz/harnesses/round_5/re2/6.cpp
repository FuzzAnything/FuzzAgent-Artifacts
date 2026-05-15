// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::RE2::Options::set_max_mem at re2.h:697:10 in re2.h
// re2::RE2::Options::set_encoding at re2.h:700:10 in re2.h
// re2::RE2::ok at re2.h:307:8 in re2.h
// re2::RE2::Replace at re2.cc:450:11 in re2.h
// re2::RE2::GlobalReplace at re2.cc:472:10 in re2.h
// re2::RE2::PossibleMatchRange at re2.cc:597:11 in re2.h
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
#include <cstring>
#include <fstream>
#include <string>
#include <vector>

extern "C" int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    // Early exit for too small inputs
    if (Size < 4) {
        return 0;
    }

    // Split input into parts for pattern, text, and rewrite
    // Ensure we have at least 1 byte for each part
    size_t pattern_end = Size / 4;
    size_t text_end = Size / 2;
    size_t rewrite_end = (3 * Size) / 4;
    
    // Adjust boundaries to ensure non-zero sizes
    if (pattern_end == 0) pattern_end = 1;
    if (text_end <= pattern_end) text_end = pattern_end + 1;
    if (rewrite_end <= text_end) rewrite_end = text_end + 1;
    if (rewrite_end >= Size) rewrite_end = Size - 1;
    
    // Create strings with bounds checking
    std::string pattern;
    std::string text;
    std::string rewrite;
    
    if (pattern_end > 0 && pattern_end <= Size) {
        pattern = std::string(reinterpret_cast<const char*>(Data), pattern_end);
    } else {
        pattern = "a";
    }
    
    if (text_end > pattern_end && text_end <= Size) {
        text = std::string(reinterpret_cast<const char*>(Data + pattern_end), 
                          text_end - pattern_end);
    } else {
        text = "test";
    }
    
    if (rewrite_end > text_end && rewrite_end <= Size) {
        rewrite = std::string(reinterpret_cast<const char*>(Data + text_end), 
                             rewrite_end - text_end);
    } else {
        rewrite = "replacement";
    }
    
    // Clean up any null bytes that might break string operations
    for (auto& c : pattern) if (c == 0) c = 'a';
    for (auto& c : text) if (c == 0) c = 'b';
    for (auto& c : rewrite) if (c == 0) c = 'c';
    
    // Ensure pattern is not empty
    if (pattern.empty()) pattern = "a";
    if (text.empty()) text = "test";
    if (rewrite.empty()) rewrite = "r";
    
    try {
        // Create RE2 object with options to limit memory usage
        re2::RE2::Options options;
        options.set_max_mem(64 * 1024);  // Limit memory to 64KB
        options.set_encoding(re2::RE2::Options::EncodingLatin1);
        
        re2::RE2 re(pattern, options);
        if (!re.ok()) {
            return 0;
        }
        
        // 1. Test RE2::Replace
        std::string str1 = text;
        bool replaced = re2::RE2::Replace(&str1, re, rewrite);
        
        // 2. Test RE2::GlobalReplace
        std::string str2 = text;
        int global_replaced = re2::RE2::GlobalReplace(&str2, re, rewrite);
        
        // 3. Test RE2::PossibleMatchRange
        std::string min_str, max_str;
        // Use a reasonable maxlen derived from input size, but limit it
        int maxlen = static_cast<int>(Size % 100) + 1;
        if (maxlen > 100) maxlen = 100;
        bool range_success = re.PossibleMatchRange(&min_str, &max_str, maxlen);
        
        // 4. Test RE2::NamedCapturingGroups
        const std::map<std::string, int>& groups = re.NamedCapturingGroups();
        // Simply access the map to exercise the code
        volatile size_t group_count = groups.size();
        (void)group_count;
        
    } catch (...) {
        // Catch any exceptions to prevent crashes
        return 0;
    }
    
    return 0;
}