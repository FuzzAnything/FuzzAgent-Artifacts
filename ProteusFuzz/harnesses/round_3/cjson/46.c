// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateStringArray at cJSON.c:2741:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_CreateFloatArray at cJSON.c:2661:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateIntArray at cJSON.c:2621:23 in cJSON.h
// cJSON_CreateDoubleArray at cJSON.c:2701:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cJSON.h>

static void cleanup_json(cJSON *json) {
    if (json) {
        cJSON_Delete(json);
    }
}

int LLVMFuzzerTestOneInput_46(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    unsigned int op = Data[0] % 7;

    if (op == 0) {
        // Fuzz cJSON_CreateFloatArray
        if (Size < 2) return 0;
        int count = Data[1];
        if (count <= 0) return 0;
        // Ensure we have enough bytes for the float array
        if ((size_t)count * sizeof(float) > Size - 2) {
            return 0;
        }
        const float *numbers = (const float *)(Data + 2);
        cJSON *arr = cJSON_CreateFloatArray(numbers, count);
        cleanup_json(arr);
    } else if (op == 1) {
        // Fuzz cJSON_CreateArray
        cJSON *arr = cJSON_CreateArray();
        cleanup_json(arr);
    } else if (op == 2) {
        // Fuzz cJSON_CreateIntArray
        if (Size < 2) return 0;
        int count = Data[1];
        if (count <= 0) return 0;
        // Ensure we have enough bytes for the int array
        if ((size_t)count * sizeof(int) > Size - 2) {
            return 0;
        }
        const int *numbers = (const int *)(Data + 2);
        cJSON *arr = cJSON_CreateIntArray(numbers, count);
        cleanup_json(arr);
    } else if (op == 3) {
        // Fuzz cJSON_CreateDoubleArray
        if (Size < 2) return 0;
        int count = Data[1];
        if (count <= 0) return 0;
        // Ensure we have enough bytes for the double array
        if ((size_t)count * sizeof(double) > Size - 2) {
            return 0;
        }
        const double *numbers = (const double *)(Data + 2);
        cJSON *arr = cJSON_CreateDoubleArray(numbers, count);
        cleanup_json(arr);
    } else if (op == 4) {
        // Fuzz cJSON_InsertItemInArray
        if (Size < 3) return 0;
        cJSON *arr = cJSON_CreateArray();
        if (!arr) return 0;

        int which = (int)Data[1];
        cJSON *newitem = cJSON_CreateNumber((int)Data[2]);
        if (!newitem) {
            cleanup_json(arr);
            return 0;
        }

        cJSON_InsertItemInArray(arr, which, newitem);
        cleanup_json(arr);
    } else if (op == 5) {
        // Fuzz cJSON_CreateStringArray
        if (Size < 2) return 0;
        int count = Data[1];
        if (count <= 0) return 0;

        // Limit count to avoid excessive allocation and ensure we have enough data
        // We need at least 'count' strings, each at least 1 byte + null terminator
        // Simplified: limit count to Size/2 to be safe
        if (count > (int)(Size / 2)) {
            count = (int)(Size / 2);
        }
        if (count <= 0) return 0;

        const char **strings = (const char **)malloc(count * sizeof(char *));
        if (!strings) return 0;

        size_t offset = 2;
        int valid_count = 0;

        for (int i = 0; i < count; i++) {
            // Check remaining data
            if (offset >= Size) break;

            // Find a null terminator or use remaining data
            size_t len = 0;
            while (offset + len < Size && Data[offset + len] != '\0') {
                len++;
            }
            
            // If no null terminator found, take the rest or a single byte
            if (len == 0) {
                len = 1;
            }
            
            // Ensure we don't read past the buffer
            if (offset + len > Size) {
                len = Size - offset;
            }
            if (len == 0) break;

            char *str = (char *)malloc(len + 1);
            if (!str) {
                // Cleanup previously allocated strings
                for (int j = 0; j < i; j++) {
                    free((void *)strings[j]);
                }
                free(strings);
                return 0;
            }
            memcpy(str, Data + offset, len);
            str[len] = '\0';
            strings[i] = str;
            offset += len + 1; // Skip the null terminator we found or the byte we used
            if (offset > Size) offset = Size; // Safety check
            valid_count++;
        }

        cJSON *arr = cJSON_CreateStringArray(strings, valid_count);
        cleanup_json(arr);

        for (int i = 0; i < valid_count; i++) {
            if (strings[i]) {
                free((void *)strings[i]);
            }
        }
        free(strings);
    } else if (op == 6) {
        // Mixed operations: Create array, add items, insert items
        cJSON *arr = cJSON_CreateArray();
        if (!arr) return 0;

        // Try to add some items based on input data
        for (size_t i = 1; i < Size; i += 2) {
            if (i + 1 >= Size) break;
            int val = (int)Data[i];
            cJSON *item = cJSON_CreateNumber(val);
            if (item) {
                cJSON_AddItemToArray(arr, item);
            }
        }

        // Try to insert an item at a random index
        if (Size > 0) {
            int which = (int)Data[0];
            cJSON *newitem = cJSON_CreateNumber(123);
            if (newitem) {
                cJSON_InsertItemInArray(arr, which, newitem);
            }
        }

        cleanup_json(arr);
    }

    return 0;
}