// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateIntArray at cJSON.c:2621:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
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

static int parse_int_from_data(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + sizeof(int) > size) {
        return 0;
    }
    int value;
    memcpy(&value, data + *offset, sizeof(int));
    *offset += sizeof(int);
    return value;
}

static const char* parse_string_from_data(const uint8_t *data, size_t size, size_t *offset, size_t max_len) {
    if (*offset >= size) {
        return NULL;
    }
    size_t remaining = size - *offset;
    size_t str_len = remaining < max_len ? remaining : max_len;
    if (str_len == 0) {
        return NULL;
    }
    
    static char str_buf[256];
    size_t copy_len = str_len < sizeof(str_buf) - 1 ? str_len : sizeof(str_buf) - 1;
    memcpy(str_buf, data + *offset, copy_len);
    str_buf[copy_len] = '\0';
    *offset += copy_len;
    return str_buf;
}

int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(int) * 2) {
        return 0;
    }
    
    size_t offset = 0;
    
    cJSON *root1 = cJSON_CreateObject();
    if (!root1) {
        return 0;
    }
    
    int array_count = parse_int_from_data(Data, Size, &offset);
    if (array_count < 0 || array_count > 100) {
        array_count = 10;
    }
    
    int *int_array = (int*)malloc(sizeof(int) * array_count);
    if (!int_array) {
        cJSON_Delete(root1);
        return 0;
    }
    
    for (int i = 0; i < array_count; i++) {
        int_array[i] = parse_int_from_data(Data, Size, &offset);
    }
    
    cJSON *int_array_obj = cJSON_CreateIntArray(int_array, array_count);
    free(int_array);
    
    if (!int_array_obj) {
        cJSON_Delete(root1);
        return 0;
    }
    
    int array_index = parse_int_from_data(Data, Size, &offset);
    cJSON *array_item = cJSON_GetArrayItem(int_array_obj, array_index);
    
    const char *key1 = parse_string_from_data(Data, Size, &offset, 50);
    if (!key1) {
        key1 = "test_key";
    }
    
    cJSON_AddItemToObject(root1, key1, int_array_obj);
    
    cJSON *obj2 = cJSON_CreateObject();
    if (!obj2) {
        cJSON_Delete(root1);
        return 0;
    }
    
    const char *str1 = parse_string_from_data(Data, Size, &offset, 100);
    if (!str1) {
        str1 = "test_string";
    }
    
    cJSON *str_obj1 = cJSON_CreateString(str1);
    if (!str_obj1) {
        cJSON_Delete(root1);
        cJSON_Delete(obj2);
        return 0;
    }
    
    const char *key2 = parse_string_from_data(Data, Size, &offset, 50);
    if (!key2) {
        key2 = "str_key";
    }
    
    cJSON_AddItemToObject(obj2, key2, str_obj1);
    
    cJSON *obj3 = cJSON_CreateObject();
    if (!obj3) {
        cJSON_Delete(root1);
        cJSON_Delete(obj2);
        return 0;
    }
    
    const char *str2 = parse_string_from_data(Data, Size, &offset, 100);
    if (!str2) {
        str2 = "another_string";
    }
    
    cJSON *str_obj2 = cJSON_CreateString(str2);
    if (!str_obj2) {
        cJSON_Delete(root1);
        cJSON_Delete(obj2);
        cJSON_Delete(obj3);
        return 0;
    }
    
    const char *key3 = parse_string_from_data(Data, Size, &offset, 50);
    if (!key3) {
        key3 = "another_key";
    }
    
    cJSON_AddItemToObject(obj3, key3, str_obj2);
    
    cJSON_Delete(int_array_obj);
    cJSON_Delete(obj2);
    cJSON_Delete(obj3);
    
    return 0;
}