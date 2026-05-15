// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::RE2::Rewrite at re2.cc:1030:11 in re2.h
// re2::RE2::NamedCapturingGroups at re2.cc:395:40 in re2.h
// re2::RE2::GlobalReplace at re2.cc:472:10 in re2.h
// re2::RE2::Replace at re2.cc:450:11 in re2.h
// re2::RE2::CheckRewriteString at re2.cc:971:11 in re2.h
// re2::RE2::Options::set_encoding at re2.h:700:10 in re2.h
// re2::RE2::ok at re2.h:307:8 in re2.h
// re2::RE2::Extract at re2.cc:542:11 in re2.h
// re2::RE2::GlobalReplace at re2.cc:472:10 in re2.h
// re2::RE2::Extract at re2.cc:542:11 in re2.h
// re2::RE2::GlobalReplace at re2.cc:472:10 in re2.h
// re2::RE2::CheckRewriteString at re2.cc:971:11 in re2.h
// re2::RE2::ok at re2.h:307:8 in re2.h
// re2::RE2::CheckRewriteString at re2.cc:971:11 in re2.h
// re2::RE2::Extract at re2.cc:542:11 in re2.h
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
#include <cstddef>
#include <string>
#include <vector>
#include <map>

#include "re2/re2.h"

static std::string generate_string(const uint8_t* data, size_t size, size_t max_len) {
    if (size == 0) return "";
    size_t len = data[0] % max_len + 1;
    if (len > size - 1) len = size - 1;
    return std::string(reinterpret_cast<const char*>(data + 1), len);
}

static std::vector<absl::string_view> generate_string_views(const uint8_t* data, size_t size, int max_vec_len) {
    std::vector<absl::string_view> vec;
    if (size < 2) return vec;
    
    int vec_len = data[0] % max_vec_len;
    if (vec_len == 0) return vec;
    
    size_t offset = 1;
    for (int i = 0; i < vec_len && offset < size; i++) {
        size_t str_len = data[offset] % 16;
        offset++;
        if (offset + str_len > size) str_len = size - offset;
        if (str_len > 0) {
            vec.push_back(absl::string_view(reinterpret_cast<const char*>(data + offset), str_len));
            offset += str_len;
        } else {
            vec.push_back(absl::string_view("", 0));
        }
    }
    return vec;
}

extern "C" int LLVMFuzzerTestOneInput_19(const uint8_t* data, size_t size) {
    if (size < 3) return 0;
    
    // Generate pattern, text, and rewrite strings from fuzzer input
    size_t offset = 0;
    
    // Pattern
    size_t pattern_len = data[offset] % 32 + 1;
    offset++;
    if (offset + pattern_len > size) pattern_len = size - offset;
    std::string pattern(reinterpret_cast<const char*>(data + offset), pattern_len);
    offset += pattern_len;
    
    // Text
    size_t text_len = 0;
    if (offset < size) {
        text_len = data[offset] % 64 + 1;
        offset++;
        if (offset + text_len > size) text_len = size - offset;
    }
    std::string text(reinterpret_cast<const char*>(data + offset), text_len);
    offset += text_len;
    
    // Rewrite
    size_t rewrite_len = 0;
    if (offset < size) {
        rewrite_len = data[offset] % 32 + 1;
        offset++;
        if (offset + rewrite_len > size) rewrite_len = size - offset;
    }
    std::string rewrite(reinterpret_cast<const char*>(data + offset), rewrite_len);
    
    // Create RE2 object with default options
    re2::RE2::Options options;
    re2::RE2 re(pattern, options);
    
    if (!re.ok()) {
        // If pattern is invalid, still test some functions that don't require valid regex
        std::string error;
        re.CheckRewriteString(rewrite, &error);
        return 0;
    }
    
    // Test 1: RE2::Extract
    {
        std::string out;
        re2::RE2::Extract(text, re, rewrite, &out);
    }
    
    // Test 2: RE2::Rewrite
    {
        std::string out;
        std::vector<absl::string_view> vec = generate_string_views(data, size, 10);
        if (!vec.empty()) {
            re.Rewrite(&out, rewrite, vec.data(), vec.size());
        }
    }
    
    // Test 3: RE2::NamedCapturingGroups
    {
        const std::map<std::string, int>& groups = re.NamedCapturingGroups();
        // Just access the map to ensure it works
        for (const auto& pair : groups) {
            // Do nothing, just ensure access doesn't crash
            (void)pair;
        }
    }
    
    // Test 4: RE2::GlobalReplace
    {
        std::string str = text;
        re2::RE2::GlobalReplace(&str, re, rewrite);
    }
    
    // Test 5: RE2::Replace
    {
        std::string str = text;
        re2::RE2::Replace(&str, re, rewrite);
    }
    
    // Test 6: RE2::CheckRewriteString
    {
        std::string error;
        re.CheckRewriteString(rewrite, &error);
    }
    
    // Additional tests with different options
    {
        // Test with Latin1 encoding
        re2::RE2::Options latin1_options;
        latin1_options.set_encoding(re2::RE2::Options::EncodingLatin1);
        re2::RE2 re_latin1(pattern, latin1_options);
        
        if (re_latin1.ok()) {
            std::string out;
            re2::RE2::Extract(text, re_latin1, rewrite, &out);
            
            std::string str = text;
            re2::RE2::GlobalReplace(&str, re_latin1, rewrite);
        }
    }
    
    // Test with empty strings
    {
        std::string empty;
        re2::RE2::Extract(empty, re, empty, &empty);
        
        std::string str;
        re2::RE2::GlobalReplace(&str, re, empty);
        
        std::string error;
        re.CheckRewriteString(empty, &error);
    }
    
    return 0;
}