#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <cmath>
#include <fuzzer/FuzzedDataProvider.h>
#include "cjson/cJSON.h"

namespace harness_009 {

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need sufficient input for comprehensive comparison testing
    if (size < 100) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Determine test scenario to target specific uncovered branches
    uint8_t scenario = fdp.ConsumeIntegral<uint8_t>() % 16;
    
    // Consume case_sensitive parameter
    cJSON_bool case_sensitive = fdp.ConsumeBool();
    
    // Helper function to create random strings
    auto create_random_string = [&fdp](size_t max_len = 32) -> std::string {
        if (fdp.remaining_bytes() == 0) return "";
        size_t len = fdp.ConsumeIntegralInRange<size_t>(1, max_len);
        if (fdp.remaining_bytes() < len) return "";
        return fdp.ConsumeBytesAsString(len);
    };
    
    // Helper function to create random number (including special values)
    auto create_random_number = [&fdp]() -> double {
        if (fdp.remaining_bytes() < sizeof(double)) return 0.0;
        double val = fdp.ConsumeFloatingPoint<double>();
        
        // Occasionally create special values (NaN, Inf)
        if (fdp.ConsumeBool() && fdp.remaining_bytes() > 0) {
            uint8_t special = fdp.ConsumeIntegral<uint8_t>() % 4;
            switch (special) {
                case 0: return NAN;
                case 1: return INFINITY;
                case 2: return -INFINITY;
                case 3: return 0.0/0.0; // Another NaN
            }
        }
        return val;
    };
    
    cJSON* json_a = NULL;
    cJSON* json_b = NULL;
    cJSON_bool compare_result = false;
    
    switch (scenario) {
        case 0: {
            // Scenario 0: NULL string checks (target line 3124)
            // Create strings where one or both have NULL valuestring
            // Use CreateStringReference to create string objects with NULL valuestring
            json_a = cJSON_CreateStringReference(NULL);  // NULL string via reference
            if (fdp.ConsumeBool()) {
                json_b = cJSON_CreateStringReference(NULL);  // Both NULL via reference
            } else {
                std::string str = create_random_string(20);
                json_b = cJSON_CreateString(str.c_str());  // One NULL, one valid
            }
            
            if (json_a && json_b) {
                compare_result = cJSON_Compare(json_a, json_b, case_sensitive);
            }
            break;
        }
        
        case 1: {
            // Scenario 1: String/Raw type with NULL strings (more targeted)
            uint8_t str_type = fdp.ConsumeIntegral<uint8_t>() % 2;
            
            if (str_type == 0) {
                json_a = cJSON_CreateStringReference(NULL);  // NULL string via reference
                std::string str = create_random_string(20);
                json_b = cJSON_CreateString(str.c_str());
            } else {
                // Raw type - CreateRaw(NULL) might fail, so we need alternative approach
                // Create a valid raw string for both
                std::string str1 = create_random_string(20);
                std::string str2 = str1;
                if (fdp.ConsumeBool()) {
                    str2 = create_random_string(20);  // Different string
                }
                json_a = cJSON_CreateRaw(str1.c_str());
                json_b = cJSON_CreateRaw(str2.c_str());
            }
            
            if (json_a && json_b) {
                compare_result = cJSON_Compare(json_a, json_b, case_sensitive);
            }
            break;
        }
        case 2: {
            // Scenario 2: Array length mismatch with different patterns
            json_a = cJSON_CreateArray();
            json_b = cJSON_CreateArray();
            
            if (json_a && json_b) {
                // Create arrays with intentionally different lengths
                size_t size_a = fdp.ConsumeIntegralInRange<size_t>(0, 10);
                size_t size_b = size_a;  // Start same
                
                // Force different lengths based on input
                if (fdp.ConsumeBool()) {
                    size_b = size_a + 1 + (fdp.ConsumeIntegral<uint8_t>() % 5);
                } else if (size_a > 0) {
                    size_b = size_a - 1 - (fdp.ConsumeIntegral<uint8_t>() % size_a);
                }
                
                // Fill array A
                for (size_t i = 0; i < size_a && fdp.remaining_bytes() > 10; i++) {
                    cJSON* elem = cJSON_CreateNumber(create_random_number());
                    if (elem) cJSON_AddItemToArray(json_a, elem);
                }
                
                // Fill array B with different number of elements
                for (size_t i = 0; i < size_b && fdp.remaining_bytes() > 10; i++) {
                    cJSON* elem = cJSON_CreateNumber(create_random_number());
                    if (elem) cJSON_AddItemToArray(json_b, elem);
                }
                
                compare_result = cJSON_Compare(json_a, json_b, case_sensitive);
            }
            break;
        }
        
        case 3: {
            // Scenario 3: Object subset detection - second iteration (lines 3183, 3188)
            // Create objects where B is a strict superset of A
            json_a = cJSON_CreateObject();
            json_b = cJSON_CreateObject();
            
            if (json_a && json_b) {
                // Create some shared keys first
                std::vector<std::string> shared_keys;
                size_t num_shared = fdp.ConsumeIntegralInRange<size_t>(1, 5);
                
                for (size_t i = 0; i < num_shared && fdp.remaining_bytes() > 20; i++) {
                    std::string key = "shared_" + std::to_string(i);
                    shared_keys.push_back(key);
                    
                    double val = create_random_number();
                    cJSON_AddNumberToObject(json_a, key.c_str(), val);
                    cJSON_AddNumberToObject(json_b, key.c_str(), val);
                }
                
                // Add unique keys ONLY to object B (making B a superset)
                size_t num_unique_b = fdp.ConsumeIntegralInRange<size_t>(1, 5);
                for (size_t i = 0; i < num_unique_b && fdp.remaining_bytes() > 20; i++) {
                    std::string key = "unique_b_" + std::to_string(i);
                    double val = create_random_number();
                    cJSON_AddNumberToObject(json_b, key.c_str(), val);
                    // DON'T add to object A - this creates the subset condition
                }
                
                // This should trigger the second iteration check (lines 3183, 3188)
                compare_result = cJSON_Compare(json_a, json_b, case_sensitive);
            }
            break;
        }
        
        case 4: {
            // Scenario 4: Object superset detection - first iteration already covered
            // But test more complex subset/superset relationships
            json_a = cJSON_CreateObject();
            json_b = cJSON_CreateObject();
            
            if (json_a && json_b) {
                // Create partially overlapping key sets
                std::vector<std::string> all_keys;
                size_t total_keys = fdp.ConsumeIntegralInRange<size_t>(3, 8);
                
                for (size_t i = 0; i < total_keys && fdp.remaining_bytes() > 20; i++) {
                    std::string key = "key_" + std::to_string(i);
                    all_keys.push_back(key);
                }
                
                // Distribute keys between objects
                for (size_t i = 0; i < all_keys.size(); i++) {
                    double val = create_random_number();
                    if (i % 2 == 0) {
                        cJSON_AddNumberToObject(json_a, all_keys[i].c_str(), val);
                    }
                    if (i % 3 == 0) {  // Different distribution pattern
                        cJSON_AddNumberToObject(json_b, all_keys[i].c_str(), val + 0.001); // Different value
                    }
                }
                
                // Add some completely unique keys to both
                std::string unique_a = "unique_a_key";
                std::string unique_b = "unique_b_key";
                cJSON_AddNumberToObject(json_a, unique_a.c_str(), 1.0);
                cJSON_AddNumberToObject(json_b, unique_b.c_str(), 2.0);
                
                compare_result = cJSON_Compare(json_a, json_b, case_sensitive);
            }
            break;
        }
        
        case 5: {
            // Scenario 5: Invalid type comparisons (default switch case)
            // Create cJSON objects with invalid type bits
            json_a = cJSON_CreateNull();
            json_b = cJSON_CreateNull();
            
            if (json_a && json_b) {
                // Manipulate type to be invalid
                // Note: This is hacky but we need to trigger the default case
                // The type check happens before the switch, so we need different approach
                
                // Instead, let's test type mismatches that should fail early
                json_a = cJSON_CreateNumber(42.0);
                json_b = cJSON_CreateString("test");
                
                if (json_a && json_b) {
                    compare_result = cJSON_Compare(json_a, json_b, case_sensitive);
                }
            }
            break;
        }
        
        case 6: {
            // Scenario 6: Number comparisons with subtle differences
            json_a = cJSON_CreateNumber(create_random_number());
            json_b = cJSON_CreateNumber(create_random_number());
            
            // Sometimes make them very close but not equal
            if (fdp.ConsumeBool() && json_a && json_b) {
                double val = create_random_number();
                json_a->valuedouble = val;
                json_b->valuedouble = val + 0.0000000001; // Very small difference
            }
            
            if (json_a && json_b) {
                compare_result = cJSON_Compare(json_a, json_b, case_sensitive);
            }
            break;
        }
        
        case 7: {
            // Scenario 7: String comparisons with case variations
            std::string str1 = create_random_string(20);
            std::string str2 = str1; // Start same
            
            // Modify case if case_sensitive is false and we want to test
            if (!case_sensitive && fdp.ConsumeBool() && !str1.empty()) {
                // Toggle case of first character
                if (islower(str1[0])) {
                    str2[0] = toupper(str1[0]);
                } else if (isupper(str1[0])) {
                    str2[0] = tolower(str1[0]);
                }
            } else if (fdp.ConsumeBool()) {
                // Make completely different
                str2 = create_random_string(20);
            }
            
            json_a = cJSON_CreateString(str1.c_str());
            json_b = cJSON_CreateString(str2.c_str());
            
            if (json_a && json_b) {
                compare_result = cJSON_Compare(json_a, json_b, case_sensitive);
            }
            break;
        }
        
        case 8: {
            // Scenario 8: Deeply nested structures with differences at various levels
            json_a = cJSON_CreateObject();
            json_b = cJSON_CreateObject();
            
            if (json_a && json_b) {
                // Create nested array structures
                cJSON* nested_array_a = cJSON_CreateArray();
                cJSON* nested_array_b = cJSON_CreateArray();
                
                if (nested_array_a && nested_array_b) {
                    // Fill with different depths
                    size_t depth_a = fdp.ConsumeIntegralInRange<size_t>(1, 3);
                    size_t depth_b = fdp.ConsumeIntegralInRange<size_t>(1, 3);
                    
                    cJSON* current_a = nested_array_a;
                    cJSON* current_b = nested_array_b;
                    
                    for (size_t i = 0; i < depth_a && fdp.remaining_bytes() > 10; i++) {
                        cJSON* inner = cJSON_CreateArray();
                        if (inner) {
                            cJSON_AddItemToArray(current_a, inner);
                            current_a = inner;
                        }
                    }
                    
                    for (size_t i = 0; i < depth_b && fdp.remaining_bytes() > 10; i++) {
                        cJSON* inner = cJSON_CreateArray();
                        if (inner) {
                            cJSON_AddItemToArray(current_b, inner);
                            current_b = inner;
                        }
                    }
                    
                    // Add final different values
                    if (depth_a > 0) {
                        cJSON_AddItemToArray(current_a, cJSON_CreateNumber(1.0));
                    }
                    if (depth_b > 0) {
                        cJSON_AddItemToArray(current_b, cJSON_CreateNumber(2.0));
                    }
                    
                    cJSON_AddItemToObject(json_a, "nested", nested_array_a);
                    cJSON_AddItemToObject(json_b, "nested", nested_array_b);
                }
                
                compare_result = cJSON_Compare(json_a, json_b, case_sensitive);
            }
            break;
        }
        
        case 9: {
            // Scenario 9: Mixed type hierarchies
            json_a = cJSON_CreateObject();
            json_b = cJSON_CreateObject();
            
            if (json_a && json_b) {
                // Add array with mixed types to both
                cJSON* mixed_array_a = cJSON_CreateArray();
                cJSON* mixed_array_b = cJSON_CreateArray();
                
                if (mixed_array_a && mixed_array_b) {
                    size_t array_size = fdp.ConsumeIntegralInRange<size_t>(2, 6);
                    
                    for (size_t i = 0; i < array_size && fdp.remaining_bytes() > 10; i++) {
                        uint8_t elem_type = fdp.ConsumeIntegral<uint8_t>() % 6;
                        
                        switch (elem_type) {
                            case 0: 
                                cJSON_AddItemToArray(mixed_array_a, cJSON_CreateNull());
                                cJSON_AddItemToArray(mixed_array_b, cJSON_CreateNull());
                                break;
                            case 1:
                                cJSON_AddItemToArray(mixed_array_a, cJSON_CreateBool(fdp.ConsumeBool()));
                                cJSON_AddItemToArray(mixed_array_b, cJSON_CreateBool(fdp.ConsumeBool()));
                                break;
                            case 2: {
                                double val_a = create_random_number();
                                double val_b = create_random_number();
                                cJSON_AddItemToArray(mixed_array_a, cJSON_CreateNumber(val_a));
                                cJSON_AddItemToArray(mixed_array_b, cJSON_CreateNumber(val_b));
                                break;
                            }
                            case 3: {
                                std::string str = create_random_string(10);
                                cJSON_AddItemToArray(mixed_array_a, cJSON_CreateString(str.c_str()));
                                cJSON_AddItemToArray(mixed_array_b, cJSON_CreateString(str.c_str()));
                                break;
                            }
                            case 4: {
                                cJSON* obj_a = cJSON_CreateObject();
                                cJSON* obj_b = cJSON_CreateObject();
                                if (obj_a && obj_b) {
                                    cJSON_AddNumberToObject(obj_a, "val", create_random_number());
                                    cJSON_AddNumberToObject(obj_b, "val", create_random_number());
                                    cJSON_AddItemToArray(mixed_array_a, obj_a);
                                    cJSON_AddItemToArray(mixed_array_b, obj_b);
                                }
                                break;
                            }
                            case 5: {
                                cJSON* raw_a = cJSON_CreateRaw("raw_data");
                                cJSON* raw_b = cJSON_CreateRaw("raw_data");
                                if (raw_a && raw_b) {
                                    cJSON_AddItemToArray(mixed_array_a, raw_a);
                                    cJSON_AddItemToArray(mixed_array_b, raw_b);
                                }
                                break;
                            }
                        }
                    }
                    
                    cJSON_AddItemToObject(json_a, "mixed", mixed_array_a);
                    cJSON_AddItemToObject(json_b, "mixed", mixed_array_b);
                }
                
                compare_result = cJSON_Compare(json_a, json_b, case_sensitive);
            }
            break;
        }
        
        case 10: {
            // Scenario 10: Edge cases with special numeric values
            json_a = cJSON_CreateNumber(NAN);
            json_b = cJSON_CreateNumber(NAN);
            
            if (json_a && json_b) {
                compare_result = cJSON_Compare(json_a, json_b, case_sensitive);
                
                // Test other combinations
                cJSON_Delete(json_a);
                cJSON_Delete(json_b);
                
                json_a = cJSON_CreateNumber(INFINITY);
                json_b = cJSON_CreateNumber(INFINITY);
                
                if (json_a && json_b) {
                    compare_result = cJSON_Compare(json_a, json_b, case_sensitive);
                }
            }
            break;
        }
        
        case 11: {
            // Scenario 11: Empty vs non-empty strings
            json_a = cJSON_CreateString("");
            std::string str = create_random_string(20);
            json_b = cJSON_CreateString(str.c_str());
            
            if (json_a && json_b) {
                compare_result = cJSON_Compare(json_a, json_b, case_sensitive);
            }
            break;
        }
        
        case 12: {
            // Scenario 12: Raw type comparisons
            std::string raw1 = create_random_string(30);
            std::string raw2 = raw1;
            
            // Sometimes modify slightly
            if (fdp.ConsumeBool() && !raw1.empty()) {
                raw2 = create_random_string(30);
            }
            
            json_a = cJSON_CreateRaw(raw1.c_str());
            json_b = cJSON_CreateRaw(raw2.c_str());
            
            if (json_a && json_b) {
                compare_result = cJSON_Compare(json_a, json_b, case_sensitive);
            }
            break;
        }
        
        case 13: {
            // Scenario 13: Boolean and NULL comparisons
            uint8_t type_combo = fdp.ConsumeIntegral<uint8_t>() % 4;
            
            switch (type_combo) {
                case 0:
                    json_a = cJSON_CreateFalse();
                    json_b = cJSON_CreateFalse();
                    break;
                case 1:
                    json_a = cJSON_CreateTrue();
                    json_b = cJSON_CreateTrue();
                    break;
                case 2:
                    json_a = cJSON_CreateNull();
                    json_b = cJSON_CreateNull();
                    break;
                case 3:
                    json_a = cJSON_CreateFalse();
                    json_b = cJSON_CreateTrue();  // Different booleans
                    break;
            }
            
            if (json_a && json_b) {
                compare_result = cJSON_Compare(json_a, json_b, case_sensitive);
            }
            break;
        }
        
        case 14: {
            // Scenario 14: Complex object with array children having differences
            json_a = cJSON_CreateObject();
            json_b = cJSON_CreateObject();
            
            if (json_a && json_b) {
                // Add multiple array properties with potential differences
                for (int i = 0; i < 3 && fdp.remaining_bytes() > 30; i++) {
                    std::string key = "arr_" + std::to_string(i);
                    
                    cJSON* arr_a = cJSON_CreateArray();
                    cJSON* arr_b = cJSON_CreateArray();
                    
                    if (arr_a && arr_b) {
                        size_t size = fdp.ConsumeIntegralInRange<size_t>(1, 4);
                        
                        for (size_t j = 0; j < size && fdp.remaining_bytes() > 10; j++) {
                            double val = create_random_number();
                            cJSON_AddItemToArray(arr_a, cJSON_CreateNumber(val));
                            
                            // Sometimes use different value in B
                            if (fdp.ConsumeBool() && j == size - 1) {
                                cJSON_AddItemToArray(arr_b, cJSON_CreateNumber(val + 0.5));
                            } else {
                                cJSON_AddItemToArray(arr_b, cJSON_CreateNumber(val));
                            }
                        }
                        
                        cJSON_AddItemToObject(json_a, key.c_str(), arr_a);
                        cJSON_AddItemToObject(json_b, key.c_str(), arr_b);
                    }
                }
                
                compare_result = cJSON_Compare(json_a, json_b, case_sensitive);
            }
            break;
        }
        
        case 15: {
            // Scenario 15: NULL pointer comparisons (should return false)
            uint8_t null_combo = fdp.ConsumeIntegral<uint8_t>() % 4;
            
            switch (null_combo) {
                case 0:
                    // First parameter NULL
                    json_a = NULL;
                    json_b = cJSON_CreateNumber(42.0);
                    break;
                case 1:
                    // Second parameter NULL
                    json_a = cJSON_CreateString("test");
                    json_b = NULL;
                    break;
                case 2:
                    // Both parameters NULL
                    json_a = NULL;
                    json_b = NULL;
                    break;
                case 3:
                    // Valid objects but different types (should also return false)
                    json_a = cJSON_CreateNumber(42.0);
                    json_b = cJSON_CreateString("test");
                    break;
            }
            // cJSON_Compare has NULL checks at the beginning, so it's safe to call
            compare_result = cJSON_Compare(json_a, json_b, case_sensitive);
            break;
        }
    } // End of switch(scenario)
    
    // Cleanup
    if (json_a) cJSON_Delete(json_a);
    if (json_b) cJSON_Delete(json_b);
    
    return 0;
}

} // namespace harness_009
