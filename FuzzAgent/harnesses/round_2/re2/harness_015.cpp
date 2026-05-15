#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
#include <algorithm>
#include <memory>
#include <fuzzer/FuzzedDataProvider.h>
#include "absl/strings/string_view.h"
#include "re2/filtered_re2.h"
#include "re2/re2.h"

namespace harness_015 {
// RE2 FilteredRE2 fuzzing harness - tests multi-regexp filtering functionality
// Comprehensive testing of FilteredRE2 class with prefilter tree operations
// Targets 8+ uncovered public APIs with 31+ accumulated undiscovered branches



int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size: need enough for at least basic operations
    // Need: 1 byte for num_patterns, pattern strings, text, and atoms
    if (size < 32) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume parameters for FilteredRE2 configuration
    bool use_custom_min_atom_len = fdp.ConsumeBool();
    int min_atom_len = 0;
    if (use_custom_min_atom_len) {
        min_atom_len = fdp.ConsumeIntegralInRange<int>(1, 10);
    }
    
    // Create FilteredRE2 instance (tests constructor)
    std::unique_ptr<re2::FilteredRE2> filtered_re2;
    if (use_custom_min_atom_len) {
        filtered_re2 = std::make_unique<re2::FilteredRE2>(min_atom_len);
    } else {
        filtered_re2 = std::make_unique<re2::FilteredRE2>();
    }
    
    // Determine number of regex patterns to add (1-8 patterns for diversity)
    int num_patterns = fdp.ConsumeIntegralInRange<int>(1, 8);
    
    // Vectors to store pattern IDs and RE2::Options
    std::vector<int> pattern_ids;
    std::vector<re2::RE2::Options> options_list;
    
    // Add multiple regex patterns with varying options
    for (int i = 0; i < num_patterns; i++) {
        // Check if we have enough input remaining
        if (fdp.remaining_bytes() < 4) break;
        
        // Consume pattern string (limit length to prevent excessive memory)
        size_t pattern_len = fdp.ConsumeIntegralInRange<size_t>(1, std::min(size_t(128), fdp.remaining_bytes()));
        std::string pattern = fdp.ConsumeBytesAsString(pattern_len);
        
        // Skip empty patterns
        if (pattern.empty()) continue;
        
        // Create RE2::Options with fuzzed values
        re2::RE2::Options options;
        options.set_encoding(fdp.ConsumeBool() ? re2::RE2::Options::EncodingLatin1 
                                             : re2::RE2::Options::EncodingUTF8);
        options.set_posix_syntax(fdp.ConsumeBool());
        options.set_longest_match(fdp.ConsumeBool());
        options.set_log_errors(false);  // Don't log errors during fuzzing
        options.set_max_mem(64 << 20);  // 64MB memory limit
        options.set_literal(fdp.ConsumeBool());
        options.set_never_nl(fdp.ConsumeBool());
        options.set_dot_nl(fdp.ConsumeBool());
        options.set_never_capture(fdp.ConsumeBool());
        options.set_case_sensitive(!fdp.ConsumeBool());  // Invert: true means case-sensitive
        options.set_perl_classes(fdp.ConsumeBool());
        options.set_word_boundary(fdp.ConsumeBool());
        options.set_one_line(fdp.ConsumeBool());
        
        // Store options for later reference
        options_list.push_back(options);
        
        // Add pattern to FilteredRE2
        int pattern_id = -1;
        re2::RE2::ErrorCode error = filtered_re2->Add(pattern, options, &pattern_id);
        
        // Store successful pattern IDs
        if (pattern_id != -1) {
            pattern_ids.push_back(pattern_id);
        }
    }
    
    // If no patterns were successfully added, return early
    if (pattern_ids.empty()) return 0;
    
    // Test SlowFirstMatch before compilation (should work without Compile)
    if (fdp.remaining_bytes() > 0) {
        size_t test_text_len = fdp.ConsumeIntegralInRange<size_t>(1, std::min(size_t(256), fdp.remaining_bytes()));
        std::string test_text = fdp.ConsumeBytesAsString(test_text_len);
        
        if (!test_text.empty()) {
            int slow_match = filtered_re2->SlowFirstMatch(test_text);
            // Result can be -1 (no match) or valid pattern ID
            (void)slow_match; // Use result to prevent optimization
        }
    }
    
    // Test GetRE2() for each successfully added pattern
    for (int id : pattern_ids) {
        if (id >= 0 && id < filtered_re2->NumRegexps()) {
            const re2::RE2& re2_obj = filtered_re2->GetRE2(id);
            // Access some properties to ensure the object is valid
            (void)re2_obj.ok();
            (void)re2_obj.pattern();
        }
    }
    
    // Compile the FilteredRE2 to generate prefilter atoms
    std::vector<std::string> atoms;
    filtered_re2->Compile(&atoms);
    
    // If compilation succeeded and atoms were generated, test filtering operations
    if (!atoms.empty() && fdp.remaining_bytes() > 0) {
        // Create atom indices based on available atoms
        std::vector<int> atom_indices;
        for (size_t i = 0; i < atoms.size() && atom_indices.size() < 10; i++) {
            // Randomly decide whether to include each atom (simulating string matching)
            if (fdp.ConsumeBool()) {
                atom_indices.push_back(static_cast<int>(i));
            }
        }
        
        // Consume text for matching tests
        if (fdp.remaining_bytes() > 0) {
            size_t match_text_len = fdp.ConsumeIntegralInRange<size_t>(1, std::min(size_t(512), fdp.remaining_bytes()));
            std::string match_text = fdp.ConsumeBytesAsString(match_text_len);
            
            if (!match_text.empty() && !atom_indices.empty()) {
                // Test FirstMatch with filtering
                int first_match = filtered_re2->FirstMatch(match_text, atom_indices);
                (void)first_match; // Use result
                
                // Test AllMatches
                std::vector<int> matching_regexps;
                bool has_matches = filtered_re2->AllMatches(match_text, atom_indices, &matching_regexps);
                (void)has_matches; // Use result
                
                // Test AllPotentials
                std::vector<int> potential_regexps;
                filtered_re2->AllPotentials(atom_indices, &potential_regexps);
            }
        }
    }
    
    // Test move semantics (move constructor and move assignment)
    if (!pattern_ids.empty()) {
        // Create a new FilteredRE2 instance via move constructor
        re2::FilteredRE2 moved_filtered_re2(std::move(*filtered_re2));
        
        // Verify the moved object has the patterns
        (void)moved_filtered_re2.NumRegexps();
        
        // Test move assignment
        re2::FilteredRE2 another_filtered_re2;
        another_filtered_re2 = std::move(moved_filtered_re2);
        
        // Verify the assigned object
        (void)another_filtered_re2.NumRegexps();
    }
    
    // Test with different atom configurations (empty atoms, all atoms, random subsets)
    if (!atoms.empty() && fdp.remaining_bytes() > 0) {
        // Test with empty atoms vector
        std::vector<int> empty_atoms;
        std::vector<int> empty_potentials;
        filtered_re2->AllPotentials(empty_atoms, &empty_potentials);
        
        // Test with all atoms
        std::vector<int> all_atoms;
        for (size_t i = 0; i < atoms.size(); i++) {
            all_atoms.push_back(static_cast<int>(i));
        }
        if (all_atoms.size() > 10) {
            all_atoms.resize(10); // Limit size
        }
        
        std::vector<int> all_potentials;
        filtered_re2->AllPotentials(all_atoms, &all_potentials);
        
        // Test with random atom subsets using remaining fuzzer input
        while (fdp.remaining_bytes() > 0 && all_atoms.size() > 0) {
            size_t subset_size = fdp.ConsumeIntegralInRange<size_t>(0, all_atoms.size());
            if (subset_size == 0) break;
            
            std::vector<int> subset_atoms(all_atoms.begin(), all_atoms.begin() + subset_size);
            std::vector<int> subset_potentials;
            filtered_re2->AllPotentials(subset_atoms, &subset_potentials);
            
            // Break if we're running out of input
            if (fdp.remaining_bytes() < sizeof(int)) break;
        }
    }
    
    return 0;
}

} // namespace harness_015
