// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_PrintUnformatted at cJSON.c:1312:22 in cJSON.h
// cJSON_PrintBuffered at cJSON.c:1317:22 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_Print at cJSON.c:1307:22 in cJSON.h
// cJSON_PrintUnformatted at cJSON.c:1312:22 in cJSON.h
// cJSON_PrintBuffered at cJSON.c:1317:22 in cJSON.h
// cJSON_PrintBuffered at cJSON.c:1317:22 in cJSON.h
// cJSON_PrintPreallocated at cJSON.c:1348:26 in cJSON.h
// cJSON_PrintPreallocated at cJSON.c:1348:26 in cJSON.h
// cJSON_PrintPreallocated at cJSON.c:1348:26 in cJSON.h
// cJSON_PrintPreallocated at cJSON.c:1348:26 in cJSON.h
// cJSON_PrintPreallocated at cJSON.c:1348:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_Print at cJSON.c:1307:22 in cJSON.h
// cJSON_PrintUnformatted at cJSON.c:1312:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Minify at cJSON.c:2924:20 in cJSON.h
// cJSON_Print at cJSON.c:1307:22 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cJSON.h>

static cJSON *fuzz_parse_json(const uint8_t *data, size_t size) {
    if (size == 0) {
        return NULL;
    }
    // Ensure null termination
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

int LLVMFuzzerTestOneInput_51(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Test Case 1: Parse and Print
    cJSON *item = fuzz_parse_json(Data, Size);
    if (item) {
        char *print_out = cJSON_Print(item);
        if (print_out) {
            free(print_out);
        }

        char *print_unformatted = cJSON_PrintUnformatted(item);
        if (print_unformatted) {
            free(print_unformatted);
        }

        char *print_buffered = cJSON_PrintBuffered(item, 1024, 1);
        if (print_buffered) {
            free(print_buffered);
        }

        char *print_buffered_unformatted = cJSON_PrintBuffered(item, 1024, 0);
        if (print_buffered_unformatted) {
            free(print_buffered_unformatted);
        }

        // Test Preallocated
        char buffer[4096];
        cJSON_PrintPreallocated(item, buffer, sizeof(buffer), 1);
        cJSON_PrintPreallocated(item, buffer, sizeof(buffer), 0);
        // Edge cases for Preallocated
        cJSON_PrintPreallocated(item, NULL, sizeof(buffer), 1);
        cJSON_PrintPreallocated(item, buffer, 0, 1);
        cJSON_PrintPreallocated(item, buffer, -1, 1);

        cJSON_Delete(item);
    }

    // Test Case 2: Create String and Print
    // Use input data as string content, ensuring null termination
    char *str_buf = (char *)malloc(Size + 1);
    if (str_buf) {
        memcpy(str_buf, Data, Size);
        str_buf[Size] = '\0';

        cJSON *str_item = cJSON_CreateString(str_buf);
        if (str_item) {
            char *print_out = cJSON_Print(str_item);
            if (print_out) {
                free(print_out);
            }
            char *print_unformatted = cJSON_PrintUnformatted(str_item);
            if (print_unformatted) {
                free(print_unformatted);
            }
            cJSON_Delete(str_item);
        }
        free(str_buf);
    }

    // Test Case 3: Minify
    // Create a buffer with potential whitespace/comments
    char *minify_buf = (char *)malloc(Size + 100);
    if (minify_buf) {
        memcpy(minify_buf, Data, Size);
        minify_buf[Size] = '\0';
        // Ensure it's writable and not a string literal
        cJSON_Minify(minify_buf);
        free(minify_buf);
    }

    // Test Case 4: Null inputs for Print functions
    cJSON_Print(NULL);
    cJSON_PrintUnformatted(NULL);
    cJSON_PrintBuffered(NULL, 100, 1);

    return 0;
}