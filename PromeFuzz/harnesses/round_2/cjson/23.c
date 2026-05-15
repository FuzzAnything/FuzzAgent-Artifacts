// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateTrue at cJSON.c:2472:23 in cJSON.h
// cJSON_CreateFalse at cJSON.c:2483:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddStringToObject at cJSON.c:2210:22 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateTrue at cJSON.c:2472:23 in cJSON.h
// cJSON_CreateFalse at cJSON.c:2483:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddStringToObject at cJSON.c:2210:22 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddStringToObject at cJSON.c:2210:22 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_CreateTrue at cJSON.c:2472:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateTrue at cJSON.c:2472:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateFalse at cJSON.c:2483:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_IsInvalid at cJSON.c:2972:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_IsInvalid at cJSON.c:2972:26 in cJSON.h
// cJSON_CreateTrue at cJSON.c:2472:23 in cJSON.h
// cJSON_CreateFalse at cJSON.c:2483:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_IsString at cJSON.c:3032:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_IsString at cJSON.c:3032:26 in cJSON.h
// cJSON_CreateFalse at cJSON.c:2483:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateTrue at cJSON.c:2472:23 in cJSON.h
// cJSON_CreateFalse at cJSON.c:2483:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void fuzz_cJSON_IsString(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    // Create different types of cJSON items based on input
    cJSON *item = NULL;
    unsigned char first_byte = data[0];
    
    switch (first_byte % 7) {
        case 0:
            item = cJSON_CreateString((char*)data);
            break;
        case 1:
            item = cJSON_CreateNumber((double)size);
            break;
        case 2:
            item = cJSON_CreateTrue();
            break;
        case 3:
            item = cJSON_CreateFalse();
            break;
        case 4:
            item = cJSON_CreateNull();
            break;
        case 5:
            item = cJSON_CreateArray();
            break;
        case 6:
            item = cJSON_CreateObject();
            break;
    }
    
    if (item) {
        cJSON_IsString(item);
        cJSON_Delete(item);
    }
    
    // Test with NULL
    cJSON_IsString(NULL);
}

static void fuzz_cJSON_CreateFalse(void) {
    cJSON *false_item = cJSON_CreateFalse();
    if (false_item) {
        cJSON_Delete(false_item);
    }
}

static void fuzz_cJSON_IsTrue(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    cJSON *item = NULL;
    unsigned char first_byte = data[0];
    
    switch (first_byte % 4) {
        case 0:
            item = cJSON_CreateTrue();
            break;
        case 1:
            item = cJSON_CreateFalse();
            break;
        case 2:
            item = cJSON_CreateNumber(1.0);
            break;
        case 3:
            item = cJSON_CreateString("true");
            break;
    }
    
    if (item) {
        cJSON_IsTrue(item);
        cJSON_Delete(item);
    }
    
    // Test with NULL
    cJSON_IsTrue(NULL);
}

static void fuzz_cJSON_Compare(const uint8_t *data, size_t size) {
    if (size < 2) return;
    
    // Create various cJSON items
    cJSON *item1 = NULL;
    cJSON *item2 = NULL;
    
    unsigned char first_byte = data[0];
    unsigned char second_byte = data[1];
    cJSON_bool case_sensitive = (first_byte & 0x01) ? 1 : 0;
    
    // Create first item
    switch (first_byte % 6) {
        case 0:
            item1 = cJSON_CreateString((char*)data);
            break;
        case 1:
            item1 = cJSON_CreateNumber((double)size);
            break;
        case 2:
            item1 = cJSON_CreateTrue();
            break;
        case 3:
            item1 = cJSON_CreateFalse();
            break;
        case 4:
            item1 = cJSON_CreateArray();
            if (item1 && size > 10) {
                cJSON_AddItemToArray(item1, cJSON_CreateNumber(data[2]));
                cJSON_AddItemToArray(item1, cJSON_CreateString((char*)data + 3));
            }
            break;
        case 5:
            item1 = cJSON_CreateObject();
            if (item1 && size > 20) {
                cJSON_AddStringToObject(item1, "key1", (char*)data + 4);
                cJSON_AddNumberToObject(item1, "key2", data[5]);
            }
            break;
    }
    
    // Create second item
    switch (second_byte % 6) {
        case 0:
            item2 = cJSON_CreateString((char*)data + (size/2));
            break;
        case 1:
            item2 = cJSON_CreateNumber((double)(size * 2));
            break;
        case 2:
            item2 = cJSON_CreateTrue();
            break;
        case 3:
            item2 = cJSON_CreateFalse();
            break;
        case 4:
            item2 = cJSON_CreateArray();
            if (item2 && size > 10) {
                cJSON_AddItemToArray(item2, cJSON_CreateNumber(data[6]));
                cJSON_AddItemToArray(item2, cJSON_CreateString((char*)data + 7));
            }
            break;
        case 5:
            item2 = cJSON_CreateObject();
            if (item2 && size > 20) {
                cJSON_AddStringToObject(item2, "key1", (char*)data + 8);
                cJSON_AddNumberToObject(item2, "key2", data[9]);
            }
            break;
    }
    
    // Test comparison
    if (item1 && item2) {
        cJSON_Compare(item1, item2, case_sensitive);
        
        // Also test with same item
        cJSON_Compare(item1, item1, case_sensitive);
        cJSON_Compare(item2, item2, case_sensitive);
    }
    
    // Test with NULL combinations
    cJSON_Compare(NULL, item2, case_sensitive);
    cJSON_Compare(item1, NULL, case_sensitive);
    cJSON_Compare(NULL, NULL, case_sensitive);
    
    if (item1) cJSON_Delete(item1);
    if (item2) cJSON_Delete(item2);
}

static void fuzz_cJSON_Duplicate(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    cJSON *item = NULL;
    unsigned char first_byte = data[0];
    cJSON_bool recurse = (first_byte & 0x01) ? 1 : 0;
    
    // Create different types of items to duplicate
    switch ((first_byte >> 1) % 5) {
        case 0:
            item = cJSON_CreateString((char*)data);
            break;
        case 1:
            item = cJSON_CreateNumber((double)size);
            break;
        case 2:
            item = cJSON_CreateArray();
            if (item && size > 5) {
                for (size_t i = 0; i < 3 && i < size; i++) {
                    cJSON_AddItemToArray(item, cJSON_CreateNumber(data[i]));
                }
            }
            break;
        case 3:
            item = cJSON_CreateObject();
            if (item && size > 10) {
                cJSON_AddStringToObject(item, "key1", (char*)data + 1);
                cJSON_AddNumberToObject(item, "key2", data[2]);
            }
            break;
        case 4:
            item = cJSON_CreateTrue();
            break;
    }
    
    if (item) {
        cJSON *duplicate = cJSON_Duplicate(item, recurse);
        if (duplicate) {
            cJSON_Delete(duplicate);
        }
        cJSON_Delete(item);
    }
    
    // Test with NULL
    cJSON_Duplicate(NULL, recurse);
}

static void fuzz_cJSON_IsInvalid(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    // Create various cJSON items
    cJSON *item = NULL;
    unsigned char first_byte = data[0];
    
    switch (first_byte % 5) {
        case 0:
            item = cJSON_CreateString((char*)data);
            break;
        case 1:
            item = cJSON_CreateNumber((double)size);
            break;
        case 2:
            item = cJSON_CreateTrue();
            break;
        case 3:
            item = cJSON_CreateFalse();
            break;
        case 4:
            item = cJSON_CreateNull();
            break;
    }
    
    if (item) {
        cJSON_IsInvalid(item);
        cJSON_Delete(item);
    }
    
    // Test with NULL
    cJSON_IsInvalid(NULL);
}

int LLVMFuzzerTestOneInput_23(const uint8_t *data, size_t size) {
    // Call all fuzz functions
    fuzz_cJSON_IsString(data, size);
    fuzz_cJSON_CreateFalse();
    fuzz_cJSON_IsTrue(data, size);
    fuzz_cJSON_Compare(data, size);
    fuzz_cJSON_Duplicate(data, size);
    fuzz_cJSON_IsInvalid(data, size);
    
    return 0;
}