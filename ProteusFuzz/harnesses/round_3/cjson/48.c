// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateBool at cJSON.c:2494:23 in cJSON.h
// cJSON_AddItemReferenceToObject at cJSON.c:2140:26 in cJSON.h
// cJSON_AddItemReferenceToArray at cJSON.c:2130:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateBool at cJSON.c:2494:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateBool at cJSON.c:2494:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateBool at cJSON.c:2494:23 in cJSON.h
// cJSON_CreateBool at cJSON.c:2494:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_CreateBool at cJSON.c:2494:23 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"

static void cleanup_json_tree(cJSON *item) {
    if (item) {
        cJSON_Delete(item);
    }
}

static cJSON *create_test_object_from_fuzz_data(const uint8_t *data, size_t size) {
    if (size == 0) {
        return NULL;
    }
    
    // Create a simple object to attach items to
    cJSON *obj = cJSON_CreateObject();
    if (!obj) {
        return NULL;
    }
    
    // Add some initial data based on fuzz input
    if (size > 0) {
        cJSON *bool_item = cJSON_CreateBool(data[0] % 2);
        if (bool_item) {
            cJSON_AddItemToObject(obj, "initial_bool", bool_item);
        }
    }
    
    return obj;
}

static cJSON *create_test_array_from_fuzz_data(const uint8_t *data, size_t size) {
    if (size == 0) {
        return NULL;
    }
    
    cJSON *arr = cJSON_CreateArray();
    if (!arr) {
        return NULL;
    }
    
    // Add some initial data based on fuzz input
    if (size > 0) {
        cJSON *bool_item = cJSON_CreateBool(data[0] % 2);
        if (bool_item) {
            cJSON_AddItemToArray(arr, bool_item);
        }
    }
    
    return arr;
}

int LLVMFuzzerTestOneInput_48(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    // Create test objects
    cJSON *obj = create_test_object_from_fuzz_data(Data, Size);
    cJSON *arr = create_test_array_from_fuzz_data(Data, Size);
    
    // Create some items to work with
    cJSON *item1 = cJSON_CreateBool(Data[0] % 2);
    cJSON *item2 = cJSON_CreateBool((Data[0] + 1) % 2);
    
    // Test cJSON_AddItemToObject
    if (obj && item1) {
        cJSON_AddItemToObject(obj, "key1", item1);
    }
    
    // Test cJSON_AddItemToArray
    if (arr && item2) {
        cJSON_AddItemToArray(arr, item2);
    }
    
    // Test cJSON_Duplicate
    if (obj) {
        cJSON *dup_obj = cJSON_Duplicate(obj, Data[0] % 2);
        if (dup_obj) {
            cleanup_json_tree(dup_obj);
        }
    }
    
    if (arr) {
        cJSON *dup_arr = cJSON_Duplicate(arr, Data[0] % 2);
        if (dup_arr) {
            cleanup_json_tree(dup_arr);
        }
    }
    
    // Create items for reference tests
    cJSON *ref_item1 = cJSON_CreateBool(Data[0] % 2);
    cJSON *ref_item2 = cJSON_CreateBool((Data[0] + 1) % 2);
    
    // Test cJSON_AddItemReferenceToObject
    if (obj && ref_item1) {
        cJSON_AddItemReferenceToObject(obj, "ref_key", ref_item1);
    }
    
    // Test cJSON_AddItemReferenceToArray
    if (arr && ref_item2) {
        cJSON_AddItemReferenceToArray(arr, ref_item2);
    }
    
    // Cleanup
    cleanup_json_tree(obj);
    cleanup_json_tree(arr);
    cleanup_json_tree(ref_item1);
    cleanup_json_tree(ref_item2);
    
    return 0;
}