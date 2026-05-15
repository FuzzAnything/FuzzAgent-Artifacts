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
#include <cstdint>
#include <cstddef>
#include <string>
#include "re2/re2.h"

extern "C" int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    try {
        std::string pattern(reinterpret_cast<const char*>(Data), Size);
        re2::RE2 re(pattern);

        bool ok_result = re.ok();
        re2::RE2::ErrorCode error_code_result = re.error_code();
        const std::string& error_result1 = re.error();
        const std::string& error_arg_result = re.error_arg();
        const std::string& error_result2 = re.error();

        (void)ok_result;
        (void)error_code_result;
        (void)error_result1;
        (void)error_arg_result;
        (void)error_result2;
    } catch (...) {
        // Catch any exceptions to prevent crashes from propagating
    }

    return 0;
}