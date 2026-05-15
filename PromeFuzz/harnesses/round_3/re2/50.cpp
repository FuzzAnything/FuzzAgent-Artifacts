// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::FilteredRE2::Add at filtered_re2.cc:51:29 in filtered_re2.h
// re2::FilteredRE2::Add at filtered_re2.cc:51:29 in filtered_re2.h
// re2::FilteredRE2::Add at filtered_re2.cc:51:29 in filtered_re2.h
// re2::FilteredRE2::Compile at filtered_re2.cc:70:19 in filtered_re2.h
// re2::FilteredRE2::AllMatches at filtered_re2.cc:112:19 in filtered_re2.h
// re2::FilteredRE2::Add at filtered_re2.cc:51:29 in filtered_re2.h
// re2::FilteredRE2::Add at filtered_re2.cc:51:29 in filtered_re2.h
// re2::FilteredRE2::Add at filtered_re2.cc:51:29 in filtered_re2.h
// re2::FilteredRE2::Compile at filtered_re2.cc:70:19 in filtered_re2.h
// re2::FilteredRE2::AllPotentials at filtered_re2.cc:124:19 in filtered_re2.h
// re2::FilteredRE2::Add at filtered_re2.cc:51:29 in filtered_re2.h
// re2::FilteredRE2::Add at filtered_re2.cc:51:29 in filtered_re2.h
// re2::FilteredRE2::Add at filtered_re2.cc:51:29 in filtered_re2.h
// re2::FilteredRE2::Compile at filtered_re2.cc:70:19 in filtered_re2.h
// re2::RE2::Options::set_encoding at re2.h:700:10 in re2.h
// re2::RE2::Options::set_case_sensitive at re2.h:724:10 in re2.h
// re2::FilteredRE2::Add at filtered_re2.cc:51:29 in filtered_re2.h
// re2::FilteredRE2::Add at filtered_re2.cc:51:29 in filtered_re2.h
// re2::FilteredRE2::Add at filtered_re2.cc:51:29 in filtered_re2.h
// re2::FilteredRE2::Compile at filtered_re2.cc:70:19 in filtered_re2.h
// re2::FilteredRE2::FirstMatch at filtered_re2.cc:98:18 in filtered_re2.h
// re2::RE2::Set::Add at set.cc:64:15 in set.h
// re2::RE2::Set::Add at set.cc:64:15 in set.h
// re2::RE2::Set::Add at set.cc:64:15 in set.h
// re2::RE2::Set::Match at set.cc:133:16 in set.h
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
#include <algorithm>

#include "re2/filtered_re2.h"
#include "re2/set.h"

static void WriteToDummyFile(const uint8_t* data, size_t size) {
    FILE* f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_50(const uint8_t* Data, size_t Size) {
    if (Size < 2) return 0;

    // Use first byte to determine which function to test
    uint8_t selector = Data[0] % 6;
    const uint8_t* fuzz_data = Data + 1;
    size_t fuzz_size = Size - 1;

    // Create options for RE2 objects
    re2::RE2::Options options;
    options.set_encoding(re2::RE2::Options::EncodingUTF8);
    options.set_case_sensitive(true);

    switch (selector) {
        case 0: {  // FilteredRE2::FirstMatch
            re2::FilteredRE2 filtered_re2;
            
            // Add some patterns
            int id1, id2, id3;
            std::string pattern1 = "test.*pattern";
            std::string pattern2 = "[a-z]+[0-9]*";
            std::string pattern3 = "\\w+\\s+\\w+";
            
            filtered_re2.Add(pattern1, options, &id1);
            filtered_re2.Add(pattern2, options, &id2);
            filtered_re2.Add(pattern3, options, &id3);
            
            // Compile
            std::vector<std::string> strings_to_match;
            filtered_re2.Compile(&strings_to_match);
            
            // Create atoms vector
            std::vector<int> atoms;
            if (fuzz_size > 0) {
                for (size_t i = 0; i < std::min(fuzz_size, (size_t)10); ++i) {
                    atoms.push_back(fuzz_data[i] % 10);
                }
            }
            
            // Create text from fuzz data
            std::string text(reinterpret_cast<const char*>(fuzz_data), 
                           std::min(fuzz_size, (size_t)100));
            
            // Call FirstMatch
            int result = filtered_re2.FirstMatch(text, atoms);
            (void)result;  // Prevent unused variable warning
            
            break;
        }
        
        case 1: {  // RE2::Set::Match
            re2::RE2::Set set(options, re2::RE2::Anchor::UNANCHORED);
            
            // Add some patterns to the set
            std::string error;
            set.Add("hello.*world", &error);
            set.Add("[0-9]{3,5}", &error);
            set.Add("\\bword\\b", &error);
            
            // Create text from fuzz data
            std::string text(reinterpret_cast<const char*>(fuzz_data), 
                           std::min(fuzz_size, (size_t)100));
            
            // Call Match
            std::vector<int> matches;
            bool match_result = set.Match(text, &matches);
            (void)match_result;  // Prevent unused variable warning
            
            break;
        }
        
        case 2: {  // FilteredRE2::AllMatches
            re2::FilteredRE2 filtered_re2;
            
            // Add some patterns
            int id1, id2, id3;
            std::string pattern1 = "a.*b";
            std::string pattern2 = "c.*d";
            std::string pattern3 = "e.*f";
            
            filtered_re2.Add(pattern1, options, &id1);
            filtered_re2.Add(pattern2, options, &id2);
            filtered_re2.Add(pattern3, options, &id3);
            
            // Compile
            std::vector<std::string> strings_to_match;
            filtered_re2.Compile(&strings_to_match);
            
            // Create atoms vector
            std::vector<int> atoms;
            if (fuzz_size > 0) {
                for (size_t i = 0; i < std::min(fuzz_size, (size_t)5); ++i) {
                    atoms.push_back(fuzz_data[i] % 5);
                }
            }
            
            // Create text from fuzz data
            std::string text(reinterpret_cast<const char*>(fuzz_data), 
                           std::min(fuzz_size, (size_t)50));
            
            // Call AllMatches
            std::vector<int> matching_regexps;
            bool result = filtered_re2.AllMatches(text, atoms, &matching_regexps);
            (void)result;  // Prevent unused variable warning
            
            break;
        }
        
        case 3: {  // FilteredRE2::AllPotentials
            re2::FilteredRE2 filtered_re2;
            
            // Add some patterns
            int id1, id2, id3;
            std::string pattern1 = "x.*y";
            std::string pattern2 = "m.*n";
            std::string pattern3 = "p.*q";
            
            filtered_re2.Add(pattern1, options, &id1);
            filtered_re2.Add(pattern2, options, &id2);
            filtered_re2.Add(pattern3, options, &id3);
            
            // Compile
            std::vector<std::string> strings_to_match;
            filtered_re2.Compile(&strings_to_match);
            
            // Create atoms vector
            std::vector<int> atoms;
            if (fuzz_size > 0) {
                for (size_t i = 0; i < std::min(fuzz_size, (size_t)8); ++i) {
                    atoms.push_back(fuzz_data[i] % 8);
                }
            }
            
            // Call AllPotentials
            std::vector<int> potential_regexps;
            filtered_re2.AllPotentials(atoms, &potential_regexps);
            
            break;
        }
        
        case 4: {  // FilteredRE2::Add
            re2::FilteredRE2 filtered_re2;
            
            // Create pattern from fuzz data
            std::string pattern(reinterpret_cast<const char*>(fuzz_data), 
                              std::min(fuzz_size, (size_t)50));
            
            // Call Add with the fuzz pattern
            int id;
            re2::RE2::ErrorCode error = filtered_re2.Add(pattern, options, &id);
            (void)error;  // Prevent unused variable warning
            
            break;
        }
        
        case 5: {  // FilteredRE2::Compile
            re2::FilteredRE2 filtered_re2;
            
            // Add some patterns
            int id1, id2;
            std::string pattern1 = "compile.*test";
            std::string pattern2 = "another.*pattern";
            
            filtered_re2.Add(pattern1, options, &id1);
            filtered_re2.Add(pattern2, options, &id2);
            
            // Call Compile
            std::vector<std::string> strings_to_match;
            filtered_re2.Compile(&strings_to_match);
            
            break;
        }
    }

    return 0;
}