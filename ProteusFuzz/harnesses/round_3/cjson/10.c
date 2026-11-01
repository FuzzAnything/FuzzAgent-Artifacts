// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateFalse at cJSON.c:2483:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateFalse at cJSON.c:2483:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateStringReference at cJSON.c:2548:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_CreateObjectReference at cJSON.c:2560:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"
#include <stdint.h>
#include <stdlib.h>

int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Step 1: cJSON_CreateFalse
    cJSON *falseNode = cJSON_CreateFalse();
    if (falseNode == NULL) {
        return 0;
    }

    // Step 2: cJSON_CreateString
    // Use input data as the string content, ensuring null termination
    char *strBuf = (char *)malloc(Size + 1);
    if (strBuf == NULL) {
        cJSON_Delete(falseNode);
        return 0;
    }
    // Copy data and ensure null termination
    if (Size > 0) {
        // Only copy up to the first null byte if present, or all data
        size_t copyLen = Size;
        for (size_t i = 0; i < Size; i++) {
            if (Data[i] == 0) {
                copyLen = i;
                break;
            }
        }
        if (copyLen > 0) {
            memcpy(strBuf, Data, copyLen);
            strBuf[copyLen] = '\0';
        } else {
            strBuf[0] = '\0';
        }
    } else {
        strBuf[0] = '\0';
    }

    cJSON *stringNode = cJSON_CreateString(strBuf);
    free(strBuf);

    if (stringNode == NULL) {
        cJSON_Delete(falseNode);
        return 0;
    }

    // Step 3: cJSON_CreateObject
    cJSON *objNode = cJSON_CreateObject();
    if (objNode == NULL) {
        cJSON_Delete(falseNode);
        cJSON_Delete(stringNode);
        return 0;
    }

    // Step 4: cJSON_CreateStringReference
    // Create a static string for reference to ensure safety
    static const char *refStr = "fuzz_reference_string";
    cJSON *refNode = cJSON_CreateStringReference(refStr);
    if (refNode == NULL) {
        cJSON_Delete(falseNode);
        cJSON_Delete(stringNode);
        cJSON_Delete(objNode);
        return 0;
    }

    // Optional: Add items to the object to exercise more logic
    cJSON_AddItemToObject(objNode, "false_key", falseNode);
    cJSON_AddItemToObject(objNode, "string_key", stringNode);
    cJSON_AddItemToObject(objNode, "ref_key", refNode);

    // Step 5: cJSON_Delete (First call)
    // Delete the object which should clean up children (falseNode, stringNode, refNode)
    // Note: refNode is a reference, so the external string "fuzz_reference_string" is not freed.
    cJSON_Delete(objNode);

    // Step 6: cJSON_CreateObjectReference
    // Create a new object to reference
    cJSON *targetObj = cJSON_CreateObject();
    if (targetObj == NULL) {
        return 0;
    }
    // Add a dummy item to targetObj to make it non-empty
    cJSON *dummyItem = cJSON_CreateString("dummy");
    if (dummyItem != NULL) {
        cJSON_AddItemToObject(targetObj, "dummy", dummyItem);
    }

    cJSON *refObjNode = cJSON_CreateObjectReference(targetObj);
    if (refObjNode == NULL) {
        cJSON_Delete(targetObj);
        return 0;
    }

    // Step 7: cJSON_Delete (Second call)
    // Delete the reference node. This should NOT free targetObj.
    cJSON_Delete(refObjNode);

    // Step 8: cJSON_Delete (Third call)
    // Delete the target object itself.
    cJSON_Delete(targetObj);

    // Step 9: cJSON_Delete (Fourth call)
    // We need 4 more deletes. Let's create and delete a few more simple nodes.
    cJSON *extra1 = cJSON_CreateFalse();
    if (extra1 != NULL) {
        cJSON_Delete(extra1);
    }

    // Step 10: cJSON_Delete (Fifth call)
    cJSON *extra2 = cJSON_CreateString("cleanup");
    if (extra2 != NULL) {
        cJSON_Delete(extra2);
    }

    // Step 11: cJSON_Delete (Sixth call - if needed, but we have 4 extra calls in the sequence)
    // The prompt asks for specific sequence:
    // 1. CreateFalse
    // 2. CreateString
    // 3. CreateObject
    // 4. CreateStringReference
    // 5. Delete (objNode)
    // 6. CreateObjectReference
    // 7. Delete (refObjNode)
    // 8. Delete (targetObj)
    // 9. Delete (extra1)
    // 10. Delete (extra2)
    
    // The prompt lists 4 deletes after CreateObjectReference.
    // My sequence:
    // 5. Delete objNode
    // 7. Delete refObjNode
    // 8. Delete targetObj
    // 9. Delete extra1
    // 10. Delete extra2
    // This matches the count.

    return 0;
}