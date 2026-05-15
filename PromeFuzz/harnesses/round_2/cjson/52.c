// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_PrintPreallocated at cJSON.c:1348:26 in cJSON.h
// cJSON_PrintBuffered at cJSON.c:1317:22 in cJSON.h
// cJSON_GetStringValue at cJSON.c:99:22 in cJSON.h
// cJSON_Print at cJSON.c:1307:22 in cJSON.h
// cJSON_PrintUnformatted at cJSON.c:1312:22 in cJSON.h
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

static cJSON *create_test_json_from_input(const uint8_t *data, size_t size) {
    if (size == 0) {
        return cJSON_CreateNull();
    }
    
    if (size < 256) {
        char *str = (char *)malloc(size + 1);
        if (!str) return cJSON_CreateNull();
        memcpy(str, data, size);
        str[size] = '\0';
        cJSON *json = cJSON_Parse(str);
        free(str);
        if (json) return json;
    }
    
    cJSON *root = cJSON_CreateObject();
    if (!root) return NULL;
    
    cJSON_AddItemToObject(root, "data", cJSON_CreateString((const char *)data));
    cJSON_AddItemToObject(root, "size", cJSON_CreateNumber((double)size));
    
    if (size > 0) {
        cJSON *array = cJSON_CreateArray();
        for (size_t i = 0; i < (size < 10 ? size : 10); i++) {
            cJSON_AddItemToArray(array, cJSON_CreateNumber((double)data[i]));
        }
        cJSON_AddItemToObject(root, "values", array);
    }
    
    return root;
}

int LLVMFuzzerTestOneInput_52(const uint8_t *data, size_t size) {
    cJSON *json = create_test_json_from_input(data, size);
    if (!json) return 0;
    
    // Test cJSON_PrintPreallocated
    if (size > 0) {
        int buffer_size = (data[0] % 1024) + 64;
        char *buffer = (char *)malloc(buffer_size);
        if (buffer) {
            int format = size > 1 ? (data[1] % 2) : 0;
            cJSON_PrintPreallocated(json, buffer, buffer_size, format);
            free(buffer);
        }
    }
    
    // Test cJSON_PrintBuffered
    if (size > 0) {
        int prebuffer = (data[0] % 2048) + 1;
        int fmt = size > 1 ? (data[1] % 2) : 0;
        char *result = cJSON_PrintBuffered(json, prebuffer, fmt);
        if (result) free(result);
    }
    
    // Test cJSON_GetStringValue
    char *str_val = cJSON_GetStringValue(json);
    (void)str_val;
    
    // Test cJSON_Print
    char *printed = cJSON_Print(json);
    if (printed) free(printed);
    
    // Test cJSON_PrintUnformatted
    char *unformatted = cJSON_PrintUnformatted(json);
    if (unformatted) free(unformatted);
    
    // Test cJSON_Minify
    if (size > 0) {
        FILE *fp = fopen("./dummy_file", "wb");
        if (fp) {
            fwrite(data, 1, size, fp);
            fclose(fp);
            
            fp = fopen("./dummy_file", "rb");
            if (fp) {
                fseek(fp, 0, SEEK_END);
                long file_size = ftell(fp);
                fseek(fp, 0, SEEK_SET);
                
                if (file_size > 0) {
                    char *json_str = (char *)malloc(file_size + 1);
                    if (json_str) {
                        fread(json_str, 1, file_size, fp);
                        json_str[file_size] = '\0';
                        cJSON_Minify(json_str);
                        free(json_str);
                    }
                }
                fclose(fp);
            }
        }
    }
    
    cJSON_Delete(json);
    return 0;
}