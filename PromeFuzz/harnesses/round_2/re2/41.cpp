// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::FilteredRE2::Add at filtered_re2.cc:51:29 in filtered_re2.h
// re2::FilteredRE2::Compile at filtered_re2.cc:70:19 in filtered_re2.h
// re2::FilteredRE2::AllMatches at filtered_re2.cc:112:19 in filtered_re2.h
// re2::FilteredRE2::FirstMatch at filtered_re2.cc:98:18 in filtered_re2.h
// re2::RE2::Set::Add at set.cc:64:15 in set.h
// re2::RE2::Set::Compile at set.cc:103:16 in set.h
// re2::RE2::Set::Match at set.cc:133:16 in set.h
// re2::RE2::Set::Match at set.cc:133:16 in set.h
// re2::RE2::Set::Size at set.cc:58:15 in set.h
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
#include <vector>
#include <string>

#include "re2/set.h"
#include "re2/filtered_re2.h"

static std::vector<std::string> splitPatterns(const uint8_t *Data, size_t Size) {
    std::vector<std::string> patterns;
    if (Size == 0) return patterns;
    
    const char* data = reinterpret_cast<const char*>(Data);
    size_t start = 0;
    
    for (size_t i = 0; i < Size; ++i) {
        if (data[i] == '\0' || i == Size - 1) {
            size_t end = (data[i] == '\0') ? i : i + 1;
            if (end > start) {
                patterns.emplace_back(data + start, end - start);
            }
            start = i + 1;
            if (patterns.size() >= 10) break;
        }
    }
    
    return patterns;
}

static std::string extractText(const uint8_t *Data, size_t Size) {
    if (Size == 0) return "";
    
    const char* data = reinterpret_cast<const char*>(Data);
    size_t null_count = 0;
    size_t text_start = 0;
    
    for (size_t i = 0; i < Size; ++i) {
        if (data[i] == '\0') {
            null_count++;
            if (null_count >= 2) {
                text_start = i + 1;
                break;
            }
        }
    }
    
    if (text_start < Size) {
        return std::string(data + text_start, Size - text_start);
    }
    
    return std::string(data, Size);
}

extern "C" int LLVMFuzzerTestOneInput_41(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;
    
    std::vector<std::string> patterns = splitPatterns(Data, Size);
    std::string text = extractText(Data, Size);
    
    if (patterns.empty()) {
        patterns.push_back(".");
    }
    
    // Test RE2::Set
    {
        re2::RE2::Options options;
        re2::RE2::Set set(options, re2::RE2::UNANCHORED);
        
        for (const auto& pattern : patterns) {
            std::string error;
            set.Add(pattern, &error);
        }
        
        if (set.Compile()) {
            std::vector<int> matches;
            set.Match(text, &matches);
            
            set.Match(text, nullptr);
            
            set.Size();
        }
    }
    
    // Test FilteredRE2
    {
        re2::FilteredRE2 filtered;
        std::vector<int> atom_indices;
        
        for (const auto& pattern : patterns) {
            int id = -1;
            int error_code = filtered.Add(pattern, re2::RE2::Options(), &id);
            if (error_code == 0 && id >= 0) {
                atom_indices.push_back(id);
            }
        }
        
        std::vector<std::string> strings_to_match;
        filtered.Compile(&strings_to_match);
        
        if (!atom_indices.empty()) {
            std::vector<int> matching_regexps;
            filtered.AllMatches(text, atom_indices, &matching_regexps);
            
            filtered.FirstMatch(text, atom_indices);
        }
    }
    
    return 0;
}