// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_IsFalse at cJSON.c:2982:26 in cJSON.h
// cJSON_IsNull at cJSON.c:3012:26 in cJSON.h
// cJSON_IsInvalid at cJSON.c:2972:26 in cJSON.h
// cJSON_IsObject at cJSON.c:3052:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_free at cJSON.c:3202:20 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_IsFalse at cJSON.c:2982:26 in cJSON.h
// cJSON_IsNull at cJSON.c:3012:26 in cJSON.h
// cJSON_IsInvalid at cJSON.c:2972:26 in cJSON.h
// cJSON_IsObject at cJSON.c:3052:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_IsFalse at cJSON.c:2982:26 in cJSON.h
// cJSON_IsNull at cJSON.c:3012:26 in cJSON.h
// cJSON_IsInvalid at cJSON.c:2972:26 in cJSON.h
// cJSON_IsObject at cJSON.c:3052:26 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_IsFalse at cJSON.c:2982:26 in cJSON.h
// cJSON_IsNull at cJSON.c:3012:26 in cJSON.h
// cJSON_IsInvalid at cJSON.c:2972:26 in cJSON.h
// cJSON_IsObject at cJSON.c:3052:26 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <cJSON.h>

/* Helper function to create a cJSON item with a specific type from fuzz data */
static cJSON* create_item_from_data(const uint8_t *data, size_t size, int type) {
    cJSON *item = cJSON_CreateNull();
    if (!item) {
        return NULL;
    }
    
    /* Manually override the type to test specific type checkers */
    item->type = type;
    
    /* If data is provided and type supports it, attach a string value to avoid crashes in some internal logic */
    if (size > 0 && (type == cJSON_String || type == cJSON_Raw || type == cJSON_Object || type == cJSON_Array)) {
        char *str = (char *)malloc(size + 1);
        if (str) {
            memcpy(str, data, size);
            str[size] = '\0';
            cJSON_free(item->valuestring);
            item->valuestring = str;
        }
    }
    
    return item;
}

/* Helper function to create a valid JSON object/array from string data for parsing tests */
static cJSON* parse_json_from_data(const uint8_t *data, size_t size) {
    if (size == 0) {
        return NULL;
    }
    
    char *str = (char *)malloc(size + 1);
    if (!str) {
        return NULL;
    }
    
    memcpy(str, data, size);
    str[size] = '\0';
    
    cJSON *item = cJSON_Parse(str);
    free(str);
    return item;
}

int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Test Case 1: Direct type manipulation using create_item_from_data */
    /* We iterate through all known types to ensure Is* functions handle them correctly */
    int types[] = {
        cJSON_False, cJSON_True, cJSON_NULL, cJSON_Number, 
        cJSON_String, cJSON_Array, cJSON_Object, cJSON_Raw, cJSON_Invalid
    };
    int num_types = sizeof(types) / sizeof(types[0]);

    for (int i = 0; i < num_types; i++) {
        cJSON *item = create_item_from_data(Data, Size, types[i]);
        if (item) {
            /* Invoke target APIs */
            cJSON_IsArray(item);
            cJSON_IsBool(item);
            cJSON_IsFalse(item);
            cJSON_IsNull(item);
            cJSON_IsInvalid(item);
            cJSON_IsObject(item);
            
            cJSON_Delete(item);
        }
    }

    /* Test Case 2: NULL pointer handling */
    cJSON_IsArray(NULL);
    cJSON_IsBool(NULL);
    cJSON_IsFalse(NULL);
    cJSON_IsNull(NULL);
    cJSON_IsInvalid(NULL);
    cJSON_IsObject(NULL);

    /* Test Case 3: Parsing valid/invalid JSON strings from fuzz data */
    /* This tests the APIs on items created by the parser */
    cJSON *parsed = parse_json_from_data(Data, Size);
    if (parsed) {
        cJSON_IsArray(parsed);
        cJSON_IsBool(parsed);
        cJSON_IsFalse(parsed);
        cJSON_IsNull(parsed);
        cJSON_IsInvalid(parsed);
        cJSON_IsObject(parsed);
        
        /* Recursively check children to explore deeper states */
        cJSON *child = parsed->child;
        while (child) {
            cJSON_IsArray(child);
            cJSON_IsBool(child);
            cJSON_IsFalse(child);
            cJSON_IsNull(child);
            cJSON_IsInvalid(child);
            cJSON_IsObject(child);
            child = child->next;
        }
        
        cJSON_Delete(parsed);
    }

    return 0;
}