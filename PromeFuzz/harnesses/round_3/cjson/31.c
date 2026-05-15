// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_AddObjectToObject at cJSON.c:2234:22 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateRaw at cJSON.c:2581:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddRawToObject at cJSON.c:2222:22 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_AddItemToObjectCS at cJSON.c:2125:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_DeleteItemFromObject at cJSON.c:2323:20 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
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

static cJSON* create_dummy_item(void) {
    // Create a simple item for testing
    return cJSON_CreateString("dummy_value");
}

static const char* get_key_from_data(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset >= size) return NULL;
    
    const char* key = (const char*)(data + *offset);
    size_t key_len = 0;
    
    // Find null terminator or end of data
    while (*offset + key_len < size && data[*offset + key_len] != 0) {
        key_len++;
    }
    
    if (*offset + key_len >= size) {
        // No null terminator found, use remaining data
        key_len = size - *offset;
        *offset = size;
        // We need to copy to a null-terminated buffer
        static char buffer[256];
        size_t copy_len = key_len < 255 ? key_len : 255;
        memcpy(buffer, key, copy_len);
        buffer[copy_len] = '\0';
        return buffer;
    } else {
        // Null terminator found
        *offset += key_len + 1;
        return key;
    }
}

static char* get_raw_from_data(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset >= size) return NULL;
    
    const char* raw = (const char*)(data + *offset);
    size_t raw_len = 0;
    
    // Find null terminator or end of data
    while (*offset + raw_len < size && data[*offset + raw_len] != 0) {
        raw_len++;
    }
    
    if (*offset + raw_len >= size) {
        // No null terminator found, use remaining data
        raw_len = size - *offset;
        *offset = size;
        // We need to copy to a null-terminated buffer
        static char buffer[512];
        size_t copy_len = raw_len < 511 ? raw_len : 511;
        memcpy(buffer, raw, copy_len);
        buffer[copy_len] = '\0';
        return buffer;
    } else {
        // Null terminator found
        *offset += raw_len + 1;
        // Return as non-const for functions that need char*
        static char buffer[512];
        size_t copy_len = raw_len < 511 ? raw_len : 511;
        memcpy(buffer, raw, copy_len);
        buffer[copy_len] = '\0';
        return buffer;
    }
}

int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;
    
    // Use first byte to determine which operations to perform
    uint8_t operation_mask = Data[0];
    size_t offset = 1;
    
    // Create parent object
    cJSON* parent = cJSON_CreateObject();
    if (!parent) return 0;
    
    // Create some child objects for testing
    cJSON* child1 = cJSON_CreateObject();
    cJSON* child2 = cJSON_CreateObject();
    cJSON* child3 = cJSON_CreateObject();
    
    if (!child1 || !child2 || !child3) {
        cJSON_Delete(parent);
        if (child1) cJSON_Delete(child1);
        if (child2) cJSON_Delete(child2);
        if (child3) cJSON_Delete(child3);
        return 0;
    }
    
    // Test cJSON_AddObjectToObject
    if (operation_mask & 0x01) {
        const char* key = get_key_from_data(Data, Size, &offset);
        if (key) {
            cJSON* new_obj = cJSON_AddObjectToObject(parent, key);
            if (new_obj) {
                // Add something to the new object
                cJSON_AddItemToObject(new_obj, "nested_key", cJSON_CreateNumber(42));
            }
        }
    }
    
    // Test cJSON_CreateRaw
    if (operation_mask & 0x02) {
        char* raw_str = get_raw_from_data(Data, Size, &offset);
        if (raw_str) {
            cJSON* raw_item = cJSON_CreateRaw(raw_str);
            if (raw_item) {
                const char* key = get_key_from_data(Data, Size, &offset);
                if (key) {
                    cJSON_AddItemToObject(parent, key, raw_item);
                } else {
                    cJSON_Delete(raw_item);
                }
            }
        }
    }
    
    // Test cJSON_AddRawToObject
    if (operation_mask & 0x04) {
        const char* key = get_key_from_data(Data, Size, &offset);
        if (key) {
            char* raw_str = get_raw_from_data(Data, Size, &offset);
            if (raw_str) {
                cJSON* raw_item = cJSON_AddRawToObject(parent, key, raw_str);
                // raw_item is already added to parent, no need to delete separately
            }
        }
    }
    
    // Test cJSON_AddItemToObject with child1
    if (operation_mask & 0x08) {
        const char* key = get_key_from_data(Data, Size, &offset);
        if (key) {
            cJSON_AddItemToObject(parent, key, child1);
            child1 = NULL; // Ownership transferred
        }
    }
    
    // Test cJSON_AddItemToObjectCS with child2
    if (operation_mask & 0x10) {
        const char* key = get_key_from_data(Data, Size, &offset);
        if (key) {
            // For cJSON_AddItemToObjectCS, we need to ensure the string is const
            // We'll use a literal string to satisfy the requirement
            static const char* const_keys[] = {"literal1", "literal2", "literal3"};
            int key_index = key[0] % 3;
            cJSON_AddItemToObjectCS(parent, const_keys[key_index], child2);
            child2 = NULL; // Ownership transferred
        }
    }
    
    // Test cJSON_DeleteItemFromObject
    if (operation_mask & 0x20) {
        const char* key = get_key_from_data(Data, Size, &offset);
        if (key) {
            // First add an item, then delete it
            cJSON* temp_item = cJSON_CreateNumber(123);
            if (temp_item) {
                cJSON_AddItemToObject(parent, key, temp_item);
                cJSON_DeleteItemFromObject(parent, key);
            }
        }
    }
    
    // Add child3 using regular AddItemToObject if not already added
    if (child3) {
        const char* key = get_key_from_data(Data, Size, &offset);
        if (key) {
            cJSON_AddItemToObject(parent, key, child3);
        } else {
            cJSON_Delete(child3);
        }
    }
    
    // Clean up any remaining children that weren't added
    if (child1) cJSON_Delete(child1);
    if (child2) cJSON_Delete(child2);
    
    // Clean up parent (will delete all attached children)
    cJSON_Delete(parent);
    
    return 0;
}