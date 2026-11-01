// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateRaw at cJSON.c:2581:23 in cJSON.h
// cJSON_AddItemToObjectCS at cJSON.c:2125:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateRaw at cJSON.c:2581:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddRawToObject at cJSON.c:2222:22 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_DetachItemFromObject at cJSON.c:2309:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_DeleteItemFromObject at cJSON.c:2323:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <cJSON.h>

static void cleanup_cjson(cJSON *root) {
    if (root) {
        cJSON_Delete(root);
    }
}

int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Ensure the input data is treated as a null-terminated string */
    char *safe_str = (char *)malloc(Size + 1);
    if (!safe_str) {
        return 0;
    }
    memcpy(safe_str, Data, Size);
    safe_str[Size] = '\0';

    /* Create a base object to work with */
    cJSON *obj = cJSON_CreateObject();
    if (!obj) {
        free(safe_str);
        return 0;
    }

    /* Fuzz cJSON_AddItemToObjectCS */
    cJSON *item1 = cJSON_CreateRaw(safe_str);
    if (item1) {
        cJSON_bool res = cJSON_AddItemToObjectCS(obj, safe_str, item1);
        if (!res) {
            cJSON_Delete(item1);
        }
    }

    /* Fuzz cJSON_CreateRaw and cJSON_AddItemToObject */
    cJSON *raw_item = cJSON_CreateRaw(safe_str);
    if (raw_item) {
        cJSON_bool res2 = cJSON_AddItemToObject(obj, "raw_key", raw_item);
        if (!res2) {
            cJSON_Delete(raw_item);
        }
    }

    /* Fuzz cJSON_AddRawToObject */
    /* This function handles its own allocation and deletion on failure */
    cJSON_AddRawToObject(obj, "raw_obj_key", safe_str);

    /* Fuzz cJSON_AddItemToObject with a number */
    cJSON *item2 = cJSON_CreateNumber(123);
    if (item2) {
        cJSON_bool res3 = cJSON_AddItemToObject(obj, "num_key", item2);
        if (!res3) {
            cJSON_Delete(item2);
        }
    }

    /* Fuzz cJSON_DetachItemFromObject */
    /* Detach an item to test the detach logic */
    cJSON *detached = cJSON_DetachItemFromObject(obj, "raw_key");
    if (detached) {
        /* Try to reattach it under a new name */
        cJSON_bool res4 = cJSON_AddItemToObject(obj, "reattached", detached);
        if (!res4) {
            /* If reattachment fails, we must delete the detached item to avoid leaks */
            cJSON_Delete(detached);
        }
    }

    /* Fuzz cJSON_DeleteItemFromObject */
    cJSON_DeleteItemFromObject(obj, "num_key");

    /* Cleanup the main object tree */
    cleanup_cjson(obj);
    free(safe_str);

    return 0;
}