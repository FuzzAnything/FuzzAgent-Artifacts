// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::RE2::CheckRewriteString at re2.cc:971:11 in re2.h
// re2::RE2::FUZZING_ONLY_set_maximum_global_replace_count at re2.cc:48:11 in re2.h
// re2::RE2::GlobalReplace at re2.cc:472:10 in re2.h
// re2::RE2::FUZZING_ONLY_set_maximum_global_replace_count at re2.cc:48:11 in re2.h
// re2::RE2::Replace at re2.cc:450:11 in re2.h
// re2::RE2::Options::set_max_mem at re2.h:697:10 in re2.h
// re2::RE2::Options::set_log_errors at re2.h:709:10 in re2.h
// re2::RE2::ok at re2.h:307:8 in re2.h
// re2::RE2::Extract at re2.cc:542:11 in re2.h
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
#include <cstring>
#include <string>
#include <vector>

extern "C" int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    // Split input into parts for pattern, text, and rewrite
    size_t pattern_len = Data[0] % (Size - 3);
    if (pattern_len == 0) pattern_len = 1; // Ensure non-empty pattern
    
    size_t text_len = Data[1] % (Size - pattern_len - 2);
    size_t rewrite_len = Data[2] % (Size - pattern_len - text_len - 1);
    
    // Ensure we don't overflow and have reasonable sizes
    if (pattern_len + text_len + rewrite_len > Size - 3) {
        return 0;
    }
    
    // Limit sizes to prevent excessive memory usage
    if (pattern_len > 1024 || text_len > 4096 || rewrite_len > 1024) {
        return 0;
    }
    
    const char* data_ptr = reinterpret_cast<const char*>(Data + 3);
    
    std::string pattern(data_ptr, pattern_len);
    std::string text(data_ptr + pattern_len, text_len);
    std::string rewrite(data_ptr + pattern_len + text_len, rewrite_len);
    
    // Ensure strings don't contain null bytes in the middle
    for (size_t i = 0; i < pattern_len; i++) {
        if (pattern[i] == '\0') {
            pattern[i] = 'a';
        }
    }
    for (size_t i = 0; i < text_len; i++) {
        if (text[i] == '\0') {
            text[i] = 'a';
        }
    }
    for (size_t i = 0; i < rewrite_len; i++) {
        if (rewrite[i] == '\0') {
            rewrite[i] = 'a';
        }
    }
    
    // Create RE2 object with options to limit memory usage
    re2::RE2::Options options;
    options.set_max_mem(1 << 20); // Limit to 1MB
    options.set_log_errors(false); // Don't log errors
    
    re2::RE2 re(pattern, options);
    if (!re.ok()) {
        return 0;
    }
    
    // Test Extract - ensure out string is separate from text
    {
        std::string out;
        // Ensure text and out don't alias
        if (&text != &out) {
            re2::RE2::Extract(text, re, rewrite, &out);
        }
    }
    
    // Test Rewrite
    {
        std::string out;
        std::vector<absl::string_view> vec;
        
        // Create some dummy capture groups
        if (text_len > 0) {
            size_t group1_len = std::min(text_len, (size_t)10);
            vec.push_back(absl::string_view(text.data(), group1_len));
            
            if (text_len > 10) {
                size_t group2_len = std::min(text_len - 10, (size_t)10);
                vec.push_back(absl::string_view(text.data() + 10, group2_len));
            }
        }
        
        if (!vec.empty()) {
            re.Rewrite(&out, rewrite, vec.data(), static_cast<int>(vec.size()));
        }
    }
    
    // Test CheckRewriteString
    {
        std::string error;
        re.CheckRewriteString(rewrite, &error);
    }
    
    // Test GlobalReplace with safety limit
    {
        // Set a safe limit for fuzzing
        re2::RE2::FUZZING_ONLY_set_maximum_global_replace_count(10);
        
        std::string str = text;
        re2::RE2::GlobalReplace(&str, re, rewrite);
        
        // Reset the limit
        re2::RE2::FUZZING_ONLY_set_maximum_global_replace_count(-1);
    }
    
    // Test Replace
    {
        std::string str = text;
        re2::RE2::Replace(&str, re, rewrite);
    }
    
    return 0;
}