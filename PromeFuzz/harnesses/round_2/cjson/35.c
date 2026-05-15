// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateTrue at cJSON.c:2472:23 in cJSON.h
// cJSON_CreateFalse at cJSON.c:2483:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_CreateFalse at cJSON.c:2483:23 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateTrue at cJSON.c:2472:23 in cJSON.h
// cJSON_CreateFalse at cJSON.c:2483:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddBoolToObject at cJSON.c:2186:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateFalse at cJSON.c:2483:23 in cJSON.h
// cJSON_CreateTrue at cJSON.c:2472:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_IsFalse at cJSON.c:2982:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_IsFalse at cJSON.c:2982:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateFalse at cJSON.c:2483:23 in cJSON.h
// cJSON_CreateFalse at cJSON.c:2483:23 in cJSON.h
// cJSON_CreateTrue at cJSON.c:2472:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void fuzz_cJSON_CreateFalse(void) {
    cJSON *false_item = cJSON_CreateFalse();
    if (false_item != NULL) {
        cJSON_Delete(false_item);
    }
}

static void fuzz_cJSON_IsFalse(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    cJSON *items[4];
    items[0] = cJSON_CreateFalse();
    items[1] = cJSON_CreateTrue();
    items[2] = cJSON_CreateNull();
    items[3] = cJSON_CreateNumber(42.0);
    
    for (int i = 0; i < 4; i++) {
        if (items[i] != NULL) {
            cJSON_bool result = cJSON_IsFalse(items[i]);
            (void)result;
            cJSON_Delete(items[i]);
        }
    }
    
    cJSON *parsed = NULL;
    if (size > 0) {
        char *json_string = (char *)malloc(size + 1);
        if (json_string != NULL) {
            memcpy(json_string, data, size);
            json_string[size] = '\0';
            parsed = cJSON_Parse(json_string);
            free(json_string);
            
            if (parsed != NULL) {
                cJSON_bool result = cJSON_IsFalse(parsed);
                (void)result;
                cJSON_Delete(parsed);
            }
        }
    }
}

static void fuzz_cJSON_Compare(const uint8_t *data, size_t size) {
    if (size < 2) return;
    
    cJSON_bool case_sensitive = (data[0] & 1) ? 1 : 0;
    
    cJSON *item1 = cJSON_CreateFalse();
    cJSON *item2 = cJSON_CreateFalse();
    cJSON *item3 = cJSON_CreateTrue();
    cJSON *item4 = cJSON_CreateNull();
    
    if (item1 != NULL && item2 != NULL) {
        cJSON_bool result = cJSON_Compare(item1, item2, case_sensitive);
        (void)result;
    }
    
    if (item1 != NULL && item3 != NULL) {
        cJSON_bool result = cJSON_Compare(item1, item3, case_sensitive);
        (void)result;
    }
    
    if (item1 != NULL && item4 != NULL) {
        cJSON_bool result = cJSON_Compare(item1, item4, case_sensitive);
        (void)result;
    }
    
    cJSON_Delete(item1);
    cJSON_Delete(item2);
    cJSON_Delete(item3);
    cJSON_Delete(item4);
    
    if (size > 0) {
        char *json_string = (char *)malloc(size + 1);
        if (json_string != NULL) {
            memcpy(json_string, data, size);
            json_string[size] = '\0';
            
            cJSON *parsed1 = cJSON_Parse(json_string);
            cJSON *parsed2 = cJSON_Parse(json_string);
            
            if (parsed1 != NULL && parsed2 != NULL) {
                cJSON_bool result = cJSON_Compare(parsed1, parsed2, case_sensitive);
                (void)result;
            }
            
            cJSON_Delete(parsed1);
            cJSON_Delete(parsed2);
            free(json_string);
        }
    }
}

static void fuzz_cJSON_IsTrue(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    cJSON *items[4];
    items[0] = cJSON_CreateTrue();
    items[1] = cJSON_CreateFalse();
    items[2] = cJSON_CreateNull();
    items[3] = cJSON_CreateNumber(42.0);
    
    for (int i = 0; i < 4; i++) {
        if (items[i] != NULL) {
            cJSON_bool result = cJSON_IsTrue(items[i]);
            (void)result;
            cJSON_Delete(items[i]);
        }
    }
    
    cJSON *parsed = NULL;
    if (size > 0) {
        char *json_string = (char *)malloc(size + 1);
        if (json_string != NULL) {
            memcpy(json_string, data, size);
            json_string[size] = '\0';
            parsed = cJSON_Parse(json_string);
            free(json_string);
            
            if (parsed != NULL) {
                cJSON_bool result = cJSON_IsTrue(parsed);
                (void)result;
                cJSON_Delete(parsed);
            }
        }
    }
}

static void fuzz_cJSON_IsBool(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    cJSON *items[5];
    items[0] = cJSON_CreateTrue();
    items[1] = cJSON_CreateFalse();
    items[2] = cJSON_CreateNull();
    items[3] = cJSON_CreateNumber(42.0);
    items[4] = cJSON_CreateString("test");
    
    for (int i = 0; i < 5; i++) {
        if (items[i] != NULL) {
            cJSON_bool result = cJSON_IsBool(items[i]);
            (void)result;
            cJSON_Delete(items[i]);
        }
    }
    
    cJSON *parsed = NULL;
    if (size > 0) {
        char *json_string = (char *)malloc(size + 1);
        if (json_string != NULL) {
            memcpy(json_string, data, size);
            json_string[size] = '\0';
            parsed = cJSON_Parse(json_string);
            free(json_string);
            
            if (parsed != NULL) {
                cJSON_bool result = cJSON_IsBool(parsed);
                (void)result;
                cJSON_Delete(parsed);
            }
        }
    }
}

static void fuzz_cJSON_AddBoolToObject(const uint8_t *data, size_t size) {
    if (size < 2) return;
    
    cJSON *object = cJSON_CreateObject();
    if (object == NULL) return;
    
    size_t name_len = size / 2;
    char *name = (char *)malloc(name_len + 1);
    if (name == NULL) {
        cJSON_Delete(object);
        return;
    }
    
    memcpy(name, data, name_len);
    name[name_len] = '\0';
    
    cJSON_bool boolean = (data[name_len] & 1) ? 1 : 0;
    
    cJSON *added_item = cJSON_AddBoolToObject(object, name, boolean);
    (void)added_item;
    
    free(name);
    cJSON_Delete(object);
}

int LLVMFuzzerTestOneInput_35(const uint8_t *data, size_t size) {
    if (size == 0) return 0;
    
    fuzz_cJSON_CreateFalse();
    fuzz_cJSON_IsFalse(data, size);
    fuzz_cJSON_Compare(data, size);
    fuzz_cJSON_IsTrue(data, size);
    fuzz_cJSON_IsBool(data, size);
    fuzz_cJSON_AddBoolToObject(data, size);
    
    return 0;
}