// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_ParseWithLength at cJSON.c:1232:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_ParseWithOpts at cJSON.c:1131:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_ParseWithOpts at cJSON.c:1131:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetErrorPtr at cJSON.c:94:28 in cJSON.h
// cJSON_Minify at cJSON.c:2924:20 in cJSON.h
// cJSON_Version at cJSON.c:124:27 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void test_parse_functions(const uint8_t *data, size_t size) {
    // Create null-terminated string for functions requiring it
    char *null_terminated = NULL;
    if (size > 0) {
        null_terminated = (char *)malloc(size + 1);
        if (null_terminated) {
            memcpy(null_terminated, data, size);
            null_terminated[size] = '\0';
        }
    } else {
        null_terminated = (char *)malloc(1);
        if (null_terminated) {
            null_terminated[0] = '\0';
        }
    }
    
    if (!null_terminated) {
        return;
    }
    
    // Test cJSON_Parse
    cJSON *parsed = cJSON_Parse(null_terminated);
    if (parsed) {
        cJSON_Delete(parsed);
    }
    
    // Test cJSON_ParseWithLength
    cJSON *parsed_with_length = cJSON_ParseWithLength(null_terminated, size);
    if (parsed_with_length) {
        cJSON_Delete(parsed_with_length);
    }
    
    // Test cJSON_ParseWithOpts with various options
    const char *error_ptr = NULL;
    
    // Test with require_null_terminated = false
    cJSON *parsed_opts1 = cJSON_ParseWithOpts(null_terminated, &error_ptr, 0);
    if (parsed_opts1) {
        cJSON_Delete(parsed_opts1);
    }
    
    // Test with require_null_terminated = true
    cJSON *parsed_opts2 = cJSON_ParseWithOpts(null_terminated, &error_ptr, 1);
    if (parsed_opts2) {
        cJSON_Delete(parsed_opts2);
    }
    
    // Test cJSON_GetErrorPtr after parse attempts
    const char *error_location = cJSON_GetErrorPtr();
    (void)error_location; // Suppress unused variable warning
    
    // Test cJSON_Minify - requires writable buffer
    char *writable_copy = (char *)malloc(size + 1);
    if (writable_copy) {
        if (size > 0) {
            memcpy(writable_copy, data, size);
        }
        writable_copy[size] = '\0';
        cJSON_Minify(writable_copy);
        free(writable_copy);
    }
    
    free(null_terminated);
}

static void test_version_function(void) {
    const char *version = cJSON_Version();
    (void)version; // Suppress unused variable warning
}

int LLVMFuzzerTestOneInput_52(const uint8_t *Data, size_t Size) {
    // Test all parsing functions with the fuzzer input
    test_parse_functions(Data, Size);
    
    // Test version function
    test_version_function();
    
    // Also test with file input for completeness
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        if (Size > 0) {
            fwrite(Data, 1, Size, fp);
        }
        fclose(fp);
        
        // Read back and parse
        fp = fopen("./dummy_file", "rb");
        if (fp) {
            fseek(fp, 0, SEEK_END);
            long file_size = ftell(fp);
            fseek(fp, 0, SEEK_SET);
            
            if (file_size > 0) {
                char *file_data = (char *)malloc(file_size + 1);
                if (file_data) {
                    fread(file_data, 1, file_size, fp);
                    file_data[file_size] = '\0';
                    
                    // Test parse functions with file data
                    cJSON *file_parsed = cJSON_Parse(file_data);
                    if (file_parsed) {
                        cJSON_Delete(file_parsed);
                    }
                    
                    free(file_data);
                }
            }
            fclose(fp);
        }
    }
    
    return 0;
}