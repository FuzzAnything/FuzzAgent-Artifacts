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
#include <cstdint>
#include <cstring>
#include <string>

#include "re2/re2.h"

static std::string generate_string(const uint8_t* data, size_t size, size_t offset, size_t length) {
    if (offset >= size) return "";
    size_t actual_length = (offset + length > size) ? size - offset : length;
    return std::string(reinterpret_cast<const char*>(data + offset), actual_length);
}

extern "C" int LLVMFuzzerTestOneInput_13(const uint8_t* Data, size_t Size) {
    if (Size < 4) return 0;

    // Split input into three parts for pattern, rewrite, and text
    size_t pattern_size = Size / 3;
    size_t rewrite_size = Size / 3;
    size_t text_size = Size - pattern_size - rewrite_size;

    std::string pattern = generate_string(Data, Size, 0, pattern_size);
    std::string rewrite = generate_string(Data, Size, pattern_size, rewrite_size);
    std::string text = generate_string(Data, Size, pattern_size + rewrite_size, text_size);

    // Check if pattern is empty - RE2 doesn't handle empty patterns well
    if (pattern.empty()) {
        pattern = "a";  // Use a simple default pattern
    }

    // Initialize RE2 object with pattern
    re2::RE2 re(pattern);
    if (!re.ok()) {
        return 0;
    }

    // 1. Test RE2::Extract
    std::string extract_out;
    // Ensure text is not empty for Extract
    if (!text.empty()) {
        bool extract_result = re2::RE2::Extract(text, re, rewrite, &extract_out);
    }

    // 2. Test RE2::Replace
    std::string replace_str = text;
    // Ensure replace_str is not empty for Replace
    if (!replace_str.empty()) {
        bool replace_result = re2::RE2::Replace(&replace_str, re, rewrite);
    }

    // 3. Test RE2::GlobalReplace
    std::string global_replace_str = text;
    // Ensure global_replace_str is not empty for GlobalReplace
    if (!global_replace_str.empty()) {
        int global_replace_count = re2::RE2::GlobalReplace(&global_replace_str, re, rewrite);
    }

    return 0;
}