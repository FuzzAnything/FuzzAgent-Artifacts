// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::RE2::Options::set_never_nl at re2.h:715:10 in re2.h
// re2::RE2::Options::set_dot_nl at re2.h:718:10 in re2.h
// re2::RE2::Options::word_boundary at re2.h:729:10 in re2.h
// re2::RE2::Options::one_line at re2.h:732:10 in re2.h
// re2::RE2::Options::dot_nl at re2.h:717:10 in re2.h
// re2::RE2::Options::never_nl at re2.h:714:10 in re2.h
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

static void test_options_with_canned_option(re2::RE2::CannedOptions canned_option, uint8_t bool_flags) {
    re2::RE2::Options options(canned_option);
    
    // Use bits from bool_flags to set different boolean values
    bool never_nl_val = bool_flags & 0x01;
    bool dot_nl_val = bool_flags & 0x02;
    
    // Call setters
    options.set_never_nl(never_nl_val);
    options.set_dot_nl(dot_nl_val);
    
    // Call getters and use results to avoid dead code elimination
    volatile bool word_boundary_result = options.word_boundary();
    volatile bool one_line_result = options.one_line();
    volatile bool dot_nl_result = options.dot_nl();
    volatile bool never_nl_result = options.never_nl();
    
    // Prevent compiler optimizations
    (void)word_boundary_result;
    (void)one_line_result;
    (void)dot_nl_result;
    (void)never_nl_result;
}

extern "C" int LLVMFuzzerTestOneInput_33(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }
    
    // Use first byte to determine canned option (mod 4 for 4 canned options)
    uint8_t canned_option_selector = Data[0] % 4;
    re2::RE2::CannedOptions canned_option;
    
    switch (canned_option_selector) {
        case 0: canned_option = re2::RE2::DefaultOptions; break;
        case 1: canned_option = re2::RE2::Latin1; break;
        case 2: canned_option = re2::RE2::POSIX; break;
        case 3: canned_option = re2::RE2::Quiet; break;
        default: canned_option = re2::RE2::DefaultOptions; break;
    }
    
    // Use second byte (if available) for boolean flags, otherwise use 0
    uint8_t bool_flags = (Size > 1) ? Data[1] : 0;
    
    test_options_with_canned_option(canned_option, bool_flags);
    
    return 0;
}