// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_InitHooks at cJSON.c:209:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_InitHooks at cJSON.c:209:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void dummy_free(void *ptr) {
    free(ptr);
}

static void *dummy_malloc(size_t sz) {
    return malloc(sz);
}

int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    // Create a null-terminated string from fuzzer input for cJSON_CreateString
    size_t str_len = Size > 100 ? 100 : Size;
    char *str1 = (char *)malloc(str_len + 1);
    if (!str1) return 0;
    memcpy(str1, Data, str_len);
    str1[str_len] = '\0';
    
    // First cJSON_CreateString
    cJSON *string1 = cJSON_CreateString(str1);
    free(str1);
    
    // cJSON_CreateArray
    cJSON *array = cJSON_CreateArray();
    
    // Create three more strings with different parts of input
    size_t offset = str_len;
    size_t remaining = Size - offset;
    if (remaining > 0) {
        size_t str2_len = remaining > 50 ? 50 : remaining;
        char *str2 = (char *)malloc(str2_len + 1);
        if (str2) {
            memcpy(str2, Data + offset, str2_len);
            str2[str2_len] = '\0';
            cJSON *string2 = cJSON_CreateString(str2);
            free(str2);
            
            offset += str2_len;
            remaining = Size - offset;
            
            if (remaining > 0) {
                size_t str3_len = remaining > 30 ? 30 : remaining;
                char *str3 = (char *)malloc(str3_len + 1);
                if (str3) {
                    memcpy(str3, Data + offset, str3_len);
                    str3[str3_len] = '\0';
                    cJSON *string3 = cJSON_CreateString(str3);
                    free(str3);
                    
                    offset += str3_len;
                    remaining = Size - offset;
                    
                    if (remaining > 0) {
                        size_t str4_len = remaining > 20 ? 20 : remaining;
                        char *str4 = (char *)malloc(str4_len + 1);
                        if (str4) {
                            memcpy(str4, Data + offset, str4_len);
                            str4[str4_len] = '\0';
                            cJSON *string4 = cJSON_CreateString(str4);
                            free(str4);
                            
                            // cJSON_InitHooks
                            cJSON_Hooks hooks;
                            hooks.malloc_fn = dummy_malloc;
                            hooks.free_fn = dummy_free;
                            cJSON_InitHooks(&hooks);
                            
                            // cJSON_Delete (delete the first string)
                            if (string1) {
                                cJSON_Delete(string1);
                            }
                            
                            // cJSON_GetArraySize
                            if (array) {
                                int array_size = cJSON_GetArraySize(array);
                                (void)array_size; // Use the result to avoid unused variable warning
                            }
                            
                            // cJSON_AddItemToArray (add string2 and string3)
                            if (array && string2) {
                                cJSON_AddItemToArray(array, string2);
                            }
                            if (array && string3) {
                                cJSON_AddItemToArray(array, string3);
                            }
                            
                            // cJSON_AddItemToObject - need to create an object first
                            cJSON *object = cJSON_CreateObject();
                            if (object && string4) {
                                // Use a fixed key name for the object
                                cJSON_AddItemToObject(object, "fuzz_key", string4);
                            }
                            
                            // Cleanup
                            if (array) {
                                cJSON_Delete(array);
                            }
                            if (object) {
                                cJSON_Delete(object);
                            }
                            
                            // Reset hooks to default
                            cJSON_InitHooks(NULL);
                        }
                    } else {
                        // Cleanup if we couldn't create string4
                        if (string2) cJSON_Delete(string2);
                        if (string3) cJSON_Delete(string3);
                        if (array) cJSON_Delete(array);
                        if (string1) cJSON_Delete(string1);
                    }
                }
            } else {
                // Cleanup if we couldn't create string3
                if (string2) cJSON_Delete(string2);
                if (array) cJSON_Delete(array);
                if (string1) cJSON_Delete(string1);
            }
        }
    } else {
        // Cleanup if we couldn't create string2
        if (array) cJSON_Delete(array);
        if (string1) cJSON_Delete(string1);
    }
    
    return 0;
}