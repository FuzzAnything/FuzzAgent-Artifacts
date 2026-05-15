#include <fuzzer/FuzzedDataProvider.h>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>
#include "cjson/cJSON.h"

namespace harness_027 {
/*
 * Harness 027: Error handling paths in cJSON_AddXToObject functions targeting uncovered error cleanup paths
 * 
 * Primary targets (all with 0% coverage):
 * 1) cJSON_AddNullToObject error path (lines 2161-2162) - when add_item_to_object returns false
 * 2) cJSON_AddTrueToObject error path (lines 2173-2174) - when add_item_to_object returns false  
 * 3) cJSON_AddFalseToObject error path (lines 2185-2186) - when add_item_to_object returns false
 * 4) cJSON_AddBoolToObject error path (lines 2197-2198) - when add_item_to_object returns false
 * 5) cJSON_AddNumberToObject error path (lines 2209-2210) - when add_item_to_object returns false
 * 6) cJSON_AddStringToObject error path (lines 2221-2222) - when add_item_to_object returns false
 * 7) cJSON_AddRawToObject error path (lines 2233-2234) - when add_item_to_object returns false
 * 
 * Error conditions to trigger:
 * - NULL object parameter (add_item_to_object line 2087 check)
 * - NULL name/string parameter (add_item_to_object line 2087 check)
 * - NULL item parameter (when cJSON_CreateX fails due to memory allocation)
 * - object == item self-reference check (add_item_to_object line 2087)
 * - cJSON_strdup failure (memory allocation failure for key string)
 * - add_item_to_array failure (nested array addition failure)
 * 
 * Semantic diversity from existing 27 harnesses:
 * - Previous harnesses test successful addition of items to objects
 * - This harness specifically targets failure paths and error cleanup
 * - Tests memory allocation failure simulation and invalid parameter handling
 * - Focuses on error recovery and cleanup code paths
 * 
 * Strategy:
 * 1) Create test scenarios with NULL objects/strings to trigger parameter validation failures
 * 2) Create self-referential objects (object == item) to trigger equality check failures
 * 3) Test with potentially invalid/malformed objects that might cause add_item_to_array failures
 * 4) Use FuzzedDataProvider to select between different error scenarios
 */



// Helper to create test scenarios that trigger add_item_to_object failures
static void test_add_null_error_paths(FuzzedDataProvider& fdp) {
    uint8_t scenario = fdp.ConsumeIntegral<uint8_t>() % 5;
    
    switch (scenario) {
        case 0: {
            // NULL object parameter
            cJSON* result = cJSON_AddNullToObject(NULL, "test_key");
            // result should be NULL, error path executed
            break;
        }
        
        case 1: {
            // NULL name parameter
            cJSON* obj = cJSON_CreateObject();
            if (obj) {
                cJSON* result = cJSON_AddNullToObject(obj, NULL);
                // result should be NULL, error path executed
                cJSON_Delete(obj);
            }
            break;
        }
        
        case 2: {
            // Valid object, but test with self-reference (object == item scenario)
            // This is tricky since cJSON_AddNullToObject creates new item
            // So we test with other functions that accept existing items
            cJSON* obj = cJSON_CreateObject();
            if (obj) {
                // Try to add the object to itself using AddItemToObject
                // This should trigger object == item check in add_item_to_object
                cJSON_bool success = cJSON_AddItemToObject(obj, "self", obj);
                // success should be false, error path in add_item_to_object
                cJSON_Delete(obj);
            }
            break;
        }
        
        case 3: {
            // Test with potentially exhausted memory scenario
            // Create many objects first to potentially exhaust memory
            // then try to add null
            cJSON* obj = cJSON_CreateObject();
            if (obj) {
                // Try to add null with a very long key that might cause allocation issues
                std::string long_key = fdp.ConsumeRandomLengthString(4096);
                cJSON* result = cJSON_AddNullToObject(obj, long_key.c_str());
                // If memory allocation fails in cJSON_strdup, result will be NULL
                cJSON_Delete(obj);
            }
            break;
        }
        
        case 4: {
            // Test with object that might have issues with add_item_to_array
            // Create an object and manipulate it to potentially cause issues
            cJSON* obj = cJSON_CreateObject();
            if (obj) {
                // Add some items first
                cJSON_AddItemToObject(obj, "existing1", cJSON_CreateNumber(1.0));
                cJSON_AddItemToObject(obj, "existing2", cJSON_CreateString("test"));
                
                // Now try to add null
                cJSON* result = cJSON_AddNullToObject(obj, "new_key");
                // This should succeed normally, but we're testing the pattern
                cJSON_Delete(obj);
            }
            break;
        }
    }
}

static void test_add_bool_error_paths(FuzzedDataProvider& fdp) {
    uint8_t scenario = fdp.ConsumeIntegral<uint8_t>() % 5;
    cJSON_bool bool_value = fdp.ConsumeBool();
    
    switch (scenario) {
        case 0: {
            // NULL object
            cJSON* result = cJSON_AddBoolToObject(NULL, "test_key", bool_value);
            break;
        }
        
        case 1: {
            // NULL name
            cJSON* obj = cJSON_CreateObject();
            if (obj) {
                cJSON* result = cJSON_AddBoolToObject(obj, NULL, bool_value);
                cJSON_Delete(obj);
            }
            break;
        }
        
        case 2: {
            // Test with CreateBool potentially failing (though unlikely)
            // We'll still test the API call pattern
            cJSON* obj = cJSON_CreateObject();
            if (obj) {
                std::string key = fdp.ConsumeRandomLengthString(128);
                cJSON* result = cJSON_AddBoolToObject(obj, key.c_str(), bool_value);
                cJSON_Delete(obj);
            }
            break;
        }
        
        case 3: {
            // Test true/false specific functions
            cJSON* obj = cJSON_CreateObject();
            if (obj) {
                if (bool_value) {
                    cJSON* result = cJSON_AddTrueToObject(obj, "true_key");
                } else {
                    cJSON* result = cJSON_AddFalseToObject(obj, "false_key");
                }
                cJSON_Delete(obj);
            }
            break;
        }
        
        case 4: {
            // Mixed test with various bool addition functions
            cJSON* obj = cJSON_CreateObject();
            if (obj) {
                // Test all three bool-related functions
                cJSON* result1 = cJSON_AddTrueToObject(obj, "key1");
                cJSON* result2 = cJSON_AddFalseToObject(obj, "key2");
                cJSON* result3 = cJSON_AddBoolToObject(obj, "key3", bool_value);
                cJSON_Delete(obj);
            }
            break;
        }
    }
}

static void test_add_number_error_paths(FuzzedDataProvider& fdp) {
    uint8_t scenario = fdp.ConsumeIntegral<uint8_t>() % 4;
    double number_value = fdp.ConsumeFloatingPoint<double>();
    
    switch (scenario) {
        case 0: {
            // NULL object
            cJSON* result = cJSON_AddNumberToObject(NULL, "test_key", number_value);
            break;
        }
        
        case 1: {
            // NULL name
            cJSON* obj = cJSON_CreateObject();
            if (obj) {
                cJSON* result = cJSON_AddNumberToObject(obj, NULL, number_value);
                cJSON_Delete(obj);
            }
            break;
        }
        
        case 2: {
            // Test with special floating point values
            cJSON* obj = cJSON_CreateObject();
            if (obj) {
                // Test with various numeric values including edge cases
                std::string key = fdp.ConsumeRandomLengthString(64);
                
                // Test with the provided fuzzed value
                cJSON* result1 = cJSON_AddNumberToObject(obj, key.c_str(), number_value);
                
                // Test with some special values
                cJSON* result2 = cJSON_AddNumberToObject(obj, "zero", 0.0);
                cJSON* result3 = cJSON_AddNumberToObject(obj, "negative", -1.0);
                cJSON* result4 = cJSON_AddNumberToObject(obj, "large", 1e308);
                
                cJSON_Delete(obj);
            }
            break;
        }
        
        case 3: {
            // Test with potentially problematic key (very long)
            cJSON* obj = cJSON_CreateObject();
            if (obj) {
                std::string long_key = fdp.ConsumeRandomLengthString(8192);
                cJSON* result = cJSON_AddNumberToObject(obj, long_key.c_str(), number_value);
                cJSON_Delete(obj);
            }
            break;
        }
    }
}

static void test_add_string_error_paths(FuzzedDataProvider& fdp) {
    uint8_t scenario = fdp.ConsumeIntegral<uint8_t>() % 5;
    std::string string_value = fdp.ConsumeRandomLengthString(256);
    
    switch (scenario) {
        case 0: {
            // NULL object
            cJSON* result = cJSON_AddStringToObject(NULL, "test_key", string_value.c_str());
            break;
        }
        
        case 1: {
            // NULL name
            cJSON* obj = cJSON_CreateObject();
            if (obj) {
                cJSON* result = cJSON_AddStringToObject(obj, NULL, string_value.c_str());
                cJSON_Delete(obj);
            }
            break;
        }
        
        case 2: {
            // NULL string value
            cJSON* obj = cJSON_CreateObject();
            if (obj) {
                cJSON* result = cJSON_AddStringToObject(obj, "key", NULL);
                cJSON_Delete(obj);
            }
            break;
        }
        
        case 3: {
            // Test raw addition as well
            cJSON* obj = cJSON_CreateObject();
            if (obj) {
                std::string key = fdp.ConsumeRandomLengthString(64);
                std::string raw_value = fdp.ConsumeRandomLengthString(128);
                
                cJSON* string_result = cJSON_AddStringToObject(obj, key.c_str(), string_value.c_str());
                cJSON* raw_result = cJSON_AddRawToObject(obj, "raw_key", raw_value.c_str());
                
                cJSON_Delete(obj);
            }
            break;
        }
        
        case 4: {
            // Test with empty strings
            cJSON* obj = cJSON_CreateObject();
            if (obj) {
                cJSON* result1 = cJSON_AddStringToObject(obj, "empty_key", "");
                cJSON* result2 = cJSON_AddRawToObject(obj, "empty_raw", "");
                cJSON_Delete(obj);
            }
            break;
        }
    }
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check for meaningful fuzzing
    if (size < 16) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Determine which error path tests to run based on fuzzer input
    uint8_t test_type = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    switch (test_type) {
        case 0:
            // Test null addition error paths
            test_add_null_error_paths(fdp);
            break;
            
        case 1:
            // Test bool addition error paths  
            test_add_bool_error_paths(fdp);
            break;
            
        case 2:
            // Test number addition error paths
            test_add_number_error_paths(fdp);
            break;
            
        case 3:
            // Test string/raw addition error paths
            test_add_string_error_paths(fdp);
            break;
    }
    
    // Additional comprehensive test: try all AddX functions in sequence
    // with various error conditions
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 100) {
        cJSON* obj = cJSON_CreateObject();
        if (obj) {
            std::string base_key = fdp.ConsumeRandomLengthString(32);
            
            // Try all AddX functions in sequence
            cJSON* null_result = cJSON_AddNullToObject(obj, (base_key + "_null").c_str());
            cJSON* true_result = cJSON_AddTrueToObject(obj, (base_key + "_true").c_str());
            cJSON* false_result = cJSON_AddFalseToObject(obj, (base_key + "_false").c_str());
            
            cJSON_bool bool_val = fdp.ConsumeBool();
            cJSON* bool_result = cJSON_AddBoolToObject(obj, (base_key + "_bool").c_str(), bool_val);
            
            double num_val = fdp.ConsumeFloatingPoint<double>();
            cJSON* num_result = cJSON_AddNumberToObject(obj, (base_key + "_num").c_str(), num_val);
            
            std::string str_val = fdp.ConsumeRandomLengthString(64);
            cJSON* str_result = cJSON_AddStringToObject(obj, (base_key + "_str").c_str(), str_val.c_str());
            
            std::string raw_val = fdp.ConsumeRandomLengthString(64);
            cJSON* raw_result = cJSON_AddRawToObject(obj, (base_key + "_raw").c_str(), raw_val.c_str());
            
            cJSON_Delete(obj);
        }
    }
    
    return 0;
}

} // namespace harness_027
