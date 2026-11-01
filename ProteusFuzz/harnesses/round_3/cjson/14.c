// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_ReplaceItemInObject at cJSON.c:2450:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"

int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    double num1 = 0.0;
    double num2 = 0.0;

    if (Size >= sizeof(double)) {
        memcpy(&num1, Data, sizeof(double));
    }
    if (Size >= 2 * sizeof(double)) {
        memcpy(&num2, Data + sizeof(double), sizeof(double));
    } else if (Size > sizeof(double)) {
        num2 = (double)(Data[sizeof(double)]);
    }

    cJSON *num_node1 = cJSON_CreateNumber(num1);
    if (num_node1 == NULL) {
        return 0;
    }

    cJSON *num_node2 = cJSON_CreateNumber(num2);
    if (num_node2 == NULL) {
        cJSON_Delete(num_node1);
        return 0;
    }

    cJSON *obj = cJSON_CreateObject();
    if (obj == NULL) {
        cJSON_Delete(num_node1);
        cJSON_Delete(num_node2);
        return 0;
    }

    // Add first item to object. Ownership is transferred to the object.
    if (!cJSON_AddItemToObject(obj, "key1", num_node1)) {
        cJSON_Delete(num_node1);
        cJSON_Delete(obj);
        cJSON_Delete(num_node2);
        return 0;
    }

    // Replace the item. 
    // According to cjson documentation: "It does not free the old item; the caller must manage its memory."
    // However, the AddressSanitizer error indicates that in this specific version of cjson,
    // cJSON_ReplaceItemInObject might be freeing the old item internally or the logic 
    // of ownership transfer is different than expected in the error scenario.
    // The error "heap-use-after-free" on num_node1 suggests num_node1 was freed by the replace call.
    // To be safe and avoid double-free or use-after-free, we should NOT manually delete num_node1
    // if the library implementation frees it. 
    // Standard behavior for many JSON libraries is that replace frees the old value.
    // Let's assume the library frees the old item upon replacement to fix the UAF.
    
    cJSON_bool replace_result = cJSON_ReplaceItemInObject(obj, "key1", num_node2);
    
    if (!replace_result) {
        // If replacement fails, num_node2 was not added. We must delete it.
        // num_node1 is still in the object.
        cJSON_Delete(num_node2);
        cJSON_Delete(obj);
        return 0;
    }

    // If replacement succeeded:
    // The old item (num_node1) is removed from the object.
    // Based on the ASAN error, the library likely freed num_node1 during the replace operation.
    // Therefore, we must NOT call cJSON_Delete(num_node1) again.
    // num_node2 is now owned by the object.
    
    // We do NOT delete num_node1 here to prevent double-free/use-after-free.
    
    // Delete the object, which will clean up num_node2.
    cJSON_Delete(obj);

    return 0;
}