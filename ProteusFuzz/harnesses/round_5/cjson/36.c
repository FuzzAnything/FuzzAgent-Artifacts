// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateStringArray at cJSON.c:2741:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_CreateDoubleArray at cJSON.c:2701:23 in cJSON.h
// cJSON_CreateDoubleArray at cJSON.c:2701:23 in cJSON.h
// cJSON_CreateIntArray at cJSON.c:2621:23 in cJSON.h
// cJSON_CreateIntArray at cJSON.c:2621:23 in cJSON.h
// cJSON_CreateFloatArray at cJSON.c:2661:23 in cJSON.h
// cJSON_CreateFloatArray at cJSON.c:2661:23 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cJSON.h>

static void cleanup_cjson(cJSON *item) {
    if (item != NULL) {
        cJSON_Delete(item);
    }
}

int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    cJSON *array = NULL;
    cJSON *newitem = NULL;
    cJSON *result_array = NULL;
    int which;
    int count;
    const double *double_arr = NULL;
    const int *int_arr = NULL;
    const float *float_arr = NULL;
    const char *const *str_arr = NULL;

    // Parse input to determine operation and parameters
    uint8_t op = Data[0] % 6;

    // Extract count safely
    if (Size > 1) {
        count = (int)(Data[1] % 100);
    } else {
        count = 0;
    }

    // Extract index for insertion
    if (Size > 2) {
        which = (int)(Data[2] % 200) - 50; // Range -50 to 149
    } else {
        which = 0;
    }

    switch (op) {
        case 0: {
            // Test cJSON_CreateArray and cJSON_InsertItemInArray
            array = cJSON_CreateArray();
            if (array != NULL) {
                // Create a simple item to insert
                newitem = cJSON_CreateNumber(123.0);
                if (newitem != NULL) {
                    cJSON_InsertItemInArray(array, which, newitem);
                    // newitem ownership transferred, do not delete
                }
                cleanup_cjson(array);
            }
            break;
        }
        case 1: {
            // Test cJSON_CreateDoubleArray
            if (Size >= 2 + (size_t)count * sizeof(double)) {
                double_arr = (const double *)(Data + 2);
                result_array = cJSON_CreateDoubleArray(double_arr, count);
                cleanup_cjson(result_array);
            } else {
                // Fallback for small input
                double dummy[1] = {0.0};
                result_array = cJSON_CreateDoubleArray(dummy, 1);
                cleanup_cjson(result_array);
            }
            break;
        }
        case 2: {
            // Test cJSON_CreateIntArray
            if (Size >= 2 + (size_t)count * sizeof(int)) {
                int_arr = (const int *)(Data + 2);
                result_array = cJSON_CreateIntArray(int_arr, count);
                cleanup_cjson(result_array);
            } else {
                int dummy[1] = {0};
                result_array = cJSON_CreateIntArray(dummy, 1);
                cleanup_cjson(result_array);
            }
            break;
        }
        case 3: {
            // Test cJSON_CreateFloatArray
            if (Size >= 2 + (size_t)count * sizeof(float)) {
                float_arr = (const float *)(Data + 2);
                result_array = cJSON_CreateFloatArray(float_arr, count);
                cleanup_cjson(result_array);
            } else {
                float dummy[1] = {0.0f};
                result_array = cJSON_CreateFloatArray(dummy, 1);
                cleanup_cjson(result_array);
            }
            break;
        }
        case 4: {
            // Test cJSON_CreateStringArray
            // We need to construct an array of string pointers
            // For fuzzing, we must ensure strings are null-terminated and within bounds
            int safe_count = (count > 100) ? 100 : count;
            
            // Limit safe_count based on available data size to ensure we can create valid strings
            // Each string needs at least 1 byte for content + 1 for null terminator
            if (safe_count > 0 && Size > 2) {
                // We will create a temporary array of char* pointers
                // We need to ensure each string is null-terminated.
                // Since Data is raw bytes, we can't guarantee null terminators.
                // Strategy: Use substrings of Data but ensure we don't read past Size.
                // A safer approach for fuzzing strings without nulls is to copy them into a buffer
                // or limit the length. However, cJSON_CreateStringArray expects const char* strings.
                // We will create a buffer of pointers to substrings, but we must ensure the strings are valid C-strings.
                // To do this safely without allocating too much, we will just use empty strings or short valid strings
                // if we can't guarantee null termination in the raw data.
                
                // Alternative: Create a temporary array of strings that we know are safe.
                // Let's just use a fixed set of safe strings if the input data is not suitable.
                // Or, better: copy chunks of data into a buffer and null-terminate them.
                
                char **strs = (char **)malloc(safe_count * sizeof(char *));
                if (strs == NULL) {
                    break;
                }
                
                int strings_created = 0;
                size_t offset = 2;
                
                for (int i = 0; i < safe_count; i++) {
                    if (offset >= Size) {
                        // Not enough data, use empty string
                        strs[i] = strdup("");
                        if (strs[i] == NULL) {
                            // Cleanup previous
                            for (int j = 0; j < i; j++) free(strs[j]);
                            free(strs);
                            break;
                        }
                        strings_created++;
                        continue;
                    }
                    
                    // Find a null terminator or limit length to remaining size
                    size_t len = 0;
                    size_t remaining = Size - offset;
                    
                    // Scan for null terminator within remaining bytes
                    while (len < remaining && Data[offset + len] != '\0') {
                        len++;
                    }
                    
                    if (len == 0) {
                        // No null terminator found, or empty string.
                        // Use a single byte string to be safe, or empty.
                        strs[i] = strdup("");
                    } else {
                        // Copy the string and ensure null termination
                        strs[i] = (char *)malloc(len + 1);
                        if (strs[i] == NULL) {
                            for (int j = 0; j < i; j++) free(strs[j]);
                            free(strs);
                            break;
                        }
                        memcpy(strs[i], Data + offset, len);
                        strs[i][len] = '\0';
                    }
                    
                    if (strs[i] == NULL) {
                        for (int j = 0; j < i; j++) free(strs[j]);
                        free(strs);
                        break;
                    }
                    
                    strings_created++;
                    offset += len + 1; // Move past the string and null terminator
                }
                
                if (strings_created > 0) {
                    str_arr = (const char *const *)strs;
                    result_array = cJSON_CreateStringArray(str_arr, strings_created);
                    cleanup_cjson(result_array);
                }
                
                // Cleanup temporary strings
                for (int i = 0; i < strings_created; i++) {
                    free(strs[i]);
                }
                free(strs);
            }
            break;
        }
        case 5: {
            // Complex scenario: Create array, add items, then insert
            array = cJSON_CreateArray();
            if (array != NULL) {
                // Add a few items
                cJSON_AddItemToArray(array, cJSON_CreateNumber(1.0));
                cJSON_AddItemToArray(array, cJSON_CreateNumber(2.0));
                
                newitem = cJSON_CreateNumber(999.0);
                if (newitem != NULL) {
                    cJSON_InsertItemInArray(array, which, newitem);
                }
                cleanup_cjson(array);
            }
            break;
        }
        default:
            break;
    }

    return 0;
}