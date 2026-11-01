// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_IsInvalid at cJSON.c:2972:26 in cJSON.h
// cJSON_IsRaw at cJSON.c:3062:26 in cJSON.h
// cJSON_IsNull at cJSON.c:3012:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsFalse at cJSON.c:2982:26 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_IsInvalid at cJSON.c:2972:26 in cJSON.h
// cJSON_IsRaw at cJSON.c:3062:26 in cJSON.h
// cJSON_IsNull at cJSON.c:3012:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsFalse at cJSON.c:2982:26 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_IsInvalid at cJSON.c:2972:26 in cJSON.h
// cJSON_IsRaw at cJSON.c:3062:26 in cJSON.h
// cJSON_IsNull at cJSON.c:3012:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsFalse at cJSON.c:2982:26 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_IsInvalid at cJSON.c:2972:26 in cJSON.h
// cJSON_IsRaw at cJSON.c:3062:26 in cJSON.h
// cJSON_IsNull at cJSON.c:3012:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsFalse at cJSON.c:2982:26 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cJSON.h>

static void cleanup_cjson(cJSON *item) {
    if (item != NULL) {
        cJSON_Delete(item);
    }
}

int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a dummy cJSON item to test the type checking functions
    cJSON *item = (cJSON *)malloc(sizeof(cJSON));
    if (item == NULL) {
        return 0;
    }

    // Initialize the item with safe defaults
    memset(item, 0, sizeof(cJSON));

    // Use the fuzzer input to determine the type of the item
    uint8_t type_byte = Data[0];
    
    // Define a set of valid types to test
    int valid_types[] = {
        cJSON_Invalid,
        cJSON_False,
        cJSON_True,
        cJSON_NULL,
        cJSON_Number,
        cJSON_String,
        cJSON_Array,
        cJSON_Object,
        cJSON_Raw
    };
    int num_types = sizeof(valid_types) / sizeof(valid_types[0]);
    
    item->type = valid_types[type_byte % num_types];

    // Test all the target API functions
    cJSON_bool is_invalid = cJSON_IsInvalid(item);
    cJSON_bool is_raw = cJSON_IsRaw(item);
    cJSON_bool is_null = cJSON_IsNull(item);
    cJSON_bool is_true = cJSON_IsTrue(item);
    cJSON_bool is_false = cJSON_IsFalse(item);
    cJSON_bool is_bool = cJSON_IsBool(item);

    // Test with NULL pointer to ensure functions handle NULL gracefully
    cJSON_bool null_is_invalid = cJSON_IsInvalid(NULL);
    cJSON_bool null_is_raw = cJSON_IsRaw(NULL);
    cJSON_bool null_is_null = cJSON_IsNull(NULL);
    cJSON_bool null_is_true = cJSON_IsTrue(NULL);
    cJSON_bool null_is_false = cJSON_IsFalse(NULL);
    cJSON_bool null_is_bool = cJSON_IsBool(NULL);

    // Test with a parsed JSON object if the input looks like valid JSON
    // FIX: Ensure the input is null-terminated before passing to cJSON_Parse
    // which relies on strlen.
    if (Size > 0 && Data[Size - 1] != 0) {
        // Create a temporary buffer with null termination
        char *null_terminated_data = (char *)malloc(Size + 1);
        if (null_terminated_data != NULL) {
            memcpy(null_terminated_data, Data, Size);
            null_terminated_data[Size] = '\0';
            
            cJSON *parsed_item = cJSON_Parse(null_terminated_data);
            if (parsed_item != NULL) {
                cJSON_bool parsed_is_invalid = cJSON_IsInvalid(parsed_item);
                cJSON_bool parsed_is_raw = cJSON_IsRaw(parsed_item);
                cJSON_bool parsed_is_null = cJSON_IsNull(parsed_item);
                cJSON_bool parsed_is_true = cJSON_IsTrue(parsed_item);
                cJSON_bool parsed_is_false = cJSON_IsFalse(parsed_item);
                cJSON_bool parsed_is_bool = cJSON_IsBool(parsed_item);
                
                cJSON_Delete(parsed_item);
            }
            free(null_terminated_data);
        }
    } else {
        // If already null-terminated or empty (handled by Size check), parse directly
        cJSON *parsed_item = cJSON_Parse((const char *)Data);
        if (parsed_item != NULL) {
            cJSON_bool parsed_is_invalid = cJSON_IsInvalid(parsed_item);
            cJSON_bool parsed_is_raw = cJSON_IsRaw(parsed_item);
            cJSON_bool parsed_is_null = cJSON_IsNull(parsed_item);
            cJSON_bool parsed_is_true = cJSON_IsTrue(parsed_item);
            cJSON_bool parsed_is_false = cJSON_IsFalse(parsed_item);
            cJSON_bool parsed_is_bool = cJSON_IsBool(parsed_item);
            
            cJSON_Delete(parsed_item);
        }
    }

    // Cleanup
    cleanup_cjson(item);

    return 0;
}