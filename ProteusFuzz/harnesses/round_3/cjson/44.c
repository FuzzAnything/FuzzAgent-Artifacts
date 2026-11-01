// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_GetNumberValue at cJSON.c:109:22 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_GetNumberValue at cJSON.c:109:22 in cJSON.h
// cJSON_GetNumberValue at cJSON.c:109:22 in cJSON.h
// cJSON_CreateDoubleArray at cJSON.c:2701:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateDoubleArray at cJSON.c:2701:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateDoubleArray at cJSON.c:2701:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_SetNumberHelper at cJSON.c:411:22 in cJSON.h
// cJSON_SetNumberHelper at cJSON.c:411:22 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_GetObjectItem at cJSON.c:1983:23 in cJSON.h
// cJSON_IsNumber at cJSON.c:3022:26 in cJSON.h
// cJSON_GetNumberValue at cJSON.c:109:22 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_IsNumber at cJSON.c:3022:26 in cJSON.h
// cJSON_GetNumberValue at cJSON.c:109:22 in cJSON.h
// cJSON_CreateDoubleArray at cJSON.c:2701:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_IsNumber at cJSON.c:3022:26 in cJSON.h
// cJSON_IsNumber at cJSON.c:3022:26 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_IsNumber at cJSON.c:3022:26 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_IsNumber at cJSON.c:3022:26 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_IsNumber at cJSON.c:3022:26 in cJSON.h
// cJSON_CreateTrue at cJSON.c:2472:23 in cJSON.h
// cJSON_IsNumber at cJSON.c:3022:26 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_IsNumber at cJSON.c:3022:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_IsNumber at cJSON.c:3022:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cJSON.h>
#include <math.h>
#include <limits.h>

static void cleanup_cjson(cJSON *item) {
    if (item) {
        cJSON_Delete(item);
    }
}

int LLVMFuzzerTestOneInput_44(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    cJSON *obj = NULL;
    cJSON *arr = NULL;
    cJSON *item = NULL;
    double num;
    const char *key;
    int count;

    // Extract data for different test scenarios
    // Use first byte to decide operation type
    int op = Data[0] % 10;

    // Create a basic object for tests requiring an object
    obj = cJSON_CreateObject();
    if (!obj) {
        return 0;
    }

    switch (op) {
        case 0: // Test cJSON_IsNumber with various inputs
            if (Size > 1) {
                num = Data[1];
                item = cJSON_CreateNumber(num);
                if (item) {
                    cJSON_IsNumber(item);
                    cleanup_cjson(item);
                    item = NULL;
                }
            }
            // Test with non-number types
            cJSON_IsNumber(NULL);
            
            item = cJSON_CreateString("test");
            if (item) {
                cJSON_IsNumber(item);
                cleanup_cjson(item);
                item = NULL;
            }
            
            item = cJSON_CreateObject();
            if (item) {
                cJSON_IsNumber(item);
                cleanup_cjson(item);
                item = NULL;
            }
            
            item = cJSON_CreateArray();
            if (item) {
                cJSON_IsNumber(item);
                cleanup_cjson(item);
                item = NULL;
            }
            
            item = cJSON_CreateTrue();
            if (item) {
                cJSON_IsNumber(item);
                cleanup_cjson(item);
                item = NULL;
            }
            
            item = cJSON_CreateNull();
            if (item) {
                cJSON_IsNumber(item);
                cleanup_cjson(item);
                item = NULL;
            }
            break;

        case 1: // Test cJSON_CreateNumber
            if (Size > 1) {
                num = Data[1];
                item = cJSON_CreateNumber(num);
                if (item) {
                    cJSON_IsNumber(item);
                    cleanup_cjson(item);
                    item = NULL;
                }
            }
            // Test with special values
            item = cJSON_CreateNumber(INFINITY);
            if (item) {
                cleanup_cjson(item);
                item = NULL;
            }
            item = cJSON_CreateNumber(-INFINITY);
            if (item) {
                cleanup_cjson(item);
                item = NULL;
            }
            item = cJSON_CreateNumber(NAN);
            if (item) {
                cleanup_cjson(item);
                item = NULL;
            }
            break;

        case 2: // Test cJSON_AddNumberToObject
            if (Size > 2) {
                num = Data[1];
                // Use a simple key derived from data
                key = "fuzz_key";
                cJSON_AddNumberToObject(obj, key, num);
            }
            // Try adding duplicate key
            if (Size > 2) {
                num = Data[1] * 2.0;
                cJSON_AddNumberToObject(obj, "fuzz_key", num);
            }
            break;

        case 3: // Test cJSON_GetNumberValue
            if (Size > 1) {
                num = Data[1];
                item = cJSON_CreateNumber(num);
                if (item) {
                    double val = cJSON_GetNumberValue(item);
                    (void)val; // Use value to avoid unused warning
                    cleanup_cjson(item);
                    item = NULL;
                }
            }
            // Test with non-number
            item = cJSON_CreateString("not a number");
            if (item) {
                double val = cJSON_GetNumberValue(item);
                if (isnan(val)) {
                    // Expected behavior
                }
                cleanup_cjson(item);
                item = NULL;
            }
            // Test with NULL
            double val_null = cJSON_GetNumberValue(NULL);
            if (isnan(val_null)) {
                // Expected behavior
            }
            break;

        case 4: // Test cJSON_CreateDoubleArray
            if (Size > 2) {
                count = (Data[1] % 100) + 1;
                if (Size >= (size_t)(count + 2)) {
                    double *numbers = malloc(count * sizeof(double));
                    if (numbers) {
                        for (int i = 0; i < count; i++) {
                            numbers[i] = Data[2 + i];
                        }
                        arr = cJSON_CreateDoubleArray(numbers, count);
                        if (arr) {
                            // Verify array creation
                            cJSON_Delete(arr);
                            arr = NULL;
                        }
                        free(numbers);
                    }
                }
            }
            // Test with invalid count
            arr = cJSON_CreateDoubleArray(NULL, -1);
            if (arr) {
                cJSON_Delete(arr);
                arr = NULL;
            }
            arr = cJSON_CreateDoubleArray(NULL, 0);
            if (arr) {
                cJSON_Delete(arr);
                arr = NULL;
            }
            break;

        case 5: // Test cJSON_SetNumberHelper
            if (Size > 1) {
                num = Data[1];
                item = cJSON_CreateNumber(0.0);
                if (item) {
                    double result = cJSON_SetNumberHelper(item, num);
                    (void)result;
                    cleanup_cjson(item);
                    item = NULL;
                }
            }
            // Test with NULL
            double null_result = cJSON_SetNumberHelper(NULL, 1.0);
            if (isnan(null_result)) {
                // Expected behavior
            }
            break;

        case 6: // Combined test: Create object, add numbers, check types
            if (Size > 3) {
                double n1 = Data[1];
                double n2 = Data[2];
                double n3 = Data[3];
                
                cJSON_AddNumberToObject(obj, "num1", n1);
                cJSON_AddNumberToObject(obj, "num2", n2);
                cJSON_AddNumberToObject(obj, "num3", n3);
                
                cJSON *child1 = cJSON_GetObjectItem(obj, "num1");
                if (child1) {
                    cJSON_IsNumber(child1);
                    cJSON_GetNumberValue(child1);
                    // Do not delete child1 here, it's owned by obj
                }
            }
            break;

        case 7: // Stress test with many numbers
            if (Size > 10) {
                for (int i = 1; i < Size && i < 100; i++) {
                    double val = Data[i];
                    cJSON_AddNumberToObject(obj, "stress_key", val);
                }
            }
            break;

        case 8: // Test with extreme values
            {
                double extremes[] = {
                    0.0,
                    -0.0,
                    1.0,
                    -1.0,
                    1e308,
                    -1e308,
                    1e-308,
                    -1e-308,
                    INFINITY,
                    -INFINITY,
                    NAN
                };
                for (int i = 0; i < 11; i++) {
                    item = cJSON_CreateNumber(extremes[i]);
                    if (item) {
                        cJSON_IsNumber(item);
                        cJSON_GetNumberValue(item);
                        cleanup_cjson(item);
                        item = NULL;
                    }
                }
            }
            break;

        case 9: // Test array creation with various counts
            {
                double test_nums[] = {1.0, 2.0, 3.0, 4.0, 5.0};
                for (int i = 0; i <= 5; i++) {
                    arr = cJSON_CreateDoubleArray(test_nums, i);
                    if (arr) {
                        cJSON_Delete(arr);
                        arr = NULL;
                    }
                }
            }
            break;
    }

    cleanup_cjson(obj);
    cleanup_cjson(arr);
    cleanup_cjson(item);

    return 0;
}