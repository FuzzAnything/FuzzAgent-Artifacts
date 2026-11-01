#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cJSON.h>

/* Helper to manually create a cJSON item with specific type and valuestring */
static cJSON *create_manual_cjson(int type, const char *valuestring) {
    cJSON *item = cJSON_CreateNull(); // Start with a valid item to get memory
    if (!item) {
        return NULL;
    }
    
    // Override the type
    item->type = type;
    
    // Override valuestring if provided
    if (valuestring) {
        item->valuestring = strdup(valuestring);
    } else {
        item->valuestring = NULL;
    }
    
    // Clear other fields to avoid dangling pointers if we reuse the structure
    item->next = NULL;
    item->prev = NULL;
    item->child = NULL;
    item->string = NULL;
    item->valueint = 0;
    item->valuedouble = 0.0;
    
    return item;
}

/* Cleanup helper */
static void cleanup_cjson(cJSON *item) {
    if (item) {
        cJSON_Delete(item);
    }
}

int LLVMFuzzerTestOneInput_54(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* 
     * Target Line 1427: if ((item == NULL) || (output_buffer == NULL))
     * Strategy: Pass NULL as the item to cJSON_PrintBuffered.
     */
    char *result = cJSON_PrintBuffered(NULL, 100, 0);
    if (result) {
        free(result);
    }

    /* 
     * Target Line 1491: default: return false;
     * Strategy: Create a cJSON item with an invalid type (e.g., 999).
     */
    cJSON *invalid_item = create_manual_cjson(999, NULL);
    if (invalid_item) {
        result = cJSON_PrintBuffered(invalid_item, 100, 0);
        if (result) {
            free(result);
        }
        cleanup_cjson(invalid_item);
    }

    /* 
     * Target Line 1464: case cJSON_Raw:
     * Target Line 1478: memcpy(output, item->valuestring, raw_length);
     * Strategy: Create a valid cJSON_Raw item with a string.
     */
    cJSON *raw_item_valid = create_manual_cjson(cJSON_Raw, "raw_value");
    if (raw_item_valid) {
        result = cJSON_PrintBuffered(raw_item_valid, 100, 0);
        if (result) {
            free(result);
        }
        cleanup_cjson(raw_item_valid);
    }

    /* 
     * Target Line 1467: if (item->valuestring == NULL)
     * Strategy: Create a cJSON_Raw item but with valuestring = NULL.
     */
    cJSON *raw_item_null = create_manual_cjson(cJSON_Raw, NULL);
    if (raw_item_null) {
        result = cJSON_PrintBuffered(raw_item_null, 100, 0);
        if (result) {
            free(result);
        }
        cleanup_cjson(raw_item_null);
    }

    /* 
     * Target Line 1474: if (output == NULL) inside cJSON_Raw case.
     * Strategy: Create a cJSON_Raw item with a very large string and a small prebuffer.
     * This forces ensure() to reallocate. If the allocation fails, it hits this line.
     */
    if (Size > 100) {
        char *large_str = (char *)malloc(Size + 1);
        if (large_str) {
            memcpy(large_str, Data, Size);
            large_str[Size] = '\0';
            
            cJSON *raw_item_large = create_manual_cjson(cJSON_Raw, large_str);
            if (raw_item_large) {
                result = cJSON_PrintBuffered(raw_item_large, 1, 0);
                if (result) {
                    free(result);
                }
                cleanup_cjson(raw_item_large);
            }
            free(large_str);
        }
    }

    /* 
     * Additional coverage: Test with parsed JSON to ensure normal paths work.
     */
    if (Size > 0) {
        char *safe_input = (char *)malloc(Size + 1);
        if (safe_input) {
            memcpy(safe_input, Data, Size);
            safe_input[Size] = '\0';
            
            cJSON *parsed = cJSON_Parse(safe_input);
            if (parsed) {
                result = cJSON_PrintBuffered(parsed, Size + 100, 0);
                if (result) {
                    free(result);
                }
                cJSON_Delete(parsed);
            }
            free(safe_input);
        }
    }

    return 0;
}