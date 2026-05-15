// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateIntArray at cJSON.c:2621:23 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static char* extract_string(const uint8_t* data, size_t size, size_t* pos, size_t max_len) {
    if (*pos >= size) {
        return NULL;
    }
    size_t len = max_len;
    if (*pos + len > size) {
        len = size - *pos;
    }
    char* str = (char*)malloc(len + 1);
    if (str == NULL) {
        return NULL;
    }
    memcpy(str, data + *pos, len);
    str[len] = '\0';
    *pos += len;
    return str;
}

int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Extract parameters from fuzzer input
    size_t pos = 0;
    
    // First byte determines array count
    int count = Data[pos] % 10;
    pos++;
    
    // Extract integer array data
    int* numbers = NULL;
    if (count > 0) {
        numbers = (int*)malloc(count * sizeof(int));
        if (numbers == NULL) {
            return 0;
        }
        for (int i = 0; i < count; i++) {
            if (pos + sizeof(int) <= Size) {
                numbers[i] = *(int*)(Data + pos);
                pos += sizeof(int);
            } else {
                numbers[i] = 0;
            }
        }
    }
    
    // Extract index for array access
    int index = 0;
    if (pos < Size) {
        index = Data[pos] % (count + 1); // Avoid division by zero
        pos++;
    }
    
    // Extract strings for keys and values
    char* key1 = extract_string(Data, Size, &pos, 10);
    char* key2 = extract_string(Data, Size, &pos, 10);
    char* key3 = extract_string(Data, Size, &pos, 10);
    char* str1 = extract_string(Data, Size, &pos, 20);
    char* str2 = extract_string(Data, Size, &pos, 20);
    
    // Use defaults if extraction failed
    if (key1 == NULL) key1 = strdup("key1");
    if (key2 == NULL) key2 = strdup("key2");
    if (key3 == NULL) key3 = strdup("key3");
    if (str1 == NULL) str1 = strdup("string1");
    if (str2 == NULL) str2 = strdup("string2");
    
    // Create first object
    cJSON* obj1 = cJSON_CreateObject();
    
    // Create integer array
    cJSON* int_array = cJSON_CreateIntArray(numbers, count);
    
    // Get array item (may be NULL)
    cJSON* array_item = cJSON_GetArrayItem(int_array, index);
    
    // Add array to first object
    cJSON_AddItemToObject(obj1, key1, int_array);
    
    // Create second object
    cJSON* obj2 = cJSON_CreateObject();
    
    // Create first string
    cJSON* json_str1 = cJSON_CreateString(str1);
    
    // Add string to second object
    cJSON_AddItemToObject(obj2, key2, json_str1);
    
    // Create third object
    cJSON* obj3 = cJSON_CreateObject();
    
    // Create second string
    cJSON* json_str2 = cJSON_CreateString(str2);
    
    // Add string to third object
    cJSON_AddItemToObject(obj3, key3, json_str2);
    
    // Clean up - delete all objects
    cJSON_Delete(obj1);
    cJSON_Delete(obj2);
    cJSON_Delete(obj3);
    
    // Free allocated memory
    free(numbers);
    free(key1);
    free(key2);
    free(key3);
    free(str1);
    free(str2);
    
    return 0;
}