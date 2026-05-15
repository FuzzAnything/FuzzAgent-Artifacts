// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_free at cJSON.c:3202:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_ParseWithLength at cJSON.c:1232:23 in cJSON.h
// cJSON_Print at cJSON.c:1307:22 in cJSON.h
// cJSON_PrintBuffered at cJSON.c:1317:22 in cJSON.h
// cJSON_malloc at cJSON.c:3197:22 in cJSON.h
// cJSON_Minify at cJSON.c:2924:20 in cJSON.h
// cJSON_free at cJSON.c:3202:20 in cJSON.h
// cJSON_malloc at cJSON.c:3197:22 in cJSON.h
// cJSON_free at cJSON.c:3202:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "cJSON.h"

static void cleanup_string(char *str) {
    if (str != NULL) {
        cJSON_free(str);
    }
}

static void cleanup_json(cJSON *json) {
    if (json != NULL) {
        cJSON_Delete(json);
    }
}

int LLVMFuzzerTestOneInput_55(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Test cJSON_ParseWithLength
    cJSON *parsed_json = cJSON_ParseWithLength((const char*)Data, Size);
    
    // Test cJSON_Print if parsing succeeded
    if (parsed_json != NULL) {
        char *printed = cJSON_Print(parsed_json);
        cleanup_string(printed);
        
        // Test cJSON_PrintBuffered with different parameters
        // Use first byte for format flag, second and third bytes for prebuffer
        int fmt = (Size > 0) ? (Data[0] % 2) : 0;
        int prebuffer = (Size > 2) ? ((Data[1] << 8) | Data[2]) : 64;
        
        char *buffered = cJSON_PrintBuffered(parsed_json, prebuffer, fmt);
        cleanup_string(buffered);
    }
    
    // Test cJSON_Minify - need writable buffer
    char *writable_json = (char*)cJSON_malloc(Size + 1);
    if (writable_json != NULL) {
        memcpy(writable_json, Data, Size);
        writable_json[Size] = '\0';
        cJSON_Minify(writable_json);
        cJSON_free(writable_json);
    }
    
    // Test cJSON_malloc and cJSON_free with various sizes
    size_t alloc_size = Size % 1024 + 1;  // Avoid zero allocation
    void *allocated = cJSON_malloc(alloc_size);
    if (allocated != NULL) {
        // Try to use the allocated memory
        memset(allocated, 0xAA, alloc_size);
        cJSON_free(allocated);
    }
    
    // Cleanup parsed JSON
    cleanup_json(parsed_json);
    
    return 0;
}