// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_GetObjectItem at cJSON.c:1983:23 in cJSON.h
// cJSON_GetObjectItem at cJSON.c:1983:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateStringReference at cJSON.c:2548:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_GetObjectItem at cJSON.c:1983:23 in cJSON.h
// cJSON_SetValuestring at cJSON.c:435:21 in cJSON.h
// cJSON_GetObjectItem at cJSON.c:1983:23 in cJSON.h
// cJSON_GetObjectItem at cJSON.c:1983:23 in cJSON.h
// cJSON_SetValuestring at cJSON.c:435:21 in cJSON.h
// cJSON_GetObjectItem at cJSON.c:1983:23 in cJSON.h
// cJSON_GetObjectItem at cJSON.c:1983:23 in cJSON.h
// cJSON_SetValuestring at cJSON.c:435:21 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cJSON.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void safe_delete(cJSON *item) {
    if (item) {
        cJSON_Delete(item);
    }
}

int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Ensure null termination for parsing
    char *json_str = (char *)malloc(Size + 1);
    if (!json_str) {
        return 0;
    }
    memcpy(json_str, Data, Size);
    json_str[Size] = '\0';

    // 1. cJSON_Parse
    cJSON *root = cJSON_Parse(json_str);
    free(json_str);

    if (!root || root->type != cJSON_Object) {
        safe_delete(root);
        return 0;
    }

    // 2. cJSON_CreateString
    // Use a small fixed string to avoid allocation issues with fuzz data
    cJSON *str_item = cJSON_CreateString("fuzz_test_value");
    if (!str_item) {
        safe_delete(root);
        return 0;
    }

    // 3. cJSON_CreateStringReference
    // Create a reference to a static string to ensure lifetime safety
    cJSON *ref_item = cJSON_CreateStringReference("static_reference");
    if (!ref_item) {
        safe_delete(str_item);
        safe_delete(root);
        return 0;
    }

    // 4. cJSON_AddItemToObject (first addition)
    if (!cJSON_AddItemToObject(root, "key1", str_item)) {
        safe_delete(str_item);
        safe_delete(ref_item);
        safe_delete(root);
        return 0;
    }

    // 5. cJSON_AddItemToObject (second addition)
    if (!cJSON_AddItemToObject(root, "key2", ref_item)) {
        safe_delete(root); // This will clean up key1 (str_item)
        safe_delete(ref_item);
        return 0;
    }

    // 6. cJSON_SetValuestring
    // Attempt to modify the string value of key1
    cJSON *item1 = cJSON_GetObjectItem(root, "key1");
    if (item1 && item1->type == cJSON_String) {
        cJSON_SetValuestring(item1, "new_fuzz_value");
    }

    // 7. cJSON_GetObjectItem (first retrieval)
    cJSON *retrieved1 = cJSON_GetObjectItem(root, "key1");

    // 8. cJSON_GetObjectItem (second retrieval)
    cJSON *retrieved2 = cJSON_GetObjectItem(root, "key2");

    // 9. cJSON_SetValuestring (second modification attempt)
    // Try to modify key2 if it's a string (it should be, but it's a reference)
    // This might fail or behave differently depending on implementation details regarding references
    if (retrieved2 && retrieved2->type == cJSON_String) {
        cJSON_SetValuestring(retrieved2, "modified_ref_value");
    }

    // 10. cJSON_GetObjectItem (third retrieval)
    cJSON *retrieved3 = cJSON_GetObjectItem(root, "key1");

    // 11. cJSON_GetObjectItem (fourth retrieval)
    cJSON *retrieved4 = cJSON_GetObjectItem(root, "key2");

    // 12. cJSON_SetValuestring (third modification attempt)
    if (retrieved3 && retrieved3->type == cJSON_String) {
        cJSON_SetValuestring(retrieved3, "final_value");
    }

    // 13. cJSON_GetObjectItem (fifth retrieval)
    cJSON *retrieved5 = cJSON_GetObjectItem(root, "key1");

    // 14. cJSON_GetObjectItem (sixth retrieval)
    cJSON *retrieved6 = cJSON_GetObjectItem(root, "key2");

    // 15. cJSON_Delete
    safe_delete(root);

    return 0;
}