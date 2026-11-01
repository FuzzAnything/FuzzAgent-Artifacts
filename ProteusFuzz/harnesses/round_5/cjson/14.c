// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_ReplaceItemInObject at cJSON.c:2450:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include "cJSON.h"

static void cleanup_json_tree(cJSON *root) {
    if (root != NULL) {
        cJSON_Delete(root);
    }
}

int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    // Extract two doubles from input data to create numbers safely
    double num1 = 0.0;
    double num2 = 0.0;

    if (Size >= 8) {
        memcpy(&num1, Data, sizeof(double));
        if (Size >= 16) {
            memcpy(&num2, Data + 8, sizeof(double));
        } else {
            // Safe fallback: use available bytes without overflow
            uint64_t temp = 0;
            size_t available = Size - 8;
            if (available > 0) {
                memcpy(&temp, Data + 8, available);
            }
            num2 = (double)(temp ^ 0x10);
        }
    } else {
        num1 = (double)(Data[0] ^ 0x10);
        if (Size > 1) {
            num2 = (double)(Data[1] ^ 0x20);
        } else {
            num2 = (double)(Data[0] ^ 0x30);
        }
    }

    // Create first number node
    cJSON *numNode1 = cJSON_CreateNumber(num1);
    if (numNode1 == NULL) {
        return 0;
    }

    // Create second number node
    cJSON *numNode2 = cJSON_CreateNumber(num2);
    if (numNode2 == NULL) {
        cJSON_Delete(numNode1);
        return 0;
    }

    // Create an object to hold items
    cJSON *obj = cJSON_CreateObject();
    if (obj == NULL) {
        cJSON_Delete(numNode1);
        cJSON_Delete(numNode2);
        return 0;
    }

    // Add first item to object
    cJSON_bool addResult = cJSON_AddItemToObject(obj, "key1", numNode1);
    
    // If add failed, we still own numNode1, so delete it.
    // If add succeeded, ownership of numNode1 is transferred to 'obj'.
    if (!addResult) {
        cJSON_Delete(numNode1);
        numNode1 = NULL;
    } else {
        // Mark that numNode1 is now owned by obj
        numNode1 = NULL; 
    }

    // Replace item in object with second number
    // cJSON_ReplaceItemInObject takes ownership of 'numNode2' if successful.
    // If it fails, 'numNode2' remains owned by the caller.
    cJSON_bool replaceResult = cJSON_ReplaceItemInObject(obj, "key1", numNode2);
    
    if (replaceResult) {
        // Success: numNode2 is now owned by 'obj'. 
        // The old item (if any) was deleted by cJSON_ReplaceItemInObject.
        // We set numNode2 to NULL to indicate we no longer own it.
        numNode2 = NULL;
    } else {
        // Failure: numNode2 was not attached. We must delete it to avoid leak.
        cJSON_Delete(numNode2);
        numNode2 = NULL;
    }

    // Final cleanup of the root object.
    // This will delete numNode2 (if replace succeeded) and any other children.
    // It will NOT double-free numNode1 because we set it to NULL after adding.
    cleanup_json_tree(obj);

    return 0;
}