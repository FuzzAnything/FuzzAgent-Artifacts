// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddBoolToObject at cJSON.c:2186:22 in cJSON.h
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

static char* extract_string(const uint8_t* data, size_t size, size_t* consumed) {
    if (size == 0) {
        *consumed = 0;
        char* empty = (char*)malloc(1);
        if (empty) empty[0] = '\0';
        return empty;
    }
    
    size_t len = size > 256 ? 256 : size;
    char* str = (char*)malloc(len + 1);
    if (!str) return NULL;
    
    for (size_t i = 0; i < len; i++) {
        str[i] = data[i] ? data[i] : 'a';
    }
    str[len] = '\0';
    *consumed = len;
    return str;
}

int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;
    
    size_t consumed1 = 0, consumed2 = 0;
    char* name1 = extract_string(Data, Size, &consumed1);
    char* name2 = extract_string(Data + consumed1, Size - consumed1, &consumed2);
    
    if (!name1 || !name2) {
        free(name1);
        free(name2);
        return 0;
    }
    
    cJSON* object = cJSON_CreateObject();
    if (!object) {
        free(name1);
        free(name2);
        return 0;
    }
    
    cJSON_bool bool1 = (Data[0] & 1) ? 1 : 0;
    cJSON_bool bool2 = (Data[1] & 1) ? 1 : 0;
    
    cJSON_AddBoolToObject(object, name1, bool1);
    cJSON_AddBoolToObject(object, name2, bool2);
    
    cJSON_Delete(object);
    
    free(name1);
    free(name2);
    
    return 0;
}