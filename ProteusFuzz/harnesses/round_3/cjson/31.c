// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_AddArrayToObject at cJSON.c:2246:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_DeleteItemFromArray at cJSON.c:2304:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_PrintBuffered at cJSON.c:1317:22 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_DeleteItemFromArray at cJSON.c:2304:20 in cJSON.h
// cJSON_CreateDoubleArray at cJSON.c:2701:23 in cJSON.h
// cJSON_DeleteItemFromArray at cJSON.c:2304:20 in cJSON.h
// cJSON_CreateStringArray at cJSON.c:2741:23 in cJSON.h
// cJSON_DeleteItemFromArray at cJSON.c:2304:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cJSON.h>

static void cleanup_json(cJSON *item) {
    if (item) {
        cJSON_Delete(item);
    }
}

int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Fuzz cJSON_CreateArray
    cJSON *arr = cJSON_CreateArray();
    if (arr) {
        // Fuzz cJSON_DeleteItemFromArray with various indices
        // Ensure we have at least 2 bytes before accessing Data[1]
        int idx;
        if (Size >= 2) {
            idx = (Data[0] % (Size > 0 ? (int)Size : 1)) - (Data[1] % 5);
        } else {
            idx = 0;
        }
        cJSON_DeleteItemFromArray(arr, idx);
        cleanup_json(arr);
    }

    // Fuzz cJSON_CreateDoubleArray
    if (Size >= 2) {
        double *doubles = (double *)malloc((Size / 2) * sizeof(double));
        if (doubles) {
            for (size_t i = 0; i < Size / 2; i++) {
                doubles[i] = (double)(Data[i * 2] ^ Data[i * 2 + 1]);
            }
            cJSON *darr = cJSON_CreateDoubleArray(doubles, (int)(Size / 2));
            if (darr) {
                // Try to delete an item
                int del_idx = Data[0] % (Size / 2);
                cJSON_DeleteItemFromArray(darr, del_idx);
                cleanup_json(darr);
            }
            free(doubles);
        }
    }

    // Fuzz cJSON_CreateStringArray
    if (Size >= 4) {
        const char **strings = (const char **)malloc((Size / 4) * sizeof(char *));
        if (strings) {
            for (size_t i = 0; i < Size / 4; i++) {
                size_t len = (Data[i * 4] % 10) + 1;
                // Ensure we don't read past the end of Data
                size_t available = Size - (i * 4 + 1);
                if (len > available) {
                    len = available;
                    if (len == 0) continue;
                }
                
                char *str = (char *)malloc(len + 1);
                if (str) {
                    memcpy(str, Data + i * 4 + 1, len);
                    str[len] = '\0';
                    strings[i] = str;
                } else {
                    strings[i] = NULL;
                }
            }
            cJSON *sarr = cJSON_CreateStringArray(strings, (int)(Size / 4));
            if (sarr) {
                int del_idx = Data[0] % (Size / 4);
                cJSON_DeleteItemFromArray(sarr, del_idx);
                cleanup_json(sarr);
            }
            for (size_t i = 0; i < Size / 4; i++) {
                if (strings[i]) {
                    free((void *)strings[i]);
                }
            }
            free(strings);
        }
    }

    // Fuzz cJSON_AddArrayToObject
    cJSON *obj = cJSON_CreateObject();
    if (obj) {
        char key[16];
        size_t key_len = (Data[0] % 15) + 1;
        if (Size >= key_len) {
            memcpy(key, Data, key_len);
            key[key_len] = '\0';
        } else {
            key[0] = 'k';
            key[1] = '\0';
        }

        cJSON *new_arr = cJSON_AddArrayToObject(obj, key);
        if (new_arr) {
            // Try to delete from the newly added array
            // Ensure Size >= 2 before accessing Data[1]
            int del_idx;
            if (Size >= 2) {
                del_idx = Data[1] % 10;
            } else {
                del_idx = 0;
            }
            cJSON_DeleteItemFromArray(new_arr, del_idx);
        }
        cleanup_json(obj);
    }

    // Fuzz cJSON_PrintBuffered
    cJSON *test_item = cJSON_CreateObject();
    if (test_item) {
        int prebuffer = (int)(Data[0] * Size) + 100;
        if (prebuffer < 0) prebuffer = 100;
        cJSON_bool fmt = (Size >= 2) ? (Data[1] % 2) : 0;
        char *printed = cJSON_PrintBuffered(test_item, prebuffer, fmt);
        if (printed) {
            free(printed);
        }
        cleanup_json(test_item);
    }

    return 0;
}