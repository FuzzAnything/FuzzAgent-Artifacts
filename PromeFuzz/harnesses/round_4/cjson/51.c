// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateStringArray at cJSON.c:2741:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateIntArray at cJSON.c:2621:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateTrue at cJSON.c:2472:23 in cJSON.h
// cJSON_CreateFalse at cJSON.c:2483:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void fuzz_cJSON_GetArraySize(const uint8_t *Data, size_t Size) {
    // Test with NULL input
    cJSON_GetArraySize(NULL);
    
    // Create various cJSON objects to test
    cJSON *array = cJSON_CreateArray();
    if (array) {
        cJSON_GetArraySize(array);
        
        // Add some items to the array
        for (int i = 0; i < 5 && i * sizeof(int) < Size; i++) {
            int value = 0;
            if (i * sizeof(int) + sizeof(int) <= Size) {
                memcpy(&value, Data + i * sizeof(int), sizeof(int));
            }
            cJSON_AddItemToArray(array, cJSON_CreateNumber(value));
        }
        
        cJSON_GetArraySize(array);
        cJSON_Delete(array);
    }
    
    cJSON *object = cJSON_CreateObject();
    if (object) {
        cJSON_GetArraySize(object);
        cJSON_Delete(object);
    }
}

static void fuzz_cJSON_CreateIntArray(const uint8_t *Data, size_t Size) {
    // Determine count based on available data
    int max_count = Size / sizeof(int);
    if (max_count > 100) max_count = 100; // Limit to reasonable size
    
    for (int count = 0; count <= max_count; count++) {
        if (count * sizeof(int) <= Size) {
            int *numbers = NULL;
            if (count > 0) {
                numbers = (int*)malloc(count * sizeof(int));
                if (numbers) {
                    memcpy(numbers, Data, count * sizeof(int));
                }
            }
            
            cJSON *array = cJSON_CreateIntArray(numbers, count);
            if (array) {
                cJSON_Delete(array);
            }
            
            free(numbers);
        }
    }
}

static void fuzz_cJSON_IsArray(const uint8_t *Data, size_t Size) {
    // Test with NULL
    cJSON_IsArray(NULL);
    
    // Create different types of cJSON items
    cJSON *array = cJSON_CreateArray();
    cJSON *object = cJSON_CreateObject();
    cJSON *string = cJSON_CreateString("test");
    cJSON *number = cJSON_CreateNumber(42);
    cJSON *bool_true = cJSON_CreateTrue();
    cJSON *bool_false = cJSON_CreateFalse();
    cJSON *null = cJSON_CreateNull();
    
    if (array) {
        cJSON_IsArray(array);
        cJSON_Delete(array);
    }
    if (object) {
        cJSON_IsArray(object);
        cJSON_Delete(object);
    }
    if (string) {
        cJSON_IsArray(string);
        cJSON_Delete(string);
    }
    if (number) {
        cJSON_IsArray(number);
        cJSON_Delete(number);
    }
    if (bool_true) {
        cJSON_IsArray(bool_true);
        cJSON_Delete(bool_true);
    }
    if (bool_false) {
        cJSON_IsArray(bool_false);
        cJSON_Delete(bool_false);
    }
    if (null) {
        cJSON_IsArray(null);
        cJSON_Delete(null);
    }
}

static void fuzz_cJSON_CreateStringArray(const uint8_t *Data, size_t Size) {
    // Create a dummy file with the data
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }
    
    // Reopen for reading to extract strings
    fp = fopen("./dummy_file", "rb");
    if (fp) {
        // Read the entire file
        char *buffer = (char*)malloc(Size + 1);
        if (buffer) {
            size_t bytes_read = fread(buffer, 1, Size, fp);
            buffer[bytes_read] = '\0';
            
            // Try to create string arrays with different counts
            const char *strings[10];
            int string_count = 0;
            
            // Split buffer into strings by null bytes or newlines
            char *token = strtok(buffer, "\n");
            while (token && string_count < 10) {
                strings[string_count++] = token;
                token = strtok(NULL, "\n");
            }
            
            if (string_count > 0) {
                cJSON *array = cJSON_CreateStringArray(strings, string_count);
                if (array) {
                    cJSON_Delete(array);
                }
            }
            
            free(buffer);
        }
        fclose(fp);
    }
}

static void fuzz_cJSON_GetArrayItem(const uint8_t *Data, size_t Size) {
    // Create an array and test GetArrayItem
    cJSON *array = cJSON_CreateArray();
    if (array) {
        // Add some items
        for (int i = 0; i < 10; i++) {
            cJSON_AddItemToArray(array, cJSON_CreateNumber(i));
        }
        
        // Test various indices
        int array_size = cJSON_GetArraySize(array);
        for (int i = -5; i < array_size + 5; i++) {
            cJSON_GetArrayItem(array, i);
        }
        
        // Test with indices derived from fuzzer data
        if (Size >= sizeof(int)) {
            int index = 0;
            memcpy(&index, Data, sizeof(int));
            cJSON_GetArrayItem(array, index);
        }
        
        cJSON_Delete(array);
    }
    
    // Test with NULL array
    cJSON_GetArrayItem(NULL, 0);
}

static void fuzz_cJSON_InsertItemInArray(const uint8_t *Data, size_t Size) {
    cJSON *array = cJSON_CreateArray();
    if (array) {
        // Add some initial items
        for (int i = 0; i < 5; i++) {
            cJSON_AddItemToArray(array, cJSON_CreateNumber(i * 10));
        }
        
        // Create new items to insert
        cJSON *new_items[3];
        new_items[0] = cJSON_CreateString("inserted");
        new_items[1] = cJSON_CreateNumber(999);
        new_items[2] = cJSON_CreateObject();
        
        // Test insertion at various positions
        for (int which = -3; which < 10; which++) {
            for (int item_idx = 0; item_idx < 3; item_idx++) {
                if (new_items[item_idx]) {
                    cJSON_InsertItemInArray(array, which, new_items[item_idx]);
                    // Create a new item for next insertion since the previous one is now owned by array
                    switch(item_idx) {
                        case 0: new_items[0] = cJSON_CreateString("inserted"); break;
                        case 1: new_items[1] = cJSON_CreateNumber(999); break;
                        case 2: new_items[2] = cJSON_CreateObject(); break;
                    }
                }
            }
        }
        
        // Clean up any remaining items
        for (int i = 0; i < 3; i++) {
            if (new_items[i]) {
                cJSON_Delete(new_items[i]);
            }
        }
        
        cJSON_Delete(array);
    }
    
    // Test with NULL array
    cJSON *item = cJSON_CreateNumber(42);
    if (item) {
        cJSON_InsertItemInArray(NULL, 0, item);
        cJSON_Delete(item);
    }
}

int LLVMFuzzerTestOneInput_51(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        // Use default test cases with minimal data
        uint8_t dummy[1] = {0};
        Data = dummy;
        Size = 1;
    }
    
    // Fuzz all target functions
    fuzz_cJSON_GetArraySize(Data, Size);
    fuzz_cJSON_CreateIntArray(Data, Size);
    fuzz_cJSON_IsArray(Data, Size);
    fuzz_cJSON_CreateStringArray(Data, Size);
    fuzz_cJSON_GetArrayItem(Data, Size);
    fuzz_cJSON_InsertItemInArray(Data, Size);
    
    return 0;
}