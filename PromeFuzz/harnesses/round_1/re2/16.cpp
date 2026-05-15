// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::RE2::ok at re2.h:307:8 in re2.h
// re2::RE2::Extract at re2.cc:542:11 in re2.h
// re2::RE2::CheckRewriteString at re2.cc:971:11 in re2.h
// re2::RE2::MaxSubmatch at re2.cc:1011:10 in re2.h
// re2::RE2::Replace at re2.cc:450:11 in re2.h
// re2::RE2::GlobalReplace at re2.cc:472:10 in re2.h
// re2::RE2::Rewrite at re2.cc:1030:11 in re2.h
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
#include <vector>

extern "C" int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    if (Size < 3) return 0;

    // Split input into three parts: pattern, text, rewrite
    size_t pattern_len = Size / 3;
    size_t text_len = Size / 3;
    size_t rewrite_len = Size - pattern_len - text_len;

    absl::string_view pattern(reinterpret_cast<const char*>(Data), pattern_len);
    absl::string_view text(reinterpret_cast<const char*>(Data + pattern_len), text_len);
    absl::string_view rewrite(reinterpret_cast<const char*>(Data + pattern_len + text_len), rewrite_len);

    // Create RE2 object with the pattern
    re2::RE2 re(pattern);
    if (!re.ok()) return 0;

    // Test Extract function
    static std::string extract_out;
    extract_out.clear();
    re2::RE2::Extract(text, re, rewrite, &extract_out);

    // Test CheckRewriteString function
    static std::string error_msg;
    error_msg.clear();
    re.CheckRewriteString(rewrite, &error_msg);

    // Test MaxSubmatch function
    int max_submatch = re2::RE2::MaxSubmatch(rewrite);

    // Test Replace function
    static std::string replace_str(text);
    re2::RE2::Replace(&replace_str, re, rewrite);

    // Test GlobalReplace function
    static std::string global_replace_str(text);
    re2::RE2::GlobalReplace(&global_replace_str, re, rewrite);

    // Test Rewrite function - create dummy capture groups
    static std::string rewrite_out;
    rewrite_out.clear();
    
    // Create a vector of captured groups for testing Rewrite
    // Use up to 10 capture groups, with dummy data from input
    std::vector<absl::string_view> vec;
    int veclen = std::min(10, static_cast<int>(Size / 10));
    if (veclen > 0) {
        vec.reserve(veclen);
        for (int i = 0; i < veclen; ++i) {
            size_t offset = (i * 10) % Size;
            size_t len = std::min(static_cast<size_t>(5), Size - offset);
            vec.push_back(absl::string_view(reinterpret_cast<const char*>(Data + offset), len));
        }
        re.Rewrite(&rewrite_out, rewrite, vec.data(), vec.size());
    }

    return 0;
}