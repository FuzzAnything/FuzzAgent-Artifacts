// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateArrayReference at cJSON.c:2571:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cJSON.h>

int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    if (Data == NULL || Size == 0) {
        return 0;
    }

    // Step 1: Create an empty array
    cJSON *array = cJSON_CreateArray();
    if (array == NULL) {
        return 0;
    }

    // Step 2: Create a number from input data
    double num = (double)Data[0];
    if (Size > 1) {
        num += (double)Data[1] * 0.1;
    }
    cJSON *numberItem = cJSON_CreateNumber(num);
    if (numberItem == NULL) {
        cJSON_Delete(array);
        return 0;
    }

    // Step 3: Add the number item to the array
    // cJSON_AddItemToArray takes ownership of the item, so we should not delete numberItem manually later.
    if (!cJSON_AddItemToArray(array, numberItem)) {
        cJSON_Delete(array);
        cJSON_Delete(numberItem);
        return 0;
    }

    // Step 4: Create an array reference to the number item
    // This creates a new node that references numberItem.
    cJSON *refItem = cJSON_CreateArrayReference(numberItem);
    
    // Step 5: Delete the reference item if it was created.
    // refItem is a separate node from numberItem. Deleting it frees the refItem node itself.
    // It does NOT free numberItem because it's a reference.
    if (refItem != NULL) {
        cJSON_Delete(refItem);
    }

    // Step 6: Delete the main array.
    // This will recursively delete the children of 'array'.
    // 'numberItem' is a child of 'array', so it will be freed here.
    // This is safe because we did not delete numberItem manually, and refItem was already deleted.
    cJSON_Delete(array);

    return 0;
}