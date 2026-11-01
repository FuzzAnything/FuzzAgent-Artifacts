// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddItemReferenceToObject at cJSON.c:2140:26 in cJSON.h
// cJSON_AddRawToObject at cJSON.c:2222:22 in cJSON.h
// cJSON_AddStringToObject at cJSON.c:2210:22 in cJSON.h
// cJSON_DeleteItemFromObject at cJSON.c:2323:20 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToObjectCS at cJSON.c:2125:26 in cJSON.h
// cJSON_AddObjectToObject at cJSON.c:2234:22 in cJSON.h
// cJSON_AddStringToObject at cJSON.c:2210:22 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddStringToObject at cJSON.c:2210:22 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cJSON.h>

static void safe_cleanup(cJSON *root) {
    if (root) {
        cJSON_Delete(root);
    }
}

int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    cJSON *root = cJSON_CreateObject();
    if (!root) {
        return 0;
    }

    // Interpret input data to drive fuzzing logic
    // Use first byte to select operation mode
    int mode = Data[0] % 6;
    
    // Create a key and value string from input data
    char key[256];
    char value[256];
    
    // Calculate lengths safely, ensuring we don't read past Size
    size_t available_data = Size;
    size_t key_len = (available_data > 1) ? (Data[1] % 255) + 1 : 1;
    size_t val_len = (available_data > 2) ? (Data[2] % 255) + 1 : 1;
    
    // Ensure lengths do not exceed buffer sizes and available data
    if (key_len >= sizeof(key)) key_len = sizeof(key) - 1;
    if (val_len >= sizeof(value)) val_len = sizeof(value) - 1;
    
    // Ensure we don't read more data than available for key
    if (key_len > available_data) key_len = available_data;
    
    // Copy data into buffers, ensuring null termination
    if (key_len > 0) {
        memcpy(key, Data, key_len);
    }
    key[key_len] = '\0';
    
    // Ensure we don't read more data than available for value
    size_t data_offset = key_len + 1; // Skip the byte used for key_len calculation if needed, but here we just use raw data
    // Actually, the logic above used Data[1] and Data[2] for lengths, so data starts at index 0 for content?
    // Let's simplify: Use Data[0] for mode. Data[1] for key_len. Data[2] for val_len.
    // Content for key starts at Data[3]. Content for value starts after key content.
    
    // Refined parsing logic to avoid overlap and buffer overflows
    size_t content_start = 3;
    if (Size <= content_start) {
        // Not enough data to have lengths and content
        safe_cleanup(root);
        return 0;
    }

    // Re-calculate lengths based on available data after headers
    size_t remaining = Size - content_start;
    
    // Re-read lengths from Data[1] and Data[2] but cap them strictly
    key_len = Data[1] % (sizeof(key) - 1);
    val_len = Data[2] % (sizeof(value) - 1);
    
    // Ensure we don't exceed remaining data
    if (key_len > remaining) key_len = remaining;
    
    // Copy key
    if (key_len > 0) {
        memcpy(key, Data + content_start, key_len);
    }
    key[key_len] = '\0';
    
    // Calculate value length based on what's left
    size_t value_start_offset = key_len;
    if (value_start_offset >= remaining) {
        val_len = 0;
    } else {
        size_t available_val = remaining - value_start_offset;
        if (val_len > available_val) val_len = available_val;
        if (val_len > 0) {
            memcpy(value, Data + content_start + value_start_offset, val_len);
        }
    }
    value[val_len] = '\0';

    cJSON *temp_obj = NULL;
    cJSON *temp_item = NULL;

    switch (mode) {
        case 0: {
            // Test cJSON_AddStringToObject
            temp_obj = cJSON_AddStringToObject(root, key, value);
            if (!temp_obj) {
                // Expected on duplicate keys or allocation failure
            }
            break;
        }
        case 1: {
            // Test cJSON_AddItemReferenceToObject
            // Create a separate item to reference
            cJSON *ref_item = cJSON_CreateString("reference_value");
            if (ref_item) {
                cJSON_AddItemReferenceToObject(root, key, ref_item);
                // Note: ref_item is now owned by root, do not delete separately
            }
            break;
        }
        case 2: {
            // Test cJSON_AddRawToObject
            temp_obj = cJSON_AddRawToObject(root, key, value);
            if (!temp_obj) {
                // Expected on duplicate keys or allocation failure
            }
            break;
        }
        case 3: {
            // Test cJSON_DeleteItemFromObject
            // First add something to delete
            cJSON_AddStringToObject(root, "temp_key", "temp_value");
            cJSON_DeleteItemFromObject(root, key);
            break;
        }
        case 4: {
            // Test cJSON_AddItemToObjectCS
            cJSON *cs_item = cJSON_CreateNumber(123);
            if (cs_item) {
                cJSON_AddItemToObjectCS(root, key, cs_item);
                // cs_item is now owned by root
            }
            break;
        }
        case 5: {
            // Test cJSON_AddObjectToObject
            temp_obj = cJSON_AddObjectToObject(root, key);
            if (temp_obj) {
                // Add something inside the new object to exercise deeper logic
                cJSON_AddStringToObject(temp_obj, "nested_key", value);
            }
            break;
        }
        default:
            break;
    }

    // Cleanup
    safe_cleanup(root);
    return 0;
}