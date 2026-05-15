// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_ParseWithLength at cJSON.c:1232:23 in cJSON.h
// cJSON_Print at cJSON.c:1307:22 in cJSON.h
// cJSON_free at cJSON.c:3202:20 in cJSON.h
// cJSON_PrintBuffered at cJSON.c:1317:22 in cJSON.h
// cJSON_free at cJSON.c:3202:20 in cJSON.h
// cJSON_PrintBuffered at cJSON.c:1317:22 in cJSON.h
// cJSON_free at cJSON.c:3202:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Minify at cJSON.c:2924:20 in cJSON.h
// cJSON_malloc at cJSON.c:3197:22 in cJSON.h
// cJSON_free at cJSON.c:3202:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size) {
    // 1. Test cJSON_ParseWithLength
    char *json_str = (char *)malloc(Size + 1);
    if (json_str == NULL) return 0;
    memcpy(json_str, Data, Size);
    json_str[Size] = '\0';
    
    cJSON *parsed = cJSON_ParseWithLength(json_str, Size);
    
    // 2. Test cJSON_Print and cJSON_PrintBuffered if parsing succeeded
    if (parsed != NULL) {
        char *printed = cJSON_Print(parsed);
        if (printed != NULL) {
            cJSON_free(printed);
        }
        
        char *buffered_unformatted = cJSON_PrintBuffered(parsed, 1024, 0);
        if (buffered_unformatted != NULL) {
            cJSON_free(buffered_unformatted);
        }
        
        char *buffered_formatted = cJSON_PrintBuffered(parsed, 1024, 1);
        if (buffered_formatted != NULL) {
            cJSON_free(buffered_formatted);
        }
        
        cJSON_Delete(parsed);
    }
    
    // 3. Test cJSON_Minify (requires writable buffer)
    char *writable_json = (char *)malloc(Size + 1);
    if (writable_json != NULL) {
        memcpy(writable_json, Data, Size);
        writable_json[Size] = '\0';
        cJSON_Minify(writable_json);
        free(writable_json);
    }
    
    // 4. Test cJSON_malloc with small size to avoid OOM
    if (Size > 0) {
        size_t alloc_size = Data[0] % 256; // Small size for fuzzing
        void *allocated = cJSON_malloc(alloc_size);
        if (allocated != NULL) {
            cJSON_free(allocated);
        }
    }
    
    free(json_str);
    return 0;
}