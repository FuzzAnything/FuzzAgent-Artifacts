// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateFalse at cJSON.c:2483:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateStringReference at cJSON.c:2548:23 in cJSON.h
// cJSON_CreateObjectReference at cJSON.c:2560:23 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cJSON.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void safe_cleanup(cJSON *item) {
    if (item != NULL) {
        cJSON_Delete(item);
    }
}

int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Step 1: cJSON_CreateFalse
    cJSON *false_node = cJSON_CreateFalse();
    if (false_node == NULL) {
        return 0;
    }

    // Step 2: cJSON_CreateString
    // Ensure we have a null-terminated string from fuzz input
    char *str_buf = (char *)malloc(Size + 1);
    if (str_buf == NULL) {
        safe_cleanup(false_node);
        return 0;
    }
    memcpy(str_buf, Data, Size);
    str_buf[Size] = '\0';

    cJSON *string_node = cJSON_CreateString(str_buf);
    free(str_buf);

    if (string_node == NULL) {
        safe_cleanup(false_node);
        return 0;
    }

    // Step 3: cJSON_CreateObject
    cJSON *obj_node = cJSON_CreateObject();
    if (obj_node == NULL) {
        safe_cleanup(false_node);
        safe_cleanup(string_node);
        return 0;
    }

    // Step 4: cJSON_CreateStringReference
    // Create a static string buffer to reference
    static char ref_str[] = "reference_string";
    cJSON *ref_node = cJSON_CreateStringReference(ref_str);
    if (ref_node == NULL) {
        safe_cleanup(false_node);
        safe_cleanup(string_node);
        safe_cleanup(obj_node);
        return 0;
    }

    // Step 5: cJSON_Delete (on false_node)
    safe_cleanup(false_node);

    // Step 6: cJSON_CreateObjectReference
    // Create a reference to the string_node
    cJSON *obj_ref_node = cJSON_CreateObjectReference(string_node);
    if (obj_ref_node == NULL) {
        safe_cleanup(string_node);
        safe_cleanup(obj_node);
        safe_cleanup(ref_node);
        return 0;
    }

    // Step 7: cJSON_Delete (on string_node)
    // Note: string_node is now referenced by obj_ref_node, but we are deleting the original.
    // Depending on implementation, this might be safe if the reference doesn't own it.
    // However, the task asks to delete specific nodes in order.
    safe_cleanup(string_node);

    // Step 8: cJSON_Delete (on obj_node)
    safe_cleanup(obj_node);

    // Step 9: cJSON_Delete (on ref_node)
    safe_cleanup(ref_node);

    // Step 10: cJSON_Delete (on obj_ref_node)
    safe_cleanup(obj_ref_node);

    return 0;
}