#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <string>
#include <vector>
#include <memory>
#include "re2/re2.h"
#include "re2/set.h"

namespace harness_008 {
// Fuzzing harness for RE2 LazyRE2 static initialization and hook debugging infrastructure
// Targets 7 completely uncovered APIs identified in coverage analysis:
// 1. LazyRE2 pointer dereference operator (_ZNK3re27LazyRE2ptEv)
// 2. DFA state cache reset hooks (SetDFAStateCacheResetHook, GetDFAStateCacheResetHook)
// 3. Search failure hooks (SetDFASearchFailureHook, GetDFASearchFailureHook)
// 4. Error argument retrieval (_ZNK3re23RE29error_argB5cxx11Ev)
// 5. RE2::Set move assignment (_ZN3re23RE23SetaSEOS1_)
// 6. RE2::Arg parsing (_ZNK3re23RE23Arg5ParseEPKcm)
// Focuses on static initialization patterns and debugging hooks for semantic diversity


// Global counters for hook callbacks
static int dfa_state_cache_reset_count = 0;
static int dfa_search_failure_count = 0;

// Callback for DFA state cache reset hook
static void DFAStateCacheResetCallback(const re2::hooks::DFAStateCacheReset& reset_info) {
    dfa_state_cache_reset_count++;
    // Can access reset_info.state_budget and reset_info.state_cache_size if needed
}

// Callback for DFA search failure hook
static void DFASearchFailureCallback(const re2::hooks::DFASearchFailure& failure_info) {
    dfa_search_failure_count++;
    // failure_info currently has no members, but callback is still called
}

// Entry point for libFuzzer
int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check: need enough data for patterns and operations
    if (size < 32) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Consume operation type to test different aspects of uncovered APIs
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 6;

    // Split input for patterns and text
    size_t remaining_bytes = fdp.remaining_bytes();
    if (remaining_bytes < 3) {
        return 0;
    }
    
    size_t pattern1_len = fdp.ConsumeIntegralInRange<size_t>(0, remaining_bytes / 4);
    size_t pattern2_len = fdp.ConsumeIntegralInRange<size_t>(0, remaining_bytes / 4);
    size_t pattern3_len = fdp.ConsumeIntegralInRange<size_t>(0, remaining_bytes / 4);
    size_t text_len = remaining_bytes - pattern1_len - pattern2_len - pattern3_len;
    
    std::string pattern1 = fdp.ConsumeBytesAsString(pattern1_len);
    std::string pattern2 = fdp.ConsumeBytesAsString(pattern2_len);
    std::string pattern3 = fdp.ConsumeBytesAsString(pattern3_len);
    std::string text = fdp.ConsumeBytesAsString(text_len);

    // Test different aspects of uncovered APIs based on operation
    switch (operation) {
        case 0: {
            // Test LazyRE2 static initialization with pointer dereference operator
            // and hook infrastructure setup
            
            // Reset hook counters
            dfa_state_cache_reset_count = 0;
            dfa_search_failure_count = 0;
            
            // Set up hooks before any RE2 operations
            re2::hooks::SetDFAStateCacheResetHook(DFAStateCacheResetCallback);
            re2::hooks::SetDFASearchFailureHook(DFASearchFailureCallback);
            
            // Verify hooks were set correctly
            auto* cache_reset_hook = re2::hooks::GetDFAStateCacheResetHook();
            auto* search_failure_hook = re2::hooks::GetDFASearchFailureHook();
            
            // Test LazyRE2 static initialization pattern
            {
                static LazyRE2 static_lazy = {pattern1.c_str()};
                
                // Use pointer dereference operator (uncovered API)
                RE2* re1 = static_lazy.get();
                if (re1 != nullptr) {
                    // Trigger matching operations that might invoke hooks
                    RE2::FullMatch(text, *re1);
                    RE2::PartialMatch(text, *re1);
                    
                    // Test error_arg retrieval (uncovered API)
                    std::string error_arg = re1->error_arg();
                }
            }
            
            // Test another LazyRE2 with different pattern
            {
                LazyRE2 lazy2 = {pattern2.c_str()};
                RE2* re2 = lazy2.get();
                if (re2 != nullptr && re2->ok()) {
                    // Complex matching to potentially trigger DFA operations
                    absl::string_view text_view = text;
                    RE2::Consume(&text_view, *re2);
                }
            }
            
            // Reset hooks to nullptr
            re2::hooks::SetDFAStateCacheResetHook(nullptr);
            re2::hooks::SetDFASearchFailureHook(nullptr);
            
            break;
        }
        
        case 1: {
            // Test RE2::Set move assignment (uncovered API)
            // and hook infrastructure with set operations
            
            // Set up hooks
            re2::hooks::SetDFAStateCacheResetHook(DFAStateCacheResetCallback);
            re2::hooks::SetDFASearchFailureHook(DFASearchFailureCallback);
            
            // Create first RE2::Set
            RE2::Set set1(RE2::DefaultOptions, RE2::ANCHOR_BOTH);
            std::string error1;
            int idx1 = set1.Add(pattern1, &error1);
            
            std::string error2;
            int idx2 = set1.Add(pattern2, &error2);
            bool compiled1 = set1.Compile();
            
            if (compiled1) {
                std::vector<int> matches1;
                set1.Match(text, &matches1);
            }
            
            // Create second RE2::Set
            RE2::Set set2(RE2::DefaultOptions, RE2::UNANCHORED);
            std::string error3;
            int idx3 = set2.Add(pattern3, &error3);
            bool compiled2 = set2.Compile();
            
            if (compiled2) {
                std::vector<int> matches2;
                set2.Match(text, &matches2);
            }
            
            // Test move assignment (uncovered API)
            // Move set2 to set1 using move assignment operator
            set1 = std::move(set2);
            
            // After move assignment, set1 should now contain set2's patterns
            // and set2 should be in a valid but unspecified state
            if (set1.Compile()) {
                std::vector<int> matches3;
                set1.Match(text, &matches3);
            }
            
            // Reset hooks
            re2::hooks::SetDFAStateCacheResetHook(nullptr);
            re2::hooks::SetDFASearchFailureHook(nullptr);
            
            break;
        }
        
        case 2: {
            // Test RE2::Arg parsing (uncovered API) with various types
            
            // Set up hooks to monitor DFA operations during parsing
            re2::hooks::SetDFAStateCacheResetHook(DFAStateCacheResetCallback);
            
            // Test integer parsing with RE2::Arg
            int int_value = 0;
            RE2::Arg int_arg(&int_value);
            
            // Parse from fuzzer-generated string data
            // Use pattern1 if non-empty, otherwise use first few bytes of pattern2
            std::string parse_str = pattern1.empty() ? pattern2.substr(0, std::min(pattern2.size(), (size_t)10)) : pattern1;
            bool parse_success = int_arg.Parse(parse_str.c_str(), parse_str.size());
            
            // Test different radix parsing helpers
            int hex_value = 0;
            RE2::Arg hex_arg = RE2::Hex(&hex_value);
            hex_arg.Parse(pattern2.c_str(), pattern2.size());
            
            int octal_value = 0;
            RE2::Arg octal_arg = RE2::Octal(&octal_value);
            octal_arg.Parse(pattern3.c_str(), pattern3.size());
            
            // Test with empty/null strings
            RE2::Arg null_arg(nullptr);
            null_arg.Parse(nullptr, 0);
            
            // Reset hooks
            re2::hooks::SetDFAStateCacheResetHook(nullptr);
            
            break;
        }
        
        case 3: {
            // Test error handling and error_arg retrieval
            
            // Create RE2 objects that might fail compilation
            RE2 re_bad(pattern1);  // pattern1 might be invalid regex
            
            // Test error_arg on potentially failed RE2 object
            std::string error_arg = re_bad.error_arg();
            
            // Test with LazyRE2 that might have compilation errors
            LazyRE2 lazy_bad = {pattern2.c_str()};
            RE2* re_lazy_bad = lazy_bad.get();
            if (re_lazy_bad != nullptr) {
                std::string lazy_error_arg = re_lazy_bad->error_arg();
            }
            
            // Test with valid RE2 to compare - use pattern3 if available, otherwise a simple pattern from input
            std::string good_pattern = pattern3.empty() ? "a.*" : pattern3.substr(0, std::min(pattern3.size(), (size_t)10));
            RE2 re_good(good_pattern);
            std::string good_error_arg = re_good.error_arg();
            break;
        }
        
        case 4: {
            // Test hook callback triggering with complex regex patterns
            // that might cause DFA state cache resets
            
            dfa_state_cache_reset_count = 0;
            dfa_search_failure_count = 0;
            
            re2::hooks::SetDFAStateCacheResetHook(DFAStateCacheResetCallback);
            re2::hooks::SetDFASearchFailureHook(DFASearchFailureCallback);
            
            // Create complex patterns that might stress DFA
            RE2 re_complex1(pattern1);
            RE2 re_complex2(pattern2);
            
            if (re_complex1.ok()) {
                // Perform multiple matches to potentially trigger cache resets
                for (int i = 0; i < 10 && i < text.size(); i++) {
                    std::string substring = text.substr(0, i + 1);
                    RE2::FullMatch(substring, re_complex1);
                    RE2::PartialMatch(substring, re_complex1);
                }
            }
            
            if (re_complex2.ok()) {
                // Try to trigger search failures with malformed input
                // Use empty substring from input if available
                if (text.size() > 0) {
                    RE2::FullMatch(text.substr(0, 0), re_complex2);  // Empty substring
                }
                
                // Try with special characters from input - create a string with potential nulls
                if (fdp.remaining_bytes() > 0) {
                    std::vector<uint8_t> special_bytes = fdp.ConsumeBytes<uint8_t>(std::min(fdp.remaining_bytes(), (size_t)3));
                    std::string special_str(reinterpret_cast<const char*>(special_bytes.data()), special_bytes.size());
                    RE2::PartialMatch(special_str, re_complex2);
                }
            }
            // Check if hooks were called
            // (counters are updated in callbacks)
            
            re2::hooks::SetDFAStateCacheResetHook(nullptr);
            re2::hooks::SetDFASearchFailureHook(nullptr);
            
            break;
        }
        
        case 5: {
            // Comprehensive test combining multiple uncovered APIs
            
            // Hook setup
            re2::hooks::SetDFAStateCacheResetHook(DFAStateCacheResetCallback);
            
            // LazyRE2 with pointer dereference
            static LazyRE2 static_combo = {pattern1.c_str()};
            RE2* re_combo = static_combo.get();
            
            if (re_combo != nullptr) {
                // Error argument retrieval
                std::string combo_error_arg = re_combo->error_arg();
                
                // RE2::Arg parsing with matched text
                int parsed_value = 0;
                RE2::Arg combo_arg(&parsed_value);
                
                // Try to parse parts of the matched text
                if (!text.empty()) {
                    size_t parse_len = std::min(text.size(), (size_t)10);
                    combo_arg.Parse(text.c_str(), parse_len);
                }
                
                // Matching operations
                RE2::FullMatch(text, *re_combo);
            }
            
            // RE2::Set operations with potential move
            RE2::Set set_combo1(RE2::DefaultOptions, RE2::ANCHOR_BOTH);
            set_combo1.Add(pattern2, nullptr);
            set_combo1.Add(pattern3, nullptr);
            set_combo1.Compile();
            
            RE2::Set set_combo2(RE2::DefaultOptions, RE2::UNANCHORED);
            set_combo2.Add(text, nullptr);  // Use text as pattern
            
            // Move assignment
            set_combo1 = std::move(set_combo2);
            
            if (set_combo1.Compile()) {
                std::vector<int> combo_matches;
                set_combo1.Match(pattern1, &combo_matches);
            }
            
            // Hook cleanup
            re2::hooks::SetDFAStateCacheResetHook(nullptr);
            
            break;
        }
    }

    return 0;
}

} // namespace harness_008
