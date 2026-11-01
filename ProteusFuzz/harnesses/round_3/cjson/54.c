// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_PrintUnformatted at cJSON.c:1312:22 in cJSON.h
// cJSON_PrintBuffered at cJSON.c:1317:22 in cJSON.h
// cJSON_PrintBuffered at cJSON.c:1317:22 in cJSON.h
// cJSON_PrintPreallocated at cJSON.c:1348:26 in cJSON.h
// cJSON_PrintPreallocated at cJSON.c:1348:26 in cJSON.h
// cJSON_Minify at cJSON.c:2924:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddStringToObject at cJSON.c:2210:22 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_AddBoolToObject at cJSON.c:2186:22 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_Version at cJSON.c:124:27 in cJSON.h
// cJSON_Print at cJSON.c:1307:22 in cJSON.h
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

/* Helper to create a simple cJSON object from fuzz data */
static cJSON *create_cjson_from_data(const uint8_t *data, size_t size) {
    cJSON *root = cJSON_CreateObject();
    if (!root) {
        return NULL;
    }

    if (size == 0) {
        return root;
    }

    /* Add a string item */
    char *str_val = (char *)malloc(size + 1);
    if (!str_val) {
        cJSON_Delete(root);
        return NULL;
    }
    memcpy(str_val, data, size);
    str_val[size] = '\0';
    cJSON_AddStringToObject(root, "fuzz_string", str_val);
    free(str_val);

    /* Add a number item if data allows */
    if (size >= 4) {
        int num_val = *(const int *)(data);
        cJSON_AddNumberToObject(root, "fuzz_number", (double)num_val);
    }

    /* Add a boolean item */
    cJSON_AddBoolToObject(root, "fuzz_bool", (size % 2) ? cJSON_True : cJSON_False);

    /* Add a nested object */
    cJSON *nested = cJSON_CreateObject();
    if (nested) {
        cJSON_AddItemToObject(root, "nested", nested);
        cJSON_AddNumberToObject(nested, "inner", (double)(size));
    }

    return root;
}

/* Helper to create a JSON string from fuzz data */
static char *create_json_string_from_data(const uint8_t *data, size_t size) {
    if (size == 0) {
        char *str = strdup("{}");
        return str;
    }

    char *json_str = (char *)malloc(size + 100);
    if (!json_str) {
        return NULL;
    }

    /* Create a simple JSON-like structure */
    int len = snprintf(json_str, size + 100, "{\"data\":\"%.*s\",\"num\":%d}", 
                       (int)size, data, (int)(size % 1000));
    if (len < 0 || len >= (int)(size + 100)) {
        free(json_str);
        return NULL;
    }

    return json_str;
}

int LLVMFuzzerTestOneInput_54(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Test cJSON_Version */
    const char *version = cJSON_Version();
    if (version) {
        /* Just read to ensure it doesn't crash */
        (void)version[0];
    }

    /* Create cJSON object from input data */
    cJSON *item = create_cjson_from_data(Data, Size);
    if (!item) {
        return 0;
    }

    /* Test cJSON_Print */
    char *print_result = cJSON_Print(item);
    if (print_result) {
        free(print_result);
    }

    /* Test cJSON_PrintUnformatted */
    char *print_unformatted = cJSON_PrintUnformatted(item);
    if (print_unformatted) {
        free(print_unformatted);
    }

    /* Test cJSON_PrintBuffered */
    int prebuffer_guess = (int)(Size * 2 + 100);
    char *print_buffered = cJSON_PrintBuffered(item, prebuffer_guess, 1);
    if (print_buffered) {
        free(print_buffered);
    }
    char *print_buffered_unformatted = cJSON_PrintBuffered(item, prebuffer_guess, 0);
    if (print_buffered_unformatted) {
        free(print_buffered_unformatted);
    }

    /* Test cJSON_PrintPreallocated */
    char *buffer = (char *)malloc(Size * 4 + 100);
    if (buffer) {
        int buffer_len = (int)(Size * 4 + 100);
        cJSON_PrintPreallocated(item, buffer, buffer_len, 1);
        cJSON_PrintPreallocated(item, buffer, buffer_len, 0);
        free(buffer);
    }

    /* Test cJSON_Minify */
    char *json_string = create_json_string_from_data(Data, Size);
    if (json_string) {
        cJSON_Minify(json_string);
        free(json_string);
    }

    /* Cleanup */
    cJSON_Delete(item);

    return 0;
}