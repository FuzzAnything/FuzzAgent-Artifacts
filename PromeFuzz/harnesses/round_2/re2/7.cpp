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
#include <stdint.h>
#include <string>
#include <map>

extern "C" int LLVMFuzzerTestOneInput_7(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a pattern string from fuzz data
    std::string pattern(reinterpret_cast<const char*>(Data), Size);
    
    // 1. Create RE2 object and call CapturingGroupNames
    {
        re2::RE2 re(pattern);
        const std::map<int, std::string>& group_names = re.CapturingGroupNames();
        // Use the result to avoid optimization
        for (const auto& pair : group_names) {
            (void)pair.first;
            (void)pair.second;
        }
    }
    
    // 2. Call QuoteMeta (static function)
    {
        std::string quoted = re2::RE2::QuoteMeta(pattern);
        (void)quoted;
    }
    
    // 3. Create another RE2 object and call Regexp
    {
        re2::RE2 re(pattern);
        re2::Regexp* regexp = re.Regexp();
        (void)regexp;
    }
    
    // 4. Create RE2::Set and call Add
    {
        re2::RE2::Options options;
        re2::RE2::Set set(options, re2::RE2::ANCHOR_BOTH);
        std::string error;
        int index = set.Add(pattern, &error);
        (void)index;
        (void)error;
    }
    
    return 0;
}