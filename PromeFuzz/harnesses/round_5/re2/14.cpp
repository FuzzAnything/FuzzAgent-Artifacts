// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::RE2::Options::set_max_mem at re2.h:697:10 in re2.h
// re2::RE2::Set::Size at set.cc:58:15 in set.h
// re2::RE2::Set::Add at set.cc:64:15 in set.h
// re2::RE2::Set::Size at set.cc:58:15 in set.h
// re2::RE2::Set::Add at set.cc:64:15 in set.h
// re2::RE2::Set::Size at set.cc:58:15 in set.h
// re2::RE2::Set::Add at set.cc:64:15 in set.h
// re2::RE2::Set::Size at set.cc:58:15 in set.h
// re2::RE2::Set::Compile at set.cc:103:16 in set.h
// re2::RE2::Set::Size at set.cc:58:15 in set.h
// re2::RE2::Set::Match at set.cc:133:16 in set.h
// re2::RE2::Set::Match at set.cc:133:16 in set.h
// re2::RE2::Set::Match at set.cc:133:16 in set.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include "re2/set.h"
#include "re2/re2.h"

static std::vector<absl::string_view> splitInput(const uint8_t* data, size_t size) {
    std::vector<absl::string_view> parts;
    const char* start = reinterpret_cast<const char*>(data);
    const char* end = start + size;
    
    while (start < end) {
        size_t len = 0;
        while (start + len < end && start[len] != '\0') {
            ++len;
        }
        if (len > 0) {
            parts.push_back(absl::string_view(start, len));
        }
        start += len + 1;
    }
    return parts;
}

extern "C" int LLVMFuzzerTestOneInput_14(const uint8_t* data, size_t size) {
    if (size < 4) {
        return 0;
    }
    
    std::vector<absl::string_view> parts = splitInput(data, size);
    if (parts.size() < 4) {
        return 0;
    }
    
    absl::string_view pattern1 = parts[0];
    absl::string_view pattern2 = parts[1];
    absl::string_view pattern3 = parts[2];
    absl::string_view text = parts[3];
    
    re2::RE2::Options options;
    options.set_max_mem(64 * 1024 * 1024);
    re2::RE2::Set set(options, re2::RE2::ANCHOR_BOTH);
    
    std::string error;
    
    set.Size();
    int idx1 = set.Add(pattern1, &error);
    set.Size();
    int idx2 = set.Add(pattern2, &error);
    set.Size();
    int idx3 = set.Add(pattern3, &error);
    set.Size();
    
    bool compile_called = false;
    if (idx1 >= 0 || idx2 >= 0 || idx3 >= 0) {
        compile_called = true;
        set.Compile();
    }
    
    set.Size();
    
    if (compile_called) {
        std::vector<int> matches1;
        set.Match(text, &matches1);
        
        std::vector<int> matches2;
        set.Match(text, &matches2);
        
        set.Match(text, nullptr);
    }
    
    return 0;
}