// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::RE2::CapturingGroupNames at re2.cc:406:40 in re2.h
// re2::RE2::QuoteMeta at re2.cc:559:18 in re2.h
// re2::RE2::Regexp at re2.h:340:16 in re2.h
// re2::RE2::Set::Add at set.cc:64:15 in set.h
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
#include "re2/set.h"

extern "C" int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a string from the fuzzer input
    std::string input(reinterpret_cast<const char*>(Data), Size);
    
    // 1. Create an RE2 object and call CapturingGroupNames
    try {
        re2::RE2 re(input);
        const std::map<int, std::string>& group_names = re.CapturingGroupNames();
        // Use the result to avoid optimization
        static volatile size_t dummy = group_names.size();
        (void)dummy;
    } catch (...) {
        // Ignore exceptions
    }
    
    // 2. Call QuoteMeta (static method)
    try {
        std::string quoted = re2::RE2::QuoteMeta(input);
        static volatile size_t dummy2 = quoted.size();
        (void)dummy2;
    } catch (...) {
        // Ignore exceptions
    }
    
    // 3. Create another RE2 object and call Regexp
    try {
        re2::RE2 re2(input);
        re2::Regexp* regexp = re2.Regexp();
        static volatile void* dummy3 = regexp;
        (void)dummy3;
    } catch (...) {
        // Ignore exceptions
    }
    
    // 4. Create an RE2::Set and call Add
    try {
        re2::RE2::Options options;
        re2::RE2::Set set(options, re2::RE2::ANCHOR_BOTH);
        std::string error;
        int index = set.Add(input, &error);
        static volatile int dummy4 = index;
        (void)dummy4;
    } catch (...) {
        // Ignore exceptions
    }
    
    return 0;
}