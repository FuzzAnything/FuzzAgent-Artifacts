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
#include <cstring>
#include <string>

#include "re2/re2.h"

extern "C" int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    if (Size < 3) {
        return 0;
    }

    // Split input into three parts: pattern, text, and rewrite
    size_t pattern_size = Size / 3;
    size_t text_size = Size / 3;
    size_t rewrite_size = Size - pattern_size - text_size;

    // Ensure non-empty strings
    if (pattern_size == 0 || text_size == 0 || rewrite_size == 0) {
        return 0;
    }

    // Create strings with proper null termination
    std::string pattern(reinterpret_cast<const char*>(Data), pattern_size);
    std::string text(reinterpret_cast<const char*>(Data + pattern_size), text_size);
    std::string rewrite(reinterpret_cast<const char*>(Data + pattern_size + text_size), rewrite_size);

    // Clean the pattern string - remove null bytes that might break RE2 parsing
    pattern.erase(std::remove(pattern.begin(), pattern.end(), '\0'), pattern.end());
    if (pattern.empty()) {
        return 0;
    }

    // Create RE2 object with the pattern
    re2::RE2 re(pattern, re2::RE2::Quiet);
    if (!re.ok()) {
        return 0;
    }

    // Clean text and rewrite strings as well
    text.erase(std::remove(text.begin(), text.end(), '\0'), text.end());
    rewrite.erase(std::remove(rewrite.begin(), rewrite.end(), '\0'), rewrite.end());
    
    if (text.empty() || rewrite.empty()) {
        return 0;
    }

    // 1. Test Extract
    std::string extract_out;
    bool extract_result = false;
    try {
        extract_result = re2::RE2::Extract(text, re, rewrite, &extract_out);
    } catch (...) {
        // Catch any exceptions to prevent crash
    }

    // 2. Test Replace
    std::string replace_str = text;
    bool replace_result = false;
    try {
        replace_result = re2::RE2::Replace(&replace_str, re, rewrite);
    } catch (...) {
        // Catch any exceptions to prevent crash
    }

    // 3. Test GlobalReplace
    std::string global_replace_str = text;
    int global_replace_result = 0;
    try {
        global_replace_result = re2::RE2::GlobalReplace(&global_replace_str, re, rewrite);
    } catch (...) {
        // Catch any exceptions to prevent crash
    }

    return 0;
}