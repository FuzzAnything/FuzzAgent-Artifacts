// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::FilteredRE2::Compile at filtered_re2.cc:70:19 in filtered_re2.h
// re2::FilteredRE2::FirstMatch at filtered_re2.cc:98:18 in filtered_re2.h
// re2::LazyRE2::get at re2.h:1000:8 in re2.h
// re2::FilteredRE2::Add at filtered_re2.cc:51:29 in filtered_re2.h
// re2::FilteredRE2::NumRegexps at filtered_re2.h:90:7 in filtered_re2.h
// re2::FilteredRE2::GetRE2 at filtered_re2.h:93:14 in filtered_re2.h
// re2::FilteredRE2::SlowFirstMatch at filtered_re2.cc:91:18 in filtered_re2.h
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

#include "re2/filtered_re2.h"
#include "re2/re2.h"

static void TestFilteredRE2Functions(const uint8_t* data, size_t size) {
    if (size < 2) return;
    
    re2::FilteredRE2 filtered_re2;
    
    // Add some regex patterns using data
    int pattern_count = data[0] % 5 + 1;  // 1-5 patterns
    size_t offset = 1;
    
    std::vector<int> pattern_ids;
    
    for (int i = 0; i < pattern_count && offset < size; i++) {
        // Determine pattern length from next byte
        size_t pattern_len = data[offset] % 50 + 1;  // 1-50 chars
        offset++;
        
        if (offset + pattern_len > size) {
            pattern_len = size - offset;
        }
        
        if (pattern_len == 0) break;
        
        std::string pattern(reinterpret_cast<const char*>(data + offset), pattern_len);
        offset += pattern_len;
        
        int id = -1;
        re2::RE2::Options options;
        
        // Use different options based on data
        if (i % 3 == 0) {
            options = re2::RE2::Options(re2::RE2::Latin1);
        } else if (i % 3 == 1) {
            options = re2::RE2::Options(re2::RE2::POSIX);
        }
        
        re2::RE2::ErrorCode error = filtered_re2.Add(pattern, options, &id);
        
        if (error == re2::RE2::NoError && id != -1) {
            pattern_ids.push_back(id);
        }
    }
    
    // Test NumRegexps
    int num_regexps = filtered_re2.NumRegexps();
    
    // Test GetRE2 for valid indices
    for (int i = 0; i < num_regexps && i < 10; i++) {
        try {
            const re2::RE2& re2_obj = filtered_re2.GetRE2(i);
            (void)re2_obj;  // Avoid unused variable warning
        } catch (...) {
            // Ignore exceptions
        }
    }
    
    // Create text for matching from remaining data
    if (offset < size) {
        size_t text_len = size - offset;
        if (text_len > 1000) text_len = 1000;  // Limit text size
        
        std::string text(reinterpret_cast<const char*>(data + offset), text_len);
        
        // Test SlowFirstMatch
        int slow_match = filtered_re2.SlowFirstMatch(text);
        (void)slow_match;
        
        // Try to compile if we have patterns
        if (num_regexps > 0) {
            std::vector<std::string> strings_to_match;
            
            // Add some strings to match from the text
            if (text_len > 0) {
                // Use first 100 chars of text as a string to match
                size_t match_str_len = text_len > 100 ? 100 : text_len;
                strings_to_match.push_back(text.substr(0, match_str_len));
            }
            
            // Add a simple string from data if available
            if (size > 0 && strings_to_match.size() < 2) {
                std::string simple_str("test");
                strings_to_match.push_back(simple_str);
            }
            
            // Compile the filtered RE2
            filtered_re2.Compile(&strings_to_match);
            
            // Test FirstMatch if compilation succeeded
            // Create atoms vector with some indices
            std::vector<int> atoms;
            for (int i = 0; i < num_regexps && i < 10; i++) {
                atoms.push_back(i);
            }
            
            if (!atoms.empty()) {
                int first_match = filtered_re2.FirstMatch(text, atoms);
                (void)first_match;
            }
        }
    }
}

static void TestLazyRE2Functions(const uint8_t* data, size_t size) {
    if (size == 0) return;
    
    // Use first byte to determine pattern length (max 255)
    size_t pattern_len = data[0] % 256;
    if (pattern_len == 0) pattern_len = 1;
    
    // Ensure we don't exceed available data
    if (pattern_len > size - 1) {
        pattern_len = size - 1;
    }
    
    if (pattern_len == 0) return;
    
    std::string pattern(reinterpret_cast<const char*>(data + 1), pattern_len);
    
    // Create a static LazyRE2 with a fixed pattern
    // LazyRE2 doesn't have a public constructor that takes a string directly
    // We'll create a static RE2 object and test the LazyRE2 pattern
    static re2::LazyRE2 lazy_re2;
    
    // For testing purposes, we'll create a simple RE2 object directly
    // and test the get() method on a default LazyRE2
    try {
        re2::RE2* re2_ptr = lazy_re2.get();
        (void)re2_ptr;  // Avoid unused variable warning
    } catch (...) {
        // Ignore exceptions
    }
    
    // Also test with a RE2 object created from the pattern
    re2::RE2 re2_obj(pattern);
    (void)re2_obj;
}

extern "C" int LLVMFuzzerTestOneInput_51(const uint8_t* data, size_t size) {
    // Test FilteredRE2 functions
    TestFilteredRE2Functions(data, size);
    
    // Test LazyRE2 functions
    TestLazyRE2Functions(data, size);
    
    return 0;
}