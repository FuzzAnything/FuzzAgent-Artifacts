// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateFalse at cJSON.c:2483:23 in cJSON.h
// cJSON_CreateTrue at cJSON.c:2472:23 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddBoolToObject at cJSON.c:2186:22 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsFalse at cJSON.c:2982:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateFalse at cJSON.c:2483:23 in cJSON.h
// cJSON_CreateFalse at cJSON.c:2483:23 in cJSON.h
// cJSON_IsFalse at cJSON.c:2982:26 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateTrue at cJSON.c:2472:23 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_IsFalse at cJSON.c:2982:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateFalse at cJSON.c:2483:23 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_IsFalse at cJSON.c:2982:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddBoolToObject at cJSON.c:2186:22 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddBoolToObject at cJSON.c:2186:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_CreateFalse at cJSON.c:2483:23 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateTrue at cJSON.c:2472:23 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_IsFalse at cJSON.c:2982:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddBoolToObject at cJSON.c:2186:22 in cJSON.h
// cJSON_AddBoolToObject at cJSON.c:2186:22 in cJSON.h
// cJSON_GetObjectItem at cJSON.c:1983:23 in cJSON.h
// cJSON_IsFalse at cJSON.c:2982:26 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_GetObjectItem at cJSON.c:1983:23 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsFalse at cJSON.c:2982:26 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "cJSON.h"

static void fuzz_boolean_functions(void) {
    /* Test cJSON_CreateFalse */
    cJSON *false_item = cJSON_CreateFalse();
    if (false_item != NULL) {
        /* Test cJSON_IsFalse on false item */
        (void)cJSON_IsFalse(false_item);
        
        /* Test cJSON_IsBool on false item */
        (void)cJSON_IsBool(false_item);
        
        /* Test cJSON_IsTrue on false item (should return false) */
        (void)cJSON_IsTrue(false_item);
        
        /* Create another false item for comparison */
        cJSON *false_item2 = cJSON_CreateFalse();
        if (false_item2 != NULL) {
            /* Test cJSON_Compare with case_sensitive = 0 */
            (void)cJSON_Compare(false_item, false_item2, 0);
            
            /* Test cJSON_Compare with case_sensitive = 1 */
            (void)cJSON_Compare(false_item, false_item2, 1);
            
            cJSON_Delete(false_item2);
        }
        
        /* Create a true item for comparison */
        cJSON *true_item = cJSON_CreateTrue();
        if (true_item != NULL) {
            /* Test cJSON_IsTrue on true item */
            (void)cJSON_IsTrue(true_item);
            
            /* Test cJSON_IsBool on true item */
            (void)cJSON_IsBool(true_item);
            
            /* Test cJSON_IsFalse on true item (should return false) */
            (void)cJSON_IsFalse(true_item);
            
            /* Compare false with true item */
            (void)cJSON_Compare(false_item, true_item, 0);
            (void)cJSON_Compare(false_item, true_item, 1);
            
            /* Create an object and add boolean to it */
            cJSON *object = cJSON_CreateObject();
            if (object != NULL) {
                /* Test cJSON_AddBoolToObject with false */
                (void)cJSON_AddBoolToObject(object, "false_key", 0);
                
                /* Test cJSON_AddBoolToObject with true */
                (void)cJSON_AddBoolToObject(object, "true_key", 1);
                
                /* Test boolean checks on object items */
                cJSON *false_obj_item = cJSON_GetObjectItem(object, "false_key");
                if (false_obj_item != NULL) {
                    (void)cJSON_IsFalse(false_obj_item);
                    (void)cJSON_IsBool(false_obj_item);
                }
                
                cJSON *true_obj_item = cJSON_GetObjectItem(object, "true_key");
                if (true_obj_item != NULL) {
                    (void)cJSON_IsTrue(true_obj_item);
                    (void)cJSON_IsBool(true_obj_item);
                }
                
                cJSON_Delete(object);
            }
            
            cJSON_Delete(true_item);
        }
        
        cJSON_Delete(false_item);
    }
}

static void fuzz_with_input(const uint8_t *data, size_t size) {
    if (size == 0) {
        return;
    }
    
    /* Use first byte to determine behavior */
    uint8_t mode = data[0] % 8;
    
    switch (mode) {
        case 0: {
            /* Test with NULL inputs for boolean checks */
            (void)cJSON_IsTrue(NULL);
            (void)cJSON_IsFalse(NULL);
            (void)cJSON_IsBool(NULL);
            (void)cJSON_Compare(NULL, NULL, 0);
            break;
        }
        case 1: {
            /* Create mixed items and compare */
            cJSON *false_item = cJSON_CreateFalse();
            cJSON *true_item = cJSON_CreateTrue();
            
            if (false_item && true_item) {
                /* Use second byte for case_sensitive if available */
                cJSON_bool case_sensitive = (size > 1) ? (data[1] % 2) : 0;
                (void)cJSON_Compare(false_item, true_item, case_sensitive);
                
                /* Compare item with itself */
                (void)cJSON_Compare(false_item, false_item, case_sensitive);
                (void)cJSON_Compare(true_item, true_item, case_sensitive);
            }
            
            if (false_item) cJSON_Delete(false_item);
            if (true_item) cJSON_Delete(true_item);
            break;
        }
        case 2: {
            /* Test AddBoolToObject with various names */
            cJSON *object = cJSON_CreateObject();
            if (object) {
                /* Create name from input data if available */
                char name[32];
                if (size > 1) {
                    size_t name_len = (size - 1) < 31 ? (size - 1) : 31;
                    memcpy(name, data + 1, name_len);
                    name[name_len] = '\0';
                } else {
                    strcpy(name, "test");
                }
                
                /* Use second byte for boolean value if available */
                cJSON_bool bool_val = (size > 1) ? (data[1] % 2) : 0;
                cJSON *added = cJSON_AddBoolToObject(object, name, bool_val);
                
                if (added) {
                    /* Test the added item */
                    (void)cJSON_IsBool(added);
                    if (bool_val) {
                        (void)cJSON_IsTrue(added);
                    } else {
                        (void)cJSON_IsFalse(added);
                    }
                }
                
                cJSON_Delete(object);
            }
            break;
        }
        case 3: {
            /* Create false item and test all boolean functions */
            cJSON *item = cJSON_CreateFalse();
            if (item) {
                (void)cJSON_IsFalse(item);
                (void)cJSON_IsBool(item);
                (void)cJSON_IsTrue(item);  /* Should return false */
                cJSON_Delete(item);
            }
            break;
        }
        case 4: {
            /* Create true item and test all boolean functions */
            cJSON *item = cJSON_CreateTrue();
            if (item) {
                (void)cJSON_IsTrue(item);
                (void)cJSON_IsBool(item);
                (void)cJSON_IsFalse(item);  /* Should return false */
                cJSON_Delete(item);
            }
            break;
        }
        case 5: {
            /* Test comparison with one NULL item */
            cJSON *item = cJSON_CreateFalse();
            if (item) {
                (void)cJSON_Compare(item, NULL, 0);
                (void)cJSON_Compare(NULL, item, 1);
                cJSON_Delete(item);
            }
            break;
        }
        case 6: {
            /* Test AddBoolToObject with NULL object */
            (void)cJSON_AddBoolToObject(NULL, "test", 0);
            break;
        }
        case 7: {
            /* Test AddBoolToObject with NULL name */
            cJSON *object = cJSON_CreateObject();
            if (object) {
                (void)cJSON_AddBoolToObject(object, NULL, 1);
                cJSON_Delete(object);
            }
            break;
        }
    }
}

int LLVMFuzzerTestOneInput_24(const uint8_t *data, size_t size) {
    /* Call the standard boolean function tests */
    fuzz_boolean_functions();
    
    /* Call the input-dependent tests */
    fuzz_with_input(data, size);
    
    return 0;
}