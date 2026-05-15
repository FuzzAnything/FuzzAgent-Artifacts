// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::RE2::Options::set_log_errors at re2.h:709:10 in re2.h
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
#include <cstdint>
#include <cstddef>
#include <string>

extern "C" int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    if (Size < 3) {
        return 0;
    }

    // Split input into three parts: pattern, text, and rewrite
    size_t pattern_len = Data[0] % (Size - 2) + 1;
    size_t text_len = Data[1] % (Size - pattern_len - 1) + 1;
    size_t rewrite_len = Size - pattern_len - text_len - 2;

    const char* pattern_ptr = reinterpret_cast<const char*>(Data + 2);
    const char* text_ptr = reinterpret_cast<const char*>(Data + 2 + pattern_len);
    const char* rewrite_ptr = reinterpret_cast<const char*>(Data + 2 + pattern_len + text_len);

    std::string pattern(pattern_ptr, pattern_len);
    std::string text(text_ptr, text_len);
    std::string rewrite(rewrite_ptr, rewrite_len);

    // Create RE2 object with default options
    re2::RE2::Options options;
    options.set_log_errors(false);
    re2::RE2 re(pattern, options);

    if (!re.ok()) {
        return 0;
    }

    // Test Extract
    std::string out;
    static bool extract_result = re2::RE2::Extract(text, re, rewrite, &out);

    // Test Replace
    std::string replace_str = text;
    static bool replace_result = re2::RE2::Replace(&replace_str, re, rewrite);

    // Test GlobalReplace
    std::string global_replace_str = text;
    static int global_replace_result = re2::RE2::GlobalReplace(&global_replace_str, re, rewrite);

    return 0;
}