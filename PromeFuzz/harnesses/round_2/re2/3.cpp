// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::RE2::Options::set_never_nl at re2.h:715:10 in re2.h
// re2::RE2::Options::set_dot_nl at re2.h:718:10 in re2.h
// re2::RE2::Options::set_never_capture at re2.h:721:10 in re2.h
// re2::RE2::Options::set_case_sensitive at re2.h:724:10 in re2.h
// re2::RE2::Options::set_perl_classes at re2.h:727:10 in re2.h
// re2::RE2::Options::set_word_boundary at re2.h:730:10 in re2.h
// re2::RE2::Options::set_one_line at re2.h:733:10 in re2.h
// re2::RE2::ok at re2.h:307:8 in re2.h
// re2::RE2::PartialMatch at re2.h:427:15 in re2.h
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
#include <cstring>

#include "re2/re2.h"

static void setOptionsFromData(re2::RE2::Options& options, const uint8_t* data, size_t size) {
    if (size < 7) return;
    
    // Use first 7 bytes as boolean values for the options
    options.set_never_nl(data[0] & 1);
    options.set_dot_nl(data[1] & 1);
    options.set_never_capture(data[2] & 1);
    options.set_case_sensitive(data[3] & 1);
    options.set_perl_classes(data[4] & 1);
    options.set_word_boundary(data[5] & 1);
    options.set_one_line(data[6] & 1);
}

extern "C" int LLVMFuzzerTestOneInput_3(const uint8_t* Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Create options with default settings
    re2::RE2::Options options(re2::RE2::DefaultOptions);
    
    // Set options based on fuzzer input
    setOptionsFromData(options, Data, Size);
    
    // Try to create RE2 objects with different canned options to explore more states
    re2::RE2::Options latin1_options(re2::RE2::Latin1);
    re2::RE2::Options posix_options(re2::RE2::POSIX);
    re2::RE2::Options quiet_options(re2::RE2::Quiet);
    
    // Apply the same settings to other option instances
    setOptionsFromData(latin1_options, Data, Size);
    setOptionsFromData(posix_options, Data, Size);
    setOptionsFromData(quiet_options, Data, Size);
    
    // Try to create RE2 patterns with the configured options
    // Use a simple pattern to avoid crashes from invalid regex
    const char* simple_pattern = "test";
    
    // Create RE2 objects with different option configurations
    // These may fail to compile if pattern is invalid, but we use a fixed simple pattern
    re2::RE2 re1(simple_pattern, options);
    re2::RE2 re2(simple_pattern, latin1_options);
    re2::RE2 re3(simple_pattern, posix_options);
    re2::RE2 re4(simple_pattern, quiet_options);
    
    // Also try with pattern derived from input (limited to avoid crashes)
    if (Size > 7) {
        // Use up to 256 bytes for pattern to avoid excessive memory usage
        size_t pattern_len = Size - 7;
        if (pattern_len > 256) pattern_len = 256;
        
        std::string input_pattern(reinterpret_cast<const char*>(Data + 7), pattern_len);
        
        // Try to create RE2 with input pattern (may fail silently)
        re2::RE2 re5(input_pattern, options);
        
        // If pattern compiles successfully, try a simple match
        if (re5.ok()) {
            std::string test_string = "test string";
            re2::RE2::PartialMatch(test_string, re5);
        }
    }
    
    return 0;
}