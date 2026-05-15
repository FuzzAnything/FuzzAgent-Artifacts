// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Print at cJSON.c:1307:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"

static char* create_string_from_data(const uint8_t *data, size_t size) {
    if (size == 0) {
        char* str = (char*)malloc(1);
        if (str) str[0] = '\0';
        return str;
    }
    
    char* str = (char*)malloc(size + 1);
    if (!str) return NULL;
    
    memcpy(str, data, size);
    str[size] = '\0';
    return str;
}

static double create_number_from_data(const uint8_t *data, size_t size) {
    if (size == 0) return 0.0;
    
    double num = 0.0;
    size_t bytes_to_copy = size > sizeof(double) ? sizeof(double) : size;
    memcpy(&num, data, bytes_to_copy);
    return num;
}

int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    // Create root object
    cJSON *root = cJSON_CreateObject();
    if (!root) return 0;
    
    // Create string from fuzzer data
    char* str_data = create_string_from_data(Data, Size);
    if (!str_data) {
        cJSON_Delete(root);
        return 0;
    }
    
    // Create string JSON node
    cJSON *string_item = cJSON_CreateString(str_data);
    free(str_data);
    if (!string_item) {
        cJSON_Delete(root);
        return 0;
    }
    
    // Add string to root object
    if (!cJSON_AddItemToObject(root, "fuzz_string", string_item)) {
        cJSON_Delete(root);
        return 0;
    }
    
    // Create array
    cJSON *array = cJSON_CreateArray();
    if (!array) {
        cJSON_Delete(root);
        return 0;
    }
    
    // Add array to root object
    if (!cJSON_AddItemToObject(root, "fuzz_array", array)) {
        cJSON_Delete(root);
        return 0;
    }
    
    // Create inner object
    cJSON *inner_obj = cJSON_CreateObject();
    if (!inner_obj) {
        cJSON_Delete(root);
        return 0;
    }
    
    // Add inner object to array
    if (!cJSON_AddItemToArray(array, inner_obj)) {
        cJSON_Delete(root);
        return 0;
    }
    
    // Create first number from fuzzer data
    double num1 = create_number_from_data(Data, Size);
    cJSON *number_item1 = cJSON_CreateNumber(num1);
    if (!number_item1) {
        cJSON_Delete(root);
        return 0;
    }
    
    // Add first number to inner object
    if (!cJSON_AddItemToObject(inner_obj, "number1", number_item1)) {
        cJSON_Delete(root);
        return 0;
    }
    
    // Create second number from reversed fuzzer data
    double num2 = 0.0;
    if (Size > 0) {
        uint8_t reversed_data[sizeof(double)] = {0};
        size_t copy_size = Size > sizeof(double) ? sizeof(double) : Size;
        for (size_t i = 0; i < copy_size; i++) {
            reversed_data[i] = Data[copy_size - 1 - i];
        }
        memcpy(&num2, reversed_data, copy_size);
    }
    
    cJSON *number_item2 = cJSON_CreateNumber(num2);
    if (!number_item2) {
        cJSON_Delete(root);
        return 0;
    }
    
    // Add second number to inner object
    if (!cJSON_AddItemToObject(inner_obj, "number2", number_item2)) {
        cJSON_Delete(root);
        return 0;
    }
    
    // Print the JSON structure
    char *printed = cJSON_Print(root);
    if (printed) {
        free(printed);
    }
    
    // Clean up
    cJSON_Delete(root);
    
    return 0;
}