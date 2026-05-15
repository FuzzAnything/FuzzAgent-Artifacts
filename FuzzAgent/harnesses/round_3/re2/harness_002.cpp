#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <string>
#include <vector>
#include "re2/re2.h"

namespace harness_002 {
// Fuzzing harness for RE2 regex string extraction with rewrite templates
// Targets RE2::Extract and RE2::CheckRewriteString functions which are 0% covered
// with 209+ undiscovered branches according to coverage analysis



// Entry point for libFuzzer
int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check: need pattern, text, rewrite strings, and options
    if (size < 32) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Step 1: Consume fixed-size configuration data first
    // Configure RE2::Options from fuzzer input
    RE2::Options options;
    
    // Set encoding: UTF-8 (default) or Latin1
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

    // Step 2: Consume variable-size strings from remaining input
    // Split remaining input into three parts: pattern, text, rewrite
    size_t remaining = fdp.remaining_bytes();
    if (remaining < 3) {
        return 0;  // Need at least 1 byte for each string
    }
    
    // Allocate bytes for three strings
    size_t pattern_size = remaining / 3;
    size_t text_size = remaining / 3;
    size_t rewrite_size = remaining - pattern_size - text_size;
    
    std::string pattern = fdp.ConsumeBytesAsString(pattern_size);
    std::string text = fdp.ConsumeBytesAsString(text_size);
    std::string rewrite = fdp.ConsumeBytesAsString(rewrite_size);
    
    // If pattern is empty, use a simple default to ensure RE2 object creation
    if (pattern.empty()) {
        pattern = ".*";
    }

    // Step 3: Create RE2 object with fuzzed pattern and options
    RE2 re(pattern, options);
    
    // Skip if pattern compilation failed
    if (!re.ok()) {
        return 0;
    }

    // Don't waste time on very complex patterns during fuzzing
    // Limit program size to prevent timeouts
    int program_size = re.ProgramSize();
    int reverse_program_size = re.ReverseProgramSize();
    if (program_size > 9999 || reverse_program_size > 9999) {
        return 0;
    }

    // Step 4: Validate rewrite string using CheckRewriteString
    std::string rewrite_error;
    bool rewrite_valid = re.CheckRewriteString(rewrite, &rewrite_error);
    
    // Step 5: Perform extraction using Extract (only if rewrite is valid or we want to test invalid cases)
    std::string extracted;
    
    // Test both valid and invalid rewrite strings to explore different code paths
    // Extract should fail gracefully for invalid rewrite strings
    bool extract_success = RE2::Extract(text, re, rewrite, &extracted);
    
    // Step 6: Additional tests with different rewrite patterns
    // Try some common rewrite patterns to explore more code paths
    
    // Test with backreference patterns if we have capturing groups
    if (re.NumberOfCapturingGroups() > 0) {
        std::string extracted2;
        
        // Try different rewrite patterns based on capturing group count
        if (re.NumberOfCapturingGroups() >= 1) {
            RE2::Extract(text, re, "\\1", &extracted2);
        }
        if (re.NumberOfCapturingGroups() >= 2) {
            RE2::Extract(text, re, "\\2-\\1", &extracted2);
        }
        
        // Test with empty rewrite string
        RE2::Extract(text, re, "", &extracted2);
        
        // Test with literal rewrite string (no backreferences) - generate from fuzzer input
        if (fdp.remaining_bytes() > 0) {
            std::string literal_rewrite = fdp.ConsumeRandomLengthString(20);
            RE2::Extract(text, re, literal_rewrite, &extracted2);
        }
    }
    
    // Step 7: Test edge cases with empty strings
    std::string empty_extracted;
    RE2::Extract("", re, rewrite, &empty_extracted);
    
    // Test with pattern that matches empty string
    if (pattern == ".*" || pattern == "") {
        if (fdp.remaining_bytes() > 0) {
            std::string empty_pattern_rewrite = fdp.ConsumeRandomLengthString(10);
            RE2::Extract(text, re, empty_pattern_rewrite, &empty_extracted);
        }
    }
    
    // Step 8: Test CheckRewriteString with various rewrite strings
    // Generate additional rewrite strings from the input to test validation
    if (fdp.remaining_bytes() > 0) {
        std::string additional_rewrite = fdp.ConsumeRandomLengthString(50);
        std::string additional_error;
        re.CheckRewriteString(additional_rewrite, &additional_error);
    }

    return 0;
}

} // namespace harness_002
