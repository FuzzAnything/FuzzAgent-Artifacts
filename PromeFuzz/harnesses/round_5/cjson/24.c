// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Version at cJSON.c:124:27 in cJSON.h
// cJSON_ParseWithLength at cJSON.c:1232:23 in cJSON.h
// cJSON_GetErrorPtr at cJSON.c:94:28 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_ParseWithLength at cJSON.c:1232:23 in cJSON.h
// cJSON_ParseWithLengthOpts at cJSON.c:1147:23 in cJSON.h
// cJSON_ParseWithLengthOpts at cJSON.c:1147:23 in cJSON.h
// cJSON_ParseWithLengthOpts at cJSON.c:1147:23 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_ParseWithLength at cJSON.c:1232:23 in cJSON.h
// cJSON_ParseWithOpts at cJSON.c:1131:23 in cJSON.h
// cJSON_ParseWithLength at cJSON.c:1232:23 in cJSON.h
// cJSON_ParseWithOpts at cJSON.c:1131:23 in cJSON.h
// cJSON_ParseWithLength at cJSON.c:1232:23 in cJSON.h
// cJSON_Version at cJSON.c:124:27 in cJSON.h
// cJSON_ParseWithLength at cJSON.c:1232:23 in cJSON.h
// cJSON_GetErrorPtr at cJSON.c:94:28 in cJSON.h
// cJSON_Version at cJSON.c:124:27 in cJSON.h
// cJSON_ParseWithLength at cJSON.c:1232:23 in cJSON.h
// cJSON_GetErrorPtr at cJSON.c:94:28 in cJSON.h
// cJSON_ParseWithLengthOpts at cJSON.c:1147:23 in cJSON.h
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

static void cleanup_json(cJSON *json) {
    if (json != NULL) {
        cJSON_Delete(json);
    }
}

static void test_parse_functions(const char *data, size_t size, uint8_t selector, int is_null_terminated) {
    cJSON *parsed = NULL;
    const char *parse_end = NULL;
    
    // Use selector to choose different parameter combinations
    switch (selector % 8) {
        case 0:
            parsed = cJSON_ParseWithLength(data, size);
            break;
        case 1:
            parsed = cJSON_ParseWithLengthOpts(data, size, &parse_end, 0);
            break;
        case 2:
            if (is_null_terminated) {
                parsed = cJSON_ParseWithLengthOpts(data, size, &parse_end, 1);
            } else {
                parsed = cJSON_ParseWithLengthOpts(data, size, &parse_end, 0);
            }
            break;
        case 3:
            if (is_null_terminated) {
                parsed = cJSON_Parse(data);
            } else {
                parsed = cJSON_ParseWithLength(data, size);
            }
            break;
        case 4:
            if (is_null_terminated) {
                parsed = cJSON_ParseWithOpts(data, &parse_end, 0);
            } else {
                parsed = cJSON_ParseWithLength(data, size);
            }
            break;
        case 5:
            if (is_null_terminated) {
                parsed = cJSON_ParseWithOpts(data, &parse_end, 1);
            } else {
                parsed = cJSON_ParseWithLength(data, size);
            }
            break;
        case 6:
            // Test version function
            cJSON_Version();
            // Also test error pointer after a parse attempt
            parsed = cJSON_ParseWithLength(data, size);
            if (parsed == NULL) {
                cJSON_GetErrorPtr();
            }
            break;
        case 7:
            // Test multiple functions in sequence
            cJSON_Version();
            parsed = cJSON_ParseWithLength(data, size);
            if (parsed == NULL) {
                cJSON_GetErrorPtr();
            }
            cleanup_json(parsed);
            parsed = cJSON_ParseWithLengthOpts(data, size, NULL, is_null_terminated ? (selector & 1) : 0);
            break;
    }
    
    cleanup_json(parsed);
}

int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    // Create a null-terminated copy of the data for functions that expect it
    char *null_terminated_data = (char *)malloc(Size + 1);
    if (null_terminated_data == NULL) {
        return 0;
    }
    
    memcpy(null_terminated_data, Data, Size);
    null_terminated_data[Size] = '\0';
    
    // Use first byte as selector for different code paths
    uint8_t selector = Data[0];
    
    // Test with null-terminated data
    test_parse_functions(null_terminated_data, Size, selector, 1);
    
    // Test version function independently
    cJSON_Version();
    
    // Test error pointer in various scenarios
    cJSON *temp = cJSON_ParseWithLength(null_terminated_data, Size);
    if (temp == NULL) {
        cJSON_GetErrorPtr();
    }
    cleanup_json(temp);
    
    // Test with file if needed (though not required for these APIs)
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp != NULL) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
        
        // Read back and test
        fp = fopen("./dummy_file", "rb");
        if (fp != NULL) {
            char *file_data = (char *)malloc(Size + 1);
            if (file_data != NULL) {
                size_t read_size = fread(file_data, 1, Size, fp);
                file_data[read_size] = '\0';
                test_parse_functions(file_data, read_size, selector + 2, 1);
                free(file_data);
            }
            fclose(fp);
        }
    }
    
    free(null_terminated_data);
    return 0;
}