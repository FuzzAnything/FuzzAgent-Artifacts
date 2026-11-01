// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateStringArray at cJSON.c:2741:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_PrintBuffered at cJSON.c:1317:22 in cJSON.h
// cJSON_PrintBuffered at cJSON.c:1317:22 in cJSON.h
// cJSON_PrintBuffered at cJSON.c:1317:22 in cJSON.h
// cJSON_CreateStringReference at cJSON.c:2548:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_SetValuestring at cJSON.c:435:21 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddStringToObject at cJSON.c:2210:22 in cJSON.h
// cJSON_AddStringToObject at cJSON.c:2210:22 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <cJSON.h>

static void cleanup_cjson(cJSON *item) {
    if (item) {
        cJSON_Delete(item);
    }
}

int LLVMFuzzerTestOneInput_60(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Ensure we have at least one byte to avoid immediate NULL pointer issues
    const char *input_str = (const char *)Data;
    
    // Test 1: cJSON_CreateStringReference
    // Create a reference to a portion of the input data (treat as string)
    // We need a valid string, so we ensure null termination if not present
    char *safe_str = NULL;
    if (Size > 0) {
        safe_str = malloc(Size + 1);
        if (safe_str) {
            memcpy(safe_str, Data, Size);
            safe_str[Size] = '\0';
        }
    }
    
    cJSON *ref_node = NULL;
    if (safe_str) {
        ref_node = cJSON_CreateStringReference(safe_str);
        // ref_node is a reference, do not delete it here, we'll handle it with the parent if attached
        // For standalone reference, we just let it exist or attach to an object
    }

    // Test 2: cJSON_CreateString
    cJSON *str_node = NULL;
    if (safe_str) {
        str_node = cJSON_CreateString(safe_str);
    }

    // Test 3: cJSON_SetValuestring
    // We need a valid cJSON_String object that is NOT a reference
    if (str_node && safe_str) {
        // Create a new string to set as value
        char *new_val = malloc(Size + 1);
        if (new_val) {
            memcpy(new_val, Data, Size);
            new_val[Size] = '\0';
            cJSON_SetValuestring(str_node, new_val);
            free(new_val);
        }
    }

    // Test 4: cJSON_AddStringToObject
    cJSON *obj = cJSON_CreateObject();
    if (obj && safe_str) {
        // Add a string to the object
        cJSON_AddStringToObject(obj, "fuzz_key", safe_str);
        
        // Try adding the reference node if it exists (though it's a reference, AddStringToObject creates a new one usually, 
        // but let's try to add a regular string node manually to test logic if needed, 
        // but the API specifically takes a string literal. 
        // Let's add another key with a different value
        char *alt_str = malloc(Size + 1);
        if (alt_str) {
            memcpy(alt_str, Data, Size);
            alt_str[Size] = '\0';
            // Reverse the string for variety
            int len = strlen(alt_str);
            for (int i = 0; i < len / 2; i++) {
                char tmp = alt_str[i];
                alt_str[i] = alt_str[len - 1 - i];
                alt_str[len - 1 - i] = tmp;
            }
            cJSON_AddStringToObject(obj, "alt_key", alt_str);
            free(alt_str);
        }
    }

    // Test 5: cJSON_CreateStringArray
    // Create a small array of strings from the input
    if (Size > 0) {
        const char *arr[3];
        int count = 0;
        
        // Split input into chunks for array
        if (Size >= 1) {
            arr[0] = safe_str;
            count++;
        }
        if (Size >= 2) {
            // Create a second string from the middle
            char *mid_str = malloc(Size + 1);
            if (mid_str) {
                memcpy(mid_str, Data, Size);
                mid_str[Size] = '\0';
                // Modify slightly
                mid_str[0] = 'X';
                arr[1] = mid_str;
                count++;
            }
        }
        if (Size >= 3) {
            char *end_str = malloc(Size + 1);
            if (end_str) {
                memcpy(end_str, Data, Size);
                end_str[Size] = '\0';
                end_str[0] = 'Y';
                arr[2] = end_str;
                count++;
            }
        }
        
        cJSON *arr_node = cJSON_CreateStringArray(arr, count);
        if (arr_node) {
            // Cleanup the manually allocated strings in the array if they were not the original safe_str
            // Note: cJSON_CreateStringArray copies the strings, so we can free our temp allocations
            if (count > 1 && arr[1]) free((void*)arr[1]);
            if (count > 2 && arr[2]) free((void*)arr[2]);
            
            // Add array to object if exists
            if (obj) {
                cJSON_AddItemToObject(obj, "array_key", arr_node);
            } else {
                cleanup_cjson(arr_node);
            }
        }
    }

    // Test 6: cJSON_PrintBuffered
    if (obj) {
        // Try different buffer sizes and formats
        char *output = cJSON_PrintBuffered(obj, Size * 2, 0);
        if (output) {
            free(output);
        }
        
        output = cJSON_PrintBuffered(obj, Size * 2, 1);
        if (output) {
            free(output);
        }
        
        // Try with a very small buffer guess to trigger reallocation logic
        output = cJSON_PrintBuffered(obj, 1, 0);
        if (output) {
            free(output);
        }
    }

    // Cleanup
    if (obj) {
        // If we added ref_node to obj, it won't be freed by cJSON_Delete (which is correct for references)
        // But ref_node points to safe_str which we allocated. 
        // Since ref_node was created via CreateStringReference, it does not own the memory.
        // We must ensure safe_str is freed.
        cleanup_cjson(obj);
    }
    
    if (str_node) {
        cleanup_cjson(str_node);
    }
    
    if (safe_str) {
        free(safe_str);
    }

    return 0;
}