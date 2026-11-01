// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateIntArray at cJSON.c:2621:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_AddItemReferenceToArray at cJSON.c:2130:26 in cJSON.h
// cJSON_DeleteItemFromArray at cJSON.c:2304:20 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemReferenceToArray at cJSON.c:2130:26 in cJSON.h
// cJSON_AddItemReferenceToArray at cJSON.c:2130:26 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_DeleteItemFromArray at cJSON.c:2304:20 in cJSON.h
// cJSON_DeleteItemFromArray at cJSON.c:2304:20 in cJSON.h
// cJSON_CreateDoubleArray at cJSON.c:2701:23 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cJSON.h>

static void cleanup_cjson(cJSON *root) {
    if (root) {
        cJSON_Delete(root);
    }
}

int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Use the input data to drive fuzzing logic
    uint8_t mode = Data[0] % 6;
    
    cJSON *array = NULL;
    cJSON *item = NULL;
    int count = 0;
    
    // Determine count based on input, ensuring it's reasonable
    if (Size >= 2) {
        count = (Data[1] % 10) + 1; // 1 to 10
    } else {
        count = 1;
    }

    switch (mode) {
        case 0: {
            // Test cJSON_CreateArray and cJSON_AddItemToArray
            array = cJSON_CreateArray();
            if (array) {
                item = cJSON_CreateNumber(1.0);
                if (item) {
                    cJSON_AddItemToArray(array, item);
                }
                // Add more items if count allows
                for (int i = 1; i < count; i++) {
                    item = cJSON_CreateNumber((double)i);
                    if (item) {
                        cJSON_AddItemToArray(array, item);
                    }
                }
            }
            break;
        }
        case 1: {
            // Test cJSON_AddItemReferenceToArray
            array = cJSON_CreateArray();
            if (array) {
                item = cJSON_CreateNumber(42.0);
                if (item) {
                    cJSON_AddItemReferenceToArray(array, item);
                    // Try adding the same reference again
                    cJSON_AddItemReferenceToArray(array, item);
                }
            }
            break;
        }
        case 2: {
            // Test cJSON_DeleteItemFromArray
            array = cJSON_CreateArray();
            if (array) {
                for (int i = 0; i < count; i++) {
                    cJSON *num = cJSON_CreateNumber((double)i);
                    if (num) {
                        cJSON_AddItemToArray(array, num);
                    }
                }
                // Delete items based on input data
                int index = 0;
                if (Size >= 2) {
                    index = Data[1] % (count > 0 ? count : 1);
                }
                cJSON_DeleteItemFromArray(array, index);
                // Try deleting out of bounds
                cJSON_DeleteItemFromArray(array, count + 10);
            }
            break;
        }
        case 3: {
            // Test cJSON_CreateDoubleArray
            double *doubles = (double *)malloc(count * sizeof(double));
            if (doubles) {
                for (int i = 0; i < count; i++) {
                    doubles[i] = (double)(i * 1.5);
                }
                array = cJSON_CreateDoubleArray(doubles, count);
                free(doubles);
            }
            break;
        }
        case 4: {
            // Test cJSON_CreateIntArray
            int *ints = (int *)malloc(count * sizeof(int));
            if (ints) {
                for (int i = 0; i < count; i++) {
                    ints[i] = i * 10;
                }
                array = cJSON_CreateIntArray(ints, count);
                free(ints);
            }
            break;
        }
        case 5: {
            // Combined test: Create array, add items, delete, add reference
            array = cJSON_CreateArray();
            if (array) {
                // Add some items
                for (int i = 0; i < count; i++) {
                    cJSON *num = cJSON_CreateNumber((double)i);
                    if (num) {
                        cJSON_AddItemToArray(array, num);
                    }
                }
                
                // Create a separate item to reference
                item = cJSON_CreateString("test");
                if (item) {
                    cJSON_AddItemReferenceToArray(array, item);
                }
                
                // Delete an item
                if (count > 0) {
                    cJSON_DeleteItemFromArray(array, 0);
                }
            }
            break;
        }
    }

    // Cleanup
    cleanup_cjson(array);
    
    return 0;
}