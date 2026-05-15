#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <string>
#include <vector>
#include <memory>
#include "re2/re2.h"
#include "re2/filtered_re2.h"

namespace harness_001 {
// Copyright 2024 Fuzzing Harness Generator
// Second harness for RE2 library - targeting FilteredRE2 multi-pattern matching and RE2::Extract
// Based on coverage guidance to target 0% covered FilteredRE2 module and RE2::Extract function




int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Require minimum input size for meaningful testing
    // We need enough data for at least 2 patterns, options, text, and operations
    if (size < 32) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Consume basic options for RE2 patterns from fuzzer input
    RE2::Options options;
    options.set_encoding(fdp.ConsumeBool() ? RE2::Options::EncodingLatin1
                                          : RE2::Options::EncodingUTF8);
    options.set_posix_syntax(fdp.ConsumeBool());
    options.set_longest_match(fdp.ConsumeBool());
    options.set_log_errors(false);  // Don't log during fuzzing
    options.set_max_mem(64 << 20);  // 64 MB max memory
    options.set_literal(fdp.ConsumeBool());
    options.set_never_nl(fdp.ConsumeBool());
    options.set_dot_nl(fdp.ConsumeBool());
    options.set_never_capture(fdp.ConsumeBool());
    options.set_case_sensitive(!fdp.ConsumeBool());
    options.set_perl_classes(fdp.ConsumeBool());
    options.set_word_boundary(fdp.ConsumeBool());
    options.set_one_line(fdp.ConsumeBool());
    
    // Determine number of patterns to create (1-4 patterns)
    int num_patterns = fdp.ConsumeIntegralInRange<int>(1, 4);
    
    // Create patterns and RE2 objects
    std::vector<std::unique_ptr<RE2>> re2_objects;
    std::vector<std::string> patterns;
    std::vector<int> pattern_ids;
    
    for (int i = 0; i < num_patterns; i++) {
        // Consume pattern string
        std::string pattern = fdp.ConsumeRandomLengthString(128);
        if (pattern.empty() && fdp.remaining_bytes() > 0) {
            // Try to get at least 1 character for pattern
            pattern = fdp.ConsumeRandomLengthString(1);
        }
        
        patterns.push_back(pattern);
        
        // Create RE2 object
        auto re2 = std::make_unique<RE2>(pattern, options);
        if (re2->ok()) {
            re2_objects.push_back(std::move(re2));
        }
    }
    
    // If no valid RE2 objects were created, return early
    if (re2_objects.empty()) {
        return 0;
    }
    
    // Create FilteredRE2 object
    int min_atom_len = fdp.ConsumeIntegralInRange<int>(1, 8);
    re2::FilteredRE2 filtered_re2(min_atom_len);
    
    // Add patterns to FilteredRE2
    std::vector<int> added_ids;
    for (size_t i = 0; i < patterns.size(); i++) {
        int id = -1;
        RE2::ErrorCode error = filtered_re2.Add(patterns[i], options, &id);
        (void)error;  // Use error to avoid unused variable warning
        if (id != -1) {
            added_ids.push_back(id);
        }
    }
    
    // Compile the FilteredRE2
    std::vector<std::string> atoms;
    filtered_re2.Compile(&atoms);
    
    // Consume text to match against
    std::string text = fdp.ConsumeRandomLengthString(256);
    
    // Create atom indices for filtering (simulate matching some atoms)
    std::vector<int> atom_indices;
    if (!atoms.empty()) {
        // Randomly select some atoms to "match"
        int num_atoms_to_match = fdp.ConsumeIntegralInRange<int>(0, static_cast<int>(atoms.size()));
        for (int i = 0; i < num_atoms_to_match; i++) {
            if (i < static_cast<int>(atoms.size())) {
                atom_indices.push_back(i);
            }
        }
    }
    
    // Test FilteredRE2::FirstMatch if we have atoms
    if (!atom_indices.empty()) {
        int first_match = filtered_re2.FirstMatch(text, atom_indices);
        (void)first_match;
    }
    
    // Test FilteredRE2::AllMatches
    if (!atom_indices.empty()) {
        std::vector<int> matching_regexps;
        bool all_matches_result = filtered_re2.AllMatches(text, atom_indices, &matching_regexps);
        (void)all_matches_result;
    }
    
    // Test FilteredRE2::SlowFirstMatch (doesn't require compile)
    int slow_first_match = filtered_re2.SlowFirstMatch(text);
    (void)slow_first_match;
    
    // Test RE2::Extract functionality
    // Use one of the created RE2 objects for extraction
    if (!re2_objects.empty() && !text.empty()) {
        // Consume rewrite string
        std::string rewrite = fdp.ConsumeRandomLengthString(64);
        
        // Test RE2::CheckRewriteString first
        std::string error;
        bool check_rewrite = re2_objects[0]->CheckRewriteString(rewrite, &error);
        (void)check_rewrite;
        
        // Test RE2::Extract if we have capturing groups
        if (re2_objects[0]->NumberOfCapturingGroups() > 0) {
            std::string extracted;
            bool extract_result = RE2::Extract(text, *re2_objects[0], rewrite, &extracted);
            (void)extract_result;
        }
        
        // Test RE2::MaxSubmatch
        int max_submatch = RE2::MaxSubmatch(rewrite);
        (void)max_submatch;
        
        // Test RE2::Rewrite directly
        if (re2_objects[0]->NumberOfCapturingGroups() > 0) {
            // Create a dummy match for rewrite testing
            absl::string_view dummy_vec[16];
            int nvec = 1 + re2_objects[0]->NumberOfCapturingGroups();
            if (nvec > 16) nvec = 16;
            
            // Try to match to get actual submatch values
            if (re2_objects[0]->Match(text, 0, text.size(), RE2::UNANCHORED, dummy_vec, nvec)) {
                std::string rewrite_output;
                bool rewrite_result = re2_objects[0]->Rewrite(&rewrite_output, rewrite, dummy_vec, nvec);
                (void)rewrite_result;
            }
        }
    }
    
    // Test additional helper APIs
    if (!re2_objects.empty() && !text.empty()) {
        // Test RE2::PartialMatch (used by FilteredRE2::SlowFirstMatch)
        bool partial_match = RE2::PartialMatch(text, *re2_objects[0]);
        (void)partial_match;
        
        // Test RE2::Match directly
        absl::string_view match_vec[4];
        bool match_result = re2_objects[0]->Match(text, 0, text.size(), RE2::UNANCHORED, match_vec, 4);
        (void)match_result;
    }
    
    // Test AllPotentials method
    if (!atom_indices.empty()) {
        std::vector<int> potential_regexps;
        filtered_re2.AllPotentials(atom_indices, &potential_regexps);
    }
    
    // Get number of regexps added
    int num_regexps = filtered_re2.NumRegexps();
    (void)num_regexps;
    
    // Test GetRE2 method if we have added regexps
    if (!added_ids.empty()) {
        const RE2& re2_ref = filtered_re2.GetRE2(added_ids[0]);
        (void)re2_ref;
    }
    
    // Cleanup happens automatically via destructors
    return 0;
}

} // namespace harness_001
