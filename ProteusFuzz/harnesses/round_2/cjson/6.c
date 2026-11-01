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
#include <stdio.h>
#include <cJSON.h>

int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // 1. cJSON_CreateFalse
    cJSON *item_false = cJSON_CreateFalse();
    if (item_false == NULL) {
        return 0;
    }

    // 2. cJSON_CreateString
    // Use input data as string, ensuring null termination
    char *str_buf = (char *)malloc(Size + 1);
    if (str_buf == NULL) {
        cJSON_Delete(item_false);
        return 0;
    }
    memcpy(str_buf, Data, Size);
    str_buf[Size] = '\0';

    cJSON *item_str = cJSON_CreateString(str_buf);
    free(str_buf);

    if (item_str == NULL) {
        cJSON_Delete(item_false);
        return 0;
    }

    // 3. cJSON_CreateObject
    cJSON *item_obj = cJSON_CreateObject();
    if (item_obj == NULL) {
        cJSON_Delete(item_false);
        cJSON_Delete(item_str);
        return 0;
    }

    // 4. cJSON_CreateStringReference
    // Create a reference to a static string to test reference behavior
    const char *static_ref = "reference_string";
    cJSON *item_ref = cJSON_CreateStringReference(static_ref);
    if (item_ref == NULL) {
        cJSON_Delete(item_false);
        cJSON_Delete(item_str);
        cJSON_Delete(item_obj);
        return 0;
    }

    // 5. cJSON_Delete (First call in sequence)
    // Delete the false item
    cJSON_Delete(item_false);

    // 6. cJSON_CreateObjectReference
    // Create a reference to the object created in step 3
    cJSON *item_obj_ref = cJSON_CreateObjectReference(item_obj);
    if (item_obj_ref == NULL) {
        cJSON_Delete(item_str);
        cJSON_Delete(item_obj);
        cJSON_Delete(item_ref);
        return 0;
    }

    // 7. cJSON_Delete (Second call in sequence)
    // Delete the string item
    cJSON_Delete(item_str);

    // 8. cJSON_Delete (Third call in sequence)
    // Delete the string reference item
    cJSON_Delete(item_ref);

    // 9. cJSON_Delete (Fourth call in sequence)
    // Delete the object reference. This should NOT free item_obj.
    cJSON_Delete(item_obj_ref);

    // 10. cJSON_Delete (Fifth call in sequence)
    // Delete the original object. This should now free the memory.
    cJSON_Delete(item_obj);

    return 0;
}