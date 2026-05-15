// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::RE2::Set::Add at set.cc:64:15 in set.h
// re2::RE2::Set::Compile at set.cc:103:16 in set.h
// re2::RE2::Set::Size at set.cc:58:15 in set.h
// re2::RE2::Set::Match at set.cc:133:16 in set.h
// re2::RE2::Set::Match at set.cc:137:16 in set.h
// re2::FilteredRE2::Add at filtered_re2.cc:51:29 in filtered_re2.h
// re2::FilteredRE2::AllMatches at filtered_re2.cc:112:19 in filtered_re2.h
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

static std::string generate_pattern(const uint8_t* data, size_t size, size_t& offset) {
    if (offset >= size) {
        return "";
    }
    size_t pattern_len = data[offset] % 16 + 1;
    offset++;
    if (offset + pattern_len > size) {
        pattern_len = size - offset;
    }
    std::string pattern(reinterpret_cast<const char*>(data + offset), pattern_len);
    offset += pattern_len;
    return pattern;
}

extern "C" int LLVMFuzzerTestOneInput_28(const uint8_t* Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    size_t offset = 0;
    uint8_t num_patterns = Data[offset] % 8 + 1;
    offset++;

    re2::RE2::Set set(re2::RE2::DefaultOptions, re2::RE2::UNANCHORED);
    std::vector<std::string> patterns;
    std::vector<int> pattern_indices;

    for (int i = 0; i < num_patterns && offset < Size; ++i) {
        std::string pattern = generate_pattern(Data, Size, offset);
        if (pattern.empty()) {
            continue;
        }
        std::string error;
        int index = set.Add(pattern, &error);
        if (index >= 0) {
            patterns.push_back(pattern);
            pattern_indices.push_back(index);
        }
    }

    if (offset >= Size) {
        return 0;
    }

    bool compile_success = set.Compile();
    if (!compile_success) {
        return 0;
    }

    int set_size = set.Size();

    size_t text_len = Data[offset] % 32 + 1;
    offset++;
    if (offset + text_len > Size) {
        text_len = Size - offset;
    }
    absl::string_view text(reinterpret_cast<const char*>(Data + offset), text_len);
    offset += text_len;

    std::vector<int> match_indices;
    bool match_result = set.Match(text, &match_indices);

    re2::RE2::Set::ErrorInfo error_info;
    bool match_result_with_error = set.Match(text, &match_indices, &error_info);

    if (offset < Size) {
        re2::FilteredRE2 filtered_re2(3);
        std::vector<int> filtered_atoms;
        std::vector<int> filtered_matches;

        for (size_t i = 0; i < patterns.size() && i < 4; ++i) {
            int id;
            re2::RE2::ErrorCode error_code = filtered_re2.Add(patterns[i], re2::RE2::DefaultOptions, &id);
            if (error_code == re2::RE2::NoError) {
                filtered_atoms.push_back(id);
            }
        }

        if (!filtered_atoms.empty()) {
            bool all_matches_result = filtered_re2.AllMatches(text, filtered_atoms, &filtered_matches);
        }
    }

    return 0;
}