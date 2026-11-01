// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddStringToObject at cJSON.c:2210:22 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_AddBoolToObject at cJSON.c:2186:22 in cJSON.h
// cJSON_Print at cJSON.c:1307:22 in cJSON.h
// cJSON_PrintUnformatted at cJSON.c:1312:22 in cJSON.h
// cJSON_PrintBuffered at cJSON.c:1317:22 in cJSON.h
// cJSON_PrintBuffered at cJSON.c:1317:22 in cJSON.h
// cJSON_PrintPreallocated at cJSON.c:1348:26 in cJSON.h
// cJSON_Print at cJSON.c:1307:22 in cJSON.h
// cJSON_Minify at cJSON.c:2924:20 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
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

/* Helper to create a simple cJSON object from input data for testing */
static cJSON *create_test_object(const uint8_t *data, size_t size) {
    cJSON *root = cJSON_CreateObject();
    if (!root) {
        return NULL;
    }

    /* Add a string from input data */
    if (size > 0) {
        char *str_buf = (char *)malloc(size + 1);
        if (str_buf) {
            memcpy(str_buf, data, size);
            str_buf[size] = '\0';
            cJSON_AddStringToObject(root, "input_data", str_buf);
            free(str_buf);
        }
    }

    /* Add a number from first byte */
    if (size > 0) {
        cJSON_AddNumberToObject(root, "first_byte", (double)data[0]);
    }

    /* Add a boolean */
    cJSON_AddBoolToObject(root, "is_fuzz", cJSON_True);

    return root;
}

int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Create a test cJSON object */
    cJSON *item = create_test_object(Data, Size);
    if (!item) {
        return 0;
    }

    /* Test 1: cJSON_Print */
    char *printed = cJSON_Print(item);
    if (printed) {
        free(printed);
    }

    /* Test 2: cJSON_PrintUnformatted */
    char *unformatted = cJSON_PrintUnformatted(item);
    if (unformatted) {
        free(unformatted);
    }

    /* Test 3: cJSON_PrintBuffered */
    char *buffered_fmt = cJSON_PrintBuffered(item, 256, 1);
    if (buffered_fmt) {
        free(buffered_fmt);
    }
    char *buffered_unfmt = cJSON_PrintBuffered(item, 256, 0);
    if (buffered_unfmt) {
        free(buffered_unfmt);
    }

    /* Test 4: cJSON_PrintPreallocated */
    char buffer[4096];
    cJSON_bool prealloc_result = cJSON_PrintPreallocated(item, buffer, sizeof(buffer), 1);
    (void)prealloc_result; /* Suppress unused variable warning */

    /* Test 5: cJSON_Minify (on a copy of printed output) */
    char *minify_target = cJSON_Print(item);
    if (minify_target) {
        cJSON_Minify(minify_target);
        free(minify_target);
    }

    /* Test 6: cJSON_CreateString with input data */
    char *str_buf = (char *)malloc(Size + 1);
    if (str_buf) {
        memcpy(str_buf, Data, Size);
        str_buf[Size] = '\0';
        cJSON *str_item = cJSON_CreateString(str_buf);
        if (str_item) {
            cJSON_Delete(str_item);
        }
        free(str_buf);
    }

    /* Cleanup */
    cJSON_Delete(item);

    return 0;
}