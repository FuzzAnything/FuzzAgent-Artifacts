// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetStringValue at cJSON.c:99:22 in cJSON.h
// cJSON_PrintBuffered at cJSON.c:1317:22 in cJSON.h
// cJSON_free at cJSON.c:3202:20 in cJSON.h
// cJSON_PrintPreallocated at cJSON.c:1348:26 in cJSON.h
// cJSON_Minify at cJSON.c:2924:20 in cJSON.h
// cJSON_Print at cJSON.c:1307:22 in cJSON.h
// cJSON_free at cJSON.c:3202:20 in cJSON.h
// cJSON_PrintUnformatted at cJSON.c:1312:22 in cJSON.h
// cJSON_free at cJSON.c:3202:20 in cJSON.h
// cJSON_ParseWithLength at cJSON.c:1232:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
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
    char *str = cJSON_GetStringValue(item);
    (void)str; // Use the result to avoid unused variable warning
}

static void test_cJSON_PrintBuffered(const cJSON *item) {
    // Use fuzzer input to determine prebuffer and fmt values
    static int prebuffer_values[] = {0, 1, 10, 100, 1024, 4096};
    static cJSON_bool fmt_values[] = {0, 1};
    
    for (size_t i = 0; i < sizeof(prebuffer_values) / sizeof(prebuffer_values[0]); i++) {
        for (size_t j = 0; j < sizeof(fmt_values) / sizeof(fmt_values[0]); j++) {
            char *result = cJSON_PrintBuffered(item, prebuffer_values[i], fmt_values[j]);
            if (result != NULL) {
                cJSON_free(result);
            }
        }
    }
}

static void test_cJSON_PrintPreallocated(cJSON *item, const uint8_t *data, size_t size) {
    // Calculate buffer length based on input size, with safety margin
    int buffer_len = (size % 4096) + 5; // Ensure at least 5 bytes as per documentation
    if (buffer_len <= 0) buffer_len = 5;
    
    char *buffer = (char*)malloc(buffer_len);
    if (buffer == NULL) return;
    
    // Use fuzzer input to determine format (0 or 1)
    cJSON_bool format = (size % 2) ? 1 : 0;
    
    cJSON_bool success = cJSON_PrintPreallocated(item, buffer, buffer_len, format);
    (void)success; // Use the result to avoid unused variable warning
    
    free(buffer);
}

static void test_cJSON_Minify(const uint8_t *data, size_t size) {
    // Create a writable copy of the input for minification
    char *json = (char*)malloc(size + 1);
    if (json == NULL) return;
    
    memcpy(json, data, size);
    json[size] = '\0';
    
    cJSON_Minify(json);
    
    free(json);
}

static void test_cJSON_Print(const cJSON *item) {
    char *result = cJSON_Print(item);
    if (result != NULL) {
        cJSON_free(result);
    }
}

static void test_cJSON_PrintUnformatted(const cJSON *item) {
    char *result = cJSON_PrintUnformatted(item);
    if (result != NULL) {
        cJSON_free(result);
    }
}

static cJSON* create_test_item(const uint8_t *data, size_t size) {
    // Try to parse the input as JSON first
    cJSON *parsed = cJSON_ParseWithLength((const char*)data, size);
    if (parsed != NULL) {
        return parsed;
    }
    
    // If parsing fails, create a simple string item using the input
    // Limit string length to avoid excessive memory usage
    size_t str_len = size > 256 ? 256 : size;
    char *str = (char*)malloc(str_len + 1);
    if (str == NULL) return NULL;
    
    memcpy(str, data, str_len);
    str[str_len] = '\0';
    
    cJSON *string_item = cJSON_CreateString(str);
    free(str);
    
    return string_item;
}

int LLVMFuzzerTestOneInput_48(const uint8_t *data, size_t size) {
    if (size == 0) return 0;
    
    // Create a test cJSON item from the fuzzer input
    cJSON *test_item = create_test_item(data, size);
    if (test_item == NULL) return 0;
    
    // Test all target API functions
    test_cJSON_GetStringValue(test_item);
    test_cJSON_PrintBuffered(test_item);
    test_cJSON_PrintPreallocated(test_item, data, size);
    test_cJSON_Minify(data, size);
    test_cJSON_Print(test_item);
    test_cJSON_PrintUnformatted(test_item);
    
    // Clean up
    cJSON_Delete(test_item);
    
    return 0;
}