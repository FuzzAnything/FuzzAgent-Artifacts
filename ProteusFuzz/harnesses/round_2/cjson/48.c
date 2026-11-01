// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_PrintPreallocated at cJSON.c:1348:26 in cJSON.h
// cJSON_PrintPreallocated at cJSON.c:1348:26 in cJSON.h
// cJSON_PrintPreallocated at cJSON.c:1348:26 in cJSON.h
// cJSON_PrintPreallocated at cJSON.c:1348:26 in cJSON.h
// cJSON_PrintPreallocated at cJSON.c:1348:26 in cJSON.h
// cJSON_PrintPreallocated at cJSON.c:1348:26 in cJSON.h
// cJSON_PrintPreallocated at cJSON.c:1348:26 in cJSON.h
// cJSON_Print at cJSON.c:1307:22 in cJSON.h
// cJSON_Minify at cJSON.c:2924:20 in cJSON.h
// cJSON_Minify at cJSON.c:2924:20 in cJSON.h
// cJSON_Minify at cJSON.c:2924:20 in cJSON.h
// cJSON_Version at cJSON.c:124:27 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddStringToObject at cJSON.c:2210:22 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_AddArrayToObject at cJSON.c:2246:22 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_Print at cJSON.c:1307:22 in cJSON.h
// cJSON_Print at cJSON.c:1307:22 in cJSON.h
// cJSON_PrintUnformatted at cJSON.c:1312:22 in cJSON.h
// cJSON_PrintUnformatted at cJSON.c:1312:22 in cJSON.h
// cJSON_PrintBuffered at cJSON.c:1317:22 in cJSON.h
// cJSON_PrintBuffered at cJSON.c:1317:22 in cJSON.h
// cJSON_PrintBuffered at cJSON.c:1317:22 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cJSON.h>

/* Helper function to create a cJSON object from raw input data.
 * This attempts to parse the input as JSON. If parsing fails, it returns NULL.
 * If parsing succeeds, the caller is responsible for freeing the returned object.
 */
static cJSON *create_cjson_from_input(const uint8_t *Data, size_t Size) {
    if (Data == NULL || Size == 0) {
        return NULL;
    }
    
    /* Allocate a buffer for the JSON string with a null terminator */
    char *json_str = (char *)malloc(Size + 1);
    if (json_str == NULL) {
        return NULL;
    }
    
    memcpy(json_str, Data, Size);
    json_str[Size] = '\0';
    
    cJSON *item = cJSON_Parse(json_str);
    free(json_str);
    
    return item;
}

/* Helper function to create a dummy cJSON object if parsing fails,
 * to ensure we still test the print functions with valid structures. */
static cJSON *create_dummy_cjson(void) {
    cJSON *root = cJSON_CreateObject();
    if (root) {
        cJSON_AddStringToObject(root, "key", "value");
        cJSON_AddNumberToObject(root, "num", 123.45);
        cJSON *arr = cJSON_AddArrayToObject(root, "arr");
        if (arr) {
            cJSON_AddItemToArray(arr, cJSON_CreateString("item1"));
            cJSON_AddItemToArray(arr, cJSON_CreateNumber(42));
        }
    }
    return root;
}

int LLVMFuzzerTestOneInput_48(const uint8_t *Data, size_t Size) {
    if (Data == NULL || Size == 0) {
        return 0;
    }

    /* 1. Prepare the environment */
    cJSON *item = create_cjson_from_input(Data, Size);
    
    /* If parsing fails, create a dummy object to ensure coverage of print functions */
    if (item == NULL) {
        item = create_dummy_cjson();
    }

    char *formatted_str = NULL;
    char *unformatted_str = NULL;
    char *buffered_str = NULL;
    char *prealloc_buf = NULL;
    char *minify_buf = NULL;
    int prealloc_size = 0;
    cJSON_bool prealloc_result;

    /* 2. Invoke target functions diversely */

    /* Test cJSON_Print */
    if (item != NULL) {
        formatted_str = cJSON_Print(item);
    } else {
        /* Test with NULL input */
        formatted_str = cJSON_Print(NULL);
    }

    /* Test cJSON_PrintUnformatted */
    if (item != NULL) {
        unformatted_str = cJSON_PrintUnformatted(item);
    } else {
        unformatted_str = cJSON_PrintUnformatted(NULL);
    }

    /* Test cJSON_PrintBuffered */
    if (item != NULL) {
        /* Try with a small guess and a large guess */
        buffered_str = cJSON_PrintBuffered(item, 10, 1);
        if (buffered_str != NULL) {
            free(buffered_str);
            buffered_str = NULL;
        }
        buffered_str = cJSON_PrintBuffered(item, 10000, 0);
    } else {
        buffered_str = cJSON_PrintBuffered(NULL, 100, 1);
    }

    /* Test cJSON_PrintPreallocated */
    if (item != NULL) {
        /* Allocate a buffer */
        prealloc_size = 4096;
        prealloc_buf = (char *)malloc(prealloc_size);
        if (prealloc_buf != NULL) {
            prealloc_result = cJSON_PrintPreallocated(item, prealloc_buf, prealloc_size, 1);
            /* Test with format=0 */
            prealloc_result = cJSON_PrintPreallocated(item, prealloc_buf, prealloc_size, 0);
            /* Test with a very small buffer to trigger failure */
            prealloc_result = cJSON_PrintPreallocated(item, prealloc_buf, 5, 1);
        }
    } else {
        /* Test with NULL item */
        prealloc_size = 100;
        prealloc_buf = (char *)malloc(prealloc_size);
        if (prealloc_buf != NULL) {
            prealloc_result = cJSON_PrintPreallocated(NULL, prealloc_buf, prealloc_size, 1);
        }
        /* Test with NULL buffer */
        prealloc_result = cJSON_PrintPreallocated(item, NULL, 100, 1);
        /* Test with length 0 or negative */
        if (prealloc_buf != NULL) {
            prealloc_result = cJSON_PrintPreallocated(item, prealloc_buf, 0, 1);
            prealloc_result = cJSON_PrintPreallocated(item, prealloc_buf, -1, 1);
        }
    }

    /* Test cJSON_Minify */
    if (item != NULL) {
        /* We need a string to minify. Let's generate one first if we haven't already */
        if (formatted_str == NULL) {
            formatted_str = cJSON_Print(item);
        }
        
        if (formatted_str != NULL) {
            /* Create a copy to minify, as Minify modifies in-place */
            size_t len = strlen(formatted_str);
            minify_buf = (char *)malloc(len + 1);
            if (minify_buf != NULL) {
                strcpy(minify_buf, formatted_str);
                cJSON_Minify(minify_buf);
            }
        }
        
        /* Test Minify with a fixed string that has comments and whitespace */
        /* Allocate a buffer large enough for the test string */
        const char *test_str = "  { \"key\" : \"value\" , \"num\" : 123 }  ";
        size_t test_len = strlen(test_str);
        char *test_buf = (char *)malloc(test_len + 1);
        if (test_buf != NULL) {
            strcpy(test_buf, test_str);
            cJSON_Minify(test_buf);
            free(test_buf);
        }
    } else {
        /* Test Minify with NULL */
        cJSON_Minify(NULL);
    }

    /* Test cJSON_Version */
    const char *version = cJSON_Version();
    /* Just access it to ensure no crash */
    if (version != NULL) {
        (void)strlen(version);
    }

    /* 3. Cleanup */
    if (formatted_str != NULL) {
        free(formatted_str);
    }
    if (unformatted_str != NULL) {
        free(unformatted_str);
    }
    if (buffered_str != NULL) {
        free(buffered_str);
    }
    if (prealloc_buf != NULL) {
        free(prealloc_buf);
    }
    if (minify_buf != NULL) {
        free(minify_buf);
    }
    if (item != NULL) {
        cJSON_Delete(item);
    }

    return 0;
}