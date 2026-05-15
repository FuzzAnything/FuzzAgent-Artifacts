#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <cmath>
#include <cfloat>
#include <limits>
#include <fuzzer/FuzzedDataProvider.h>
#include "cjson/cJSON.h"

namespace harness_017 {

using std::isinf;
using std::isnan;
using std::nextafter;
// Harness_017: Focus on triggering the symmetric comparison failure path in cJSON_Compare
// Specifically targets line 3188-3190 which has 0 hits in coverage
// This path is executed when cJSON_Compare(b_element, a_element, case_sensitive) returns false
// even though cJSON_Compare(a_element, b_element, case_sensitive) might have returned true

// Strategy: Exploit potential asymmetry in compare_double function and create objects
// where comparing A->B passes but comparing B->A fails due to floating-point precision issues

// Helper to create asymmetric numeric comparisons
// Returns a pair of double values where compare_double(a, b) might differ from compare_double(b, a)
std::pair<double, double> create_asymmetric_doubles(FuzzedDataProvider& fdp) {
    uint8_t strategy = fdp.ConsumeIntegral<uint8_t>() % 8;
    
    switch (strategy) {
        case 0: {
            // Very large numbers near DBL_MAX where multiplication with DBL_EPSILON might cause rounding
            double base = fdp.ConsumeFloatingPoint<double>();
            double scale = DBL_MAX / fabs(base);
            if (scale > 0 && !isinf(scale)) {
                double a = base * (scale * 0.999999);
                double b = base * scale;
                return {a, b};
            }
            break;
        }
        case 1: {
            // Very small numbers near DBL_MIN where relative error calculation might be asymmetric
            double base = fdp.ConsumeFloatingPoint<double>();
            double a = base * DBL_MIN * (1.0 + 1.1 * DBL_EPSILON);
            double b = base * DBL_MIN;
            return {a, b};
        }
        case 2: {
            // Numbers where fabs(a) and fabs(b) differ slightly, causing different maxVal in compare_double
            double a = fdp.ConsumeFloatingPoint<double>();
            double b = a * (1.0 + 0.5 * DBL_EPSILON);
            // Ensure they're not exactly equal
            if (a != b) {
                return {a, b};
            }
            break;
        }
        case 3: {
            // Create values where a-b is exactly at the boundary of maxVal * DBL_EPSILON
            double maxVal = fdp.ConsumeFloatingPointInRange<double>(1.0, 1e10);
            double diff = maxVal * DBL_EPSILON;
            double a = maxVal + diff * 0.4999999999;  // Just below threshold
            double b = maxVal;
            return {a, b};
        }
        case 4: {
            // Use NaN values (should always compare false)
            double a = std::numeric_limits<double>::quiet_NaN();
            double b = fdp.ConsumeFloatingPoint<double>();
            return {a, b};
        }
        case 5: {
            // Use Infinity values
            double a = std::numeric_limits<double>::infinity();
            double b = fdp.ConsumeFloatingPoint<double>();
            if (fdp.ConsumeBool()) {
                a = -a;
            }
            return {a, b};
        }
        case 6: {
            // Values with opposite signs where maxVal calculation might be asymmetric
            double a = fdp.ConsumeFloatingPoint<double>();
            double b = -a * (1.0 + 1.1 * DBL_EPSILON);
            return {a, b};
        }
        case 7: {
            // Denormal numbers
            double a = fdp.ConsumeFloatingPoint<double>() * DBL_MIN * 1e-10;
            double b = a * (1.0 + 2.0 * DBL_EPSILON);
            return {a, b};
        }
    }
    
    // Fallback: random values
    return {fdp.ConsumeFloatingPoint<double>(), fdp.ConsumeFloatingPoint<double>()};
}

// Create object A with specific numeric values
cJSON* create_object_a(FuzzedDataProvider& fdp, const std::vector<std::string>& keys) {
    cJSON* obj = cJSON_CreateObject();
    if (!obj) return NULL;
    
    for (size_t i = 0; i < keys.size() && fdp.remaining_bytes() > 4; i++) {
        const std::string& key = keys[i];
        uint8_t value_type = fdp.ConsumeIntegral<uint8_t>() % 4;
        
        switch (value_type) {
            case 0: {
                // Use carefully crafted double values for object A
                auto [a_val, b_val] = create_asymmetric_doubles(fdp);
                (void)b_val; // b_val will be used for object B
                cJSON_AddNumberToObject(obj, key.c_str(), a_val);
                break;
            }
            case 1: {
                bool val = fdp.ConsumeBool();
                cJSON_AddBoolToObject(obj, key.c_str(), val);
                break;
            }
            case 2: {
                size_t str_len = fdp.ConsumeIntegralInRange<size_t>(1, 32);
                if (fdp.remaining_bytes() >= str_len) {
                    std::string str_val = fdp.ConsumeBytesAsString(str_len);
                    cJSON_AddStringToObject(obj, key.c_str(), str_val.c_str());
                }
                break;
            }
            case 3: {
                cJSON_AddNullToObject(obj, key.c_str());
                break;
            }
        }
    }
    
    return obj;
}

// Create object B with corresponding but slightly different values
cJSON* create_object_b(FuzzedDataProvider& fdp, const std::vector<std::string>& keys, 
                      const std::vector<double>& a_values) {
    cJSON* obj = cJSON_CreateObject();
    if (!obj) return NULL;
    
    size_t value_index = 0;
    for (size_t i = 0; i < keys.size() && fdp.remaining_bytes() > 4; i++) {
        const std::string& key = keys[i];
        uint8_t value_type = fdp.ConsumeIntegral<uint8_t>() % 4;
        
        switch (value_type) {
            case 0: {
                // For numeric values in object B, use the corresponding b_val
                // from the asymmetric pair created for this key
                if (value_index < a_values.size()) {
                    // Recreate the asymmetric pair to get b_val
                    // We need to simulate the same asymmetric behavior
                    double a_val = a_values[value_index++];
                    
                    // Create a b_val that's close to a_val but might cause asymmetric comparison
                    // Try several strategies to trigger the symmetric comparison failure
                    uint8_t strategy = fdp.ConsumeIntegral<uint8_t>() % 3;
                    double b_val;
                    
                    switch (strategy) {
                        case 0:
                            // b_val that's just beyond the tolerance when comparing b->a
                            b_val = a_val * (1.0 + 1.0001 * DBL_EPSILON);
                            break;
                        case 1:
                            // b_val with different sign magnitude for maxVal calculation
                            b_val = -a_val * (1.0 + 0.9999 * DBL_EPSILON);
                            break;
                        case 2:
                            // b_val that's very close but not identical
                            b_val = nextafter(a_val, a_val * 2.0);
                            break;
                        default:
                            b_val = a_val;
                    }
                    
                    cJSON_AddNumberToObject(obj, key.c_str(), b_val);
                } else {
                    // Fallback
                    cJSON_AddNumberToObject(obj, key.c_str(), fdp.ConsumeFloatingPoint<double>());
                }
                break;
            }
            case 1: {
                // For boolean values, sometimes use the opposite value to create mismatch
                bool val = fdp.ConsumeBool();
                if (fdp.ConsumeBool()) {
                    val = !val; // Flip for potential mismatch
                }
                cJSON_AddBoolToObject(obj, key.c_str(), val);
                break;
            }
            case 2: {
                // For strings, create similar but not identical strings
                size_t str_len = fdp.ConsumeIntegralInRange<size_t>(1, 32);
                if (fdp.remaining_bytes() >= str_len) {
                    std::string str_val = fdp.ConsumeBytesAsString(str_len);
                    // Sometimes modify the last character
                    if (!str_val.empty() && fdp.ConsumeBool()) {
                        str_val[str_val.length() - 1] = 'X';
                    }
                    cJSON_AddStringToObject(obj, key.c_str(), str_val.c_str());
                }
                break;
            }
            case 3: {
                cJSON_AddNullToObject(obj, key.c_str());
                break;
            }
        }
    }
    
    return obj;
}

// Test case 1: Direct asymmetric numeric comparison
void test_asymmetric_numeric_comparison(FuzzedDataProvider& fdp) {
    if (fdp.remaining_bytes() < 32) return;
    
    // Create keys
    std::vector<std::string> keys;
    int num_keys = fdp.ConsumeIntegralInRange<int>(2, 6);
    for (int i = 0; i < num_keys && fdp.remaining_bytes() > 8; i++) {
        size_t key_len = fdp.ConsumeIntegralInRange<size_t>(1, 16);
        if (fdp.remaining_bytes() < key_len) break;
        std::string key = fdp.ConsumeBytesAsString(key_len);
        if (!key.empty()) {
            keys.push_back(key);
        }
    }
    
    if (keys.empty()) {
        keys.push_back("num1");
        keys.push_back("num2");
        keys.push_back("num3");
    }
    
    // Store A values for reference
    std::vector<double> a_values;
    
    // Create object A
    cJSON* obj_a = create_object_a(fdp, keys);
    if (!obj_a) return;
    
    // Extract numeric values from object A for reference
    for (const auto& key : keys) {
        cJSON* item = cJSON_GetObjectItem(obj_a, key.c_str());
        if (item && cJSON_IsNumber(item)) {
            a_values.push_back(item->valuedouble);
        }
    }
    
    // Create object B with corresponding values
    cJSON* obj_b = create_object_b(fdp, keys, a_values);
    if (!obj_b) {
        cJSON_Delete(obj_a);
        return;
    }
    
    // Test comparison with case_sensitive = true
    cJSON_bool result1 = cJSON_Compare(obj_a, obj_b, true);
    
    // Test comparison with case_sensitive = false
    cJSON_bool result2 = cJSON_Compare(obj_a, obj_b, false);
    
    // Also test swapping order (a vs b, b vs a)
    cJSON_bool result3 = cJSON_Compare(obj_b, obj_a, true);
    cJSON_bool result4 = cJSON_Compare(obj_b, obj_a, false);
    
    // Clean up
    cJSON_Delete(obj_a);
    cJSON_Delete(obj_b);
}

// Test case 2: Nested objects with asymmetric comparisons
void test_nested_asymmetric_objects(FuzzedDataProvider& fdp) {
    if (fdp.remaining_bytes() < 64) return;
    
    // Create root objects
    cJSON* root_a = cJSON_CreateObject();
    cJSON* root_b = cJSON_CreateObject();
    if (!root_a || !root_b) {
        if (root_a) cJSON_Delete(root_a);
        if (root_b) cJSON_Delete(root_b);
        return;
    }
    
    // Add nested objects with asymmetric numeric values
    cJSON* nested_a = cJSON_CreateObject();
    cJSON* nested_b = cJSON_CreateObject();
    
    if (nested_a && nested_b) {
        // Add asymmetric numeric values to nested objects
        for (int i = 0; i < 3 && fdp.remaining_bytes() > 16; i++) {
            std::string key = "val" + std::to_string(i);
            
            // Create asymmetric pair
            auto [a_val, b_val] = create_asymmetric_doubles(fdp);
            
            cJSON_AddNumberToObject(nested_a, key.c_str(), a_val);
            cJSON_AddNumberToObject(nested_b, key.c_str(), b_val);
        }
        
        cJSON_AddItemToObject(root_a, "nested", nested_a);
        cJSON_AddItemToObject(root_b, "nested", nested_b);
    } else {
        if (nested_a) cJSON_Delete(nested_a);
        if (nested_b) cJSON_Delete(nested_b);
    }
    
    // Add some direct asymmetric values at root level
    for (int i = 0; i < 2 && fdp.remaining_bytes() > 16; i++) {
        std::string key = "root" + std::to_string(i);
        auto [a_val, b_val] = create_asymmetric_doubles(fdp);
        
        cJSON_AddNumberToObject(root_a, key.c_str(), a_val);
        cJSON_AddNumberToObject(root_b, key.c_str(), b_val);
    }
    
    // Test comparisons
    cJSON_bool result1 = cJSON_Compare(root_a, root_b, true);
    cJSON_bool result2 = cJSON_Compare(root_b, root_a, true);
    cJSON_bool result3 = cJSON_Compare(root_a, root_b, false);
    cJSON_bool result4 = cJSON_Compare(root_b, root_a, false);
    
    // Clean up
    cJSON_Delete(root_a);
    cJSON_Delete(root_b);
}

// Test case 3: Arrays containing objects with asymmetric values
void test_arrays_with_asymmetric_objects(FuzzedDataProvider& fdp) {
    if (fdp.remaining_bytes() < 48) return;
    
    cJSON* array_a = cJSON_CreateArray();
    cJSON* array_b = cJSON_CreateArray();
    if (!array_a || !array_b) {
        if (array_a) cJSON_Delete(array_a);
        if (array_b) cJSON_Delete(array_b);
        return;
    }
    
    // Add some objects with asymmetric values to arrays
    int num_objects = fdp.ConsumeIntegralInRange<int>(1, 3);
    for (int i = 0; i < num_objects && fdp.remaining_bytes() > 32; i++) {
        cJSON* obj_a = cJSON_CreateObject();
        cJSON* obj_b = cJSON_CreateObject();
        
        if (obj_a && obj_b) {
            // Add asymmetric numeric value
            auto [a_val, b_val] = create_asymmetric_doubles(fdp);
            cJSON_AddNumberToObject(obj_a, "value", a_val);
            cJSON_AddNumberToObject(obj_b, "value", b_val);
            
            cJSON_AddItemToArray(array_a, obj_a);
            cJSON_AddItemToArray(array_b, obj_b);
        } else {
            if (obj_a) cJSON_Delete(obj_a);
            if (obj_b) cJSON_Delete(obj_b);
        }
    }
    
    // Test comparisons
    cJSON_bool result1 = cJSON_Compare(array_a, array_b, true);
    cJSON_bool result2 = cJSON_Compare(array_b, array_a, true);
    
    // Clean up
    cJSON_Delete(array_a);
    cJSON_Delete(array_b);
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 16) return 0;  // Need minimum input
    
    FuzzedDataProvider fdp(data, size);
    
    // Determine which test case to run based on input
    uint8_t test_case = fdp.ConsumeIntegral<uint8_t>() % 3;
    
    switch (test_case) {
        case 0:
            test_asymmetric_numeric_comparison(fdp);
            break;
        case 1:
            test_nested_asymmetric_objects(fdp);
            break;
        case 2:
            test_arrays_with_asymmetric_objects(fdp);
            break;
    }
    
    return 0;
}

} // namespace harness_017
