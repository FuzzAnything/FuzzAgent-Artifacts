// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_ReplaceItemInArray at cJSON.c:2417:26 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_CreateFloatArray at cJSON.c:2661:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateBool at cJSON.c:2494:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateBool at cJSON.c:2494:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_DeleteItemFromArray at cJSON.c:2304:20 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_ReplaceItemViaPointer at cJSON.c:2368:26 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static cJSON* create_test_array(const uint8_t *data, size_t size) {
    cJSON *array = cJSON_CreateArray();
    if (!array) return NULL;
    
    size_t max_items = size > 100 ? 100 : size;
    for (size_t i = 0; i < max_items; i++) {
        if (i % 4 == 0) {
            cJSON_AddItemToArray(array, cJSON_CreateNumber(data[i]));
        } else if (i % 4 == 1) {
            char str[2] = {data[i] % 128, 0};
            cJSON_AddItemToArray(array, cJSON_CreateString(str));
        } else if (i % 4 == 2) {
            cJSON_AddItemToArray(array, cJSON_CreateBool(data[i] % 2));
        } else {
            cJSON_AddItemToArray(array, cJSON_CreateNull());
        }
    }
    return array;
}

static cJSON* create_replacement_item(const uint8_t *data, size_t size, size_t offset) {
    if (offset >= size) return cJSON_CreateNull();
    
    switch (data[offset] % 5) {
        case 0: return cJSON_CreateNumber(data[offset]);
        case 1: {
            char str[32];
            size_t len = (size - offset) > 31 ? 31 : (size - offset);
            for (size_t i = 0; i < len; i++) {
                str[i] = data[offset + i] % 128;
                if (str[i] == 0) str[i] = 1;
            }
            str[len] = 0;
            return cJSON_CreateString(str);
        }
        case 2: return cJSON_CreateBool(data[offset] % 2);
        case 3: return cJSON_CreateArray();
        case 4: return cJSON_CreateObject();
        default: return cJSON_CreateNull();
    }
}

int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;
    
    cJSON *array = create_test_array(Data, Size);
    if (!array) return 0;
    
    int array_size = cJSON_GetArraySize(array);
    
    if (array_size > 0) {
        size_t data_index = 0;
        
        for (size_t i = 0; i < 10; i++) {
            if (data_index >= Size) break;
            int operation = Data[data_index++] % 6;
            
            if (data_index >= Size) break;
            int index = (Data[data_index++] % array_size);
            
            switch (operation) {
                case 0: {
                    cJSON_DeleteItemFromArray(array, index);
                    break;
                }
                case 1: {
                    cJSON *item = cJSON_GetArrayItem(array, index);
                    if (item) {
                        cJSON_GetArraySize(item);
                    }
                    break;
                }
                case 2: {
                    cJSON *item = cJSON_GetArrayItem(array, index);
                    if (item) {
                        cJSON *replacement = create_replacement_item(Data, Size, data_index);
                        if (replacement) {
                            cJSON_ReplaceItemViaPointer(array, item, replacement);
                            if (data_index + 4 < Size) {
                                data_index += 4;
                            } else {
                                data_index = Size - 1;
                            }
                        }
                    }
                    break;
                }
                case 3: {
                    cJSON *replacement = create_replacement_item(Data, Size, data_index);
                    if (replacement) {
                        cJSON_ReplaceItemInArray(array, index, replacement);
                        if (data_index + 4 < Size) {
                            data_index += 4;
                        } else {
                            data_index = Size - 1;
                        }
                    }
                    break;
                }
                case 4: {
                    cJSON_GetArraySize(array);
                    break;
                }
                case 5: {
                    if (Size - data_index >= 4) {
                        int float_count = Data[data_index++] % 10;
                        if (float_count > 0 && (Size - data_index) >= (size_t)(float_count * 4)) {
                            float *floats = (float*)malloc(float_count * sizeof(float));
                            if (floats) {
                                for (int j = 0; j < float_count; j++) {
                                    uint32_t val = 0;
                                    for (int k = 0; k < 4 && data_index < Size; k++) {
                                        val = (val << 8) | Data[data_index++];
                                    }
                                    floats[j] = *(float*)&val;
                                }
                                cJSON *float_array = cJSON_CreateFloatArray(floats, float_count);
                                if (float_array) {
                                    cJSON_AddItemToArray(array, float_array);
                                }
                                free(floats);
                            }
                        }
                    }
                    break;
                }
            }
            
            array_size = cJSON_GetArraySize(array);
            if (array_size <= 0) break;
        }
    }
    
    cJSON_Delete(array);
    
    return 0;
}