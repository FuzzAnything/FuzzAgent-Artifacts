// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_IsNull at cJSON.c:3012:26 in cJSON.h
// cJSON_IsFalse at cJSON.c:2982:26 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_IsInvalid at cJSON.c:2972:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_IsNull at cJSON.c:3012:26 in cJSON.h
// cJSON_IsFalse at cJSON.c:2982:26 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_IsInvalid at cJSON.c:2972:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_IsNull at cJSON.c:3012:26 in cJSON.h
// cJSON_IsFalse at cJSON.c:2982:26 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_IsInvalid at cJSON.c:2972:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_IsNull at cJSON.c:3012:26 in cJSON.h
// cJSON_IsFalse at cJSON.c:2982:26 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_IsInvalid at cJSON.c:2972:26 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cJSON.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Helper to create a cJSON item with a specific type from raw bytes */
static cJSON* create_item_from_input(const uint8_t *data, size_t size) {
    cJSON *item = NULL;
    
    if (size == 0) {
        return NULL;
    }

    /* Interpret the first byte as a type indicator to diversify states */
    int type = data[0] % 16; /* Map to 0-15 range, covering common types */
    
    /* Create a basic item */
    item = cJSON_CreateNull();
    if (!item) {
        return NULL;
    }

    /* Override the type based on input to test various type checks */
    /* Note: Directly modifying type is unsafe in production but valid for fuzzing internal states */
    item->type = type;

    /* If we have enough data, try to attach a string or number to make it more complex */
    if (size > 1) {
        if (type == cJSON_String || type == cJSON_Raw) {
            size_t len = (size - 1);
            if (len > 1024) len = 1024; /* Limit size for safety */
            char *str = (char*)malloc(len + 1);
            if (str) {
                memcpy(str, &data[1], len);
                str[len] = '\0';
                item->valuestring = str;
            }
        } else if (type == cJSON_Number) {
            if (size >= 9) {
                double val = 0.0;
                /* Simple heuristic to create a double from bytes */
                uint64_t bits = 0;
                for (int i = 0; i < 8 && (i + 1) < size; i++) {
                    bits |= ((uint64_t)data[i+1]) << (i * 8);
                }
                memcpy(&val, &bits, sizeof(double));
                item->valuedouble = val;
            }
        }
    }

    return item;
}

/* Helper to parse JSON string from input if it looks like valid JSON */
static cJSON* parse_json_from_input(const uint8_t *data, size_t size) {
    if (size == 0) return NULL;
    
    /* Ensure null termination for cJSON_Parse */
    char *json_str = (char*)malloc(size + 1);
    if (!json_str) return NULL;
    
    memcpy(json_str, data, size);
    json_str[size] = '\0';
    
    cJSON *item = cJSON_Parse(json_str);
    free(json_str);
    return item;
}

int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Strategy 1: Parse valid JSON string */
    cJSON *parsed_item = parse_json_from_input(Data, Size);
    
    if (parsed_item) {
        /* Test all target functions on parsed JSON */
        cJSON_IsNull(parsed_item);
        cJSON_IsFalse(parsed_item);
        cJSON_IsArray(parsed_item);
        cJSON_IsInvalid(parsed_item);
        cJSON_IsTrue(parsed_item);
        cJSON_IsBool(parsed_item);
        
        /* Also test on children if they exist */
        cJSON *child = parsed_item->child;
        while (child) {
            cJSON_IsNull(child);
            cJSON_IsFalse(child);
            cJSON_IsArray(child);
            cJSON_IsInvalid(child);
            cJSON_IsTrue(child);
            cJSON_IsBool(child);
            child = child->next;
        }
        
        cJSON_Delete(parsed_item);
    }

    /* Strategy 2: Construct raw cJSON items with manipulated types */
    cJSON *raw_item = create_item_from_input(Data, Size);
    
    if (raw_item) {
        /* Test all target functions on raw constructed items */
        cJSON_IsNull(raw_item);
        cJSON_IsFalse(raw_item);
        cJSON_IsArray(raw_item);
        cJSON_IsInvalid(raw_item);
        cJSON_IsTrue(raw_item);
        cJSON_IsBool(raw_item);
        
        cJSON_Delete(raw_item);
    }

    /* Strategy 3: Test with NULL pointer explicitly */
    cJSON_IsNull(NULL);
    cJSON_IsFalse(NULL);
    cJSON_IsArray(NULL);
    cJSON_IsInvalid(NULL);
    cJSON_IsTrue(NULL);
    cJSON_IsBool(NULL);

    return 0;
}