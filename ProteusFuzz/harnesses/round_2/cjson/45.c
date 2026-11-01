// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_DeleteItemFromArray at cJSON.c:2304:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddArrayToObject at cJSON.c:2246:22 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_PrintBuffered at cJSON.c:1317:22 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_DeleteItemFromArray at cJSON.c:2304:20 in cJSON.h
// cJSON_PrintBuffered at cJSON.c:1317:22 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateStringArray at cJSON.c:2741:23 in cJSON.h
// cJSON_CreateDoubleArray at cJSON.c:2701:23 in cJSON.h
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

int LLVMFuzzerTestOneInput_45(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Use input data to drive fuzzing logic
    uint8_t mode = Data[0] % 10;

    switch (mode) {
        case 0: {
            // Fuzz cJSON_CreateStringArray
            // We need to construct a valid array of C strings (null-terminated)
            // to pass to cJSON_CreateStringArray.
            if (Size < 2) break;
            
            int count = (Data[1] % 5) + 1; // Limit count to avoid excessive allocation
            // Allocate an array of pointers
            const char **strings = (const char **)malloc(count * sizeof(const char *));
            if (!strings) break;

            // Initialize pointers to NULL for safe cleanup
            for (int i = 0; i < count; i++) strings[i] = NULL;

            // Prepare valid null-terminated strings from the input data
            // We have Size bytes total. We used 2 bytes for mode and count.
            // Remaining bytes: Size - 2.
            size_t remaining_bytes = (Size > 2) ? (Size - 2) : 0;
            size_t bytes_per_string = (remaining_bytes / count) + 1; // +1 to ensure at least 1 byte if possible
            
            // If we don't have enough bytes, we can't create meaningful strings, skip
            if (bytes_per_string < 1) {
                free(strings);
                break;
            }

            for (int i = 0; i < count; i++) {
                // Determine the actual length to copy, ensuring we don't read past buffer
                size_t offset = 2 + (i * bytes_per_string);
                size_t available = (Size > offset) ? (Size - offset) : 0;
                
                // We want to copy up to bytes_per_string - 1 chars to leave room for null terminator
                size_t copy_len = (available > bytes_per_string - 1) ? (bytes_per_string - 1) : available;
                
                // Create a temporary buffer for the string
                char *temp_str = (char *)malloc(copy_len + 1);
                if (!temp_str) {
                    // Cleanup on allocation failure
                    for (int j = 0; j < i; j++) free((void *)strings[j]);
                    free(strings);
                    break;
                }
                
                // Copy data
                if (copy_len > 0) {
                    memcpy(temp_str, Data + offset, copy_len);
                }
                temp_str[copy_len] = '\0';
                
                strings[i] = temp_str;
            }

            if (strings[0] != NULL) { // Ensure we have valid strings
                cJSON *arr = cJSON_CreateStringArray(strings, count);
                cleanup_cjson(arr);
            }

            // Cleanup our string buffers
            for (int i = 0; i < count; i++) {
                if (strings[i]) free((void *)strings[i]);
            }
            free(strings);
            break;
        }
        case 1: {
            // Fuzz cJSON_CreateDoubleArray
            if (Size < 2) break;
            int count = (Data[1] % 10) + 1;
            double numbers[10];
            for (int i = 0; i < count; i++) {
                if (i + 1 < Size) {
                    numbers[i] = (double)Data[i + 1];
                } else {
                    numbers[i] = 0.0;
                }
            }
            cJSON *arr = cJSON_CreateDoubleArray(numbers, count);
            cleanup_cjson(arr);
            break;
        }
        case 2: {
            // Fuzz cJSON_CreateArray and cJSON_DeleteItemFromArray
            cJSON *arr = cJSON_CreateArray();
            if (!arr) break;
            
            // Add some dummy items
            for (int i = 0; i < 5; i++) {
                cJSON_AddItemToArray(arr, cJSON_CreateNumber(i));
            }
            
            if (Size >= 2) {
                int which = Data[1] % 10; // Index to delete
                cJSON_DeleteItemFromArray(arr, which);
            }
            
            cleanup_cjson(arr);
            break;
        }
        case 3: {
            // Fuzz cJSON_AddArrayToObject
            cJSON *obj = cJSON_CreateObject();
            if (!obj) break;
            
            const char *key = "fuzz_key";
            cJSON *arr = cJSON_AddArrayToObject(obj, key);
            if (arr) {
                // Optionally add items to the array
                cJSON_AddItemToArray(arr, cJSON_CreateNumber(42));
            }
            cleanup_cjson(obj);
            break;
        }
        case 4: {
            // Fuzz cJSON_PrintBuffered
            cJSON *item = cJSON_CreateObject();
            if (!item) break;
            cJSON_AddNumberToObject(item, "test", 123);
            
            if (Size >= 2) {
                int prebuffer = Data[1] * 100;
                cJSON_bool fmt = (Size >= 3) ? (Data[2] % 2) : 0;
                char *printed = cJSON_PrintBuffered(item, prebuffer, fmt);
                if (printed) {
                    free(printed);
                }
            }
            cleanup_cjson(item);
            break;
        }
        case 5: {
            // Combined fuzzing: Create array, add items, delete, print
            cJSON *arr = cJSON_CreateArray();
            if (!arr) break;
            
            for (int i = 0; i < 5; i++) {
                cJSON_AddItemToArray(arr, cJSON_CreateString("test"));
            }
            
            if (Size >= 2) {
                int which = Data[1] % 6;
                cJSON_DeleteItemFromArray(arr, which);
            }
            
            if (Size >= 3) {
                int prebuffer = Data[2] * 50;
                cJSON_bool fmt = (Size >= 4) ? (Data[3] % 2) : 0;
                char *printed = cJSON_PrintBuffered(arr, prebuffer, fmt);
                if (printed) {
                    free(printed);
                }
            }
            cleanup_cjson(arr);
            break;
        }
        default:
            // Fallback: Create and delete a simple object
            cJSON *obj = cJSON_CreateObject();
            cleanup_cjson(obj);
            break;
    }

    return 0;
}