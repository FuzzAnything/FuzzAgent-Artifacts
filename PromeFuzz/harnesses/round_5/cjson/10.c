// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateFalse at cJSON.c:2483:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateStringReference at cJSON.c:2548:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObjectReference at cJSON.c:2560:23 in cJSON.h
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

int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    // Step 1: Create false value
    cJSON *false_item = cJSON_CreateFalse();
    if (false_item == NULL) {
        // If allocation fails, we can't continue with the sequence
        return 0;
    }

    // Step 2: Create string from fuzz data
    // Ensure we have a null-terminated string for cJSON_CreateString
    char *string1 = NULL;
    if (Size > 0) {
        string1 = (char *)malloc(Size + 1);
        if (string1 != NULL) {
            memcpy(string1, Data, Size);
            string1[Size] = '\0';
        }
    } else {
        // If no data, use empty string
        string1 = strdup("");
    }
    
    cJSON *string_item = cJSON_CreateString(string1);
    if (string1) {
        free(string1);
    }

    // Step 3: Create object
    cJSON *object_item = cJSON_CreateObject();

    // Step 4: Create string reference
    // Use a static string for the reference to avoid freeing issues
    static const char ref_string[] = "reference_string";
    cJSON *string_ref_item = cJSON_CreateStringReference(ref_string);

    // Step 5: Delete false item
    cJSON_Delete(false_item);

    // Step 6: Create object reference (reference to the object created in step 3)
    cJSON *object_ref_item = NULL;
    if (object_item != NULL) {
        object_ref_item = cJSON_CreateObjectReference(object_item);
    }

    // Step 7: Delete string item
    cJSON_Delete(string_item);

    // Step 8: Delete object item
    cJSON_Delete(object_item);

    // Step 9: Delete string reference item
    cJSON_Delete(string_ref_item);

    // Step 10: Delete object reference item
    cJSON_Delete(object_ref_item);

    return 0;
}