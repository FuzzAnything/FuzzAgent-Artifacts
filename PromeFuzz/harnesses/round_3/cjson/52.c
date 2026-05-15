// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetStringValue at cJSON.c:99:22 in cJSON.h
// cJSON_PrintBuffered at cJSON.c:1317:22 in cJSON.h
// cJSON_PrintPreallocated at cJSON.c:1348:26 in cJSON.h
// cJSON_Minify at cJSON.c:2924:20 in cJSON.h
// cJSON_Print at cJSON.c:1307:22 in cJSON.h
// cJSON_PrintUnformatted at cJSON.c:1312:22 in cJSON.h
// cJSON_ParseWithLength at cJSON.c:1232:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddStringToObject at cJSON.c:2210:22 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_AddBoolToObject at cJSON.c:2186:22 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void test_cJSON_GetStringValue(const cJSON *item) {
    char *str_value = cJSON_GetStringValue(item);
    (void)str_value; // Use the value to avoid unused variable warning
}

static void test_cJSON_PrintBuffered(const cJSON *item) {
    // Test with different buffer sizes and formats
    int prebuffer_sizes[] = {0, 1, 64, 1024, 4096};
    cJSON_bool formats[] = {0, 1};
    
    for (size_t i = 0; i < sizeof(prebuffer_sizes)/sizeof(prebuffer_sizes[0]); i++) {
        for (size_t j = 0; j < sizeof(formats)/sizeof(formats[0]); j++) {
            char *result = cJSON_PrintBuffered(item, prebuffer_sizes[i], formats[j]);
            if (result != NULL) {
                free(result);
            }
        }
    }
}

static void test_cJSON_PrintPreallocated(cJSON *item) {
    // Test with different buffer sizes and formats
    int buffer_sizes[] = {64, 256, 1024, 4096};
    cJSON_bool formats[] = {0, 1};
    
    for (size_t i = 0; i < sizeof(buffer_sizes)/sizeof(buffer_sizes[0]); i++) {
        char *buffer = (char *)malloc(buffer_sizes[i]);
        if (buffer != NULL) {
            for (size_t j = 0; j < sizeof(formats)/sizeof(formats[0]); j++) {
                cJSON_PrintPreallocated(item, buffer, buffer_sizes[i], formats[j]);
            }
            free(buffer);
        }
    }
}

static void test_cJSON_Minify(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    // Create a writable copy of the input
    char *json_str = (char *)malloc(size + 1);
    if (json_str == NULL) return;
    
    memcpy(json_str, data, size);
    json_str[size] = '\0';
    
    cJSON_Minify(json_str);
    free(json_str);
}

static void test_cJSON_Print(const cJSON *item) {
    char *printed = cJSON_Print(item);
    if (printed != NULL) {
        free(printed);
    }
}

static void test_cJSON_PrintUnformatted(const cJSON *item) {
    char *printed = cJSON_PrintUnformatted(item);
    if (printed != NULL) {
        free(printed);
    }
}

int LLVMFuzzerTestOneInput_52(const uint8_t *data, size_t size) {
    if (size == 0) return 0;
    
    // Try to parse the input as JSON
    cJSON *parsed_json = cJSON_ParseWithLength((const char *)data, size);
    
    if (parsed_json != NULL) {
        // Test all functions with the parsed JSON
        test_cJSON_GetStringValue(parsed_json);
        test_cJSON_PrintBuffered(parsed_json);
        test_cJSON_PrintPreallocated(parsed_json);
        test_cJSON_Print(parsed_json);
        test_cJSON_PrintUnformatted(parsed_json);
        
        // Clean up
        cJSON_Delete(parsed_json);
    }
    
    // Also test with manually created JSON objects
    cJSON *test_obj = cJSON_CreateObject();
    if (test_obj != NULL) {
        cJSON_AddStringToObject(test_obj, "test_key", "test_value");
        cJSON_AddNumberToObject(test_obj, "number_key", 42.5);
        cJSON_AddBoolToObject(test_obj, "bool_key", 1);
        
        test_cJSON_GetStringValue(test_obj);
        test_cJSON_PrintBuffered(test_obj);
        test_cJSON_PrintPreallocated(test_obj);
        test_cJSON_Print(test_obj);
        test_cJSON_PrintUnformatted(test_obj);
        
        cJSON_Delete(test_obj);
    }
    
    // Test cJSON_Minify with raw input data
    test_cJSON_Minify(data, size);
    
    return 0;
}