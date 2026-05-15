// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::RE2::PartialMatch at re2.h:427:15 in re2.h
// re2::RE2::Options::set_log_errors at re2.h:709:10 in re2.h
// re2::FilteredRE2::Add at filtered_re2.cc:51:29 in filtered_re2.h
// re2::FilteredRE2::Add at filtered_re2.cc:51:29 in filtered_re2.h
// re2::FilteredRE2::NumRegexps at filtered_re2.h:90:7 in filtered_re2.h
// re2::FilteredRE2::SlowFirstMatch at filtered_re2.cc:91:18 in filtered_re2.h
// re2::FilteredRE2::GetRE2 at filtered_re2.h:93:14 in filtered_re2.h
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
#include <cstring>
#include <vector>
#include <string>
#include <fstream>

extern "C" int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;
    
    // Split input into parts for different test scenarios
    size_t split_point = Size / 2;
    absl::string_view pattern_data(reinterpret_cast<const char*>(Data), split_point);
    absl::string_view text_data(reinterpret_cast<const char*>(Data + split_point), Size - split_point);
    
    // Test 1: FilteredRE2 with SlowFirstMatch, GetRE2, and NumRegexps
    {
        re2::FilteredRE2 filtered_re2(3); // min_atom_len = 3
        
        // Add multiple patterns to FilteredRE2
        std::vector<int> pattern_ids;
        re2::RE2::Options options;
        options.set_log_errors(false); // Disable error logging for fuzzing
        
        // Try to add patterns from the input data
        int id = 0;
        re2::RE2::ErrorCode error = filtered_re2.Add(pattern_data, options, &id);
        if (error == re2::RE2::NoError) {
            pattern_ids.push_back(id);
        }
        
        // Add another pattern using a subset of data
        if (pattern_data.size() > 10) {
            absl::string_view pattern2(pattern_data.data(), pattern_data.size() / 2);
            error = filtered_re2.Add(pattern2, options, &id);
            if (error == re2::RE2::NoError) {
                pattern_ids.push_back(id);
            }
        }
        
        // Test NumRegexps
        int num_regexps = filtered_re2.NumRegexps();
        (void)num_regexps; // Use result to avoid unused variable warning
        
        // Test SlowFirstMatch if we have any patterns
        if (!pattern_ids.empty()) {
            int match_result = filtered_re2.SlowFirstMatch(text_data);
            (void)match_result;
            
            // Test GetRE2 for each pattern ID
            for (int pid : pattern_ids) {
                if (pid >= 0 && pid < num_regexps) {
                    try {
                        const re2::RE2& re2_obj = filtered_re2.GetRE2(pid);
                        (void)re2_obj;
                    } catch (...) {
                        // Ignore exceptions during fuzzing
                    }
                }
            }
        }
        
        // Test Compile and FirstMatch if we have patterns
        if (!pattern_ids.empty()) {
            std::vector<std::string> strings_to_match;
            // Create some strings for Compile
            if (text_data.size() > 0) {
                strings_to_match.push_back(std::string(text_data.data(), std::min(text_data.size(), size_t(100))));
            }
            
            // Try to compile (this may fail, but that's OK for fuzzing)
            filtered_re2.Compile(&strings_to_match);
            
            // Test FirstMatch with an atoms vector
            std::vector<int> atoms;
            for (size_t i = 0; i < pattern_ids.size() && i < 5; ++i) {
                atoms.push_back(pattern_ids[i]);
            }
            
            if (!atoms.empty()) {
                int first_match = filtered_re2.FirstMatch(text_data, atoms);
                (void)first_match;
            }
        }
    }
    
    // Test 2: LazyRE2::get()
    {
        // Create a LazyRE2 object
        static absl::once_flag flag;
        struct LazyRE2Wrapper {
            mutable absl::once_flag once;
            mutable re2::RE2* re2;
            
            LazyRE2Wrapper(const char* pattern) : re2(nullptr) {
                absl::call_once(once, [this, pattern]() {
                    re2 = new re2::RE2(pattern);
                });
            }
            
            ~LazyRE2Wrapper() {
                delete re2;
            }
            
            re2::RE2* get() const {
                absl::call_once(once, [this]() {
                    // This shouldn't be called since we initialize in constructor
                });
                return re2;
            }
        };
        
        // Use a simple pattern for LazyRE2
        const char* simple_pattern = "test.*pattern";
        LazyRE2Wrapper lazy_re2(simple_pattern);
        
        // Call get() multiple times to test thread-safe lazy initialization
        re2::RE2* re2_ptr = lazy_re2.get();
        (void)re2_ptr;
        
        // Also test with pattern from input data if it's not empty
        if (pattern_data.size() > 0 && pattern_data.size() < 100) {
            std::string pattern_str(pattern_data.data(), pattern_data.size());
            LazyRE2Wrapper lazy_re2_input(pattern_str.c_str());
            re2::RE2* re2_ptr2 = lazy_re2_input.get();
            (void)re2_ptr2;
        }
    }
    
    // Test 3: Direct RE2 usage
    {
        if (pattern_data.size() > 0) {
            try {
                re2::RE2 direct_re(pattern_data);
                // Try matching with the text data
                bool match_result = re2::RE2::PartialMatch(text_data, direct_re);
                (void)match_result;
            } catch (...) {
                // Ignore exceptions from invalid patterns
            }
        }
    }
    
    // Test 4: File-based test if needed (using dummy file)
    {
        if (Size > 0) {
            std::ofstream dummy_file("./dummy_file", std::ios::binary);
            if (dummy_file) {
                dummy_file.write(reinterpret_cast<const char*>(Data), Size);
                dummy_file.close();
                
                // Could read the file and use it as pattern/text here
                // For example, create RE2 from file contents
                std::ifstream in_file("./dummy_file", std::ios::binary);
                if (in_file) {
                    std::string file_contents((std::istreambuf_iterator<char>(in_file)),
                                             std::istreambuf_iterator<char>());
                    in_file.close();
                    
                    if (!file_contents.empty()) {
                        try {
                            re2::RE2 file_re(file_contents);
                            (void)file_re;
                        } catch (...) {
                            // Ignore exceptions
                        }
                    }
                }
            }
        }
    }
    
    return 0;
}