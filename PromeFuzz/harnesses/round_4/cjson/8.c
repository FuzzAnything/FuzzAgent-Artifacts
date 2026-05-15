// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateFalse at cJSON.c:2483:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateStringReference at cJSON.c:2548:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObjectReference at cJSON.c:2560:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
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

static char* create_null_terminated_string(const uint8_t *data, size_t size) {
    if (size == 0) {
        char* str = (char*)malloc(1);
        if (str) str[0] = '\0';
        return str;
    }
    
    char* str = (char*)malloc(size + 1);
    if (!str) return NULL;
    
    memcpy(str, data, size);
    str[size] = '\0';
    return str;
}

int LLVMFuzzerTestOneInput_8(const uint8_t *data, size_t size) {
    // Create null-terminated string from fuzzer input
    char* fuzz_string = create_null_terminated_string(data, size);
    if (!fuzz_string) return 0;
    
    // 1. Create false value
    cJSON* false_item = cJSON_CreateFalse();
    
    // 2. Create string from fuzzer input
    cJSON* string_item = cJSON_CreateString(fuzz_string);
    
    // 3. Create empty object
    cJSON* object_item = cJSON_CreateObject();
    
    // 4. Create string reference using fuzzer input
    cJSON* string_ref_item = cJSON_CreateStringReference(fuzz_string);
    
    // 5. Delete false item
    if (false_item) cJSON_Delete(false_item);
    
    // 6. Create object reference
    cJSON* object_ref_item = cJSON_CreateObjectReference(object_item);
    
    // 7. Delete string item
    if (string_item) cJSON_Delete(string_item);
    
    // 8. Delete object item
    if (object_item) cJSON_Delete(object_item);
    
    // 9. Delete string reference item
    if (string_ref_item) cJSON_Delete(string_ref_item);
    
    // 10. Delete object reference item
    if (object_ref_item) cJSON_Delete(object_ref_item);
    
    // Clean up
    free(fuzz_string);
    
    return 0;
}