// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_ParseWithLength at cJSON.c:1232:23 in cJSON.h
// cJSON_ParseWithLengthOpts at cJSON.c:1147:23 in cJSON.h
// cJSON_ParseWithLengthOpts at cJSON.c:1147:23 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_GetErrorPtr at cJSON.c:94:28 in cJSON.h
// cJSON_ParseWithOpts at cJSON.c:1131:23 in cJSON.h
// cJSON_ParseWithOpts at cJSON.c:1131:23 in cJSON.h
// cJSON_Minify at cJSON.c:2924:20 in cJSON.h
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

static void cleanup_json(cJSON *json) {
    if (json != NULL) {
        cJSON_Delete(json);
    }
}

static void test_parse_functions(const char *data, size_t size) {
    cJSON *parsed = NULL;
    const char *parse_end = NULL;
    
    // Test cJSON_ParseWithLength
    parsed = cJSON_ParseWithLength(data, size);
    cleanup_json(parsed);
    
    // Test cJSON_ParseWithLengthOpts with different options
    parsed = cJSON_ParseWithLengthOpts(data, size, &parse_end, 0);
    cleanup_json(parsed);
    
    parsed = cJSON_ParseWithLengthOpts(data, size, &parse_end, 1);
    cleanup_json(parsed);
    
    // Test cJSON_Parse (requires null-terminated string)
    if (size > 0) {
        char *null_terminated = malloc(size + 1);
        if (null_terminated != NULL) {
            memcpy(null_terminated, data, size);
            null_terminated[size] = '\0';
            
            parsed = cJSON_Parse(null_terminated);
            cleanup_json(parsed);
            
            // Test cJSON_GetErrorPtr after failed parse
            if (parsed == NULL) {
                const char *error_ptr = cJSON_GetErrorPtr();
                (void)error_ptr; // Avoid unused variable warning
            }
            
            free(null_terminated);
        }
    }
    
    // Test cJSON_ParseWithOpts with different options
    if (size > 0) {
        char *null_terminated = malloc(size + 1);
        if (null_terminated != NULL) {
            memcpy(null_terminated, data, size);
            null_terminated[size] = '\0';
            
            parsed = cJSON_ParseWithOpts(null_terminated, &parse_end, 0);
            cleanup_json(parsed);
            
            parsed = cJSON_ParseWithOpts(null_terminated, &parse_end, 1);
            cleanup_json(parsed);
            
            free(null_terminated);
        }
    }
}

static void test_minify(const char *data, size_t size) {
    if (size > 0) {
        // Create a writable copy for minification
        char *writable_json = malloc(size + 1);
        if (writable_json != NULL) {
            memcpy(writable_json, data, size);
            writable_json[size] = '\0';
            
            // Test cJSON_Minify
            cJSON_Minify(writable_json);
            
            free(writable_json);
        }
    }
}

int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    // Test all parse functions with the fuzzer input
    test_parse_functions((const char *)Data, Size);
    
    // Test minify function
    test_minify((const char *)Data, Size);
    
    // Also test with file input for completeness
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp != NULL) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
        
        // Read back and test
        fp = fopen("./dummy_file", "rb");
        if (fp != NULL) {
            char *file_data = malloc(Size + 1);
            if (file_data != NULL) {
                size_t read_size = fread(file_data, 1, Size, fp);
                if (read_size > 0) {
                    file_data[read_size] = '\0';
                    test_parse_functions(file_data, read_size);
                    test_minify(file_data, read_size);
                }
                free(file_data);
            }
            fclose(fp);
        }
    }
    
    return 0;
}