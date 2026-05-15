// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::RE2::Set::Size at set.cc:58:15 in set.h
// re2::RE2::Set::Add at set.cc:64:15 in set.h
// re2::RE2::Set::Size at set.cc:58:15 in set.h
// re2::RE2::Set::Add at set.cc:64:15 in set.h
// re2::RE2::Set::Size at set.cc:58:15 in set.h
// re2::RE2::Set::Add at set.cc:64:15 in set.h
// re2::RE2::Set::Size at set.cc:58:15 in set.h
// re2::RE2::Set::Compile at set.cc:103:16 in set.h
// re2::RE2::Set::Size at set.cc:58:15 in set.h
// re2::RE2::Set::Match at set.cc:133:16 in set.h
// re2::RE2::Set::Match at set.cc:133:16 in set.h
// re2::RE2::Set::Match at set.cc:133:16 in set.h
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

#include "re2/set.h"

extern "C" int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    // Create RE2::Set object using move constructor
    re2::RE2::Set set(re2::RE2::Set(re2::RE2::Options(), re2::RE2::ANCHOR_BOTH));
    
    // Prepare patterns from fuzzer input
    // We'll use the first 3/4 of data for patterns (3 patterns) and last 1/4 for text
    if (Size < 4) return 0; // Need minimum data
    
    size_t pattern_part = (Size * 3) / 4;
    size_t text_part = Size - pattern_part;
    
    // Split pattern_part into 3 roughly equal parts for patterns
    size_t pattern1_size = pattern_part / 3;
    size_t pattern2_size = pattern_part / 3;
    size_t pattern3_size = pattern_part - pattern1_size - pattern2_size;
    
    const char* data_ptr = reinterpret_cast<const char*>(Data);
    
    std::string pattern1(data_ptr, std::min(pattern1_size, Size));
    std::string pattern2(data_ptr + pattern1_size, std::min(pattern2_size, Size - pattern1_size));
    std::string pattern3(data_ptr + pattern1_size + pattern2_size, 
                        std::min(pattern3_size, Size - pattern1_size - pattern2_size));
    
    std::string text(data_ptr + pattern_part, std::min(text_part, Size - pattern_part));
    
    std::string error;
    std::vector<int> matches;
    
    // Call functions in specified order
    set.Size();
    
    // Check return values of Add() to ensure patterns are valid
    int idx1 = set.Add(pattern1, &error);
    if (idx1 < 0) {
        // Invalid pattern, skip further processing
        return 0;
    }
    set.Size();
    
    int idx2 = set.Add(pattern2, &error);
    if (idx2 < 0) {
        return 0;
    }
    set.Size();
    
    int idx3 = set.Add(pattern3, &error);
    if (idx3 < 0) {
        return 0;
    }
    set.Size();
    
    // Check return value of Compile() before using the set
    if (!set.Compile()) {
        // Compilation failed, do not call Match()
        return 0;
    }
    set.Size();
    
    set.Match(text, &matches);
    set.Match(text, &matches);
    set.Match(text, &matches);
    
    return 0;
}