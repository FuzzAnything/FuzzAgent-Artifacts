// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_ParseWithOpts at cJSON.c:1131:23 in cJSON.h
// cJSON_PrintBuffered at cJSON.c:1317:22 in cJSON.h
// cJSON_Print at cJSON.c:1307:22 in cJSON.h
// cJSON_PrintUnformatted at cJSON.c:1312:22 in cJSON.h
// cJSON_Minify at cJSON.c:2924:20 in cJSON.h
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

int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    // Create a null-terminated string for parsing
    char *json_string = (char *)malloc(Size + 1);
    if (!json_string) {
        return 0;
    }
    memcpy(json_string, Data, Size);
    json_string[Size] = '\0';
    
    // Parse the JSON with options
    const char *parse_end = NULL;
    cJSON *json = cJSON_ParseWithOpts(json_string, &parse_end, 0);
    
    if (json) {
        // Test cJSON_PrintBuffered with varying parameters
        int prebuffer = 1024;
        cJSON_bool fmt = 1;
        if (Size > 0) {
            // Use first byte to determine formatting
            fmt = Data[0] & 1;
            // Use second and third bytes for prebuffer if available
            if (Size > 1) {
                prebuffer = (int)Data[1];
                if (Size > 2) {
                    prebuffer = (prebuffer << 8) | (int)Data[2];
                }
            }
        }
        char *buffered = cJSON_PrintBuffered(json, prebuffer, fmt);
        if (buffered) {
            free(buffered);
        }
        
        // Test cJSON_Print
        char *printed = cJSON_Print(json);
        if (printed) {
            free(printed);
        }
        
        // Test cJSON_PrintUnformatted
        char *unformatted = cJSON_PrintUnformatted(json);
        if (unformatted) {
            free(unformatted);
        }
        
        // Test cJSON_Minify on a writable JSON string
        // Create a mutable copy of the original JSON string for minification
        char *mutable_json = (char *)malloc(Size + 1);
        if (mutable_json) {
            memcpy(mutable_json, Data, Size);
            mutable_json[Size] = '\0';
            cJSON_Minify(mutable_json);
            free(mutable_json);
        }
        
        // Clean up
        cJSON_Delete(json);
    }
    
    free(json_string);
    return 0;
}