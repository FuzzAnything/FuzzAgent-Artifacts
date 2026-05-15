#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <cmath>
#include <fuzzer/FuzzedDataProvider.h>
#include "cjson/cJSON.h"

namespace harness_010 {


// Custom allocation hooks that can simulate failures
static size_t custom_malloc_failure_count = 0;
static size_t custom_free_failure_count = 0;

static void* CJSON_CDECL custom_malloc_failer(size_t size) {
    // Simulate malloc failure based on input-controlled probability
    if (custom_malloc_failure_count > 0) {
        custom_malloc_failure_count--;
        return NULL;
    }
    
    // Otherwise, allocate normally
    return malloc(size);
}

static void CJSON_CDECL custom_free_failer(void* ptr) {
    // Simulate free failure (though free rarely fails in reality)
    if (custom_free_failure_count > 0) {
        custom_free_failure_count--;
        // Don't actually free to simulate failure
        return;
    }
    
    // Otherwise, free normally
    free(ptr);
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need sufficient input for comprehensive error handling testing
    if (size < 32) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Reset custom allocation counters
    custom_malloc_failure_count = 0;
    custom_free_failure_count = 0;
    
    // Determine test scenario
    uint8_t scenario = fdp.ConsumeIntegral<uint8_t>() % 8;
    
    // Helper function to create random strings
    auto create_random_string = [&fdp](size_t max_len = 64) -> std::string {
        if (fdp.remaining_bytes() == 0) return "";
        size_t len = fdp.ConsumeIntegralInRange<size_t>(1, max_len);
        if (fdp.remaining_bytes() < len) return "";
        return fdp.ConsumeBytesAsString(len);
    };
    
    switch (scenario) {
        case 0: {
            // Test cJSON_InitHooks with custom allocation functions
            cJSON_Hooks hooks;
            
            bool use_custom_hooks = fdp.ConsumeBool();
            
            if (use_custom_hooks) {
                // Set failure probabilities for custom allocators
                custom_malloc_failure_count = fdp.ConsumeIntegralInRange<size_t>(0, 5);
                custom_free_failure_count = fdp.ConsumeIntegralInRange<size_t>(0, 3);
                
                hooks.malloc_fn = custom_malloc_failer;
                hooks.free_fn = custom_free_failer;
                
                cJSON_InitHooks(&hooks);
            } else {
                // Reset to default hooks (pass NULL)
                cJSON_InitHooks(NULL);
            }
            
            // Test some operations that might trigger allocation failures
            std::string json_str = create_random_string(256);
            cJSON* parsed = cJSON_Parse(json_str.c_str());
            
            if (parsed) {
                cJSON_AddStringToObject(parsed, "test_key", "test_value");
                cJSON_DeleteItemFromObject(parsed, "test_key");
                cJSON_Delete(parsed);
            }
            break;
        }
        
        case 1: {
            // Test cJSON_DeleteItemFromObject with various object structures
            cJSON_Hooks hooks;
            hooks.malloc_fn = custom_malloc_failer;
            hooks.free_fn = custom_free_failer;
            
            custom_malloc_failure_count = fdp.ConsumeIntegralInRange<size_t>(0, 2);
            custom_free_failure_count = fdp.ConsumeIntegralInRange<size_t>(0, 1);
            
            cJSON_InitHooks(&hooks);
            
            cJSON* root = cJSON_CreateObject();
            if (!root) {
                return 0;
            }
            
            // Add various items to the object
            int num_items = fdp.ConsumeIntegralInRange<int>(1, 10);
            std::vector<std::string> keys_added;
            
            for (int i = 0; i < num_items && fdp.remaining_bytes() > 0; i++) {
                std::string key = "key_" + std::to_string(i) + "_" + create_random_string(8);
                keys_added.push_back(key);
                
                uint8_t item_type = fdp.ConsumeIntegral<uint8_t>() % 7;
                
                switch (item_type) {
                    case 0:
                        cJSON_AddNullToObject(root, key.c_str());
                        break;
                    case 1:
                        cJSON_AddTrueToObject(root, key.c_str());
                        break;
                    case 2:
                        cJSON_AddFalseToObject(root, key.c_str());
                        break;
                    case 3:
                        cJSON_AddNumberToObject(root, key.c_str(), fdp.ConsumeFloatingPoint<double>());
                        break;
                    case 4: {
                        std::string str_val = create_random_string(32);
                        cJSON_AddStringToObject(root, key.c_str(), str_val.c_str());
                        break;
                    }
                    case 5: {
                        cJSON* nested = cJSON_AddObjectToObject(root, key.c_str());
                        if (nested) {
                            cJSON_AddNumberToObject(nested, "nested_key", fdp.ConsumeFloatingPoint<double>());
                        }
                        break;
                    }
                    case 6: {
                        cJSON* array = cJSON_AddArrayToObject(root, key.c_str());
                        if (array) {
                            int array_items = fdp.ConsumeIntegralInRange<int>(1, 5);
                            for (int j = 0; j < array_items && fdp.remaining_bytes() > 0; j++) {
                                cJSON* number_item = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
                                if (number_item) {
                                    cJSON_AddItemToArray(array, number_item);
                                }
                            }
                        }
                        break;
                    }
                }
            }
            
            // Test DeleteItemFromObject with various keys
            if (!keys_added.empty()) {
                for (size_t i = 0; i < std::min(keys_added.size(), static_cast<size_t>(3)); i++) {
                    cJSON_DeleteItemFromObject(root, keys_added[i].c_str());
                }
                
                for (int i = 0; i < 3 && fdp.remaining_bytes() > 0; i++) {
                    std::string non_existent_key = "non_existent_" + create_random_string(10);
                    cJSON_DeleteItemFromObject(root, non_existent_key.c_str());
                }
                
                for (size_t i = 3; i < keys_added.size(); i++) {
                    cJSON_DeleteItemFromObject(root, keys_added[i].c_str());
                }
            }
            
            cJSON_Delete(root);
            break;
        }
        
        case 2: {
            // Test parse_number error paths with invalid numeric inputs
            cJSON_Hooks hooks;
            hooks.malloc_fn = custom_malloc_failer;
            hooks.free_fn = custom_free_failer;
            
            custom_malloc_failure_count = fdp.ConsumeIntegralInRange<size_t>(0, 3);
            cJSON_InitHooks(&hooks);
            
            // Create invalid number strings
            int num_tests = fdp.ConsumeIntegralInRange<int>(1, 10);
            
            for (int i = 0; i < num_tests && fdp.remaining_bytes() > 0; i++) {
                // Create various invalid number formats
                std::string invalid_num;
                uint8_t type = fdp.ConsumeIntegral<uint8_t>() % 6;
                
                switch (type) {
                    case 0: invalid_num = "12.3a4.5"; break;
                    case 1: invalid_num = "12..34"; break;
                    case 2: invalid_num = "1.2e+e3"; break;
                    case 3: invalid_num = "abcxyz"; break;
                    case 4: invalid_num = ""; break;
                    case 5: invalid_num = "1.0e999999999"; break;
                }
                
                // Mix with random characters
                if (fdp.ConsumeBool() && fdp.remaining_bytes() > 0) {
                    size_t extra_len = fdp.ConsumeIntegralInRange<size_t>(1, 10);
                    if (fdp.remaining_bytes() >= extra_len) {
                        std::string extra = fdp.ConsumeBytesAsString(extra_len);
                        invalid_num += extra;
                    }
                }
                
                std::string json_str = "{\"value\": " + invalid_num + "}";
                cJSON* parsed = cJSON_Parse(json_str.c_str());
                
                if (parsed) {
                    cJSON_Delete(parsed);
                }
            }
            break;
        }
        
        case 3: {
            // Combine allocation failures with DeleteItemFromObject
            cJSON_Hooks hooks;
            hooks.malloc_fn = custom_malloc_failer;
            hooks.free_fn = custom_free_failer;
            
            custom_malloc_failure_count = fdp.ConsumeIntegralInRange<size_t>(1, 10);
            custom_free_failure_count = fdp.ConsumeIntegralInRange<size_t>(0, 5);
            
            cJSON_InitHooks(&hooks);
            
            cJSON* obj = cJSON_CreateObject();
            if (!obj) {
                return 0;
            }
            
            // Try to add items (might fail due to allocation)
            int attempts = fdp.ConsumeIntegralInRange<int>(1, 20);
            std::vector<std::string> successful_keys;
            
            for (int i = 0; i < attempts && fdp.remaining_bytes() > 0; i++) {
                std::string key = "item_" + std::to_string(i);
                
                cJSON* added = cJSON_AddNumberToObject(obj, key.c_str(), fdp.ConsumeFloatingPoint<double>());
                if (added) {
                    successful_keys.push_back(key);
                }
            }
            
            // Now try to delete items (free might fail)
            for (const auto& key : successful_keys) {
                cJSON_DeleteItemFromObject(obj, key.c_str());
            }
            
            // Try to delete non-existent items
            for (int i = 0; i < 5 && fdp.remaining_bytes() > 0; i++) {
                std::string fake_key = "fake_" + create_random_string(8);
                cJSON_DeleteItemFromObject(obj, fake_key.c_str());
            }
            
            cJSON_Delete(obj);
            break;
        }
        
        case 4: {
            // Test error handling in complex nested structures
            cJSON_Hooks hooks;
            hooks.malloc_fn = custom_malloc_failer;
            hooks.free_fn = custom_free_failer;
            
            custom_malloc_failure_count = fdp.ConsumeIntegralInRange<size_t>(0, 4);
            custom_free_failure_count = fdp.ConsumeIntegralInRange<size_t>(0, 2);
            
            cJSON_InitHooks(&hooks);
            
            cJSON* root = cJSON_CreateObject();
            if (!root) return 0;
            
            // Build nested objects
            cJSON* current = root;
            int depth = fdp.ConsumeIntegralInRange<int>(1, 5);
            
            for (int i = 0; i < depth; i++) {
                std::string key = "level_" + std::to_string(i);
                cJSON* nested = cJSON_AddObjectToObject(current, key.c_str());
                if (!nested) break;
                
                cJSON_AddNumberToObject(nested, "data", fdp.ConsumeFloatingPoint<double>());
                current = nested;
            }
            
            // Delete items at various levels
            cJSON* temp = root;
            for (int i = 0; i < depth && temp; i++) {
                std::string key_to_delete = "level_" + std::to_string(i);
                cJSON_DeleteItemFromObject(temp, key_to_delete.c_str());
                
                if (i < depth - 1) {
                    std::string next_key = "level_" + std::to_string(i + 1);
                    cJSON* next = cJSON_GetObjectItem(temp, next_key.c_str());
                    temp = next;
                }
            }
            
            cJSON_Delete(root);
            break;
        }
        
        case 5: {
            // Test with mixed valid and invalid JSON
            cJSON_InitHooks(NULL);
            
            int num_strings = fdp.ConsumeIntegralInRange<int>(5, 20);
            
            for (int i = 0; i < num_strings && fdp.remaining_bytes() > 0; i++) {
                uint8_t string_type = fdp.ConsumeIntegral<uint8_t>() % 4;
                std::string json_str;
                
                switch (string_type) {
                    case 0:
                        json_str = "{\"num\": " + std::to_string(fdp.ConsumeFloatingPoint<double>()) + 
                                   ", \"str\": \"" + create_random_string(20) + "\"}";
                        break;
                    case 1:
                        json_str = "{\"invalid_num\": 12.3a4.5}";
                        break;
                    case 2:
                        json_str = create_random_string(100);
                        break;
                    case 3:
                        json_str = fdp.ConsumeBool() ? "" : "{}";
                        break;
                }
                
                cJSON* parsed = cJSON_Parse(json_str.c_str());
                
                if (parsed) {
                    if (cJSON_IsObject(parsed)) {
                        cJSON* child = parsed->child;
                        while (child) {
                            cJSON* next = child->next;
                            if (child->string) {
                                cJSON_DeleteItemFromObject(parsed, child->string);
                            }
                            child = next;
                        }
                    }
                    
                    cJSON_Delete(parsed);
                }
            }
            break;
        }
        
        case 6: {
            // Stress test with rapid allocation/deallocation
            cJSON_Hooks hooks;
            hooks.malloc_fn = custom_malloc_failer;
            hooks.free_fn = custom_free_failer;
            
            custom_malloc_failure_count = fdp.ConsumeIntegralInRange<size_t>(0, 20);
            custom_free_failure_count = fdp.ConsumeIntegralInRange<size_t>(0, 10);
            
            cJSON_InitHooks(&hooks);
            
            int iterations = fdp.ConsumeIntegralInRange<int>(10, 50);
            
            for (int i = 0; i < iterations && fdp.remaining_bytes() > 0; i++) {
                cJSON* obj = cJSON_CreateObject();
                if (!obj) continue;
                
                int num_items = fdp.ConsumeIntegralInRange<int>(1, 10);
                for (int j = 0; j < num_items && fdp.remaining_bytes() > 0; j++) {
                    std::string key = "key_" + std::to_string(j);
                    cJSON_AddNumberToObject(obj, key.c_str(), fdp.ConsumeFloatingPoint<double>());
                }
                
                if (num_items > 0) {
                    for (int j = 0; j < std::min(num_items, 3); j++) {
                        std::string key_to_delete = "key_" + std::to_string(j);
                        cJSON_DeleteItemFromObject(obj, key_to_delete.c_str());
                    }
                }
                
                cJSON_Delete(obj);
            }
            break;
        }
        
        case 7: {
            // Test with NULL and edge case inputs
            cJSON_InitHooks(NULL);
            
            // Test DeleteItemFromObject with NULL or empty object
            cJSON* empty_obj = cJSON_CreateObject();
            
            if (empty_obj) {
                cJSON_DeleteItemFromObject(empty_obj, "non_existent");
                cJSON_DeleteItemFromObject(empty_obj, NULL);
                
                cJSON_AddNumberToObject(empty_obj, "test", 42.0);
                cJSON_DeleteItemFromObject(empty_obj, "test");
                
                cJSON_Delete(empty_obj);
            }
            
            // Test parse_number error paths with edge cases
            std::vector<std::string> edge_cases = {
                "", " ", "  ", "\t", "\n", ".", "-", "+", "e", "E",
                ".", "-.", "+.", ".e", ".E", "inf", "INF", "nan", "NAN",
                "0x123", "1.2.3", "1e2e3"
            };
            
            for (const auto& edge_case : edge_cases) {
                std::string json_str = "{\"num\": " + edge_case + "}";
                cJSON* parsed = cJSON_Parse(json_str.c_str());
                
                if (parsed) {
                    cJSON_Delete(parsed);
                }
            }
            break;
        }
    }
    
    // Reset hooks to default before exiting
    cJSON_InitHooks(NULL);
    
    return 0;
}

} // namespace harness_010
