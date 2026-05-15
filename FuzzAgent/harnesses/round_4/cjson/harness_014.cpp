#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <cmath>
#include <fuzzer/FuzzedDataProvider.h>
#include "cjson/cJSON.h"

namespace harness_014 {

// Helper function to create deeply nested JSON structures
cJSON* create_nested_structure(FuzzedDataProvider& fdp, int depth, int max_depth) {
    if (depth >= max_depth || fdp.remaining_bytes() < 8) {
        // Create leaf node
        uint8_t leaf_type = fdp.ConsumeIntegral<uint8_t>() % 7;
        switch (leaf_type) {
            case 0: return cJSON_CreateNull();
            case 1: return cJSON_CreateBool(fdp.ConsumeBool());
            case 2: return cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
            case 3: {
                size_t str_len = fdp.ConsumeIntegralInRange<size_t>(1, 64);
                if (fdp.remaining_bytes() < str_len) return cJSON_CreateNull();
                std::string str_val = fdp.ConsumeBytesAsString(str_len);
                return cJSON_CreateString(str_val.c_str());
            }
            case 4: {
                size_t raw_len = fdp.ConsumeIntegralInRange<size_t>(1, 64);
                if (fdp.remaining_bytes() < raw_len) return cJSON_CreateNull();
                std::string raw_val = fdp.ConsumeBytesAsString(raw_len);
                return cJSON_CreateRaw(raw_val.c_str());
            }
            case 5: return cJSON_CreateArray();  // Empty array at leaf
            case 6: return cJSON_CreateObject(); // Empty object at leaf
            default: return cJSON_CreateNull();
        }
    }
    
    // Create container node (array or object)
    bool is_array = fdp.ConsumeBool();
    
    if (is_array) {
        cJSON* array = cJSON_CreateArray();
        if (!array) return NULL;
        
        int num_elements = fdp.ConsumeIntegralInRange<int>(1, 5);
        for (int i = 0; i < num_elements && fdp.remaining_bytes() > 8; i++) {
            cJSON* element = create_nested_structure(fdp, depth + 1, max_depth);
            if (element) {
                cJSON_AddItemToArray(array, element);
            }
        }
        return array;
    } else {
        cJSON* object = cJSON_CreateObject();
        if (!object) return NULL;
        
        int num_properties = fdp.ConsumeIntegralInRange<int>(1, 5);
        for (int i = 0; i < num_properties && fdp.remaining_bytes() > 16; i++) {
            // Create key
            size_t key_len = fdp.ConsumeIntegralInRange<size_t>(1, 32);
            if (fdp.remaining_bytes() < key_len) break;
            std::string key = fdp.ConsumeBytesAsString(key_len);
            
            // Create value
            cJSON* value = create_nested_structure(fdp, depth + 1, max_depth);
            if (value) {
                cJSON_AddItemToObject(object, key.c_str(), value);
            }
        }
        return object;
    }
}

// Helper to create complex mixed-type structures with specific patterns
cJSON* create_mixed_structure(FuzzedDataProvider& fdp) {
    // Create a structure with arrays containing objects, and objects containing arrays
    cJSON* root = cJSON_CreateObject();
    if (!root) return NULL;
    
    // Add some primitive values
    if (fdp.remaining_bytes() > 8) {
        cJSON_AddNumberToObject(root, "num1", fdp.ConsumeFloatingPoint<double>());
    }
    if (fdp.remaining_bytes() > 1) {
        cJSON_AddBoolToObject(root, "bool1", fdp.ConsumeBool());
    }
    
    // Add an array containing mixed types
    if (fdp.remaining_bytes() > 16) {
        cJSON* mixed_array = cJSON_CreateArray();
        if (mixed_array) {
            // Add various types to array
            cJSON_AddItemToArray(mixed_array, cJSON_CreateNull());
            cJSON_AddItemToArray(mixed_array, cJSON_CreateBool(fdp.ConsumeBool()));
            cJSON_AddItemToArray(mixed_array, cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>()));
            
            // Add a string
            size_t str_len = fdp.ConsumeIntegralInRange<size_t>(1, 32);
            if (fdp.remaining_bytes() >= str_len) {
                std::string str_val = fdp.ConsumeBytesAsString(str_len);
                cJSON_AddItemToArray(mixed_array, cJSON_CreateString(str_val.c_str()));
            }
            
            // Add an object inside the array
            cJSON* nested_obj = cJSON_CreateObject();
            if (nested_obj && fdp.remaining_bytes() > 8) {
                cJSON_AddNumberToObject(nested_obj, "nested_num", fdp.ConsumeFloatingPoint<double>());
                cJSON_AddItemToArray(mixed_array, nested_obj);
            }
            
            cJSON_AddItemToObject(root, "mixed_array", mixed_array);
        }
    }
    
    // Add an object containing an array
    if (fdp.remaining_bytes() > 16) {
        cJSON* obj_with_array = cJSON_CreateObject();
        if (obj_with_array) {
            cJSON* inner_array = cJSON_CreateArray();
            if (inner_array) {
                int array_size = fdp.ConsumeIntegralInRange<int>(1, 3);
                for (int i = 0; i < array_size && fdp.remaining_bytes() > 8; i++) {
                    cJSON_AddItemToArray(inner_array, cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>()));
                }
                cJSON_AddItemToObject(obj_with_array, "inner_array", inner_array);
            }
            cJSON_AddItemToObject(root, "obj_with_array", obj_with_array);
        }
    }
    
    return root;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need sufficient input for complex nested structure creation and comparison
    if (size < 128) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Determine test scenario
    uint8_t scenario = fdp.ConsumeIntegral<uint8_t>() % 8;
    cJSON_bool case_sensitive = fdp.ConsumeBool();
    
    cJSON* json_a = NULL;
    cJSON* json_b = NULL;
    cJSON_bool compare_result = false;
    
    switch (scenario) {
        case 0: {
            // Scenario 0: Deeply nested structures (5+ levels)
            int max_depth = fdp.ConsumeIntegralInRange<int>(3, 7);
            json_a = create_nested_structure(fdp, 0, max_depth);
            
            // Create similar but potentially different structure
            // Reset some state to allow creating different structure
            // We'll create a new structure with potentially different content
            int max_depth_b = fdp.ConsumeIntegralInRange<int>(3, 7);
            json_b = create_nested_structure(fdp, 0, max_depth_b);
            
            if (json_a && json_b) {
                compare_result = cJSON_Compare(json_a, json_b, case_sensitive);
            }
            break;
        }
        
        case 1: {
            // Scenario 1: Mixed structures with arrays in objects and objects in arrays
            json_a = create_mixed_structure(fdp);
            
            // Create a similar mixed structure
            json_b = create_mixed_structure(fdp);
            
            if (json_a && json_b) {
                compare_result = cJSON_Compare(json_a, json_b, case_sensitive);
            }
            break;
        }
        
        case 2: {
            // Scenario 2: Identical complex structures (should compare equal)
            // Create a structure and duplicate it
            cJSON* original = create_nested_structure(fdp, 0, 4);
            if (original) {
                // Duplicate the structure
                json_a = original;
                json_b = cJSON_Duplicate(original, 1);  // Deep copy
                
                if (json_a && json_b) {
                    compare_result = cJSON_Compare(json_a, json_b, case_sensitive);
                    // Should be true for identical structures
                }
            }
            break;
        }
        
        case 3: {
            // Scenario 3: Structures with NaN and Infinity values
            json_a = cJSON_CreateObject();
            json_b = cJSON_CreateObject();
            
            if (json_a && json_b) {
                // Add NaN values
                cJSON_AddNumberToObject(json_a, "nan_val", NAN);
                cJSON_AddNumberToObject(json_b, "nan_val", NAN);
                
                // Add Infinity values
                cJSON_AddNumberToObject(json_a, "inf_val", INFINITY);
                cJSON_AddNumberToObject(json_b, "inf_val", INFINITY);
                
                // Add -Infinity values
                cJSON_AddNumberToObject(json_a, "neg_inf_val", -INFINITY);
                cJSON_AddNumberToObject(json_b, "neg_inf_val", -INFINITY);
                
                compare_result = cJSON_Compare(json_a, json_b, case_sensitive);
            }
            break;
        }
        
        case 4: {
            // Scenario 4: Large arrays with mixed content
            json_a = cJSON_CreateArray();
            json_b = cJSON_CreateArray();
            
            if (json_a && json_b) {
                int array_size = fdp.ConsumeIntegralInRange<int>(5, 15);
                
                // Fill arrays with same size but potentially different content
                for (int i = 0; i < array_size && fdp.remaining_bytes() > 8; i++) {
                    uint8_t elem_type = fdp.ConsumeIntegral<uint8_t>() % 6;
                    cJSON* elem_a = NULL;
                    cJSON* elem_b = NULL;
                    
                    switch (elem_type) {
                        case 0: elem_a = cJSON_CreateNull(); elem_b = cJSON_CreateNull(); break;
                        case 1: {
                            bool val = fdp.ConsumeBool();
                            elem_a = cJSON_CreateBool(val);
                            elem_b = cJSON_CreateBool(val);
                            break;
                        }
                        case 2: {
                            double val = fdp.ConsumeFloatingPoint<double>();
                            elem_a = cJSON_CreateNumber(val);
                            // Sometimes make second value different
                            if (fdp.ConsumeBool() && fdp.remaining_bytes() > 0) {
                                elem_b = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
                            } else {
                                elem_b = cJSON_CreateNumber(val);
                            }
                            break;
                        }
                        case 3: {
                            size_t str_len = fdp.ConsumeIntegralInRange<size_t>(1, 32);
                            if (fdp.remaining_bytes() >= str_len) {
                                std::string str_val = fdp.ConsumeBytesAsString(str_len);
                                elem_a = cJSON_CreateString(str_val.c_str());
                                // Sometimes make second string different
                                if (fdp.ConsumeBool() && fdp.remaining_bytes() >= str_len) {
                                    std::string str_val2 = fdp.ConsumeBytesAsString(str_len);
                                    elem_b = cJSON_CreateString(str_val2.c_str());
                                } else {
                                    elem_b = cJSON_CreateString(str_val.c_str());
                                }
                            }
                            break;
                        }
                        case 4: {
                            cJSON* nested_obj_a = cJSON_CreateObject();
                            cJSON* nested_obj_b = cJSON_CreateObject();
                            if (nested_obj_a && nested_obj_b) {
                                cJSON_AddNumberToObject(nested_obj_a, "n", fdp.ConsumeFloatingPoint<double>());
                                // Sometimes make nested objects different
                                if (fdp.ConsumeBool()) {
                                    cJSON_AddNumberToObject(nested_obj_b, "n", fdp.ConsumeFloatingPoint<double>());
                                } else {
                                    cJSON_AddNumberToObject(nested_obj_b, "n", fdp.ConsumeFloatingPoint<double>());
                                }
                            }
                            elem_a = nested_obj_a;
                            elem_b = nested_obj_b;
                            break;
                        }
                        case 5: {
                            // Array within array
                            cJSON* inner_array_a = cJSON_CreateArray();
                            cJSON* inner_array_b = cJSON_CreateArray();
                            if (inner_array_a && inner_array_b) {
                                int inner_size = fdp.ConsumeIntegralInRange<int>(1, 3);
                                for (int j = 0; j < inner_size && fdp.remaining_bytes() > 4; j++) {
                                    cJSON_AddItemToArray(inner_array_a, cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>()));
                                    cJSON_AddItemToArray(inner_array_b, cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>()));
                                }
                            }
                            elem_a = inner_array_a;
                            elem_b = inner_array_b;
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
        
        case 5: {
            // Scenario 5: Objects with same keys but different values
            json_a = cJSON_CreateObject();
            json_b = cJSON_CreateObject();
            
            if (json_a && json_b) {
                // Create some common keys
                std::vector<std::string> keys;
                int num_keys = fdp.ConsumeIntegralInRange<int>(3, 8);
                for (int i = 0; i < num_keys && fdp.remaining_bytes() > 8; i++) {
                    size_t key_len = fdp.ConsumeIntegralInRange<size_t>(1, 16);
                    if (fdp.remaining_bytes() >= key_len) {
                        std::string key = fdp.ConsumeBytesAsString(key_len);
                        keys.push_back(key);
                    }
                }
                
                // Add values with same keys but potentially different values
                for (const auto& key : keys) {
                    uint8_t val_type = fdp.ConsumeIntegral<uint8_t>() % 4;
                    
                    switch (val_type) {
                        case 0: {
                            // Same value for both
                            double val = fdp.ConsumeFloatingPoint<double>();
                            cJSON_AddNumberToObject(json_a, key.c_str(), val);
                            cJSON_AddNumberToObject(json_b, key.c_str(), val);
                            break;
                        }
                        case 1: {
                            // Different values
                            cJSON_AddNumberToObject(json_a, key.c_str(), fdp.ConsumeFloatingPoint<double>());
                            cJSON_AddNumberToObject(json_b, key.c_str(), fdp.ConsumeFloatingPoint<double>());
                            break;
                        }
                        case 2: {
                            // Same string
                            size_t str_len = fdp.ConsumeIntegralInRange<size_t>(1, 32);
                            if (fdp.remaining_bytes() >= str_len) {
                                std::string str_val = fdp.ConsumeBytesAsString(str_len);
                                cJSON_AddStringToObject(json_a, key.c_str(), str_val.c_str());
                                cJSON_AddStringToObject(json_b, key.c_str(), str_val.c_str());
                            }
                            break;
                        }
                        case 3: {
                            // Different strings
                            size_t str_len1 = fdp.ConsumeIntegralInRange<size_t>(1, 32);
                            if (fdp.remaining_bytes() >= str_len1) {
                                std::string str_val1 = fdp.ConsumeBytesAsString(str_len1);
                                cJSON_AddStringToObject(json_a, key.c_str(), str_val1.c_str());
                            }
                            size_t str_len2 = fdp.ConsumeIntegralInRange<size_t>(1, 32);
                            if (fdp.remaining_bytes() >= str_len2) {
                                std::string str_val2 = fdp.ConsumeBytesAsString(str_len2);
                                cJSON_AddStringToObject(json_b, key.c_str(), str_val2.c_str());
                            }
                            break;
                        }
                    }
                }
                
                compare_result = cJSON_Compare(json_a, json_b, case_sensitive);
            }
            break;
        }
        
        case 6: {
            // Scenario 6: Complex object with subset/superset relationships
            // Create object A with some properties
            json_a = cJSON_CreateObject();
            if (json_a) {
                int num_props = fdp.ConsumeIntegralInRange<int>(3, 6);
                for (int i = 0; i < num_props && fdp.remaining_bytes() > 16; i++) {
                    size_t key_len = fdp.ConsumeIntegralInRange<size_t>(1, 16);
                    if (fdp.remaining_bytes() >= key_len) {
                        std::string key = fdp.ConsumeBytesAsString(key_len);
                        cJSON_AddNumberToObject(json_a, key.c_str(), fdp.ConsumeFloatingPoint<double>());
                    }
                }
                
                // Create object B as a subset of A (fewer properties)
                json_b = cJSON_CreateObject();
                if (json_b) {
                    // Add some but not all properties from A
                    cJSON* child = json_a->child;
                    int count = 0;
                    int subset_size = fdp.ConsumeIntegralInRange<int>(1, num_props - 1);
                    while (child && count < subset_size) {
                        cJSON_AddNumberToObject(json_b, child->string, fdp.ConsumeFloatingPoint<double>());
                        child = child->next;
                        count++;
                    }
                }
                
                if (json_b) {
                    compare_result = cJSON_Compare(json_a, json_b, case_sensitive);
                    // Should be false for subset/superset
                }
            }
            break;
        }
        
        case 7: {
            // Scenario 7: String comparison edge cases (case sensitivity, NULL strings)
            json_a = cJSON_CreateStringReference(NULL);  // NULL string via reference
            if (fdp.ConsumeBool()) {
                json_b = cJSON_CreateStringReference(NULL);  // Both NULL
            } else {
                size_t str_len = fdp.ConsumeIntegralInRange<size_t>(1, 32);
                if (fdp.remaining_bytes() >= str_len) {
                    std::string str_val = fdp.ConsumeBytesAsString(str_len);
                    json_b = cJSON_CreateString(str_val.c_str());
                } else {
                    json_b = cJSON_CreateString("");
                }
            }
            
            if (json_a && json_b) {
                compare_result = cJSON_Compare(json_a, json_b, case_sensitive);
            }
            break;
        }
    }
    
    // Clean up
    if (json_a) cJSON_Delete(json_a);
    if (json_b) cJSON_Delete(json_b);
    
    return 0;
}

} // namespace harness_014
