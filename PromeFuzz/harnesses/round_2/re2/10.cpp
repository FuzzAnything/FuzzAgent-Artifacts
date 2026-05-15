// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::RE2::Options::set_max_mem at re2.h:697:10 in re2.h
// re2::RE2::Options::set_encoding at re2.h:700:10 in re2.h
// re2::RE2::Options::set_log_errors at re2.h:709:10 in re2.h
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
#include "re2/re2.h"
#include <cstdint>
#include <string>
#include <map>
#include <algorithm>

extern "C" int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    // Split input into parts for pattern, text, and rewrite
    // Ensure we have at least 1 byte for each part
    size_t pattern_len = std::min(Size / 4, static_cast<size_t>(50));
    size_t text_len = std::min(Size / 4, static_cast<size_t>(50));
    size_t rewrite_len = std::min(Size / 4, static_cast<size_t>(50));
    
    // Adjust indices to avoid out-of-bounds
    if (pattern_len == 0 || text_len == 0 || rewrite_len == 0) {
        return 0;
    }

    // Ensure we don't read beyond Data
    if (pattern_len + text_len + rewrite_len > Size) {
        return 0;
    }

    std::string pattern(reinterpret_cast<const char*>(Data), pattern_len);
    std::string text(reinterpret_cast<const char*>(Data + pattern_len), text_len);
    std::string rewrite(reinterpret_cast<const char*>(Data + pattern_len + text_len), rewrite_len);

    // Sanitize inputs to avoid problematic patterns
    // Remove null bytes from pattern to avoid issues
    pattern.erase(std::remove(pattern.begin(), pattern.end(), '\0'), pattern.end());
    if (pattern.empty()) {
        pattern = "a";  // Default safe pattern
    }

    // Create RE2 object with safer options
    re2::RE2::Options options;
    options.set_max_mem(32 * 1024);  // Limit memory to 32KB
    options.set_encoding(re2::RE2::Options::EncodingLatin1);
    options.set_log_errors(false);  // Disable error logging
    
    // Try to create RE2 object, return if fails
    re2::RE2 re(pattern, options);
    if (!re.ok()) {
        return 0;
    }

    // 1. Test RE2::Replace with defensive checks
    {
        std::string str1 = text;
        // Ensure string is not empty and has reasonable size
        if (!str1.empty() && str1.size() < 1000) {
            re2::RE2::Replace(&str1, re, rewrite);
        }
    }

    // 2. Test RE2::GlobalReplace with defensive checks
    {
        std::string str2 = text;
        // Ensure string is not empty and has reasonable size
        if (!str2.empty() && str2.size() < 1000) {
            re2::RE2::GlobalReplace(&str2, re, rewrite);
        }
    }

    // 3. Test RE2::PossibleMatchRange with defensive checks
    {
        std::string min, max;
        int maxlen = static_cast<int>(std::min(Size % 50, static_cast<size_t>(50))) + 1;
        if (maxlen > 0 && maxlen <= 100) {
            re.PossibleMatchRange(&min, &max, maxlen);
        }
    }

    // 4. Test RE2::NamedCapturingGroups
    {
        const std::map<std::string, int>& groups = re.NamedCapturingGroups();
        // Just access the reference, don't iterate if empty
        static_cast<void>(groups);
    }

    return 0;
}