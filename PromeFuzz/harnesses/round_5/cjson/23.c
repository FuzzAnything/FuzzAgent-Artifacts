// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddFalseToObject at cJSON.c:2174:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddStringToObject at cJSON.c:2210:22 in cJSON.h
// cJSON_AddBoolToObject at cJSON.c:2186:22 in cJSON.h
// cJSON_AddNullToObject at cJSON.c:2150:22 in cJSON.h
// cJSON_AddNullToObject at cJSON.c:2150:22 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_ReplaceItemInObject at cJSON.c:2450:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddArrayToObject at cJSON.c:2246:22 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"

static char* extract_string(const uint8_t* data, size_t size, size_t* offset, size_t max_len) {
    if (*offset >= size) return NULL;
    
    size_t len = 0;
    while (len < max_len && *offset + len < size && data[*offset + len] != 0) {
        len++;
    }
    
    char* str = (char*)malloc(len + 1);
    if (str == NULL) return NULL;
    
    memcpy(str, data + *offset, len);
    str[len] = 0;
    *offset += len;
    
    if (*offset < size && data[*offset] == 0) {
        (*offset)++;
    }
    
    return str;
}

static void cleanup_strings(char** strings, int count) {
    for (int i = 0; i < count; i++) {
        if (strings[i]) free(strings[i]);
    }
}

int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;
    
    size_t offset = 0;
    char* strings[3] = {NULL, NULL, NULL};
    
    cJSON* root = cJSON_CreateObject();
    if (!root) return 0;
    
    uint8_t func_selector = Data[offset++] % 6;
    
    switch (func_selector) {
        case 0: {  // cJSON_AddStringToObject
            strings[0] = extract_string(Data, Size, &offset, 100);
            strings[1] = extract_string(Data, Size, &offset, 100);
            if (strings[0] && strings[1]) {
                cJSON* item = cJSON_AddStringToObject(root, strings[0], strings[1]);
                (void)item;
            }
            break;
        }
        
        case 1: {  // cJSON_AddBoolToObject
            strings[0] = extract_string(Data, Size, &offset, 100);
            if (strings[0]) {
                cJSON_bool bool_val = (offset < Size) ? (Data[offset++] % 2) : 0;
                cJSON* item = cJSON_AddBoolToObject(root, strings[0], bool_val);
                (void)item;
            }
            break;
        }
        
        case 2: {  // cJSON_AddNullToObject
            strings[0] = extract_string(Data, Size, &offset, 100);
            if (strings[0]) {
                cJSON* item = cJSON_AddNullToObject(root, strings[0]);
                (void)item;
            }
            break;
        }
        
        case 3: {  // cJSON_ReplaceItemInObject
            strings[0] = extract_string(Data, Size, &offset, 100);
            strings[1] = extract_string(Data, Size, &offset, 100);
            if (strings[0] && strings[1]) {
                cJSON* existing = cJSON_AddNullToObject(root, strings[0]);
                cJSON* new_item = cJSON_CreateString(strings[1]);
                if (existing && new_item) {
                    cJSON_bool success = cJSON_ReplaceItemInObject(root, strings[0], new_item);
                    (void)success;
                } else {
                    if (new_item) cJSON_Delete(new_item);
                }
            }
            break;
        }
        
        case 4: {  // cJSON_AddArrayToObject
            strings[0] = extract_string(Data, Size, &offset, 100);
            if (strings[0]) {
                cJSON* item = cJSON_AddArrayToObject(root, strings[0]);
                if (item) {
                    for (int i = 0; i < 3 && offset + i < Size; i++) {
                        cJSON_AddItemToArray(item, cJSON_CreateNumber(Data[offset + i]));
                    }
                }
                (void)item;
            }
            break;
        }
        
        case 5: {  // cJSON_AddFalseToObject
            strings[0] = extract_string(Data, Size, &offset, 100);
            if (strings[0]) {
                cJSON* item = cJSON_AddFalseToObject(root, strings[0]);
                (void)item;
            }
            break;
        }
    }
    
    cleanup_strings(strings, 3);
    cJSON_Delete(root);
    
    return 0;
}