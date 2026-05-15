// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::FilteredRE2::Compile at filtered_re2.cc:70:19 in filtered_re2.h
// re2::FilteredRE2::FirstMatch at filtered_re2.cc:98:18 in filtered_re2.h
// re2::LazyRE2::get at re2.h:1000:8 in re2.h
// re2::FilteredRE2::Add at filtered_re2.cc:51:29 in filtered_re2.h
// re2::FilteredRE2::NumRegexps at filtered_re2.h:90:7 in filtered_re2.h
// re2::FilteredRE2::NumRegexps at filtered_re2.h:90:7 in filtered_re2.h
// re2::FilteredRE2::GetRE2 at filtered_re2.h:93:14 in filtered_re2.h
// re2::FilteredRE2::SlowFirstMatch at filtered_re2.cc:91:18 in filtered_re2.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include "re2/filtered_re2.h"
#include "re2/re2.h"
#include <cstdint>
#include <cstring>
#include <vector>
#include <string>
#include <fstream>

static void writeDummyFile(const uint8_t* data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data), size);
        file.close();
    }
}

static std::string generateString(const uint8_t* data, size_t size, size_t max_len) {
    if (size == 0) return "";
    size_t len = data[0] % max_len + 1;
    if (len > size - 1) len = size - 1;
    return std::string(reinterpret_cast<const char*>(data + 1), len);
}

static std::vector<int> generateIntVector(const uint8_t* data, size_t size, size_t max_count) {
    std::vector<int> result;
    if (size < 2) return result;
    
    size_t count = data[0] % max_count + 1;
    if (count * sizeof(int) > size - 1) count = (size - 1) / sizeof(int);
    
    for (size_t i = 0; i < count; ++i) {
        int value;
        memcpy(&value, data + 1 + i * sizeof(int), sizeof(int));
        result.push_back(value);
    }
    return result;
}

extern "C" int LLVMFuzzerTestOneInput_46(const uint8_t* data, size_t size) {
    if (size < 2) return 0;
    
    // Initialize dummy file
    writeDummyFile(data, size);
    
    // Create FilteredRE2 instance
    int min_atom_len = data[0] % 10 + 1;
    re2::FilteredRE2 filtered_re2(min_atom_len);
    
    // Test Add function
    re2::RE2::Options options(static_cast<re2::RE2::CannedOptions>(data[1] % 4));
    std::vector<int> pattern_ids;
    
    size_t offset = 2;
    while (offset < size && pattern_ids.size() < 10) {
        size_t pattern_len = data[offset] % 50 + 1;
        if (offset + pattern_len >= size) break;
        
        std::string pattern(reinterpret_cast<const char*>(data + offset + 1), pattern_len);
        int id;
        re2::RE2::ErrorCode error = filtered_re2.Add(pattern, options, &id);
        
        if (error == re2::RE2::NoError) {
            pattern_ids.push_back(id);
        }
        
        offset += pattern_len + 1;
    }
    
    // Test NumRegexps
    int num_regexps = filtered_re2.NumRegexps();
    (void)num_regexps;
    
    // Test GetRE2
    for (int id : pattern_ids) {
        if (id >= 0 && id < filtered_re2.NumRegexps()) {
            try {
                const re2::RE2& re2_obj = filtered_re2.GetRE2(id);
                (void)re2_obj;
            } catch (...) {
                // Ignore exceptions
            }
        }
    }
    
    // Test SlowFirstMatch
    if (offset < size) {
        std::string text(reinterpret_cast<const char*>(data + offset), size - offset);
        try {
            int match_result = filtered_re2.SlowFirstMatch(text);
            (void)match_result;
        } catch (...) {
            // Ignore exceptions
        }
    }
    
    // Test Compile and FirstMatch
    std::vector<std::string> strings_to_match;
    size_t compile_offset = offset;
    
    // Generate some strings for Compile
    while (compile_offset < size && strings_to_match.size() < 5) {
        size_t str_len = data[compile_offset] % 20 + 1;
        if (compile_offset + str_len >= size) break;
        
        std::string str(reinterpret_cast<const char*>(data + compile_offset + 1), str_len);
        strings_to_match.push_back(str);
        compile_offset += str_len + 1;
    }
    
    if (!strings_to_match.empty()) {
        filtered_re2.Compile(&strings_to_match);
    }
    
    if (offset < size && pattern_ids.size() > 0) {
        std::string text(reinterpret_cast<const char*>(data + offset), 
                        std::min(size - offset, (size_t)100));
        
        std::vector<int> atoms = generateIntVector(data, size, 10);
        try {
            int first_match = filtered_re2.FirstMatch(text, atoms);
            (void)first_match;
        } catch (...) {
            // Ignore exceptions
        }
    }
    
    // Test LazyRE2
    if (size > 10) {
        std::string lazy_pattern(reinterpret_cast<const char*>(data), 
                                std::min(size, (size_t)50));
        
        static absl::once_flag flag;
        static re2::LazyRE2* lazy_re2 = nullptr;
        
        absl::call_once(flag, [&]() {
            lazy_re2 = new re2::LazyRE2();
        });
        
        if (lazy_re2) {
            try {
                re2::RE2* re2_ptr = lazy_re2->get();
                (void)re2_ptr;
            } catch (...) {
                // Ignore exceptions
            }
        }
    }
    
    return 0;
}