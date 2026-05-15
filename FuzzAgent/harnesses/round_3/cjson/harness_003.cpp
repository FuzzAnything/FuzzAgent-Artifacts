#include <fuzzer/FuzzedDataProvider.h>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include "cjson/cJSON.h"

namespace harness_003 {

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check for meaningful testing
    if (size < 64) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Consume fixed-size data first
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 6;
    uint8_t num_items = fdp.ConsumeIntegralInRange<uint8_t>(1, 10);
    bool case_sensitive = fdp.ConsumeBool();
    int item_index = fdp.ConsumeIntegralInRange<int>(0, num_items - 1);
    int compare_type = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    // Consume strings for keys with different case variations
    // Ensure all key vectors have exactly num_items elements
    std::vector<std::string> keys_lower(num_items);
    std::vector<std::string> keys_upper(num_items);
    std::vector<std::string> keys_mixed(num_items);
    
    for (int i = 0; i < num_items && fdp.remaining_bytes() > 0; i++) {
        std::string base_key = fdp.ConsumeRandomLengthString(32);
        if (base_key.empty()) {
            // Use default key if empty
            base_key = "key" + std::to_string(i);
        }
        
        // Create different case variations of the same key
        std::string lower_key = base_key;
        for (char &c : lower_key) {
            if (c >= 'A' && c <= 'Z') c = c + ('a' - 'A');
        }
        
        std::string upper_key = base_key;
        for (char &c : upper_key) {
            if (c >= 'a' && c <= 'z') c = c - ('a' - 'A');
        }
        
        // Mixed case (capitalize first letter)
        std::string mixed_key = base_key;
        if (!mixed_key.empty()) {
            if (mixed_key[0] >= 'a' && mixed_key[0] <= 'z') {
                mixed_key[0] = mixed_key[0] - ('a' - 'A');
            }
        }
        
        keys_lower[i] = lower_key;
        keys_upper[i] = upper_key;
        keys_mixed[i] = mixed_key;
    }
    
    // Fill remaining keys with default values if fuzzer data was insufficient
    for (int i = 0; i < num_items; i++) {
        if (keys_mixed[i].empty()) {
            std::string default_key = "default" + std::to_string(i);
            keys_lower[i] = default_key;
            keys_upper[i] = default_key;
            keys_mixed[i] = default_key;
        }
    }
    
    // Ensure all value vectors have exactly num_items elements
    // If fuzzer data is insufficient, use default values
    std::vector<std::string> string_values(num_items);
    std::vector<double> number_values(num_items, 0.0);
    std::vector<bool> bool_values(num_items, false);
    
    for (int i = 0; i < num_items && fdp.remaining_bytes() > 0; i++) {
        string_values[i] = fdp.ConsumeRandomLengthString(128);
        if (fdp.remaining_bytes() >= sizeof(double)) {
            number_values[i] = fdp.ConsumeFloatingPoint<double>();
        }
        if (fdp.remaining_bytes() > 0) {
            bool_values[i] = fdp.ConsumeBool();
        }
    }
    
    // Consume remaining data for JSON input
    std::string json_input = fdp.ConsumeRemainingBytesAsString();

    // Use operation to determine API flow
    switch (operation) {
        case 0: {
            // Test cJSON_GetObjectItemCaseSensitive vs cJSON_GetObjectItem
            cJSON* root = cJSON_CreateObject();
            if (root != NULL) {
                // Add items with mixed case keys
                for (int i = 0; i < num_items; i++) {
                    cJSON_AddStringToObject(root, keys_mixed[i].c_str(), string_values[i].c_str());
                }
                // Test case-sensitive lookup
                for (int i = 0; i < keys_mixed.size(); i++) {
                    // Try to get with case-sensitive API
                    cJSON* case_sensitive_item = cJSON_GetObjectItemCaseSensitive(root, keys_mixed[i].c_str());
                    
                    // Try different case variations
                    if (!keys_lower.empty() && i < keys_lower.size()) {
                        cJSON* lower_case_item = cJSON_GetObjectItemCaseSensitive(root, keys_lower[i].c_str());
                        // Should be NULL if keys_lower[i] != keys_mixed[i] (different case)
                    }
                    
                    if (!keys_upper.empty() && i < keys_upper.size()) {
                        cJSON* upper_case_item = cJSON_GetObjectItemCaseSensitive(root, keys_upper[i].c_str());
                        // Should be NULL if keys_upper[i] != keys_mixed[i] (different case)
                    }
                }
                
                // Test case-insensitive lookup for comparison
                for (int i = 0; i < keys_mixed.size(); i++) {
                    cJSON* case_insensitive_item = cJSON_GetObjectItem(root, keys_mixed[i].c_str());
                }
                
                cJSON_Delete(root);
            }
            break;
        }
        
        case 1: {
            // Test cJSON_DetachItemFromObjectCaseSensitive
            cJSON* root = cJSON_CreateObject();
            if (root != NULL) {
                // Add items with various case keys
                for (int i = 0; i < num_items; i++) {
                    cJSON_AddNumberToObject(root, keys_mixed[i].c_str(), number_values[i]);
                }
                // Detach items using case-sensitive API
                if (!keys_mixed.empty() && item_index < keys_mixed.size()) {
                    cJSON* detached = cJSON_DetachItemFromObjectCaseSensitive(root, keys_mixed[item_index].c_str());
                    if (detached != NULL) {
                        cJSON_Delete(detached);
                    }
                    
                    // Try to detach with different case (should fail)
                    if (!keys_lower.empty() && item_index < keys_lower.size()) {
                        cJSON* detached_wrong_case = cJSON_DetachItemFromObjectCaseSensitive(root, keys_lower[item_index].c_str());
                        // Should be NULL if case doesn't match
                    }
                }
                
                cJSON_Delete(root);
            }
            break;
        }
        
        case 2: {
            // Test cJSON_DeleteItemFromObjectCaseSensitive
            cJSON* root = cJSON_CreateObject();
            if (root != NULL) {
                // Add items with mixed case keys
                for (int i = 0; i < num_items; i++) {
                    cJSON_AddBoolToObject(root, keys_mixed[i].c_str(), bool_values[i]);
                }
                
                // Delete items using case-sensitive API
                if (!keys_mixed.empty() && item_index < keys_mixed.size()) {
                    cJSON_DeleteItemFromObjectCaseSensitive(root, keys_mixed[item_index].c_str());
                    
                    // Try to delete with different case (should have no effect)
                    if (!keys_lower.empty() && item_index < keys_lower.size()) {
                        cJSON_DeleteItemFromObjectCaseSensitive(root, keys_lower[item_index].c_str());
                    }
                }
                
                cJSON_Delete(root);
            }
            break;
        }
        
        case 3: {
            // Test cJSON_ReplaceItemInObjectCaseSensitive
            cJSON* root = cJSON_CreateObject();
            if (root != NULL) {
                // Add initial items
                // Add initial items
                for (int i = 0; i < num_items; i++) {
                    cJSON_AddStringToObject(root, keys_mixed[i].c_str(), string_values[i].c_str());
                }
                if (!keys_mixed.empty() && item_index < keys_mixed.size()) {
                    cJSON* new_item = cJSON_CreateString(string_values[item_index].c_str());
                    cJSON_bool replaced1 = cJSON_ReplaceItemInObjectCaseSensitive(root, keys_mixed[item_index].c_str(), new_item);
                    if (!replaced1 && new_item != NULL) {
                        cJSON_Delete(new_item);
                    }
                    
                    // Try to replace with different case (should not replace)
                    if (!keys_upper.empty() && item_index < keys_upper.size()) {
                        cJSON* another_new_item = cJSON_CreateNumber(number_values[item_index]);
                        cJSON_bool replaced2 = cJSON_ReplaceItemInObjectCaseSensitive(root, keys_upper[item_index].c_str(), another_new_item);
                        // If keys_upper[item_index] != keys_mixed[item_index], item won't be replaced
                        // Need to clean up the created item if not used
                        if (!replaced2 && another_new_item != NULL) {
                            cJSON_Delete(another_new_item);
                        }
                    }
                }
                
                cJSON_Delete(root);
            }
            break;
        }
        
        case 4: {
            // Test cJSON_Compare with case_sensitive parameter
            cJSON* obj1 = cJSON_CreateObject();
            cJSON* obj2 = cJSON_CreateObject();
            
            if (obj1 != NULL && obj2 != NULL) {
                // Create two similar objects with possibly different case keys
                for (int i = 0; i < num_items; i++) {
                    cJSON_AddStringToObject(obj1, keys_mixed[i].c_str(), string_values[i].c_str());
                    
                    // For obj2, use different case keys based on compare_type
                    std::string key_for_obj2;
                    switch (compare_type % 3) {
                        case 0: key_for_obj2 = keys_mixed[i]; break;
                        case 1: key_for_obj2 = keys_lower[i]; break;
                        case 2: key_for_obj2 = keys_upper[i]; break;
                        default: key_for_obj2 = keys_mixed[i]; break;
                    }
                    
                    cJSON_AddStringToObject(obj2, key_for_obj2.c_str(), string_values[i].c_str());
                }
                
                // Compare with case-sensitive option
                cJSON_bool sensitive_equal = cJSON_Compare(obj1, obj2, 1); // case_sensitive = true
                
                // Compare with case-insensitive option
                cJSON_bool insensitive_equal = cJSON_Compare(obj1, obj2, 0); // case_sensitive = false
                
                // Create identical objects for baseline comparison
                cJSON* obj3 = cJSON_CreateObject();
                if (obj3 != NULL) {
                    for (int i = 0; i < num_items; i++) {
                        cJSON_AddStringToObject(obj3, keys_mixed[i].c_str(), string_values[i].c_str());
                    }
                    
                    cJSON_bool identical_sensitive = cJSON_Compare(obj1, obj3, 1);
                    cJSON_bool identical_insensitive = cJSON_Compare(obj1, obj3, 0);
                    
                    cJSON_Delete(obj3);
                }
                
                cJSON_Delete(obj1);
                cJSON_Delete(obj2);
            } else {
                if (obj1 != NULL) cJSON_Delete(obj1);
                if (obj2 != NULL) cJSON_Delete(obj2);
            }
            break;
        }
        
        case 5: {
            // Comprehensive case-sensitive object manipulation test
            cJSON* root = cJSON_CreateObject();
            if (root != NULL) {
                // Phase 1: Add items with various case keys
                // Phase 1: Add items with various case keys
                for (int i = 0; i < num_items; i++) {
                    switch (i % 3) {
                        case 0:
                            cJSON_AddStringToObject(root, keys_mixed[i].c_str(), string_values[i].c_str());
                            break;
                        case 1:
                            cJSON_AddNumberToObject(root, keys_mixed[i].c_str(), number_values[i]);
                            break;
                        case 2:
                            cJSON_AddBoolToObject(root, keys_mixed[i].c_str(), bool_values[i]);
                            break;
                    }
                }
                
                // Phase 2: Test all case-sensitive APIs in sequence
                if (!keys_mixed.empty()) {
                    int test_idx = item_index % keys_mixed.size();
                    
                    // 1. Get item case-sensitive
                    cJSON* item1 = cJSON_GetObjectItemCaseSensitive(root, keys_mixed[test_idx].c_str());
                    
                    // 2. Try to get with different case (should fail)
                    if (!keys_lower.empty() && test_idx < keys_lower.size()) {
                        cJSON* item2 = cJSON_GetObjectItemCaseSensitive(root, keys_lower[test_idx].c_str());
                    }
                    
                    // 3. Replace item case-sensitive
                    if (item1 != NULL) {
                        cJSON* new_item = cJSON_CreateString("replaced_value");
                        cJSON_bool replaced = cJSON_ReplaceItemInObjectCaseSensitive(root, keys_mixed[test_idx].c_str(), new_item);
                        if (!replaced && new_item != NULL) {
                            cJSON_Delete(new_item);
                        }
                    }
                    
                    // 4. Detach another item case-sensitive
                    int detach_idx = (test_idx + 1) % keys_mixed.size();
                    cJSON* detached = cJSON_DetachItemFromObjectCaseSensitive(root, keys_mixed[detach_idx].c_str());
                    if (detached != NULL) {
                        cJSON_Delete(detached);
                    }
                    
                    // 5. Delete another item case-sensitive
                    int delete_idx = (test_idx + 2) % keys_mixed.size();
                    if (delete_idx < keys_mixed.size()) {
                        cJSON_DeleteItemFromObjectCaseSensitive(root, keys_mixed[delete_idx].c_str());
                    }
                }
                
                // Phase 3: Verify object state
                char* printed = cJSON_Print(root);
                if (printed != NULL) {
                    free(printed);
                }
                
                cJSON_Delete(root);
            }
            break;
        }
    }
    
    return 0;
}

} // namespace harness_003
