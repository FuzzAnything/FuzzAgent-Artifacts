// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::RE2::ok at re2.h:307:8 in re2.h
// re2::RE2::Replace at re2.cc:450:11 in re2.h
// re2::RE2::GlobalReplace at re2.cc:472:10 in re2.h
// re2::RE2::PossibleMatchRange at re2.cc:597:11 in re2.h
// re2::RE2::NamedCapturingGroups at re2.cc:395:40 in re2.h
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

static std::string generate_string(const uint8_t* data, size_t size, size_t max_len) {
    if (size == 0) return "";
    size_t len = size % max_len;
    if (len == 0) len = max_len;
    return std::string(reinterpret_cast<const char*>(data), len);
}

extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t* Data, size_t Size) {
    if (Size < 4) return 0;

    // Split input into parts for pattern, text, rewrite, and maxlen
    size_t quarter = Size / 4;
    size_t pattern_size = quarter;
    size_t text_size = quarter;
    size_t rewrite_size = quarter;
    size_t remaining = Size - 3 * quarter;

    const uint8_t* pattern_data = Data;
    const uint8_t* text_data = Data + pattern_size;
    const uint8_t* rewrite_data = Data + pattern_size + text_size;
    const uint8_t* remaining_data = Data + pattern_size + text_size + rewrite_size;

    // Generate strings with reasonable length limits
    std::string pattern = generate_string(pattern_data, pattern_size, 100);
    std::string text = generate_string(text_data, text_size, 200);
    std::string rewrite = generate_string(rewrite_data, rewrite_size, 100);

    // Create RE2 object
    re2::RE2 re(pattern);
    if (!re.ok()) {
        return 0;
    }

    // 1. Test RE2::Replace
    std::string str1 = text;
    re2::RE2::Replace(&str1, re, rewrite);

    // 2. Test RE2::GlobalReplace
    std::string str2 = text;
    re2::RE2::GlobalReplace(&str2, re, rewrite);

    // 3. Test RE2::PossibleMatchRange
    std::string min_str, max_str;
    int maxlen = 100;
    if (remaining >= sizeof(int)) {
        memcpy(&maxlen, remaining_data, sizeof(int));
        maxlen = maxlen % 1000;
        if (maxlen < 0) maxlen = -maxlen;
        if (maxlen == 0) maxlen = 100;
    }
    re.PossibleMatchRange(&min_str, &max_str, maxlen);

    // 4. Test RE2::NamedCapturingGroups
    const std::map<std::string, int>& groups = re.NamedCapturingGroups();
    // Just access the map to ensure it's valid
    for (const auto& pair : groups) {
        // Do nothing, just ensure we can iterate
        (void)pair;
    }

    return 0;
}