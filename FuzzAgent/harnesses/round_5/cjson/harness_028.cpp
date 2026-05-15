#include <fuzzer/FuzzedDataProvider.h>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include "cjson/cJSON.h"

namespace harness_028 {
/*
 * Harness 028: Constant string key object addition and typed array creation with actual data
 * 
 * Primary targets with 0% coverage:
 * 1) cJSON_AddItemToObjectCS (lines 2128-2131) - Adds items to objects with constant string keys
 * 2) cJSON_CreateIntArray main logic (lines 2640-2666) - Array creation with actual data (currently only NULL/negative count check is covered)
 * 3) cJSON_CreateFloatArray main logic (lines 2677-2706) - Array creation with actual data
 * 4) cJSON_CreateDoubleArray main logic (lines 2708-2737) - Array creation with actual data
 * 
 * Coverage analysis from existing harnesses:
 * - harness_002 calls typed array functions but with NULL pointers first, then recreates
 * - According to coverage data, the actual array creation logic (for loops, child linking) has 0 hits
 * - cJSON_AddItemToObjectCS has never been called (0 hits)
 * 
 * Semantic diversity from existing 27 harnesses:
 * - harness_002 tests array functions but doesn't use cJSON_AddItemToObjectCS
 * - No harness tests constant string key addition (cJSON_AddItemToObjectCS vs regular cJSON_AddItemToObject)
 * - This harness focuses on combining constant string keys with typed array creation
 * - Tests actual data population in typed arrays (not just NULL pointer cases)
 * 
 * Key differences between cJSON_AddItemToObject and cJSON_AddItemToObjectCS:
 * - cJSON_AddItemToObject: duplicates the string key (allocates new memory)
 * - cJSON_AddItemToObjectCS: assumes string is constant (no duplication, marked with cJSON_StringIsConst flag)
 * - The "CS" version is more efficient for static/constant string keys
 * 
 * Strategy:
 * 1) Create objects and use cJSON_AddItemToObjectCS with constant/literal string keys
 * 2) Create typed arrays (int, float, double) with actual data from fuzzer input
 * 3) Add created arrays to objects using constant string keys
 * 4) Test both success and failure paths (negative counts, NULL pointers)
 * 5) Test with various array sizes and data values
 */



int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check: need enough data for array sizes, indices, and values
    if (size < 64) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Part 1: Test typed array creation with actual data (not just NULL pointers)
    // --------------------------------------------------------------------------
    
    // Determine array sizes (limit to reasonable values for fuzzing)
    int int_array_count = fdp.ConsumeIntegralInRange<int>(0, 20);
    int float_array_count = fdp.ConsumeIntegralInRange<int>(0, 20);
    int double_array_count = fdp.ConsumeIntegralInRange<int>(0, 20);
    
    // Test 1: Create int array with actual data (main logic path)
    cJSON* int_array = NULL;
    if (int_array_count > 0) {
        std::vector<int> int_values;
        for (int i = 0; i < int_array_count; i++) {
            int_values.push_back(fdp.ConsumeIntegral<int>());
        }
        // This should hit the main creation logic (for loop, child linking)
        int_array = cJSON_CreateIntArray(int_values.data(), int_array_count);
    } else {
        // Test with 0 count (should return empty array)
        int_array = cJSON_CreateIntArray(NULL, 0);
    }
    
    // Test 2: Create float array with actual data
    cJSON* float_array = NULL;
    if (float_array_count > 0) {
        std::vector<float> float_values;
        for (int i = 0; i < float_array_count; i++) {
            float_values.push_back(fdp.ConsumeFloatingPoint<float>());
        }
        float_array = cJSON_CreateFloatArray(float_values.data(), float_array_count);
    } else {
        float_array = cJSON_CreateFloatArray(NULL, 0);
    }
    
    // Test 3: Create double array with actual data
    cJSON* double_array = NULL;
    if (double_array_count > 0) {
        std::vector<double> double_values;
        for (int i = 0; i < double_array_count; i++) {
            double_values.push_back(fdp.ConsumeFloatingPoint<double>());
        }
        double_array = cJSON_CreateDoubleArray(double_values.data(), double_array_count);
    } else {
        double_array = cJSON_CreateDoubleArray(NULL, 0);
    }
    
    // Test 4: Negative count cases (error paths)
    int negative_count = fdp.ConsumeIntegralInRange<int>(-10, -1);
    cJSON* negative_int_array = cJSON_CreateIntArray(NULL, negative_count);
    cJSON* negative_float_array = cJSON_CreateFloatArray(NULL, negative_count);
    cJSON* negative_double_array = cJSON_CreateDoubleArray(NULL, negative_count);
    // These should all return NULL
    
    // Part 2: Test cJSON_AddItemToObjectCS with constant string keys
    // --------------------------------------------------------------
    
    // Create objects to test constant string key addition
    cJSON* main_object = cJSON_CreateObject();
    cJSON* nested_object = cJSON_CreateObject();
    
    if (main_object && nested_object) {
        // Test 1: Add various items using constant string keys
        // Using string literals as constant strings (won't be duplicated)
        cJSON_bool success;
        
        // Add primitive values with constant string keys
        success = cJSON_AddItemToObjectCS(main_object, "int_key", cJSON_CreateNumber(fdp.ConsumeIntegral<int>()));
        success = cJSON_AddItemToObjectCS(main_object, "float_key", cJSON_CreateNumber(fdp.ConsumeFloatingPoint<float>()));
        success = cJSON_AddItemToObjectCS(main_object, "bool_key", cJSON_CreateBool(fdp.ConsumeBool()));
        
        // Add strings with constant string keys
        std::string dynamic_string = fdp.ConsumeRandomLengthString(50);
        success = cJSON_AddItemToObjectCS(main_object, "string_key", cJSON_CreateString(dynamic_string.c_str()));
        
        // Add arrays (created above) with constant string keys
        if (int_array) {
            success = cJSON_AddItemToObjectCS(main_object, "int_array_key", int_array);
            // Don't delete int_array here - ownership transferred to main_object
            int_array = NULL; // Prevent double deletion
        }
        
        if (float_array) {
            success = cJSON_AddItemToObjectCS(main_object, "float_array_key", float_array);
            float_array = NULL;
        }
        
        if (double_array) {
            success = cJSON_AddItemToObjectCS(main_object, "double_array_key", double_array);
            double_array = NULL;
        }
        
        // Test 2: Add nested object with constant string key
        // Populate nested object
        success = cJSON_AddItemToObjectCS(nested_object, "nested_int", cJSON_CreateNumber(fdp.ConsumeIntegral<int>()));
        success = cJSON_AddItemToObjectCS(nested_object, "nested_string", cJSON_CreateString(fdp.ConsumeRandomLengthString(30).c_str()));
        
        // Add nested object to main object with constant string key
        success = cJSON_AddItemToObjectCS(main_object, "nested_object_key", nested_object);
        nested_object = NULL; // Ownership transferred
        
        // Test 3: Mix cJSON_AddItemToObject and cJSON_AddItemToObjectCS for comparison
        // Create another object for mixing APIs
        cJSON* mixed_object = cJSON_CreateObject();
        if (mixed_object) {
            // Add with regular AddItemToObject (duplicates key)
            std::string regular_key = "regular_" + fdp.ConsumeRandomLengthString(10);
            success = cJSON_AddItemToObject(mixed_object, regular_key.c_str(), cJSON_CreateNumber(fdp.ConsumeIntegral<int>()));
            
            // Add with AddItemToObjectCS (constant key)
            success = cJSON_AddItemToObjectCS(mixed_object, "constant_key", cJSON_CreateBool(fdp.ConsumeBool()));
            
            // Add mixed object to main object
            success = cJSON_AddItemToObjectCS(main_object, "mixed_object_key", mixed_object);
            mixed_object = NULL;
        }
        
        // Test 4: Error cases for cJSON_AddItemToObjectCS
        // NULL object or string should return false
        cJSON* temp_item = cJSON_CreateNumber(42);
        if (temp_item) {
            // NULL object
            success = cJSON_AddItemToObjectCS(NULL, "test_key", temp_item);
            // NULL string
            success = cJSON_AddItemToObjectCS(main_object, NULL, temp_item);
            // NULL item (though function expects non-NULL item)
            success = cJSON_AddItemToObjectCS(main_object, "null_item_key", NULL);
            
            cJSON_Delete(temp_item);
        }
        
        // Test 5: Self-reference (object == item) - should fail
        success = cJSON_AddItemToObjectCS(main_object, "self_ref_key", main_object);
        // Should return false, item not added
        
        // Clean up main object (will delete all contained items)
        cJSON_Delete(main_object);
    }
    
    // Part 3: Clean up any arrays that weren't added to objects
    // ---------------------------------------------------------
    if (int_array) cJSON_Delete(int_array);
    if (float_array) cJSON_Delete(float_array);
    if (double_array) cJSON_Delete(double_array);
    if (nested_object) cJSON_Delete(nested_object);
    
    // Negative test arrays should already be NULL, but check
    if (negative_int_array) cJSON_Delete(negative_int_array);
    if (negative_float_array) cJSON_Delete(negative_float_array);
    if (negative_double_array) cJSON_Delete(negative_double_array);
    
    return 0;
}

} // namespace harness_028
