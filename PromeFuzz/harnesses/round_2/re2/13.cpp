// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::RE2::ok at re2.h:307:8 in re2.h
// re2::RE2::Extract at re2.cc:542:11 in re2.h
// re2::RE2::Replace at re2.cc:450:11 in re2.h
// re2::RE2::GlobalReplace at re2.cc:472:10 in re2.h
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
#include <stdint.h>
#include <stddef.h>
#include <string>

extern "C" int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    if (Size < 3) {
        return 0;
    }

    // Split input into three parts: pattern, text, and rewrite
    size_t pattern_size = Size / 3;
    size_t text_size = Size / 3;
    size_t rewrite_size = Size - pattern_size - text_size;

    // Create null-terminated strings from the input data
    std::string pattern_str(reinterpret_cast<const char*>(Data), pattern_size);
    std::string text_str(reinterpret_cast<const char*>(Data) + pattern_size, text_size);
    std::string rewrite_str(reinterpret_cast<const char*>(Data) + pattern_size + text_size, rewrite_size);

    // Create RE2 object with default options
    re2::RE2 re(pattern_str);
    if (!re.ok()) {
        return 0;
    }

    // Test RE2::Extract
    std::string extract_out;
    static bool extract_result = re2::RE2::Extract(text_str, re, rewrite_str, &extract_out);
    (void)extract_result;

    // Test RE2::Replace
    std::string replace_str = text_str;
    static bool replace_result = re2::RE2::Replace(&replace_str, re, rewrite_str);
    (void)replace_result;

    // Test RE2::GlobalReplace
    std::string global_replace_str = text_str;
    static int global_replace_count = re2::RE2::GlobalReplace(&global_replace_str, re, rewrite_str);
    (void)global_replace_count;

    return 0;
}