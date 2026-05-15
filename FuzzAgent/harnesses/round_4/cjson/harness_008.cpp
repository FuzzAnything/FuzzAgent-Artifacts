#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include "cjson/cJSON.h"

namespace harness_008 {

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need sufficient input for creating diverse JSON structures and comparisons
    if (size < 64) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Determine the comparison scenario to test
    uint8_t scenario = fdp.ConsumeIntegral<uint8_t>() % 8;
    
    // Consume case_sensitive parameter
    cJSON_bool case_sensitive = fdp.ConsumeBool();
    
    // Helper function to create random strings for object keys and values
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
            // Scenario 0: Arrays with different lengths (target line 3152)
            // Create two arrays with different number of elements
            json_a = cJSON_CreateArray();
            json_b = cJSON_CreateArray();
            
            if (json_a && json_b) {
                // Determine array sizes (different for mismatch)
                size_t size_a = fdp.ConsumeIntegralInRange<size_t>(1, 10);
                size_t size_b = fdp.ConsumeIntegralInRange<size_t>(1, 10);
                
                // Ensure they are different
                if (size_a == size_b) {
                    size_b = size_a + 1;
                }
                
                // Fill array A
                for (size_t i = 0; i < size_a && fdp.remaining_bytes() > 0; i++) {
                    uint8_t elem_type = fdp.ConsumeIntegral<uint8_t>() % 4;
                    cJSON* elem = NULL;
                    
                    switch (elem_type) {
                        case 0: elem = cJSON_CreateNull(); break;
                        case 1: elem = cJSON_CreateBool(fdp.ConsumeBool()); break;
                        case 2: elem = cJSON_CreateNumber(create_random_number()); break;
                        case 3: {
                            std::string str = create_random_string(20);
                            elem = cJSON_CreateString(str.c_str());
                            break;
                        }
                    }
                    
                    if (elem) {
                        cJSON_AddItemToArray(json_a, elem);
                    }
                }
                
                // Fill array B with different size
                for (size_t i = 0; i < size_b && fdp.remaining_bytes() > 0; i++) {
                    uint8_t elem_type = fdp.ConsumeIntegral<uint8_t>() % 4;
                    cJSON* elem = NULL;
                    
                    switch (elem_type) {
                        case 0: elem = cJSON_CreateNull(); break;
                        case 1: elem = cJSON_CreateBool(fdp.ConsumeBool()); break;
                        case 2: elem = cJSON_CreateNumber(create_random_number()); break;
                        case 3: {
                            std::string str = create_random_string(20);
                            elem = cJSON_CreateString(str.c_str());
                            break;
                        }
                    }
                    
                    if (elem) {
                        cJSON_AddItemToArray(json_b, elem);
                    }
                }
                
                // Compare arrays with different lengths - should return false
                compare_result = cJSON_Compare(json_a, json_b, case_sensitive);
            }
            break;
        }
        
        case 1: {
            // Scenario 1: Objects with different key sets (target lines 3183, 3188)
            // Create two objects with some overlapping and some different keys
            json_a = cJSON_CreateObject();
            json_b = cJSON_CreateObject();
            
            if (json_a && json_b) {
                // Create some shared keys
                std::vector<std::string> shared_keys;
                size_t num_shared = fdp.ConsumeIntegralInRange<size_t>(1, 5);
                
                for (size_t i = 0; i < num_shared && fdp.remaining_bytes() > 0; i++) {
                    std::string key = create_random_string(15);
                    if (!key.empty()) {
                        shared_keys.push_back(key);
                        
                        // Add same key with potentially different values to both objects
                        double val_a = create_random_number();
                        double val_b = create_random_number();
                        
                        cJSON_AddNumberToObject(json_a, key.c_str(), val_a);
                        cJSON_AddNumberToObject(json_b, key.c_str(), val_b);
                    }
                }
                
                // Add unique keys to object A
                size_t num_unique_a = fdp.ConsumeIntegralInRange<size_t>(1, 5);
                for (size_t i = 0; i < num_unique_a && fdp.remaining_bytes() > 0; i++) {
                    std::string key = create_random_string(15);
                    if (!key.empty()) {
                        double val = create_random_number();
                        cJSON_AddNumberToObject(json_a, key.c_str(), val);
                        // Don't add to object B - this creates key mismatch
                    }
                }
                
                // Add unique keys to object B  
                size_t num_unique_b = fdp.ConsumeIntegralInRange<size_t>(1, 5);
                for (size_t i = 0; i < num_unique_b && fdp.remaining_bytes() > 0; i++) {
                    std::string key = create_random_string(15);
                    if (!key.empty()) {
                        double val = create_random_number();
                        cJSON_AddNumberToObject(json_b, key.c_str(), val);
                        // Don't add to object A - this creates key mismatch
                    }
                }
                
                // Compare objects with different key sets - should return false
                compare_result = cJSON_Compare(json_a, json_b, case_sensitive);
            }
            break;
        }
        
        case 2: {
            // Scenario 2: Nested structures with variations at different depths
            // Create complex nested JSON structures with subtle differences
            json_a = cJSON_CreateObject();
            json_b = cJSON_CreateObject();
            
            if (json_a && json_b) {
                // Add top-level properties
                std::string key1 = create_random_string(10);
                std::string key2 = create_random_string(10);
                
                if (!key1.empty() && !key2.empty()) {
                    // Create nested array for object A
                    cJSON* nested_array_a = cJSON_CreateArray();
                    if (nested_array_a) {
                        size_t array_size = fdp.ConsumeIntegralInRange<size_t>(1, 5);
                        for (size_t i = 0; i < array_size && fdp.remaining_bytes() > 0; i++) {
                            cJSON* num = cJSON_CreateNumber(create_random_number());
                            if (num) cJSON_AddItemToArray(nested_array_a, num);
                        }
                        cJSON_AddItemToObject(json_a, key1.c_str(), nested_array_a);
                    }
                    
                    // Create nested array for object B with DIFFERENT size
                    cJSON* nested_array_b = cJSON_CreateArray();
                    if (nested_array_b) {
                        size_t array_size = fdp.ConsumeIntegralInRange<size_t>(1, 5);
                        // Ensure different size if possible
                        if (array_size == 1) array_size = 2;
                        for (size_t i = 0; i < array_size && fdp.remaining_bytes() > 0; i++) {
                            cJSON* num = cJSON_CreateNumber(create_random_number());
                            if (num) cJSON_AddItemToArray(nested_array_b, num);
                        }
                        cJSON_AddItemToObject(json_b, key1.c_str(), nested_array_b);
                    }
                    
                    // Add second nested object with same structure but different values
                    cJSON* nested_obj_a = cJSON_CreateObject();
                    cJSON* nested_obj_b = cJSON_CreateObject();
                    
                    if (nested_obj_a && nested_obj_b) {
                        std::string nested_key = create_random_string(8);
                        if (!nested_key.empty()) {
                            cJSON_AddStringToObject(nested_obj_a, nested_key.c_str(), 
                                                   create_random_string(15).c_str());
                            cJSON_AddStringToObject(nested_obj_b, nested_key.c_str(),
                                                   create_random_string(15).c_str()); // Different value
                        }
                        cJSON_AddItemToObject(json_a, key2.c_str(), nested_obj_a);
                        cJSON_AddItemToObject(json_b, key2.c_str(), nested_obj_b);
                    }
                    
                    compare_result = cJSON_Compare(json_a, json_b, case_sensitive);
                }
            }
            break;
        }
        
        case 3: {
            // Scenario 3: String comparison with case sensitivity variations
            // Test cJSON_String comparison paths
            std::string str_a = create_random_string(20);
            std::string str_b = str_a; // Start with same string
            
            // Modify str_b based on case_sensitive parameter
            if (!str_b.empty() && fdp.remaining_bytes() > 0) {
                // With 50% probability, make strings different
                if (fdp.ConsumeBool()) {
                    // Create different string
                    str_b = create_random_string(20);
                } else if (str_b.length() > 0) {
                    // Modify case of first character to test case sensitivity
                    if (isalpha(str_b[0])) {
                        if (islower(str_b[0])) {
                            str_b[0] = toupper(str_b[0]);
                        } else {
                            str_b[0] = tolower(str_b[0]);
                        }
                    }
                }
            }
            
            json_a = cJSON_CreateString(str_a.c_str());
            json_b = cJSON_CreateString(str_b.c_str());
            
            if (json_a && json_b) {
                compare_result = cJSON_Compare(json_a, json_b, case_sensitive);
                
                // Also test with swapped case_sensitive
                cJSON_bool opposite_case = !case_sensitive;
                cJSON_bool compare_opposite = cJSON_Compare(json_a, json_b, opposite_case);
            }
            break;
        }
        
        case 4: {
            // Scenario 4: Number comparison with precision differences
            // Test cJSON_Number comparison paths including compare_double
            double num_a = create_random_number();
            double num_b = num_a;
            
            // With probability, make numbers slightly different
            if (fdp.ConsumeBool() && fdp.remaining_bytes() > 0) {
                // Add small epsilon to create "almost equal" numbers
                double epsilon = fdp.ConsumeFloatingPoint<double>();
                // Scale epsilon to be very small
                epsilon = epsilon * 1e-10;
                num_b = num_a + epsilon;
            }
            
            json_a = cJSON_CreateNumber(num_a);
            json_b = cJSON_CreateNumber(num_b);
            
            if (json_a && json_b) {
                compare_result = cJSON_Compare(json_a, json_b, case_sensitive);
            }
            break;
        }
        
        case 5: {
            // Scenario 5: Mixed type comparisons (should return false)
            // Compare objects of completely different types
            uint8_t type_a = fdp.ConsumeIntegral<uint8_t>() % 7;
            uint8_t type_b = fdp.ConsumeIntegral<uint8_t>() % 7;
            
            // Ensure different types
            while (type_b == type_a && fdp.remaining_bytes() > 0) {
                type_b = fdp.ConsumeIntegral<uint8_t>() % 7;
            }
            
            // Create item A
            switch (type_a) {
                case 0: json_a = cJSON_CreateNull(); break;
                case 1: json_a = cJSON_CreateBool(fdp.ConsumeBool()); break;
                case 2: json_a = cJSON_CreateNumber(create_random_number()); break;
                case 3: {
                    std::string str = create_random_string(15);
                    json_a = cJSON_CreateString(str.c_str());
                    break;
                }
                case 4: json_a = cJSON_CreateArray(); break;
                case 5: json_a = cJSON_CreateObject(); break;
                case 6: {
                    std::string raw = create_random_string(20);
                    json_a = cJSON_CreateRaw(raw.c_str());
                    break;
                }
            }
            
            // Create item B (different type)
            switch (type_b) {
                case 0: json_b = cJSON_CreateNull(); break;
                case 1: json_b = cJSON_CreateBool(fdp.ConsumeBool()); break;
                case 2: json_b = cJSON_CreateNumber(create_random_number()); break;
                case 3: {
                    std::string str = create_random_string(15);
                    json_b = cJSON_CreateString(str.c_str());
                    break;
                }
                case 4: json_b = cJSON_CreateArray(); break;
                case 5: json_b = cJSON_CreateObject(); break;
                case 6: {
                    std::string raw = create_random_string(20);
                    json_b = cJSON_CreateRaw(raw.c_str());
                    break;
                }
            }
            
            if (json_a && json_b) {
                compare_result = cJSON_Compare(json_a, json_b, case_sensitive);
            }
            break;
        }
        
        case 6: {
            // Scenario 6: Identical structures (should return true)
            // Create identical JSON structures and compare
            uint8_t structure_type = fdp.ConsumeIntegral<uint8_t>() % 3;
            
            if (structure_type == 0) {
                // Identical arrays
                json_a = cJSON_CreateArray();
                json_b = cJSON_CreateArray();
                
                if (json_a && json_b) {
                    size_t array_size = fdp.ConsumeIntegralInRange<size_t>(1, 5);
                    for (size_t i = 0; i < array_size && fdp.remaining_bytes() > 0; i++) {
                        double num = create_random_number();
                        cJSON* elem_a = cJSON_CreateNumber(num);
                        cJSON* elem_b = cJSON_CreateNumber(num); // Same number
                        
                        if (elem_a) cJSON_AddItemToArray(json_a, elem_a);
                        if (elem_b) cJSON_AddItemToArray(json_b, elem_b);
                    }
                }
            } else if (structure_type == 1) {
                // Identical objects
                json_a = cJSON_CreateObject();
                json_b = cJSON_CreateObject();
                
                if (json_a && json_b) {
                    size_t num_keys = fdp.ConsumeIntegralInRange<size_t>(1, 5);
                    for (size_t i = 0; i < num_keys && fdp.remaining_bytes() > 0; i++) {
                        std::string key = create_random_string(10);
                        double value = create_random_number();
                        
                        if (!key.empty()) {
                            cJSON_AddNumberToObject(json_a, key.c_str(), value);
                            cJSON_AddNumberToObject(json_b, key.c_str(), value); // Same key-value pair
                        }
                    }
                }
            } else {
                // Identical nested structure
                json_a = cJSON_CreateObject();
                json_b = cJSON_CreateObject();
                
                if (json_a && json_b) {
                    std::string key = create_random_string(8);
                    if (!key.empty()) {
                        cJSON* nested_a = cJSON_CreateArray();
                        cJSON* nested_b = cJSON_CreateArray();
                        
                        if (nested_a && nested_b) {
                            size_t array_size = fdp.ConsumeIntegralInRange<size_t>(1, 3);
                            for (size_t i = 0; i < array_size && fdp.remaining_bytes() > 0; i++) {
                                std::string str = create_random_string(10);
                                cJSON_AddItemToArray(nested_a, cJSON_CreateString(str.c_str()));
                                cJSON_AddItemToArray(nested_b, cJSON_CreateString(str.c_str())); // Same string
                            }
                            
                            cJSON_AddItemToObject(json_a, key.c_str(), nested_a);
                            cJSON_AddItemToObject(json_b, key.c_str(), nested_b);
                        }
                    }
                }
            }
            
            if (json_a && json_b) {
                compare_result = cJSON_Compare(json_a, json_b, case_sensitive);
                // Should be true for identical structures
            }
            break;
        }
        
        case 7: {
            // Scenario 7: Edge cases - NULL comparisons, same pointer comparisons
            uint8_t edge_case = fdp.ConsumeIntegral<uint8_t>() % 4;
            
            switch (edge_case) {
                case 0: {
                    // Compare NULL with NULL
                    compare_result = cJSON_Compare(NULL, NULL, case_sensitive);
                    break;
                }
                case 1: {
                    // Compare NULL with valid object
                    json_a = cJSON_CreateNull();
                    compare_result = cJSON_Compare(NULL, json_a, case_sensitive);
                    compare_result = cJSON_Compare(json_a, NULL, case_sensitive);
                    break;
                }
                case 2: {
                    // Compare object with itself (same pointer)
                    json_a = cJSON_CreateNumber(create_random_number());
                    if (json_a) {
                        compare_result = cJSON_Compare(json_a, json_a, case_sensitive);
                        // Should be true
                    }
                    break;
                }
                case 3: {
                    // Invalid type comparison (though cJSON_Compare handles this internally)
                    // Create an item with potentially invalid type (though API prevents this)
                    json_a = cJSON_CreateNumber(create_random_number());
                    if (json_a) {
                        // Try to compare with something that has different internal type bits
                        // This is tricky to do safely without violating API constraints
                        compare_result = cJSON_Compare(json_a, json_a, case_sensitive);
                    }
                    break;
                }
            }
            break;
        }
    }
    
    // Cleanup
    if (json_a) cJSON_Delete(json_a);
    if (json_b) cJSON_Delete(json_b);
    
    return 0;
}

} // namespace harness_008
