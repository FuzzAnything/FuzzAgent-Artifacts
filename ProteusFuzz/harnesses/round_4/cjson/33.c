// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsNull at cJSON.c:3012:26 in cJSON.h
// cJSON_IsFalse at cJSON.c:2982:26 in cJSON.h
// cJSON_IsInvalid at cJSON.c:2972:26 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsNull at cJSON.c:3012:26 in cJSON.h
// cJSON_IsFalse at cJSON.c:2982:26 in cJSON.h
// cJSON_IsInvalid at cJSON.c:2972:26 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateTrue at cJSON.c:2472:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateFalse at cJSON.c:2483:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateRaw at cJSON.c:2581:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cJSON.h>

static cJSON *create_random_cjson(const uint8_t *data, size_t size) {
    cJSON *root = NULL;
    cJSON *item = NULL;

    if (size == 0) {
        return NULL;
    }

    // Ensure the input data is null-terminated before passing to cJSON_Parse
    // We create a buffer with size + 1 to safely add the null terminator
    char *safe_data = (char *)malloc(size + 1);
    if (!safe_data) {
        return NULL;
    }

    memcpy(safe_data, data, size);
    safe_data[size] = '\0';

    // Try to parse the input as JSON first
    root = cJSON_Parse(safe_data);
    free(safe_data);

    if (root) {
        return root;
    }

    // If parsing fails, try to manually construct a cJSON item based on input data
    // This helps explore more states by creating items with specific types
    item = cJSON_CreateNull();
    if (!item) return NULL;

    // Use input data to randomly set type and other fields
    // Ensure we check bounds before accessing data[1]
    if (size >= 1) {
        int type_choice = data[0] % 10;
        switch (type_choice) {
            case 0:
                cJSON_Delete(item);
                item = cJSON_CreateTrue();
                break;
            case 1:
                cJSON_Delete(item);
                item = cJSON_CreateFalse();
                break;
            case 2:
                cJSON_Delete(item);
                // Check if size >= 2 before accessing data[1]
                if (size >= 2) {
                    item = cJSON_CreateNumber(data[1] % 100);
                } else {
                    item = cJSON_CreateNumber(0);
                }
                break;
            case 3:
                cJSON_Delete(item);
                item = cJSON_CreateString("test");
                break;
            case 4:
                cJSON_Delete(item);
                item = cJSON_CreateArray();
                break;
            case 5:
                cJSON_Delete(item);
                item = cJSON_CreateObject();
                break;
            case 6:
                cJSON_Delete(item);
                item = cJSON_CreateNull();
                break;
            case 7:
                cJSON_Delete(item);
                item = cJSON_CreateRaw("raw");
                break;
            case 8:
                cJSON_Delete(item);
                item = cJSON_CreateNull();
                break;
            case 9:
                cJSON_Delete(item);
                item = cJSON_CreateNull();
                break;
        }
    }

    return item ? item : root;
}

int LLVMFuzzerTestOneInput_33(const uint8_t *Data, size_t Size) {
    cJSON *item = NULL;

    // Step 1: Prepare the environment
    // Create a cJSON item from the fuzzed input
    item = create_random_cjson(Data, Size);

    // If we couldn't create an item, try to create a NULL item explicitly to test NULL handling
    if (!item) {
        // Test with NULL pointer
        cJSON_IsTrue(NULL);
        cJSON_IsNull(NULL);
        cJSON_IsFalse(NULL);
        cJSON_IsInvalid(NULL);
        cJSON_IsBool(NULL);
        cJSON_IsArray(NULL);
        return 0;
    }

    // Step 2: Invoke target functions
    // Call all target API functions to explore different code paths
    cJSON_IsTrue(item);
    cJSON_IsNull(item);
    cJSON_IsFalse(item);
    cJSON_IsInvalid(item);
    cJSON_IsBool(item);
    cJSON_IsArray(item);

    // Step 3: Cleanup
    cJSON_Delete(item);

    return 0;
}