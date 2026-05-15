#include <cstdint>
#include <cstddef>
#include <cstring>
#include <string>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include "cjson/cJSON.h"

namespace harness_003 {

extern "C" {
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 32) {
        return 0;  // Minimum input size for meaningful testing
    }

    FuzzedDataProvider fdp(data, size);

    // 1. Create root JSON object
    cJSON* root = cJSON_CreateObject();
    if (root == NULL) {
        return 0;
    }

    // Consume various parameters from fuzzer input
    std::string key1 = fdp.ConsumeRandomLengthString(32);
    std::string key2 = fdp.ConsumeRandomLengthString(32);
    std::string key3 = fdp.ConsumeRandomLengthString(32);
    std::string key4 = fdp.ConsumeRandomLengthString(32);
    std::string key5 = fdp.ConsumeRandomLengthString(32);
    
    bool bool_value = fdp.ConsumeBool();
    
    // Consume remaining bytes for raw JSON string
    std::string raw_json = fdp.ConsumeRemainingBytesAsString();
    if (raw_json.empty()) {
        // If no raw data left, use a small default
        raw_json = "{}";
    }

    // 2. Create raw JSON string
    cJSON* raw_item = cJSON_CreateRaw(raw_json.c_str());
    
    // 3. Add raw JSON to object with key name
    if (raw_item != NULL) {
        cJSON_AddRawToObject(root, key1.c_str(), raw_json.c_str());
        // Note: cJSON_AddRawToObject creates its own raw item, so we should delete our raw_item
        cJSON_Delete(raw_item);
    }

    // 4. Create an array
    cJSON* array_item = cJSON_CreateArray();
    
    // 5. Add array to the object
    if (array_item != NULL) {
        cJSON_AddArrayToObject(root, key2.c_str());
        // Note: cJSON_AddArrayToObject creates its own array, so we should delete our array_item
        cJSON_Delete(array_item);
        
        // Get the newly created array to add some items
        cJSON* added_array = cJSON_GetObjectItem(root, key2.c_str());
        if (added_array != NULL && cJSON_IsArray(added_array)) {
            // Add some random numbers to the array
            int array_size = fdp.ConsumeIntegralInRange<int>(0, 5);
            for (int i = 0; i < array_size && fdp.remaining_bytes() > 0; i++) {
                double num = fdp.ConsumeFloatingPoint<double>();
                cJSON_AddItemToArray(added_array, cJSON_CreateNumber(num));
            }
        }
    }

    // 6. Create a boolean value
    cJSON* bool_item = cJSON_CreateBool(bool_value);
    
    // 7. Add boolean to the object
    if (bool_item != NULL) {
        cJSON_AddBoolToObject(root, key3.c_str(), bool_value);
        // Note: cJSON_AddBoolToObject creates its own bool item, so we should delete our bool_item
        cJSON_Delete(bool_item);
    }

    // 8. Create a nested object
    cJSON* nested_obj = cJSON_CreateObject();
    
    // 9. Add nested object to the root object
    if (nested_obj != NULL) {
        cJSON_AddObjectToObject(root, key4.c_str());
        // Note: cJSON_AddObjectToObject creates its own object, so we should delete our nested_obj
        cJSON_Delete(nested_obj);
        
        // Get the newly created nested object to add some items
        cJSON* added_nested = cJSON_GetObjectItem(root, key4.c_str());
        if (added_nested != NULL && cJSON_IsObject(added_nested)) {
            std::string nested_key = fdp.ConsumeRandomLengthString(16);
            std::string nested_value = fdp.ConsumeRandomLengthString(32);
            cJSON_AddStringToObject(added_nested, nested_key.c_str(), nested_value.c_str());
        }
    }

    // 10. Add item reference to object
    // First, get an existing item to reference (the array we added earlier)
    cJSON* existing_item = cJSON_GetObjectItem(root, key2.c_str());
    if (existing_item != NULL) {
        // Add a reference to the existing item
        cJSON_AddItemReferenceToObject(root, key5.c_str(), existing_item);
        
        // Verify the reference was added
        cJSON* referenced_item = cJSON_GetObjectItem(root, key5.c_str());
        // The referenced item should have the reference flag set
        if (referenced_item != NULL) {
            // Check if it has the reference flag (though we can't directly check the flag)
            // Just accessing it is enough for fuzzing
        }
    }

    // 11. Clean up all allocated memory
    cJSON_Delete(root);

    return 0;
}

} // namespace harness_003
