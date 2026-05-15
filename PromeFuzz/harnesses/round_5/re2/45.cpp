// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::RE2::ok at re2.h:307:8 in re2.h
// re2::RE2::FullMatch at re2.h:411:15 in re2.h
// re2::RE2::ok at re2.h:307:8 in re2.h
// re2::RE2::Consume at re2.h:445:15 in re2.h
// re2::RE2::ok at re2.h:307:8 in re2.h
// re2::RE2::Consume at re2.h:445:15 in re2.h
// re2::RE2::ok at re2.h:307:8 in re2.h
// re2::RE2::Consume at re2.h:445:15 in re2.h
// re2::RE2::Options::set_never_capture at re2.h:721:10 in re2.h
// re2::RE2::ok at re2.h:307:8 in re2.h
// re2::RE2::PartialMatch at re2.h:427:15 in re2.h
// re2::RE2::NumberOfCapturingGroups at re2.h:551:7 in re2.h
// re2::RE2::ok at re2.h:307:8 in re2.h
// re2::RE2::PartialMatch at re2.h:427:15 in re2.h
// re2::RE2::Options::set_never_capture at re2.h:721:10 in re2.h
// re2::RE2::Options::never_capture at re2.h:720:10 in re2.h
// re2::RE2::ok at re2.h:307:8 in re2.h
// re2::RE2::NumberOfCapturingGroups at re2.h:551:7 in re2.h
// re2::RE2::ok at re2.h:307:8 in re2.h
// re2::RE2::PartialMatch at re2.h:427:15 in re2.h
// re2::RE2::ok at re2.h:307:8 in re2.h
// re2::RE2::PartialMatch at re2.h:427:15 in re2.h
// re2::RE2::ok at re2.h:307:8 in re2.h
// re2::RE2::PartialMatch at re2.h:427:15 in re2.h
// re2::RE2::ok at re2.h:307:8 in re2.h
// re2::RE2::FullMatch at re2.h:411:15 in re2.h
// re2::RE2::ok at re2.h:307:8 in re2.h
// re2::RE2::FullMatch at re2.h:411:15 in re2.h
// re2::RE2::ok at re2.h:307:8 in re2.h
// re2::RE2::FullMatch at re2.h:411:15 in re2.h
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
#include <string>
#include <vector>

extern "C" int LLVMFuzzerTestOneInput_45(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    // Split input into pattern and text with bounds checking
    size_t pattern_len = Data[0] % (Size - 1) + 1;
    if (pattern_len > Size - 1) pattern_len = Size - 1;
    
    std::string pattern(reinterpret_cast<const char*>(Data + 1), pattern_len);
    std::string text(reinterpret_cast<const char*>(Data + 1 + pattern_len), 
                     Size - 1 - pattern_len);

    // Test RE2::Options::set_never_capture and never_capture
    re2::RE2::Options options;
    bool never_capture_val = (Data[0] & 0x01) != 0;
    options.set_never_capture(never_capture_val);
    bool retrieved = options.never_capture();
    (void)retrieved;

    // Create RE2 object with pattern and options
    re2::RE2 re(pattern, options);
    
    // Only proceed if the RE2 object was constructed successfully
    if (re.ok()) {
        // Test NumberOfCapturingGroups
        int num_groups = re.NumberOfCapturingGroups();
        (void)num_groups;

        // Test PartialMatch with varying number of capture arguments
        // Use string instead of string_view for captures to avoid dangling references
        std::string cap1, cap2, cap3;
        
        // Test with 0 capture arguments - limit text size to avoid excessive recursion
        if (text.size() < 1000) {
            // Create a new RE2 object for each test to avoid state corruption
            re2::RE2 re_copy(pattern, options);
            if (re_copy.ok()) {
                bool partial_match0 = re2::RE2::PartialMatch(text, re_copy);
                (void)partial_match0;
            }
        }
        
        // Test with 1 capture argument
        if (text.size() < 1000) {
            re2::RE2 re_copy(pattern, options);
            if (re_copy.ok()) {
                bool partial_match1 = re2::RE2::PartialMatch(text, re_copy, &cap1);
                (void)partial_match1;
            }
        }
        
        // Test with 2 capture arguments
        if (text.size() < 1000) {
            re2::RE2 re_copy(pattern, options);
            if (re_copy.ok()) {
                bool partial_match2 = re2::RE2::PartialMatch(text, re_copy, &cap1, &cap2);
                (void)partial_match2;
            }
        }

        // Test FullMatch with varying number of capture arguments
        if (text.size() < 1000) {
            re2::RE2 re_copy(pattern, options);
            if (re_copy.ok()) {
                bool full_match0 = re2::RE2::FullMatch(text, re_copy);
                (void)full_match0;
            }
        }
        
        if (text.size() < 1000) {
            re2::RE2 re_copy(pattern, options);
            if (re_copy.ok()) {
                bool full_match1 = re2::RE2::FullMatch(text, re_copy, &cap1);
                (void)full_match1;
            }
        }
        
        if (text.size() < 1000) {
            re2::RE2 re_copy(pattern, options);
            if (re_copy.ok()) {
                bool full_match2 = re2::RE2::FullMatch(text, re_copy, &cap1, &cap2);
                (void)full_match2;
            }
        }
        
        if (text.size() < 1000) {
            re2::RE2 re_copy(pattern, options);
            if (re_copy.ok()) {
                bool full_match3 = re2::RE2::FullMatch(text, re_copy, &cap1, &cap2, &cap3);
                (void)full_match3;
            }
        }

        // Test Consume - create fresh string_views for each test
        if (text.size() < 1000) {
            re2::RE2 re_copy(pattern, options);
            if (re_copy.ok()) {
                absl::string_view input_view = text;
                bool consume0 = re2::RE2::Consume(&input_view, re_copy);
                (void)consume0;
            }
        }
        
        if (text.size() < 1000) {
            re2::RE2 re_copy(pattern, options);
            if (re_copy.ok()) {
                absl::string_view input_view = text;
                std::string cap1_local;
                bool consume1 = re2::RE2::Consume(&input_view, re_copy, &cap1_local);
                (void)consume1;
            }
        }
        
        if (text.size() < 1000) {
            re2::RE2 re_copy(pattern, options);
            if (re_copy.ok()) {
                absl::string_view input_view = text;
                std::string cap1_local, cap2_local;
                bool consume2 = re2::RE2::Consume(&input_view, re_copy, &cap1_local, &cap2_local);
                (void)consume2;
            }
        }
    }

    // Test with different options
    re2::RE2::Options options2;
    options2.set_never_capture(!never_capture_val);
    re2::RE2 re2(pattern, options2);
    
    if (re2.ok() && text.size() < 1000) {
        std::string cap;
        bool test = re2::RE2::PartialMatch(text, re2, &cap);
        (void)test;
        
        int groups = re2.NumberOfCapturingGroups();
        (void)groups;
    }

    // Test with empty pattern (only if we have enough data)
    if (Size > 10 && (Data[0] & 0x02) != 0 && text.size() < 1000) {
        std::string empty_pattern;
        re2::RE2 empty_re(empty_pattern, options);
        if (empty_re.ok()) {
            bool empty_test = re2::RE2::PartialMatch(text, empty_re);
            (void)empty_test;
        }
    }

    return 0;
}