// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::RE2::ok at re2.h:307:8 in re2.h
// re2::RE2::GlobalReplace at re2.cc:472:10 in re2.h
// re2::RE2::CheckRewriteString at re2.cc:971:11 in re2.h
// re2::RE2::MaxSubmatch at re2.cc:1011:10 in re2.h
// re2::RE2::Rewrite at re2.cc:1030:11 in re2.h
// re2::RE2::Extract at re2.cc:542:11 in re2.h
// re2::RE2::Replace at re2.cc:450:11 in re2.h
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
#include <string>
#include <vector>

extern "C" int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    // Split input into three parts: pattern, rewrite, and text
    size_t pattern_len = Size / 3;
    size_t rewrite_len = Size / 3;
    size_t text_len = Size - pattern_len - rewrite_len;

    std::string pattern(reinterpret_cast<const char*>(Data), pattern_len);
    std::string rewrite(reinterpret_cast<const char*>(Data + pattern_len), rewrite_len);
    std::string text(reinterpret_cast<const char*>(Data + pattern_len + rewrite_len), text_len);

    // Try to create RE2 object - this might fail for invalid patterns
    re2::RE2 re(pattern);
    if (!re.ok()) {
        return 0; // Skip invalid patterns
    }

    // Test GlobalReplace
    {
        std::string str_copy = text;
        re2::RE2::GlobalReplace(&str_copy, re, rewrite);
    }

    // Test CheckRewriteString
    {
        std::string error;
        re.CheckRewriteString(rewrite, &error);
    }

    // Test MaxSubmatch
    {
        re2::RE2::MaxSubmatch(rewrite);
    }

    // Test Rewrite
    {
        std::string out;
        // Create a simple vec with some dummy matches
        std::vector<absl::string_view> vec;
        if (text_len > 0) {
            vec.push_back(absl::string_view(text.c_str(), std::min(text_len, (size_t)10)));
            if (text_len > 10) {
                vec.push_back(absl::string_view(text.c_str() + 10, std::min(text_len - 10, (size_t)10)));
            }
        }
        if (!vec.empty()) {
            re.Rewrite(&out, rewrite, vec.data(), vec.size());
        }
    }

    // Test Extract
    {
        std::string out;
        re2::RE2::Extract(text, re, rewrite, &out);
    }

    // Test Replace
    {
        std::string str_copy = text;
        re2::RE2::Replace(&str_copy, re, rewrite);
    }

    return 0;
}