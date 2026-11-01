// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateFalse at cJSON.c:2483:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateStringReference at cJSON.c:2548:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObjectReference at cJSON.c:2560:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"

int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size) {
    cJSON *false_node = NULL;
    cJSON *string_node = NULL;
    cJSON *object_node = NULL;
    cJSON *string_ref_node = NULL;
    cJSON *object_ref_node = NULL;
    const char *test_string = "fuzz_test_string";

    if (Size > 0 && Data[0] == 0) {
        test_string = NULL;
    }

    // 1. cJSON_CreateFalse
    false_node = cJSON_CreateFalse();
    if (false_node == NULL) {
        return 0;
    }

    // 2. cJSON_CreateString
    string_node = cJSON_CreateString(test_string);
    if (string_node == NULL) {
        cJSON_Delete(false_node);
        return 0;
    }

    // 3. cJSON_CreateObject
    object_node = cJSON_CreateObject();
    if (object_node == NULL) {
        cJSON_Delete(false_node);
        cJSON_Delete(string_node);
        return 0;
    }

    // 4. cJSON_CreateStringReference
    string_ref_node = cJSON_CreateStringReference(test_string);
    if (string_ref_node == NULL) {
        cJSON_Delete(false_node);
        cJSON_Delete(string_node);
        cJSON_Delete(object_node);
        return 0;
    }

    // 5. cJSON_Delete (false_node)
    cJSON_Delete(false_node);

    // 6. cJSON_CreateObjectReference (reference to object_node)
    object_ref_node = cJSON_CreateObjectReference(object_node);
    if (object_ref_node == NULL) {
        cJSON_Delete(string_node);
        cJSON_Delete(object_node);
        cJSON_Delete(string_ref_node);
        return 0;
    }

    // 7. cJSON_Delete (string_node)
    cJSON_Delete(string_node);

    // 8. cJSON_Delete (object_node)
    // Note: This deletes the original object. object_ref_node is a reference to it.
    // Depending on cJSON implementation, this might leave object_ref_node pointing to freed memory.
    // However, we follow the required sequence strictly.
    cJSON_Delete(object_node);

    // 9. cJSON_Delete (string_ref_node)
    cJSON_Delete(string_ref_node);

    // 10. cJSON_Delete (object_ref_node)
    // This attempts to delete the reference node.
    // Since it is a reference, cJSON_Delete should not free the child (which is already freed).
    // It should only free the reference node structure itself.
    cJSON_Delete(object_ref_node);

    return 0;
}