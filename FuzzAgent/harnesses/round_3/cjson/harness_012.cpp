#include <fuzzer/FuzzedDataProvider.h>
#include "cjson/cJSON.h"
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <memory>

namespace harness_012 {
/*
 * Harness 012: Advanced Memory Hooks, Buffer Management, and Combined API Testing
 * 
 * Target: Remaining uncovered cJSON APIs with semantic diversity from existing harnesses
 * 
 * This harness focuses on:
 * 1. cJSON_InitHooks - Memory management hooks with various configurations
 * 2. cJSON_PrintBuffered - Advanced buffer management with various prebuffer sizes
 * 3. cJSON_PrintPreallocated - Complex buffer allocation scenarios
 * 4. cJSON_SetValuestring - String value modification with edge cases
 * 5. Combined realistic workflows for semantic diversity
 * 
 * Key coverage targets:
 * - Memory hook initialization with NULL, custom, and default allocators
 * - Buffer management edge cases (zero buffers, small buffers, large buffers)
 * - String value modification with overlapping memory scenarios
 * - Combined operations: parse → set value → print with hooks → cleanup
 * - Testing error paths and boundary conditions
 */


// Custom memory allocation functions for hook testing
static void* custom_malloc_hook(size_t size) {
    return malloc(size);
}

static void custom_free_hook(void* ptr) {
    free(ptr);
}

// Failing malloc for error path testing
static void* failing_malloc_hook(size_t size) {
    (void)size;
    return NULL;
}

static void noop_free_hook(void* ptr) {
    (void)ptr;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check for meaningful testing
    // Need enough data for JSON input, buffer sizes, and operation selection
    if (size < 64) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size configuration data first
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 8;
    bool test_edge_cases = fdp.ConsumeBool();
    bool test_memory_hooks = fdp.ConsumeBool();
    bool use_formatted_print = fdp.ConsumeBool();
    int hook_config = fdp.ConsumeIntegral<uint8_t>() % 4;
    int buffer_size = fdp.ConsumeIntegralInRange<int>(1, 4096);
    int prebuffer_size = fdp.ConsumeIntegralInRange<int>(1, 2048);
    
    // Consume JSON input for parsing and manipulation
    std::string json_input = fdp.ConsumeRandomLengthString(2048);
    std::string original_string = fdp.ConsumeRandomLengthString(512);
    std::string new_string = fdp.ConsumeRandomLengthString(512);
    std::string key_name = fdp.ConsumeRandomLengthString(128);
    
    // Consume numeric values
    double number_value = fdp.ConsumeFloatingPoint<double>();
    bool bool_value = fdp.ConsumeBool();
    
    // Use operation to determine API flow with semantic diversity
    switch (operation) {
        case 0: {
            // Test cJSON_InitHooks with various configurations
            cJSON_Hooks hooks;
            
            switch (hook_config) {
                case 0:
                    // NULL hooks (use default allocators)
                    hooks.malloc_fn = NULL;
                    hooks.free_fn = NULL;
                    break;
                case 1:
                    // Custom malloc, NULL free
                    hooks.malloc_fn = custom_malloc_hook;
                    hooks.free_fn = NULL;
                    break;
                case 2:
                    // NULL malloc, custom free
                    hooks.malloc_fn = NULL;
                    hooks.free_fn = custom_free_hook;
                    break;
                case 3:
                    // Both custom hooks
                    hooks.malloc_fn = custom_malloc_hook;
                    hooks.free_fn = custom_free_hook;
                    break;
            }
            
            // Initialize hooks (target API with limited coverage)
            cJSON_InitHooks(&hooks);
            
            // Create and manipulate JSON with current hooks
            cJSON* root = cJSON_CreateObject();
            if (root != NULL) {
                cJSON_AddStringToObject(root, key_name.c_str(), original_string.c_str());
                cJSON_AddNumberToObject(root, "number", number_value);
                cJSON_AddBoolToObject(root, "bool", bool_value);
                
                // Print with current hooks
                char* printed = cJSON_Print(root);
                if (printed != NULL) {
                    free(printed);
                }
                cJSON_Delete(root);
            }
            
            // Test edge cases with hooks
            if (test_edge_cases) {
                cJSON* item = cJSON_CreateString("test_item");
                if (item != NULL) {
                    char* printed2 = cJSON_PrintBuffered(item, 1, use_formatted_print);
                    if (printed2 != NULL) {
                        free(printed2);
                    }
                    
                    // Zero buffer (edge case)
                    char* printed3 = cJSON_PrintBuffered(item, 0, use_formatted_print);
                    if (printed3 != NULL) {
                        free(printed3);
                    }
                    
                    // Large buffer
                    char* printed4 = cJSON_PrintBuffered(item, 65536, use_formatted_print);
                    if (printed4 != NULL) {
                        free(printed4);
                    }
                    
                    cJSON_Delete(item);
                }
                
                // Test with NULL item (edge case)
                char* null_printed = cJSON_PrintBuffered(NULL, prebuffer_size, use_formatted_print);
                if (null_printed != NULL) {
                    free(null_printed);
                }
            }
            
            // Test with manually created object
            cJSON* manual_obj = cJSON_CreateObject();
            if (manual_obj != NULL) {
                cJSON_AddStringToObject(manual_obj, "manual_key", original_string.c_str());
                cJSON_AddNumberToObject(manual_obj, "manual_number", number_value);
                char* manual_printed = cJSON_PrintBuffered(manual_obj, prebuffer_size, !use_formatted_print);
                if (manual_printed != NULL) {
                    free(manual_printed);
                }
                
                cJSON_Delete(manual_obj);
            }
            break;
        }
        
        case 1: {
            // Test cJSON_SetValuestring with various scenarios
            cJSON* string_obj = cJSON_CreateString(original_string.c_str());
            if (string_obj != NULL) {
                // Normal replacement
                char* result1 = cJSON_SetValuestring(string_obj, new_string.c_str());
                (void)result1;
                
                // Replace with empty string
                char* result2 = cJSON_SetValuestring(string_obj, "");
                (void)result2;
                
                // Replace with same string (should handle gracefully)
                char* result3 = cJSON_SetValuestring(string_obj, new_string.c_str());
                (void)result3;
                
                // Test with NULL string (edge case)
                if (test_edge_cases) {
                    char* result4 = cJSON_SetValuestring(string_obj, NULL);
                    (void)result4;
                }
                
                // Print results
                char* printed = cJSON_Print(string_obj);
                if (printed != NULL) {
                    free(printed);
                }
                
                cJSON_Delete(string_obj);
            }
            
            // Test with array item containing strings
            cJSON* array = cJSON_CreateArray();
            if (array != NULL) {
                cJSON_AddItemToArray(array, cJSON_CreateString("array_string_1"));
                cJSON_AddItemToArray(array, cJSON_CreateString("array_string_2"));
                
                // Try to modify first string in array
                cJSON* first_item = cJSON_GetArrayItem(array, 0);
                if (first_item != NULL && cJSON_IsString(first_item)) {
                    char* array_result = cJSON_SetValuestring(first_item, "modified_array_string");
                    (void)array_result;
                }
                
                cJSON_Delete(array);
            }
            break;
        }
        
        case 2: {
            // Test cJSON_PrintPreallocated with complex buffer management
            cJSON* root = cJSON_CreateObject();
            if (root != NULL) {
                // Add complex nested structure
                cJSON_AddStringToObject(root, key_name.c_str(), original_string.c_str());
                cJSON_AddNumberToObject(root, "number", number_value);
                
                cJSON* nested = cJSON_AddObjectToObject(root, "nested");
                if (nested != NULL) {
                    cJSON_AddBoolToObject(nested, "nested_bool", bool_value);
                    cJSON_AddStringToObject(nested, "nested_string", new_string.c_str());
                    
                    cJSON* deep_nested = cJSON_AddArrayToObject(nested, "deep_array");
                    if (deep_nested != NULL) {
                        for (int i = 0; i < 3; i++) {
                            cJSON_AddItemToArray(deep_nested, cJSON_CreateNumber(i * 10.0));
                        }
                    }
                }
                
                // Test with various buffer sizes
                char* buffer = (char*)malloc(buffer_size);
                if (buffer != NULL) {
                    // Test 1: Normal case
                    cJSON_bool success1 = cJSON_PrintPreallocated(root, buffer, buffer_size, use_formatted_print);
                    (void)success1;
                    
                    // Test 2: Small buffer (should fail for large JSON)
                    if (test_edge_cases && buffer_size > 10) {
                        cJSON_bool success2 = cJSON_PrintPreallocated(root, buffer, 10, use_formatted_print);
                        (void)success2;
                    }
                    
                    // Test 3: Zero buffer (edge case)
                    if (test_edge_cases) {
                        char zero_buffer[1];
                        cJSON_bool success3 = cJSON_PrintPreallocated(root, zero_buffer, 0, use_formatted_print);
                        (void)success3;
                    }
                    
                    free(buffer);
                }
                
                // Test with stack-allocated buffer
                char stack_buffer[1024];
                cJSON_bool stack_success = cJSON_PrintPreallocated(root, stack_buffer, sizeof(stack_buffer), !use_formatted_print);
                (void)stack_success;
                
                cJSON_Delete(root);
            }
            break;
        }
        
        case 3: {
            // Test cJSON_PrintBuffered with various configurations
            cJSON* obj = cJSON_CreateObject();
            if (obj != NULL) {
                cJSON_AddStringToObject(obj, "test_string", original_string.c_str());
                cJSON_AddNumberToObject(obj, "test_number", number_value);
                cJSON_AddBoolToObject(obj, "test_bool", bool_value);
                
                // Test with different buffer sizes
                char* buffered1 = cJSON_PrintBuffered(obj, buffer_size, use_formatted_print);
                if (buffered1 != NULL) {
                    free(buffered1);
                }
                
                // Test with small buffer
                if (test_edge_cases) {
                    char* buffered2 = cJSON_PrintBuffered(obj, 1, use_formatted_print);
                    if (buffered2 != NULL) {
                        free(buffered2);
                    }
                    
                    char* buffered3 = cJSON_PrintBuffered(obj, 0, use_formatted_print);
                    if (buffered3 != NULL) {
                        free(buffered3);
                    }
                }
                
                // Test with large buffer
                char* buffered4 = cJSON_PrintBuffered(obj, 65536, use_formatted_print);
                if (buffered4 != NULL) {
                    free(buffered4);
                }
                
                cJSON_Delete(obj);
            }
            
            // Test with array
            cJSON* array = cJSON_CreateArray();
            if (array != NULL) {
                for (int i = 0; i < 5 && fdp.remaining_bytes() > 0; i++) {
                    std::string array_item = fdp.ConsumeRandomLengthString(64);
                    cJSON_AddItemToArray(array, cJSON_CreateString(array_item.c_str()));
                }
                
                char* array_buffered = cJSON_PrintBuffered(array, buffer_size, !use_formatted_print);
                if (array_buffered != NULL) {
                    free(array_buffered);
                }
                
                cJSON_Delete(array);
            }
            break;
        }
        
        case 4: {
            // Combined workflow: parse -> modify -> print with hooks
            if (test_memory_hooks) {
                cJSON_Hooks workflow_hooks = {custom_malloc_hook, custom_free_hook};
                cJSON_InitHooks(&workflow_hooks);
            }
            
            // Parse JSON input
            cJSON* parsed = cJSON_Parse(json_input.c_str());
            if (parsed != NULL) {
                // Find and modify string values
                cJSON* child = parsed->child;
                while (child != NULL) {
                    if (cJSON_IsString(child)) {
                        char* modify_result = cJSON_SetValuestring(child, "modified_by_workflow");
                        (void)modify_result;
                    }
                    child = child->next;
                }
                
                // Print with buffered method
                char* workflow_print = cJSON_PrintBuffered(parsed, buffer_size, use_formatted_print);
                if (workflow_print != NULL) {
                    free(workflow_print);
                }
                
                // Print with preallocated method
                char prealloc_buffer[2048];
                cJSON_bool prealloc_success = cJSON_PrintPreallocated(parsed, prealloc_buffer, sizeof(prealloc_buffer), false);
                (void)prealloc_success;
                
                cJSON_Delete(parsed);
            }
            
            if (test_memory_hooks) {
                cJSON_Hooks default_hooks = {NULL, NULL};
                cJSON_InitHooks(&default_hooks);
            }
            break;
        }
        
        case 5: {
            // Test with parse options and combined operations
            const char* return_parse_end = NULL;
            size_t input_length = json_input.length();
            
            // Test various parsing options
            cJSON* parsed1 = cJSON_ParseWithLengthOpts(json_input.c_str(), input_length, &return_parse_end, false);
            if (parsed1 != NULL) {
                // Test SetValuestring on parsed content
                cJSON* string_item = cJSON_GetObjectItem(parsed1, "string_field");
                if (string_item == NULL && cJSON_IsArray(parsed1) && parsed1->child != NULL) {
                    string_item = parsed1->child;
                }
                
                if (string_item != NULL && cJSON_IsString(string_item)) {
                    char* parse_set_result = cJSON_SetValuestring(string_item, "parsed_modified");
                    (void)parse_set_result;
                }
                
                // Test buffered printing
                char* parse_print = cJSON_PrintBuffered(parsed1, buffer_size, true);
                if (parse_print != NULL) {
                    free(parse_print);
                }
                
                cJSON_Delete(parsed1);
            }
            
            // Test with require_null_terminated = true
            cJSON* parsed2 = cJSON_ParseWithLengthOpts(json_input.c_str(), input_length, NULL, true);
            if (parsed2 != NULL) {
                // Test preallocated printing
                char prealloc_buf[512];
                cJSON_bool prealloc_ok = cJSON_PrintPreallocated(parsed2, prealloc_buf, sizeof(prealloc_buf), false);
                (void)prealloc_ok;
                
                cJSON_Delete(parsed2);
            }
            break;
        }
        
        case 6: {
            // Edge case testing for all target APIs
            // Test hooks with various invalid configurations
            if (test_edge_cases) {
                // NULL hooks struct
                cJSON_InitHooks(NULL);
                
                // Hooks with failing malloc to test error paths
                cJSON_Hooks failing_hooks;
                failing_hooks.malloc_fn = failing_malloc_hook;
                failing_hooks.free_fn = noop_free_hook;
                cJSON_InitHooks(&failing_hooks);
                
                // Try to create something - should fail due to malloc returning NULL
                cJSON* failing_obj = cJSON_CreateObject();
                if (failing_obj != NULL) {
                    cJSON_Delete(failing_obj);
                }
                
                // Restore sensible defaults
            }
            
            // Test PrintBuffered with extreme values
            cJSON* simple = cJSON_CreateString("simple");
            if (simple != NULL) {
                // Very large prebuffer
                char* huge_buffer_print = cJSON_PrintBuffered(simple, 1024 * 1024, use_formatted_print);
                if (huge_buffer_print != NULL) {
                    free(huge_buffer_print);
                }
                
                // Large prebuffer value from fuzzer input
                size_t large_prebuffer = fdp.ConsumeIntegralInRange<size_t>(1024, 4096 * 1024);
                char* wrapped_print = cJSON_PrintBuffered(simple, large_prebuffer, use_formatted_print);
                if (wrapped_print != NULL) {
                    free(wrapped_print);
                }
                
                cJSON_Delete(simple);
            }
            break;
        }
        
        case 7: {
            // Memory stress test with hooks and repeated operations
            if (test_memory_hooks) {
                cJSON_Hooks stress_hooks = {custom_malloc_hook, custom_free_hook};
                cJSON_InitHooks(&stress_hooks);
            }
            
            // Create complex nested structure
            cJSON* stress_root = cJSON_CreateObject();
            if (stress_root != NULL) {
                // Add many items
                for (int i = 0; i < 10 && fdp.remaining_bytes() > 0; i++) {
                    std::string item_key = "item_" + std::to_string(i);
                    std::string item_value = fdp.ConsumeRandomLengthString(64);
                    cJSON_AddStringToObject(stress_root, item_key.c_str(), item_value.c_str());
                }
                
                // Repeatedly modify and print
                for (int i = 0; i < 3; i++) {
                    // Find a string item to modify
                    cJSON* string_item = NULL;
                    cJSON* child = stress_root->child;
                    while (child != NULL) {
                        if (cJSON_IsString(child)) {
                            string_item = child;
                            break;
                        }
                        child = child->next;
                    }
                    
                    if (string_item != NULL) {
                        std::string mod_value = "modified_" + std::to_string(i);
                        char* mod_result = cJSON_SetValuestring(string_item, mod_value.c_str());
                        (void)mod_result;
                    }
                    
                    // Print with different methods
                    if (i == 0) {
                        char* buffered = cJSON_PrintBuffered(stress_root, 1024, true);
                        if (buffered != NULL) free(buffered);
                    } else if (i == 1) {
                        char buffer[2048];
                        cJSON_bool prealloc = cJSON_PrintPreallocated(stress_root, buffer, sizeof(buffer), false);
                        (void)prealloc;
                    }
                }
                
                cJSON_Delete(stress_root);
            }
            
            if (test_memory_hooks) {
                cJSON_Hooks default_hooks = {NULL, NULL};
                cJSON_InitHooks(&default_hooks);
            }
            break;
        }
    }
    
    return 0;
}

} // namespace harness_012
