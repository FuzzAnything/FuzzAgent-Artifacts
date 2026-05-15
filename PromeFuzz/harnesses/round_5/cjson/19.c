// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddRawToObject at cJSON.c:2222:22 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddRawToObject at cJSON.c:2222:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
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
#include <unistd.h>
#include <fcntl.h>

static int write_to_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return -1;
    ssize_t written = write(fd, data, size);
    close(fd);
    return (written == (ssize_t)size) ? 0 : -1;
}

int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    // Split input into name and raw string parts
    size_t name_len = Size / 2;
    size_t raw_len = Size - name_len;
    
    // Allocate and null-terminate the strings
    char *name = (char *)malloc(name_len + 1);
    char *raw = (char *)malloc(raw_len + 1);
    if (!name || !raw) {
        free(name);
        free(raw);
        return 0;
    }
    
    memcpy(name, Data, name_len);
    name[name_len] = '\0';
    memcpy(raw, Data + name_len, raw_len);
    raw[raw_len] = '\0';

    // Create JSON object
    cJSON *obj = cJSON_CreateObject();
    if (!obj) {
        free(name);
        free(raw);
        return 0;
    }

    // Add raw JSON to object
    cJSON *added_item = cJSON_AddRawToObject(obj, name, raw);
    
    // Also test with file-based input
    if (write_to_dummy_file(Data, Size) == 0) {
        FILE *fp = fopen("./dummy_file", "r");
        if (fp) {
            // Recreate object for file-based test
            cJSON *file_obj = cJSON_CreateObject();
            if (file_obj) {
                // Use different string combinations
                char *alt_name = (char *)malloc(name_len + 1);
                char *alt_raw = (char *)malloc(raw_len + 1);
                if (alt_name && alt_raw) {
                    memcpy(alt_name, Data, name_len);
                    alt_name[name_len] = '\0';
                    memcpy(alt_raw, Data, raw_len);
                    alt_raw[raw_len] = '\0';
                    
                    cJSON *file_added = cJSON_AddRawToObject(file_obj, alt_name, alt_raw);
                    (void)file_added; // Mark as used
                    
                    free(alt_name);
                    free(alt_raw);
                }
                cJSON_Delete(file_obj);
            }
            fclose(fp);
        }
        unlink("./dummy_file");
    }

    // Clean up
    cJSON_Delete(obj);
    free(name);
    free(raw);

    return 0;
}