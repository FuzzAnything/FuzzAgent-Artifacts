// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::RE2::Options::set_dot_nl at re2.h:718:10 in re2.h
// re2::RE2::Options::set_word_boundary at re2.h:730:10 in re2.h
// re2::RE2::Options::log_errors at re2.h:708:10 in re2.h
// re2::RE2::Options::posix_syntax at re2.h:702:10 in re2.h
// re2::RE2::Options::never_nl at re2.h:714:10 in re2.h
// re2::RE2::Options::literal at re2.h:711:10 in re2.h
// re2::RE2::Options::set_dot_nl at re2.h:718:10 in re2.h
// re2::RE2::Options::set_word_boundary at re2.h:730:10 in re2.h
// re2::RE2::Options::log_errors at re2.h:708:10 in re2.h
// re2::RE2::Options::posix_syntax at re2.h:702:10 in re2.h
// re2::RE2::Options::never_nl at re2.h:714:10 in re2.h
// re2::RE2::Options::literal at re2.h:711:10 in re2.h
// re2::RE2::Options::log_errors at re2.h:708:10 in re2.h
// re2::RE2::Options::posix_syntax at re2.h:702:10 in re2.h
// re2::RE2::Options::never_nl at re2.h:714:10 in re2.h
// re2::RE2::Options::literal at re2.h:711:10 in re2.h
// re2::RE2::Options::set_dot_nl at re2.h:718:10 in re2.h
// re2::RE2::Options::set_word_boundary at re2.h:730:10 in re2.h
// re2::RE2::Options::log_errors at re2.h:708:10 in re2.h
// re2::RE2::Options::posix_syntax at re2.h:702:10 in re2.h
// re2::RE2::Options::never_nl at re2.h:714:10 in re2.h
// re2::RE2::Options::literal at re2.h:711:10 in re2.h
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

#include "re2/re2.h"

extern "C" int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size) {
    // Create an Options object
    re2::RE2::Options options;
    
    // Use fuzz data to determine boolean values for setters
    // We'll use the first byte for various boolean flags
    if (Size > 0) {
        // Use bit 0 for set_dot_nl
        bool dot_nl = Data[0] & 0x01;
        options.set_dot_nl(dot_nl);
        
        // Use bit 1 for set_word_boundary
        bool word_boundary = Data[0] & 0x02;
        options.set_word_boundary(word_boundary);
    }
    
    // Call all getter functions to test them
    // These should not crash with any input
    (void)options.log_errors();
    (void)options.posix_syntax();
    (void)options.never_nl();
    (void)options.literal();
    
    // Test with different boolean values if we have more data
    if (Size > 1) {
        bool dot_nl2 = Data[1] & 0x01;
        options.set_dot_nl(dot_nl2);
        
        bool word_boundary2 = Data[1] & 0x02;
        options.set_word_boundary(word_boundary2);
        
        // Call getters again with different state
        (void)options.log_errors();
        (void)options.posix_syntax();
        (void)options.never_nl();
        (void)options.literal();
    }
    
    // Create another Options object with default constructor
    // and test getters on fresh object
    re2::RE2::Options options2;
    (void)options2.log_errors();
    (void)options2.posix_syntax();
    (void)options2.never_nl();
    (void)options2.literal();
    
    // Set some values on the second object
    if (Size > 2) {
        bool dot_nl3 = Data[2] & 0x01;
        options2.set_dot_nl(dot_nl3);
        
        bool word_boundary3 = Data[2] & 0x02;
        options2.set_word_boundary(word_boundary3);
    }
    
    // Call getters on second object
    (void)options2.log_errors();
    (void)options2.posix_syntax();
    (void)options2.never_nl();
    (void)options2.literal();
    
    return 0;
}