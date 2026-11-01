// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_Version at cJSON.c:124:27 in cJSON.h
// cJSON_PrintUnformatted at cJSON.c:1312:22 in cJSON.h
// cJSON_Print at cJSON.c:1307:22 in cJSON.h
// cJSON_PrintBuffered at cJSON.c:1317:22 in cJSON.h
// cJSON_PrintBuffered at cJSON.c:1317:22 in cJSON.h
// cJSON_PrintPreallocated at cJSON.c:1348:26 in cJSON.h
// cJSON_PrintPreallocated at cJSON.c:1348:26 in cJSON.h
// cJSON_Print at cJSON.c:1307:22 in cJSON.h
// cJSON_Minify at cJSON.c:2924:20 in cJSON.h
// cJSON_Minify at cJSON.c:2924:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <cJSON.h>

static void cleanup_json(cJSON *item) {
    if (item) {
        cJSON_Delete(item);
    }
}

static char *create_fuzzed_json_string(const uint8_t *data, size_t size) {
    if (size == 0) return NULL;
    char *str = (char *)malloc(size + 1);
    if (!str) return NULL;
    memcpy(str, data, size);
    str[size] = '\0';
    return str;
}

static cJSON *parse_fuzzed_json(const uint8_t *data, size_t size) {
    char *str = create_fuzzed_json_string(data, size);
    if (!str) return NULL;
    
    cJSON *item = cJSON_Parse(str);
    free(str);
    return item;
}

int LLVMFuzzerTestOneInput_50(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Test 1: cJSON_Version
    const char *version = cJSON_Version();
    if (version) {
        // Just ensure it doesn't crash accessing the string
        size_t len = strlen(version);
        if (len > 0) {
            (void)version[0];
        }
    }

    // Test 2: Parse JSON and test print functions
    cJSON *item = parse_fuzzed_json(Data, Size);
    
    // Test cJSON_PrintUnformatted
    if (item) {
        char *unformatted = cJSON_PrintUnformatted(item);
        if (unformatted) {
            free(unformatted);
        }
        
        // Test cJSON_Print
        char *formatted = cJSON_Print(item);
        if (formatted) {
            free(formatted);
        }
        
        // Test cJSON_PrintBuffered
        char *buffered = cJSON_PrintBuffered(item, Size * 2, 0);
        if (buffered) {
            free(buffered);
        }
        buffered = cJSON_PrintBuffered(item, Size * 2, 1);
        if (buffered) {
            free(buffered);
        }
        
        // Test cJSON_PrintPreallocated
        char *prealloc_buf = (char *)malloc(Size * 4 + 100);
        if (prealloc_buf) {
            cJSON_bool success = cJSON_PrintPreallocated(item, prealloc_buf, Size * 4 + 100, 0);
            (void)success;
            success = cJSON_PrintPreallocated(item, prealloc_buf, Size * 4 + 100, 1);
            (void)success;
            free(prealloc_buf);
        }
        
        // Test cJSON_Minify
        // Create a writable copy of formatted JSON to minify
        char *minify_input = cJSON_Print(item);
        if (minify_input) {
            cJSON_Minify(minify_input);
            free(minify_input);
        }
        
        cleanup_json(item);
    }

    // Test 3: Direct minify on raw input (if it looks like JSON string)
    // We create a mutable copy to test minify
    char *mutable_input = create_fuzzed_json_string(Data, Size);
    if (mutable_input) {
        cJSON_Minify(mutable_input);
        free(mutable_input);
    }

    return 0;
}