// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::FilteredRE2::AllPotentials at filtered_re2.cc:124:19 in filtered_re2.h
// re2::FilteredRE2::Add at filtered_re2.cc:51:29 in filtered_re2.h
// re2::FilteredRE2::NumRegexps at filtered_re2.h:90:7 in filtered_re2.h
// re2::FilteredRE2::NumRegexps at filtered_re2.h:90:7 in filtered_re2.h
// re2::FilteredRE2::GetRE2 at filtered_re2.h:93:14 in filtered_re2.h
// re2::LazyRE2::get at re2.h:1000:8 in re2.h
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

static void WriteDummyFile(const uint8_t* data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data), size);
        file.close();
    }
}

extern "C" int LLVMFuzzerTestOneInput_15(const uint8_t* Data, size_t Size) {
    if (Size < 2) return 0;

    // Create FilteredRE2 instance
    re2::FilteredRE2 filtered_re2;

    // Add some regex patterns using fuzzer data
    // Use first few bytes to determine number of patterns to add
    size_t num_patterns = Data[0] % 5 + 1;  // 1-5 patterns
    size_t pattern_index = 1;
    
    std::vector<int> pattern_ids;
    for (size_t i = 0; i < num_patterns && pattern_index < Size; i++) {
        int id;
        // Create pattern from fuzzer data
        size_t pattern_len = Data[pattern_index] % 16 + 1;  // 1-16 bytes
        if (pattern_index + pattern_len >= Size) break;
        
        std::string pattern(reinterpret_cast<const char*>(Data + pattern_index), pattern_len);
        pattern_index += pattern_len;
        
        // Add pattern to FilteredRE2
        filtered_re2.Add(pattern, re2::RE2::DefaultOptions, &id);
        pattern_ids.push_back(id);
    }

    if (pattern_ids.empty()) return 0;

    // Test NumRegexps
    int num_regexps = filtered_re2.NumRegexps();
    (void)num_regexps;  // Use variable to avoid unused warning

    // Test GetRE2 for each pattern
    for (int id : pattern_ids) {
        if (id >= 0 && id < filtered_re2.NumRegexps()) {
            const re2::RE2& re2_obj = filtered_re2.GetRE2(id);
            (void)re2_obj;  // Use variable to avoid unused warning
        }
    }

    // Test LazyRE2::get()
    static re2::LazyRE2 lazy_re2 = {"test.*pattern"};
    re2::RE2* re2_ptr = lazy_re2.get();
    (void)re2_ptr;  // Use variable to avoid unused warning

    // Prepare text for matching
    if (pattern_index >= Size) return 0;
    size_t text_len = std::min(Size - pattern_index, static_cast<size_t>(64));
    std::string text(reinterpret_cast<const char*>(Data + pattern_index), text_len);

    // Test SlowFirstMatch (can be called before Compile)
    int slow_match = filtered_re2.SlowFirstMatch(text);
    (void)slow_match;  // Use variable to avoid unused warning

    // Compile the filtered RE2
    std::vector<std::string> atoms_vec;
    filtered_re2.Compile(&atoms_vec);

    // Create atoms vector for FirstMatch and AllPotentials
    std::vector<int> atoms;
    for (size_t i = 0; i < atoms_vec.size() && i < 10; i++) {
        if (!atoms_vec[i].empty()) {
            atoms.push_back(static_cast<int>(atoms_vec[i][0]) % 256);
        }
    }

    // Test FirstMatch
    if (!atoms.empty()) {
        int first_match = filtered_re2.FirstMatch(text, atoms);
        (void)first_match;  // Use variable to avoid unused warning
    }

    // Test AllPotentials
    if (!atoms.empty()) {
        std::vector<int> potential_regexps;
        filtered_re2.AllPotentials(atoms, &potential_regexps);
    }

    // Write to dummy file and test with file reading
    WriteDummyFile(Data, std::min(Size, static_cast<size_t>(1024)));

    return 0;
}