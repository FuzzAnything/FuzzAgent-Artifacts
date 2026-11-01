// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateFloatArray at cJSON.c:2661:23 in cJSON.h
// cJSON_CreateIntArray at cJSON.c:2621:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateDoubleArray at cJSON.c:2701:23 in cJSON.h
// cJSON_CreateStringArray at cJSON.c:2741:23 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cJSON.h>
#include <float.h>
#include <math.h>

static void cleanup_cjson(cJSON *item) {
    if (item) {
        cJSON_Delete(item);
    }
}

int LLVMFuzzerTestOneInput_37(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Use the first byte to select the fuzzing mode
    int mode = Data[0] % 6;
    size_t offset = 1;

    // Extract count if available
    int count = 0;
    if (Size - offset >= sizeof(int)) {
        count = *(const int *)(Data + offset);
        offset += sizeof(int);
        // Sanitize count to prevent huge allocations or negative indices
        if (count < 0) count = 0;
        if (count > 1000) count = 1000; 
    }

    // Extract index for insertion if available
    int index = 0;
    if (Size - offset >= sizeof(int)) {
        index = *(const int *)(Data + offset);
        offset += sizeof(int);
    }

    cJSON *root = NULL;
    cJSON *item_to_insert = NULL;

    switch (mode) {
        case 0: {
            // Fuzz cJSON_CreateDoubleArray
            if (Size - offset >= (size_t)count * sizeof(double)) {
                const double *numbers = (const double *)(Data + offset);
                root = cJSON_CreateDoubleArray(numbers, count);
            }
            break;
        }
        case 1: {
            // Fuzz cJSON_CreateStringArray
            // We need to construct an array of strings from the input buffer.
            // cJSON_CreateStringArray expects an array of const char pointers.
            // It will duplicate the strings internally, so we just need to ensure
            // the pointers we pass point to valid, null-terminated strings within the buffer.
            
            int str_count = (count > 0 && count < 10) ? count : 0;
            const char *strings[10] = {0};
            
            size_t current = offset;
            int parsed = 0;
            
            // Parse null-terminated strings from the buffer
            while (parsed < str_count && current < Size) {
                const char *start = (const char *)(Data + current);
                
                // Check for null terminator within the remaining buffer
                size_t len = 0;
                while (current + len < Size && Data[current + len] != '\0') {
                    len++;
                }
                
                // If we hit the end of buffer without a null terminator, stop
                if (current + len >= Size) {
                    break;
                }
                
                // We found a null terminator. 
                // Ensure the string is not empty (optional, but good for sanity)
                if (len == 0) {
                    // Empty string is valid C string, but let's skip if we want non-empty
                    // Or we can include it. cJSON handles empty strings.
                    // Move past the null terminator
                    current++;
                    continue;
                }

                strings[parsed] = start;
                parsed++;
                current += len + 1; // Move past the string and the null terminator
            }
            
            if (parsed > 0) {
                root = cJSON_CreateStringArray(strings, parsed);
            }
            break;
        }
        case 2: {
            // Fuzz cJSON_CreateFloatArray
            if (Size - offset >= (size_t)count * sizeof(float)) {
                const float *numbers = (const float *)(Data + offset);
                root = cJSON_CreateFloatArray(numbers, count);
            }
            break;
        }
        case 3: {
            // Fuzz cJSON_CreateIntArray
            if (Size - offset >= (size_t)count * sizeof(int)) {
                const int *numbers = (const int *)(Data + offset);
                root = cJSON_CreateIntArray(numbers, count);
            }
            break;
        }
        case 4: {
            // Fuzz cJSON_CreateArray
            root = cJSON_CreateArray();
            break;
        }
        case 5: {
            // Fuzz cJSON_InsertItemInArray
            root = cJSON_CreateArray();
            if (!root) break;

            // Create a new item to insert
            if (Size - offset >= sizeof(double)) {
                double val = *(const double *)(Data + offset);
                item_to_insert = cJSON_CreateNumber(val);
            } else {
                item_to_insert = cJSON_CreateNumber(0.0);
            }

            if (item_to_insert) {
                cJSON_bool res = cJSON_InsertItemInArray(root, index, item_to_insert);
                if (!res) {
                    // Insertion failed, we must delete new_item
                    cJSON_Delete(item_to_insert);
                }
                // If res is true, new_item is owned by root.
            }
            break;
        }
    }

    cleanup_cjson(root);
    // item_to_insert is either owned by root or deleted in case 5 logic

    return 0;
}