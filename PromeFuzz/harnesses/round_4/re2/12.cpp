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
#include <cstdint>
#include <cstring>
#include <string>

extern "C" int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    if (Size < 3) return 0;

    // Split input into three parts: pattern, rewrite, and text
    size_t pattern_len = Data[0] % (Size - 2) + 1;
    if (pattern_len >= Size) return 0;
    
    size_t rewrite_len = Data[1] % (Size - pattern_len - 1) + 1;
    if (pattern_len + rewrite_len >= Size) return 0;
    
    size_t text_len = Size - pattern_len - rewrite_len;
    
    std::string pattern(reinterpret_cast<const char*>(Data), pattern_len);
    std::string rewrite(reinterpret_cast<const char*>(Data + pattern_len), rewrite_len);
    std::string text(reinterpret_cast<const char*>(Data + pattern_len + rewrite_len), text_len);
    
    // Create RE2 object with default options
    re2::RE2 re(pattern);
    if (!re.ok()) return 0;
    
    // 1. Test Extract
    std::string extract_out;
    static bool extract_result = re2::RE2::Extract(text, re, rewrite, &extract_out);
    
    // 2. Test Replace
    std::string replace_str = text;
    static bool replace_result = re2::RE2::Replace(&replace_str, re, rewrite);
    
    // 3. Test GlobalReplace
    std::string global_replace_str = text;
    static int global_replace_count = re2::RE2::GlobalReplace(&global_replace_str, re, rewrite);
    
    return 0;
}