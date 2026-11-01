// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateDoubleArray at cJSON.c:2701:23 in cJSON.h
// cJSON_CreateStringArray at cJSON.c:2741:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddArrayToObject at cJSON.c:2246:22 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_PrintBuffered at cJSON.c:1317:22 in cJSON.h
// cJSON_DeleteItemFromArray at cJSON.c:2304:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddArrayToObject at cJSON.c:2246:22 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_PrintBuffered at cJSON.c:1317:22 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_DeleteItemFromArray at cJSON.c:2304:20 in cJSON.h
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

int LLVMFuzzerTestOneInput_33(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    cJSON *root = NULL;
    cJSON *array = NULL;
    cJSON *item = NULL;
    char *output = NULL;
    int mode = Data[0] % 10;

    // Ensure we don't double free. Only root owns the entire tree.
    // array and item are children of root or standalone.
    // If array is attached to root, deleting root deletes array.
    // If array is standalone, we delete it.
    
    switch (mode) {
        case 0: {
            // Test cJSON_CreateArray and cJSON_AddArrayToObject
            root = cJSON_CreateObject();
            if (root) {
                array = cJSON_AddArrayToObject(root, "test_array");
                if (array) {
                    // Try to add some items if data allows
                    if (Size > 1) {
                        item = cJSON_CreateNumber((double)Data[1]);
                        if (item) {
                            cJSON_AddItemToArray(array, item);
                        }
                    }
                }
                // array is owned by root, so we only delete root
                cleanup_cjson(root);
                root = NULL;
                array = NULL; // Mark as handled
            }
            break;
        }
        case 1: {
            // Test cJSON_CreateArray directly
            array = cJSON_CreateArray();
            if (array && Size > 0) {
                item = cJSON_CreateNumber((double)Data[0]);
                if (item) {
                    cJSON_AddItemToArray(array, item);
                }
            }
            // array is standalone
            cleanup_cjson(array);
            array = NULL;
            break;
        }
        case 2: {
            // Test cJSON_PrintBuffered
            root = cJSON_CreateObject();
            if (root) {
                int prebuffer = (Size > 0) ? (int)Data[0] * 10 : 100;
                if (prebuffer < 0) prebuffer = 100;
                cJSON_bool fmt = (Size > 1) ? (Data[1] % 2) : 0;
                output = cJSON_PrintBuffered(root, prebuffer, fmt);
                if (output) {
                    free(output);
                }
                cleanup_cjson(root);
                root = NULL;
            }
            break;
        }
        case 3: {
            // Test cJSON_DeleteItemFromArray
            array = cJSON_CreateArray();
            if (array) {
                // Add some items
                for (size_t i = 0; i < Size && i < 10; i++) {
                    item = cJSON_CreateNumber((double)Data[i]);
                    if (item) {
                        cJSON_AddItemToArray(array, item);
                    }
                }
                // Delete an item based on input
                int index = (Size > 1) ? (int)Data[1] % 10 : 0;
                cJSON_DeleteItemFromArray(array, index);
            }
            // array is standalone
            cleanup_cjson(array);
            array = NULL;
            break;
        }
        case 4: {
            // Test cJSON_CreateDoubleArray
            if (Size >= 2) {
                int count = Data[0] % 10;
                if (count > 0 && Size >= (size_t)(count + 1)) {
                    double *doubles = (double *)malloc(count * sizeof(double));
                    if (doubles) {
                        for (int i = 0; i < count; i++) {
                            doubles[i] = (double)Data[i + 1];
                        }
                        array = cJSON_CreateDoubleArray(doubles, count);
                        free(doubles);
                    }
                }
            }
            // array is standalone
            cleanup_cjson(array);
            array = NULL;
            break;
        }
        case 5: {
            // Test cJSON_CreateStringArray
            if (Size >= 2) {
                int count = Data[0] % 10;
                if (count > 0 && Size >= (size_t)(count + 1)) {
                    const char *strings[10];
                    char *str_bufs[10];
                    for (int i = 0; i < count; i++) {
                        // Create dummy strings
                        str_bufs[i] = (char *)malloc(16);
                        if (str_bufs[i]) {
                            snprintf(str_bufs[i], 16, "str%d", Data[i + 1]);
                            strings[i] = str_bufs[i];
                        } else {
                            strings[i] = NULL;
                        }
                    }
                    array = cJSON_CreateStringArray(strings, count);
                    for (int i = 0; i < count; i++) {
                        free(str_bufs[i]);
                    }
                }
            }
            // array is standalone
            cleanup_cjson(array);
            array = NULL;
            break;
        }
        case 6: {
            // Complex scenario: Create object, add array, print, delete
            root = cJSON_CreateObject();
            if (root) {
                array = cJSON_AddArrayToObject(root, "dynamic");
                if (array) {
                    if (Size > 0) {
                        item = cJSON_CreateNumber((double)Data[0]);
                        if (item) cJSON_AddItemToArray(array, item);
                    }
                    if (Size > 1) {
                        item = cJSON_CreateNumber((double)Data[1]);
                        if (item) cJSON_AddItemToArray(array, item);
                    }
                    
                    output = cJSON_PrintBuffered(root, Size * 2, 1);
                    if (output) {
                        free(output);
                    }
                    
                    if (Size > 2) {
                        int idx = Data[2] % 3;
                        cJSON_DeleteItemFromArray(array, idx);
                    }
                }
                // array is owned by root
                cleanup_cjson(root);
                root = NULL;
                array = NULL;
            }
            break;
        }
        default:
            break;
    }

    // Final cleanup for any stragglers (should be NULL if logic is correct)
    cleanup_cjson(root);
    cleanup_cjson(array);

    return 0;
}