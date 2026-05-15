// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::RE2::ok at re2.h:307:8 in re2.h
// re2::RE2::GlobalReplace at re2.cc:472:10 in re2.h
// re2::RE2::FUZZING_ONLY_set_maximum_global_replace_count at re2.cc:48:11 in re2.h
// re2::RE2::QuoteMeta at re2.cc:559:18 in re2.h
// re2::RE2::ok at re2.h:307:8 in re2.h
// re2::RE2::Rewrite at re2.cc:1030:11 in re2.h
// re2::RE2::ok at re2.h:307:8 in re2.h
// re2::RE2::NamedCapturingGroups at re2.cc:395:40 in re2.h
// re2::RE2::ok at re2.h:307:8 in re2.h
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
#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include <map>

static std::string generateStringFromFuzz(const uint8_t* data, size_t size, size_t max_len = 256) {
    if (size == 0) return "";
    size_t len = size < max_len ? size : max_len;
    return std::string(reinterpret_cast<const char*>(data), len);
}

static std::vector<absl::string_view> generateStringViews(const uint8_t* data, size_t size, int max_vec = 5) {
    std::vector<absl::string_view> vec;
    if (size == 0) return vec;
    
    const uint8_t* ptr = data;
    size_t remaining = size;
    int count = 0;
    
    while (remaining > 0 && count < max_vec) {
        size_t chunk_size = (remaining > 32) ? 32 : remaining;
        vec.push_back(absl::string_view(reinterpret_cast<const char*>(ptr), chunk_size));
        ptr += chunk_size;
        remaining -= chunk_size;
        count++;
    }
    
    return vec;
}

extern "C" int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;
    
    // Set safety limit for GlobalReplace first
    re2::RE2::FUZZING_ONLY_set_maximum_global_replace_count(50);
    
    // Generate pattern and input strings from fuzz data with smaller limits
    size_t pattern_len = Size / 4;
    if (pattern_len > 128) pattern_len = 128;
    
    size_t text_len = Size / 4;
    if (text_len > 256) text_len = 256;
    
    size_t rewrite_len = Size / 4;
    if (rewrite_len > 128) rewrite_len = 128;
    
    std::string pattern = generateStringFromFuzz(Data, pattern_len, 128);
    std::string text = generateStringFromFuzz(Data + pattern_len, text_len, 256);
    std::string rewrite = generateStringFromFuzz(Data + pattern_len + text_len, rewrite_len, 128);
    
    // Test 5: RE2::QuoteMeta (safe to call without RE2 object)
    {
        std::string quoted = re2::RE2::QuoteMeta(text);
        (void)quoted;
    }
    
    // Test 1: RE2::Rewrite - create RE2 object but only use it for Rewrite
    {
        re2::RE2 re1(pattern);
        if (re1.ok()) {
            std::string out;
            std::vector<absl::string_view> vec = generateStringViews(Data, Size, 3);
            if (!vec.empty()) {
                re1.Rewrite(&out, rewrite, vec.data(), static_cast<int>(vec.size()));
            }
        }
    }
    
    // Test 3: RE2::NamedCapturingGroups - create fresh RE2 object
    {
        re2::RE2 re3(pattern);
        if (re3.ok()) {
            const std::map<std::string, int>& groups = re3.NamedCapturingGroups();
            (void)groups;
        }
    }
    
    // Test 4: RE2::Replace - create fresh RE2 object
    {
        re2::RE2 re4(pattern);
        if (re4.ok()) {
            std::string str_copy = text;
            re2::RE2::Replace(&str_copy, re4, rewrite);
        }
    }
    
    // Test 2: RE2::GlobalReplace - create fresh RE2 object
    {
        re2::RE2 re2(pattern);
        if (re2.ok()) {
            std::string str_copy = text;
            re2::RE2::GlobalReplace(&str_copy, re2, rewrite);
        }
    }
    
    return 0;
}