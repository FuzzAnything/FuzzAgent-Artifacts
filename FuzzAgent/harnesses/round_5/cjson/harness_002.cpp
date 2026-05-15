#include <fuzzer/FuzzedDataProvider.h>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <string>
#include <vector>
#include <cmath>
#include "cjson/cJSON.h"

namespace harness_002 {
/*
 * Harness for cJSON array manipulation operations - targets specialized array
 * creation and modification functions with 140+ undiscovered branches.
 * 
 * This harness specifically targets array manipulation functions identified
 * in coverage guidance as having high undiscovered branch counts:
 * - cJSON_CreateIntArray, cJSON_CreateDoubleArray, cJSON_CreateFloatArray
 * - cJSON_InsertItemInArray, cJSON_ReplaceItemInArray
 * - cJSON_DeleteItemFromArray, cJSON_DetachItemFromArray
 * - cJSON_ReplaceItemViaPointer
 * 
 * Implementation follows the lifecycle: Create → Populate → Insert → Replace → Delete/Detach
 */



int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check for meaningful fuzzing of array operations
    // Need enough data for array sizes, indices, and element values
    if (size < 64) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Part 1: Create typed arrays
    // ----------------------------
    
    // Determine array sizes (limit to reasonable values for fuzzing)
    size_t int_array_size = fdp.ConsumeIntegralInRange<size_t>(0, 20);
    size_t double_array_size = fdp.ConsumeIntegralInRange<size_t>(0, 20);
    size_t float_array_size = fdp.ConsumeIntegralInRange<size_t>(0, 20);
    
    // Create typed arrays
    cJSON* int_array = cJSON_CreateIntArray(NULL, int_array_size);
    cJSON* double_array = cJSON_CreateDoubleArray(NULL, double_array_size);
    cJSON* float_array = cJSON_CreateFloatArray(NULL, float_array_size);
    
    // Create a regular array for comparison and manipulation
    cJSON* regular_array = cJSON_CreateArray();
    if (regular_array == NULL) {
        // Clean up typed arrays if regular array creation failed
        if (int_array != NULL) cJSON_Delete(int_array);
        if (double_array != NULL) cJSON_Delete(double_array);
        if (float_array != NULL) cJSON_Delete(float_array);
        return 0;
    }
    
    // Part 2: Populate arrays with elements
    // -------------------------------------
    
    // Populate typed arrays with actual values if they were created
    if (int_array != NULL && int_array_size > 0) {
        std::vector<int> int_values;
        for (size_t i = 0; i < int_array_size; i++) {
            int_values.push_back(fdp.ConsumeIntegral<int>());
        }
        // Recreate array with actual values
        cJSON_Delete(int_array);
        int_array = cJSON_CreateIntArray(int_values.data(), int_array_size);
    }
    
    if (double_array != NULL && double_array_size > 0) {
        std::vector<double> double_values;
        for (size_t i = 0; i < double_array_size; i++) {
            double_values.push_back(fdp.ConsumeFloatingPoint<double>());
        }
        cJSON_Delete(double_array);
        double_array = cJSON_CreateDoubleArray(double_values.data(), double_array_size);
    }
    
    if (float_array != NULL && float_array_size > 0) {
        std::vector<float> float_values;
        for (size_t i = 0; i < float_array_size; i++) {
            float_values.push_back(fdp.ConsumeFloatingPoint<float>());
        }
        cJSON_Delete(float_array);
        float_array = cJSON_CreateFloatArray(float_values.data(), float_array_size);
    }
    
    // Populate regular array with mixed types
    size_t regular_array_size = fdp.ConsumeIntegralInRange<size_t>(2, 10);
    for (size_t i = 0; i < regular_array_size; i++) {
        // Use different element types based on input data
        uint8_t element_type = fdp.ConsumeIntegral<uint8_t>() % 4;
        
        switch (element_type) {
            case 0:
                cJSON_AddItemToArray(regular_array, cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>()));
                break;
            case 1:
                cJSON_AddItemToArray(regular_array, cJSON_CreateString(fdp.ConsumeRandomLengthString(32).c_str()));
                break;
            case 2:
                cJSON_AddItemToArray(regular_array, cJSON_CreateBool(fdp.ConsumeBool()));
                break;
            case 3:
                cJSON_AddItemToArray(regular_array, cJSON_CreateNull());
                break;
        }
    }
    
    // Part 3: Array manipulation operations
    // -------------------------------------
    
    // Get current array size for index calculations
    int current_size = cJSON_GetArraySize(regular_array);
    
    if (current_size > 0) {
        // Test cJSON_InsertItemInArray - insert at various positions
        size_t insert_index = fdp.ConsumeIntegralInRange<size_t>(0, current_size);
        std::string insert_string = fdp.ConsumeRandomLengthString(32);
        cJSON* insert_item = cJSON_CreateString(insert_string.c_str());
        if (insert_item != NULL) {
            cJSON_InsertItemInArray(regular_array, insert_index, insert_item);
        }
        current_size = cJSON_GetArraySize(regular_array);
        
        // Test cJSON_ReplaceItemInArray - replace at valid index
        if (current_size > 0) {
            size_t replace_index = fdp.ConsumeIntegralInRange<size_t>(0, current_size - 1);
            cJSON* replace_item = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
            if (replace_item != NULL) {
                cJSON_ReplaceItemInArray(regular_array, replace_index, replace_item);
            }
        }
        
        // Test cJSON_ReplaceItemViaPointer - replace using pointer
        if (current_size > 0) {
            size_t pointer_index = fdp.ConsumeIntegralInRange<size_t>(0, current_size - 1);
            cJSON* target_item = cJSON_GetArrayItem(regular_array, pointer_index);
            if (target_item != NULL) {
                cJSON* new_item = cJSON_CreateBool(fdp.ConsumeBool());
                if (new_item != NULL) {
                    cJSON_ReplaceItemViaPointer(regular_array, target_item, new_item);
                }
            }
        }
        
        // Test cJSON_DeleteItemFromArray - delete at valid index
        if (current_size > 0) {
            size_t delete_index = fdp.ConsumeIntegralInRange<size_t>(0, current_size - 1);
            cJSON_DeleteItemFromArray(regular_array, delete_index);
        }
        
        // Test cJSON_DetachItemFromArray - detach without deleting
        current_size = cJSON_GetArraySize(regular_array);
        if (current_size > 0) {
            size_t detach_index = fdp.ConsumeIntegralInRange<size_t>(0, current_size - 1);
            cJSON* detached_item = cJSON_DetachItemFromArray(regular_array, detach_index);
            if (detached_item != NULL) {
                // Can use the detached item or delete it
                cJSON_Delete(detached_item);
            }
        }
    }
    
    // Part 4: Additional edge cases and combinations
    // ----------------------------------------------
    
    // Test with empty arrays
    cJSON* empty_array = cJSON_CreateArray();
    if (empty_array != NULL) {
        // These should handle empty/edge cases
        cJSON_InsertItemInArray(empty_array, 0, cJSON_CreateNull());
        cJSON_DeleteItemFromArray(empty_array, 0);  // Index 0 should exist after insertion
        
        // Test detach from potentially invalid index
        cJSON* invalid_detach = cJSON_DetachItemFromArray(empty_array, 5);
        // Should return NULL for invalid index
        
        cJSON_Delete(empty_array);
    }
    
    // Test array of arrays (nested arrays)
    cJSON* nested_arrays = cJSON_CreateArray();
    if (nested_arrays != NULL) {
        for (int i = 0; i < 3 && fdp.remaining_bytes() > 10; i++) {
            cJSON* inner_array = cJSON_CreateArray();
            if (inner_array != NULL) {
                cJSON_AddItemToArray(inner_array, cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>()));
                cJSON_AddItemToArray(inner_array, cJSON_CreateString(fdp.ConsumeRandomLengthString(16).c_str()));
                cJSON_AddItemToArray(nested_arrays, inner_array);
            }
        }
        
        // Manipulate nested arrays
        if (cJSON_GetArraySize(nested_arrays) > 0) {
            cJSON* first_nested = cJSON_GetArrayItem(nested_arrays, 0);
            if (first_nested != NULL && cJSON_GetArraySize(first_nested) > 0) {
                // Replace an item within the nested array
                cJSON* nested_replace = cJSON_CreateBool(fdp.ConsumeBool());
                if (nested_replace != NULL) {
                    cJSON_ReplaceItemInArray(first_nested, 0, nested_replace);
                }
            }
        }
        
        cJSON_Delete(nested_arrays);
    }
    
    // Part 5: Cleanup
    // ---------------
    
    if (int_array != NULL) cJSON_Delete(int_array);
    if (double_array != NULL) cJSON_Delete(double_array);
    if (float_array != NULL) cJSON_Delete(float_array);
    if (regular_array != NULL) cJSON_Delete(regular_array);
    
    return 0;
}

} // namespace harness_002
