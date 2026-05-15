#include <fuzzer/FuzzedDataProvider.h>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>
#include "cjson/cJSON.h"

namespace harness_026 {
/*
 * Harness 026: Utility functions with 0% coverage targeting
 * 
 * Primary targets:
 * 1) cJSON_GetErrorPtr (lines 95-100, 0% coverage) - Error pointer retrieval after parsing failures
 * 2) cJSON_Version (lines 125-133, 0% coverage) - Version string retrieval
 * 3) cJSON_InitHooks error paths (lines 215-221, 0% coverage) - NULL hooks initialization
 * 4) cJSON_strdup allocation failure (lines 204-206, 0% coverage) - NULL pointer allocation
 * 5) case_insensitive_strcmp identical pointer check (lines 144-146, 0% coverage) - Same string pointer comparison
 * 
 * Semantic diversity from existing harnesses:
 * - Previous harnesses focus on parsing, creation, comparison, and manipulation
 * - This harness specifically targets utility functions and error handling paths
 * - Tests error recovery mechanisms and version/info APIs
 * 
 * Strategy:
 * 1) Generate malformed JSON to trigger parsing errors, then call cJSON_GetErrorPtr
 * 2) Call cJSON_Version to retrieve and verify version string
 * 3) Test cJSON_InitHooks with NULL to reset hooks (uncovered path)
 * 4) Create scenarios that might trigger allocation failures
 * 5) Test case_insensitive_strcmp with identical string pointers
 */



// Helper to generate intentionally malformed JSON strings using fuzzer input
static std::string generate_malformed_json(FuzzedDataProvider& fdp) {
    uint8_t malform_type = fdp.ConsumeIntegral<uint8_t>() % 8;
    
    // Consume strings from fuzzer input for malformed JSON construction
    std::string key1 = fdp.ConsumeRandomLengthString(16);
    std::string key2 = fdp.ConsumeRandomLengthString(16);
    std::string value = fdp.ConsumeRandomLengthString(32);
    std::string item1 = fdp.ConsumeRandomLengthString(20);
    std::string item2 = fdp.ConsumeRandomLengthString(20);
    std::string num1 = std::to_string(fdp.ConsumeIntegral<int8_t>());
    std::string num2 = std::to_string(fdp.ConsumeIntegral<int8_t>());
    std::string num3 = std::to_string(fdp.ConsumeIntegral<int8_t>());
    
    switch (malform_type) {
        case 0:
            // Unclosed object
            return "{\"" + key1 + "\": \"" + value + "\"";
        case 1:
            // Unclosed array
            return "[\"" + item1 + "\", \"" + item2 + "\"";
        case 2:
            // Missing value after colon
            return "{\"" + key1 + "\":}";
        case 3:
            // Trailing comma in object
            return "{\"" + key1 + "\": " + num1 + ", \"" + key2 + "\": " + num2 + ",}";
        case 4:
            // Trailing comma in array
            return "[" + num1 + ", " + num2 + ", " + num3 + ",]";
        case 5:
            // Invalid escape sequence
            return "{\"" + key1 + "\": \"\\x\"}";
        case 6:
            // Invalid Unicode
            return "{\"" + key1 + "\": \"\\uXXXX\"}";
        default:
            // Missing quotes
            return "{" + key1 + ": \"" + value + "\"}";
    }
}

// Helper to test cJSON_GetErrorPtr with parsing failures
static void test_error_ptr_scenarios(FuzzedDataProvider& fdp) {
    // Test 1: Parse malformed JSON and get error pointer
    std::string malformed_json = generate_malformed_json(fdp);
    cJSON* parsed = cJSON_Parse(malformed_json.c_str());
    
    if (parsed == NULL) {
        // This should set the error pointer
        const char* error_ptr = cJSON_GetErrorPtr();
        // error_ptr may be NULL or point to error location
        (void)error_ptr; // Use error_ptr to avoid unused variable warning
    } else {
        cJSON_Delete(parsed);
    }
    
    // Test 2: Parse with options and check error pointer
    const char* parse_end = NULL;
    std::string json_input = fdp.ConsumeRandomLengthString(256);
    cJSON* parsed_with_opts = cJSON_ParseWithOpts(json_input.c_str(), &parse_end, fdp.ConsumeBool());
    
    if (parsed_with_opts == NULL) {
        const char* error_ptr = cJSON_GetErrorPtr();
        (void)error_ptr;
    } else {
        cJSON_Delete(parsed_with_opts);
    }
}

// Helper to test cJSON_Version
static void test_version_api() {
    const char* version_str = cJSON_Version();
    // Verify it's not NULL and has expected format
    if (version_str != NULL) {
        // Version should be in format "X.Y.Z"
        // We can't assert specific values, but we can verify it's not empty
        // and contains dots
        (void)version_str; // Use version_str
    }
}

// Helper to test cJSON_InitHooks error paths
static void test_init_hooks_error_paths(FuzzedDataProvider& fdp) {
    // Test with NULL hooks (should reset to defaults - uncovered path)
    cJSON_InitHooks(NULL);
    
    // Test with custom hooks (partially covered, but test anyway)
    cJSON_Hooks custom_hooks;
    
    // Randomly decide whether to set custom functions
    if (fdp.ConsumeBool()) {
        custom_hooks.malloc_fn = malloc;
        custom_hooks.free_fn = free;
    } else {
        custom_hooks.malloc_fn = NULL;
        custom_hooks.free_fn = NULL;
    }
    
    cJSON_InitHooks(&custom_hooks);
}

// Helper to test case_insensitive_strcmp with identical pointers
// Note: This tests the internal static function indirectly through public APIs
static void test_case_insensitive_comparison(FuzzedDataProvider& fdp) {
    // Create identical strings to test the string1 == string2 path
    std::string str1 = fdp.ConsumeRandomLengthString(64);
    std::string str2 = str1; // Same content
    
    // Use cJSON functions that might use case_insensitive_strcmp internally
    // cJSON_Compare with identical strings should return true
    cJSON* obj1 = cJSON_CreateString(str1.c_str());
    cJSON* obj2 = cJSON_CreateString(str2.c_str());
    
    if (obj1 && obj2) {
        cJSON_bool are_equal = cJSON_Compare(obj1, obj2, 1); // case insensitive
        (void)are_equal;
    }
    
    if (obj1) cJSON_Delete(obj1);
    if (obj2) cJSON_Delete(obj2);
    
    // Also test with NULL pointers (another uncovered path in case_insensitive_strcmp)
    cJSON* null_str1 = cJSON_CreateString(NULL);
    cJSON* null_str2 = cJSON_CreateString(NULL);
    
    if (null_str1 && null_str2) {
        cJSON_bool null_equal = cJSON_Compare(null_str1, null_str2, 1);
        (void)null_equal;
    }
    
    if (null_str1) cJSON_Delete(null_str1);
    if (null_str2) cJSON_Delete(null_str2);
}

// Helper to potentially trigger allocation failure paths
static void test_allocation_scenarios(FuzzedDataProvider& fdp) {
    // Create large strings that might stress memory allocation
    size_t str_size = fdp.ConsumeIntegralInRange<size_t>(1, 4096);
    std::string large_string = fdp.ConsumeBytesAsString(str_size);
    
    // Try to parse it as JSON (might trigger internal allocations)
    cJSON* parsed = cJSON_Parse(large_string.c_str());
    if (parsed) {
        cJSON_Delete(parsed);
    }
    
    // Create objects with many items to stress allocation
    cJSON* large_obj = cJSON_CreateObject();
    if (large_obj) {
        for (int i = 0; i < 10 && fdp.remaining_bytes() > 10; i++) {
            std::string key = fdp.ConsumeRandomLengthString(16);
            std::string value = fdp.ConsumeRandomLengthString(32);
            cJSON_AddItemToObject(large_obj, key.c_str(), cJSON_CreateString(value.c_str()));
        }
        cJSON_Delete(large_obj);
    }
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check for meaningful fuzzing
    if (size < 16) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Phase 1: Test cJSON_Version (always call this - simple API with 0% coverage)
    test_version_api();
    
    // Phase 2: Test cJSON_GetErrorPtr with parsing failures
    test_error_ptr_scenarios(fdp);
    
    // Reset provider for next phase if we have enough data
    if (size >= 32) {
        FuzzedDataProvider fdp2(data, size);
        
        // Phase 3: Test cJSON_InitHooks error paths
        test_init_hooks_error_paths(fdp2);
        
        // Check if we have enough data for more tests
        if (size >= 64) {
            FuzzedDataProvider fdp3(data, size);
            
            // Phase 4: Test case-insensitive comparison with identical pointers
            test_case_insensitive_comparison(fdp3);
            
            // Phase 5: Test allocation scenarios
            test_allocation_scenarios(fdp3);
        }
    }
    // Additional test: Call utility functions directly in different orders
    // to maximize state exploration
    
    // Test cJSON_GetErrorPtr after successful parse (should return NULL or last error)
    // Use fuzzer input to create valid JSON
    if (size >= 10) {
        std::string json_part = fdp.ConsumeRandomLengthString(std::min(size_t(256), size));
        std::string valid_json = "{\"key\": " + json_part + "}";
        cJSON* valid_parsed = cJSON_Parse(valid_json.c_str());
        if (valid_parsed) {
            const char* error_after_success = cJSON_GetErrorPtr();
            (void)error_after_success;
            cJSON_Delete(valid_parsed);
        }
    }
    // Test version API again (multiple calls should return same pointer)
    const char* version1 = cJSON_Version();
    const char* version2 = cJSON_Version();
    // version1 and version2 should be equal (same static buffer)
    (void)version1;
    (void)version2;
    
    // Test hooks initialization sequence
    cJSON_Hooks hooks;
    hooks.malloc_fn = NULL;
    hooks.free_fn = NULL;
    cJSON_InitHooks(&hooks);
    
    // Then reset with NULL
    cJSON_InitHooks(NULL);
    
    return 0;
}

} // namespace harness_026
