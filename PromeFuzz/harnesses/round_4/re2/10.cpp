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

extern "C" int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Convert fuzzer input to string for regex pattern
    std::string pattern(reinterpret_cast<const char*>(Data), Size);
    
    // Create RE2 object with the pattern
    re2::RE2 re(pattern);
    
    // Call functions in specified order
    bool ok_result = re.ok();
    re2::RE2::ErrorCode error_code_result = re.error_code();
    const std::string& error_result1 = re.error();
    const std::string& error_arg_result = re.error_arg();
    const std::string& error_result2 = re.error();
    
    // Use the results to avoid dead code elimination
    static volatile bool keep_ok = ok_result;
    static volatile re2::RE2::ErrorCode keep_error_code = error_code_result;
    static volatile const std::string* keep_error1 = &error_result1;
    static volatile const std::string* keep_error_arg = &error_arg_result;
    static volatile const std::string* keep_error2 = &error_result2;
    
    (void)keep_ok;
    (void)keep_error_code;
    (void)keep_error1;
    (void)keep_error_arg;
    (void)keep_error2;
    
    return 0;
}