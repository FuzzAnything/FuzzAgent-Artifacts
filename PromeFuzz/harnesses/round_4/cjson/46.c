// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateStringArray at cJSON.c:2741:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateBool at cJSON.c:2494:23 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_CreateIntArray at cJSON.c:2621:23 in cJSON.h
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

static void test_array_functions(cJSON *array) {
    if (!array) return;
    
    // Test cJSON_IsArray
    cJSON_bool is_array = cJSON_IsArray(array);
    
    // Test cJSON_GetArraySize
    int array_size = cJSON_GetArraySize(array);
    
    // Test cJSON_GetArrayItem with various indices
    if (array_size > 0) {
        // Test valid indices
        for (int i = 0; i < array_size && i < 10; i++) {
            cJSON *item = cJSON_GetArrayItem(array, i);
            (void)item; // Avoid unused variable warning
        }
        
        // Test boundary indices
        cJSON_GetArrayItem(array, -1); // Invalid negative index
        cJSON_GetArrayItem(array, array_size); // Just beyond size
        cJSON_GetArrayItem(array, array_size - 1); // Last valid index
    }
    
    // Test cJSON_InsertItemInArray
    if (is_array) {
        // Create some items to insert
        cJSON *new_item1 = cJSON_CreateNumber(42.0);
        cJSON *new_item2 = cJSON_CreateString("fuzz");
        
        if (new_item1) {
            // Insert at various positions
            cJSON_InsertItemInArray(array, 0, new_item1); // Insert at beginning
            if (new_item2) {
                cJSON_InsertItemInArray(array, array_size, new_item2); // Insert at end
                // Note: new_item2 is now owned by array, don't delete separately
            } else {
                // If new_item2 creation failed, we need to clean up new_item1
                // But it's already inserted into array, so array owns it
            }
        } else if (new_item2) {
            // If new_item1 failed but new_item2 succeeded
            cJSON_InsertItemInArray(array, 0, new_item2);
        }
        
        // Don't delete new_item1 or new_item2 here as they may be inserted into array
        // The array will clean them up when deleted
    }
}

int LLVMFuzzerTestOneInput_46(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Test with NULL inputs
    cJSON_GetArraySize(NULL);
    cJSON_IsArray(NULL);
    cJSON_GetArrayItem(NULL, 0);
    cJSON_InsertItemInArray(NULL, 0, NULL);
    
    // Test 1: Create integer array
    if (Size >= sizeof(int)) {
        int count = (Size / sizeof(int)) % 100; // Limit to reasonable size
        if (count > 0) {
            int *numbers = (int*)malloc(count * sizeof(int));
            if (numbers) {
                // Fill with data from fuzzer input
                for (int i = 0; i < count; i++) {
                    if (i * sizeof(int) + sizeof(int) <= Size) {
                        memcpy(&numbers[i], Data + (i * sizeof(int)), sizeof(int));
                    } else {
                        numbers[i] = 0;
                    }
                }
                
                cJSON *int_array = cJSON_CreateIntArray(numbers, count);
                test_array_functions(int_array);
                if (int_array) cJSON_Delete(int_array);
                
                free(numbers);
            }
        }
    }
    
    // Test 2: Create string array
    if (Size > 0) {
        // Limit to reasonable number of strings
        int max_strings = 20;
        const char **strings = (const char**)malloc(max_strings * sizeof(const char*));
        if (strings) {
            int string_count = 0;
            size_t offset = 0;
            
            // Create strings from fuzzer input
            while (string_count < max_strings && offset < Size) {
                // Find null terminator or use remaining bytes
                size_t str_len = 0;
                while (offset + str_len < Size && Data[offset + str_len] != 0 && str_len < 256) {
                    str_len++;
                }
                
                if (str_len > 0) {
                    char *str = (char*)malloc(str_len + 1);
                    if (str) {
                        memcpy(str, Data + offset, str_len);
                        str[str_len] = '\0';
                        strings[string_count++] = str;
                    }
                    offset += str_len;
                }
                
                // Skip null terminator if present
                if (offset < Size && Data[offset] == 0) {
                    offset++;
                }
                
                // Break if we don't have enough data for another string
                if (offset >= Size) break;
            }
            
            if (string_count > 0) {
                cJSON *string_array = cJSON_CreateStringArray(strings, string_count);
                test_array_functions(string_array);
                if (string_array) cJSON_Delete(string_array);
            }
            
            // Free allocated strings
            for (int i = 0; i < string_count; i++) {
                free((void*)strings[i]);
            }
            free(strings);
        }
    }
    
    // Test 3: Create array from parsed JSON
    if (Size > 0) {
        // Write data to a file for parsing
        FILE *fp = fopen("./dummy_file", "wb");
        if (fp) {
            fwrite(Data, 1, Size, fp);
            fclose(fp);
            
            // Read and parse the file
            fp = fopen("./dummy_file", "rb");
            if (fp) {
                fseek(fp, 0, SEEK_END);
                long file_size = ftell(fp);
                fseek(fp, 0, SEEK_SET);
                
                if (file_size > 0) {
                    char *buffer = (char*)malloc(file_size + 1);
                    if (buffer) {
                        fread(buffer, 1, file_size, fp);
                        buffer[file_size] = '\0';
                        
                        cJSON *parsed = cJSON_Parse(buffer);
                        if (parsed) {
                            test_array_functions(parsed);
                            cJSON_Delete(parsed);
                        }
                        
                        free(buffer);
                    }
                }
                fclose(fp);
            }
        }
    }
    
    // Test 4: Create empty array and test insertion
    cJSON *empty_array = cJSON_CreateArray();
    if (empty_array) {
        // Insert various items
        cJSON *num_item = cJSON_CreateNumber(123.456);
        cJSON *str_item = cJSON_CreateString("test");
        cJSON *bool_item = cJSON_CreateBool(1);
        
        if (num_item) cJSON_InsertItemInArray(empty_array, 0, num_item);
        if (str_item) cJSON_InsertItemInArray(empty_array, 1, str_item);
        if (bool_item) cJSON_InsertItemInArray(empty_array, 5, bool_item); // Beyond current size
        
        test_array_functions(empty_array);
        cJSON_Delete(empty_array);
    }
    
    return 0;
}