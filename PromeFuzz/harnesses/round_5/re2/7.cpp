// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::RE2::pattern at re2.h:312:22 in re2.h
// re2::RE2::error at re2.h:316:22 in re2.h
// re2::RE2::ok at re2.h:307:8 in re2.h
// re2::RE2::error_code at re2.h:320:13 in re2.h
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
#include <cstddef>
#include <string>

extern "C" int LLVMFuzzerTestOneInput_7(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Convert input to string for pattern
    std::string pattern_str(reinterpret_cast<const char*>(Data), Size);
    
    // Create RE2 object with default options
    re2::RE2::Options options;
    re2::RE2 re(pattern_str, options);
    
    // Call functions in specified order
    const std::string& pattern_result = re.pattern();
    const std::string& error_result = re.error();
    bool ok_result = re.ok();
    re2::RE2::ErrorCode error_code_result = re.error_code();
    
    // Use results to avoid optimization
    static volatile auto sink = [&]() {
        return pattern_result.size() + error_result.size() + ok_result + error_code_result;
    }();
    
    return 0;
}