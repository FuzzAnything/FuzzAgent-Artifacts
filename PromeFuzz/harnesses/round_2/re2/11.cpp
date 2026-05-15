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
#include <cstdint>
#include <cstring>

extern "C" int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a null-terminated pattern string from fuzz data
    // Leave space for null terminator
    char *pattern = new char[Size + 1];
    memcpy(pattern, Data, Size);
    pattern[Size] = '\0';

    // Create RE2 object with the pattern
    re2::RE2 re(pattern);
    
    // Call functions in specified order
    bool ok_result = re.ok();
    re2::RE2::ErrorCode error_code_result = re.error_code();
    const std::string& error_result1 = re.error();
    const std::string& error_arg_result = re.error_arg();
    const std::string& error_result2 = re.error();

    // Use the results to avoid dead code elimination
    (void)ok_result;
    (void)error_code_result;
    (void)error_result1;
    (void)error_arg_result;
    (void)error_result2;

    delete[] pattern;
    return 0;
}