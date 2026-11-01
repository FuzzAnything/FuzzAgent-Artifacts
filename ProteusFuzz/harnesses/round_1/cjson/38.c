// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_Version at cJSON.c:124:27 in cJSON.h
// cJSON_Minify at cJSON.c:2924:20 in cJSON.h
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

static void cleanup_cjson(cJSON *item) {
    if (item) {
        cJSON_Delete(item);
    }
}

static char *create_writable_json_string(const uint8_t *data, size_t size) {
    if (size == 0) {
        return NULL;
    }
    char *str = (char *)malloc(size + 1);
    if (!str) {
        return NULL;
    }
    memcpy(str, data, size);
    str[size] = '\0';
    return str;
}

static cJSON *create_cjson_from_string(const char *json_str) {
    if (!json_str) {
        return NULL;
    }
    return cJSON_Parse(json_str);
}

int LLVMFuzzerTestOneInput_38(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Test cJSON_Version
    const char *version = cJSON_Version();
    if (version) {
        (void)strlen(version);
    }

    // Test cJSON_Minify
    char *minify_input = create_writable_json_string(Data, Size);
    if (minify_input) {
        cJSON_Minify(minify_input);
        free(minify_input);
    }

    // Test parsing and printing functions
    // FIX: Ensure the input string is null-terminated before passing to cJSON_Parse
    char *safe_input = create_writable_json_string(Data, Size);
    cJSON *item = NULL;
    if (safe_input) {
        item = create_cjson_from_string(safe_input);
        free(safe_input);
    }

    // Test cJSON_Print
    char *print_result = cJSON_Print(item);
    if (print_result) {
        free(print_result);
    }

    // Test cJSON_PrintUnformatted
    char *print_unformatted_result = cJSON_PrintUnformatted(item);
    if (print_unformatted_result) {
        free(print_unformatted_result);
    }

    // Test cJSON_PrintBuffered
    int prebuffer = (Size > 1000) ? 1000 : (int)Size;
    char *print_buffered_result = cJSON_PrintBuffered(item, prebuffer, 0);
    if (print_buffered_result) {
        free(print_buffered_result);
    }
    print_buffered_result = cJSON_PrintBuffered(item, prebuffer, 1);
    if (print_buffered_result) {
        free(print_buffered_result);
    }

    // Test cJSON_PrintPreallocated
    char buffer[4096];
    cJSON_bool print_preallocated_result = cJSON_PrintPreallocated(item, buffer, sizeof(buffer), 0);
    (void)print_preallocated_result;
    print_preallocated_result = cJSON_PrintPreallocated(item, buffer, sizeof(buffer), 1);
    (void)print_preallocated_result;

    // Cleanup
    cleanup_cjson(item);

    return 0;
}