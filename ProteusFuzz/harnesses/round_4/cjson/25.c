// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_AddItemReferenceToArray at cJSON.c:2130:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateArrayReference at cJSON.c:2571:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddArrayToObject at cJSON.c:2246:22 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddArrayToObject at cJSON.c:2246:22 in cJSON.h
// cJSON_AddArrayToObject at cJSON.c:2246:22 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_AddItemReferenceToArray at cJSON.c:2130:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddItemReferenceToArray at cJSON.c:2130:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"

int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Test cJSON_CreateArray
    cJSON *array = cJSON_CreateArray();
    if (array) {
        // Test cJSON_AddItemToArray with a null item (safe, returns false)
        cJSON_AddItemToArray(array, NULL);

        // Create a null item to add
        cJSON *null_item = cJSON_CreateNull();
        if (null_item) {
            cJSON_AddItemToArray(array, null_item);
            // null_item is now owned by array, do not delete separately
        }

        // Test cJSON_AddItemReferenceToArray
        cJSON *ref_item = cJSON_CreateNull();
        if (ref_item) {
            cJSON_AddItemReferenceToArray(array, ref_item);
            // ref_item is referenced by array. When array is deleted, ref_item is NOT deleted.
            // We must delete ref_item manually here to prevent leak.
            cJSON_Delete(ref_item);
        }

        // Test cJSON_CreateArrayReference
        cJSON *array_ref = cJSON_CreateArrayReference(array);
        if (array_ref) {
            // array_ref is a reference to 'array', deleting it does not delete 'array'
            cJSON_Delete(array_ref);
        }

        // Test cJSON_AddArrayToObject
        cJSON *obj = cJSON_CreateObject();
        if (obj) {
            cJSON *new_array = cJSON_AddArrayToObject(obj, "test_key");
            if (new_array) {
                // Add something to the new array
                cJSON *another_null = cJSON_CreateNull();
                if (another_null) {
                    cJSON_AddItemToArray(new_array, another_null);
                }
            }
            // obj owns new_array and its children
            cJSON_Delete(obj);
        }

        // array owns null_item
        cJSON_Delete(array);
    }

    // Additional diverse testing: Create multiple arrays and objects
    cJSON *obj2 = cJSON_CreateObject();
    if (obj2) {
        cJSON *arr1 = cJSON_AddArrayToObject(obj2, "arr1");
        cJSON *arr2 = cJSON_AddArrayToObject(obj2, "arr2");
        
        if (arr1 && arr2) {
            cJSON *item1 = cJSON_CreateNull();
            cJSON *item2 = cJSON_CreateNull();
            
            if (item1) cJSON_AddItemToArray(arr1, item1);
            if (item2) {
                cJSON_AddItemReferenceToArray(arr2, item2);
                // item2 is referenced by arr2, must delete manually
                cJSON_Delete(item2);
            }
            
            // Try adding reference to same item in another array
            if (item1) {
                cJSON_AddItemReferenceToArray(arr2, item1);
                // item1 is now referenced by arr2, but still owned by arr1.
                // This creates a shared reference scenario. 
                // When obj2 is deleted, arr2 is deleted, but item1 remains owned by arr1.
                // We must ensure we don't double free. Since arr1 owns item1, we don't delete item1 here.
            }
        }
        // obj2 owns arr1 and arr2. arr1 owns item1. arr2 references item1 but doesn't own it.
        // Deleting obj2 will delete arr1 (and item1) and arr2.
        cJSON_Delete(obj2);
    }

    // Test with minimal data to trigger edge cases
    cJSON *minimal = cJSON_CreateArray();
    if (minimal) {
        cJSON_AddItemToArray(minimal, NULL);
        cJSON_Delete(minimal);
    }

    return 0;
}