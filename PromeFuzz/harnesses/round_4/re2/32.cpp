// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::FilteredRE2::Compile at filtered_re2.cc:70:19 in filtered_re2.h
// re2::FilteredRE2::AllMatches at filtered_re2.cc:112:19 in filtered_re2.h
// re2::RE2::Set::Add at set.cc:64:15 in set.h
// re2::RE2::Set::Size at set.cc:58:15 in set.h
// re2::RE2::Set::Compile at set.cc:103:16 in set.h
// re2::RE2::Set::Match at set.cc:133:16 in set.h
// re2::RE2::Set::Match at set.cc:137:16 in set.h
// re2::RE2::Set::Size at set.cc:58:15 in set.h
// re2::FilteredRE2::Add at filtered_re2.cc:51:29 in filtered_re2.h
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
#include <vector>
#include <string>

#include "re2/set.h"
#include "re2/filtered_re2.h"

static void SplitInput(const uint8_t *Data, size_t Size, 
                       std::vector<absl::string_view> &patterns,
                       absl::string_view &text) {
    if (Size == 0) {
        text = absl::string_view();
        return;
    }
    
    const char* data = reinterpret_cast<const char*>(Data);
    size_t pos = 0;
    
    while (pos < Size) {
        size_t pattern_start = pos;
        while (pos < Size && data[pos] != '\0') {
            ++pos;
        }
        
        if (pos - pattern_start > 0) {
            patterns.push_back(absl::string_view(data + pattern_start, pos - pattern_start));
        }
        
        if (pos < Size && data[pos] == '\0') {
            ++pos;
        }
    }
    
    if (!patterns.empty()) {
        text = patterns.back();
        patterns.pop_back();
    } else {
        text = absl::string_view(data, Size);
    }
}

extern "C" int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    std::vector<absl::string_view> patterns;
    absl::string_view text;
    
    SplitInput(Data, Size, patterns, text);
    
    // Test RE2::Set
    if (!patterns.empty()) {
        re2::RE2::Options options;
        re2::RE2::Set set(options, re2::RE2::Anchor::UNANCHORED);
        
        std::vector<std::string> errors;
        for (const auto &pattern : patterns) {
            std::string error;
            int index = set.Add(pattern, &error);
            if (!error.empty()) {
                errors.push_back(error);
            }
        }
        
        if (set.Size() > 0) {
            if (set.Compile()) {
                std::vector<int> matches;
                set.Match(text, &matches);
                
                re2::RE2::Set::ErrorInfo error_info;
                set.Match(text, &matches, &error_info);
                
                int size = set.Size();
            }
        }
    }
    
    // Test FilteredRE2
    if (!patterns.empty()) {
        re2::FilteredRE2 filtered_re2;
        std::vector<int> atom_ids;
        
        for (const auto &pattern : patterns) {
            int id;
            if (filtered_re2.Add(pattern, re2::RE2::Options(), &id) >= 0) {
                atom_ids.push_back(id);
            }
        }
        
        if (!atom_ids.empty()) {
            std::vector<std::string> atoms;
            filtered_re2.Compile(&atoms);
            
            std::vector<int> matching_regexps;
            filtered_re2.AllMatches(text, atom_ids, &matching_regexps);
        }
    }
    
    return 0;
}