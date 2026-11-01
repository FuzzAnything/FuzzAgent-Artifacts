// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Version at cJSON.c:124:27 in cJSON.h
// cJSON_Print at cJSON.c:1307:22 in cJSON.h
// cJSON_PrintUnformatted at cJSON.c:1312:22 in cJSON.h
// cJSON_PrintBuffered at cJSON.c:1317:22 in cJSON.h
// cJSON_PrintBuffered at cJSON.c:1317:22 in cJSON.h
// cJSON_PrintPreallocated at cJSON.c:1348:26 in cJSON.h
// cJSON_PrintPreallocated at cJSON.c:1348:26 in cJSON.h
// cJSON_Minify at cJSON.c:2924:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cJSON.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static cJSON *create_fuzzed_cjson(const uint8_t *data, size_t size) {
    if (size == 0) {
        return NULL;
    }

    // Create a temporary buffer to hold the input string
    char *json_str = (char *)malloc(size + 1);
    if (!json_str) {
        return NULL;
    }
    memcpy(json_str, data, size);
    json_str[size] = '\0';

    cJSON *item = cJSON_Parse(json_str);
    free(json_str);
    return item;
}

static void cleanup_cjson(cJSON *item) {
    if (item) {
        cJSON_Delete(item);
    }
}

static void cleanup_string(char *str) {
    if (str) {
        free(str);
    }
}

int LLVMFuzzerTestOneInput_39(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Test cJSON_Version (no input dependency)
    const char *version = cJSON_Version();
    if (version) {
        // Just ensure it doesn't crash; no need to store
        (void)version;
    }

    // Create a cJSON object from input data
    cJSON *item = create_fuzzed_cjson(Data, Size);

    // Test cJSON_Print
    char *print_result = cJSON_Print(item);
    cleanup_string(print_result);

    // Test cJSON_PrintUnformatted
    char *print_unformatted = cJSON_PrintUnformatted(item);
    cleanup_string(print_unformatted);

    // Test cJSON_PrintBuffered
    int prebuffer = (Size > 0) ? (int)(Size * 2) : 100;
    char *print_buffered = cJSON_PrintBuffered(item, prebuffer, 0);
    cleanup_string(print_buffered);
    char *print_buffered_fmt = cJSON_PrintBuffered(item, prebuffer, 1);
    cleanup_string(print_buffered_fmt);

    // Test cJSON_PrintPreallocated
    char buffer[4096];
    cJSON_bool result_prealloc = cJSON_PrintPreallocated(item, buffer, sizeof(buffer), 0);
    (void)result_prealloc;
    cJSON_bool result_prealloc_fmt = cJSON_PrintPreallocated(item, buffer, sizeof(buffer), 1);
    (void)result_prealloc_fmt;

    // Test cJSON_Minify
    // Create a writable copy of the input for minification
    char *minify_buffer = (char *)malloc(Size + 1);
    if (minify_buffer) {
        memcpy(minify_buffer, Data, Size);
        minify_buffer[Size] = '\0';
        cJSON_Minify(minify_buffer);
        free(minify_buffer);
    }

    // Cleanup cJSON object
    cleanup_cjson(item);

    return 0;
}