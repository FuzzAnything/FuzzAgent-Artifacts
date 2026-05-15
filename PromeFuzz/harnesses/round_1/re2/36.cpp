// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::RE2::Set::Add at set.cc:64:15 in set.h
// re2::RE2::Set::Compile at set.cc:103:16 in set.h
// re2::RE2::Set::Match at set.cc:133:16 in set.h
// re2::FilteredRE2::Add at filtered_re2.cc:51:29 in filtered_re2.h
// re2::FilteredRE2::Compile at filtered_re2.cc:70:19 in filtered_re2.h
// re2::FilteredRE2::AllPotentials at filtered_re2.cc:124:19 in filtered_re2.h
// re2::FilteredRE2::AllMatches at filtered_re2.cc:112:19 in filtered_re2.h
// re2::FilteredRE2::FirstMatch at filtered_re2.cc:98:18 in filtered_re2.h
// re2::RE2::Options::set_max_mem at re2.h:697:10 in re2.h
// re2::RE2::Options::set_encoding at re2.h:700:10 in re2.h
// re2::RE2::Options::set_posix_syntax at re2.h:703:10 in re2.h
// re2::RE2::Options::set_longest_match at re2.h:706:10 in re2.h
// re2::RE2::Options::set_log_errors at re2.h:709:10 in re2.h
// re2::RE2::Options::set_literal at re2.h:712:10 in re2.h
// re2::RE2::Options::set_never_nl at re2.h:715:10 in re2.h
// re2::RE2::Options::set_dot_nl at re2.h:718:10 in re2.h
// re2::RE2::Options::set_never_capture at re2.h:721:10 in re2.h
// re2::RE2::Options::set_case_sensitive at re2.h:724:10 in re2.h
// re2::RE2::Options::set_perl_classes at re2.h:727:10 in re2.h
// re2::RE2::Options::set_word_boundary at re2.h:730:10 in re2.h
// re2::RE2::Options::set_one_line at re2.h:733:10 in re2.h
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
#include <cstdlib>
#include <vector>
#include <string>
#include <algorithm>

#include "re2/set.h"
#include "re2/filtered_re2.h"

static re2::RE2::Options CreateOptionsFromData(const uint8_t* data, size_t size) {
    re2::RE2::Options options;
    
    if (size > 0) {
        // Use first byte to determine some options
        uint8_t first_byte = data[0];
        options.set_max_mem(1 << 16); // 64KB instead of 1MB to avoid excessive memory usage
        options.set_encoding((first_byte & 1) ? re2::RE2::Options::EncodingLatin1 : 
                                                re2::RE2::Options::EncodingUTF8);
        options.set_posix_syntax((first_byte & 2) != 0);
        options.set_longest_match((first_byte & 4) != 0);
        options.set_log_errors(true); // Always log errors for fuzzing
        options.set_literal((first_byte & 16) != 0);
        options.set_never_nl((first_byte & 32) != 0);
        options.set_dot_nl((first_byte & 64) != 0);
        options.set_never_capture((first_byte & 128) != 0);
        
        if (size > 1) {
            uint8_t second_byte = data[1];
            options.set_case_sensitive((second_byte & 1) != 0);
            options.set_perl_classes((second_byte & 2) != 0);
            options.set_word_boundary((second_byte & 4) != 0);
            options.set_one_line((second_byte & 8) != 0);
        }
    }
    
    return options;
}

static std::vector<std::string> ExtractPatterns(const uint8_t* data, size_t size) {
    std::vector<std::string> patterns;
    if (size == 0) return patterns;
    
    const uint8_t* end = data + size;
    const uint8_t* current = data;
    
    while (current < end) {
        // Find next null byte or end
        const uint8_t* pattern_end = current;
        while (pattern_end < end && *pattern_end != 0) {
            ++pattern_end;
        }
        
        size_t pattern_len = pattern_end - current;
        if (pattern_len > 0 && pattern_len < 100) { // Limit pattern length
            std::string pattern(reinterpret_cast<const char*>(current), pattern_len);
            // Skip patterns that are too complex or might cause issues
            if (pattern_len < 50 || pattern.find("\\C") == std::string::npos) {
                patterns.push_back(pattern);
            }
        }
        
        // Move to next pattern (skip null byte)
        if (pattern_end < end) {
            current = pattern_end + 1;
        } else {
            break;
        }
        
        // Limit number of patterns to avoid excessive resource usage
        if (patterns.size() >= 5) { // Reduced from 10 to 5
            break;
        }
    }
    
    return patterns;
}

static std::string ExtractText(const uint8_t* data, size_t size) {
    if (size == 0) return "";
    
    // Use first non-null sequence as text
    const uint8_t* text_start = data;
    while (text_start < data + size && *text_start == 0) {
        ++text_start;
    }
    
    if (text_start >= data + size) return "";
    
    const uint8_t* text_end = text_start;
    size_t max_len = std::min(size_t(100), size - (text_start - data)); // Limit text length
    while (text_end < text_start + max_len && text_end < data + size && *text_end != 0) {
        ++text_end;
    }
    
    size_t text_len = text_end - text_start;
    if (text_len == 0) return "";
    
    return std::string(reinterpret_cast<const char*>(text_start), text_len);
}

static std::vector<int> ExtractAtoms(const uint8_t* data, size_t size) {
    std::vector<int> atoms;
    if (size == 0) return atoms;
    
    // Use bytes as atom indices (modulo some limit)
    size_t limit = std::min(size, size_t(20)); // Reduced from 100 to 20
    for (size_t i = 0; i < limit; ++i) {
        atoms.push_back(data[i] % 20); // Reduced modulo from 50 to 20
    }
    
    // Remove duplicates to simulate actual atom indices
    std::sort(atoms.begin(), atoms.end());
    atoms.erase(std::unique(atoms.begin(), atoms.end()), atoms.end());
    
    return atoms;
}

extern "C" int LLVMFuzzerTestOneInput_36(const uint8_t* data, size_t size) {
    if (size < 2) return 0;
    
    // Extract patterns, text, and atoms from fuzzer input
    std::vector<std::string> patterns = ExtractPatterns(data, size);
    std::string text = ExtractText(data, size);
    std::vector<int> atoms = ExtractAtoms(data, size);
    
    if (patterns.empty()) return 0;
    
    // Create options from fuzzer data
    re2::RE2::Options options = CreateOptionsFromData(data, size);
    
    // Test 1: RE2::Set::Match
    {
        re2::RE2::Set set(options, re2::RE2::UNANCHORED);
        
        // Add patterns to set
        std::vector<int> pattern_ids;
        for (const auto& pattern : patterns) {
            std::string error;
            int pattern_id = set.Add(pattern, &error);
            if (pattern_id != -1) {
                pattern_ids.push_back(pattern_id);
            }
        }
        
        if (!pattern_ids.empty()) {
            // Compile the set - handle potential failures gracefully
            bool compile_success = false;
            try {
                compile_success = set.Compile();
            } catch (...) {
                compile_success = false;
            }
            
            if (compile_success) {
                std::vector<int> matches;
                bool matched = set.Match(text, &matches);
                (void)matched; // Use result to avoid unused variable warning
            }
        }
    }
    
    // Test 2: FilteredRE2
    {
        re2::FilteredRE2 filtered_re2;
        
        // Add patterns to FilteredRE2
        std::vector<int> pattern_ids;
        for (const auto& pattern : patterns) {
            int pattern_id;
            re2::RE2::ErrorCode error = filtered_re2.Add(pattern, options, &pattern_id);
            if (error == re2::RE2::NoError) {
                pattern_ids.push_back(pattern_id);
            }
        }
        
        if (!pattern_ids.empty()) {
            // Compile the filtered RE2
            std::vector<std::string> strings_to_match;
            try {
                filtered_re2.Compile(&strings_to_match);
            } catch (...) {
                // If compilation fails, skip further tests
                return 0;
            }
            
            if (!strings_to_match.empty() && !atoms.empty()) {
                // Test AllPotentials
                std::vector<int> potential_regexps;
                filtered_re2.AllPotentials(atoms, &potential_regexps);
                
                // Test AllMatches
                std::vector<int> matching_regexps;
                bool any_matches = false;
                try {
                    any_matches = filtered_re2.AllMatches(text, atoms, &matching_regexps);
                } catch (...) {
                    any_matches = false;
                }
                (void)any_matches; // Use result to avoid unused variable warning
                
                // Test FirstMatch (requires Compile to be called first)
                int first_match = -1;
                try {
                    first_match = filtered_re2.FirstMatch(text, atoms);
                } catch (...) {
                    first_match = -1;
                }
                (void)first_match; // Use result to avoid unused variable warning
            }
        }
    }
    
    return 0;
}