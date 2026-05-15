// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::RE2::pattern at re2.h:312:22 in re2.h
// re2::RE2::options at re2.h:758:18 in re2.h
// re2::RE2::Options::encoding at re2.h:699:14 in re2.h
// re2::RE2::pattern at re2.h:312:22 in re2.h
// re2::RE2::options at re2.h:758:18 in re2.h
// re2::RE2::Options::encoding at re2.h:699:14 in re2.h
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

extern "C" int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create pattern string from fuzzer input
    std::string pattern_str(reinterpret_cast<const char*>(Data), Size);
    
    // Create RE2 object with the pattern
    re2::RE2 re(pattern_str.c_str());
    
    // First sequence of API calls
    const std::string& pattern1 = re.pattern();
    const re2::RE2::Options& options1 = re.options();
    re2::RE2::Options::Encoding encoding1 = options1.encoding();
    
    // Second sequence of API calls
    const std::string& pattern2 = re.pattern();
    const re2::RE2::Options& options2 = re.options();
    re2::RE2::Options::Encoding encoding2 = options2.encoding();
    
    // Use the variables to avoid compiler warnings
    (void)pattern1;
    (void)pattern2;
    (void)encoding1;
    (void)encoding2;
    
    return 0;
}