#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <string>
#include <vector>
#include "re2/re2.h"

namespace harness_021 {


// Callback counters for tracking hook invocations
static int dfa_state_cache_reset_count = 0;
static int dfa_search_failure_count = 0;

// Callback function for DFA state cache reset
void DFACacheResetCallback(const re2::hooks::DFAStateCacheReset& info) {
    dfa_state_cache_reset_count++;
    // The callback can access info.state_budget and info.state_cache_size
    // but we don't need to do anything with them for fuzzing
    (void)info; // Avoid unused parameter warning
}

// Callback function for DFA search failure  
void DFASearchFailureCallback(const re2::hooks::DFASearchFailure& info) {
    dfa_search_failure_count++;
    (void)info; // Avoid unused parameter warning
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - we need enough data for patterns and inputs
    if (size < 256) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Strategy: Target RE2 DFA debugging hook operations
    // 1. Create RE2 objects with complex patterns that trigger DFA operations
    // 2. Register DFA state cache reset and search failure hooks
    // 3. Perform regex matching operations to potentially trigger DFA execution
    // 4. Verify hooks were properly set by calling Get*Hook functions
    // 5. Use patterns and inputs designed to stress DFA cache and cause failures
    
    // Step 1: Generate multiple regex patterns that are likely to trigger DFA operations
    std::vector<std::string> patterns;
    std::vector<RE2*> re2_objects;
    
    // Create 3-5 different patterns with varying complexity
    int num_patterns = fdp.ConsumeIntegralInRange<int>(3, 5);
    for (int i = 0; i < num_patterns && fdp.remaining_bytes() > 50; i++) {
        uint8_t pattern_type = fdp.ConsumeIntegral<uint8_t>() % 5;
        std::string pattern;
        
        switch (pattern_type) {
            case 0:
                // Complex pattern with alternation - can stress DFA
                pattern = "(a|b|c|d|e|f|g|h|i|j|k|l|m|n|o|p|q|r|s|t|u|v|w|x|y|z)+";
                break;
            case 1:
                // Pattern with many character classes - DFA intensive
                pattern = "[a-z][A-Z][0-9]\\w\\s\\d\\D\\S\\W.*?";
                break;
            case 2:
                // Pattern with nested quantifiers - can cause DFA state explosion
                pattern = "a{1,10}b{1,10}c{1,10}d{1,10}e{1,10}";
                break;
            case 3:
                // Pattern with lookahead (if supported) - complex DFA
                pattern = "(?=.*[a-z])(?=.*[A-Z])(?=.*[0-9]).{8,}";
                break;
            case 4:
                // Very repetitive pattern - could trigger cache resets
                pattern = "(ab|cd|ef|gh|ij|kl|mn|op|qr|st|uv|wx|yz){10,100}";
                break;
        }
        
        patterns.push_back(pattern);
        
        // Create RE2 object with the pattern
        RE2::Options options;
        options.set_log_errors(false); // Don't log errors during fuzzing
        
        RE2* re2_obj = new RE2(pattern, options);
        if (re2_obj->ok()) {
            re2_objects.push_back(re2_obj);
        } else {
            delete re2_obj;
        }
    }
    
    if (re2_objects.empty()) return 0;
    
    // Reset callback counters
    dfa_state_cache_reset_count = 0;
    dfa_search_failure_count = 0;
    
    // Step 2: Register the DFA debugging hooks
    // Note: These functions should be available based on re2.h declarations
    re2::hooks::SetDFAStateCacheResetHook(DFACacheResetCallback);
    re2::hooks::SetDFASearchFailureHook(DFASearchFailureCallback);
    
    // Step 3: Verify hooks were properly set
    auto* cache_reset_hook = re2::hooks::GetDFAStateCacheResetHook();
    auto* search_failure_hook = re2::hooks::GetDFASearchFailureHook();
    
    // The hooks should point to our callback functions
    // We can't easily compare function pointers in a portable way,
    // but we can at least verify they're not null
    if (cache_reset_hook == nullptr || search_failure_hook == nullptr) {
        // Hook registration failed, but continue fuzzing anyway
        // This might happen if hooks aren't implemented in this build
    }
    
    // Step 4: Generate input strings and perform matching operations
    // to trigger DFA execution and potentially invoke hooks
    
    // Generate multiple test inputs
    std::vector<std::string> test_inputs;
    int num_inputs = fdp.ConsumeIntegralInRange<int>(5, 20);
    
    for (int i = 0; i < num_inputs && fdp.remaining_bytes() > 20; i++) {
        // Generate inputs of varying lengths and content
        uint8_t input_type = fdp.ConsumeIntegral<uint8_t>() % 4;
        std::string input;
        
        switch (input_type) {
            case 0:
                // Long repetitive string - might trigger DFA cache issues
                input = std::string(fdp.ConsumeIntegralInRange<int>(10, 100), 
                                   fdp.ConsumeIntegralInRange<char>('a', 'z'));
                break;
            case 1:
                // Mixed alphanumeric with special characters
                input = fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(5, 200));
                break;
            case 2:
                // Pattern that might match some regexes
                input = "abc123XYZ!@#";
                for (int j = 0; j < fdp.ConsumeIntegralInRange<int>(1, 10); j++) {
                    input += fdp.ConsumeRandomLengthString(5);
                }
                break;
            case 3:
                // Very long string to stress DFA
                input = std::string(500, 'a') + std::string(500, 'b') + 
                       std::string(500, 'c') + std::string(500, 'd');
                break;
        }
        
        test_inputs.push_back(input);
    }
    
    // Step 5: Perform matching operations with different strategies
    // to maximize chances of triggering DFA operations
    
    for (const auto& re2_obj : re2_objects) {
        for (const auto& input : test_inputs) {
            if (fdp.remaining_bytes() == 0) break;
            
            uint8_t match_type = fdp.ConsumeIntegral<uint8_t>() % 3;
            
            switch (match_type) {
                case 0:
                    // FullMatch - exact matching
                    RE2::FullMatch(input, *re2_obj);
                    break;
                case 1:
                    // PartialMatch - substring matching
                    RE2::PartialMatch(input, *re2_obj);
                    break;
                case 2:
                    // FindAndConsume - iterative matching
                    {
                        absl::string_view remaining(input);
                        absl::string_view match;
                        while (RE2::FindAndConsume(&remaining, *re2_obj, &match)) {
                            // Keep consuming matches
                            if (fdp.ConsumeBool()) break; // Randomly stop
                        }
                    }
                    break;
            }
            
            // Also test with StringPiece variants
            absl::string_view sp_input(input);
            RE2::PartialMatch(sp_input, *re2_obj);
        }
    }
    
    // Step 6: Create patterns specifically designed to stress DFA cache
    // These patterns might trigger DFAStateCacheResetHook
    
    if (fdp.remaining_bytes() > 100) {
        // Create a pattern with high state complexity
        std::string complex_pattern;
        int complexity = fdp.ConsumeIntegralInRange<int>(5, 20);
        
        for (int i = 0; i < complexity; i++) {
            char c = 'a' + (i % 26);
            complex_pattern += std::string("[") + c + std::string("]");
            if (i < complexity - 1) {
                complex_pattern += "|";
            }
        }
        
        RE2::Options options;
        options.set_log_errors(false);
        RE2 complex_re(complex_pattern, options);
        
        if (complex_re.ok()) {
            // Test with many different inputs to potentially fill DFA cache
            for (int i = 0; i < 50 && fdp.remaining_bytes() > 10; i++) {
                std::string test_str = fdp.ConsumeRandomLengthString(20);
                RE2::PartialMatch(test_str, complex_re);
            }
        }
    }
    
    // Step 7: Test with patterns that might cause search failures
    // These might trigger DFASearchFailureHook
    
    if (fdp.remaining_bytes() > 50) {
        // Create potentially problematic patterns
        std::vector<std::string> tricky_patterns = {
            "a{1000000,2000000}",  // Very large repetition
            "(.)*",                 // Unbounded repetition
            "a*",                   // Simple but can cause issues
            "[\\s\\S]*",            // Match everything
        };
        
        for (const auto& pattern : tricky_patterns) {
            RE2::Options options;
            options.set_log_errors(false);
            RE2 tricky_re(pattern, options);
            
            if (tricky_re.ok()) {
                // Test with various inputs
                for (int i = 0; i < 10 && fdp.remaining_bytes() > 5; i++) {
                    std::string test_input = fdp.ConsumeRandomLengthString(100);
                    RE2::PartialMatch(test_input, tricky_re);
                }
            }
        }
    }
    
    // Step 8: Verify hook counters changed if hooks were invoked
    // We can't rely on hooks being invoked every time, but we can check
    // if the hook system is working by seeing if our callbacks were called
    
    // The callback counters might have been incremented if DFA operations
    // triggered cache resets or search failures. This is probabilistic.
    
    // Clean up: Delete RE2 objects
    for (auto re2_obj : re2_objects) {
        delete re2_obj;
    }
    
    // Clean up: Unregister hooks (optional, but good practice)
    // Note: The API might not have unregister functions, so we just leave them
    
    return 0;
}

} // namespace harness_021
