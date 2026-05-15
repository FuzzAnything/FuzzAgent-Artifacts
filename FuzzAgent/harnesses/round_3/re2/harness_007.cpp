#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <string>
#include <vector>
#include "re2/re2.h"

namespace harness_007 {
// Fuzzing harness for RE2 LazyRE2 lazy regex compilation and matching
// Targets LazyRE2 lazy compilation lifecycle which is 0% covered according to coverage analysis
// Focuses on lazy compilation rather than immediate compilation for semantic diversity



// Entry point for libFuzzer
int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check: need pattern string and text for matching
    if (size < 32) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Consume operation type to test different LazyRE2 usage patterns
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 4;

    // Choose CannedOptions from fuzzer input
    RE2::CannedOptions canned_opts = RE2::DefaultOptions;
    uint8_t opt_choice = fdp.ConsumeIntegral<uint8_t>() % 4;
    switch (opt_choice) {
        case 0: canned_opts = RE2::DefaultOptions; break;
        case 1: canned_opts = RE2::Latin1; break;
        case 2: canned_opts = RE2::POSIX; break;
        case 3: canned_opts = RE2::Quiet; break;
    }

    // Split input for patterns and text - ensure at least some bytes for each
    size_t remaining_bytes = fdp.remaining_bytes();
    if (remaining_bytes < 3) {
        return 0;
    }
    
    size_t pattern1_len = fdp.ConsumeIntegralInRange<size_t>(0, remaining_bytes / 3);
    size_t pattern2_len = fdp.ConsumeIntegralInRange<size_t>(0, (remaining_bytes - pattern1_len) / 2);
    size_t text_len = remaining_bytes - pattern1_len - pattern2_len;
    
    std::string pattern1 = fdp.ConsumeBytesAsString(pattern1_len);
    std::string pattern2 = fdp.ConsumeBytesAsString(pattern2_len);
    std::string text = fdp.ConsumeBytesAsString(text_len);

    // Test different LazyRE2 usage patterns based on operation
    switch (operation) {
        case 0: {
            // Test 1: Basic LazyRE2 usage with get() and operator->()
            {
                LazyRE2 lazy1 = {pattern1.c_str()};
                
                // Trigger lazy compilation by calling get()
                RE2* re1 = lazy1.get();
                if (re1 != nullptr) {
                    // Check if compilation was successful
                    bool ok1 = re1->ok();
                    if (ok1) {
                        // Use the RE2 object for matching
                        RE2::FullMatch(text, *re1);
                        RE2::PartialMatch(text, *re1);
                    }
                }
                
                // LazyRE2 destructor called here when lazy1 goes out of scope
            }
            
            // Test 2: LazyRE2 with CannedOptions
            {
                LazyRE2 lazy2 = {pattern2.c_str(), canned_opts};
                
                // Use operator->() syntax
                RE2* re2 = lazy2.get();
                if (re2 != nullptr && re2->ok()) {
                    RE2::FullMatch(text, *re2);
                }
                
                // LazyRE2 destructor called here
            }
            break;
        }
        
        case 1: {
            // Test multiple LazyRE2 objects with same pattern but different CannedOptions
            // Test multiple LazyRE2 objects with same pattern but different CannedOptions
            // Use pattern1 if non-empty, otherwise pattern2
            std::string common_pattern;
            if (!pattern1.empty()) {
                common_pattern = pattern1;
            } else if (!pattern2.empty()) {
                common_pattern = pattern2;
            } else {
                // Both patterns empty, skip this test case
                break;
            }
            
            // Create LazyRE2 objects in different scopes to test destructors
            {
                LazyRE2 lazy_default = {common_pattern.c_str(), RE2::DefaultOptions};
                RE2* re_default = lazy_default.get();
                if (re_default != nullptr && re_default->ok()) {
                    RE2::FullMatch(text, *re_default);
                }
            }
            
            {
                LazyRE2 lazy_latin1 = {common_pattern.c_str(), RE2::Latin1};
                RE2* re_latin1 = lazy_latin1.get();
                if (re_latin1 != nullptr && re_latin1->ok()) {
                    RE2::FullMatch(text, *re_latin1);
                }
            }
            
            {
                LazyRE2 lazy_posix = {common_pattern.c_str(), RE2::POSIX};
                RE2* re_posix = lazy_posix.get();
                if (re_posix != nullptr && re_posix->ok()) {
                    RE2::FullMatch(text, *re_posix);
                }
            }
            
            {
                LazyRE2 lazy_quiet = {common_pattern.c_str(), RE2::Quiet};
                RE2* re_quiet = lazy_quiet.get();
                if (re_quiet != nullptr && re_quiet->ok()) {
                    RE2::FullMatch(text, *re_quiet);
                }
            }
            break;
        }
        
        case 2: {
            // Test LazyRE2 with operator*() and complex operations
            LazyRE2 lazy_complex = {pattern1.c_str(), canned_opts};
            
            // Test operator->() chaining and get()
            RE2* re_ptr = lazy_complex.get();
            if (re_ptr != nullptr && re_ptr->ok()) {
                // Test pattern extraction
                std::string extracted_pattern = re_ptr->pattern();
                
                // Test different matching functions
                absl::string_view text_view = text;
                RE2::Consume(&text_view, *re_ptr);
                
                text_view = text;
                RE2::FindAndConsume(&text_view, *re_ptr);
                
                // Test operator*() syntax
                RE2& re_ref = *lazy_complex;
                RE2::FullMatch(text, re_ref);
                RE2::PartialMatch(text, re_ref);
                
                // Test operator->() directly
                RE2::FullMatch(text, *lazy_complex);
                
                // Test program size queries
                int prog_size = re_ptr->ProgramSize();
                int rev_prog_size = re_ptr->ReverseProgramSize();
                
                // Test error code access
                RE2::ErrorCode error_code = re_ptr->error_code();
                std::string error_str = re_ptr->error();
            }
            break;
        }
        
        case 3: {
            // Test LazyRE2 error handling, boundary cases, and repeated access
            // Test empty or very short patterns
            if (!pattern1.empty()) {
                LazyRE2 lazy1 = {pattern1.c_str()};
                
                // Multiple calls to get() should return same pointer
                RE2* re1_first = lazy1.get();
                RE2* re1_second = lazy1.get();
                
                if (re1_first != nullptr && re1_first == re1_second) {
                    if (re1_first->ok()) {
                        RE2::FullMatch(text, *re1_first);
                    }
                }
            }
            
            // Test pattern2 with CannedOptions
            if (!pattern2.empty()) {
                LazyRE2 lazy2 = {pattern2.c_str(), canned_opts};
                RE2* re2 = lazy2.get();
                if (re2 != nullptr && re2->ok()) {
                    // Test various matching operations
                    RE2::FullMatch(text, *re2);
                    RE2::PartialMatch(text, *re2);
                    
                    // Test with different anchor types
                    RE2::Anchor anchor = fdp.ConsumeBool() ? RE2::UNANCHORED : 
                                   fdp.ConsumeBool() ? RE2::ANCHOR_START : RE2::ANCHOR_BOTH;
                    
                    // Simple Match call (not FullMatch/PartialMatch)
                    absl::string_view matches[10];
                    re2->Match(text, 0, text.size(), anchor, matches, 10);
                }
            }
            
            // Test with remaining bytes as additional pattern if available
            if (fdp.remaining_bytes() > 0) {
                std::string extra_pattern = fdp.ConsumeRemainingBytesAsString();
                LazyRE2 lazy_extra = {extra_pattern.c_str()};
                RE2* re_extra = lazy_extra.get();
                if (re_extra != nullptr && re_extra->ok()) {
                    RE2::PartialMatch(text, *re_extra);
                }
            }
            break;
        }
    }

    return 0;
}

} // namespace harness_007
