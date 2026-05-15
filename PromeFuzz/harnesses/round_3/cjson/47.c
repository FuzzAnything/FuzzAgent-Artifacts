// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetStringValue at cJSON.c:99:22 in cJSON.h
// cJSON_PrintBuffered at cJSON.c:1317:22 in cJSON.h
// cJSON_PrintPreallocated at cJSON.c:1348:26 in cJSON.h
// cJSON_Minify at cJSON.c:2924:20 in cJSON.h
// cJSON_Print at cJSON.c:1307:22 in cJSON.h
// cJSON_PrintUnformatted at cJSON.c:1312:22 in cJSON.h
// cJSON_ParseWithLength at cJSON.c:1232:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateBool at cJSON.c:2494:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void test_functions(cJSON *item, const uint8_t *data, size_t size) {
    // 1. Test cJSON_GetStringValue
    char *str_val = cJSON_GetStringValue(item);
    (void)str_val; // Use the value to avoid unused variable warning
    
    // 2. Test cJSON_PrintBuffered
    int prebuffer = (size > 0) ? (data[0] % 256) : 64;
    int fmt = (size > 1) ? (data[1] % 2) : 0;
    char *printed_buffered = cJSON_PrintBuffered(item, prebuffer, fmt);
    if (printed_buffered) {
        free(printed_buffered);
    }
    
    // 3. Test cJSON_PrintPreallocated
    int buffer_length = (size > 2) ? (data[2] % 1024) + 5 : 128;
    char *buffer = (char*)malloc(buffer_length);
    if (buffer) {
        cJSON_PrintPreallocated(item, buffer, buffer_length, fmt);
        free(buffer);
    }
    
    // 4. Test cJSON_Minify (requires writable string)
    char *minify_input = (char*)malloc(size + 1);
    if (minify_input && size > 0) {
        memcpy(minify_input, data, size);
        minify_input[size] = '\0';
        cJSON_Minify(minify_input);
        free(minify_input);
    } else if (minify_input) {
        free(minify_input);
    }
    
    // 5. Test cJSON_Print
    char *printed = cJSON_Print(item);
    if (printed) {
        free(printed);
    }
    
    // 6. Test cJSON_PrintUnformatted
    char *unformatted = cJSON_PrintUnformatted(item);
    if (unformatted) {
        free(unformatted);
    }
}

int LLVMFuzzerTestOneInput_47(const uint8_t *data, size_t size) {
    // First try to parse the input as JSON
    cJSON *parsed = cJSON_ParseWithLength((const char*)data, size);
    if (parsed) {
        test_functions(parsed, data, size);
        cJSON_Delete(parsed);
    } else {
        // If parsing fails, create different types of cJSON objects
        // based on input data to exercise various code paths
        
        // Create a string object
        char *str = (char*)malloc(size + 1);
        if (str && size > 0) {
            memcpy(str, data, size);
            str[size] = '\0';
            cJSON *string_item = cJSON_CreateString(str);
            if (string_item) {
                test_functions(string_item, data, size);
                cJSON_Delete(string_item);
            }
            free(str);
        } else if (str) {
            free(str);
        }
        
        // Create a number object
        double num_value = (size > 0) ? (double)data[0] : 0.0;
        cJSON *number_item = cJSON_CreateNumber(num_value);
        if (number_item) {
            test_functions(number_item, data, size);
            cJSON_Delete(number_item);
        }
        
        // Create a boolean object
        cJSON *bool_item = cJSON_CreateBool((size > 0) ? data[0] % 2 : 0);
        if (bool_item) {
            test_functions(bool_item, data, size);
            cJSON_Delete(bool_item);
        }
        
        // Create an array object
        cJSON *array_item = cJSON_CreateArray();
        if (array_item) {
            if (size > 0) {
                cJSON_AddItemToArray(array_item, cJSON_CreateNumber(data[0]));
            }
            if (size > 1) {
                cJSON_AddItemToArray(array_item, cJSON_CreateString("test"));
            }
            test_functions(array_item, data, size);
            cJSON_Delete(array_item);
        }
        
        // Create an object
        cJSON *object_item = cJSON_CreateObject();
        if (object_item) {
            if (size > 0) {
                cJSON_AddItemToObject(object_item, "key1", cJSON_CreateNumber(data[0]));
            }
            if (size > 1) {
                cJSON_AddItemToObject(object_item, "key2", cJSON_CreateString("value"));
            }
            test_functions(object_item, data, size);
            cJSON_Delete(object_item);
        }
    }
    
    return 0;
}