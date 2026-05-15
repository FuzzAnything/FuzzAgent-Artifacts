// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Print at cJSON.c:1307:22 in cJSON.h
// cJSON_free at cJSON.c:3202:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_ParseWithLength at cJSON.c:1232:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateRaw at cJSON.c:2581:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_InitHooks at cJSON.c:209:20 in cJSON.h
// cJSON_malloc at cJSON.c:3197:22 in cJSON.h
// cJSON_free at cJSON.c:3202:20 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_Print at cJSON.c:1307:22 in cJSON.h
// cJSON_free at cJSON.c:3202:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_ParseWithLength at cJSON.c:1232:23 in cJSON.h
// cJSON_Print at cJSON.c:1307:22 in cJSON.h
// cJSON_free at cJSON.c:3202:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateRaw at cJSON.c:2581:23 in cJSON.h
// cJSON_Print at cJSON.c:1307:22 in cJSON.h
// cJSON_free at cJSON.c:3202:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddStringToObject at cJSON.c:2210:22 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
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

static void *dummy_malloc(size_t size) {
    return malloc(size);
}

int LLVMFuzzerTestOneInput_44(const uint8_t *Data, size_t Size) {
    // Initialize custom hooks
    cJSON_Hooks hooks = {dummy_malloc, dummy_free};
    cJSON_InitHooks(&hooks);
    
    // Test cJSON_malloc with various sizes
    if (Size > 0) {
        size_t alloc_size = Size % 1024; // Limit to reasonable size
        void *mem = cJSON_malloc(alloc_size);
        if (mem) {
            // Write some data to the allocated memory
            size_t copy_size = alloc_size < Size ? alloc_size : Size;
            memcpy(mem, Data, copy_size);
            cJSON_free(mem);
        }
    }
    
    // Test cJSON_Parse - needs null-terminated string
    if (Size > 0) {
        char *str = (char *)malloc(Size + 1);
        if (str) {
            memcpy(str, Data, Size);
            str[Size] = '\0';
            
            cJSON *json = cJSON_Parse(str);
            if (json) {
                // Test cJSON_Print on the parsed object
                char *printed = cJSON_Print(json);
                if (printed) {
                    cJSON_free(printed);
                }
                cJSON_Delete(json);
            }
            free(str);
        }
    }
    
    // Test cJSON_ParseWithLength
    if (Size > 0) {
        char *str = (char *)malloc(Size);
        if (str) {
            memcpy(str, Data, Size);
            
            cJSON *json = cJSON_ParseWithLength(str, Size);
            if (json) {
                // Test cJSON_Print on the parsed object
                char *printed = cJSON_Print(json);
                if (printed) {
                    cJSON_free(printed);
                }
                cJSON_Delete(json);
            }
            free(str);
        }
    }
    
    // Test cJSON_CreateRaw
    if (Size > 0) {
        char *raw_str = (char *)malloc(Size + 1);
        if (raw_str) {
            memcpy(raw_str, Data, Size);
            raw_str[Size] = '\0';
            
            cJSON *raw_json = cJSON_CreateRaw(raw_str);
            if (raw_json) {
                // Test cJSON_Print on the raw JSON
                char *printed = cJSON_Print(raw_json);
                if (printed) {
                    cJSON_free(printed);
                }
                cJSON_Delete(raw_json);
            }
            free(raw_str);
        }
    }
    
    // Additional test: create a simple JSON object and print it
    cJSON *test_obj = cJSON_CreateObject();
    if (test_obj) {
        cJSON_AddStringToObject(test_obj, "key", "value");
        cJSON_AddNumberToObject(test_obj, "number", 42);
        
        char *test_printed = cJSON_Print(test_obj);
        if (test_printed) {
            cJSON_free(test_printed);
        }
        cJSON_Delete(test_obj);
    }
    
    // Test edge cases with zero size
    if (Size == 0) {
        cJSON *empty_parse = cJSON_Parse("");
        if (empty_parse) {
            cJSON_Delete(empty_parse);
        }
        
        cJSON *empty_parse_len = cJSON_ParseWithLength("", 0);
        if (empty_parse_len) {
            cJSON_Delete(empty_parse_len);
        }
        
        cJSON *empty_raw = cJSON_CreateRaw("");
        if (empty_raw) {
            cJSON_Delete(empty_raw);
        }
    }
    
    return 0;
}