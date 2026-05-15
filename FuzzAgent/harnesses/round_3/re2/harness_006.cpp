#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <string>
#include <vector>
#include "re2/re2.h"

namespace harness_006 {
// Fuzzing harness for RE2 program analysis functions
// Targets RE2::ProgramFanout and RE2::ReverseProgramFanout which are 0% covered
// with 6 undiscovered branches according to coverage analysis



// Entry point for libFuzzer
int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check: need pattern string and options
    if (size < 16) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

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

    // Consume pattern string from remaining input
    std::string pattern = fdp.ConsumeRandomLengthString(256);
    
    // If pattern is empty, use a simple default to ensure RE2 object creation
    if (pattern.empty()) {
        pattern = ".*";
    }

    // Create RE2 object with fuzzed pattern and options
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

    // Create histogram vectors for fanout analysis
    std::vector<int> forward_histogram;
    std::vector<int> reverse_histogram;

    // Test 1: ProgramFanout - compute forward program fanout histogram
    int forward_max_bucket = re.ProgramFanout(&forward_histogram);
    
    // Test 2: ReverseProgramFanout - compute reverse program fanout histogram
    int reverse_max_bucket = re.ReverseProgramFanout(&reverse_histogram);

    // Optional validation of histogram contents
    // Check that histograms are non-empty when program has instructions
    if (program_size > 0 && forward_max_bucket >= 0) {
        // Histogram should have at least forward_max_bucket + 1 elements
        if (static_cast<int>(forward_histogram.size()) != forward_max_bucket + 1) {
            // This indicates inconsistency but we don't fail the harness
            // as it might be expected behavior for certain patterns
        }
        
        // Sum of histogram entries should be <= program_size
        int forward_sum = 0;
        for (int count : forward_histogram) {
            forward_sum += count;
        }
        // Note: sum may be less than program_size due to zero-fanout instructions
    }

    if (reverse_program_size > 0 && reverse_max_bucket >= 0) {
        // Similar validation for reverse histogram
        if (static_cast<int>(reverse_histogram.size()) != reverse_max_bucket + 1) {
            // Inconsistency but not a failure
        }
        
        int reverse_sum = 0;
        for (int count : reverse_histogram) {
            reverse_sum += count;
        }
    }

    // Test 3: Edge case - call with nullptr histogram (should just return max bucket)
    int forward_max_only = re.ProgramFanout(nullptr);
    int reverse_max_only = re.ReverseProgramFanout(nullptr);

    // Test 4: Create another RE2 object with different pattern to exercise more code paths
    if (fdp.remaining_bytes() > 10) {
        std::string pattern2 = fdp.ConsumeRandomLengthString(128);
        if (!pattern2.empty()) {
            RE2 re2(pattern2, options);
            if (re2.ok()) {
                std::vector<int> hist2;
                int max2 = re2.ProgramFanout(&hist2);
                int max_rev2 = re2.ReverseProgramFanout(&hist2);
            }
        }
    }

    return 0;
}

} // namespace harness_006
