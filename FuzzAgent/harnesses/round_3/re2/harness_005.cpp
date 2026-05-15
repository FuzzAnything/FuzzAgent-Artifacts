#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <string>
#include <vector>
#include "re2/re2.h"

namespace harness_005 {
// Fuzzing harness for RE2 string replacement operations with comprehensive coverage
// Targets: RE2::Rewrite, RE2::GlobalReplace, RE2::Replace with complex rewrite patterns
// Focus: Backreferences (\1, \2), captured submatches, edge cases, different RE2::Options
// Goal: Cover 317+ uncovered branches across these three related APIs



// Entry point for libFuzzer
int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check: need pattern, text, rewrite strings, and options
    if (size < 32) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // ==================== STEP 1: CONFIGURE RE2 OPTIONS ====================
    RE2::Options options;
    
    // Set encoding from fuzzer input
    options.set_encoding(fdp.ConsumeBool() ? RE2::Options::EncodingLatin1 
                                           : RE2::Options::EncodingUTF8);
    
    // Set various option flags from fuzzer input
    options.set_posix_syntax(fdp.ConsumeBool());
    options.set_longest_match(fdp.ConsumeBool());
    options.set_log_errors(false);  // Don't log errors during fuzzing
    options.set_max_mem(64 << 20);  // 64 MB memory limit
    options.set_literal(fdp.ConsumeBool());
    options.set_never_nl(fdp.ConsumeBool());
    options.set_dot_nl(fdp.ConsumeBool());
    options.set_never_capture(fdp.ConsumeBool());
    options.set_case_sensitive(!fdp.ConsumeBool());  // Invert: default is true
    options.set_perl_classes(fdp.ConsumeBool());
    options.set_word_boundary(fdp.ConsumeBool());
    options.set_one_line(fdp.ConsumeBool());

    // Optionally set maximum global replace count for fuzzing safety
    int max_replace_count = fdp.ConsumeIntegralInRange<int>(-1, 100);
    RE2::FUZZING_ONLY_set_maximum_global_replace_count(max_replace_count);

    // ==================== STEP 2: CONSUME PATTERNS AND TEXT ====================
    // Split remaining input for pattern, text, and rewrite strings
    size_t remaining = fdp.remaining_bytes();
    if (remaining < 12) {
        return 0;  // Need at least some bytes for strings
    }
    
    // Allocate bytes: pattern (30%), text (40%), rewrite (30%)
    size_t pattern_size = (remaining * 3) / 10;
    size_t text_size = (remaining * 4) / 10;
    size_t rewrite_size = remaining - pattern_size - text_size;
    
    std::string pattern = fdp.ConsumeBytesAsString(pattern_size);
    std::string text = fdp.ConsumeBytesAsString(text_size);
    std::string rewrite = fdp.ConsumeBytesAsString(rewrite_size);
    
    // If pattern is empty, use a simple default with capture groups
    if (pattern.empty()) {
        pattern = "(\\w+)\\s+(\\w+)";  // Two capture groups
    }

    // ==================== STEP 3: CREATE RE2 OBJECT ====================
    RE2 re(pattern, options);
    
    // Skip if pattern compilation failed
    if (!re.ok()) {
        return 0;
    }

    // Limit program size to prevent timeouts
    int program_size = re.ProgramSize();
    int reverse_program_size = re.ReverseProgramSize();
    if (program_size > 9999 || reverse_program_size > 9999) {
        return 0;
    }

    // ==================== STEP 4: TEST REWRITE FUNCTION ====================
    // Rewrite requires a submatch vector from a successful match
    // First, try to match the pattern against the text
    int num_captures = re.NumberOfCapturingGroups();
    std::vector<absl::string_view> submatch_vec;
    
    if (num_captures > 0) {
        // Allocate vector for submatches (including the whole match at index 0)
        submatch_vec.resize(num_captures + 1);
        
        // Try to match
        if (re.Match(text, 0, text.size(), RE2::UNANCHORED, 
                     submatch_vec.data(), submatch_vec.size())) {
            
            // Test 4A: Rewrite with the captured submatches
            std::string rewrite_output;
            bool rewrite_success = re.Rewrite(&rewrite_output, rewrite, 
                                             submatch_vec.data(), submatch_vec.size());
            
            // Test 4B: Rewrite with empty rewrite string
            if (!rewrite.empty()) {
                std::string empty_rewrite_output;
                re.Rewrite(&empty_rewrite_output, "", 
                          submatch_vec.data(), submatch_vec.size());
            }
            
            // Test 4C: Rewrite with backreference patterns
            if (num_captures >= 1) {
                std::string backref_output;
                re.Rewrite(&backref_output, "\\1", 
                          submatch_vec.data(), submatch_vec.size());
            }
            if (num_captures >= 2) {
                std::string backref_output2;
                re.Rewrite(&backref_output2, "\\2 and \\1", 
                          submatch_vec.data(), submatch_vec.size());
            }
            if (num_captures >= 3) {
                std::string backref_output3;
                re.Rewrite(&backref_output3, "\\3-\\2-\\1", 
                          submatch_vec.data(), submatch_vec.size());
            }
        }
    }

    // ==================== STEP 5: TEST REPLACE FUNCTION ====================
    // Test 5A: Replace with the fuzzed rewrite string
    std::string replace_text = text;
    bool replace_success = RE2::Replace(&replace_text, re, rewrite);
    
    // Test 5B: Replace with empty rewrite string
    std::string empty_replace_text = text;
    RE2::Replace(&empty_replace_text, re, "");
    
    // Test 5C: Replace with backreference rewrite strings
    if (num_captures >= 1) {
        std::string backref_replace_text = text;
        RE2::Replace(&backref_replace_text, re, "\\1");
    }
    if (num_captures >= 2) {
        std::string backref_replace_text2 = text;
        RE2::Replace(&backref_replace_text2, re, "\\2");
    }
    if (num_captures >= 3) {
        std::string backref_replace_text3 = text;
        RE2::Replace(&backref_replace_text3, re, "\\1-\\2-\\3");
    }

    // ==================== STEP 6: TEST GLOBALREPLACE FUNCTION ====================
    // Test 6A: GlobalReplace with the fuzzed rewrite string
    std::string global_replace_text = text;
    int global_replace_count = RE2::GlobalReplace(&global_replace_text, re, rewrite);
    
    // Test 6B: GlobalReplace with empty rewrite string
    std::string empty_global_replace_text = text;
    RE2::GlobalReplace(&empty_global_replace_text, re, "");
    
    // Test 6C: GlobalReplace with backreference rewrite strings
    if (num_captures >= 1) {
        std::string backref_global_text = text;
        RE2::GlobalReplace(&backref_global_text, re, "\\1");
    }
    if (num_captures >= 2) {
        std::string backref_global_text2 = text;
        RE2::GlobalReplace(&backref_global_text2, re, "\\2");
    }
    if (num_captures >= 3) {
        std::string backref_global_text3 = text;
        RE2::GlobalReplace(&backref_global_text3, re, "\\1 and \\2 and \\3");
    }

    // ==================== STEP 7: TEST EDGE CASES ====================
    // Test with empty strings
    std::string empty_string = "";
    RE2::Replace(&empty_string, re, rewrite);
    RE2::GlobalReplace(&empty_string, re, rewrite);
    
    // Test with pattern that matches empty string
    RE2 empty_pattern("", options);
    if (empty_pattern.ok()) {
        std::string text_copy = text;
        RE2::Replace(&text_copy, empty_pattern, rewrite);
        RE2::GlobalReplace(&text_copy, empty_pattern, rewrite);
    }
    
    // Test with rewrite strings containing special characters
    std::string special_rewrite = "\\\\\"\t\n\r";
    std::string special_replace_text = text;
    RE2::Replace(&special_replace_text, re, special_rewrite);
    RE2::GlobalReplace(&special_replace_text, re, special_rewrite);

    // ==================== STEP 8: TEST CHECKREWRITESTRING ====================
    // Validate rewrite string using CheckRewriteString
    std::string rewrite_error;
    bool rewrite_valid = re.CheckRewriteString(rewrite, &rewrite_error);
    
    // Test CheckRewriteString with various rewrite patterns
    if (num_captures >= 1) {
        re.CheckRewriteString("\\1", &rewrite_error);
    }
    if (num_captures >= 2) {
        re.CheckRewriteString("\\2\\1", &rewrite_error);
    }
    
    // Test with invalid backreferences (should fail)
    re.CheckRewriteString("\\999", &rewrite_error);
    re.CheckRewriteString("\\", &rewrite_error);
    re.CheckRewriteString("\\a", &rewrite_error);

    // ==================== STEP 9: TEST WITH DIFFERENT PATTERNS ====================
    // Create additional RE2 objects with different patterns to explore more code paths
    
    // Pattern with many capture groups
    if (fdp.remaining_bytes() > 20) {
        std::string multi_capture_pattern = "(" + fdp.ConsumeRandomLengthString(10) + 
                                           ")-(" + fdp.ConsumeRandomLengthString(10) + 
                                           ")-(" + fdp.ConsumeRandomLengthString(10) + ")";
        RE2 re2(multi_capture_pattern, options);
        if (re2.ok()) {
            std::string text2 = fdp.ConsumeRandomLengthString(50);
            std::string rewrite2 = "\\3-\\2-\\1";
            
            std::string replace_text2 = text2;
            RE2::Replace(&replace_text2, re2, rewrite2);
            
            std::string global_text2 = text2;
            RE2::GlobalReplace(&global_text2, re2, rewrite2);
        }
    }

    // Pattern with no capture groups
    RE2 no_capture_re("\\w+", options);
    if (no_capture_re.ok()) {
        std::string no_cap_text = text;
        RE2::Replace(&no_cap_text, no_capture_re, "REPLACED");
        RE2::GlobalReplace(&no_cap_text, no_capture_re, "REPLACED");
    }

    return 0;
}

} // namespace harness_005
