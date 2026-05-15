#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <string>
#include <vector>
#include "re2/re2.h"
#include "re2/filtered_re2.h"

namespace harness_004 {
// Fuzzing harness for FilteredRE2 multi-pattern matching operations
// Targets uncovered FilteredRE2 APIs: FirstMatch, SlowFirstMatch, NumRegexps, GetRE2, AllPotentials
// Also tests Add and Compile more comprehensively than harness_001
// Follows coverage guidance to address 5 completely uncovered APIs with 30+ undiscovered branches



// Entry point for libFuzzer
int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check: need enough for basic operations
    if (size < 32) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Step 1: Initialize RE2::Options with diverse settings
    RE2::Options options;
    
    // Consume encoding setting
    uint8_t encoding_choice = fdp.ConsumeIntegral<uint8_t>() % 3;
    switch (encoding_choice) {
        case 0:
            options.set_encoding(RE2::Options::EncodingUTF8);
            break;
        case 1:
            options.set_encoding(RE2::Options::EncodingLatin1);
            break;
        case 2:
            // Leave as default (UTF8)
            break;
    }
    
    // Consume various boolean options to explore different code paths
    options.set_log_errors(fdp.ConsumeBool());
    options.set_posix_syntax(fdp.ConsumeBool());
    options.set_longest_match(fdp.ConsumeBool());
    options.set_literal(fdp.ConsumeBool());
    options.set_never_nl(fdp.ConsumeBool());
    options.set_dot_nl(fdp.ConsumeBool());
    options.set_never_capture(fdp.ConsumeBool());
    options.set_case_sensitive(fdp.ConsumeBool());
    options.set_perl_classes(fdp.ConsumeBool());
    options.set_word_boundary(fdp.ConsumeBool());
    options.set_one_line(fdp.ConsumeBool());
    
    // Consume max_mem setting
    int64_t max_mem = fdp.ConsumeIntegralInRange<int64_t>(0, 1024 * 1024); // 0-1MB
    options.set_max_mem(max_mem);

    // Step 2: Create FilteredRE2 object
    // Choose constructor: default or with min_atom_len parameter
    bool use_min_atom_len = fdp.ConsumeBool();
    re2::FilteredRE2 filter;
    if (use_min_atom_len && fdp.remaining_bytes() > 0) {
        int min_atom_len = fdp.ConsumeIntegralInRange<int>(1, 10);
        filter = re2::FilteredRE2(min_atom_len);
    } else {
        filter = re2::FilteredRE2();
    }

    // Step 3: Add multiple regex patterns
    int num_patterns = fdp.ConsumeIntegralInRange<int>(0, 8);
    std::vector<int> pattern_ids;
    
    for (int i = 0; i < num_patterns && fdp.remaining_bytes() > 10; i++) {
        // Consume pattern type to create diverse regex patterns
        uint8_t pattern_type = fdp.ConsumeIntegral<uint8_t>() % 6;
        std::string pattern;
        
        switch (pattern_type) {
            case 0:
                // Simple literal pattern
                pattern = fdp.ConsumeRandomLengthString(30);
                break;
            case 1:
                // Pattern with character classes
                pattern = "[" + fdp.ConsumeRandomLengthString(10) + "]";
                break;
            case 2:
                // Pattern with repetition
                pattern = fdp.ConsumeRandomLengthString(15) + "+";
                break;
            case 3:
                // Pattern with alternation
                pattern = fdp.ConsumeRandomLengthString(10) + "|" + 
                          fdp.ConsumeRandomLengthString(10);
                break;
            case 4:
                // Pattern with grouping and capturing
                pattern = "(" + fdp.ConsumeRandomLengthString(15) + ")";
                break;
            case 5:
                // Pattern with wildcards
                pattern = fdp.ConsumeRandomLengthString(10) + ".*" + 
                          fdp.ConsumeRandomLengthString(10);
                break;
        }
        
        int pattern_id = -1;
        re2::RE2::ErrorCode error = filter.Add(pattern, options, &pattern_id);
        
        // Store valid pattern IDs for later use
        if (error == re2::RE2::NoError && pattern_id != -1) {
            pattern_ids.push_back(pattern_id);
        }
    }

    // Step 4: Compile the filter (only if we have valid patterns)
    std::vector<std::string> atoms;
    if (!pattern_ids.empty()) {
        filter.Compile(&atoms);
    }

    // Step 5: Test matching with both FirstMatch and SlowFirstMatch
    if (fdp.remaining_bytes() > 10) {
        std::string text = fdp.ConsumeRandomLengthString(100);
        
        // Test SlowFirstMatch (doesn't require compilation)
        int slow_match = filter.SlowFirstMatch(text);
        
        // Test FirstMatch if we have compiled atoms
        if (!atoms.empty()) {
            // Create atom indices based on consumed data
            std::vector<int> atom_indices;
            int num_atoms_to_test = fdp.ConsumeIntegralInRange<int>(0, static_cast<int>(atoms.size()));
            
            for (int i = 0; i < num_atoms_to_test && fdp.remaining_bytes() > 0; i++) {
                int atom_idx = fdp.ConsumeIntegralInRange<int>(0, static_cast<int>(atoms.size()) - 1);
                atom_indices.push_back(atom_idx);
            }
            
            // Remove duplicates and sort (as might be expected by the API)
            if (!atom_indices.empty()) {
                std::sort(atom_indices.begin(), atom_indices.end());
                atom_indices.erase(std::unique(atom_indices.begin(), atom_indices.end()), 
                                  atom_indices.end());
                
                int first_match = filter.FirstMatch(text, atom_indices);
                (void)first_match; // Use result to avoid unused variable warning
            }
        }
    }

    // Step 6: Query state information using uncovered APIs
    // Get number of regexps
    int num_regexps = filter.NumRegexps();
    (void)num_regexps;
    
    // Test GetRE2 for valid pattern IDs
    for (int id : pattern_ids) {
        if (id >= 0 && id < filter.NumRegexps()) {
            const re2::RE2& re2_obj = filter.GetRE2(id);
            // Access some properties to ensure the object is valid
            bool ok = re2_obj.ok();
            (void)ok;
        }
    }
    
    // Test AllPotentials if we have atoms
    if (!atoms.empty() && fdp.remaining_bytes() > 0) {
        std::vector<int> atom_indices;
        int num_atoms_to_test = fdp.ConsumeIntegralInRange<int>(0, static_cast<int>(atoms.size()));
        
        for (int i = 0; i < num_atoms_to_test && fdp.remaining_bytes() > 0; i++) {
            int atom_idx = fdp.ConsumeIntegralInRange<int>(0, static_cast<int>(atoms.size()) - 1);
            atom_indices.push_back(atom_idx);
        }
        
        if (!atom_indices.empty()) {
            std::vector<int> potential_regexps;
            filter.AllPotentials(atom_indices, &potential_regexps);
        }
    }

    // Step 7: Test edge cases and error paths
    // Test with empty text
    std::string empty_text = "";
    int empty_slow_match = filter.SlowFirstMatch(empty_text);
    (void)empty_slow_match;
    
    // Test FirstMatch with empty atom vector
    if (!atoms.empty() && fdp.remaining_bytes() > 0) {
        std::vector<int> empty_atoms;
        std::string test_text = fdp.ConsumeRandomLengthString(50);
        int empty_first_match = filter.FirstMatch(test_text, empty_atoms);
        (void)empty_first_match;
    }
    
    // Test AllPotentials with empty atom vector
    if (!atoms.empty()) {
        std::vector<int> empty_atoms;
        std::vector<int> empty_potentials;
        filter.AllPotentials(empty_atoms, &empty_potentials);
    }

    return 0;
}

} // namespace harness_004
