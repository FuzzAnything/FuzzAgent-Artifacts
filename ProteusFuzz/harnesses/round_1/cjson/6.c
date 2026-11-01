// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
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
#include <cJSON.h>

int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    if (Data == NULL || Size == 0) {
        return 0;
    }

    // Step 1: cJSON_CreateFalse
    cJSON *item_false = cJSON_CreateFalse();
    if (item_false == NULL) {
        return 0;
    }

    // Step 2: cJSON_CreateString
    // Use input data as string content, ensuring null termination
    char *str_content = (char *)malloc(Size + 1);
    if (str_content == NULL) {
        cJSON_Delete(item_false);
        return 0;
    }
    memcpy(str_content, Data, Size);
    str_content[Size] = '\0';

    cJSON *item_string = cJSON_CreateString(str_content);
    free(str_content);

    if (item_string == NULL) {
        cJSON_Delete(item_false);
        return 0;
    }

    // Step 3: cJSON_CreateObject
    cJSON *item_obj = cJSON_CreateObject();
    if (item_obj == NULL) {
        cJSON_Delete(item_false);
        cJSON_Delete(item_string);
        return 0;
    }

    // Step 4: cJSON_CreateStringReference
    // Create a static string to reference
    static const char *static_str = "static_reference_string";
    cJSON *item_ref = cJSON_CreateStringReference(static_str);
    if (item_ref == NULL) {
        cJSON_Delete(item_false);
        cJSON_Delete(item_string);
        cJSON_Delete(item_obj);
        return 0;
    }

    // Step 5: cJSON_Delete (first call)
    // Clean up item_false as per the sequence
    cJSON_Delete(item_false);

    // Step 6: cJSON_CreateObjectReference
    // Create a reference to item_obj
    cJSON *item_obj_ref = cJSON_CreateObjectReference(item_obj);
    if (item_obj_ref == NULL) {
        cJSON_Delete(item_string);
        cJSON_Delete(item_obj);
        cJSON_Delete(item_ref);
        return 0;
    }

    // Step 7: cJSON_Delete (second call)
    // Clean up item_string
    cJSON_Delete(item_string);

    // Step 8: cJSON_Delete (third call)
    // Clean up item_obj_ref (reference, should not free item_obj)
    cJSON_Delete(item_obj_ref);

    // Step 9: cJSON_Delete (fourth call)
    // Clean up item_ref
    cJSON_Delete(item_ref);

    // Step 10: cJSON_Delete (fifth call)
    // Clean up item_obj (the actual object)
    cJSON_Delete(item_obj);

    return 0;
}