// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateIntArray at cJSON.c:2621:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "cJSON.h"

int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    // Create first object
    cJSON *obj1 = cJSON_CreateObject();
    if (!obj1) {
        return 0;
    }

    // Create integer array from fuzzer data
    int count = (int)(Size / sizeof(int));
    if (count > 10) count = 10; // Limit array size for safety
    int *numbers = (int*)Data;
    
    cJSON *int_array = cJSON_CreateIntArray(numbers, count);
    if (!int_array) {
        cJSON_Delete(obj1);
        return 0;
    }

    // Get array item (use modulo to ensure valid index)
    int index = (int)(Data[0] % (count > 0 ? count : 1));
    cJSON *array_item = cJSON_GetArrayItem(int_array, index);

    // Add integer array to first object
    const char *key1 = "int_array";
    cJSON_AddItemToObject(obj1, key1, int_array);

    // Create second object
    cJSON *obj2 = cJSON_CreateObject();
    if (!obj2) {
        cJSON_Delete(obj1);
        return 0;
    }

    // Create string from fuzzer data
    char *str_data = (char*)malloc(Size + 1);
    if (!str_data) {
        cJSON_Delete(obj2);
        cJSON_Delete(obj1);
        return 0;
    }
    
    memcpy(str_data, Data, Size);
    str_data[Size] = '\0';
    
    cJSON *str1 = cJSON_CreateString(str_data);
    free(str_data);
    
    if (!str1) {
        cJSON_Delete(obj2);
        cJSON_Delete(obj1);
        return 0;
    }

    // Add string to second object
    const char *key2 = "string";
    cJSON_AddItemToObject(obj2, key2, str1);

    // Create third object
    cJSON *obj3 = cJSON_CreateObject();
    if (!obj3) {
        cJSON_Delete(obj2);
        cJSON_Delete(obj1);
        return 0;
    }

    // Create another string from fuzzer data (reversed)
    char *str_data2 = (char*)malloc(Size + 1);
    if (!str_data2) {
        cJSON_Delete(obj3);
        cJSON_Delete(obj2);
        cJSON_Delete(obj1);
        return 0;
    }
    
    for (size_t i = 0; i < Size; i++) {
        str_data2[i] = Data[Size - 1 - i];
    }
    str_data2[Size] = '\0';
    
    cJSON *str2 = cJSON_CreateString(str_data2);
    free(str_data2);
    
    if (!str2) {
        cJSON_Delete(obj3);
        cJSON_Delete(obj2);
        cJSON_Delete(obj1);
        return 0;
    }

    // Add second string to third object
    const char *key3 = "reversed_string";
    cJSON_AddItemToObject(obj3, key3, str2);

    // Clean up in reverse order
    cJSON_Delete(obj3);
    cJSON_Delete(obj2);
    cJSON_Delete(obj1);

    return 0;
}