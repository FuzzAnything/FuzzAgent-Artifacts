// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::RE2::Consume at re2.h:445:15 in re2.h
// re2::RE2::ok at re2.h:307:8 in re2.h
// re2::RE2::error_code at re2.h:320:13 in re2.h
// re2::RE2::Consume at re2.h:445:15 in re2.h
// re2::RE2::Consume at re2.h:445:15 in re2.h
// re2::RE2::Consume at re2.h:445:15 in re2.h
// re2::RE2::Consume at re2.h:445:15 in re2.h
// re2::RE2::Consume at re2.h:445:15 in re2.h
// re2::RE2::Options::set_never_capture at re2.h:721:10 in re2.h
// re2::RE2::Options::never_capture at re2.h:720:10 in re2.h
// re2::RE2::Options::set_never_capture at re2.h:721:10 in re2.h
// re2::RE2::Options::never_capture at re2.h:720:10 in re2.h
// re2::RE2::ok at re2.h:307:8 in re2.h
// re2::RE2::error_code at re2.h:320:13 in re2.h
// re2::RE2::NumberOfCapturingGroups at re2.h:551:7 in re2.h
// re2::RE2::ok at re2.h:307:8 in re2.h
// re2::RE2::error_code at re2.h:320:13 in re2.h
// re2::RE2::NumberOfCapturingGroups at re2.h:551:7 in re2.h
// re2::RE2::ok at re2.h:307:8 in re2.h
// re2::RE2::error_code at re2.h:320:13 in re2.h
// re2::RE2::PartialMatch at re2.h:427:15 in re2.h
// re2::RE2::ok at re2.h:307:8 in re2.h
// re2::RE2::error_code at re2.h:320:13 in re2.h
// re2::RE2::PartialMatch at re2.h:427:15 in re2.h
// re2::RE2::PartialMatch at re2.h:427:15 in re2.h
// re2::RE2::PartialMatch at re2.h:427:15 in re2.h
// re2::RE2::PartialMatch at re2.h:427:15 in re2.h
// re2::RE2::PartialMatch at re2.h:427:15 in re2.h
// re2::RE2::PartialMatch at re2.h:427:15 in re2.h
// re2::RE2::ok at re2.h:307:8 in re2.h
// re2::RE2::error_code at re2.h:320:13 in re2.h
// re2::RE2::FullMatch at re2.h:411:15 in re2.h
// re2::RE2::ok at re2.h:307:8 in re2.h
// re2::RE2::error_code at re2.h:320:13 in re2.h
// re2::RE2::FullMatch at re2.h:411:15 in re2.h
// re2::RE2::FullMatch at re2.h:411:15 in re2.h
// re2::RE2::FullMatch at re2.h:411:15 in re2.h
// re2::RE2::FullMatch at re2.h:411:15 in re2.h
// re2::RE2::FullMatch at re2.h:411:15 in re2.h
// re2::RE2::FullMatch at re2.h:411:15 in re2.h
// re2::RE2::ok at re2.h:307:8 in re2.h
// re2::RE2::error_code at re2.h:320:13 in re2.h
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
#include <stddef.h>
#include <string>
#include <vector>

static void test_partial_match(const std::string& pattern, const std::string& text) {
    if (pattern.empty()) return;
    
    re2::RE2 re(pattern);
    if (!re.ok()) return;
    
    // Only proceed if the RE2 object is valid
    if (re.error_code() != re2::RE2::NoError) return;
    
    // Check if pattern is too large to avoid internal errors
    if (pattern.size() > 1000) return;
    
    std::string capture1, capture2;
    absl::string_view capture3;
    int capture4 = 0;
    
    // Test with different numbers of capture arguments
    if (!text.empty()) {
        // Test with no capture arguments first
        re2::RE2::PartialMatch(text, re);
        
        // Only proceed with captures if we have a valid RE2 object
        if (re.ok() && re.error_code() == re2::RE2::NoError) {
            re2::RE2::PartialMatch(text, re, &capture1);
            re2::RE2::PartialMatch(text, re, &capture1, &capture2);
            re2::RE2::PartialMatch(text, re, &capture3);
            re2::RE2::PartialMatch(text, re, &capture4);
            
            // Test with NULL arguments
            re2::RE2::PartialMatch(text, re, static_cast<std::string*>(nullptr));
            re2::RE2::PartialMatch(text, re, &capture1, static_cast<std::string*>(nullptr));
        }
    }
}

static void test_full_match(const std::string& pattern, const std::string& text) {
    if (pattern.empty()) return;
    
    re2::RE2 re(pattern);
    if (!re.ok()) return;
    
    // Only proceed if the RE2 object is valid
    if (re.error_code() != re2::RE2::NoError) return;
    
    // Check if pattern is too large to avoid internal errors
    if (pattern.size() > 1000) return;
    
    std::string capture1, capture2;
    absl::string_view capture3;
    int capture4 = 0;
    
    // Test with different numbers of capture arguments
    if (!text.empty()) {
        // Test with no capture arguments first
        re2::RE2::FullMatch(text, re);
        
        // Only proceed with captures if we have a valid RE2 object
        if (re.ok() && re.error_code() == re2::RE2::NoError) {
            re2::RE2::FullMatch(text, re, &capture1);
            re2::RE2::FullMatch(text, re, &capture1, &capture2);
            re2::RE2::FullMatch(text, re, &capture3);
            re2::RE2::FullMatch(text, re, &capture4);
            
            // Test with NULL arguments
            re2::RE2::FullMatch(text, re, static_cast<std::string*>(nullptr));
            re2::RE2::FullMatch(text, re, &capture1, static_cast<std::string*>(nullptr));
        }
    }
}

static void test_consume(const std::string& pattern, const std::string& text) {
    if (pattern.empty()) return;
    
    re2::RE2 re(pattern);
    if (!re.ok()) return;
    
    // Only proceed if the RE2 object is valid
    if (re.error_code() != re2::RE2::NoError) return;
    
    // Check if pattern is too large to avoid internal errors
    if (pattern.size() > 1000) return;
    
    std::string capture1, capture2;
    absl::string_view capture3;
    int capture4 = 0;
    
    // Test with different numbers of capture arguments
    if (!text.empty()) {
        // Test with no capture arguments first
        absl::string_view input1 = text;
        re2::RE2::Consume(&input1, re);
        
        // Only proceed with captures if we have a valid RE2 object
        if (re.ok() && re.error_code() == re2::RE2::NoError) {
            absl::string_view input2 = text;
            re2::RE2::Consume(&input2, re, &capture1);
            
            absl::string_view input3 = text;
            re2::RE2::Consume(&input3, re, &capture1, &capture2);
            
            absl::string_view input4 = text;
            re2::RE2::Consume(&input4, re, &capture3);
            
            absl::string_view input5 = text;
            re2::RE2::Consume(&input5, re, &capture4);
            
            // Test with NULL arguments
            absl::string_view input6 = text;
            re2::RE2::Consume(&input6, re, static_cast<std::string*>(nullptr));
        }
    }
}

static void test_options(const std::string& pattern) {
    if (pattern.empty()) return;
    
    re2::RE2::Options options;
    
    // Test set_never_capture with different values
    options.set_never_capture(true);
    bool never1 = options.never_capture();
    
    options.set_never_capture(false);
    bool never2 = options.never_capture();
    
    // Create RE2 with options - limit pattern size
    if (pattern.size() <= 1000) {
        re2::RE2 re1(pattern, options);
        if (re1.ok() && re1.error_code() == re2::RE2::NoError) {
            int groups1 = re1.NumberOfCapturingGroups();
        }
        
        // Create RE2 with default options
        re2::RE2 re2(pattern);
        if (re2.ok() && re2.error_code() == re2::RE2::NoError) {
            int groups2 = re2.NumberOfCapturingGroups();
        }
    }
}

extern "C" int LLVMFuzzerTestOneInput_45(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;
    
    // Split input into pattern and text parts
    size_t pattern_size = Size / 2;
    size_t text_size = Size - pattern_size;
    
    // Ensure we have non-empty strings
    if (pattern_size == 0) pattern_size = 1;
    if (text_size == 0) text_size = 1;
    
    // Adjust if our adjustments exceed the input size
    if (pattern_size + text_size > Size) {
        pattern_size = Size - 1;
        text_size = 1;
    }
    
    // Limit pattern size to avoid internal errors
    if (pattern_size > 1000) {
        pattern_size = 1000;
        text_size = Size - pattern_size;
        if (text_size == 0) text_size = 1;
    }
    
    std::string pattern(reinterpret_cast<const char*>(Data), pattern_size);
    std::string text(reinterpret_cast<const char*>(Data + pattern_size), text_size);
    
    // Test all API functions
    test_partial_match(pattern, text);
    test_full_match(pattern, text);
    test_consume(pattern, text);
    test_options(pattern);
    
    return 0;
}