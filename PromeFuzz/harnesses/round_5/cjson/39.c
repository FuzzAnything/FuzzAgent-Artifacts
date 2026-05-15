// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateFalse at cJSON.c:2483:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_IsRaw at cJSON.c:3062:26 in cJSON.h
// cJSON_IsString at cJSON.c:3032:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateRaw at cJSON.c:2581:23 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void fuzz_cJSON_IsTrue(const cJSON *item) {
    cJSON_IsTrue(item);
}

static void fuzz_cJSON_CreateFalse(void) {
    cJSON *false_item = cJSON_CreateFalse();
    if (false_item != NULL) {
        cJSON_Delete(false_item);
    }
}

static void fuzz_cJSON_Duplicate(const cJSON *item, cJSON_bool recurse) {
    cJSON *duplicate = cJSON_Duplicate(item, recurse);
    if (duplicate != NULL) {
        cJSON_Delete(duplicate);
    }
}

static void fuzz_cJSON_IsRaw(const cJSON *item) {
    cJSON_IsRaw(item);
}

static void fuzz_cJSON_IsString(const cJSON *item) {
    cJSON_IsString(item);
}

static void fuzz_cJSON_Compare(const cJSON *a, const cJSON *b, cJSON_bool case_sensitive) {
    cJSON_Compare(a, b, case_sensitive);
}

static cJSON *create_test_item_from_data(const uint8_t *data, size_t size) {
    if (size == 0) {
        return NULL;
    }
    
    size_t copy_size = size > 100 ? 100 : size;
    char *buffer = (char *)malloc(copy_size + 1);
    if (!buffer) {
        return NULL;
    }
    
    memcpy(buffer, data, copy_size);
    buffer[copy_size] = '\0';
    
    cJSON *item = cJSON_Parse(buffer);
    if (!item) {
        item = cJSON_CreateString(buffer);
    }
    
    free(buffer);
    return item;
}

static cJSON *create_raw_item(const uint8_t *data, size_t size) {
    if (size == 0) {
        return NULL;
    }
    
    size_t copy_size = size > 100 ? 100 : size;
    char *buffer = (char *)malloc(copy_size + 1);
    if (!buffer) {
        return NULL;
    }
    
    memcpy(buffer, data, copy_size);
    buffer[copy_size] = '\0';
    
    cJSON *item = cJSON_CreateRaw(buffer);
    free(buffer);
    return item;
}

int LLVMFuzzerTestOneInput_39(const uint8_t *data, size_t size) {
    if (size == 0) {
        return 0;
    }
    
    cJSON *item1 = create_test_item_from_data(data, size);
    cJSON *item2 = create_test_item_from_data(data + size/2, size/2);
    cJSON *raw_item = create_raw_item(data, size);
    
    fuzz_cJSON_IsTrue(item1);
    fuzz_cJSON_IsTrue(item2);
    fuzz_cJSON_IsTrue(raw_item);
    fuzz_cJSON_IsTrue(NULL);
    
    fuzz_cJSON_CreateFalse();
    
    if (item1) {
        fuzz_cJSON_Duplicate(item1, 0);
        fuzz_cJSON_Duplicate(item1, 1);
    }
    if (item2) {
        fuzz_cJSON_Duplicate(item2, 0);
        fuzz_cJSON_Duplicate(item2, 1);
    }
    fuzz_cJSON_Duplicate(NULL, 0);
    
    fuzz_cJSON_IsRaw(item1);
    fuzz_cJSON_IsRaw(item2);
    fuzz_cJSON_IsRaw(raw_item);
    fuzz_cJSON_IsRaw(NULL);
    
    fuzz_cJSON_IsString(item1);
    fuzz_cJSON_IsString(item2);
    fuzz_cJSON_IsString(raw_item);
    fuzz_cJSON_IsString(NULL);
    
    if (item1 && item2) {
        fuzz_cJSON_Compare(item1, item2, 0);
        fuzz_cJSON_Compare(item1, item2, 1);
        fuzz_cJSON_Compare(item1, item1, 0);
        fuzz_cJSON_Compare(item1, item1, 1);
    }
    fuzz_cJSON_Compare(NULL, item1, 0);
    fuzz_cJSON_Compare(item1, NULL, 0);
    fuzz_cJSON_Compare(NULL, NULL, 0);
    
    if (item1) {
        cJSON_Delete(item1);
    }
    if (item2) {
        cJSON_Delete(item2);
    }
    if (raw_item) {
        cJSON_Delete(raw_item);
    }
    
    return 0;
}