// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_ParseWithLengthOpts at cJSON.c:1147:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_ParseWithLengthOpts at cJSON.c:1147:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_ParseWithOpts at cJSON.c:1131:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_ParseWithOpts at cJSON.c:1131:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_ParseWithLength at cJSON.c:1232:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetErrorPtr at cJSON.c:94:28 in cJSON.h
// cJSON_Minify at cJSON.c:2924:20 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
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
#include <stdio.h>

static void test_parse_functions(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    // Create a null-terminated copy for functions that expect it
    char *null_terminated = malloc(size + 1);
    if (!null_terminated) return;
    memcpy(null_terminated, data, size);
    null_terminated[size] = '\0';
    
    // Test cJSON_ParseWithLengthOpts with various options
    const char *parse_end1 = NULL;
    cJSON *json1 = cJSON_ParseWithLengthOpts(null_terminated, size, &parse_end1, 0);
    if (json1) cJSON_Delete(json1);
    
    const char *parse_end2 = NULL;
    cJSON *json2 = cJSON_ParseWithLengthOpts(null_terminated, size, &parse_end2, 1);
    if (json2) cJSON_Delete(json2);
    
    // Test cJSON_ParseWithOpts with various options
    const char *parse_end3 = NULL;
    cJSON *json3 = cJSON_ParseWithOpts(null_terminated, &parse_end3, 0);
    if (json3) cJSON_Delete(json3);
    
    const char *parse_end4 = NULL;
    cJSON *json4 = cJSON_ParseWithOpts(null_terminated, &parse_end4, 1);
    if (json4) cJSON_Delete(json4);
    
    // Test cJSON_ParseWithLength
    cJSON *json5 = cJSON_ParseWithLength(null_terminated, size);
    if (json5) cJSON_Delete(json5);
    
    // Test cJSON_Parse
    cJSON *json6 = cJSON_Parse(null_terminated);
    if (json6) cJSON_Delete(json6);
    
    // Test cJSON_GetErrorPtr after potential parse failures
    const char *error_ptr = cJSON_GetErrorPtr();
    (void)error_ptr; // Use error_ptr to avoid unused variable warning
    
    // Test cJSON_Minify with a writable copy
    char *writable_copy = malloc(size + 1);
    if (writable_copy) {
        memcpy(writable_copy, data, size);
        writable_copy[size] = '\0';
        cJSON_Minify(writable_copy);
        free(writable_copy);
    }
    
    free(null_terminated);
}

static void test_with_file(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    FILE *file = fopen("./dummy_file", "wb");
    if (!file) return;
    
    fwrite(data, 1, size, file);
    fclose(file);
    
    // Read the file back and test parsing
    file = fopen("./dummy_file", "rb");
    if (!file) return;
    
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    if (file_size > 0) {
        char *file_data = malloc(file_size + 1);
        if (file_data) {
            fread(file_data, 1, file_size, file);
            file_data[file_size] = '\0';
            
            // Test parsing with file data
            cJSON *json = cJSON_Parse(file_data);
            if (json) cJSON_Delete(json);
            
            free(file_data);
        }
    }
    
    fclose(file);
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_50(const uint8_t *data, size_t size) {
    // Test all parse functions with the input data
    test_parse_functions(data, size);
    
    // Test with file-based approach
    test_with_file(data, size);
    
    return 0;
}