// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
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
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

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

int LLVMFuzzerTestOneInput_27(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Test cJSON_CreateString with input data as a string (ensure null termination)
    char *input_str = (char *)malloc(Size + 1);
    if (!input_str) {
        return 0;
    }
    memcpy(input_str, Data, Size);
    input_str[Size] = '\0';

    cJSON *item = cJSON_CreateString(input_str);
    free(input_str);

    if (!item) {
        return 0;
    }

    // Test cJSON_Print
    char *printed = cJSON_Print(item);
    cleanup_string(printed);

    // Test cJSON_PrintUnformatted
    char *unformatted = cJSON_PrintUnformatted(item);
    cleanup_string(unformatted);

    // Test cJSON_PrintBuffered
    char *buffered = cJSON_PrintBuffered(item, 1024, 0);
    cleanup_string(buffered);
    buffered = cJSON_PrintBuffered(item, 1024, 1);
    cleanup_string(buffered);

    // Test cJSON_PrintPreallocated
    char prealloc_buf[4096];
    cJSON_bool prealloc_result = cJSON_PrintPreallocated(item, prealloc_buf, sizeof(prealloc_buf), 0);
    (void)prealloc_result;
    prealloc_result = cJSON_PrintPreallocated(item, prealloc_buf, sizeof(prealloc_buf), 1);
    (void)prealloc_result;

    // Test cJSON_Minify on the preallocated buffer if it contains data
    if (prealloc_result && prealloc_buf[0] != '\0') {
        cJSON_Minify(prealloc_buf);
    }

    // Cleanup
    cleanup_cjson(item);

    return 0;
}