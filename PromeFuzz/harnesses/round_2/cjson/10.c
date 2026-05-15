// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateIntArray at cJSON.c:2621:23 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    // Use Data to generate various inputs for the API calls
    // We'll extract different values from the fuzzer input
    
    // For integer array - use first part of data for integers
    int int_count = 0;
    int *int_array = NULL;
    
    if (Size >= sizeof(int)) {
        // Determine count from first byte, but limit to reasonable size
        int_count = (Data[0] % 16) + 1;  // 1-16 integers
        if (int_count * sizeof(int) <= Size - 1) {
            int_array = (int*)malloc(int_count * sizeof(int));
            if (int_array) {
                for (int i = 0; i < int_count; i++) {
                    // Extract integers from data (simple approach)
                    if (1 + i * sizeof(int) + sizeof(int) <= Size) {
                        int_array[i] = *(int*)(Data + 1 + i * sizeof(int));
                    } else {
                        int_array[i] = 0;
                    }
                }
            }
        }
    }
    
    // For keys and strings - use remaining data
    size_t str_offset = 1 + (int_count * sizeof(int));
    if (str_offset > Size) str_offset = Size;
    
    // Create keys from data
    char key1[32] = "key1";
    char key2[32] = "key2";
    char key3[32] = "key3";
    
    // Create strings from remaining data
    char str1[256] = "default1";
    char str2[256] = "default2";
    
    if (Size - str_offset > 0) {
        size_t copy_len = Size - str_offset;
        if (copy_len > 255) copy_len = 255;
        
        // Create string1
        memcpy(str1, Data + str_offset, copy_len);
        str1[copy_len] = '\0';
        
        // Create string2 from different part of data
        size_t str2_offset = str_offset + (copy_len / 2);
        if (str2_offset < Size) {
            size_t str2_len = Size - str2_offset;
            if (str2_len > 255) str2_len = 255;
            memcpy(str2, Data + str2_offset, str2_len);
            str2[str2_len] = '\0';
        }
    }
    
    // Now execute the API calls in the specified order
    
    // 1. cJSON_CreateObject
    cJSON *obj1 = cJSON_CreateObject();
    if (!obj1) goto cleanup;
    
    // 2. cJSON_CreateIntArray
    cJSON *int_array_obj = NULL;
    if (int_array) {
        int_array_obj = cJSON_CreateIntArray(int_array, int_count);
    }
    
    // 3. cJSON_GetArrayItem
    cJSON *array_item = NULL;
    if (int_array_obj) {
        // Try different indices to explore more states
        int index = (Data[0] % 10);  // 0-9
        array_item = cJSON_GetArrayItem(int_array_obj, index);
    }
    
    // 4. cJSON_AddItemToObject
    if (obj1 && int_array_obj) {
        cJSON_AddItemToObject(obj1, key1, int_array_obj);
    }
    
    // 5. cJSON_CreateObject
    cJSON *obj2 = cJSON_CreateObject();
    if (!obj2) goto cleanup_obj1;
    
    // 6. cJSON_CreateString
    cJSON *str_obj1 = cJSON_CreateString(str1);
    
    // 7. cJSON_AddItemToObject
    if (obj2 && str_obj1) {
        cJSON_AddItemToObject(obj2, key2, str_obj1);
    }
    
    // 8. cJSON_CreateObject
    cJSON *obj3 = cJSON_CreateObject();
    if (!obj3) goto cleanup_obj2;
    
    // 9. cJSON_CreateString
    cJSON *str_obj2 = cJSON_CreateString(str2);
    
    // 10. cJSON_AddItemToObject
    if (obj3 && str_obj2) {
        cJSON_AddItemToObject(obj3, key3, str_obj2);
    }
    
    // 11-13. cJSON_Delete calls in specified order
    if (obj1) cJSON_Delete(obj1);
    if (obj2) cJSON_Delete(obj2);
    if (obj3) cJSON_Delete(obj3);
    
    goto cleanup;
    
cleanup_obj2:
    if (obj2) cJSON_Delete(obj2);
cleanup_obj1:
    if (obj1) cJSON_Delete(obj1);
cleanup:
    if (int_array) free(int_array);
    
    return 0;
}