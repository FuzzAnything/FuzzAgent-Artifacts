#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cJSON.h>

/* Helper to create a Raw cJSON item with specific content */
static cJSON *create_raw_item(const char *content, size_t len) {
    cJSON *item = cJSON_CreateRaw(content);
    if (!item) {
        return NULL;
    }
    /* Ensure type is explicitly Raw, though CreateRaw usually sets it */
    item->type = cJSON_Raw;
    return item;
}

/* Helper to create a Raw cJSON item with NULL valuestring (for error path) */
static cJSON *create_raw_item_null_string(void) {
    cJSON *item = cJSON_CreateRaw("");
    if (!item) {
        return NULL;
    }
    item->type = cJSON_Raw;
    /* Explicitly set valuestring to NULL to trigger line 1467 */
    item->valuestring = NULL;
    return item;
}

int LLVMFuzzerTestOneInput_58(const uint8_t *Data, size_t Size) {
    char buffer[4096];
    cJSON *item = NULL;
    cJSON_bool result;

    /* Case 1: Trigger Line 1427 (item == NULL) */
    /* Passing NULL as the first argument to cJSON_PrintPreallocated */
    result = cJSON_PrintPreallocated(NULL, buffer, sizeof(buffer), 1);
    (void)result; /* Suppress unused warning */

    /* Case 2: Trigger Lines 1464, 1467 (cJSON_Raw with NULL valuestring) */
    item = create_raw_item_null_string();
    if (item) {
        /* Buffer size doesn't matter here, will fail at valuestring check */
        result = cJSON_PrintPreallocated(item, buffer, sizeof(buffer), 1);
        (void)result;
        cJSON_Delete(item);
        item = NULL;
    }

    /* Case 3: Trigger Lines 1464, 1474 (cJSON_Raw with valid string but small buffer) */
    /* We need a string long enough to exceed a small buffer */
    if (Size >= 10) {
        /* Use fuzz data as the raw string content */
        char *raw_content = (char *)malloc(Size + 1);
        if (raw_content) {
            memcpy(raw_content, Data, Size);
            raw_content[Size] = '\0';
            
            item = create_raw_item(raw_content, Size);
            if (item) {
                /* Provide a tiny buffer (e.g., 5 bytes) to force ensure() to return NULL */
                /* This triggers line 1474: if (output == NULL) */
                result = cJSON_PrintPreallocated(item, buffer, 5, 1);
                (void)result;
                cJSON_Delete(item);
                item = NULL;
            }
            free(raw_content);
        }
    }

    /* Case 4: Happy path for cJSON_Raw (to ensure basic functionality works) */
    if (Size > 0) {
        char *raw_content = (char *)malloc(Size + 1);
        if (raw_content) {
            memcpy(raw_content, Data, Size);
            raw_content[Size] = '\0';
            
            item = create_raw_item(raw_content, Size);
            if (item) {
                /* Provide a large enough buffer */
                result = cJSON_PrintPreallocated(item, buffer, sizeof(buffer), 1);
                (void)result;
                cJSON_Delete(item);
                item = NULL;
            }
            free(raw_content);
        }
    }

    return 0;
}