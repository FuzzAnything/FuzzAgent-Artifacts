#include <fuzzer/FuzzedDataProvider.h>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <string>
#include <vector>
#include "cjson/cJSON.h"

namespace harness_003 {
/*
 * Harness for cJSON object manipulation functions - targets specialized object
 * manipulation functions with 90+ undiscovered branches.
 * 
 * This harness specifically targets object manipulation functions identified
 * in coverage guidance as having high undiscovered branch counts:
 * - cJSON_SetValuestring (20 undiscovered branches)
 * - cJSON_ReplaceItemInObject (10 undiscovered branches)
 * - cJSON_ReplaceItemInObjectCaseSensitive (10 undiscovered branches)
 * - cJSON_AddRawToObject (7 undiscovered branches)
 * - cJSON_AddArrayToObject (6 undiscovered branches)
 * - cJSON_AddFalseToObject (6 undiscovered branches)
 * - cJSON_AddObjectToObject (6 undiscovered branches)
 * - cJSON_AddItemReferenceToObject (4 undiscovered branches)
 * - cJSON_ParseWithLength (16 undiscovered branches)
 * - cJSON_InitHooks (Setup helper)
 * 
 * Implementation follows the object manipulation lifecycle:
 * 1. Hook setup (optional)
 * 2. Object creation (parse or create)
 * 3. Object population (Add*ToObject functions)
 * 4. Reference management (AddItemReferenceToObject)
 * 5. Value modification (SetValuestring)
 * 6. Item replacement (ReplaceItemInObject variants)
 * 7. Cleanup
 */



int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check for meaningful fuzzing of object operations
    // Need enough data for keys, values, strings, and operation selection
    if (size < 64) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Part 1: Optional memory hook setup
    // ----------------------------------
    
    // Randomly decide whether to use custom memory hooks
    bool use_custom_hooks = fdp.ConsumeBool();
    cJSON_Hooks custom_hooks;
    
    if (use_custom_hooks) {
        // Use default hooks (malloc/free)
        cJSON_InitHooks(&custom_hooks);
    }

    // Part 2: Create JSON object from input or create empty object
    // ------------------------------------------------------------
    
    cJSON* root_object = NULL;
    
    // Consume operation choice: 0 = parse from JSON string, 1 = create empty object
    uint8_t creation_method = fdp.ConsumeIntegral<uint8_t>() % 2;
    
    if (creation_method == 0) {
        // Parse JSON from fuzzer input using cJSON_ParseWithLength
        size_t json_length = fdp.ConsumeIntegralInRange<size_t>(1, 256);
        std::string json_input = fdp.ConsumeBytesAsString(json_length);
        
        root_object = cJSON_ParseWithLength(json_input.c_str(), json_input.length());
        
        // If parsing failed, create empty object as fallback
        if (root_object == NULL) {
            root_object = cJSON_CreateObject();
        }
    } else {
        // Create empty object
        root_object = cJSON_CreateObject();
    }
    
    if (root_object == NULL) {
        return 0;
    }

    // Part 3: Generate random keys for object manipulation
    // ----------------------------------------------------
    
    std::vector<std::string> object_keys;
    size_t num_keys = fdp.ConsumeIntegralInRange<size_t>(2, 8);
    
    for (size_t i = 0; i < num_keys; i++) {
        std::string key = fdp.ConsumeRandomLengthString(32);
        object_keys.push_back(key);
    }

    // Part 4: Populate object with various item types using Add*ToObject functions
    // ----------------------------------------------------------------------------
    
    // Create and add string items from fuzzer input (needed for SetValuestring later)
    if (object_keys.size() > 0) {
        std::string string_value1 = fdp.ConsumeRandomLengthString(64);
        cJSON_AddStringToObject(root_object, object_keys[0].c_str(), string_value1.c_str());
    }
    
    if (object_keys.size() > 1) {
        std::string string_value2 = fdp.ConsumeRandomLengthString(64);
        cJSON* string_item2 = cJSON_CreateString(string_value2.c_str());
        if (string_item2 != NULL) {
            cJSON_AddItemToObject(root_object, object_keys[1].c_str(), string_item2);
        }
    }

    // Add raw JSON string using cJSON_AddRawToObject
    if (object_keys.size() > 2) {
        std::string raw_json = fdp.ConsumeRandomLengthString(128);
        cJSON_AddRawToObject(root_object, object_keys[2].c_str(), raw_json.c_str());
    }

    // Add array using cJSON_AddArrayToObject
    if (object_keys.size() > 3) {
        cJSON_AddArrayToObject(root_object, object_keys[3].c_str());
    }

    // Add false boolean using cJSON_AddFalseToObject
    if (object_keys.size() > 4) {
        cJSON_AddFalseToObject(root_object, object_keys[4].c_str());
    }

    // Add nested object using cJSON_AddObjectToObject
    if (object_keys.size() > 5) {
        cJSON* nested_object = cJSON_AddObjectToObject(root_object, object_keys[5].c_str());
        if (nested_object != NULL) {
            // Add something to nested object
            cJSON_AddNumberToObject(nested_object, "nested_value", fdp.ConsumeFloatingPoint<double>());
        }
    }

    // Part 5: Add item references using cJSON_AddItemReferenceToObject
    // ----------------------------------------------------------------
    
    if (object_keys.size() > 6) {
        // Create a standalone item to reference
        cJSON* referenced_item = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
        if (referenced_item != NULL) {
            // Add reference to the object (won't be deleted with root_object)
            cJSON_AddItemReferenceToObject(root_object, object_keys[6].c_str(), referenced_item);
            // Clean up the original (reference still valid in object)
            cJSON_Delete(referenced_item);
        }
    }

    // Part 6: Modify string values using cJSON_SetValuestring
    // -------------------------------------------------------
    
    // Get the string item we added earlier
    if (object_keys.size() > 1) {
        cJSON* existing_string_item = cJSON_GetObjectItem(root_object, object_keys[1].c_str());
        if (existing_string_item != NULL && cJSON_IsString(existing_string_item)) {
            // Generate new string value from fuzzer input
            std::string new_string_value = fdp.ConsumeRandomLengthString(256);
            char* result = cJSON_SetValuestring(existing_string_item, new_string_value.c_str());
            // result contains the old valuestring (if any) or NULL
        }
    }

    // Part 7: Replace items in object using ReplaceItemInObject variants
    // ------------------------------------------------------------------
    
    if (object_keys.size() > 0) {
        // Create replacement items
        cJSON* replacement_string = cJSON_CreateString(fdp.ConsumeRandomLengthString(64).c_str());
        cJSON* replacement_number = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
        
        // Replace using case-insensitive version
        bool replaced_string = false;
        if (replacement_string != NULL) {
            replaced_string = cJSON_ReplaceItemInObject(root_object, object_keys[0].c_str(), replacement_string);
            if (replaced_string) {
                // Item now owned by object, set to NULL to prevent double use
                replacement_string = NULL;
            }
        }
        
        // Replace using case-sensitive version (if we have another key)
        bool replaced_number = false;
        if (object_keys.size() > 3 && replacement_number != NULL) {
            replaced_number = cJSON_ReplaceItemInObjectCaseSensitive(root_object, object_keys[3].c_str(), replacement_number);
            if (replaced_number) {
                // Item now owned by object, set to NULL to prevent double use
                replacement_number = NULL;
            }
        }
        
        // Clean up if replacements weren't successfully used
        if (replacement_string != NULL) {
            cJSON_Delete(replacement_string);
        }
        if (replacement_number != NULL) {
            cJSON_Delete(replacement_number);
        }
    }

    // Part 8: Additional object manipulation with remaining input
    // -----------------------------------------------------------
    
    // Add more items with remaining data if available
    while (fdp.remaining_bytes() > 32) {
        std::string extra_key = fdp.ConsumeRandomLengthString(16);
        uint8_t item_type = fdp.ConsumeIntegral<uint8_t>() % 4;
        
        switch (item_type) {
            case 0:
                cJSON_AddNumberToObject(root_object, extra_key.c_str(), fdp.ConsumeFloatingPoint<double>());
                break;
            case 1:
                cJSON_AddStringToObject(root_object, extra_key.c_str(), fdp.ConsumeRandomLengthString(64).c_str());
                break;
            case 2:
                cJSON_AddBoolToObject(root_object, extra_key.c_str(), fdp.ConsumeBool());
                break;
            case 3:
                cJSON_AddNullToObject(root_object, extra_key.c_str());
                break;
        }
        
        // Break if we're running out of keys or data
        if (fdp.remaining_bytes() < 16) {
            break;
        }
    }

    // Part 9: Cleanup
    // ---------------
    
    // No need to clean up string_item1 separately since it was added to the object
    // and will be deleted with root_object
    
    // Delete the root object (this won't delete referenced items)
    cJSON_Delete(root_object);

    return 0;
}

} // namespace harness_003
