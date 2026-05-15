// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::RE2::ok at re2.h:307:8 in re2.h
// re2::RE2::CapturingGroupNames at re2.cc:406:40 in re2.h
// re2::RE2::QuoteMeta at re2.cc:559:18 in re2.h
// re2::RE2::ok at re2.h:307:8 in re2.h
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
#include <cstdint>
#include <cstring>
#include <string>
#include <map>

#include "re2/re2.h"
#include "re2/set.h"

extern "C" int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Convert input to string for pattern usage
    std::string input(reinterpret_cast<const char*>(Data), Size);

    // 1. Test RE2::CapturingGroupNames
    {
        // Create RE2 object with input as pattern
        re2::RE2 re(input);
        
        // Only call CapturingGroupNames if pattern compiled successfully
        if (re.ok()) {
            const std::map<int, std::string>& group_names = re.CapturingGroupNames();
            // Access the map to ensure it's evaluated
            for (const auto& pair : group_names) {
                // Do nothing, just ensure the map is accessed
                (void)pair;
            }
        }
    }

    // 2. Test RE2::QuoteMeta (static function)
    {
        std::string quoted = re2::RE2::QuoteMeta(input);
        // Ensure the result is used
        (void)quoted;
    }

    // 3. Test RE2::Regexp
    {
        // Create another RE2 object
        re2::RE2 re2(input);
        
        if (re2.ok()) {
            re2::Regexp* regexp = re2.Regexp();
            // Ensure the pointer is used
            (void)regexp;
        }
    }

    // 4. Test RE2::Set::Add
    {
        // Create RE2::Set with default options
        re2::RE2::Set set(re2::RE2::DefaultOptions, re2::RE2::UNANCHORED);
        
        std::string error;
        int index = set.Add(input, &error);
        // Ensure the result is used
        (void)index;
        (void)error;
    }

    return 0;
}