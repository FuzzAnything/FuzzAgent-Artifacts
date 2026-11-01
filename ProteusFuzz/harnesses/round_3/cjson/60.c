#include <cJSON.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

static cJSON *create_test_object(const uint8_t *data, size_t size) {
    if (size == 0) {
        return cJSON_CreateObject();
    }
    cJSON *obj = cJSON_ParseWithLength((const char *)data, size);
    if (obj && cJSON_IsObject(obj)) {
        return obj;
    }
    if (obj) {
        cJSON_Delete(obj);
    }
    return cJSON_CreateObject();
}

static char *create_test_string(const uint8_t *data, size_t size) {
    if (size == 0) {
        char *s = malloc(2);
        if (s) {
            s[0] = 'k';
            s[1] = '\0';
        }
        return s;
    }
    char *s = malloc(size + 1);
    if (!s) return NULL;
    memcpy(s, data, size);
    s[size] = '\0';
    return s;
}

int LLVMFuzzerTestOneInput_60(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    size_t part = Size / 3;
    const uint8_t *obj_data = Data;
    const uint8_t *key_data = Data + part;
    const uint8_t *item_data = Data + 2 * part;
    
    size_t obj_len = part;
    size_t key_len = part;

    cJSON *obj = create_test_object(obj_data, obj_len);
    if (!obj) {
        return 0;
    }

    char *key = create_test_string(key_data, key_len);
    if (!key) {
        cJSON_Delete(obj);
        return 0;
    }

    // Create a replacement item that has a name string allocated (to trigger line 2435)
    // Fix: cJSON_DeleteItemFromObject returns void in modern cJSON versions.
    // Instead of adding to a temp object and detaching, create the item directly.
    // This creates a standalone cJSON item with a valid 'string' field.
    cJSON *replacement = cJSON_CreateString("temp_value");

    // Scenario 1: Trigger line 2429 (replacement == NULL)
    cJSON_ReplaceItemInObjectCaseSensitive(obj, key, NULL);

    // Scenario 2: Trigger line 2429 (string == NULL)
    cJSON_ReplaceItemInObjectCaseSensitive(obj, NULL, replacement);

    // Scenario 3: Trigger line 2435 and 2440 (Valid inputs)
    if (replacement) {
        // Ensure the key exists in obj so the replacement is attached (ownership transfer)
        cJSON_AddStringToObject(obj, key, "original_value");
        
        // This call should:
        // 1. Pass NULL checks (line 2429)
        // 2. Check replacement->string (line 2435) - should be true
        // 3. Call cJSON_strdup (line 2439)
        // 4. Check strdup result (line 2440)
        cJSON_ReplaceItemInObjectCaseSensitive(obj, key, replacement);
    }

    free(key);
    cJSON_Delete(obj);

    return 0;
}