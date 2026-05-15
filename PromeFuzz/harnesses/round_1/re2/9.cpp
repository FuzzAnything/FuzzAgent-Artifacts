// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::RE2::Set::Add at set.cc:64:15 in set.h
// re2::RE2::CapturingGroupNames at re2.cc:406:40 in re2.h
// re2::RE2::QuoteMeta at re2.cc:559:18 in re2.h
// re2::RE2::Regexp at re2.h:340:16 in re2.h
// re2::RE2::Set::Add at set.cc:64:15 in set.h
// re2::RE2::Set::Add at set.cc:64:15 in set.h
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
#include "re2/set.h"
#include <stdint.h>
#include <string>
#include <map>

extern "C" int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Convert input to string for use in patterns
    std::string input(reinterpret_cast<const char*>(Data), Size);

    // 1. Test RE2::CapturingGroupNames
    // First create an RE2 object with a pattern
    std::string pattern_str;
    if (Size > 0) {
        // Use first byte to determine pattern length (up to min(Size, 100))
        size_t pattern_len = (Data[0] % 100) + 1;
        if (pattern_len > Size) pattern_len = Size;
        pattern_str = std::string(reinterpret_cast<const char*>(Data), pattern_len);
    } else {
        pattern_str = "test";
    }

    try {
        re2::RE2 re(pattern_str);
        
        // Call CapturingGroupNames
        const std::map<int, std::string>& group_names = re.CapturingGroupNames();
        (void)group_names; // Use result to avoid unused variable warning
    } catch (...) {
        // Ignore exceptions from invalid patterns
    }

    // 2. Test RE2::QuoteMeta (static method)
    std::string quoted;
    try {
        quoted = re2::RE2::QuoteMeta(input);
        (void)quoted; // Use result
    } catch (...) {
        // Ignore exceptions
    }

    // 3. Test RE2::Regexp
    try {
        // Create another RE2 object with different pattern
        std::string pattern2_str;
        if (Size > 1) {
            size_t pattern2_len = (Data[1] % 100) + 1;
            if (pattern2_len > Size) pattern2_len = Size;
            pattern2_str = std::string(reinterpret_cast<const char*>(Data), pattern2_len);
        } else {
            pattern2_str = "(test)";
        }
        
        re2::RE2 re2(pattern2_str);
        re2::Regexp* regexp = re2.Regexp();
        (void)regexp; // Use result
    } catch (...) {
        // Ignore exceptions
    }

    // 4. Test RE2::Set::Add
    try {
        // Create RE2::Set with default options
        re2::RE2::Options options;
        re2::RE2::Set set(options, re2::RE2::UNANCHORED);
        
        // Try to add multiple patterns from the input
        std::string error;
        int index = -1;
        
        // Add first pattern (using first half of input)
        if (Size > 0) {
            size_t first_half = Size / 2;
            if (first_half > 0) {
                std::string pattern1(reinterpret_cast<const char*>(Data), first_half);
                index = set.Add(pattern1, &error);
                (void)index; // Use result
            }
        }
        
        // Add second pattern (using second half of input)
        if (Size > 1) {
            size_t second_half = Size - (Size / 2);
            if (second_half > 0) {
                std::string pattern2(reinterpret_cast<const char*>(Data + (Size / 2)), second_half);
                index = set.Add(pattern2, &error);
                (void)index; // Use result
            }
        }
        
        // Try to add the entire input as a pattern
        index = set.Add(input, &error);
        (void)index; // Use result
    } catch (...) {
        // Ignore exceptions
    }

    return 0;
}