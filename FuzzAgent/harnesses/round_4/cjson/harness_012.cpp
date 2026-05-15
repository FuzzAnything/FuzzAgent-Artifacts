#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <cmath>
#include <functional>
#include <fuzzer/FuzzedDataProvider.h>
#include "cjson/cJSON.h"

namespace harness_012 {
// Custom allocation hooks to test cJSON_malloc failure paths
static size_t allocation_count = 0;
static size_t fail_after_count = 0;
static bool should_fail = false;

static void* custom_malloc(size_t size) {
    allocation_count++;
    if (should_fail && allocation_count > fail_after_count) {
        return NULL;
    }
    return malloc(size);
}

static void custom_free(void* ptr) {
    free(ptr);
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need sufficient input for comprehensive comparison testing
    if (size < 64) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Set up custom allocation hooks based on fuzzer input
    should_fail = fdp.ConsumeBool();
    fail_after_count = fdp.ConsumeIntegralInRange<size_t>(0, 10);
    allocation_count = 0;
    
    cJSON_Hooks custom_hooks;
    custom_hooks.malloc_fn = custom_malloc;
    custom_hooks.free_fn = custom_free;
    cJSON_InitHooks(&custom_hooks);
    
    // Determine test scenario to target specific uncovered branches
    uint8_t scenario = fdp.ConsumeIntegral<uint8_t>() % 20;
    
    // Consume case_sensitive parameter
    cJSON_bool case_sensitive = fdp.ConsumeBool();
    
    // Helper function to create random strings
    auto create_random_string = [&fdp](size_t max_len = 32) -> std::string {
        if (fdp.remaining_bytes() == 0) return "";
        size_t len = fdp.ConsumeIntegralInRange<size_t>(1, max_len);
        if (fdp.remaining_bytes() < len) return "";
        return fdp.ConsumeBytesAsString(len);
    };
    
    // Helper function to create random number
    auto create_random_number = [&fdp]() -> double {
        if (fdp.remaining_bytes() < sizeof(double)) return 0.0;
        return fdp.ConsumeFloatingPoint<double>();
    };
    
    cJSON* json_a = NULL;
    cJSON* json_b = NULL;
    cJSON_bool compare_result = false;
    
    switch (scenario) {
        case 0: {
            // Scenario 0: NULL pointer checks (target line 3078)
            // Test all combinations of NULL pointers
            if (fdp.ConsumeBool()) {
                json_a = NULL;
            } else {
                json_a = cJSON_CreateNull();
            }
            
            if (fdp.ConsumeBool()) {
                json_b = NULL;
            } else {
                json_b = cJSON_CreateNull();
            }
            
            compare_result = cJSON_Compare(json_a, json_b, case_sensitive);
            
            // Clean up if we created objects
            if (json_a != NULL) cJSON_Delete(json_a);
            if (json_b != NULL) cJSON_Delete(json_b);
            break;
        }
            
        case 1: {
            // Scenario 1: Type mismatch - compare different types
            // (target line 3078 type check and line 3096 default case)
            uint8_t type_a = fdp.ConsumeIntegral<uint8_t>() % 8;  // 8 cJSON types
            uint8_t type_b = fdp.ConsumeIntegral<uint8_t>() % 8;
            
            // Ensure different types
            if (type_a == type_b) {
                type_b = (type_b + 1) % 8;
            }
            
            // Create items based on type
            auto create_item_by_type = [&](uint8_t type) -> cJSON* {
                switch (type) {
                    case 0: return cJSON_CreateNull();
                    case 1: return cJSON_CreateBool(fdp.ConsumeBool());
                    case 2: return cJSON_CreateNumber(create_random_number());
                    case 3: {
                        std::string str = create_random_string(20);
                        return cJSON_CreateString(str.c_str());
                    }
                    case 4: {
                        std::string raw = create_random_string(20);
                        return cJSON_CreateRaw(raw.c_str());
                    }
                    case 5: return cJSON_CreateArray();
                    case 6: return cJSON_CreateObject();
                    case 7: return cJSON_CreateBool(true); // Use True as Invalid representation
                    default: return cJSON_CreateNull();
                }
            };
            
            json_a = create_item_by_type(type_a);
            json_b = create_item_by_type(type_b);
            
            if (json_a && json_b) {
                compare_result = cJSON_Compare(json_a, json_b, case_sensitive);
            }
            
            if (json_a) cJSON_Delete(json_a);
            if (json_b) cJSON_Delete(json_b);
            break;
        }
        case 2: {
            // Scenario 2: NULL string checks in String/Raw types (target line 3123)
            // Create strings where one or both have NULL valuestring
            if (fdp.ConsumeBool()) {
                // Use CreateStringReference with NULL
                json_a = cJSON_CreateStringReference(NULL);
            } else {
                std::string str = create_random_string(20);
                json_a = cJSON_CreateString(str.c_str());
            }
            
            if (fdp.ConsumeBool()) {
                json_b = cJSON_CreateStringReference(NULL);
            } else {
                std::string str = create_random_string(20);
                json_b = cJSON_CreateString(str.c_str());
            }
            
            if (json_a && json_b) {
                compare_result = cJSON_Compare(json_a, json_b, case_sensitive);
            }
            
            if (json_a) cJSON_Delete(json_a);
            if (json_b) cJSON_Delete(json_b);
            break;
        }
        case 3: {
            // Scenario 3: Array length differences (target line 3151)
            // Create arrays with different lengths
            json_a = cJSON_CreateArray();
            json_b = cJSON_CreateArray();
            
            if (json_a && json_b) {
                size_t len_a = fdp.ConsumeIntegralInRange<size_t>(0, 5);
                size_t len_b = fdp.ConsumeIntegralInRange<size_t>(0, 5);
                
                // Ensure different lengths
                if (len_a == len_b) {
                    len_b = len_a + 1;
                }
                
                // Add elements to array A
                for (size_t i = 0; i < len_a && fdp.remaining_bytes() > 0; i++) {
                    cJSON* elem = cJSON_CreateNumber(create_random_number());
                    if (elem) {
                        cJSON_AddItemToArray(json_a, elem);
                    }
                }
                
                // Add elements to array B (different length)
                for (size_t i = 0; i < len_b && fdp.remaining_bytes() > 0; i++) {
                    cJSON* elem = cJSON_CreateNumber(create_random_number());
                    if (elem) {
                        cJSON_AddItemToArray(json_b, elem);
                    }
                }
                
                compare_result = cJSON_Compare(json_a, json_b, case_sensitive);
            }
            
            if (json_a) cJSON_Delete(json_a);
            if (json_b) cJSON_Delete(json_b);
            break;
        }
            
        case 4:
            // Scenario 4: Object key mismatches (target lines 3166 and 3182)
            // Create objects with different/missing keys
            json_a = cJSON_CreateObject();
            json_b = cJSON_CreateObject();
            
            if (json_a && json_b) {
                // Create some key-value pairs
                size_t num_keys = fdp.ConsumeIntegralInRange<size_t>(1, 4);
                std::vector<std::string> keys;
                
                for (size_t i = 0; i < num_keys && fdp.remaining_bytes() > 10; i++) {
                    std::string key = create_random_string(10);
                    keys.push_back(key);
                    
                    // Add to object A
                    cJSON* value = cJSON_CreateNumber(create_random_number());
                    if (value) {
                        cJSON_AddItemToObject(json_a, key.c_str(), value);
                    }
                    
                    // Maybe add to object B (but not always)
                    if (fdp.ConsumeBool() || i == 0) { // Ensure at least one key mismatch
                        value = cJSON_CreateNumber(create_random_number());
                        if (value) {
                            cJSON_AddItemToObject(json_b, key.c_str(), value);
                        }
                    }
                }
                
                // Add an extra key to object B that doesn't exist in A
                if (fdp.ConsumeBool()) {
                    std::string extra_key = create_random_string(10);
                    cJSON* value = cJSON_CreateNumber(create_random_number());
                    if (value) {
                        cJSON_AddItemToObject(json_b, extra_key.c_str(), value);
                    }
                }
                
                compare_result = cJSON_Compare(json_a, json_b, case_sensitive);
            }
            
            if (json_a) cJSON_Delete(json_a);
            if (json_b) cJSON_Delete(json_b);
            break;
            
        case 5:
            // Scenario 5: Case-sensitive vs case-insensitive string comparisons
            // Create objects/strings with case variations
            if (fdp.ConsumeBool()) {
                // Test with string objects
                std::string str1 = "TestString";
                std::string str2 = fdp.ConsumeBool() ? "TestString" : "teststring";
                
                json_a = cJSON_CreateString(str1.c_str());
                json_b = cJSON_CreateString(str2.c_str());
            } else {
                // Test with object keys
                json_a = cJSON_CreateObject();
                json_b = cJSON_CreateObject();
                
                std::string key1 = "KeyName";
                std::string key2 = fdp.ConsumeBool() ? "KeyName" : "keyname";
                
                cJSON* value1 = cJSON_CreateNumber(1.0);
                cJSON* value2 = cJSON_CreateNumber(2.0);
                
                if (json_a && value1) cJSON_AddItemToObject(json_a, key1.c_str(), value1);
                if (json_b && value2) cJSON_AddItemToObject(json_b, key2.c_str(), value2);
            }
            
            if (json_a && json_b) {
                compare_result = cJSON_Compare(json_a, json_b, case_sensitive);
            }
            
            if (json_a) cJSON_Delete(json_a);
            if (json_b) cJSON_Delete(json_b);
            break;
            
        case 6:
            // Scenario 6: Empty arrays vs arrays with elements
            json_a = cJSON_CreateArray();
            json_b = cJSON_CreateArray();
            
            if (json_a && json_b) {
                // Make one array empty, the other with elements
                if (fdp.ConsumeBool()) {
                    // Add elements to array B only
                    size_t len = fdp.ConsumeIntegralInRange<size_t>(1, 3);
                    for (size_t i = 0; i < len && fdp.remaining_bytes() > 0; i++) {
                        cJSON* elem = cJSON_CreateNumber(create_random_number());
                        if (elem) {
                            cJSON_AddItemToArray(json_b, elem);
                        }
                    }
                } else {
                    // Add elements to array A only
                    size_t len = fdp.ConsumeIntegralInRange<size_t>(1, 3);
                    for (size_t i = 0; i < len && fdp.remaining_bytes() > 0; i++) {
                        cJSON* elem = cJSON_CreateNumber(create_random_number());
                        if (elem) {
                            cJSON_AddItemToArray(json_a, elem);
                        }
                    }
                }
                
                compare_result = cJSON_Compare(json_a, json_b, case_sensitive);
            }
            
            if (json_a) cJSON_Delete(json_a);
            if (json_b) cJSON_Delete(json_b);
            break;
            
        case 7: {
            // Scenario 7: Recursive/nested structures with depth limits
            // Create nested objects/arrays - use a simpler approach without recursion
            // to avoid complexity issues
            size_t depth = fdp.ConsumeIntegralInRange<size_t>(0, 2); // Limit depth to avoid recursion
            
            // Create first structure
            if (depth == 0) {
                json_a = cJSON_CreateNumber(create_random_number());
            } else if (depth == 1) {
                json_a = cJSON_CreateObject();
                if (json_a) {
                    for (int i = 0; i < 2 && fdp.remaining_bytes() > 10; i++) {
                        std::string key = "key" + std::to_string(i);
                        cJSON* value = cJSON_CreateNumber(create_random_number());
                        if (value) {
                            cJSON_AddItemToObject(json_a, key.c_str(), value);
                        }
                    }
                }
            } else { // depth == 2
                json_a = cJSON_CreateArray();
                if (json_a) {
                    for (int i = 0; i < 2 && fdp.remaining_bytes() > 10; i++) {
                        cJSON* obj = cJSON_CreateObject();
                        if (obj) {
                            std::string key = "nested_key" + std::to_string(i);
                            cJSON* value = cJSON_CreateNumber(create_random_number());
                            if (value) {
                                cJSON_AddItemToObject(obj, key.c_str(), value);
                                cJSON_AddItemToArray(json_a, obj);
                            } else {
                                cJSON_Delete(obj);
                            }
                        }
                    }
                }
            }
            
            // Create second structure with possibly different depth
            size_t depth2 = fdp.ConsumeBool() ? depth : fdp.ConsumeIntegralInRange<size_t>(0, 2);
            
            if (depth2 == 0) {
                json_b = cJSON_CreateNumber(create_random_number());
            } else if (depth2 == 1) {
                json_b = cJSON_CreateObject();
                if (json_b) {
                    for (int i = 0; i < 2 && fdp.remaining_bytes() > 10; i++) {
                        std::string key = "key" + std::to_string(i);
                        cJSON* value = cJSON_CreateNumber(create_random_number());
                        if (value) {
                            cJSON_AddItemToObject(json_b, key.c_str(), value);
                        }
                    }
                }
            } else { // depth2 == 2
                json_b = cJSON_CreateArray();
                if (json_b) {
                    for (int i = 0; i < 2 && fdp.remaining_bytes() > 10; i++) {
                        cJSON* obj = cJSON_CreateObject();
                        if (obj) {
                            std::string key = "nested_key" + std::to_string(i);
                            cJSON* value = cJSON_CreateNumber(create_random_number());
                            if (value) {
                                cJSON_AddItemToObject(obj, key.c_str(), value);
                                cJSON_AddItemToArray(json_b, obj);
                            } else {
                                cJSON_Delete(obj);
                            }
                        }
                    }
                }
            }
            
            if (json_a && json_b) {
                compare_result = cJSON_Compare(json_a, json_b, case_sensitive);
            }
            
            if (json_a) cJSON_Delete(json_a);
            if (json_b) cJSON_Delete(json_b);
            break;
        }
            
        case 8:
            // Scenario 8: Same object comparison (target line 3101)
            json_a = cJSON_CreateObject();
            if (json_a) {
                // Add some items
                for (int i = 0; i < 2 && fdp.remaining_bytes() > 10; i++) {
                    std::string key = create_random_string(8);
                    cJSON* value = cJSON_CreateNumber(create_random_number());
                    if (value) {
                        cJSON_AddItemToObject(json_a, key.c_str(), value);
                    }
                }
                
                // Compare with itself
                compare_result = cJSON_Compare(json_a, json_a, case_sensitive);
                cJSON_Delete(json_a);
            }
            break;
            
        case 9: {
            // Scenario 9: Number comparison with special values
            // Test compare_double function paths
            double special_values[] = {NAN, INFINITY, -INFINITY, 0.0, -0.0, 1.0, -1.0};
            size_t idx_a = fdp.ConsumeIntegralInRange<size_t>(0, 6);
            size_t idx_b = fdp.ConsumeIntegralInRange<size_t>(0, 6);
            
            json_a = cJSON_CreateNumber(special_values[idx_a]);
            json_b = cJSON_CreateNumber(special_values[idx_b]);
            
            if (json_a && json_b) {
                compare_result = cJSON_Compare(json_a, json_b, case_sensitive);
            }
            
            if (json_a) cJSON_Delete(json_a);
            if (json_b) cJSON_Delete(json_b);
            break;
        }
        case 10: {
            // Scenario 10: Raw string comparison (similar to String but separate type)
            std::string raw1 = create_random_string(20);
            std::string raw2 = create_random_string(20);
            
            // Make them equal sometimes
            if (fdp.ConsumeBool()) {
                raw2 = raw1;
            }
            
            json_a = cJSON_CreateRaw(raw1.c_str());
            json_b = cJSON_CreateRaw(raw2.c_str());
            
            if (json_a && json_b) {
                compare_result = cJSON_Compare(json_a, json_b, case_sensitive);
            }
            
            if (json_a) cJSON_Delete(json_a);
            if (json_b) cJSON_Delete(json_b);
            break;
        }
            break;
            
        default:
            // Scenarios 11-19: Complex combinations of the above
            // Mix and match different test cases
            uint8_t sub_scenario = scenario % 5;
            
            switch (sub_scenario) {
                case 0: {
                    // Mixed types in arrays
                    json_a = cJSON_CreateArray();
                    json_b = cJSON_CreateArray();
                    
                    if (json_a && json_b) {
                        size_t len = fdp.ConsumeIntegralInRange<size_t>(1, 3);
                        for (size_t i = 0; i < len && fdp.remaining_bytes() > 10; i++) {
                            uint8_t type = fdp.ConsumeIntegral<uint8_t>() % 4;
                            cJSON* elem_a = NULL;
                            cJSON* elem_b = NULL;
                            
                            switch (type) {
                                case 0: 
                                    elem_a = cJSON_CreateNull();
                                    elem_b = cJSON_CreateNull();
                                    break;
                                case 1:
                                    elem_a = cJSON_CreateBool(fdp.ConsumeBool());
                                    elem_b = cJSON_CreateBool(fdp.ConsumeBool());
                                    break;
                                case 2:
                                    elem_a = cJSON_CreateNumber(create_random_number());
                                    elem_b = cJSON_CreateNumber(create_random_number());
                                    break;
                                case 3: {
                                    std::string str = create_random_string(10);
                                    elem_a = cJSON_CreateString(str.c_str());
                                    elem_b = cJSON_CreateString(str.c_str());
                                    break;
                                }
                            }
                            
                            if (elem_a) cJSON_AddItemToArray(json_a, elem_a);
                            if (elem_b) cJSON_AddItemToArray(json_b, elem_b);
                        }
                        
                        compare_result = cJSON_Compare(json_a, json_b, case_sensitive);
                    }
                    break;
                }
                    
                case 1: {
                    // Objects with nested arrays
                    json_a = cJSON_CreateObject();
                    json_b = cJSON_CreateObject();
                    
                    if (json_a && json_b) {
                        // Add a nested array to both objects
                        cJSON* array_a = cJSON_CreateArray();
                        cJSON* array_b = cJSON_CreateArray();
                        
                        if (array_a && array_b) {
                            size_t len = fdp.ConsumeIntegralInRange<size_t>(0, 2);
                            for (size_t i = 0; i < len && fdp.remaining_bytes() > 0; i++) {
                                cJSON* elem = cJSON_CreateNumber(create_random_number());
                                if (elem) {
                                    cJSON_AddItemToArray(array_a, elem);
                                    
                                    // Sometimes add different value to array_b
                                    if (fdp.ConsumeBool()) {
                                        elem = cJSON_CreateNumber(create_random_number());
                                    }
                                    if (elem) {
                                        cJSON_AddItemToArray(array_b, elem);
                                    }
                                }
                            }
                            
                            cJSON_AddItemToObject(json_a, "nested", array_a);
                            cJSON_AddItemToObject(json_b, "nested", array_b);
                        }
                        
                        compare_result = cJSON_Compare(json_a, json_b, case_sensitive);
                    }
                    break;
                }
                    
                case 2: {
                    // Test with allocation failure simulation
                    // This tests cJSON_malloc failure paths indirectly
                    // Create complex structure that might trigger allocations
                    size_t num_items = fdp.ConsumeIntegralInRange<size_t>(1, 5);
                    
                    json_a = cJSON_CreateArray();
                    if (json_a) {
                        for (size_t i = 0; i < num_items && fdp.remaining_bytes() > 10; i++) {
                            cJSON* obj = cJSON_CreateObject();
                            if (obj) {
                                for (int j = 0; j < 2 && fdp.remaining_bytes() > 5; j++) {
                                    std::string key = create_random_string(5);
                                    cJSON* value = cJSON_CreateString(create_random_string(8).c_str());
                                    if (value) {
                                        cJSON_AddItemToObject(obj, key.c_str(), value);
                                    }
                                }
                                cJSON_AddItemToArray(json_a, obj);
                            }
                        }
                        
                        // Compare with itself (should succeed if allocations worked)
                        compare_result = cJSON_Compare(json_a, json_a, case_sensitive);
                        cJSON_Delete(json_a);
                    }
                    break;
                }
                    
                case 3: {
                    // Arrays with objects inside
                    json_a = cJSON_CreateArray();
                    json_b = cJSON_CreateArray();
                    
                    if (json_a && json_b) {
                        size_t len = fdp.ConsumeIntegralInRange<size_t>(1, 2);
                        for (size_t i = 0; i < len && fdp.remaining_bytes() > 20; i++) {
                            cJSON* obj_a = cJSON_CreateObject();
                            cJSON* obj_b = cJSON_CreateObject();
                            
                            if (obj_a && obj_b) {
                                std::string key = create_random_string(5);
                                cJSON* val_a = cJSON_CreateNumber(create_random_number());
                                cJSON* val_b = cJSON_CreateNumber(create_random_number());
                                
                                if (val_a) cJSON_AddItemToObject(obj_a, key.c_str(), val_a);
                                if (val_b) cJSON_AddItemToObject(obj_b, key.c_str(), val_b);
                                
                                cJSON_AddItemToArray(json_a, obj_a);
                                cJSON_AddItemToArray(json_b, obj_b);
                            }
                        }
                        
                        compare_result = cJSON_Compare(json_a, json_b, case_sensitive);
                    }
                    break;
                }
                    
                case 4: {
                    // Simple boolean and null comparisons
                    uint8_t simple_type = fdp.ConsumeIntegral<uint8_t>() % 3;
                    
                    switch (simple_type) {
                        case 0:
                            json_a = cJSON_CreateNull();
                            json_b = cJSON_CreateNull();
                            break;
                        case 1:
                            json_a = cJSON_CreateBool(true);
                            json_b = cJSON_CreateBool(fdp.ConsumeBool());
                            break;
                        case 2:
                            json_a = cJSON_CreateBool(false);
                            json_b = cJSON_CreateBool(fdp.ConsumeBool());
                            break;
                    }
                    
                    if (json_a && json_b) {
                        compare_result = cJSON_Compare(json_a, json_b, case_sensitive);
                    }
                    
                    if (json_a) cJSON_Delete(json_a);
                    if (json_b) cJSON_Delete(json_b);
                    break;
                }
            }
            break;
    }
    
    // Reset hooks to default to avoid affecting other tests
    cJSON_Hooks default_hooks;
    default_hooks.malloc_fn = malloc;
    default_hooks.free_fn = free;
    cJSON_InitHooks(&default_hooks);
    
    return 0;
}

} // namespace harness_012
