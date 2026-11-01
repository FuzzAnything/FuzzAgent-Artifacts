// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_PrintPreallocated at cJSON.c:1348:26 in cJSON.h
// cJSON_Print at cJSON.c:1307:22 in cJSON.h
// cJSON_Minify at cJSON.c:2924:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddStringToObject at cJSON.c:2210:22 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_Version at cJSON.c:124:27 in cJSON.h
// cJSON_Print at cJSON.c:1307:22 in cJSON.h
// cJSON_PrintUnformatted at cJSON.c:1312:22 in cJSON.h
// cJSON_PrintBuffered at cJSON.c:1317:22 in cJSON.h
// cJSON_PrintBuffered at cJSON.c:1317:22 in cJSON.h
// cJSON_PrintPreallocated at cJSON.c:1348:26 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cJSON.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Helper function to create a simple cJSON object from input data for testing */
static cJSON *create_test_object(const uint8_t *data, size_t size) {
    if (size == 0) {
        return cJSON_CreateNull();
    }

    cJSON *root = cJSON_CreateObject();
    if (!root) {
        return NULL;
    }

    /* Create a string item from input data */
    char *str_val = (char *)malloc(size + 1);
    if (!str_val) {
        cJSON_Delete(root);
        return NULL;
    }
    memcpy(str_val, data, size);
    str_val[size] = '\0';

    cJSON_AddStringToObject(root, "input", str_val);
    free(str_val);

    /* Add some number items based on input bytes */
    for (size_t i = 0; i < size && i < 10; i++) {
        char key[16];
        snprintf(key, sizeof(key), "num_%zu", i);
        cJSON_AddNumberToObject(root, key, (double)data[i]);
    }

    /* Add a nested array */
    cJSON *arr = cJSON_CreateArray();
    if (arr) {
        for (size_t i = 0; i < size && i < 5; i++) {
            cJSON_AddItemToArray(arr, cJSON_CreateNumber((double)data[i]));
        }
        cJSON_AddItemToObject(root, "array", arr);
    }

    return root;
}

int LLVMFuzzerTestOneInput_52(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Test cJSON_Version - no cleanup needed, returns static string */
    const char *version = cJSON_Version();
    (void)version; /* Use to avoid unused variable warning */

    /* Create a test cJSON object */
    cJSON *item = create_test_object(Data, Size);
    if (!item) {
        return 0;
    }

    /* Test cJSON_Print */
    char *printed = cJSON_Print(item);
    if (printed) {
        free(printed);
    }

    /* Test cJSON_PrintUnformatted */
    char *unformatted = cJSON_PrintUnformatted(item);
    if (unformatted) {
        free(unformatted);
    }

    /* Test cJSON_PrintBuffered */
    int prebuffer = (Size > 0) ? (int)Size * 2 : 100;
    if (prebuffer < 0) prebuffer = 100;
    char *buffered = cJSON_PrintBuffered(item, prebuffer, 1);
    if (buffered) {
        free(buffered);
    }

    /* Test cJSON_PrintBuffered with unformatted */
    char *buffered_unfmt = cJSON_PrintBuffered(item, prebuffer, 0);
    if (buffered_unfmt) {
        free(buffered_unfmt);
    }

    /* Test cJSON_PrintPreallocated */
    char *prealloc_buf = (char *)malloc(Size * 4 + 100);
    if (prealloc_buf) {
        int len = (int)(Size * 4 + 100);
        if (len < 0) len = 100;
        cJSON_PrintPreallocated(item, prealloc_buf, len, 1);
        cJSON_PrintPreallocated(item, prealloc_buf, len, 0);
        free(prealloc_buf);
    }

    /* Test cJSON_Minify - need a writable copy of JSON string */
    char *json_str = cJSON_Print(item);
    if (json_str) {
        char *minify_buf = strdup(json_str);
        if (minify_buf) {
            cJSON_Minify(minify_buf);
            free(minify_buf);
        }
        free(json_str);
    }

    /* Cleanup */
    cJSON_Delete(item);

    return 0;
}