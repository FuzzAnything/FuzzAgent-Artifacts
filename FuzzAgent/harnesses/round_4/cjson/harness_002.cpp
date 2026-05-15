#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include "cjson/cJSON.h"

namespace harness_002 {


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check for meaningful object manipulation testing
    // Need enough data for: operation type, object count, key names, values, etc.
    if (size < 64) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Determine the main operation to test
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 6;
    
    // Create one or two objects for manipulation and comparison
    cJSON* obj1 = cJSON_CreateObject();
    cJSON* obj2 = NULL;
    
    if (obj1 == NULL) {
        return 0;
    }

    // Generate some random key names for testing
    std::vector<std::string> keys;
    int num_keys = fdp.ConsumeIntegralInRange<int>(2, 8);
    for (int i = 0; i < num_keys; i++) {
        size_t key_len = fdp.ConsumeIntegralInRange<size_t>(1, 32);
        if (fdp.remaining_bytes() < key_len) {
            cJSON_Delete(obj1);
            return 0;
        }
        std::string key = fdp.ConsumeBytesAsString(key_len);
        keys.push_back(key);
    }

    // Phase 1: Add various items to object based on operation
    switch (operation) {
        case 0: {
            // Test basic AddItemToObject variants
            for (size_t i = 0; i < keys.size() && fdp.remaining_bytes() > 0; i++) {
                uint8_t item_type = fdp.ConsumeIntegral<uint8_t>() % 8;
                
                switch (item_type) {
                    case 0:
                        cJSON_AddNullToObject(obj1, keys[i].c_str());
                        break;
                    case 1:
                        cJSON_AddTrueToObject(obj1, keys[i].c_str());
                        break;
                    case 2:
                        cJSON_AddFalseToObject(obj1, keys[i].c_str());
                        break;
                    case 3: {
                        bool bool_val = fdp.ConsumeBool();
                        cJSON_AddBoolToObject(obj1, keys[i].c_str(), bool_val ? 1 : 0);
                        break;
                    }
                    case 4: {
                        double num_val = fdp.ConsumeFloatingPoint<double>();
                        cJSON_AddNumberToObject(obj1, keys[i].c_str(), num_val);
                        break;
                    }
                    case 5: {
                        size_t str_len = fdp.ConsumeIntegralInRange<size_t>(1, 64);
                        if (fdp.remaining_bytes() < str_len) break;
                        std::string str_val = fdp.ConsumeBytesAsString(str_len);
                        cJSON_AddStringToObject(obj1, keys[i].c_str(), str_val.c_str());
                        break;
                    }
                    case 6: {
                        size_t raw_len = fdp.ConsumeIntegralInRange<size_t>(1, 64);
                        if (fdp.remaining_bytes() < raw_len) break;
                        std::string raw_val = fdp.ConsumeBytesAsString(raw_len);
                        cJSON_AddRawToObject(obj1, keys[i].c_str(), raw_val.c_str());
                        break;
                    }
                    case 7: {
                        // Add nested object
                        cJSON* nested_obj = cJSON_AddObjectToObject(obj1, keys[i].c_str());
                        if (nested_obj != NULL && fdp.remaining_bytes() > 0) {
                            // Add one item to nested object
                            std::string nested_key = "nested_" + std::to_string(i);
                            cJSON_AddNumberToObject(nested_obj, nested_key.c_str(), 
                                                    fdp.ConsumeFloatingPoint<double>());
                        }
                        break;
                    }
                }
            }
            break;
        }
        
        case 1: {
            // Test AddItemToObject and AddItemToObjectCS
            for (size_t i = 0; i < keys.size() && i < 4 && fdp.remaining_bytes() > 0; i++) {
                // Create items to add
                cJSON* item = NULL;
                uint8_t item_type = fdp.ConsumeIntegral<uint8_t>() % 3;
                
                switch (item_type) {
                    case 0:
                        item = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
                        break;
                    case 1: {
                        size_t str_len = fdp.ConsumeIntegralInRange<size_t>(1, 32);
                        if (fdp.remaining_bytes() < str_len) {
                            if (item != NULL) cJSON_Delete(item);
                            continue;
                        }
                        std::string str_val = fdp.ConsumeBytesAsString(str_len);
                        item = cJSON_CreateString(str_val.c_str());
                        break;
                    }
                    case 2:
                        item = cJSON_CreateArray();
                        break;
                }
                
                if (item != NULL) {
                    bool use_cs = fdp.ConsumeBool();
                    if (use_cs) {
                        cJSON_AddItemToObjectCS(obj1, keys[i].c_str(), item);
                    } else {
                        cJSON_AddItemToObject(obj1, keys[i].c_str(), item);
                    }
                }
            }
            break;
        }
        
        case 2: {
            // Test ReplaceItemInObjectCaseSensitive
            // First add some items
            for (size_t i = 0; i < std::min(keys.size(), static_cast<size_t>(3)); i++) {
                cJSON_AddNumberToObject(obj1, keys[i].c_str(), fdp.ConsumeFloatingPoint<double>());
            }
            
            // Then replace some items if we have enough keys
            if (keys.size() >= 2 && fdp.remaining_bytes() > 0) {
                // Create replacement items
                cJSON* replacement = NULL;
                uint8_t repl_type = fdp.ConsumeIntegral<uint8_t>() % 2;
                
                if (repl_type == 0) {
                    size_t str_len = fdp.ConsumeIntegralInRange<size_t>(1, 64);
                    if (fdp.remaining_bytes() < str_len) {
                        break;
                    }
                    std::string str_val = fdp.ConsumeBytesAsString(str_len);
                    replacement = cJSON_CreateString(str_val.c_str());
                } else {
                    replacement = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
                }
                
                if (replacement != NULL) {
                    // Try to replace an item (may succeed or fail depending on if key exists)
                    cJSON_ReplaceItemInObjectCaseSensitive(obj1, keys[0].c_str(), replacement);
                }
            }
            break;
        }
        
        case 3: {
            // Test DeleteItemFromObjectCaseSensitive
            // First add some items
            for (size_t i = 0; i < std::min(keys.size(), static_cast<size_t>(4)); i++) {
                size_t str_len = fdp.ConsumeIntegralInRange<size_t>(1, 32);
                if (fdp.remaining_bytes() < str_len) break;
                std::string str_val = fdp.ConsumeBytesAsString(str_len);
                cJSON_AddStringToObject(obj1, keys[i].c_str(), str_val.c_str());
            }
            // Delete some items
            if (keys.size() >= 2) {
                // Delete existing item (if key exists)
                cJSON_DeleteItemFromObjectCaseSensitive(obj1, keys[0].c_str());
                
                // Try to delete non-existent item
                if (fdp.remaining_bytes() > 0) {
                    size_t non_existent_key_len = fdp.ConsumeIntegralInRange<size_t>(1, 32);
                    if (fdp.remaining_bytes() >= non_existent_key_len) {
                        std::string non_existent_key = fdp.ConsumeBytesAsString(non_existent_key_len);
                        cJSON_DeleteItemFromObjectCaseSensitive(obj1, non_existent_key.c_str());
                    }
                }
            }
            break;
        }
        case 4: {
            // Test cJSON_Compare with two objects
            obj2 = cJSON_CreateObject();
            if (obj2 == NULL) {
                cJSON_Delete(obj1);
                return 0;
            }
            
            // Add similar items to both objects
            for (size_t i = 0; i < std::min(keys.size(), static_cast<size_t>(3)); i++) {
                double num_val = fdp.ConsumeFloatingPoint<double>();
                cJSON_AddNumberToObject(obj1, keys[i].c_str(), num_val);
                cJSON_AddNumberToObject(obj2, keys[i].c_str(), num_val);
            }
            
            // Possibly make objects different
            if (fdp.ConsumeBool() && keys.size() > 3 && fdp.remaining_bytes() > 0) {
                size_t str_len = fdp.ConsumeIntegralInRange<size_t>(1, 32);
                if (fdp.remaining_bytes() >= str_len) {
                    std::string diff_str = fdp.ConsumeBytesAsString(str_len);
                    cJSON_AddStringToObject(obj1, keys[3].c_str(), diff_str.c_str());
                }
            }
            
            // Compare objects with case sensitivity flag
            bool case_sensitive = fdp.ConsumeBool();
            cJSON_Compare(obj1, obj2, case_sensitive ? 1 : 0);
            break;
        }
        
        case 5: {
            // Comprehensive object lifecycle test
            // Create object, add various items, manipulate, then cleanup
            
            // Add mixed item types
            for (size_t i = 0; i < keys.size() && i < 5 && fdp.remaining_bytes() > 0; i++) {
                uint8_t item_type = fdp.ConsumeIntegral<uint8_t>() % 6;
                
                switch (item_type) {
                    case 0:
                        cJSON_AddNullToObject(obj1, keys[i].c_str());
                        break;
                    case 1:
                        cJSON_AddNumberToObject(obj1, keys[i].c_str(), 
                                               fdp.ConsumeFloatingPoint<double>());
                        break;
                    case 2: {
                        size_t str_len = fdp.ConsumeIntegralInRange<size_t>(1, 32);
                        if (fdp.remaining_bytes() < str_len) break;
                        std::string str_val = fdp.ConsumeBytesAsString(str_len);
                        cJSON_AddStringToObject(obj1, keys[i].c_str(), str_val.c_str());
                        break;
                    }
                    case 3:
                        cJSON_AddBoolToObject(obj1, keys[i].c_str(), fdp.ConsumeBool() ? 1 : 0);
                        break;
                    case 4:
                        cJSON_AddObjectToObject(obj1, keys[i].c_str());
                        break;
                    case 5:
                        cJSON_AddArrayToObject(obj1, keys[i].c_str());
                        break;
                }
            }
            
            // Test AddItemReferenceToObject if we have enough data
            if (fdp.remaining_bytes() > 0 && keys.size() > 0) {
                size_t ref_key_len = fdp.ConsumeIntegralInRange<size_t>(1, 32);
                if (fdp.remaining_bytes() >= ref_key_len) {
                    std::string ref_key = fdp.ConsumeBytesAsString(ref_key_len);
                    cJSON* ref_item = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
                    if (ref_item != NULL) {
                        cJSON_AddItemReferenceToObject(obj1, ref_key.c_str(), ref_item);
                        // Don't delete ref_item - reference takes ownership
                    }
                }
            }
            
            // Print the object to exercise printing code paths
            char* printed = cJSON_Print(obj1);
            if (printed != NULL) {
                free(printed);
            }
            break;
        }
    }

    // Cleanup
    cJSON_Delete(obj1);
    if (obj2 != NULL) {
        cJSON_Delete(obj2);
    }

    return 0;
}

} // namespace harness_002
