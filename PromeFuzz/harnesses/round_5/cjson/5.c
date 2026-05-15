// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddStringToObject at cJSON.c:2210:22 in cJSON.h
// cJSON_AddTrueToObject at cJSON.c:2162:22 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_GetObjectItemCaseSensitive at cJSON.c:1988:23 in cJSON.h
// cJSON_IsString at cJSON.c:3032:26 in cJSON.h
// cJSON_GetObjectItemCaseSensitive at cJSON.c:1988:23 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_GetObjectItemCaseSensitive at cJSON.c:1988:23 in cJSON.h
// cJSON_GetObjectItemCaseSensitive at cJSON.c:1988:23 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_GetObjectItemCaseSensitive at cJSON.c:1988:23 in cJSON.h
// cJSON_IsString at cJSON.c:3032:26 in cJSON.h
// cJSON_GetObjectItemCaseSensitive at cJSON.c:1988:23 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_GetObjectItemCaseSensitive at cJSON.c:1988:23 in cJSON.h
// cJSON_GetObjectItemCaseSensitive at cJSON.c:1988:23 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_GetObjectItemCaseSensitive at cJSON.c:1988:23 in cJSON.h
// cJSON_GetObjectItemCaseSensitive at cJSON.c:1988:23 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    // Create a dummy file and write the fuzzer data to it
    const char* filename = "./dummy_file";
    FILE* fp = fopen(filename, "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // Read the file content and parse as JSON
    fp = fopen(filename, "rb");
    if (!fp) {
        return 0;
    }
    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    
    char* buffer = (char*)malloc(file_size + 1);
    if (!buffer) {
        fclose(fp);
        return 0;
    }
    
    size_t read_size = fread(buffer, 1, file_size, fp);
    buffer[read_size] = '\0';
    fclose(fp);
    
    // Parse the JSON data
    cJSON* json = cJSON_Parse(buffer);
    free(buffer);
    
    if (!json) {
        return 0;
    }
    
    // Create some test keys for object lookups
    // We'll use a mix of fixed keys and keys derived from the fuzzer input
    const char* key1 = "test_key";
    const char* key2 = "boolean_key";
    const char* key3 = "string_key";
    const char* key4 = "compare_key1";
    const char* key5 = "compare_key2";
    
    // 1. First cJSON_GetObjectItemCaseSensitive
    cJSON* item1 = cJSON_GetObjectItemCaseSensitive(json, key1);
    
    // 2. cJSON_IsString
    cJSON_bool is_string = cJSON_IsString(item1);
    
    // 3. Second cJSON_GetObjectItemCaseSensitive
    cJSON* item2 = cJSON_GetObjectItemCaseSensitive(json, key2);
    
    // 4. cJSON_IsTrue
    cJSON_bool is_true = cJSON_IsTrue(item2);
    
    // 5. Third cJSON_GetObjectItemCaseSensitive
    cJSON* item3 = cJSON_GetObjectItemCaseSensitive(json, key3);
    
    // 6. Fourth cJSON_GetObjectItemCaseSensitive
    cJSON* item4 = cJSON_GetObjectItemCaseSensitive(json, key4);
    
    // 7. cJSON_Duplicate
    cJSON* duplicate = NULL;
    if (item4) {
        // Try both recursive and non-recursive duplication
        duplicate = cJSON_Duplicate(item4, 0);  // non-recursive
        if (duplicate) {
            cJSON_Delete(duplicate);
        }
        
        duplicate = cJSON_Duplicate(item4, 1);  // recursive
    }
    
    // 8. Fifth cJSON_GetObjectItemCaseSensitive
    cJSON* item5 = cJSON_GetObjectItemCaseSensitive(json, key5);
    
    // 9. Sixth cJSON_GetObjectItemCaseSensitive
    // Use a key that might not exist to test NULL handling
    const char* non_existent_key = "non_existent_key";
    cJSON* item6 = cJSON_GetObjectItemCaseSensitive(json, non_existent_key);
    
    // 10. cJSON_Compare
    cJSON_bool compare_result = 0;
    if (item5 && duplicate) {
        // Compare the original item with its duplicate
        compare_result = cJSON_Compare(item5, duplicate, 1);  // case sensitive
        // Also try case insensitive comparison
        cJSON_Compare(item5, duplicate, 0);
    }
    
    // Also compare with NULL items to test edge cases
    cJSON_Compare(NULL, item5, 1);
    cJSON_Compare(item5, NULL, 1);
    cJSON_Compare(NULL, NULL, 1);
    
    // 11. cJSON_Delete
    if (duplicate) {
        cJSON_Delete(duplicate);
    }
    
    // Clean up the original JSON
    cJSON_Delete(json);
    
    // Also test with some manually created JSON objects
    cJSON* test_obj = cJSON_CreateObject();
    if (test_obj) {
        cJSON_AddStringToObject(test_obj, "test", "value");
        cJSON_AddTrueToObject(test_obj, "bool");
        cJSON_AddNumberToObject(test_obj, "number", 42);
        
        // Exercise the API functions on the test object
        cJSON* test_item1 = cJSON_GetObjectItemCaseSensitive(test_obj, "test");
        cJSON_IsString(test_item1);
        
        cJSON* test_item2 = cJSON_GetObjectItemCaseSensitive(test_obj, "bool");
        cJSON_IsTrue(test_item2);
        
        cJSON* test_item3 = cJSON_GetObjectItemCaseSensitive(test_obj, "number");
        cJSON* test_item4 = cJSON_GetObjectItemCaseSensitive(test_obj, "test");
        
        cJSON* test_dup = cJSON_Duplicate(test_item4, 1);
        if (test_dup) {
            cJSON_Compare(test_item4, test_dup, 1);
            cJSON_Delete(test_dup);
        }
        
        cJSON_Delete(test_obj);
    }
    
    return 0;
}