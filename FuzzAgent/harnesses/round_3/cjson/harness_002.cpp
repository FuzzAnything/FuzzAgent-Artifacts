#include <fuzzer/FuzzedDataProvider.h>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include "cjson/cJSON.h"

namespace harness_002 {


// Custom allocation functions for testing hooks
static void* custom_malloc(size_t size) {
    return malloc(size);
}

static void custom_free(void* ptr) {
    free(ptr);
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check for meaningful testing
    if (size < 32) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Consume fixed-size data first
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 8;
    bool use_custom_hooks = fdp.ConsumeBool();
    bool use_string_reference = fdp.ConsumeBool();
    bool test_invalid_cases = fdp.ConsumeBool();
    int string_length = fdp.ConsumeIntegralInRange<int>(1, 1024);
    
    // Test various hook configurations
    cJSON_Hooks hooks;
    if (use_custom_hooks) {
        hooks.malloc_fn = custom_malloc;
        hooks.free_fn = custom_free;
    } else {
        hooks.malloc_fn = NULL;
        hooks.free_fn = NULL;
    }
    
    // Initialize hooks (target API with 0% coverage)
    cJSON_InitHooks(&hooks);
    
    // Consume variable-size data
    std::string original_string = fdp.ConsumeRandomLengthString(string_length);
    std::string new_string = fdp.ConsumeRandomLengthString(string_length);
    std::string key_name = fdp.ConsumeRandomLengthString(128);
    std::string json_input = fdp.ConsumeRandomLengthString(512);
    
    // Use operation to determine API flow
    switch (operation) {
        case 0: {
            // Test cJSON_SetValuestring with valid string objects
            cJSON* string_item = cJSON_CreateString(original_string.c_str());
            if (string_item != NULL) {
                // Test setting value string (target API with 20 undiscovered branches)
                char* result = cJSON_SetValuestring(string_item, new_string.c_str());
                if (result != NULL) {
                    // Verify the string was changed
                    char* string_value = cJSON_GetStringValue(string_item);
                    // Do something with the result to ensure it's not optimized away
                    if (string_value != NULL) {
                        // Access first character to ensure compiler doesn't optimize away
                        volatile char ch = string_value[0];
                        (void)ch;
                    }
                }
                cJSON_Delete(string_item);
            }
            break;
        }
        
        case 1: {
            // Test cJSON_SetValuestring with overlapping strings (edge case)
            char buffer[256];
            if (original_string.length() > 0 && new_string.length() > 0) {
                // Create overlapping strings scenario
                size_t len1 = original_string.length() < 128 ? original_string.length() : 128;
                size_t len2 = new_string.length() < 128 ? new_string.length() : 128;
                memcpy(buffer, original_string.c_str(), len1);
                buffer[len1] = '\0';
                
                // Create string item with the buffer
                cJSON* string_item = cJSON_CreateString(buffer);
                if (string_item != NULL) {
                    // Try to set with overlapping string (should fail according to code analysis)
                    char* overlapping_string = buffer + (len1 > 10 ? 10 : 0);
                    char* result = cJSON_SetValuestring(string_item, overlapping_string);
                    // Result may be NULL due to overlap detection
                    cJSON_Delete(string_item);
                }
            }
            break;
        }
        
        case 2: {
            // Test cJSON_SetValuestring with string references (should fail)
            const char* const_string = "constant string";
            cJSON* string_ref = cJSON_CreateStringReference(const_string);
            if (string_ref != NULL) {
                // This should return NULL because string references can't be modified
                char* result = cJSON_SetValuestring(string_ref, new_string.c_str());
                cJSON_Delete(string_ref);
            }
            break;
        }
        
        case 3: {
            // Test invalid cases for cJSON_SetValuestring
            if (test_invalid_cases) {
                // Test with NULL object
                char* result1 = cJSON_SetValuestring(NULL, new_string.c_str());
                
                // Test with non-string object
                cJSON* number_item = cJSON_CreateNumber(42.5);
                if (number_item != NULL) {
                    char* result2 = cJSON_SetValuestring(number_item, new_string.c_str());
                    cJSON_Delete(number_item);
                }
                
                // Test with NULL valuestring
                cJSON* string_item = cJSON_CreateString(original_string.c_str());
                if (string_item != NULL) {
                    char* result3 = cJSON_SetValuestring(string_item, NULL);
                    cJSON_Delete(string_item);
                }
            }
            break;
        }
        
        case 4: {
            // Test hook initialization with NULL (should reset to default)
            cJSON_InitHooks(NULL);
            
            // Create and delete items after resetting hooks
            cJSON* test_item = cJSON_CreateObject();
            if (test_item != NULL) {
                cJSON_AddStringToObject(test_item, key_name.c_str(), original_string.c_str());
                cJSON_Delete(test_item);
            }
            break;
        }
        
        case 5: {
            // Test string reference creation and properties
            if (use_string_reference) {
                cJSON* string_ref = cJSON_CreateStringReference(original_string.c_str());
                if (string_ref != NULL) {
                    // Check if it's marked as reference
                    bool is_reference = (string_ref->type & cJSON_IsReference) != 0;
                    (void)is_reference;
                    
                    // Check string value
                    char* string_value = cJSON_GetStringValue(string_ref);
                    if (string_value != NULL) {
                        volatile char ch = string_value[0];
                        (void)ch;
                    }
                    
                    cJSON_Delete(string_ref);
                }
            }
            break;
        }
        
        case 6: {
            // Test cJSON_GetStringValue with various object types
            cJSON* parsed = cJSON_Parse(json_input.c_str());
            if (parsed != NULL) {
                // Try to get string value (may be NULL for non-string objects)
                char* string_value = cJSON_GetStringValue(parsed);
                
                // If parsed object has children, check them too
                if (parsed->child != NULL) {
                    cJSON* child = parsed->child;
                    while (child != NULL) {
                        char* child_string = cJSON_GetStringValue(child);
                        child = child->next;
                    }
                }
                
                cJSON_Delete(parsed);
            }
            break;
        }
        
        case 7: {
            // Test combination of hooks and string operations
            // Re-initialize hooks
            if (use_custom_hooks) {
                cJSON_InitHooks(&hooks);
            } else {
                cJSON_InitHooks(NULL);
            }
            
            // Create string array and test string operations
            const char* strings[] = {
                original_string.c_str(),
                new_string.c_str(),
                "test1",
                "test2"
            };
            cJSON* string_array = cJSON_CreateStringArray(strings, 4);
            if (string_array != NULL) {
                // Get array items and try to modify them
                for (int i = 0; i < cJSON_GetArraySize(string_array); i++) {
                    cJSON* item = cJSON_GetArrayItem(string_array, i);
                    if (item != NULL && cJSON_IsString(item)) {
                        // Try to set value string on array items
                        std::string replacement = fdp.ConsumeRandomLengthString(64);
                        char* result = cJSON_SetValuestring(item, replacement.c_str());
                    }
                }
                cJSON_Delete(string_array);
            }
            break;
        }
    }

    return 0;
}

} // namespace harness_002
