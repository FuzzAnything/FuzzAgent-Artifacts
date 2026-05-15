// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_Print at cJSON.c:1307:22 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static char *extract_string(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset >= size) {
        return NULL;
    }
    size_t start = *offset;
    while (*offset < size && data[*offset] != 0) {
        (*offset)++;
    }
    size_t len = *offset - start;
    char *str = malloc(len + 1);
    if (!str) {
        return NULL;
    }
    memcpy(str, data + start, len);
    str[len] = '\0';
    if (*offset < size) {
        (*offset)++; // Skip null terminator
    }
    return str;
}

static double extract_double(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + sizeof(double) > size) {
        return 0.0;
    }
    double value;
    memcpy(&value, data + *offset, sizeof(double));
    *offset += sizeof(double);
    return value;
}

int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    size_t offset = 0;

    // Extract keys and values from fuzzer input
    char *key1 = extract_string(Data, Size, &offset);
    char *key2 = extract_string(Data, Size, &offset);
    char *key3 = extract_string(Data, Size, &offset);
    char *key4 = extract_string(Data, Size, &offset);
    char *string_val = extract_string(Data, Size, &offset);
    double num1 = extract_double(Data, Size, &offset);
    double num2 = extract_double(Data, Size, &offset);

    // Use defaults if extraction failed
    if (!key1) key1 = strdup("key1");
    if (!key2) key2 = strdup("key2");
    if (!key3) key3 = strdup("key3");
    if (!key4) key4 = strdup("key4");
    if (!string_val) string_val = strdup("default");

    // Create root object
    cJSON *root = cJSON_CreateObject();
    if (!root) {
        goto cleanup;
    }

    // Create string item and add to root
    cJSON *string_item = cJSON_CreateString(string_val);
    if (string_item) {
        cJSON_AddItemToObject(root, key1, string_item);
    }

    // Create array and add to root
    cJSON *array = cJSON_CreateArray();
    if (array) {
        cJSON_AddItemToObject(root, key2, array);
    }

    // Create inner object and add to array
    cJSON *inner_obj = cJSON_CreateObject();
    if (inner_obj && array) {
        cJSON_AddItemToArray(array, inner_obj);
    }

    // Create first number and add to inner object
    cJSON *num_item1 = cJSON_CreateNumber(num1);
    if (num_item1 && inner_obj) {
        cJSON_AddItemToObject(inner_obj, key3, num_item1);
    }

    // Create second number and add to inner object
    cJSON *num_item2 = cJSON_CreateNumber(num2);
    if (num_item2 && inner_obj) {
        cJSON_AddItemToObject(inner_obj, key4, num_item2);
    }

    // Print the JSON structure
    char *printed = cJSON_Print(root);
    if (printed) {
        free(printed);
    }

    // Cleanup
    cJSON_Delete(root);

cleanup:
    free(key1);
    free(key2);
    free(key3);
    free(key4);
    free(string_val);
    
    return 0;
}