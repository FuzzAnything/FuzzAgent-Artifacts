// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateIntArray at cJSON.c:2621:23 in cJSON.h
// cJSON_CreateStringArray at cJSON.c:2741:23 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int parse_int_from_data(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + sizeof(int) > size) {
        return 0;
    }
    int value;
    memcpy(&value, data + *offset, sizeof(int));
    *offset += sizeof(int);
    return value;
}

static char* parse_string_from_data(const uint8_t *data, size_t size, size_t *offset, size_t max_len) {
    if (*offset >= size) {
        return NULL;
    }
    size_t remaining = size - *offset;
    size_t str_len = remaining < max_len ? remaining : max_len;
    if (str_len == 0) {
        return NULL;
    }
    char* str = (char*)malloc(str_len + 1);
    if (!str) {
        return NULL;
    }
    memcpy(str, data + *offset, str_len);
    str[str_len] = '\0';
    *offset += str_len;
    return str;
}

static void free_string_array(char** strings, int count) {
    if (strings) {
        for (int i = 0; i < count; i++) {
            free(strings[i]);
        }
        free(strings);
    }
}

int LLVMFuzzerTestOneInput_45(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(int)) {
        return 0;
    }

    size_t offset = 0;
    
    // Test cJSON_CreateIntArray
    int int_count = parse_int_from_data(Data, Size, &offset);
    if (int_count < 0) int_count = -int_count;
    if (int_count > 100) int_count = 100;
    
    int* int_array = NULL;
    if (int_count > 0) {
        int_array = (int*)malloc(int_count * sizeof(int));
        if (int_array) {
            for (int i = 0; i < int_count; i++) {
                int_array[i] = parse_int_from_data(Data, Size, &offset);
            }
        }
    }
    
    cJSON* int_json_array = cJSON_CreateIntArray(int_array, int_count);
    free(int_array);
    
    // Test cJSON_CreateStringArray
    int str_count = parse_int_from_data(Data, Size, &offset);
    if (str_count < 0) str_count = -str_count;
    if (str_count > 50) str_count = 50;
    
    char** string_array = NULL;
    if (str_count > 0) {
        string_array = (char**)malloc(str_count * sizeof(char*));
        if (string_array) {
            for (int i = 0; i < str_count; i++) {
                string_array[i] = parse_string_from_data(Data, Size, &offset, 100);
            }
        }
    }
    
    cJSON* str_json_array = cJSON_CreateStringArray((const char* const*)string_array, str_count);
    free_string_array(string_array, str_count);
    
    // Test cJSON_GetArraySize with various inputs
    cJSON_GetArraySize(NULL);
    if (int_json_array) {
        cJSON_GetArraySize(int_json_array);
    }
    if (str_json_array) {
        cJSON_GetArraySize(str_json_array);
    }
    
    // Test cJSON_IsArray with various inputs
    cJSON_IsArray(NULL);
    if (int_json_array) {
        cJSON_IsArray(int_json_array);
    }
    if (str_json_array) {
        cJSON_IsArray(str_json_array);
    }
    
    // Test cJSON_GetArrayItem with various indices
    if (int_json_array) {
        int array_size = cJSON_GetArraySize(int_json_array);
        for (int i = -5; i < array_size + 5; i++) {
            cJSON_GetArrayItem(int_json_array, i);
        }
    }
    
    if (str_json_array) {
        int array_size = cJSON_GetArraySize(str_json_array);
        for (int i = -5; i < array_size + 5; i++) {
            cJSON_GetArrayItem(str_json_array, i);
        }
    }
    
    // Test cJSON_InsertItemInArray
    if (int_json_array) {
        int insert_index = parse_int_from_data(Data, Size, &offset);
        cJSON* new_item = cJSON_CreateNumber(42.0);
        if (new_item) {
            cJSON_InsertItemInArray(int_json_array, insert_index, new_item);
        }
    }
    
    if (str_json_array) {
        int insert_index = parse_int_from_data(Data, Size, &offset);
        cJSON* new_item = cJSON_CreateString("test");
        if (new_item) {
            cJSON_InsertItemInArray(str_json_array, insert_index, new_item);
        }
    }
    
    // Cleanup
    if (int_json_array) {
        cJSON_Delete(int_json_array);
    }
    if (str_json_array) {
        cJSON_Delete(str_json_array);
    }
    
    return 0;
}