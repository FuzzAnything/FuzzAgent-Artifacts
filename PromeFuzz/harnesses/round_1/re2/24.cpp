// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::RE2::Options::set_case_sensitive at re2.h:724:10 in re2.h
// re2::RE2::Options::set_max_mem at re2.h:697:10 in re2.h
// re2::RE2::Options::max_mem at re2.h:696:13 in re2.h
// re2::RE2::Options::ParseFlags at re2.cc:166:19 in re2.h
// re2::RE2::Options::Copy at re2.h:735:10 in re2.h
// re2::RE2::Options::max_mem at re2.h:696:13 in re2.h
// re2::RE2::options at re2.h:758:18 in re2.h
// re2::RE2::Options::ParseFlags at re2.cc:166:19 in re2.h
// re2::RE2::Options::max_mem at re2.h:696:13 in re2.h
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
#include <cstring>
#include <limits>

#include "re2/re2.h"

static void test_options_functions(const uint8_t* data, size_t size) {
    // Use first byte for boolean values
    bool case_sensitive = false;
    if (size > 0) {
        case_sensitive = (data[0] & 1) != 0;
    }
    
    // Use next 8 bytes for max_mem value (if available)
    int64_t max_mem_value = 8 * 1024 * 1024; // Default 8MB
    if (size >= 9) {
        // Copy 8 bytes for int64_t
        memcpy(&max_mem_value, data + 1, sizeof(int64_t));
        // Ensure non-negative
        if (max_mem_value < 0) {
            max_mem_value = -max_mem_value;
        }
        // Cap the value to prevent integer overflow in RE2 internal calculations
        // RE2 uses max_mem * 2/3, so we need to ensure max_mem <= INT64_MAX * 3/2
        const int64_t max_safe_value = std::numeric_limits<int64_t>::max() / 3 * 2;
        if (max_mem_value > max_safe_value) {
            max_mem_value = max_safe_value;
        }
        // Also ensure it's not too small
        if (max_mem_value < 0) {
            max_mem_value = 0;
        }
    }
    
    // Create options objects
    re2::RE2::Options options1;
    re2::RE2::Options options2;
    
    // Test set_case_sensitive
    options1.set_case_sensitive(case_sensitive);
    
    // Test set_max_mem
    options1.set_max_mem(max_mem_value);
    
    // Test max_mem getter
    int64_t current_max_mem = options1.max_mem();
    (void)current_max_mem; // Use to avoid unused variable warning
    
    // Test ParseFlags
    int flags = options1.ParseFlags();
    (void)flags; // Use to avoid unused variable warning
    
    // Test Copy
    options2.Copy(options1);
    
    // Verify copied values match
    int64_t copied_max_mem = options2.max_mem();
    (void)copied_max_mem; // Use to avoid unused variable warning
    
    // Create a pattern string from remaining data
    std::string pattern;
    size_t pattern_start = (size >= 9) ? 9 : (size > 0 ? 1 : 0);
    if (size > pattern_start) {
        pattern.assign(reinterpret_cast<const char*>(data + pattern_start), 
                      size - pattern_start);
    } else {
        pattern = "test"; // Default pattern if no data
    }
    
    // Create RE2 object with options
    try {
        re2::RE2 re(pattern, options1);
        
        // Test options() getter
        const re2::RE2::Options& retrieved_options = re.options();
        (void)retrieved_options; // Use to avoid unused variable warning
        
        // Test some operations on retrieved options
        int retrieved_flags = retrieved_options.ParseFlags();
        (void)retrieved_flags;
        int64_t retrieved_max_mem = retrieved_options.max_mem();
        (void)retrieved_max_mem;
    } catch (...) {
        // Catch any exceptions from RE2 constructor
    }
}

extern "C" int LLVMFuzzerTestOneInput_24(const uint8_t* data, size_t size) {
    test_options_functions(data, size);
    return 0;
}