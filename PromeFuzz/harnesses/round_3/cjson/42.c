// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_Print at cJSON.c:1307:22 in cJSON.h
// cJSON_free at cJSON.c:3202:20 in cJSON.h
// cJSON_PrintBuffered at cJSON.c:1317:22 in cJSON.h
// cJSON_free at cJSON.c:3202:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_ParseWithLength at cJSON.c:1232:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_malloc at cJSON.c:3197:22 in cJSON.h
// cJSON_free at cJSON.c:3202:20 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "code/cJSON.h"

static void test_cjson_apis(const uint8_t *data, size_t size) {
    /* Create a null-terminated string from fuzzer input */
    char *json_string = malloc(size + 1);
    if (!json_string) return;
    memcpy(json_string, data, size);
    json_string[size] = '\0';
    
    /* Test cJSON_Parse */
    cJSON *parsed = cJSON_Parse(json_string);
    
    if (parsed) {
        /* Test cJSON_Print */
        char *printed = cJSON_Print(parsed);
        if (printed) {
            cJSON_free(printed);
        }
        
        /* Test cJSON_PrintBuffered with various parameters */
        /* Use first byte for fmt flag, second byte for prebuffer */
        int prebuffer = size > 1 ? data[1] : 64;
        cJSON_bool fmt = size > 0 ? (data[0] & 1) : 0;
        
        char *printed_buffered = cJSON_PrintBuffered(parsed, prebuffer, fmt);
        if (printed_buffered) {
            cJSON_free(printed_buffered);
        }
        
        /* Clean up parsed object */
        cJSON_Delete(parsed);
    }
    
    /* Test cJSON_ParseWithLength */
    cJSON *parsed_with_len = cJSON_ParseWithLength(json_string, size);
    if (parsed_with_len) {
        cJSON_Delete(parsed_with_len);
    }
    
    /* Test cJSON_malloc and cJSON_free */
    void *allocated_mem = cJSON_malloc(size > 0 ? size : 1);
    if (allocated_mem) {
        cJSON_free(allocated_mem);
    }
    
    free(json_string);
}

static void test_with_file(const uint8_t *data, size_t size) {
    /* Write fuzzer data to a file */
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return;
    
    fwrite(data, 1, size, fp);
    fclose(fp);
    
    /* Read the file back and parse it */
    fp = fopen("./dummy_file", "rb");
    if (!fp) return;
    
    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    
    if (file_size > 0) {
        char *file_content = malloc(file_size + 1);
        if (file_content) {
            fread(file_content, 1, file_size, fp);
            file_content[file_size] = '\0';
            
            cJSON *parsed = cJSON_Parse(file_content);
            if (parsed) {
                cJSON_Delete(parsed);
            }
            
            free(file_content);
        }
    }
    
    fclose(fp);
}

int LLVMFuzzerTestOneInput_42(const uint8_t *data, size_t size) {
    if (size == 0) {
        return 0;
    }
    
    /* Test with direct string parsing */
    test_cjson_apis(data, size);
    
    /* Test with file-based parsing */
    test_with_file(data, size);
    
    return 0;
}