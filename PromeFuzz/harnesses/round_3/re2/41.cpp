// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::FilteredRE2::Add at filtered_re2.cc:51:29 in filtered_re2.h
// re2::FilteredRE2::AllMatches at filtered_re2.cc:112:19 in filtered_re2.h
// re2::RE2::Options::set_max_mem at re2.h:697:10 in re2.h
// re2::RE2::Options::set_log_errors at re2.h:709:10 in re2.h
// re2::RE2::Set::Add at set.cc:64:15 in set.h
// re2::RE2::Set::Compile at set.cc:103:16 in set.h
// re2::RE2::Set::Size at set.cc:58:15 in set.h
// re2::RE2::Set::Match at set.cc:133:16 in set.h
// re2::RE2::Set::Match at set.cc:137:16 in set.h
// re2::RE2::Set::Match at set.cc:133:16 in set.h
// re2::RE2::Set::Match at set.cc:137:16 in set.h
// re2::RE2::Set::Match at set.cc:133:16 in set.h
// re2::RE2::Set::Match at set.cc:137:16 in set.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <cstring>
#include <vector>
#include <string>
#include "re2/set.h"
#include "re2/filtered_re2.h"

extern "C" int LLVMFuzzerTestOneInput_41(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    // Split data into patterns and text
    size_t split_point = Size / 2;
    std::string patterns_data(reinterpret_cast<const char*>(Data), split_point);
    std::string text_data(reinterpret_cast<const char*>(Data + split_point), Size - split_point);
    
    // Create RE2::Set with default options
    re2::RE2::Options options;
    options.set_max_mem(64 * 1024 * 1024); // Reduce memory limit to 64MB
    options.set_log_errors(false); // Disable error logging to avoid output
    
    // Test with different anchor types
    for (int anchor_type = 0; anchor_type <= 2; ++anchor_type) {
        re2::RE2::Set regex_set(options, static_cast<re2::RE2::Anchor>(anchor_type));
        
        // Add patterns to the set
        std::vector<int> pattern_indices;
        size_t pos = 0;
        int valid_patterns = 0;
        
        while (pos < patterns_data.size() && valid_patterns < 5) { // Limit to 5 patterns
            size_t next_null = patterns_data.find('\0', pos);
            if (next_null == std::string::npos) next_null = patterns_data.size();
            
            if (next_null > pos) {
                std::string pattern = patterns_data.substr(pos, next_null - pos);
                // Skip very long patterns to avoid excessive memory usage
                if (pattern.size() > 50) {
                    pos = next_null + 1;
                    continue;
                }
                std::string error;
                int index = regex_set.Add(pattern, &error);
                if (index >= 0) {
                    pattern_indices.push_back(index);
                    valid_patterns++;
                }
            }
            
            if (next_null == patterns_data.size()) break;
            pos = next_null + 1;
        }
        
        // Only compile if we have valid patterns
        if (valid_patterns > 0) {
            bool compile_success = regex_set.Compile();
            if (!compile_success) continue;
            
            // Test Size() function
            int set_size = regex_set.Size();
            
            // Test Match() overloads with limited text size
            std::vector<int> matches;
            if (text_data.size() > 1000) {
                text_data.resize(1000);
            }
            
            bool match_result1 = regex_set.Match(text_data, &matches);
            
            re2::RE2::Set::ErrorInfo error_info;
            bool match_result2 = regex_set.Match(text_data, &matches, &error_info);
            
            // Test with empty vector
            std::vector<int> empty_vec;
            regex_set.Match(text_data, &empty_vec);
            regex_set.Match(text_data, &empty_vec, &error_info);
            
            // Test with nullptr
            regex_set.Match(text_data, nullptr);
            regex_set.Match(text_data, nullptr, &error_info);
        }
    }
    
    // Test FilteredRE2::AllMatches with limited scope
    {
        re2::FilteredRE2 filtered_re2;
        std::vector<int> filtered_atoms;
        std::vector<int> matching_regexps;
        
        // Add limited patterns to FilteredRE2
        size_t pos = 0;
        int filtered_count = 0;
        while (pos < patterns_data.size() && filtered_count < 3) {
            size_t next_null = patterns_data.find('\0', pos);
            if (next_null == std::string::npos) next_null = patterns_data.size();
            
            if (next_null > pos) {
                std::string pattern = patterns_data.substr(pos, next_null - pos);
                if (pattern.size() > 50) {
                    pos = next_null + 1;
                    continue;
                }
                int id;
                if (filtered_re2.Add(pattern, re2::RE2::DefaultOptions, &id)) {
                    filtered_atoms.push_back(id);
                    filtered_count++;
                }
            }
            
            if (next_null == patterns_data.size()) break;
            pos = next_null + 1;
        }
        
        // Test AllMatches if we have atoms
        if (!filtered_atoms.empty() && text_data.size() > 0) {
            if (text_data.size() > 1000) {
                text_data.resize(1000);
            }
            bool all_matches_result = filtered_re2.AllMatches(text_data, filtered_atoms, &matching_regexps);
        }
    }
    
    return 0;
}