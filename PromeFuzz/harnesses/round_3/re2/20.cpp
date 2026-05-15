// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::RE2::Options::set_dot_nl at re2.h:718:10 in re2.h
// re2::RE2::Options::set_max_mem at re2.h:697:10 in re2.h
// re2::RE2::Options::Copy at re2.h:735:10 in re2.h
// re2::RE2::Options::max_mem at re2.h:696:13 in re2.h
// re2::RE2::Options::ParseFlags at re2.cc:166:19 in re2.h
// re2::RE2::options at re2.h:758:18 in re2.h
// re2::RE2::Options::ParseFlags at re2.cc:166:19 in re2.h
// re2::RE2::Options::max_mem at re2.h:696:13 in re2.h
// re2::RE2::Options::set_dot_nl at re2.h:718:10 in re2.h
// re2::RE2::Options::set_max_mem at re2.h:697:10 in re2.h
// re2::RE2::options at re2.h:758:18 in re2.h
// re2::RE2::Options::ParseFlags at re2.cc:166:19 in re2.h
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
#include <cstdlib>
#include <string>

extern "C" int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Create source and destination options objects
    re2::RE2::Options src_options;
    re2::RE2::Options dst_options;

    // Use fuzz data to set various options in src_options
    // Use first byte to determine dot_nl setting
    bool dot_nl = (Data[0] & 0x01) != 0;
    src_options.set_dot_nl(dot_nl);

    // Use second and third bytes (if available) to set max_mem
    int64_t max_mem = 8192; // Default 8KB for safety
    if (Size >= 3) {
        max_mem = static_cast<int64_t>(Data[1]) << 8 | Data[2];
        // Ensure positive value
        if (max_mem < 0) max_mem = -max_mem;
        // Limit to reasonable range to avoid excessive memory usage
        if (max_mem > 1000000) max_mem = 1000000;
    }
    src_options.set_max_mem(max_mem);

    // Test Copy function
    dst_options.Copy(src_options);

    // Test max_mem getter
    int64_t retrieved_max_mem = dst_options.max_mem();

    // Test ParseFlags function
    int flags = dst_options.ParseFlags();

    // Create a simple RE2 object with a pattern derived from fuzz data
    // Use a safe pattern to avoid compilation errors
    std::string pattern = "test";
    if (Size >= 4) {
        // Use up to 50 bytes for pattern to avoid overly complex patterns
        size_t pattern_len = Data[3] % 50;
        if (pattern_len > 0 && Size >= 4 + pattern_len) {
            pattern = std::string(reinterpret_cast<const char*>(Data + 4), pattern_len);
        }
    }

    // Create RE2 object with the pattern
    re2::RE2 re(pattern, src_options);

    // Test options() getter
    const re2::RE2::Options& re_options = re.options();

    // Test ParseFlags on the RE2's options
    int re_flags = re_options.ParseFlags();

    // Test max_mem on the RE2's options
    int64_t re_max_mem = re_options.max_mem();

    // Additional test: create another RE2 object with different options
    if (Size >= 5) {
        re2::RE2::Options alt_options;
        bool alt_dot_nl = (Data[4] & 0x01) != 0;
        alt_options.set_dot_nl(alt_dot_nl);
        
        if (Size >= 7) {
            int64_t alt_max_mem = static_cast<int64_t>(Data[5]) << 8 | Data[6];
            if (alt_max_mem < 0) alt_max_mem = -alt_max_mem;
            if (alt_max_mem > 1000000) alt_max_mem = 1000000;
            alt_options.set_max_mem(alt_max_mem);
        }
        
        re2::RE2 alt_re("alt", alt_options);
        const re2::RE2::Options& alt_re_options = alt_re.options();
        int alt_flags = alt_re_options.ParseFlags();
    }

    return 0;
}