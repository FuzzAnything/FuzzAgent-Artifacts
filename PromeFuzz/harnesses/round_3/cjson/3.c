// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_ParseWithOpts at cJSON.c:1131:23 in cJSON.h
// cJSON_PrintBuffered at cJSON.c:1317:22 in cJSON.h
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

int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    // Create a null-terminated copy of the input data for parsing
    char *json_string = (char *)malloc(Size + 1);
    if (!json_string) {
        return 0;
    }
    memcpy(json_string, Data, Size);
    json_string[Size] = '\0';
    
    // Parse the JSON data
    const char *parse_end = NULL;
    cJSON *parsed = cJSON_ParseWithOpts(json_string, &parse_end, 0);
    
    if (parsed) {
        // Test cJSON_PrintBuffered with both formatted and unformatted options
        char *buffered_formatted = cJSON_PrintBuffered(parsed, 1024, 1);
        if (buffered_formatted) {
            free(buffered_formatted);
        }
        
        char *buffered_unformatted = cJSON_PrintBuffered(parsed, 1024, 0);
        if (buffered_unformatted) {
            free(buffered_unformatted);
        }
        
        // Test cJSON_Print
        char *printed = cJSON_Print(parsed);
        if (printed) {
            free(printed);
        }
        
        // Test cJSON_PrintUnformatted
        char *unformatted = cJSON_PrintUnformatted(parsed);
        if (unformatted) {
            free(unformatted);
        }
        
        // Test cJSON_Minify on a mutable copy of the original JSON string
        char *mutable_json = (char *)malloc(Size + 1);
        if (mutable_json) {
            memcpy(mutable_json, Data, Size);
            mutable_json[Size] = '\0';
            cJSON_Minify(mutable_json);
            free(mutable_json);
        }
        
        // Clean up
        cJSON_Delete(parsed);
    }
    
    free(json_string);
    return 0;
}