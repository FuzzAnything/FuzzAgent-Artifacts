// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateIntArray at cJSON.c:2621:23 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Create first object
    cJSON *object1 = cJSON_CreateObject();
    if (!object1) return 0;
    
    // Create integer array from fuzzer data
    int count = (int)(Size / sizeof(int));
    if (count > 100) count = 100; // Limit array size for safety
    int *numbers = (int*)malloc(count * sizeof(int));
    if (!numbers) {
        cJSON_Delete(object1);
        return 0;
    }
    
    // Copy fuzzer data to integer array
    for (int i = 0; i < count; i++) {
        numbers[i] = (int)Data[i % Size];
    }
    
    cJSON *intArray = cJSON_CreateIntArray(numbers, count);
    free(numbers);
    
    if (intArray) {
        // Get array item (safe even with empty array)
        int index = (int)Data[0] % (count > 0 ? count : 1);
        cJSON *arrayItem = cJSON_GetArrayItem(intArray, index);
        
        // Add array to first object with a dummy key
        cJSON_AddItemToObject(object1, "fuzz_array", intArray);
    }
    
    // Create second object and string
    cJSON *object2 = cJSON_CreateObject();
    if (object2) {
        // Create string from fuzzer data
        char *str_data = (char*)malloc(Size + 1);
        if (str_data) {
            memcpy(str_data, Data, Size);
            str_data[Size] = '\0';
            cJSON *string1 = cJSON_CreateString(str_data);
            free(str_data);
            
            if (string1) {
                cJSON_AddItemToObject(object2, "fuzz_string", string1);
            }
        }
        
        // Create third object and string
        cJSON *object3 = cJSON_CreateObject();
        if (object3) {
            // Create another string from fuzzer data (reversed)
            char *str_data2 = (char*)malloc(Size + 1);
            if (str_data2) {
                for (size_t i = 0; i < Size; i++) {
                    str_data2[i] = Data[Size - 1 - i];
                }
                str_data2[Size] = '\0';
                cJSON *string2 = cJSON_CreateString(str_data2);
                free(str_data2);
                
                if (string2) {
                    cJSON_AddItemToObject(object3, "fuzz_string2", string2);
                }
            }
            
            // Clean up objects
            cJSON_Delete(object3);
        }
        
        cJSON_Delete(object2);
    }
    
    cJSON_Delete(object1);
    
    return 0;
}