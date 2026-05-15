// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateFalse at cJSON.c:2483:23 in cJSON.h
// cJSON_IsFalse at cJSON.c:2982:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddBoolToObject at cJSON.c:2186:22 in cJSON.h
// cJSON_CreateFalse at cJSON.c:2483:23 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_IsFalse at cJSON.c:2982:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_IsFalse at cJSON.c:2982:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_AddBoolToObject at cJSON.c:2186:22 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"

int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    uint8_t operation = Data[0];
    size_t data_index = 1;
    
    cJSON *false_item = cJSON_CreateFalse();
    if (false_item == NULL) {
        return 0;
    }
    
    cJSON_bool is_false = cJSON_IsFalse(false_item);
    cJSON_bool is_true = cJSON_IsTrue(false_item);
    cJSON_bool is_bool = cJSON_IsBool(false_item);
    
    cJSON *root_object = cJSON_CreateObject();
    if (root_object == NULL) {
        cJSON_Delete(false_item);
        return 0;
    }
    
    char name[51] = "fuzz_name";
    if (data_index < Size) {
        size_t name_len = (Size - data_index) > 50 ? 50 : (Size - data_index);
        memcpy(name, Data + data_index, name_len);
        name[name_len] = '\0';
        data_index += name_len;
    }
    
    cJSON_bool boolean_value = cJSON_False;
    if (data_index < Size) {
        boolean_value = (Data[data_index] & 1) ? cJSON_True : cJSON_False;
        data_index++;
    }
    
    cJSON *added_bool = cJSON_AddBoolToObject(root_object, name, boolean_value);
    
    cJSON *false_item2 = cJSON_CreateFalse();
    
    if (false_item2 != NULL) {
        cJSON_bool compare_result1 = cJSON_Compare(false_item, false_item2, 1);
        cJSON_bool compare_result2 = cJSON_Compare(false_item, false_item2, 0);
        
        if (added_bool != NULL) {
            cJSON_bool compare_result3 = cJSON_Compare(false_item, added_bool, 1);
            cJSON_bool compare_result4 = cJSON_Compare(false_item, added_bool, 0);
        }
        
        cJSON_bool compare_result5 = cJSON_Compare(root_object, root_object, 1);
        cJSON_bool compare_result6 = cJSON_Compare(root_object, root_object, 0);
        
        cJSON_Delete(false_item2);
    }
    
    if (added_bool != NULL) {
        cJSON_bool added_is_false = cJSON_IsFalse(added_bool);
        cJSON_bool added_is_true = cJSON_IsTrue(added_bool);
        cJSON_bool added_is_bool = cJSON_IsBool(added_bool);
    }
    
    switch (operation % 4) {
        case 0:
            cJSON_bool null_compare1 = cJSON_Compare(NULL, false_item, 1);
            cJSON_bool null_compare2 = cJSON_Compare(false_item, NULL, 0);
            cJSON_bool null_compare3 = cJSON_Compare(NULL, NULL, 1);
            break;
            
        case 1:
            cJSON_bool null_is_false = cJSON_IsFalse(NULL);
            cJSON_bool null_is_true = cJSON_IsTrue(NULL);
            cJSON_bool null_is_bool = cJSON_IsBool(NULL);
            break;
            
        case 2:
            if (data_index < Size) {
                char name2[51] = "fuzz_name2";
                if (data_index < Size) {
                    size_t name2_len = (Size - data_index) > 50 ? 50 : (Size - data_index);
                    memcpy(name2, Data + data_index, name2_len);
                    name2[name2_len] = '\0';
                }
                cJSON_bool boolean_value2 = (operation & 2) ? cJSON_True : cJSON_False;
                cJSON_AddBoolToObject(root_object, name2, boolean_value2);
            }
            break;
            
        case 3:
            if (root_object != NULL && false_item != NULL) {
                cJSON_bool compare_diff = cJSON_Compare(root_object, false_item, 1);
            }
            break;
    }
    
    cJSON_Delete(false_item);
    cJSON_Delete(root_object);
    
    return 0;
}