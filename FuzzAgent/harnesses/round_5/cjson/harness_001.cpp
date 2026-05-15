#include <fuzzer/FuzzedDataProvider.h>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <string>
#include <vector>
#include "cjson/cJSON.h"
#include "cjson/cJSON_Utils.h"

namespace harness_001 {
/*
 * Harness for cJSON_Utils library - targets JSON utilities including
 * JSON Pointer, JSON Patch, JSON Merge Patch, and sorting functions.
 * 
 * This harness exercises the cJSON_Utils library functions:
 * - JSON Pointer operations (cJSONUtils_GetPointer)
 * - JSON Patch generation and application (cJSONUtils_GeneratePatches, cJSONUtils_ApplyPatches)
 * - JSON Merge Patch operations (cJSONUtils_MergePatch, cJSONUtils_GenerateMergePatch)
 * - Object sorting (cJSONUtils_SortObject)
 * - Pointer finding (cJSONUtils_FindPointerFromObjectTo)
 */



int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check for meaningful fuzzing
    if (size < 64) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Part 1: Create test JSON objects for utilities testing
    // ------------------------------------------------------
    
    // Create first object
    cJSON* obj1 = cJSON_CreateObject();
    if (obj1 == NULL) {
        return 0;
    }
    
    // Add some items to obj1
    std::string key1 = fdp.ConsumeRandomLengthString(16);
    std::string key2 = fdp.ConsumeRandomLengthString(16);
    std::string key3 = fdp.ConsumeRandomLengthString(16);
    
    cJSON_AddNumberToObject(obj1, key1.c_str(), fdp.ConsumeFloatingPoint<double>());
    cJSON_AddStringToObject(obj1, key2.c_str(), fdp.ConsumeRandomLengthString(32).c_str());
    
    // Create nested object
    cJSON* nested = cJSON_CreateObject();
    if (nested != NULL) {
        cJSON_AddBoolToObject(nested, "nested_bool", fdp.ConsumeBool());
        cJSON_AddItemToObject(obj1, key3.c_str(), nested);
    }
    
    // Create second object with some different properties
    cJSON* obj2 = cJSON_CreateObject();
    if (obj2 == NULL) {
        cJSON_Delete(obj1);
        return 0;
    }
    
    std::string key4 = fdp.ConsumeRandomLengthString(16);
    std::string key5 = fdp.ConsumeRandomLengthString(16);
    
    cJSON_AddNumberToObject(obj2, key1.c_str(), fdp.ConsumeFloatingPoint<double>()); // Same key, different value
    cJSON_AddStringToObject(obj2, key4.c_str(), fdp.ConsumeRandomLengthString(32).c_str());
    cJSON_AddNullToObject(obj2, key5.c_str());
    
    // Part 2: JSON Pointer operations
    // -------------------------------
    
    // Test JSON Pointer with various paths
    std::vector<std::string> pointer_paths;
    pointer_paths.push_back("/" + key1);
    pointer_paths.push_back("/" + key2);
    pointer_paths.push_back("/" + key3);
    pointer_paths.push_back("/" + key3 + "/nested_bool");
    pointer_paths.push_back("");
    pointer_paths.push_back("/nonexistent");
    
    for (const auto& path : pointer_paths) {
        // Case insensitive version
        cJSON* pointer_result = cJSONUtils_GetPointer(obj1, path.c_str());
        
        // Case sensitive version (if enough data)
        if (fdp.remaining_bytes() > 10) {
            cJSON* pointer_result_cs = cJSONUtils_GetPointerCaseSensitive(obj1, path.c_str());
        }
    }
    
    // Part 3: JSON Patch operations
    // ------------------------------
    
    // Generate patches between obj1 and obj2
    cJSON* patches = cJSONUtils_GeneratePatches(obj1, obj2);
    cJSON* patches_cs = NULL;
    
    // Generate case sensitive patches if we have enough data
    if (fdp.remaining_bytes() > 20) {
        patches_cs = cJSONUtils_GeneratePatchesCaseSensitive(obj1, obj2);
    }
    
    // Apply patches if they were generated
    if (patches != NULL) {
        // Create a copy of obj1 to apply patches to
        cJSON* obj1_copy = cJSON_Duplicate(obj1, 1);
        if (obj1_copy != NULL) {
            int apply_result = cJSONUtils_ApplyPatches(obj1_copy, patches);
            
            // Try case sensitive application
            if (patches_cs != NULL) {
                cJSON* obj1_copy_cs = cJSON_Duplicate(obj1, 1);
                if (obj1_copy_cs != NULL) {
                    int apply_result_cs = cJSONUtils_ApplyPatchesCaseSensitive(obj1_copy_cs, patches_cs);
                    cJSON_Delete(obj1_copy_cs);
                }
            }
            
            cJSON_Delete(obj1_copy);
        }
        
        // Test adding a patch to an array manually
        cJSON* patch_array = cJSON_CreateArray();
        if (patch_array != NULL) {
            std::string op = "add";
            std::string path = "/new_field";
            cJSON* value = cJSON_CreateString(fdp.ConsumeRandomLengthString(32).c_str());
            
            cJSONUtils_AddPatchToArray(patch_array, op.c_str(), path.c_str(), value);
            
            if (value != NULL) {
                cJSON_Delete(value);
            }
            
            cJSON_Delete(patch_array);
        }
        
        cJSON_Delete(patches);
    }
    
    if (patches_cs != NULL) {
        cJSON_Delete(patches_cs);
    }
    
    // Part 4: JSON Merge Patch operations
    // -----------------------------------
    
    // Create a merge patch
    cJSON* merge_patch = cJSON_CreateObject();
    if (merge_patch != NULL) {
        std::string merge_key1 = fdp.ConsumeRandomLengthString(16);
        std::string merge_key2 = fdp.ConsumeRandomLengthString(16);
        
        cJSON_AddNumberToObject(merge_patch, merge_key1.c_str(), fdp.ConsumeFloatingPoint<double>());
        cJSON_AddStringToObject(merge_patch, merge_key2.c_str(), fdp.ConsumeRandomLengthString(32).c_str());
        
        // Create null value to remove a key (JSON Merge Patch spec)
        cJSON_AddNullToObject(merge_patch, key1.c_str());
        
        // Apply merge patch
        cJSON* merged = cJSONUtils_MergePatch(cJSON_Duplicate(obj1, 1), merge_patch);
        
        // Try case sensitive version
        if (fdp.remaining_bytes() > 10) {
            cJSON* merged_cs = cJSONUtils_MergePatchCaseSensitive(cJSON_Duplicate(obj1, 1), merge_patch);
            if (merged_cs != NULL) {
                cJSON_Delete(merged_cs);
            }
        }
        
        if (merged != NULL) {
            cJSON_Delete(merged);
        }
        
        // Generate merge patch between objects
        cJSON* generated_merge_patch = cJSONUtils_GenerateMergePatch(obj1, obj2);
        cJSON* generated_merge_patch_cs = NULL;
        
        if (fdp.remaining_bytes() > 10) {
            generated_merge_patch_cs = cJSONUtils_GenerateMergePatchCaseSensitive(obj1, obj2);
        }
        
        if (generated_merge_patch != NULL) {
            cJSON_Delete(generated_merge_patch);
        }
        if (generated_merge_patch_cs != NULL) {
            cJSON_Delete(generated_merge_patch_cs);
        }
        
        cJSON_Delete(merge_patch);
    }
    
    // Part 5: Find pointer from object to target
    // ------------------------------------------
    
    // Find pointer from obj1 to nested object (if it exists)
    if (nested != NULL) {
        char* pointer_to_nested = cJSONUtils_FindPointerFromObjectTo(obj1, nested);
        if (pointer_to_nested != NULL) {
            free(pointer_to_nested);
        }
    }
    
    // Find pointer from obj1 to itself (should be empty string)
    char* pointer_to_self = cJSONUtils_FindPointerFromObjectTo(obj1, obj1);
    if (pointer_to_self != NULL) {
        free(pointer_to_self);
    }
    
    // Part 6: Object sorting
    // ----------------------
    
    // Create an unsorted object with random keys
    cJSON* unsorted_obj = cJSON_CreateObject();
    if (unsorted_obj != NULL) {
        // Add items in non-alphabetical order
        cJSON_AddNumberToObject(unsorted_obj, "zebra", 1.0);
        cJSON_AddNumberToObject(unsorted_obj, "apple", 2.0);
        cJSON_AddNumberToObject(unsorted_obj, "banana", 3.0);
        cJSON_AddNumberToObject(unsorted_obj, "123", 4.0);
        cJSON_AddNumberToObject(unsorted_obj, "ABC", 5.0);
        cJSON_AddNumberToObject(unsorted_obj, "abc", 6.0);
        
        // Sort object (case insensitive)
        cJSONUtils_SortObject(unsorted_obj);
        
        // Sort object (case sensitive) - create a new copy
        cJSON* unsorted_obj_cs = cJSON_Duplicate(unsorted_obj, 1);
        if (unsorted_obj_cs != NULL) {
            cJSONUtils_SortObjectCaseSensitive(unsorted_obj_cs);
            cJSON_Delete(unsorted_obj_cs);
        }
        
        cJSON_Delete(unsorted_obj);
    }
    
    // Part 7: Test with array objects
    // --------------------------------
    
    // Create array for testing
    cJSON* test_array = cJSON_CreateArray();
    if (test_array != NULL) {
        for (int i = 0; i < 5 && fdp.remaining_bytes() > 10; i++) {
            cJSON* array_item = cJSON_CreateObject();
            if (array_item != NULL) {
                std::string array_key = fdp.ConsumeRandomLengthString(8);
                cJSON_AddNumberToObject(array_item, array_key.c_str(), fdp.ConsumeFloatingPoint<double>());
                cJSON_AddItemToArray(test_array, array_item);
            }
        }
        
        // Test JSON Pointer on array
        cJSON* array_pointer = cJSONUtils_GetPointer(test_array, "/0");
        cJSON* array_pointer2 = cJSONUtils_GetPointer(test_array, "/1");
        
        if (array_pointer != NULL) {
            // Find pointer to array element
            char* pointer_to_element = cJSONUtils_FindPointerFromObjectTo(test_array, array_pointer);
            if (pointer_to_element != NULL) {
                free(pointer_to_element);
            }
        }
        
        // Clean up
        if (array_pointer != NULL) {
            // pointer result is not owned by us if it points to existing object
        }
        if (array_pointer2 != NULL) {
            // pointer result is not owned by us if it points to existing object
        }
        
        cJSON_Delete(test_array);
    }
    
    // Part 8: Clean up
    // ----------------
    cJSON_Delete(obj1);
    cJSON_Delete(obj2);
    
    return 0;
}

} // namespace harness_001
