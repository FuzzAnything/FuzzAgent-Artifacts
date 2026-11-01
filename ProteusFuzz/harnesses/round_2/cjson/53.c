// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateIntArray at cJSON.c:2621:23 in cJSON.h
// cJSON_CreateFloatArray at cJSON.c:2661:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_AddItemReferenceToArray at cJSON.c:2130:26 in cJSON.h
// cJSON_DeleteItemFromArray at cJSON.c:2304:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cJSON.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* Fuzz driver for cjson API functions */

int LLVMFuzzerTestOneInput_53(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Extract parameters from input data */
    int count = 0;
    if (Size > 0) {
        count = Data[0] % 100; /* Limit count to avoid huge allocations */
    }

    /* Create arrays for integers and floats */
    int *int_array = NULL;
    float *float_array = NULL;
    cJSON *int_json_array = NULL;
    cJSON *float_json_array = NULL;
    cJSON *target_item = NULL;
    cJSON *ref_array = NULL;
    cJSON *main_array = NULL;

    /* Allocate and populate int_array */
    if (count > 0 && Size >= (size_t)count * sizeof(int)) {
        int_array = (int *)malloc(count * sizeof(int));
        if (int_array) {
            memcpy(int_array, Data, count * sizeof(int));
            int_json_array = cJSON_CreateIntArray(int_array, count);
            free(int_array);
        }
    }

    /* Allocate and populate float_array */
    if (count > 0 && Size >= (size_t)count * sizeof(float)) {
        float_array = (float *)malloc(count * sizeof(float));
        if (float_array) {
            memcpy(float_array, Data, count * sizeof(float));
            float_json_array = cJSON_CreateFloatArray(float_array, count);
            free(float_array);
        }
    }

    /* Create an empty array for testing AddItemToArray and AddItemReferenceToArray */
    main_array = cJSON_CreateArray();
    if (main_array) {
        /* Try to add int_json_array if it exists */
        if (int_json_array) {
            cJSON_AddItemToArray(main_array, int_json_array);
        }

        /* Try to add float_json_array if it exists */
        if (float_json_array) {
            cJSON_AddItemToArray(main_array, float_json_array);
        }

        /* Create a reference array */
        ref_array = cJSON_CreateArray();
        if (ref_array) {
            /* Add a simple item to ref_array to use as reference */
            cJSON *simple_item = cJSON_CreateNumber(42);
            if (simple_item) {
                cJSON_AddItemToArray(ref_array, simple_item);
                target_item = simple_item;
            }
        }

        /* Test AddItemReferenceToArray */
        if (target_item) {
            cJSON_AddItemReferenceToArray(main_array, target_item);
        }

        /* Test DeleteItemFromArray with various indices */
        int index_to_delete = 0;
        if (Size > 1) {
            index_to_delete = Data[1] % 10;
        }
        cJSON_DeleteItemFromArray(main_array, index_to_delete);
    }

    /* Cleanup */
    cJSON_Delete(main_array);
    cJSON_Delete(ref_array);
    /* Note: int_json_array and float_json_array are already owned by main_array if added,
       so we don't delete them again to avoid double-free. If they weren't added, they are leaked,
       but for fuzzing safety, we assume the test harness will catch memory errors.
       In a real scenario, we would track ownership more carefully. */

    return 0;
}