// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::RE2::Options::set_max_mem at re2.h:697:10 in re2.h
// re2::RE2::Set::Add at set.cc:64:15 in set.h
// re2::RE2::Set::Compile at set.cc:103:16 in set.h
// re2::RE2::Set::Match at set.cc:133:16 in set.h
// re2::RE2::Set::Match at set.cc:137:16 in set.h
// re2::RE2::Set::Size at set.cc:58:15 in set.h
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
#include <limits>

#include "re2/set.h"
#include "re2/filtered_re2.h"

static std::string dummy_file = "./dummy_file";

extern "C" int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    // Limit input size to prevent excessive memory usage
    if (Size > 4096) return 0;

    // Split input into pattern and text parts
    size_t split = Size / 2;
    if (split == 0) split = 1;
    if (Size - split == 0) split = Size - 1;
    
    // Ensure we have valid string views
    if (split > Size || Size - split > Size) return 0;
    
    absl::string_view pattern_view(reinterpret_cast<const char*>(Data), split);
    absl::string_view text_view(reinterpret_cast<const char*>(Data + split), Size - split);

    // Test RE2::Set
    {
        re2::RE2::Options options;
        // Set reasonable limits to prevent excessive memory usage
        options.set_max_mem(1 << 20); // 1MB limit
        
        re2::RE2::Set set(options, re2::RE2::ANCHOR_BOTH);
        
        std::string error;
        int index = set.Add(pattern_view, &error);
        
        // Only proceed if addition was successful
        if (index != -1) {
            // Try to compile, but handle potential failures gracefully
            bool compile_result = set.Compile();
            
            if (compile_result) {
                // Test both Match overloads
                std::vector<int> matches;
                bool result1 = set.Match(text_view, &matches);
                
                re2::RE2::Set::ErrorInfo error_info;
                bool result2 = set.Match(text_view, &matches, &error_info);
                
                // Check size
                int size = set.Size();
            }
        }
    }
    
    // Test FilteredRE2::AllMatches with more careful input validation
    {
        // Create a FilteredRE2 object
        re2::FilteredRE2 filtered;
        
        std::vector<int> atoms;
        std::vector<int> matching_regexps;
        
        // Add some atoms based on input data, but limit the number
        if (Size > 0) {
            size_t num_atoms = Data[0] % 16; // Limit to 16 atoms max
            for (size_t i = 0; i < num_atoms && i < Size; i++) {
                atoms.push_back(static_cast<int>(Data[i % Size]) % 100);
            }
        }
        
        // Only call AllMatches if we have valid input
        if (!text_view.empty() && !atoms.empty()) {
            bool result = filtered.AllMatches(text_view, atoms, &matching_regexps);
        }
    }
    
    return 0;
}