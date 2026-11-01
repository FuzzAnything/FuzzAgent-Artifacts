// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_Version at cJSON.c:124:27 in cJSON.h
// cJSON_Print at cJSON.c:1307:22 in cJSON.h
// cJSON_PrintUnformatted at cJSON.c:1312:22 in cJSON.h
// cJSON_PrintBuffered at cJSON.c:1317:22 in cJSON.h
// cJSON_PrintBuffered at cJSON.c:1317:22 in cJSON.h
// cJSON_PrintPreallocated at cJSON.c:1348:26 in cJSON.h
// cJSON_Print at cJSON.c:1307:22 in cJSON.h
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

static cJSON *create_test_object(const uint8_t *data, size_t size) {
    if (size == 0) {
        return NULL;
    }

    cJSON *root = cJSON_CreateObject();
    if (!root) {
        return NULL;
    }

    cJSON *arr = cJSON_CreateArray();
    if (!arr) {
        cJSON_Delete(root);
        return NULL;
    }

    for (size_t i = 0; i < size; i++) {
        int val = data[i];
        cJSON *item = cJSON_CreateNumber((double)val);
        if (!item) {
            cJSON_Delete(root);
            return NULL;
        }
        cJSON_AddItemToArray(arr, item);
    }

    cJSON_AddItemToObject(root, "data", arr);
    cJSON_AddItemToObject(root, "size", cJSON_CreateNumber((double)size));
    cJSON_AddItemToObject(root, "str", cJSON_CreateString("test"));

    return root;
}

int LLVMFuzzerTestOneInput_53(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    cJSON *item = create_test_object(Data, Size);
    if (!item) {
        return 0;
    }

    /* Test cJSON_Version */
    const char *ver = cJSON_Version();
    (void)ver; /* Use version to ensure it's called */

    /* Test cJSON_Print */
    char *print_res = cJSON_Print(item);
    if (print_res) {
        free(print_res);
    }

    /* Test cJSON_PrintUnformatted */
    char *print_unfmt = cJSON_PrintUnformatted(item);
    if (print_unfmt) {
        free(print_unfmt);
    }

    /* Test cJSON_PrintBuffered */
    int prebuffer = (Size > 0) ? (int)(Size * 2 + 100) : 100;
    char *print_buf = cJSON_PrintBuffered(item, prebuffer, 1);
    if (print_buf) {
        free(print_buf);
    }
    char *print_buf_unfmt = cJSON_PrintBuffered(item, prebuffer, 0);
    if (print_buf_unfmt) {
        free(print_buf_unfmt);
    }

    /* Test cJSON_PrintPreallocated */
    char buffer[4096];
    memset(buffer, 0, sizeof(buffer));
    cJSON_bool prealloc_res = cJSON_PrintPreallocated(item, buffer, sizeof(buffer), 1);
    (void)prealloc_res;

    /* Test cJSON_Minify */
    char minify_buf[4096];
    memset(minify_buf, 0, sizeof(minify_buf));
    if (print_res) {
        /* Re-allocate to ensure we have a valid string to minify */
        char *tmp = cJSON_Print(item);
        if (tmp && strlen(tmp) < sizeof(minify_buf) - 1) {
            strcpy(minify_buf, tmp);
            cJSON_Minify(minify_buf);
            free(tmp);
        }
    }

    /* Cleanup */
    cJSON_Delete(item);

    return 0;
}