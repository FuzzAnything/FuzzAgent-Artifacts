// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::RE2::ok at re2.h:307:8 in re2.h
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
#include "re2/re2.h"
#include <cstdint>
#include <cstddef>
#include <string>

extern "C" int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Use first byte to determine encoding option
    uint8_t encoding_choice = Data[0] % 2;
    re2::RE2::CannedOptions canned_opt = encoding_choice == 0 ? 
        re2::RE2::DefaultOptions : re2::RE2::Latin1;

    // Create pattern from remaining data
    std::string pattern_str;
    if (Size > 1) {
        pattern_str.assign(reinterpret_cast<const char*>(Data + 1), Size - 1);
    } else {
        pattern_str = "test";  // Default pattern if no data
    }

    try {
        // Create options and RE2 object
        re2::RE2::Options options(canned_opt);
        re2::RE2 re(pattern_str, options);

        if (!re.ok()) {
            return 0;
        }

        // First sequence of calls
        const std::string& pattern1 = re.pattern();
        const re2::RE2::Options& opts1 = re.options();
        re2::RE2::Options::Encoding enc1 = opts1.encoding();

        // Second sequence of calls
        const std::string& pattern2 = re.pattern();
        const re2::RE2::Options& opts2 = re.options();
        re2::RE2::Options::Encoding enc2 = opts2.encoding();

        // Use the results to avoid optimization
        static volatile auto dummy1 = pattern1.c_str();
        static volatile auto dummy2 = pattern2.c_str();
        static volatile auto dummy3 = enc1;
        static volatile auto dummy4 = enc2;
        (void)dummy1;
        (void)dummy2;
        (void)dummy3;
        (void)dummy4;

    } catch (...) {
        // Catch any exceptions to prevent crashes
    }

    return 0;
}