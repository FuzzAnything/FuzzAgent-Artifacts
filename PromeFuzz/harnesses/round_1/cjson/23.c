// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateFalse at cJSON.c:2483:23 in cJSON.h
// cJSON_IsFalse at cJSON.c:2982:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddBoolToObject at cJSON.c:2186:22 in cJSON.h
// cJSON_IsFalse at cJSON.c:2982:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_CreateFalse at cJSON.c:2483:23 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
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

static void fuzz_cjson_apis(const uint8_t *data, size_t size) {
    /* Create a false item */
    cJSON *false_item = cJSON_CreateFalse();
    if (false_item == NULL) {
        return;
    }
    
    /* Test cJSON_IsFalse, cJSON_IsTrue, cJSON_IsBool on false item */
    cJSON_bool is_false = cJSON_IsFalse(false_item);
    cJSON_bool is_true = cJSON_IsTrue(false_item);
    cJSON_bool is_bool = cJSON_IsBool(false_item);
    
    /* Create an object to add boolean values to */
    cJSON *object = cJSON_CreateObject();
    if (object == NULL) {
        cJSON_Delete(false_item);
        return;
    }
    
    /* Generate a name from fuzzer data for adding boolean */
    char name[256];
    size_t name_len = size < 255 ? size : 255;
    if (name_len > 0) {
        memcpy(name, data, name_len);
        name[name_len] = '\0';
    } else {
        strcpy(name, "default");
    }
    
    /* Add boolean to object using fuzzer data to decide value */
    cJSON_bool bool_value = (size > 0) ? (data[0] & 1) : 0;
    cJSON *added_bool = cJSON_AddBoolToObject(object, name, bool_value);
    
    /* Test boolean checking functions on the added item */
    if (added_bool != NULL) {
        cJSON_bool added_is_false = cJSON_IsFalse(added_bool);
        cJSON_bool added_is_true = cJSON_IsTrue(added_bool);
        cJSON_bool added_is_bool = cJSON_IsBool(added_bool);
    }
    
    /* Create another false item for comparison */
    cJSON *false_item2 = cJSON_CreateFalse();
    
    /* Test comparisons with various case sensitivity values */
    if (false_item2 != NULL) {
        cJSON_bool compare_same = cJSON_Compare(false_item, false_item2, 1);
        cJSON_bool compare_same_insensitive = cJSON_Compare(false_item, false_item2, 0);
        
        /* Compare false item with object */
        cJSON_bool compare_diff = cJSON_Compare(false_item, object, 1);
        cJSON_bool compare_diff_insensitive = cJSON_Compare(false_item, object, 0);
        
        cJSON_Delete(false_item2);
    }
    
    /* Test comparison with NULL items */
    cJSON_bool compare_null1 = cJSON_Compare(NULL, false_item, 1);
    cJSON_bool compare_null2 = cJSON_Compare(false_item, NULL, 1);
    cJSON_bool compare_both_null = cJSON_Compare(NULL, NULL, 1);
    
    /* Test comparison of object with itself */
    cJSON_bool compare_self = cJSON_Compare(object, object, 1);
    
    /* Clean up */
    cJSON_Delete(false_item);
    cJSON_Delete(object);
}

int LLVMFuzzerTestOneInput_23(const uint8_t *data, size_t size) {
    /* Call the main fuzzing function */
    fuzz_cjson_apis(data, size);
    
    return 0;
}