// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::RE2::ok at re2.h:307:8 in re2.h
// re2::RE2::error_code at re2.h:320:13 in re2.h
// re2::RE2::error at re2.h:316:22 in re2.h
// re2::RE2::error_arg at re2.h:324:22 in re2.h
// re2::RE2::error at re2.h:316:22 in re2.h
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

extern "C" int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create pattern string from fuzzer input
    std::string pattern(reinterpret_cast<const char*>(Data), Size);
    
    // Create RE2 object with the pattern
    re2::RE2 re(pattern.c_str());
    
    // Call functions in specified order
    bool is_ok = re.ok();
    re2::RE2::ErrorCode code = re.error_code();
    const std::string& error_str1 = re.error();
    const std::string& error_arg_str = re.error_arg();
    const std::string& error_str2 = re.error();
    
    // Use the results in some way to avoid dead code elimination
    static volatile bool dummy = false;
    if (is_ok) {
        dummy = true;
    }
    if (code != re2::RE2::NoError) {
        dummy = !dummy;
    }
    if (!error_str1.empty()) {
        dummy = true;
    }
    if (!error_arg_str.empty()) {
        dummy = !dummy;
    }
    if (!error_str2.empty()) {
        dummy = true;
    }
    
    return 0;
}