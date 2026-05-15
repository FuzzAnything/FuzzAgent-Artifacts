// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::FilteredRE2::SlowFirstMatch at filtered_re2.cc:91:18 in filtered_re2.h
// re2::FilteredRE2::SlowFirstMatch at filtered_re2.cc:91:18 in filtered_re2.h
// re2::FilteredRE2::SlowFirstMatch at filtered_re2.cc:91:18 in filtered_re2.h
// re2::FilteredRE2::Compile at filtered_re2.cc:70:19 in filtered_re2.h
// re2::FilteredRE2::AllMatches at filtered_re2.cc:112:19 in filtered_re2.h
// re2::FilteredRE2::FirstMatch at filtered_re2.cc:98:18 in filtered_re2.h
// re2::FilteredRE2::SlowFirstMatch at filtered_re2.cc:91:18 in filtered_re2.h
// re2::FilteredRE2::AllMatches at filtered_re2.cc:112:19 in filtered_re2.h
// re2::FilteredRE2::FirstMatch at filtered_re2.cc:98:18 in filtered_re2.h
// re2::RE2::Options::set_encoding at re2.h:700:10 in re2.h
// re2::RE2::Options::set_log_errors at re2.h:709:10 in re2.h
// re2::RE2::Options::set_max_mem at re2.h:697:10 in re2.h
// re2::RE2::Options::set_encoding at re2.h:700:10 in re2.h
// re2::RE2::Options::set_log_errors at re2.h:709:10 in re2.h
// re2::RE2::Options::set_max_mem at re2.h:697:10 in re2.h
// re2::FilteredRE2::Add at filtered_re2.cc:51:29 in filtered_re2.h
// re2::FilteredRE2::Add at filtered_re2.cc:51:29 in filtered_re2.h
// re2::FilteredRE2::NumRegexps at filtered_re2.h:90:7 in filtered_re2.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <vector>
#include <string>
#include <fstream>

#include "re2/filtered_re2.h"
#include "re2/re2.h"

static void WriteDummyFile(const uint8_t* data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data), size);
        file.close();
    }
}

extern "C" int LLVMFuzzerTestOneInput_40(const uint8_t* data, size_t size) {
    if (size < 2) return 0;
    
    // Use first byte to control behavior diversity
    uint8_t selector = data[0];
    size_t offset = 1;
    
    // Ensure we have enough data for text
    if (size - offset < 1) return 0;
    
    // Create FilteredRE2 instance
    re2::FilteredRE2 filtered_re2;
    
    // Add patterns with careful bounds checking
    int id1 = -1, id2 = -1;
    re2::RE2::Options options1;
    options1.set_encoding(re2::RE2::Options::EncodingUTF8);
    options1.set_log_errors(false);
    options1.set_max_mem(1 << 20);  // Limit memory usage
    
    re2::RE2::Options options2;
    options2.set_encoding(re2::RE2::Options::EncodingLatin1);
    options2.set_log_errors(false);
    options2.set_max_mem(1 << 20);  // Limit memory usage
    
    // Create pattern1 from data
    std::string pattern1;
    if (offset < size) {
        size_t pattern1_size = std::min((size - offset) / 4, static_cast<size_t>(50));
        if (pattern1_size > 0) {
            pattern1.assign(reinterpret_cast<const char*>(data + offset), 
                           std::min(pattern1_size, size - offset));
            // Remove null bytes to avoid issues
            pattern1.erase(std::remove(pattern1.begin(), pattern1.end(), '\0'), pattern1.end());
            if (!pattern1.empty() && pattern1.size() > 0) {
                re2::RE2::ErrorCode error1 = filtered_re2.Add(pattern1, options1, &id1);
                (void)error1;
            }
        }
    }
    
    // Create pattern2 from different part of data
    std::string pattern2;
    size_t pattern2_offset = offset + (size - offset) / 3;
    if (pattern2_offset < size) {
        size_t pattern2_size = std::min((size - pattern2_offset) / 4, static_cast<size_t>(50));
        if (pattern2_size > 0) {
            pattern2.assign(reinterpret_cast<const char*>(data + pattern2_offset), 
                           std::min(pattern2_size, size - pattern2_offset));
            pattern2.erase(std::remove(pattern2.begin(), pattern2.end(), '\0'), pattern2.end());
            if (!pattern2.empty() && pattern2.size() > 0) {
                re2::RE2::ErrorCode error2 = filtered_re2.Add(pattern2, options2, &id2);
                (void)error2;
            }
        }
    }
    
    // Test NumRegexps
    int num_regexps = filtered_re2.NumRegexps();
    (void)num_regexps;
    
    // Prepare text input - ensure it's not null and has valid data
    std::string safe_text;
    if (offset < size) {
        size_t text_size = std::min(size - offset, static_cast<size_t>(100));
        safe_text.assign(reinterpret_cast<const char*>(data + offset), text_size);
        // Remove null bytes from text to avoid issues with string_view
        safe_text.erase(std::remove(safe_text.begin(), safe_text.end(), '\0'), safe_text.end());
    }
    
    // Only test SlowFirstMatch if we have valid regexps and non-empty text
    if (num_regexps > 0 && !safe_text.empty()) {
        absl::string_view text_view(safe_text);
        
        // Test with the safe text
        int slow_match = filtered_re2.SlowFirstMatch(text_view);
        (void)slow_match;
        
        // Test with empty text
        absl::string_view empty_text("");
        filtered_re2.SlowFirstMatch(empty_text);
        
        // Test with single character if available
        if (!safe_text.empty()) {
            absl::string_view single_char(safe_text.data(), 1);
            filtered_re2.SlowFirstMatch(single_char);
        }
    }
    
    // Prepare for Compile and other functions
    std::vector<std::string> strings_to_match;
    std::vector<int> atoms;
    std::vector<int> matching_regexps;
    
    // Test Compile only if we have regexps
    if (num_regexps > 0) {
        filtered_re2.Compile(&strings_to_match);
        
        // Create atoms vector based on strings_to_match
        for (size_t i = 0; i < strings_to_match.size() && i < 5; ++i) {
            atoms.push_back(static_cast<int>(i));
        }
        
        // Test AllMatches if we have atoms and valid text
        if (!atoms.empty() && !safe_text.empty()) {
            absl::string_view text_view(safe_text);
            bool all_matches_result = filtered_re2.AllMatches(text_view, atoms, &matching_regexps);
            (void)all_matches_result;
        }
        
        // Test FirstMatch if we have atoms and valid text
        if (!atoms.empty() && !safe_text.empty()) {
            absl::string_view text_view(safe_text);
            int first_match = filtered_re2.FirstMatch(text_view, atoms);
            (void)first_match;
        }
    }
    
    // Test with file-based input for diversity
    if ((selector & 0x01) && size > 10 && num_regexps > 0) {
        WriteDummyFile(data, std::min(size, static_cast<size_t>(100)));
        std::ifstream file("./dummy_file", std::ios::binary);
        if (file.is_open()) {
            std::string file_content((std::istreambuf_iterator<char>(file)),
                                    std::istreambuf_iterator<char>());
            // Remove null bytes from file content
            file_content.erase(std::remove(file_content.begin(), file_content.end(), '\0'), file_content.end());
            
            if (!file_content.empty()) {
                absl::string_view file_text(file_content);
                
                // Test with file content
                filtered_re2.SlowFirstMatch(file_text);
                
                if (!atoms.empty()) {
                    std::vector<int> file_matching_regexps;
                    filtered_re2.AllMatches(file_text, atoms, &file_matching_regexps);
                    filtered_re2.FirstMatch(file_text, atoms);
                }
            }
            file.close();
        }
    }
    
    return 0;
}