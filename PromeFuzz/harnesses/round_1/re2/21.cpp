// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::RE2::CheckRewriteString at re2.cc:971:11 in re2.h
// re2::RE2::Extract at re2.cc:542:11 in re2.h
// re2::RE2::Replace at re2.cc:450:11 in re2.h
// re2::RE2::GlobalReplace at re2.cc:472:10 in re2.h
// re2::RE2::Rewrite at re2.cc:1030:11 in re2.h
// re2::RE2::Options::set_max_mem at re2.h:697:10 in re2.h
// re2::RE2::Options::set_case_sensitive at re2.h:724:10 in re2.h
// re2::RE2::Options::set_posix_syntax at re2.h:703:10 in re2.h
// re2::RE2::Options::set_longest_match at re2.h:706:10 in re2.h
// re2::RE2::Options::set_log_errors at re2.h:709:10 in re2.h
// re2::RE2::Options::set_encoding at re2.h:700:10 in re2.h
// re2::RE2::ok at re2.h:307:8 in re2.h
// re2::RE2::Extract at re2.cc:542:11 in re2.h
// re2::RE2::Replace at re2.cc:450:11 in re2.h
// re2::RE2::GlobalReplace at re2.cc:472:10 in re2.h
// re2::RE2::MaxSubmatch at re2.cc:1011:10 in re2.h
// re2::RE2::ok at re2.h:307:8 in re2.h
// re2::RE2::ok at re2.h:307:8 in re2.h
// re2::RE2::ok at re2.h:307:8 in re2.h
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
#include <mutex>

static std::mutex g_mutex;

static std::string createStringFromData(const uint8_t* data, size_t size, size_t max_len) {
    if (size == 0) return "";
    size_t len = size < max_len ? size : max_len;
    for (size_t i = 0; i < len; ++i) {
        if (data[i] == 0) {
            len = i;
            break;
        }
    }
    if (len == 0) return "";
    return std::string(reinterpret_cast<const char*>(data), len);
}

static void splitDataIntoParts(const uint8_t* data, size_t size, 
                               std::vector<std::string>& parts, int num_parts) {
    if (size == 0 || num_parts <= 0) return;
    
    size_t part_size = size / num_parts;
    for (int i = 0; i < num_parts; ++i) {
        size_t start = i * part_size;
        size_t end = (i == num_parts - 1) ? size : start + part_size;
        if (start < end) {
            size_t actual_len = end - start;
            for (size_t j = start; j < end; ++j) {
                if (data[j] == 0) {
                    actual_len = j - start;
                    break;
                }
            }
            if (actual_len > 0) {
                parts.push_back(std::string(reinterpret_cast<const char*>(data + start), actual_len));
            } else {
                parts.push_back("");
            }
        } else {
            parts.push_back("");
        }
    }
}

extern "C" int LLVMFuzzerTestOneInput_21(const uint8_t* Data, size_t Size) {
    if (Size < 4) return 0;
    
    if (Size > 4096) {
        Size = 4096;
    }
    
    std::vector<std::string> parts;
    splitDataIntoParts(Data, Size, parts, 4);
    
    if (parts.size() < 4) return 0;
    
    const std::string& pattern = parts[0];
    const std::string& text = parts[1];
    const std::string& rewrite = parts[2];
    const std::string& extra = parts[3];
    
    int max_submatch = re2::RE2::MaxSubmatch(rewrite);
    
    std::string safe_pattern = pattern;
    if (pattern.empty() || pattern.length() > 100) {
        safe_pattern = "a";
    }
    
    {
        std::lock_guard<std::mutex> lock(g_mutex);
        
        re2::RE2* re = nullptr;
        re2::RE2 re1(safe_pattern.c_str());
        if (re1.ok()) {
            re = &re1;
        } else {
            static re2::RE2 re_default(".");
            if (re_default.ok()) {
                re = &re_default;
            } else {
                return 0;
            }
        }
        
        if (!re || !re->ok()) {
            return 0;
        }
        
        std::string error;
        bool check_ok = re->CheckRewriteString(rewrite, &error);
        
        std::string out1;
        bool extract_ok = false;
        if (!text.empty()) {
            try {
                extract_ok = re2::RE2::Extract(text, *re, rewrite, &out1);
            } catch (...) {
            }
        }
        
        std::string str1 = text;
        bool replace_ok = false;
        if (!str1.empty()) {
            try {
                replace_ok = re2::RE2::Replace(&str1, *re, rewrite);
            } catch (...) {
            }
        }
        
        std::string str2 = text;
        int global_replace_count = 0;
        if (!str2.empty()) {
            try {
                global_replace_count = re2::RE2::GlobalReplace(&str2, *re, rewrite);
            } catch (...) {
            }
        }
        
        const int vec_len = 10;
        absl::string_view vec[vec_len];
        
        size_t extra_len = extra.length();
        if (extra_len > 0) {
            size_t chunk_size = extra_len / vec_len;
            if (chunk_size == 0) chunk_size = 1;
            
            for (int i = 0; i < vec_len; ++i) {
                size_t start = i * chunk_size;
                if (start < extra_len) {
                    size_t end = (i == vec_len - 1) ? extra_len : start + chunk_size;
                    if (end > extra_len) end = extra_len;
                    vec[i] = absl::string_view(extra.data() + start, end - start);
                } else {
                    vec[i] = absl::string_view("");
                }
            }
            
            std::string out2;
            bool rewrite_ok = false;
            try {
                rewrite_ok = re->Rewrite(&out2, rewrite, vec, vec_len);
            } catch (...) {
            }
        }
        
        if (Size > 10 && safe_pattern.length() < 100) {
            re2::RE2::Options options;
            options.set_max_mem(1 << 20);
            options.set_case_sensitive(Data[0] & 0x01);
            options.set_posix_syntax(Data[1] & 0x01);
            options.set_longest_match(Data[2] & 0x01);
            options.set_log_errors(true);
            options.set_encoding(re2::RE2::Options::EncodingLatin1);
            
            re2::RE2 re2(safe_pattern.c_str(), options);
            if (re2.ok()) {
                std::string out3;
                if (!text.empty()) {
                    try {
                        re2::RE2::Extract(text, re2, rewrite, &out3);
                    } catch (...) {
                    }
                }
                
                std::string str3 = text;
                if (!str3.empty()) {
                    try {
                        re2::RE2::Replace(&str3, re2, rewrite);
                    } catch (...) {
                    }
                }
                
                std::string str4 = text;
                if (!str4.empty()) {
                    try {
                        re2::RE2::GlobalReplace(&str4, re2, rewrite);
                    } catch (...) {
                    }
                }
            }
        }
    }
    
    return 0;
}