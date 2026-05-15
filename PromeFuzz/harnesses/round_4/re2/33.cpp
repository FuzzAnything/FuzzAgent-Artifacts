// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::RE2::Options::set_literal at re2.h:712:10 in re2.h
// re2::RE2::Options::set_case_sensitive at re2.h:724:10 in re2.h
// re2::RE2::Options::set_never_capture at re2.h:721:10 in re2.h
// re2::RE2::Options::set_perl_classes at re2.h:727:10 in re2.h
// re2::RE2::Options::set_word_boundary at re2.h:730:10 in re2.h
// re2::RE2::Options::never_capture at re2.h:720:10 in re2.h
// re2::RE2::Options::set_literal at re2.h:712:10 in re2.h
// re2::RE2::Options::set_case_sensitive at re2.h:724:10 in re2.h
// re2::RE2::Options::set_never_capture at re2.h:721:10 in re2.h
// re2::RE2::Options::never_capture at re2.h:720:10 in re2.h
// re2::RE2::Options::never_capture at re2.h:720:10 in re2.h
// re2::RE2::Options::never_capture at re2.h:720:10 in re2.h
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

static void test_options(const uint8_t* data, size_t size) {
    if (size < 6) return;
    
    // Create options object
    re2::RE2::Options options(re2::RE2::DefaultOptions);
    
    // Use first 6 bytes as boolean values for the setters
    bool literal = data[0] & 1;
    bool case_sensitive = data[1] & 1;
    bool never_capture = data[2] & 1;
    bool perl_classes = data[3] & 1;
    bool word_boundary = data[4] & 1;
    
    // Call the setter functions
    options.set_literal(literal);
    options.set_case_sensitive(case_sensitive);
    options.set_never_capture(never_capture);
    options.set_perl_classes(perl_classes);
    options.set_word_boundary(word_boundary);
    
    // Call the getter function
    bool never_capture_result = options.never_capture();
    
    // Use the result to avoid dead code elimination
    if (never_capture_result) {
        // Do nothing, just use the value
        volatile bool dummy = never_capture_result;
        (void)dummy;
    }
    
    // Test with different canned options
    re2::RE2::Options options_latin1(re2::RE2::Latin1);
    re2::RE2::Options options_posix(re2::RE2::POSIX);
    re2::RE2::Options options_quiet(re2::RE2::Quiet);
    
    // Apply settings to different option objects
    options_latin1.set_literal(!literal);
    options_posix.set_case_sensitive(!case_sensitive);
    options_quiet.set_never_capture(!never_capture);
    
    // Test getter on different objects
    bool never_capture_latin1 = options_latin1.never_capture();
    bool never_capture_posix = options_posix.never_capture();
    bool never_capture_quiet = options_quiet.never_capture();
    
    // Use results to avoid dead code elimination
    volatile bool dummy1 = never_capture_latin1;
    volatile bool dummy2 = never_capture_posix;
    volatile bool dummy3 = never_capture_quiet;
    (void)dummy1; (void)dummy2; (void)dummy3;
}

extern "C" int LLVMFuzzerTestOneInput_33(const uint8_t *Data, size_t Size) {
    test_options(Data, Size);
    return 0;
}