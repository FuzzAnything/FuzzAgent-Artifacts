// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_ReplaceItemInObject at cJSON.c:2450:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Use first byte to determine double values
    double num1 = (double)(Data[0] % 256);
    double num2 = (double)((Size > 1 ? Data[1] : Data[0]) % 256);
    
    // Create key string from fuzzer input
    static char key[256];
    size_t key_len = Size < 255 ? Size : 255;
    memcpy(key, Data, key_len);
    key[key_len] = '\0';
    
    // Create numbers
    cJSON *number1 = cJSON_CreateNumber(num1);
    cJSON *number2 = cJSON_CreateNumber(num2);
    
    if (!number1 || !number2) {
        if (number1) cJSON_Delete(number1);
        if (number2) cJSON_Delete(number2);
        return 0;
    }
    
    // Create parent object
    cJSON *object = cJSON_CreateObject();
    if (!object) {
        cJSON_Delete(number1);
        cJSON_Delete(number2);
        return 0;
    }
    
    // Add first number to object
    cJSON_AddItemToObject(object, key, number1);
    
    // Replace with second number
    cJSON_ReplaceItemInObject(object, key, number2);
    
    // Clean up
    cJSON_Delete(object);
    
    // Write dummy file for any potential file operations
    write_dummy_file(Data, Size);
    
    return 0;
}