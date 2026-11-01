// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateIntArray at cJSON.c:2621:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_AddItemReferenceToArray at cJSON.c:2130:26 in cJSON.h
// cJSON_DeleteItemFromArray at cJSON.c:2304:20 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cJSON.h>

/* Helper function to create a random cJSON array with random items */
static cJSON *create_random_array(const uint8_t *data, size_t size) {
    cJSON *array = cJSON_CreateArray();
    if (!array) return NULL;

    size_t idx = 0;
    int count = 0;
    int max_items = 100; /* Limit to prevent excessive memory usage */

    while (idx < size && count < max_items) {
        int item_type = data[idx] % 3; /* 0: Number, 1: String, 2: Null */
        cJSON *item = NULL;

        if (item_type == 0) {
            if (idx + 1 >= size) break;
            int val = (int)data[idx + 1] - 128; /* Signed byte */
            item = cJSON_CreateNumber(val);
            idx += 2;
        } else if (item_type == 1) {
            if (idx + 1 >= size) break;
            size_t str_len = data[idx + 1] % 20; /* Limit string length */
            if (idx + 2 + str_len > size) break;
            char *str = (char *)malloc(str_len + 1);
            if (str) {
                memcpy(str, data + idx + 2, str_len);
                str[str_len] = '\0';
                item = cJSON_CreateString(str);
                free(str);
                idx += 2 + str_len;
            } else {
                idx += 2;
            }
        } else {
            item = cJSON_CreateNull();
            idx += 1;
        }

        if (item) {
            cJSON_AddItemToArray(array, item);
            count++;
        }
    }

    return array;
}

int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    /* Create a random array */
    cJSON *array = create_random_array(Data, Size);
    if (!array) return 0;

    /* Ensure it's an array (0x2000 is the bit for cJSON_Array) */
    if ((array->type & 0x2000) == 0) {
        cJSON_Delete(array);
        return 0;
    }

    /* Extract parameters from input data */
    int index = (int)Data[0] - 128; /* Signed index */
    int count = Data[1] % 10;       /* Small count for int array */

    /* 1. Test cJSON_GetArraySize */
    int size = cJSON_GetArraySize(array);
    (void)size; /* Use the result to suppress unused warnings */

    /* 2. Test cJSON_GetArrayItem */
    cJSON *item = cJSON_GetArrayItem(array, index);
    if (item) {
        /* Do nothing, just ensure no crash */
    }

    /* 3. Test cJSON_AddItemReferenceToArray */
    if (item) {
        cJSON_AddItemReferenceToArray(array, item);
    }

    /* 4. Test cJSON_DeleteItemFromArray */
    cJSON_DeleteItemFromArray(array, index);

    /* 5. Test cJSON_InsertItemInArray */
    cJSON *new_item = cJSON_CreateNumber(42);
    if (new_item) {
        cJSON_InsertItemInArray(array, index, new_item);
    }

    /* 6. Test cJSON_CreateIntArray */
    int numbers[10];
    int num_count = 0;
    if (Size >= 2 + count) {
        for (int i = 0; i < count; i++) {
            numbers[i] = (int)Data[2 + i] - 128;
            num_count++;
        }
    }
    cJSON *int_array = cJSON_CreateIntArray(numbers, num_count);
    if (int_array) {
        cJSON_Delete(int_array);
    }

    /* Cleanup */
    cJSON_Delete(array);

    return 0;
}