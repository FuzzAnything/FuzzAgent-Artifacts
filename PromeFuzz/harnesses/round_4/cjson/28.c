// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateFalse at cJSON.c:2483:23 in cJSON.h
// cJSON_IsFalse at cJSON.c:2982:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateTrue at cJSON.c:2472:23 in cJSON.h
// cJSON_IsFalse at cJSON.c:2982:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_IsFalse at cJSON.c:2982:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_IsFalse at cJSON.c:2982:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddBoolToObject at cJSON.c:2186:22 in cJSON.h
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

static cJSON* create_cjson_from_data(const uint8_t *data, size_t size) {
    if (size == 0) {
        return NULL;
    }
    
    char *json_str = (char*)malloc(size + 1);
    if (!json_str) {
        return NULL;
    }
    
    memcpy(json_str, data, size);
    json_str[size] = '\0';
    
    cJSON *json = cJSON_Parse(json_str);
    free(json_str);
    return json;
}

static void test_comparison(const uint8_t *data, size_t size) {
    if (size < 2) {
        return;
    }
    
    size_t split = size / 2;
    cJSON *a = create_cjson_from_data(data, split);
    cJSON *b = create_cjson_from_data(data + split, size - split);
    
    if (a && b) {
        cJSON_bool case_sensitive = (data[0] & 0x01) ? 1 : 0;
        cJSON_Compare(a, b, case_sensitive);
    }
    
    if (a) cJSON_Delete(a);
    if (b) cJSON_Delete(b);
}

static void test_boolean_functions(void) {
    cJSON *false_item = cJSON_CreateFalse();
    if (false_item) {
        cJSON_IsFalse(false_item);
        cJSON_IsTrue(false_item);
        cJSON_IsBool(false_item);
        cJSON_Delete(false_item);
    }
    
    cJSON *true_item = cJSON_CreateTrue();
    if (true_item) {
        cJSON_IsFalse(true_item);
        cJSON_IsTrue(true_item);
        cJSON_IsBool(true_item);
        cJSON_Delete(true_item);
    }
    
    cJSON *null_item = NULL;
    cJSON_IsFalse(null_item);
    cJSON_IsTrue(null_item);
    cJSON_IsBool(null_item);
    
    cJSON *number_item = cJSON_CreateNumber(42);
    if (number_item) {
        cJSON_IsFalse(number_item);
        cJSON_IsTrue(number_item);
        cJSON_IsBool(number_item);
        cJSON_Delete(number_item);
    }
}

static void test_add_bool_to_object(const uint8_t *data, size_t size) {
    cJSON *object = cJSON_CreateObject();
    if (!object) {
        return;
    }
    
    char name[64];
    size_t name_len = size < 63 ? size : 63;
    if (name_len > 0) {
        memcpy(name, data, name_len);
        name[name_len] = '\0';
    } else {
        strcpy(name, "test");
    }
    
    cJSON_bool bool_val = (data[0] & 0x01) ? 1 : 0;
    cJSON *added = cJSON_AddBoolToObject(object, name, bool_val);
    (void)added;
    
    cJSON_Delete(object);
}

int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    test_comparison(Data, Size);
    test_boolean_functions();
    test_add_bool_to_object(Data, Size);
    
    return 0;
}