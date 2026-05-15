// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::RE2::Set::Compile at set.cc:103:16 in set.h
// re2::RE2::Set::Size at set.cc:58:15 in set.h
// re2::RE2::Set::Match at set.cc:133:16 in set.h
// re2::RE2::Set::Match at set.cc:133:16 in set.h
// re2::RE2::Set::Match at set.cc:133:16 in set.h
// re2::RE2::Options::set_log_errors at re2.h:709:10 in re2.h
// re2::RE2::Options::set_max_mem at re2.h:697:10 in re2.h
// re2::RE2::Set::Size at set.cc:58:15 in set.h
// re2::RE2::Set::Add at set.cc:64:15 in set.h
// re2::RE2::Set::Size at set.cc:58:15 in set.h
// re2::RE2::Set::Add at set.cc:64:15 in set.h
// re2::RE2::Set::Size at set.cc:58:15 in set.h
// re2::RE2::Set::Add at set.cc:64:15 in set.h
// re2::RE2::Set::Size at set.cc:58:15 in set.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include "re2/set.h"
#include <cstdint>
#include <vector>
#include <string>

extern "C" int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Initialize RE2::Set with default options
    re2::RE2::Options options;
    options.set_log_errors(false);
    options.set_max_mem(1 << 20); // Limit memory to avoid OOM
    
    // Create the set with proper initialization
    re2::RE2::Set set(options, re2::RE2::ANCHOR_BOTH);
    
    // Create patterns from fuzz data, ensuring they're valid strings
    std::string pattern1, pattern2, pattern3;
    
    // Use first byte to determine pattern construction strategy
    uint8_t strategy = Data[0];
    size_t offset = 1;
    
    // Ensure we have enough data for at least one pattern
    if (Size > offset) {
        // Create pattern1 - simple literal from data
        size_t len1 = std::min((size_t)(Data[offset] % 32 + 1), Size - offset);
        if (len1 > 0) {
            pattern1 = std::string(reinterpret_cast<const char*>(Data + offset), len1);
            offset += len1;
        }
        
        if (pattern1.empty()) {
            pattern1 = "a"; // Fallback to simple pattern
        }
    } else {
        pattern1 = "a";
    }
    
    if (Size > offset) {
        // Create pattern2 - different from pattern1
        size_t len2 = std::min((size_t)(Data[offset % Size] % 32 + 1), Size - offset);
        if (len2 > 0) {
            pattern2 = std::string(reinterpret_cast<const char*>(Data + offset), len2);
            offset += len2;
        }
        
        if (pattern2.empty() || pattern2 == pattern1) {
            pattern2 = pattern1 + ".*";
        }
    } else {
        pattern2 = pattern1 + ".*";
    }
    
    if (Size > offset) {
        // Create pattern3 - different from previous patterns
        size_t len3 = std::min((size_t)(Data[offset % Size] % 32 + 1), Size - offset);
        if (len3 > 0) {
            pattern3 = std::string(reinterpret_cast<const char*>(Data + offset), len3);
        }
        
        if (pattern3.empty() || pattern3 == pattern1 || pattern3 == pattern2) {
            pattern3 = ".*" + pattern1;
        }
    } else {
        pattern3 = ".*" + pattern1;
    }
    
    std::string error;
    
    // Call functions in specified order
    set.Size();
    
    int idx1 = set.Add(pattern1, &error);
    set.Size();
    
    int idx2 = set.Add(pattern2, &error);
    set.Size();
    
    int idx3 = set.Add(pattern3, &error);
    set.Size();
    
    // Only compile if we have at least one valid pattern
    bool has_valid_pattern = false;
    if (idx1 >= 0 || idx2 >= 0 || idx3 >= 0) {
        has_valid_pattern = true;
    }
    
    if (has_valid_pattern) {
        // Try to compile, but catch any potential issues
        bool compiled = false;
        try {
            compiled = set.Compile();
        } catch (...) {
            // Catch any exceptions and return early
            return 0;
        }
        
        if (!compiled) {
            // Compilation failed, don't proceed with Match
            return 0;
        }
    } else {
        // No valid patterns, don't compile or match
        return 0;
    }
    
    set.Size();
    
    // Use different text inputs for matching
    std::vector<int> matches1;
    if (Size > 0) {
        std::string text1(reinterpret_cast<const char*>(Data), std::min(Size, (size_t)100));
        try {
            set.Match(text1, &matches1);
        } catch (...) {
            // Ignore match errors
        }
    }
    
    std::vector<int> matches2;
    if (!pattern1.empty()) {
        try {
            set.Match(pattern1, &matches2);
        } catch (...) {
            // Ignore match errors
        }
    }
    
    if (!pattern2.empty()) {
        std::vector<int> matches3;
        try {
            set.Match(pattern2, &matches3);
        } catch (...) {
            // Ignore match errors
        }
    }
    
    return 0;
}