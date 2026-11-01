// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_IsFalse at cJSON.c:2982:26 in cJSON.h
// cJSON_IsRaw at cJSON.c:3062:26 in cJSON.h
// cJSON_IsInvalid at cJSON.c:2972:26 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_IsNull at cJSON.c:3012:26 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateRaw at cJSON.c:2581:23 in cJSON.h
// cJSON_IsObject at cJSON.c:3052:26 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cJSON.h>

/* Helper to create a cJSON item with a specific type for fuzzing */
static cJSON* create_fuzzed_item(int type) {
    cJSON *item = cJSON_CreateNull();
    if (!item) {
        return NULL;
    }
    /* Override the type to test specific branches */
    item->type = type;
    return item;
}

/* Helper to create a raw string item */
static cJSON* create_raw_item(const char *raw) {
    cJSON *item = cJSON_CreateRaw(raw);
    return item;
}

int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Use the first byte to determine which type to test or if we test NULL */
    int type_choice = Data[0] % 15; /* 0-14 covers various types */
    
    cJSON *item = NULL;
    cJSON_bool result = 0;

    /* Map choice to cJSON types */
    /* 0: NULL pointer test */
    /* 1: cJSON_Invalid */
    /* 2: cJSON_False */
    /* 3: cJSON_True */
    /* 4: cJSON_Number */
    /* 5: cJSON_String */
    /* 6: cJSON_Array */
    /* 7: cJSON_Object */
    /* 8: cJSON_Raw */
    /* 9: cJSON_NULL */
    /* 10-14: Random types or edge cases */

    if (type_choice == 0) {
        /* Test with NULL pointer */
        item = NULL;
    } else if (type_choice == 1) {
        item = create_fuzzed_item(cJSON_Invalid);
    } else if (type_choice == 2) {
        item = create_fuzzed_item(cJSON_False);
    } else if (type_choice == 3) {
        item = create_fuzzed_item(cJSON_True);
    } else if (type_choice == 4) {
        item = create_fuzzed_item(cJSON_Number);
    } else if (type_choice == 5) {
        item = create_fuzzed_item(cJSON_String);
    } else if (type_choice == 6) {
        item = create_fuzzed_item(cJSON_Array);
    } else if (type_choice == 7) {
        item = create_fuzzed_item(cJSON_Object);
    } else if (type_choice == 8) {
        /* Create a raw item using remaining data if available */
        if (Size > 1) {
            /* Ensure null termination for safety */
            char *raw_buf = (char *)malloc(Size);
            if (raw_buf) {
                memcpy(raw_buf, Data + 1, Size - 1);
                raw_buf[Size - 1] = '\0';
                item = create_raw_item(raw_buf);
                free(raw_buf);
            }
        }
        if (!item) {
            /* Fallback if malloc failed or size too small */
            item = create_fuzzed_item(cJSON_Raw);
        }
    } else if (type_choice == 9) {
        item = create_fuzzed_item(cJSON_NULL);
    } else {
        /* Random type for edge cases */
        int random_type = 0;
        if (Size > 1) {
            random_type = Data[1] % 16;
        } else {
            random_type = 0; /* Default to 0 if size is too small */
        }
        item = create_fuzzed_item(random_type);
    }

    /* Invoke the target API functions */
    /* 1. cJSON_IsObject */
    result = cJSON_IsObject(item);
    (void)result; /* Suppress unused variable warning */

    /* 2. cJSON_IsFalse */
    result = cJSON_IsFalse(item);
    (void)result;

    /* 3. cJSON_IsRaw */
    result = cJSON_IsRaw(item);
    (void)result;

    /* 4. cJSON_IsInvalid */
    result = cJSON_IsInvalid(item);
    (void)result;

    /* 5. cJSON_IsNull */
    result = cJSON_IsNull(item);
    (void)result;

    /* 6. cJSON_IsBool */
    result = cJSON_IsBool(item);
    (void)result;

    /* Cleanup */
    if (item) {
        cJSON_Delete(item);
    }

    return 0;
}