#include <fuzzer/FuzzedDataProvider.h>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <string>
#include <vector>
#include <cstring>
#include <cmath>
#include <cfloat>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include "cjson/cJSON.h"

namespace harness_016 {
/*
 * Harness_016: Systematic cJSON_Compare testing through JSON string generation and parsing
 * 
 * Target: cJSON_Compare function with 73/76 branches blocked
 * Focus: Sophisticated comparison scenarios targeting all blocked branches:
 *   1) Nested JSON arrays with different element orders
 *   2) Objects with same properties in different orders  
 *   3) Case-sensitive vs case-insensitive string comparisons
 *   4) Mixed type comparisons (should return false)
 *   5) NULL pointer edge cases
 *   6) Raw JSON string comparisons
 *   7) Number comparisons with floating point precision issues
 * 
 * SEMANTIC DIFFERENTIATION FROM PREVIOUS HARNESSES (000-015):
 * - harness_014: Uses cJSON_Duplicate with systematic modifications
 * - harness_015: Direct internal structure manipulation
 * - This harness_016: Generates JSON strings from fuzzer input, parses them, 
 *   and performs systematic pairwise comparisons to test the full comparison logic
 * 
 * Novel approach benefits:
 * 1. Tests the combination of cJSON_Parse and cJSON_Compare (real-world usage)
 * 2. Can generate diverse JSON structures directly from fuzzer bytes
 * 3. Systematically tests all comparison permutations
 * 4. Tests case_sensitive parameter with actual string parsing
 * 5. Tests floating point precision through JSON number parsing
 * 6. Can create invalid/malformed JSON to test edge cases
 * 
 * Implementation strategy:
 * 1. Generate multiple JSON strings from fuzzer input using different construction methods
 * 2. Parse strings into cJSON objects
 * 3. Perform systematic pairwise comparisons with both case_sensitive=true/false
 * 4. Focus on blocked branch conditions in cJSON_Compare:
 *    - Type validation failures (lines 3079-3101)
 *    - Recursive array comparison (lines 3137-3158)
 *    - Recursive object O(n^2) comparison (lines 3161-3196)
 *    - String comparison paths (case-sensitive vs insensitive)
 *    - NULL pointer and edge cases
 */



// Helper to generate a JSON string from fuzzer data
static std::string generate_json_string(FuzzedDataProvider* fdp, int depth = 0) {
    if (depth > 3 || fdp->remaining_bytes() < 16) {
        return "null";
    }
    
    uint8_t json_type = fdp->ConsumeIntegralInRange<uint8_t>(0, 7);
    
    switch (json_type) {
        case 0: { // Number (with floating point precision variations)
            double num = fdp->ConsumeFloatingPoint<double>();
            std::stringstream ss;
            ss << std::setprecision(15) << num;
            return ss.str();
        }
        
        case 1: { // String (with potential case variations)
            std::string str = fdp->ConsumeRandomLengthString(32);
            // Escape quotes and backslashes for JSON
            std::string escaped;
            for (char c : str) {
                if (c == '"' || c == '\\') {
                    escaped += '\\';
                }
                escaped += c;
            }
            return "\"" + escaped + "\"";
        }
        
        case 2: // Boolean
            return fdp->ConsumeBool() ? "true" : "false";
            
        case 3: // Null
            return "null";
            
        case 4: { // Array
            size_t array_size = fdp->ConsumeIntegralInRange<size_t>(0, 5);
            std::string result = "[";
            for (size_t i = 0; i < array_size; i++) {
                if (i > 0) result += ",";
                result += generate_json_string(fdp, depth + 1);
            }
            result += "]";
            return result;
        }
            
        case 5: { // Object
            size_t obj_size = fdp->ConsumeIntegralInRange<size_t>(0, 5);
            std::string result = "{";
            for (size_t i = 0; i < obj_size; i++) {
                if (i > 0) result += ",";
                std::string key = fdp->ConsumeRandomLengthString(12);
                // Escape key
                std::string escaped_key;
                for (char c : key) {
                    if (c == '"' || c == '\\') {
                        escaped_key += '\\';
                    }
                    escaped_key += c;
                }
                result += "\"" + escaped_key + "\":" + generate_json_string(fdp, depth + 1);
            }
            result += "}";
            return result;
        }
            
        case 6: { // Raw JSON (malformed/invalid to test type validation)
            // Sometimes generate invalid JSON to test type validation
            if (fdp->ConsumeBool()) {
                return fdp->ConsumeRandomLengthString(64);
            } else {
                return generate_json_string(fdp, depth); // Recursive call for valid JSON
            }
        }
            
        case 7: { // Mixed/nested complex structure
            std::string result = "{";
            size_t num_fields = fdp->ConsumeIntegralInRange<size_t>(1, 4);
            for (size_t i = 0; i < num_fields; i++) {
                if (i > 0) result += ",";
                std::string key = "field" + std::to_string(i);
                result += "\"" + key + "\":";
                
                uint8_t field_type = fdp->ConsumeIntegralInRange<uint8_t>(0, 3);
                switch (field_type) {
                    case 0: { // Array with potentially different order
                        result += "[";
                        size_t array_len = fdp->ConsumeIntegralInRange<size_t>(0, 4);
                        for (size_t j = 0; j < array_len; j++) {
                            if (j > 0) result += ",";
                            result += generate_json_string(fdp, depth + 1);
                        }
                        result += "]";
                        break;
                    }
                    case 1: { // Nested object
                        result += generate_json_string(fdp, depth + 1);
                        break;
                    }
                    case 2: { // String with case variations
                        std::string str = fdp->ConsumeRandomLengthString(16);
                        // Apply case transformation based on fuzzer input
                        if (fdp->ConsumeBool()) {
                            for (char& c : str) {
                                if (c >= 'a' && c <= 'z') c = c - 'a' + 'A';
                            }
                        } else {
                            for (char& c : str) {
                                if (c >= 'A' && c <= 'Z') c = c - 'A' + 'a';
                            }
                        }
                        std::string escaped;
                        for (char c : str) {
                            if (c == '"' || c == '\\') escaped += '\\';
                            escaped += c;
                        }
                        result += "\"" + escaped + "\"";
                        break;
                    }
                    case 3: { // Number with precision
                        double num = fdp->ConsumeFloatingPoint<double>();
                        std::stringstream ss;
                        // Vary precision to test floating point comparison
                        ss << std::setprecision(fdp->ConsumeIntegralInRange<int>(6, 17)) << num;
                        result += ss.str();
                        break;
                    }
                }
            }
            result += "}";
            return result;
        }
            
        default:
            return "null";
    }
}

// Helper to create cJSON object with NULL valuestring (for testing line 3123)
static cJSON* create_string_with_null_valuestring() {
    cJSON* item = cJSON_CreateString("test");
    if (item != NULL && item->valuestring != NULL) {
        free((void*)item->valuestring);
        item->valuestring = NULL;
    }
    return item;
}

// Helper to create invalid type cJSON item (for testing default switch case line 3096)
static cJSON* create_invalid_type_item() {
    cJSON* item = cJSON_CreateNull();
    if (item != NULL) {
        // Set an invalid type value
        item->type = 0xFF; // Invalid type
    }
    return item;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need sufficient data for comprehensive testing
    if (size < 512) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // ============================================================
    // PHASE 1: GENERATE DIVERSE JSON STRINGS
    // ============================================================
    
    std::vector<std::string> json_strings;
    std::vector<cJSON*> json_objects;
    
    // Generate 5-8 different JSON strings from fuzzer input
    size_t num_strings = fdp.ConsumeIntegralInRange<size_t>(5, 8);
    for (size_t i = 0; i < num_strings && fdp.remaining_bytes() > 64; i++) {
        std::string json_str = generate_json_string(&fdp);
        json_strings.push_back(json_str);
        
        // Parse the JSON string
        cJSON* parsed = cJSON_Parse(json_str.c_str());
        if (parsed != NULL) {
            json_objects.push_back(parsed);
        }
    }
    
    // Add some specially crafted objects for edge case testing
    if (fdp.remaining_bytes() > 32) {
        // NULL valuestring string object
        cJSON* null_string_obj = create_string_with_null_valuestring();
        if (null_string_obj != NULL) {
            json_objects.push_back(null_string_obj);
        }
        
        // Invalid type object (if enough bytes remain)
        if (fdp.remaining_bytes() > 16) {
            cJSON* invalid_type_obj = create_invalid_type_item();
            if (invalid_type_obj != NULL) {
                json_objects.push_back(invalid_type_obj);
            }
        }
        
        // Create raw JSON object
        std::string raw_json = fdp.ConsumeRandomLengthString(48);
        cJSON* raw_obj = cJSON_CreateRaw(raw_json.c_str());
        if (raw_obj != NULL) {
            json_objects.push_back(raw_obj);
        }
    }
    
    // ============================================================
    // PHASE 2: SYSTEMATIC PAIRWISE COMPARISONS
    // ============================================================
    
    // Test all pairs of objects with both case_sensitive settings
    for (size_t i = 0; i < json_objects.size(); i++) {
        for (size_t j = 0; j < json_objects.size(); j++) {
            cJSON* obj_a = json_objects[i];
            cJSON* obj_b = json_objects[j];
            
            if (obj_a == NULL || obj_b == NULL) {
                continue;
            }
            
            // Test with case_sensitive = true
            cJSON_bool result_true = cJSON_Compare(obj_a, obj_b, true);
            
            // Test with case_sensitive = false
            cJSON_bool result_false = cJSON_Compare(obj_a, obj_b, false);
            
            // Test NULL pointer edge cases
            if (fdp.ConsumeBool() && fdp.remaining_bytes() > 8) {
                // Test with NULL as first parameter
                cJSON_bool null_first = cJSON_Compare(NULL, obj_b, fdp.ConsumeBool());
                // Should return false
                
                // Test with NULL as second parameter  
                cJSON_bool null_second = cJSON_Compare(obj_a, NULL, fdp.ConsumeBool());
                // Should return false
                
                // Test with both NULL
                cJSON_bool both_null = cJSON_Compare(NULL, NULL, fdp.ConsumeBool());
                // Should return false
            }
            
            // Test self-comparison (should return true for valid objects)
            if (i == j) {
                cJSON_bool self_compare = cJSON_Compare(obj_a, obj_a, fdp.ConsumeBool());
                // Should return true for valid objects
            }
        }
    }
    
    // ============================================================
    // PHASE 3: TARGETED SCENARIO TESTING
    // ============================================================
    
    if (fdp.remaining_bytes() > 128) {
        // SCENARIO 1: Nested arrays with different element orders
        cJSON* array1 = cJSON_CreateArray();
        cJSON* array2 = cJSON_CreateArray();
        
        if (array1 != NULL && array2 != NULL) {
            // Create 3-5 elements with fuzzer values
            size_t array_size = fdp.ConsumeIntegralInRange<size_t>(3, 5);
            std::vector<double> values;
            
            for (size_t i = 0; i < array_size && fdp.remaining_bytes() > 8; i++) {
                double val = fdp.ConsumeFloatingPoint<double>();
                values.push_back(val);
                cJSON_AddItemToArray(array1, cJSON_CreateNumber(val));
            }
            
            // Add same values to array2 but in potentially different order
            if (!values.empty()) {
                // Maybe shuffle based on fuzzer input
                if (fdp.ConsumeBool() && values.size() > 1) {
                    // Add in reverse order
                    for (auto it = values.rbegin(); it != values.rend(); ++it) {
                        cJSON_AddItemToArray(array2, cJSON_CreateNumber(*it));
                    }
                } else {
                    // Add in same order
                    for (double val : values) {
                        cJSON_AddItemToArray(array2, cJSON_CreateNumber(val));
                    }
                }
                
                // Compare arrays
                cJSON_bool array_compare = cJSON_Compare(array1, array2, fdp.ConsumeBool());
                // Result depends on order equality
            }
        }
        
        // SCENARIO 2: Objects with same properties in different orders
        cJSON* obj1 = cJSON_CreateObject();
        cJSON* obj2 = cJSON_CreateObject();
        
        if (obj1 != NULL && obj2 != NULL && fdp.remaining_bytes() > 64) {
            std::vector<std::pair<std::string, double>> properties;
            size_t num_props = fdp.ConsumeIntegralInRange<size_t>(2, 4);
            
            for (size_t i = 0; i < num_props && fdp.remaining_bytes() > 16; i++) {
                std::string key = "key" + std::to_string(i);
                double value = fdp.ConsumeFloatingPoint<double>();
                properties.push_back({key, value});
                
                cJSON_AddNumberToObject(obj1, key.c_str(), value);
            }
            
            // Add properties to obj2 in potentially different order
            if (!properties.empty()) {
                if (fdp.ConsumeBool() && properties.size() > 1) {
                    // Reverse order
                    for (auto it = properties.rbegin(); it != properties.rend(); ++it) {
                        cJSON_AddNumberToObject(obj2, it->first.c_str(), it->second);
                    }
                } else {
                    // Same order
                    for (const auto& prop : properties) {
                        cJSON_AddNumberToObject(obj2, prop.first.c_str(), prop.second);
                    }
                }
                
                // Objects with same properties in different orders should still compare equal
                cJSON_bool obj_compare = cJSON_Compare(obj1, obj2, fdp.ConsumeBool());
                // Should return true (object comparison should be order-independent)
            }
        }
        
        // SCENARIO 3: Case-sensitive vs case-insensitive string comparisons
        if (fdp.remaining_bytes() > 32) {
            std::string base_str = fdp.ConsumeRandomLengthString(24);
            if (!base_str.empty()) {
                // Create different case variations
                std::string lower_str = base_str;
                std::string upper_str = base_str;
                
                for (char& c : lower_str) {
                    if (c >= 'A' && c <= 'Z') c = c - 'A' + 'a';
                }
                for (char& c : upper_str) {
                    if (c >= 'a' && c <= 'z') c = c - 'a' + 'A';
                }
                
                cJSON* str_lower = cJSON_CreateString(lower_str.c_str());
                cJSON* str_upper = cJSON_CreateString(upper_str.c_str());
                cJSON* str_same1 = cJSON_CreateString(lower_str.c_str());
                cJSON* str_same2 = cJSON_CreateString(upper_str.c_str());
                
                if (str_lower != NULL && str_upper != NULL) {
                    // case_sensitive = true: different strings
                    cJSON_bool case_true = cJSON_Compare(str_lower, str_upper, true);
                    // Should return false
                    
                    // case_sensitive = false: might be equal depending on implementation
                    cJSON_bool case_false = cJSON_Compare(str_lower, str_upper, false);
                }
                
                if (str_lower != NULL && str_same1 != NULL) {
                    // Identical strings should match with both settings
                    cJSON_bool same_true = cJSON_Compare(str_lower, str_same1, true);
                    cJSON_bool same_false = cJSON_Compare(str_lower, str_same1, false);
                    // Both should return true
                }
            }
        }
        
        // SCENARIO 4: Mixed type comparisons (should return false)
        if (fdp.remaining_bytes() > 32) {
            cJSON* num_obj = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
            cJSON* str_obj = cJSON_CreateString(fdp.ConsumeRandomLengthString(16).c_str());
            cJSON* bool_obj = cJSON_CreateBool(fdp.ConsumeBool());
            cJSON* null_obj = cJSON_CreateNull();
            
            if (num_obj != NULL && str_obj != NULL) {
                cJSON_bool mixed_compare = cJSON_Compare(num_obj, str_obj, fdp.ConsumeBool());
                // Should return false (different types)
            }
            
            if (bool_obj != NULL && null_obj != NULL) {
                cJSON_bool mixed_compare2 = cJSON_Compare(bool_obj, null_obj, fdp.ConsumeBool());
                // Should return false
            }
        }
        
        // SCENARIO 5: Number comparisons with floating point precision issues
        if (fdp.remaining_bytes() > 32) {
            // Generate numbers that might have precision issues
            double num1 = fdp.ConsumeFloatingPoint<double>();
            double num2 = num1; // Same value
            
            // Create numbers with potentially different representations
            cJSON* num_obj1 = cJSON_CreateNumber(num1);
            cJSON* num_obj2 = cJSON_CreateNumber(num2);
            
            if (num_obj1 != NULL && num_obj2 != NULL) {
                cJSON_bool num_compare = cJSON_Compare(num_obj1, num_obj2, fdp.ConsumeBool());
                // Should return true (compare_double handles precision)
            }
            
            // Test with very close but not equal numbers
            if (fdp.remaining_bytes() > 16) {
                double close1 = fdp.ConsumeFloatingPoint<double>();
                double close2 = close1 + DBL_EPSILON * fabs(close1) * 0.5;
                
                cJSON* close_obj1 = cJSON_CreateNumber(close1);
                cJSON* close_obj2 = cJSON_CreateNumber(close2);
                
                if (close_obj1 != NULL && close_obj2 != NULL) {
                    cJSON_bool close_compare = cJSON_Compare(close_obj1, close_obj2, fdp.ConsumeBool());
                    // Might return true due to compare_double tolerance
                }
            }
        }
        
        // Cleanup scenario objects
        if (array1 != NULL) cJSON_Delete(array1);
        if (array2 != NULL) cJSON_Delete(array2);
        if (obj1 != NULL) cJSON_Delete(obj1);
        if (obj2 != NULL) cJSON_Delete(obj2);
    }
    
    // ============================================================
    // PHASE 4: CLEANUP
    // ============================================================
    
    // Clean up all parsed objects
    for (cJSON* obj : json_objects) {
        if (obj != NULL) {
            cJSON_Delete(obj);
        }
    }
    
    return 0;
}

} // namespace harness_016
