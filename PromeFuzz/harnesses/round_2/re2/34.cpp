// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::FilteredRE2::AllMatches at filtered_re2.cc:112:19 in filtered_re2.h
// re2::FilteredRE2::AllMatches at filtered_re2.cc:112:19 in filtered_re2.h
// re2::FilteredRE2::FirstMatch at filtered_re2.cc:98:18 in filtered_re2.h
// re2::FilteredRE2::AllMatches at filtered_re2.cc:112:19 in filtered_re2.h
// re2::FilteredRE2::SlowFirstMatch at filtered_re2.cc:91:18 in filtered_re2.h
// re2::FilteredRE2::Add at filtered_re2.cc:51:29 in filtered_re2.h
// re2::FilteredRE2::NumRegexps at filtered_re2.h:90:7 in filtered_re2.h
// re2::RE2::Options::set_encoding at re2.h:700:10 in re2.h
// re2::RE2::Options::set_encoding at re2.h:700:10 in re2.h
// re2::FilteredRE2::Add at filtered_re2.cc:51:29 in filtered_re2.h
// re2::FilteredRE2::NumRegexps at filtered_re2.h:90:7 in filtered_re2.h
// re2::FilteredRE2::SlowFirstMatch at filtered_re2.cc:91:18 in filtered_re2.h
// re2::FilteredRE2::Compile at filtered_re2.cc:70:19 in filtered_re2.h
// re2::FilteredRE2::FirstMatch at filtered_re2.cc:98:18 in filtered_re2.h
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
#include <cstddef>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>

static void WriteDummyFile(const uint8_t* data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data), size);
        file.close();
    }
}

extern "C" int LLVMFuzzerTestOneInput_34(const uint8_t* Data, size_t Size) {
    if (Size < 2) return 0;
    
    // Initialize FilteredRE2 with min_atom_len from input
    int min_atom_len = Data[0] % 32 + 1;  // 1-32
    re2::FilteredRE2 filtered_re2(min_atom_len);
    
    // Use part of input for patterns
    size_t pattern_count = std::min<size_t>(Data[1] % 8 + 1, 8);  // 1-8 patterns
    size_t pattern_data_size = Size / (pattern_count + 1);
    
    std::vector<int> pattern_ids;
    pattern_ids.reserve(pattern_count);
    
    // Add patterns to FilteredRE2
    for (size_t i = 0; i < pattern_count; ++i) {
        size_t start = (i * pattern_data_size) % Size;
        size_t end = std::min(start + pattern_data_size, Size);
        if (end <= start) break;
        
        std::string pattern(reinterpret_cast<const char*>(Data + start), end - start);
        
        re2::RE2::Options options;
        // Set encoding based on input
        if ((Data[start] & 0x1) == 0) {
            options.set_encoding(re2::RE2::Options::EncodingUTF8);
        } else {
            options.set_encoding(re2::RE2::Options::EncodingLatin1);
        }
        
        int id = -1;
        re2::RE2::ErrorCode error = filtered_re2.Add(pattern, options, &id);
        
        if (error == re2::RE2::NoError && id >= 0) {
            pattern_ids.push_back(id);
        }
    }
    
    // Call NumRegexps
    int num_regexps = filtered_re2.NumRegexps();
    
    // Use remaining input as text
    size_t text_start = (pattern_count * pattern_data_size) % Size;
    if (text_start < Size) {
        size_t text_size = Size - text_start;
        absl::string_view text(reinterpret_cast<const char*>(Data + text_start), text_size);
        
        // Call SlowFirstMatch (can be called before Compile)
        int slow_match = filtered_re2.SlowFirstMatch(text);
        
        // Prepare for Compile and other functions
        std::vector<std::string> strings_to_match;
        
        // Call Compile only if we have patterns
        if (!pattern_ids.empty()) {
            filtered_re2.Compile(&strings_to_match);
            
            // Prepare atoms vector for AllMatches and FirstMatch
            std::vector<int> atoms;
            if (!strings_to_match.empty()) {
                // Select some atoms based on input data
                size_t atom_count = std::min<size_t>(Data[text_start] % strings_to_match.size(), 
                                                    strings_to_match.size());
                for (size_t i = 0; i < atom_count; ++i) {
                    atoms.push_back(i % strings_to_match.size());
                }
            }
            
            // Call FirstMatch only after Compile
            int first_match = filtered_re2.FirstMatch(text, atoms);
            
            // Call AllMatches
            std::vector<int> matching_regexps;
            bool all_matches_result = filtered_re2.AllMatches(text, atoms, &matching_regexps);
            
            // Try with empty atoms vector
            std::vector<int> empty_atoms;
            std::vector<int> empty_matches;
            filtered_re2.AllMatches(text, empty_atoms, &empty_matches);
            
            // Try with different text views
            if (text_size > 1) {
                absl::string_view partial_text(text.data(), text_size / 2);
                filtered_re2.FirstMatch(partial_text, atoms);
                
                std::vector<int> partial_matches;
                filtered_re2.AllMatches(partial_text, atoms, &partial_matches);
            }
        }
        
        // Try SlowFirstMatch with different text views even without patterns
        if (text_size > 1) {
            absl::string_view partial_text(text.data(), text_size / 2);
            filtered_re2.SlowFirstMatch(partial_text);
        }
    }
    
    // Write dummy file for any potential file operations
    WriteDummyFile(Data, std::min<size_t>(Size, 1024));
    
    // Try creating a new FilteredRE2 with different parameters
    int alt_min_atom_len = (Data[0] + Data[Size > 1 ? 1 : 0]) % 64 + 1;
    re2::FilteredRE2 alt_filtered_re2(alt_min_atom_len);
    
    // Add a simple pattern to the alternative instance
    if (Size > 10) {
        std::string alt_pattern(reinterpret_cast<const char*>(Data), 
                               std::min<size_t>(Size, 50));
        re2::RE2::Options alt_options;
        int alt_id = -1;
        alt_filtered_re2.Add(alt_pattern, alt_options, &alt_id);
        
        // Call NumRegexps on alternative instance
        int alt_num_regexps = alt_filtered_re2.NumRegexps();
    }
    
    return 0;
}