// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_DetachItemFromObject at cJSON.c:2309:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_DetachItemFromObjectCaseSensitive at cJSON.c:2316:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_DetachItemFromObject at cJSON.c:2309:23 in cJSON.h
// cJSON_DetachItemFromObjectCaseSensitive at cJSON.c:2316:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddNullToObject at cJSON.c:2150:22 in cJSON.h
// cJSON_GetObjectItem at cJSON.c:1983:23 in cJSON.h
// cJSON_AddRawToObject at cJSON.c:2222:22 in cJSON.h
// cJSON_AddStringToObject at cJSON.c:2210:22 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static void fuzz_cjson_apis(const uint8_t *data, size_t size) {
    if (size < 4) return;
    
    /* Create a dummy file if needed */
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
    
    /* Initialize cJSON objects */
    cJSON *root = cJSON_CreateObject();
    if (!root) return;
    
    /* Extract strings from fuzzer input */
    size_t key_len = size / 4;
    size_t val_len = size / 4;
    if (key_len == 0) key_len = 1;
    if (val_len == 0) val_len = 1;
    
    char *key1 = (char*)malloc(key_len + 1);
    char *key2 = (char*)malloc(key_len + 1);
    char *raw_val = (char*)malloc(val_len + 1);
    char *str_val = (char*)malloc(val_len + 1);
    
    if (!key1 || !key2 || !raw_val || !str_val) {
        free(key1); free(key2); free(raw_val); free(str_val);
        cJSON_Delete(root);
        return;
    }
    
    /* Prepare strings from fuzzer data */
    memcpy(key1, data, key_len < size ? key_len : size);
    key1[key_len] = '\0';
    
    size_t offset = key_len;
    memcpy(key2, data + offset, key_len < (size - offset) ? key_len : (size - offset));
    key2[key_len] = '\0';
    
    offset += key_len;
    memcpy(raw_val, data + offset, val_len < (size - offset) ? val_len : (size - offset));
    raw_val[val_len] = '\0';
    
    offset += val_len;
    memcpy(str_val, data + offset, val_len < (size - offset) ? val_len : (size - offset));
    str_val[val_len] = '\0';
    
    double num_val = 0.0;
    if (size >= sizeof(double) + offset) {
        memcpy(&num_val, data + offset, sizeof(double));
    } else {
        num_val = (double)(size % 1000);
    }
    
    /* Test API 1: cJSON_AddNullToObject */
    cJSON *null_item = cJSON_AddNullToObject(root, key1);
    if (null_item) {
        /* Verify the item was added */
        cJSON *get_item = cJSON_GetObjectItem(root, key1);
        if (get_item && get_item->type != cJSON_NULL) {
            /* Unexpected state */
        }
    }
    
    /* Test API 2: cJSON_AddRawToObject */
    cJSON *raw_item = cJSON_AddRawToObject(root, key2, raw_val);
    if (raw_item) {
        if (raw_item->type != cJSON_Raw) {
            /* Unexpected state */
        }
    }
    
    /* Test API 5: cJSON_AddStringToObject */
    cJSON *str_item = cJSON_AddStringToObject(root, "fuzz_string", str_val);
    if (str_item) {
        if (str_item->type != cJSON_String) {
            /* Unexpected state */
        }
    }
    
    /* Test API 6: cJSON_AddNumberToObject */
    cJSON *num_item = cJSON_AddNumberToObject(root, "fuzz_number", num_val);
    if (num_item) {
        if (num_item->type != cJSON_Number) {
            /* Unexpected state */
        }
    }
    
    /* Test API 3: cJSON_DetachItemFromObject */
    cJSON *detached = cJSON_DetachItemFromObject(root, key1);
    if (detached) {
        cJSON_Delete(detached); /* Clean up detached item */
    }
    
    /* Test API 4: cJSON_DetachItemFromObjectCaseSensitive */
    detached = cJSON_DetachItemFromObjectCaseSensitive(root, key2);
    if (detached) {
        cJSON_Delete(detached);
    }
    
    /* Try to detach non-existent item */
    cJSON_DetachItemFromObject(root, "non_existent_key");
    cJSON_DetachItemFromObjectCaseSensitive(root, "NON_EXISTENT_KEY");
    
    /* Clean up */
    free(key1);
    free(key2);
    free(raw_val);
    free(str_val);
    cJSON_Delete(root);
    
    /* Remove dummy file */
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_42(const uint8_t *data, size_t size) {
    fuzz_cjson_apis(data, size);
    return 0;
}