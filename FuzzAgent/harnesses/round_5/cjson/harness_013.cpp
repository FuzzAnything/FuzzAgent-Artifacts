#include <fuzzer/FuzzedDataProvider.h>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <string>
#include <vector>
#include <cstring>
#include <cmath>
#include <cfloat>
#include <sstream>
#include <iomanip>
#include "cjson/cJSON.h"

namespace harness_013 {
/*
 * Harness_013: Novel cJSON_Compare testing using cJSON_Parse for complex JSON strings
 * 
 * Target: Specifically targets the 73 blocked branches in cJSON_Compare identified
 * in coverage guidance harness_013_guidance.md with a novel approach:
 * 
 * SEMANTIC DIFFERENTIATION FROM PREVIOUS HARNESSES (000-012):
 * - Previous harnesses (005-012): Use programmatic creation (cJSON_Create*) to build JSON objects
 * - This harness_013: Uses cJSON_Parse to create JSON objects from complex JSON strings
 * 
 * This novel approach also exercises secondary target functions through parsing:
 * - parse_number (49/54 blocked) - through number edge cases in JSON strings
 * - parse_string (43/48 blocked) - through string edge cases in JSON strings  
 * - print_string_ptr (48/54 blocked) - through string parsing and comparison
 * 
 * Implementation strategy:
 * 1. Generate complex JSON strings with edge cases using fuzzer input
 * 2. Parse these strings using cJSON_Parse to create test objects
 * 3. Compare parsed objects using cJSON_Compare with case_sensitive variations
 * 4. Test number edge cases (NaN, Infinity, large/small numbers, precision boundaries)
 * 5. Test string edge cases (NULL strings, empty strings, special chars, Unicode, escapes)
 * 6. Test complex nested structures (5+ levels)
 * 7. Test arrays with varying lengths and contents
 * 8. Test objects with key subset/superset relationships
 */



// Helper to generate JSON number strings with edge cases
static std::string generate_number_json(double value, bool use_scientific = false, FuzzedDataProvider* fdp = nullptr) {
    std::ostringstream oss;
    
    if (std::isnan(value)) {
        return "null";  // JSON doesn't have NaN, use null as placeholder
    } else if (std::isinf(value)) {
        // Use fuzzer input to generate large numbers for infinity approximation
        if (fdp != nullptr && fdp->remaining_bytes() > 0) {
            // Generate random large exponent
            int exponent = fdp->ConsumeIntegralInRange<int>(100, 999);
            if (value > 0) {
                return std::to_string(fdp->ConsumeIntegral<int>()) + "e" + std::to_string(exponent);
            } else {
                return "-" + std::to_string(fdp->ConsumeIntegral<int>()) + "e" + std::to_string(exponent);
            }
        } else {
            // Fallback if no fuzzer data
            if (value > 0) {
                return "1e300";
            } else {
                return "-1e300";
            }
        }
    } else if (use_scientific) {
        oss << std::scientific << std::setprecision(15) << value;
    } else {
        oss << std::setprecision(15) << value;
    }
    
    return oss.str();
}

// Helper to generate JSON string with escape sequences
static std::string generate_escaped_string_json(const std::string& str) {
    std::ostringstream oss;
    oss << "\"";
    
    for (char c : str) {
        switch (c) {
            case '\"': oss << "\\\""; break;
            case '\\': oss << "\\\\"; break;
            case '\b': oss << "\\b"; break;
            case '\f': oss << "\\f"; break;
            case '\n': oss << "\\n"; break;
            case '\r': oss << "\\r"; break;
            case '\t': oss << "\\t"; break;
            default:
                if (static_cast<unsigned char>(c) < 0x20) {
                    oss << "\\u" << std::hex << std::setw(4) << std::setfill('0') 
                        << static_cast<int>(c);
                } else {
                    oss << c;
                }
                break;
        }
    }
    
    oss << "\"";
    return oss.str();
}

// Helper to generate nested JSON structure
static std::string generate_nested_json(int depth, FuzzedDataProvider& fdp, 
                                       const std::vector<std::string>& keys = {}) {
    if (depth <= 0) {
        // Leaf node - generate primitive value
        uint8_t leaf_type = fdp.ConsumeIntegral<uint8_t>() % 5;
        switch (leaf_type) {
            case 0: return "null";
            case 1: return fdp.ConsumeBool() ? "true" : "false";
            case 2: return generate_number_json(fdp.ConsumeFloatingPoint<double>(), false, &fdp);
            case 3: return generate_escaped_string_json(fdp.ConsumeRandomLengthString(32));
            case 4: return "[]";  // Empty array
            default: return "null";
        }
    }
    
    // Decide whether to create array or object
    bool create_array = fdp.ConsumeBool();
    
    if (create_array) {
        std::ostringstream oss;
        oss << "[";
        
        size_t array_size = fdp.ConsumeIntegralInRange<size_t>(0, 5);
        for (size_t i = 0; i < array_size; i++) {
            if (i > 0) oss << ",";
            oss << generate_nested_json(depth - 1, fdp, keys);
        }
        
        oss << "]";
        return oss.str();
    } else {
        std::ostringstream oss;
        oss << "{";
        
        size_t obj_size = fdp.ConsumeIntegralInRange<size_t>(0, 5);
        std::vector<std::string> used_keys;
        
        for (size_t i = 0; i < obj_size; i++) {
            if (i > 0) oss << ",";
            
            // Generate key
            std::string key;
            if (!keys.empty() && fdp.ConsumeBool() && i < keys.size()) {
                key = keys[i];
            } else {
                key = fdp.ConsumeRandomLengthString(8);
            }
            used_keys.push_back(key);
            
            oss << generate_escaped_string_json(key) << ":";
            oss << generate_nested_json(depth - 1, fdp, used_keys);
        }
        
        oss << "}";
        return oss.str();
    }
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need sufficient data for generating complex JSON strings
    if (size < 128) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // ============================================================
    // PART 1: GENERATE COMPLEX JSON STRINGS WITH EDGE CASES
    // ============================================================
    
    std::vector<std::string> json_strings;
    
    // 1.1 Generate JSON strings with number edge cases
    uint8_t num_test_cases = fdp.ConsumeIntegralInRange<uint8_t>(1, 8);
    for (uint8_t i = 0; i < num_test_cases && fdp.remaining_bytes() > 0; i++) {
        std::ostringstream oss;
        oss << "{";
        
        // Generate various number edge cases using fuzzer input
        oss << "\"nan_test\": null,";  // Placeholder for NaN
        
        // Use fuzzer input for large/small numbers instead of hardcoded values
        double large_positive = fdp.ConsumeFloatingPointInRange<double>(1.0e100, 1.0e308);
        double large_negative = fdp.ConsumeFloatingPointInRange<double>(-1.0e308, -1.0e100);
        double small_positive = fdp.ConsumeFloatingPointInRange<double>(1.0e-308, 1.0e-100);
        double small_negative = fdp.ConsumeFloatingPointInRange<double>(-1.0e-100, -1.0e-308);
        
        oss << "\"large_positive\": " << generate_number_json(large_positive, false, &fdp) << ",";
        oss << "\"large_negative\": " << generate_number_json(large_negative, false, &fdp) << ",";
        oss << "\"small_positive\": " << generate_number_json(small_positive, false, &fdp) << ",";
        oss << "\"small_negative\": " << generate_number_json(small_negative, false, &fdp) << ",";
        oss << "\"integer\": " << fdp.ConsumeIntegral<int>() << ",";
        oss << "\"float\": " << generate_number_json(fdp.ConsumeFloatingPoint<double>(), false, &fdp) << ",";
        oss << "\"scientific\": " << generate_number_json(fdp.ConsumeFloatingPoint<double>(), true, &fdp);
        
        oss << "}";
        json_strings.push_back(oss.str());
    }
    
    // 1.2 Generate JSON strings with string edge cases
    if (fdp.remaining_bytes() > 0) {
        std::ostringstream oss;
        oss << "{";
        
        // Generate string edge cases using fuzzer input
        oss << "\"empty\": \"\",";  // Empty string (valid test case)
        
        // Generate string with null bytes using fuzzer input
        size_t null_str_len = fdp.ConsumeIntegralInRange<size_t>(0, 10);
        std::string null_chars_str;
        for (size_t j = 0; j < null_str_len; j++) {
            null_chars_str += static_cast<char>(fdp.ConsumeIntegral<uint8_t>() % 32); // Include control chars
        }
        oss << "\"null_chars\": " << generate_escaped_string_json(null_chars_str) << ",";
        
        // Generate string with special characters using fuzzer input
        std::string special_chars_str;
        size_t special_len = fdp.ConsumeIntegralInRange<size_t>(0, 20);
        for (size_t j = 0; j < special_len; j++) {
            // Include various special characters
            uint8_t char_type = fdp.ConsumeIntegral<uint8_t>() % 4;
            switch (char_type) {
                case 0: special_chars_str += '\"'; break;
                case 1: special_chars_str += '\\'; break;
                case 2: special_chars_str += '\b'; break;
                case 3: special_chars_str += '\t'; break;
            }
        }
        oss << "\"special_chars\": " << generate_escaped_string_json(special_chars_str) << ",";
        // Unicode and special sequences
        std::string unicode_str;
        size_t unicode_len = fdp.ConsumeIntegralInRange<size_t>(0, 10);
        for (size_t j = 0; j < unicode_len; j++) {
            unicode_str += static_cast<char>(fdp.ConsumeIntegral<uint8_t>());
        }
        oss << "\"unicode\": " << generate_escaped_string_json(unicode_str) << ",";
        
        // Long string
        std::string long_str = fdp.ConsumeRandomLengthString(100);
        oss << "\"long\": " << generate_escaped_string_json(long_str) << ",";
        
        // Case variations for comparison testing
        std::string case_str = fdp.ConsumeRandomLengthString(20);
        oss << "\"lowercase\": " << generate_escaped_string_json(case_str) << ",";
        
        std::string upper_str = case_str;
        for (char& c : upper_str) c = std::toupper(c);
        oss << "\"uppercase\": " << generate_escaped_string_json(upper_str);
        
        oss << "}";
        json_strings.push_back(oss.str());
    }
    
    // 1.3 Generate complex nested structures (5+ levels)
    if (fdp.remaining_bytes() > 0) {
        std::string nested_json = generate_nested_json(5, fdp);
        json_strings.push_back(nested_json);
        
        // Generate slightly modified version for comparison
        if (fdp.remaining_bytes() > 0) {
            std::string modified_nested = generate_nested_json(5, fdp);
            json_strings.push_back(modified_nested);
        }
    }
    
    // 1.4 Generate arrays with varying lengths and contents
    if (fdp.remaining_bytes() > 0) {
        std::ostringstream oss;
        oss << "[";
        
        size_t array_len = fdp.ConsumeIntegralInRange<size_t>(0, 10);
        for (size_t i = 0; i < array_len; i++) {
            if (i > 0) oss << ",";
            
            uint8_t elem_type = fdp.ConsumeIntegral<uint8_t>() % 6;
            switch (elem_type) {
                case 0: oss << "null"; break;
                case 1: oss << (fdp.ConsumeBool() ? "true" : "false"); break;
                case 2: oss << generate_number_json(fdp.ConsumeFloatingPoint<double>(), false, &fdp); break;
                case 3: oss << generate_escaped_string_json(fdp.ConsumeRandomLengthString(20)); break;
                case 4: oss << "[]"; break;  // Empty nested array
                case 5: oss << "{}"; break;  // Empty nested object
            }
        }
        
        oss << "]";
        json_strings.push_back(oss.str());
    }
    
    // 1.5 Generate objects with key subset/superset relationships
    if (fdp.remaining_bytes() > 0) {
        // Base object with common keys
        std::ostringstream oss_base;
        oss_base << "{";
        
        std::vector<std::string> common_keys;
        size_t num_common = fdp.ConsumeIntegralInRange<size_t>(2, 5);
        for (size_t i = 0; i < num_common; i++) {
            if (i > 0) oss_base << ",";
            std::string key = fdp.ConsumeRandomLengthString(6);
            common_keys.push_back(key);
            oss_base << generate_escaped_string_json(key) << ":" 
                    << generate_number_json(fdp.ConsumeFloatingPoint<double>(), false, &fdp);
        }
        oss_base << "}";
        json_strings.push_back(oss_base.str());
        
        // Superset object (contains all common keys + extra)
        if (fdp.remaining_bytes() > 0) {
            std::ostringstream oss_superset;
            oss_superset << "{";
            
            // Add all common keys
            for (size_t i = 0; i < common_keys.size(); i++) {
                if (i > 0) oss_superset << ",";
                oss_superset << generate_escaped_string_json(common_keys[i]) << ":" 
                           << generate_number_json(fdp.ConsumeFloatingPoint<double>(), false, &fdp);
            }
            
            // Add extra keys
            size_t num_extra = fdp.ConsumeIntegralInRange<size_t>(1, 3);
            for (size_t i = 0; i < num_extra; i++) {
                oss_superset << ",";
                std::string extra_key = fdp.ConsumeRandomLengthString(6);
                oss_superset << generate_escaped_string_json(extra_key) << ":" 
                           << generate_escaped_string_json(fdp.ConsumeRandomLengthString(10));
            }
            
            oss_superset << "}";
            json_strings.push_back(oss_superset.str());
        }
    }
    
    // ============================================================
    // PART 2: PARSE JSON STRINGS AND PERFORM COMPARISONS
    // ============================================================
    
    std::vector<cJSON*> parsed_objects;
    
    // Parse all generated JSON strings
    for (const auto& json_str : json_strings) {
        cJSON* parsed = cJSON_Parse(json_str.c_str());
        if (parsed != NULL) {
            parsed_objects.push_back(parsed);
        }
    }
    
    // If we have at least 2 parsed objects, perform comparisons
    if (parsed_objects.size() >= 2) {
        // Compare each object with every other object
        for (size_t i = 0; i < parsed_objects.size(); i++) {
            for (size_t j = i + 1; j < parsed_objects.size(); j++) {
                if (parsed_objects[i] != NULL && parsed_objects[j] != NULL) {
                    // Test with case_sensitive = true
                    cJSON_bool result_true = cJSON_Compare(parsed_objects[i], parsed_objects[j], true);
                    
                    // Test with case_sensitive = false  
                    cJSON_bool result_false = cJSON_Compare(parsed_objects[i], parsed_objects[j], false);
                    
                    // Also compare object with itself (should return true)
                    cJSON_bool self_true = cJSON_Compare(parsed_objects[i], parsed_objects[i], true);
                    cJSON_bool self_false = cJSON_Compare(parsed_objects[i], parsed_objects[i], false);
                }
            }
        }
        
        // Special comparisons for edge cases
        if (parsed_objects.size() >= 3) {
            // Compare similar objects (e.g., nested structures with slight differences)
            cJSON_bool similar_compare = cJSON_Compare(parsed_objects[0], parsed_objects[1], fdp.ConsumeBool());
            
            // Compare very different objects
            cJSON_bool diff_compare = cJSON_Compare(parsed_objects[0], parsed_objects[2], fdp.ConsumeBool());
        }
    }
    
    // ============================================================
    // PART 3: CLEANUP
    // ============================================================
    
    // Delete all parsed objects
    for (cJSON* obj : parsed_objects) {
        if (obj != NULL) {
            cJSON_Delete(obj);
        }
    }
    
    // Also test NULL object comparisons (edge cases)
    cJSON* null_obj1 = NULL;
    cJSON* null_obj2 = NULL;
    cJSON* valid_obj = cJSON_CreateNull();
    
    if (valid_obj != NULL) {
        // Compare NULL with valid object (should handle NULL)
        cJSON_bool null_vs_valid = cJSON_Compare(null_obj1, valid_obj, true);
        cJSON_bool valid_vs_null = cJSON_Compare(valid_obj, null_obj1, true);
        
        // Compare NULL with NULL
        cJSON_bool null_vs_null = cJSON_Compare(null_obj1, null_obj2, true);
        
        cJSON_Delete(valid_obj);
    }
    
    return 0;
}

} // namespace harness_013
