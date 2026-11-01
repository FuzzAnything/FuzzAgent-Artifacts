// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Minify at cJSON.c:2924:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Print at cJSON.c:1307:22 in cJSON.h
// cJSON_PrintUnformatted at cJSON.c:1312:22 in cJSON.h
// cJSON_PrintBuffered at cJSON.c:1317:22 in cJSON.h
// cJSON_PrintBuffered at cJSON.c:1317:22 in cJSON.h
// cJSON_PrintPreallocated at cJSON.c:1348:26 in cJSON.h
// cJSON_PrintPreallocated at cJSON.c:1348:26 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cJSON.h>

static cJSON *fuzz_parse(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return cJSON_CreateString("");
    }
    
    // Ensure null termination for parsing
    char *json_str = (char *)malloc(Size + 1);
    if (!json_str) {
        return cJSON_CreateString("");
    }
    memcpy(json_str, Data, Size);
    json_str[Size] = '\0';

    cJSON *item = cJSON_Parse(json_str);
    free(json_str);
    
    if (item) {
        return item;
    }
    
    // Fallback: create a string from raw input if parsing fails
    // Ensure the input string is null-terminated before passing to cJSON_CreateString
    char *safe_str = (char *)malloc(Size + 1);
    if (!safe_str) {
        return NULL;
    }
    memcpy(safe_str, Data, Size);
    safe_str[Size] = '\0';
    
    cJSON *str_item = cJSON_CreateString(safe_str);
    free(safe_str);
    return str_item;
}

int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // 1. Test cJSON_CreateString
    // Ensure the input is null-terminated to avoid heap-buffer-overflow
    char *safe_str = (char *)malloc(Size + 1);
    if (!safe_str) {
        return 0;
    }
    memcpy(safe_str, Data, Size);
    safe_str[Size] = '\0';

    cJSON *item1 = cJSON_CreateString(safe_str);
    free(safe_str);
    if (item1) {
        cJSON_Delete(item1);
    }

    // 2. Parse input to get a cJSON object
    cJSON *item = fuzz_parse(Data, Size);
    if (!item) {
        return 0;
    }

    // 3. Test cJSON_Print
    char *print_out = cJSON_Print(item);
    if (print_out) {
        free(print_out);
    }

    // 4. Test cJSON_PrintUnformatted
    char *print_unfmt = cJSON_PrintUnformatted(item);
    if (print_unfmt) {
        free(print_unfmt);
    }

    // 5. Test cJSON_PrintBuffered
    char *print_buf = cJSON_PrintBuffered(item, Size * 2, 0);
    if (print_buf) {
        free(print_buf);
    }
    char *print_buf_fmt = cJSON_PrintBuffered(item, Size * 2, 1);
    if (print_buf_fmt) {
        free(print_buf_fmt);
    }

    // 6. Test cJSON_PrintPreallocated
    char *buffer = (char *)malloc(Size * 2 + 10);
    if (buffer) {
        cJSON_PrintPreallocated(item, buffer, Size * 2 + 10, 0);
        cJSON_PrintPreallocated(item, buffer, Size * 2 + 10, 1);
        free(buffer);
    }

    // 7. Test cJSON_Minify
    // Create a writable copy of a JSON string
    char *minify_input = (char *)malloc(Size + 1);
    if (minify_input) {
        memcpy(minify_input, Data, Size);
        minify_input[Size] = '\0';
        cJSON_Minify(minify_input);
        free(minify_input);
    }

    // Cleanup
    cJSON_Delete(item);

    return 0;
}