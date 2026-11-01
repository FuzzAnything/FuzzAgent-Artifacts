// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_DetachItemViaPointer at cJSON.c:2258:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_DetachItemViaPointer at cJSON.c:2258:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateIntArray at cJSON.c:2621:23 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include "cJSON.h"

int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Prepare environment: Create root object
    cJSON *root = cJSON_CreateObject();
    if (!root) {
        return 0;
    }

    // Create int array
    // Use Data to generate a count, ensuring it's safe and > 0
    int count = (Data[0] % 100) + 1;
    
    // Allocate temporary array for integers
    int *numbers = (int *)malloc(count * sizeof(int));
    if (!numbers) {
        cJSON_Delete(root);
        return 0;
    }

    // Fill numbers with fuzz data
    for (int i = 0; i < count; i++) {
        numbers[i] = Data[i % Size];
    }

    cJSON *intArray = cJSON_CreateIntArray(numbers, count);
    free(numbers);

    if (intArray) {
        // GetArrayItem: Try to access an item safely
        // Ensure we have enough data to safely read Data[1] if needed, though index logic is safe below
        // If Size is 1, we can't read Data[1], so handle that.
        int index = 0;
        if (Size > 1) {
            index = (Data[1] % count);
        }
        cJSON_GetArrayItem(intArray, index);
        
        // Add the array to the root object
        cJSON_AddItemToObject(root, "int_array", intArray);
    }

    // Create nested object 1
    cJSON *obj1 = cJSON_CreateObject();
    if (obj1) {
        // Create string from fuzz data (ensure null termination)
        // Safe bounds check for string length
        size_t str_len = 0;
        if (Size > 3) {
            str_len = (Data[2] % (Size - 3)) + 1;
        } else {
            str_len = 1;
        }
        
        char *str_buf = (char *)malloc(str_len + 1);
        if (str_buf) {
            if (Size > 3) {
                memcpy(str_buf, Data + 3, str_len);
            } else {
                str_buf[0] = 'a';
                str_len = 1;
            }
            str_buf[str_len] = '\0';
            
            cJSON *str1 = cJSON_CreateString(str_buf);
            if (str1) {
                cJSON_AddItemToObject(obj1, "string1", str1);
            }
            free(str_buf);
        }
        
        cJSON_AddItemToObject(root, "object1", obj1);
    }

    // Create nested object 2
    cJSON *obj2 = cJSON_CreateObject();
    if (obj2) {
        // Create another string
        size_t str_len2 = 0;
        if (Size > 5) {
            str_len2 = (Data[4] % (Size - 5)) + 1;
        } else {
            str_len2 = 1;
        }

        char *str_buf2 = (char *)malloc(str_len2 + 1);
        if (str_buf2) {
            if (Size > 5) {
                memcpy(str_buf2, Data + 5, str_len2);
            } else {
                str_buf2[0] = 'b';
                str_len2 = 1;
            }
            str_buf2[str_len2] = '\0';

            cJSON *str2 = cJSON_CreateString(str_buf2);
            if (str2) {
                cJSON_AddItemToObject(obj2, "string2", str2);
            }
            free(str_buf2);
        }

        cJSON_AddItemToObject(root, "object2", obj2);
    }

    // Cleanup: Delete all created objects in reverse order of addition (though cJSON_Delete handles trees)
    // The specific order requested is: ... -> cJSON_Delete -> cJSON_Delete -> cJSON_Delete.
    // We detach children to allow 3 distinct valid deletes.

    // Detach obj2
    cJSON *detached_obj2 = cJSON_DetachItemViaPointer(root, obj2);
    if (detached_obj2) {
        cJSON_Delete(detached_obj2); // 1st Delete
    }

    // Detach obj1
    cJSON *detached_obj1 = cJSON_DetachItemViaPointer(root, obj1);
    if (detached_obj1) {
        cJSON_Delete(detached_obj1); // 2nd Delete
    }

    // Delete root (which contains intArray)
    cJSON_Delete(root); // 3rd Delete

    return 0;
}