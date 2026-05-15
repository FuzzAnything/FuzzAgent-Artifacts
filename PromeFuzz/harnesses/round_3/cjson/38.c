// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateBool at cJSON.c:2494:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateIntArray at cJSON.c:2621:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_CreateIntArray at cJSON.c:2621:23 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static cJSON* create_test_item(const uint8_t* data, size_t size) {
    if (size == 0) {
        return cJSON_CreateNull();
    }
    
    switch (data[0] % 7) {
        case 0:
            return cJSON_CreateNumber((double)(data[0]));
        case 1:
            return cJSON_CreateBool(data[0] % 2);
        case 2:
            return cJSON_CreateString("test");
        case 3:
            return cJSON_CreateArray();
        case 4:
            return cJSON_CreateObject();
        case 5: {
            int num = (int)data[0];
            return cJSON_CreateIntArray(&num, 1);
        }
        default:
            return cJSON_CreateNull();
    }
}

int LLVMFuzzerTestOneInput_38(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    cJSON* array = cJSON_CreateArray();
    if (!array) {
        return 0;
    }

    size_t num_items = Data[0] % 10 + 1;
    for (size_t i = 0; i < num_items; i++) {
        cJSON* item = create_test_item(Data + (i % Size), Size - (i % Size));
        if (item) {
            cJSON_AddItemToArray(array, item);
        }
    }

    if (!cJSON_IsArray(array)) {
        cJSON_Delete(array);
        return 0;
    }

    int array_size = cJSON_GetArraySize(array);
    
    if (array_size > 0) {
        for (int i = 0; i < array_size; i++) {
            cJSON* item = cJSON_GetArrayItem(array, i);
            (void)item;
        }
    }

    if (Size >= 3) {
        cJSON* new_item = create_test_item(Data + 1, Size - 1);
        if (new_item) {
            int insert_index = (int)Data[1] % (array_size + 2);
            cJSON_InsertItemInArray(array, insert_index, new_item);
        }
    }

    if (Size >= 4) {
        int count = (int)Data[2] % 5 + 1;
        if (count > 0) {
            int* numbers = (int*)malloc(count * sizeof(int));
            if (numbers) {
                for (int i = 0; i < count; i++) {
                    numbers[i] = (int)Data[(i + 3) % Size];
                }
                cJSON* int_array = cJSON_CreateIntArray(numbers, count);
                if (int_array) {
                    int insert_idx = (int)Data[3] % (cJSON_GetArraySize(array) + 2);
                    cJSON_InsertItemInArray(array, insert_idx, int_array);
                }
                free(numbers);
            }
        }
    }

    cJSON_Delete(array);
    return 0;
}