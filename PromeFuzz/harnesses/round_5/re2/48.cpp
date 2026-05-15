// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::FilteredRE2::FirstMatch at filtered_re2.cc:98:18 in filtered_re2.h
// re2::LazyRE2::get at re2.h:1000:8 in re2.h
// re2::RE2::Options::set_encoding at re2.h:700:10 in re2.h
// re2::FilteredRE2::Add at filtered_re2.cc:51:29 in filtered_re2.h
// re2::FilteredRE2::NumRegexps at filtered_re2.h:90:7 in filtered_re2.h
// re2::FilteredRE2::GetRE2 at filtered_re2.h:93:14 in filtered_re2.h
// re2::FilteredRE2::SlowFirstMatch at filtered_re2.cc:91:18 in filtered_re2.h
// re2::FilteredRE2::Compile at filtered_re2.cc:70:19 in filtered_re2.h
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

static const char dummy_file_path[] = "./dummy_file";

extern "C" int LLVMFuzzerTestOneInput_48(const uint8_t *Data, size_t Size) {
    // Use a small portion of data to determine parameters
    size_t cursor = 0;
    if (Size < 4) return 0;
    
    // Extract parameters from first few bytes
    uint8_t min_atom_len = Data[cursor++] % 10;
    uint8_t num_patterns = Data[cursor++] % 8;
    uint8_t use_options = Data[cursor++] % 2;
    uint8_t test_mode = Data[cursor++] % 4;
    
    // Create FilteredRE2 instance
    re2::FilteredRE2 filtered_re2(min_atom_len);
    
    // Add patterns
    std::vector<int> pattern_ids;
    std::vector<std::string> added_patterns;
    for (int i = 0; i < num_patterns && cursor < Size; i++) {
        // Determine pattern length (1-64 bytes)
        size_t pattern_len = 1 + (Data[cursor++] % 64);
        if (cursor + pattern_len > Size) break;
        
        // Extract pattern string
        std::string pattern(reinterpret_cast<const char*>(Data + cursor), pattern_len);
        cursor += pattern_len;
        
        // Add pattern with optional options
        int id = -1;
        re2::RE2::Options options;
        if (use_options) {
            options.set_encoding(Data[cursor++] % 2 ? 
                re2::RE2::Options::EncodingUTF8 : 
                re2::RE2::Options::EncodingLatin1);
        }
        
        re2::RE2::ErrorCode error = filtered_re2.Add(pattern, options, &id);
        if (error == re2::RE2::NoError && id >= 0) {
            pattern_ids.push_back(id);
            added_patterns.push_back(pattern);
        }
    }
    
    // Test NumRegexps
    int num_regexps = filtered_re2.NumRegexps();
    
    // Test GetRE2 for valid indices
    for (int i = 0; i < num_regexps && i < 5; i++) {
        try {
            const re2::RE2& re2_obj = filtered_re2.GetRE2(i);
            (void)re2_obj; // Suppress unused warning
        } catch (...) {
            // Ignore exceptions
        }
    }
    
    // Prepare text for matching
    if (cursor < Size) {
        size_t text_len = Size - cursor;
        std::string text(reinterpret_cast<const char*>(Data + cursor), text_len);
        absl::string_view text_view(text);
        
        // Test SlowFirstMatch (can be called before Compile)
        int slow_match = filtered_re2.SlowFirstMatch(text_view);
        (void)slow_match;
        
        // Try to compile if we have patterns
        if (num_regexps > 0) {
            std::vector<std::string> strings_to_match;
            // Use added patterns for compilation
            for (const auto& pattern : added_patterns) {
                strings_to_match.push_back(pattern);
            }
            
            filtered_re2.Compile(&strings_to_match);
            
            // Create atoms vector for FirstMatch
            std::vector<int> atoms;
            for (size_t i = 0; i < strings_to_match.size() && i < 10; i++) {
                atoms.push_back(i);
            }
            
            // Test FirstMatch with atoms
            if (!atoms.empty()) {
                int first_match = filtered_re2.FirstMatch(text_view, atoms);
                (void)first_match;
            }
        }
    }
    
    // Test LazyRE2 with a simple pattern
    static re2::LazyRE2 lazy_re2;
    static absl::once_flag once;
    
    // Use a fixed pattern for LazyRE2 to avoid initialization issues
    static const char* simple_pattern = "test.*pattern";
    re2::LazyRE2* lazy_ptr = const_cast<re2::LazyRE2*>(&lazy_re2);
    
    // Try to access the RE2 object (may initialize it)
    try {
        re2::RE2* re2_ptr = lazy_ptr->get();
        (void)re2_ptr;
    } catch (...) {
        // Ignore exceptions
    }
    
    // Write data to dummy file if needed (not directly used by these APIs)
    FILE* f = fopen(dummy_file_path, "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }
    
    return 0;
}