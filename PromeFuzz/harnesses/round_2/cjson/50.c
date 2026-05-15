// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_SetNumberHelper at cJSON.c:411:22 in cJSON.h
// cJSON_SetNumberHelper at cJSON.c:411:22 in cJSON.h
// cJSON_SetNumberHelper at cJSON.c:411:22 in cJSON.h
// cJSON_SetNumberHelper at cJSON.c:411:22 in cJSON.h
// cJSON_SetNumberHelper at cJSON.c:411:22 in cJSON.h
// cJSON_SetNumberHelper at cJSON.c:411:22 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_SetNumberHelper at cJSON.c:411:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateBool at cJSON.c:2494:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_GetNumberValue at cJSON.c:109:22 in cJSON.h
// cJSON_GetNumberValue at cJSON.c:109:22 in cJSON.h
// cJSON_GetNumberValue at cJSON.c:109:22 in cJSON.h
// cJSON_GetNumberValue at cJSON.c:109:22 in cJSON.h
// cJSON_GetNumberValue at cJSON.c:109:22 in cJSON.h
// cJSON_GetNumberValue at cJSON.c:109:22 in cJSON.h
// cJSON_GetNumberValue at cJSON.c:109:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddTrueToObject at cJSON.c:2162:22 in cJSON.h
// cJSON_AddTrueToObject at cJSON.c:2162:22 in cJSON.h
// cJSON_AddTrueToObject at cJSON.c:2162:22 in cJSON.h
// cJSON_AddTrueToObject at cJSON.c:2162:22 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_AddTrueToObject at cJSON.c:2162:22 in cJSON.h
// cJSON_AddTrueToObject at cJSON.c:2162:22 in cJSON.h
// cJSON_AddTrueToObject at cJSON.c:2162:22 in cJSON.h
// cJSON_CreateBool at cJSON.c:2494:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_IsNumber at cJSON.c:3022:26 in cJSON.h
// cJSON_GetNumberValue at cJSON.c:109:22 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_SetNumberHelper at cJSON.c:411:22 in cJSON.h
// cJSON_AddTrueToObject at cJSON.c:2162:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_IsNumber at cJSON.c:3022:26 in cJSON.h
// cJSON_IsNumber at cJSON.c:3022:26 in cJSON.h
// cJSON_IsNumber at cJSON.c:3022:26 in cJSON.h
// cJSON_IsNumber at cJSON.c:3022:26 in cJSON.h
// cJSON_IsNumber at cJSON.c:3022:26 in cJSON.h
// cJSON_IsNumber at cJSON.c:3022:26 in cJSON.h
// cJSON_IsNumber at cJSON.c:3022:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
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
#include <math.h>

static void fuzz_cJSON_IsNumber(const uint8_t *data, size_t size) {
    if (size < 1) return;
    
    // Create different types of cJSON items
    cJSON *null_item = cJSON_CreateNull();
    cJSON *bool_item = cJSON_CreateBool(data[0] & 1);
    cJSON *number_item = cJSON_CreateNumber((double)(data[0]));
    cJSON *string_item = cJSON_CreateString("test");
    cJSON *array_item = cJSON_CreateArray();
    cJSON *object_item = cJSON_CreateObject();
    
    // Test cJSON_IsNumber on different types
    cJSON_IsNumber(null_item);
    cJSON_IsNumber(bool_item);
    cJSON_IsNumber(number_item);
    cJSON_IsNumber(string_item);
    cJSON_IsNumber(array_item);
    cJSON_IsNumber(object_item);
    cJSON_IsNumber(NULL);
    
    // Cleanup
    cJSON_Delete(null_item);
    cJSON_Delete(bool_item);
    cJSON_Delete(number_item);
    cJSON_Delete(string_item);
    cJSON_Delete(array_item);
    cJSON_Delete(object_item);
}

static void fuzz_cJSON_CreateNumber(const uint8_t *data, size_t size) {
    if (size < 8) return;
    
    // Extract double from fuzzer data
    double num;
    memcpy(&num, data, sizeof(double));
    
    // Test with various double values
    cJSON *item1 = cJSON_CreateNumber(num);
    cJSON *item2 = cJSON_CreateNumber(0.0);
    cJSON *item3 = cJSON_CreateNumber(1.0);
    cJSON *item4 = cJSON_CreateNumber(-1.0);
    cJSON *item5 = cJSON_CreateNumber(NAN);
    cJSON *item6 = cJSON_CreateNumber(INFINITY);
    cJSON *item7 = cJSON_CreateNumber(-INFINITY);
    
    // Cleanup
    if (item1) cJSON_Delete(item1);
    if (item2) cJSON_Delete(item2);
    if (item3) cJSON_Delete(item3);
    if (item4) cJSON_Delete(item4);
    if (item5) cJSON_Delete(item5);
    if (item6) cJSON_Delete(item6);
    if (item7) cJSON_Delete(item7);
}

static void fuzz_cJSON_AddNumberToObject(const uint8_t *data, size_t size) {
    if (size < 8) return;
    
    // Extract double from fuzzer data
    double num;
    memcpy(&num, data, sizeof(double));
    
    // Create object and test adding numbers
    cJSON *object = cJSON_CreateObject();
    if (!object) return;
    
    // Test with different names
    cJSON_AddNumberToObject(object, "test", num);
    cJSON_AddNumberToObject(object, "", num);
    cJSON_AddNumberToObject(object, "123", 123.456);
    cJSON_AddNumberToObject(object, "special", NAN);
    
    // Test with NULL object
    cJSON_AddNumberToObject(NULL, "test", num);
    
    // Test with NULL name
    cJSON_AddNumberToObject(object, NULL, num);
    
    cJSON_Delete(object);
}

static void fuzz_cJSON_SetNumberHelper(const uint8_t *data, size_t size) {
    if (size < 8) return;
    
    // Extract double from fuzzer data
    double num;
    memcpy(&num, data, sizeof(double));
    
    // Create number item
    cJSON *number_item = cJSON_CreateNumber(0.0);
    
    // Test setting number
    if (number_item) {
        cJSON_SetNumberHelper(number_item, num);
        cJSON_SetNumberHelper(number_item, 0.0);
        cJSON_SetNumberHelper(number_item, -1.0);
        cJSON_SetNumberHelper(number_item, NAN);
        cJSON_SetNumberHelper(number_item, INFINITY);
    }
    
    // Test with NULL
    cJSON_SetNumberHelper(NULL, num);
    
    // Test with non-number item
    cJSON *string_item = cJSON_CreateString("test");
    if (string_item) {
        cJSON_SetNumberHelper(string_item, num);
        cJSON_Delete(string_item);
    }
    
    if (number_item) cJSON_Delete(number_item);
}

static void fuzz_cJSON_GetNumberValue(const uint8_t *data, size_t size) {
    if (size < 1) return;
    
    // Create different types of items
    cJSON *number_item = cJSON_CreateNumber((double)(data[0]));
    cJSON *string_item = cJSON_CreateString("test");
    cJSON *bool_item = cJSON_CreateBool(data[0] & 1);
    cJSON *null_item = cJSON_CreateNull();
    cJSON *array_item = cJSON_CreateArray();
    cJSON *object_item = cJSON_CreateObject();
    
    // Test getting number value
    if (number_item) {
        cJSON_GetNumberValue(number_item);
    }
    
    // Test with non-number items (should return NaN)
    cJSON_GetNumberValue(string_item);
    cJSON_GetNumberValue(bool_item);
    cJSON_GetNumberValue(null_item);
    cJSON_GetNumberValue(array_item);
    cJSON_GetNumberValue(object_item);
    cJSON_GetNumberValue(NULL);
    
    // Cleanup
    if (number_item) cJSON_Delete(number_item);
    if (string_item) cJSON_Delete(string_item);
    if (bool_item) cJSON_Delete(bool_item);
    if (null_item) cJSON_Delete(null_item);
    if (array_item) cJSON_Delete(array_item);
    if (object_item) cJSON_Delete(object_item);
}

static void fuzz_cJSON_AddTrueToObject(const uint8_t *data, size_t size) {
    // Create object
    cJSON *object = cJSON_CreateObject();
    if (!object) return;
    
    // Test adding true with different names
    cJSON_AddTrueToObject(object, "test");
    cJSON_AddTrueToObject(object, "");
    cJSON_AddTrueToObject(object, "123");
    cJSON_AddTrueToObject(object, "true");
    
    // Test with NULL object
    cJSON_AddTrueToObject(NULL, "test");
    
    // Test with NULL name
    cJSON_AddTrueToObject(object, NULL);
    
    // Add multiple items
    for (size_t i = 0; i < 10 && i < size; i++) {
        char name[2] = { (char)data[i], '\0' };
        cJSON_AddTrueToObject(object, name);
    }
    
    cJSON_Delete(object);
}

int LLVMFuzzerTestOneInput_50(const uint8_t *Data, size_t Size) {
    // Initialize cJSON if needed (some versions might need initialization)
    
    // Test all target functions
    fuzz_cJSON_IsNumber(Data, Size);
    fuzz_cJSON_CreateNumber(Data, Size);
    fuzz_cJSON_AddNumberToObject(Data, Size);
    fuzz_cJSON_SetNumberHelper(Data, Size);
    fuzz_cJSON_GetNumberValue(Data, Size);
    fuzz_cJSON_AddTrueToObject(Data, Size);
    
    // Additional combinatorial testing
    if (Size >= 8) {
        double num;
        memcpy(&num, Data, sizeof(double));
        
        // Create object and test multiple operations
        cJSON *obj = cJSON_CreateObject();
        if (obj) {
            // Add a number
            cJSON *num_item = cJSON_AddNumberToObject(obj, "num", num);
            
            // If number was added successfully, test operations on it
            if (num_item) {
                cJSON_IsNumber(num_item);
                cJSON_GetNumberValue(num_item);
                cJSON_SetNumberHelper(num_item, num + 1.0);
            }
            
            // Add true
            cJSON_AddTrueToObject(obj, "flag");
            
            cJSON_Delete(obj);
        }
        
        // Test with file if needed (though not required for these functions)
        FILE *fp = fopen("./dummy_file", "wb");
        if (fp) {
            fwrite(Data, 1, Size, fp);
            fclose(fp);
        }
    }
    
    return 0;
}