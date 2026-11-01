// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetNumberValue at cJSON.c:109:22 in cJSON.h
// cJSON_IsNumber at cJSON.c:3022:26 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_GetNumberValue at cJSON.c:109:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_IsNumber at cJSON.c:3022:26 in cJSON.h
// cJSON_GetNumberValue at cJSON.c:109:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_SetNumberHelper at cJSON.c:411:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_SetNumberHelper at cJSON.c:411:22 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cJSON.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* Helper to extract a double from fuzzer input */
static double get_double_from_input(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + sizeof(double) > size) {
        return 0.0;
    }
    double val;
    memcpy(&val, data + *offset, sizeof(double));
    *offset += sizeof(double);
    return val;
}

/* Helper to extract a string from fuzzer input */
static char* get_string_from_input(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset >= size) {
        return NULL;
    }
    size_t len = 0;
    while (*offset + len < size && data[*offset + len] != '\0') {
        len++;
    }
    if (len == 0) {
        *offset += 1;
        return NULL;
    }
    char *str = (char *)malloc(len + 1);
    if (!str) return NULL;
    memcpy(str, data + *offset, len);
    str[len] = '\0';
    *offset += len + 1;
    return str;
}

int LLVMFuzzerTestOneInput_49(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Test 1: cJSON_CreateObject and cJSON_AddNumberToObject */
    cJSON *obj = cJSON_CreateObject();
    if (obj) {
        size_t offset = 0;
        while (offset + 1 < Size) {
            char *key = get_string_from_input(Data, Size, &offset);
            if (!key) break;
            
            double num = get_double_from_input(Data, Size, &offset);
            cJSON *item = cJSON_AddNumberToObject(obj, key, num);
            free(key);
            
            if (item) {
                /* Test cJSON_GetNumberValue on the added item */
                double val = cJSON_GetNumberValue(item);
                (void)val; /* Suppress unused warning */
            }
        }
        cJSON_Delete(obj);
    }

    /* Test 2: cJSON_CreateNumber and cJSON_IsNumber */
    size_t offset2 = 0;
    while (offset2 + sizeof(double) <= Size) {
        double num = get_double_from_input(Data, Size, &offset2);
        cJSON *numItem = cJSON_CreateNumber(num);
        
        if (numItem) {
            cJSON_bool isNum = cJSON_IsNumber(numItem);
            (void)isNum;
            
            double retrieved = cJSON_GetNumberValue(numItem);
            (void)retrieved;
            
            cJSON_Delete(numItem);
        }
    }

    /* Test 3: cJSON_SetNumberHelper on various items */
    cJSON *testObj = cJSON_CreateObject();
    if (testObj) {
        cJSON *numNode = cJSON_CreateNumber(1.0);
        if (numNode) {
            double result = cJSON_SetNumberHelper(numNode, 123.456);
            (void)result;
            cJSON_Delete(numNode);
        }
        
        /* Test with NULL object */
        double nullResult = cJSON_SetNumberHelper(NULL, 999.0);
        (void)nullResult;
        
        cJSON_Delete(testObj);
    }

    /* Test 4: Edge cases with invalid inputs */
    cJSON *nullItem = NULL;
    double nanVal = cJSON_GetNumberValue(nullItem);
    (void)nanVal;
    
    cJSON_bool isNumNull = cJSON_IsNumber(nullItem);
    (void)isNumNull;

    return 0;
}