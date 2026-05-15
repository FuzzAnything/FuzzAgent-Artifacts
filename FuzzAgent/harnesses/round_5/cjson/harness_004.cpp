#include <fuzzer/FuzzedDataProvider.h>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <string>
#include <vector>
#include "cjson/cJSON.h"

namespace harness_004 {
/*
 * Harness for cJSON reference creation and boolean type checking operations.
 * 
 * This harness targets 7 completely uncovered APIs representing 24+ branches
 * of undiscovered complexity in the cJSON library:
 * - Reference creation: cJSON_CreateArrayReference, cJSON_CreateObjectReference, 
 *   cJSON_CreateStringReference, cJSON_AddItemReferenceToArray
 * - Boolean type checking: cJSON_IsTrue, cJSON_IsFalse, cJSON_IsInvalid
 * 
 * Implementation follows the coverage guidance invocation sequence:
 * 1. Create invalid item via malformed JSON parsing (for cJSON_IsInvalid)
 * 2. Create boolean items (true/false) for type checking
 * 3. Create base items (array, object, string) for reference creation
 * 4. Create references to base items using reference creation APIs
 * 5. Add reference to array using cJSON_AddItemReferenceToArray
 * 6. Validate boolean types using IsTrue/IsFalse/IsInvalid
 * 7. Proper cleanup of all created items
 */



int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check for meaningful fuzzing
    // Need enough data for JSON strings, boolean values, and operation selection
    if (size < 32) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Part 1: Create invalid JSON item for cJSON_IsInvalid testing
    // ------------------------------------------------------------
    
    // Create deliberately malformed JSON string to trigger parsing failure
    size_t malformed_json_len = fdp.ConsumeIntegralInRange<size_t>(1, 64);
    std::string malformed_json = fdp.ConsumeBytesAsString(malformed_json_len);
    
    // Parse malformed JSON to create potentially invalid item
    cJSON* invalid_item = cJSON_Parse(malformed_json.c_str());
    
    // Part 2: Create boolean items for type checking
    // ----------------------------------------------
    
    // Create explicit true and false items
    cJSON* true_item = cJSON_CreateTrue();
    cJSON* false_item = cJSON_CreateFalse();
    
    // Create boolean with random value from fuzzer input
    bool random_bool = fdp.ConsumeBool();
    cJSON* bool_item = cJSON_CreateBool(random_bool);
    
    // Part 3: Create base items for reference creation
    // ------------------------------------------------
    
    // Create base array with some elements
    cJSON* base_array = cJSON_CreateArray();
    if (base_array != NULL) {
        // Add some items to the base array for reference testing
        size_t array_size = fdp.ConsumeIntegralInRange<size_t>(0, 5);
        for (size_t i = 0; i < array_size; i++) {
            uint8_t elem_type = fdp.ConsumeIntegral<uint8_t>() % 3;
            switch (elem_type) {
                case 0:
                    cJSON_AddItemToArray(base_array, cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>()));
                    break;
                case 1:
                    cJSON_AddItemToArray(base_array, cJSON_CreateString(fdp.ConsumeRandomLengthString(32).c_str()));
                    break;
                case 2:
                    cJSON_AddItemToArray(base_array, cJSON_CreateBool(fdp.ConsumeBool()));
                    break;
            }
        }
    }
    
    // Create base object with some properties
    cJSON* base_object = cJSON_CreateObject();
    if (base_object != NULL) {
        size_t obj_props = fdp.ConsumeIntegralInRange<size_t>(0, 3);
        for (size_t i = 0; i < obj_props; i++) {
            std::string key = fdp.ConsumeRandomLengthString(16);
            uint8_t prop_type = fdp.ConsumeIntegral<uint8_t>() % 2;
            if (prop_type == 0) {
                cJSON_AddNumberToObject(base_object, key.c_str(), fdp.ConsumeFloatingPoint<double>());
            } else {
                cJSON_AddStringToObject(base_object, key.c_str(), fdp.ConsumeRandomLengthString(32).c_str());
            }
        }
    }
    
    // Create base string for string reference
    std::string base_string_content = fdp.ConsumeRandomLengthString(128);
    cJSON* base_string = cJSON_CreateString(base_string_content.c_str());
    
    // Part 4: Create references to base items
    // ---------------------------------------
    
    cJSON* array_ref = NULL;
    cJSON* object_ref = NULL;
    cJSON* string_ref = NULL;
    
    if (base_array != NULL) {
        array_ref = cJSON_CreateArrayReference(base_array);
    }
    
    if (base_object != NULL) {
        object_ref = cJSON_CreateObjectReference(base_object);
    }
    
    if (!base_string_content.empty()) {
        string_ref = cJSON_CreateStringReference(base_string_content.c_str());
    }
    
    // Part 5: Add reference to array using cJSON_AddItemReferenceToArray
    // ------------------------------------------------------------------
    
    // Create a separate array to test adding references
    cJSON* ref_array = cJSON_CreateArray();
    if (ref_array != NULL) {
        // Add various items including references
        if (base_array != NULL) {
            cJSON_AddItemReferenceToArray(ref_array, base_array);
        }
        if (base_object != NULL) {
            cJSON_AddItemReferenceToArray(ref_array, base_object);
        }
        if (base_string != NULL) {
            cJSON_AddItemReferenceToArray(ref_array, base_string);
        }
        
        // Also add some regular items for comparison
        cJSON_AddItemToArray(ref_array, cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>()));
        cJSON_AddItemToArray(ref_array, cJSON_CreateString(fdp.ConsumeRandomLengthString(32).c_str()));
    }
    
    // Part 6: Validate boolean types using IsTrue/IsFalse/IsInvalid
    // -------------------------------------------------------------
    
    // Test cJSON_IsInvalid on the potentially invalid item
    if (invalid_item != NULL) {
        // If parsing succeeded, item is valid (not invalid)
        cJSON_bool is_invalid = cJSON_IsInvalid(invalid_item);
        // Also test on clearly valid items for contrast
        if (true_item != NULL) {
            cJSON_bool true_is_invalid = cJSON_IsInvalid(true_item);
        }
    } else {
        // NULL item is considered invalid
        cJSON_bool null_is_invalid = cJSON_IsInvalid(NULL);
    }
    
    // Test cJSON_IsTrue on various items
    if (true_item != NULL) {
        cJSON_bool is_true = cJSON_IsTrue(true_item);
    }
    if (false_item != NULL) {
        cJSON_bool false_is_true = cJSON_IsTrue(false_item);
    }
    if (bool_item != NULL) {
        cJSON_bool bool_is_true = cJSON_IsTrue(bool_item);
    }
    
    // Test cJSON_IsFalse on various items
    if (false_item != NULL) {
        cJSON_bool is_false = cJSON_IsFalse(false_item);
    }
    if (true_item != NULL) {
        cJSON_bool true_is_false = cJSON_IsFalse(true_item);
    }
    if (bool_item != NULL) {
        cJSON_bool bool_is_false = cJSON_IsFalse(bool_item);
    }
    
    // Also test on non-boolean items
    if (base_string != NULL) {
        cJSON_bool string_is_true = cJSON_IsTrue(base_string);
        cJSON_bool string_is_false = cJSON_IsFalse(base_string);
        cJSON_bool string_is_invalid = cJSON_IsInvalid(base_string);
    }
    
    // Part 7: Clean up all created items
    // ----------------------------------
    
    // Delete all items (references don't own their referenced items,
    // so we need to be careful with deletion order)
    
    // First delete references (they don't own referenced memory)
    if (array_ref != NULL) {
        cJSON_Delete(array_ref);
    }
    if (object_ref != NULL) {
        cJSON_Delete(object_ref);
    }
    if (string_ref != NULL) {
        cJSON_Delete(string_ref);
    }
    
    // Delete the reference array
    if (ref_array != NULL) {
        cJSON_Delete(ref_array);
    }
    
    // Delete base items
    if (base_array != NULL) {
        cJSON_Delete(base_array);
    }
    if (base_object != NULL) {
        cJSON_Delete(base_object);
    }
    if (base_string != NULL) {
        cJSON_Delete(base_string);
    }
    
    // Delete boolean items
    if (true_item != NULL) {
        cJSON_Delete(true_item);
    }
    if (false_item != NULL) {
        cJSON_Delete(false_item);
    }
    if (bool_item != NULL) {
        cJSON_Delete(bool_item);
    }
    
    // Delete invalid item if it was created
    if (invalid_item != NULL) {
        cJSON_Delete(invalid_item);
    }
    
    return 0;
}

} // namespace harness_004
