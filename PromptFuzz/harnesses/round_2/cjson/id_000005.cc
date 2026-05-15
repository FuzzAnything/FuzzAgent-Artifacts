#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
extern "C" {
#include <cJSON.h>
}
//<ID> 9
//<Prompt> []
/*<Combination>: [
*/
//<score> 8.333333, nr_unique_branch: 0
//<Quality> {"density":25,"unique_branches":{},"library_calls":["cJSON_ParseWithLengthOpts","cJSON_CreateObject","cJSON_CreateArray","cJSON_CreateNumber","cJSON_CreateTrue","cJSON_AddItemToObject","cJSON_AddItemToObject","cJSON_AddItemToObject","cJSON_IsTrue","cJSON_IsRaw","cJSON_Print","cJSON_Delete","cJSON_IsTrue","cJSON_IsRaw","cJSON_CreateNumber","cJSON_GetObjectItem","cJSON_IsObject","cJSON_CreateNumber","cJSON_AddItemToObject","cJSON_GetObjectItemCaseSensitive","cJSON_CreateString","cJSON_ReplaceItemInObjectCaseSensitive","cJSON_GetObjectItem","cJSON_CreateArray","cJSON_AddItemToObject","cJSON_CreateNumber","cJSON_AddItemToArray","cJSON_GetArraySize","cJSON_ReplaceItemInArray","cJSON_Delete","cJSON_IsObject","cJSON_IsNumber","cJSON_CreateNumber","cJSON_ReplaceItemViaPointer","cJSON_Print","cJSON_Delete"],"critical_calls":["cJSON_ParseWithLengthOpts","cJSON_IsTrue","cJSON_IsRaw","cJSON_CreateNumber","cJSON_GetObjectItem","cJSON_IsObject","cJSON_CreateNumber","cJSON_AddItemToObject","cJSON_GetObjectItemCaseSensitive","cJSON_CreateString","cJSON_ReplaceItemInObjectCaseSensitive","cJSON_GetObjectItem","cJSON_CreateArray","cJSON_AddItemToObject","cJSON_CreateNumber","cJSON_AddItemToArray","cJSON_GetArraySize","cJSON_ReplaceItemInArray","cJSON_IsObject","cJSON_IsNumber","cJSON_CreateNumber","cJSON_ReplaceItemViaPointer","cJSON_Print","cJSON_Delete"],"visited":2}
/*Here's a fuzzer that implements the requested functionality using the specified cjson APIs:

*/


// Include cjson headers

extern "C" int LLVMFuzzerTestOneInput_5(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
	return 0;
	}
	
    
    // Parse input JSON data using cJSON_ParseWithLengthOpts
    const char** return_parse_end = nullptr;
    cJSON_bool require_null_terminated = 0;
    
    // Convert data to string for parsing (ensure null termination)
    char* json_str = (char*)malloc(size + 1);
    if (!json_str) {
	return 0;
	}
	
    
    memcpy(json_str, data, size);
    json_str[size] = '\0';
    
    cJSON* root = cJSON_ParseWithLengthOpts(json_str, size, return_parse_end, require_null_terminated);
    free(json_str);
    
    if (!root) {
        // If parsing fails, try to create and manipulate JSON objects instead
        cJSON* new_root = cJSON_CreateObject();
        if (!new_root) {
	return 0;
	}
	
        
        // Create some JSON objects to work with
        cJSON* array = cJSON_CreateArray();
        cJSON* number = cJSON_CreateNumber(42.0);
        cJSON* true_item = cJSON_CreateTrue();
        
        // Use cJSON_AddItemToObject
        cJSON_AddItemToObject(new_root, "array", array);
        cJSON_AddItemToObject(new_root, "number", number);
        cJSON_AddItemToObject(new_root, "true_item", true_item);
        
        // Use cJSON_IsTrue and cJSON_IsRaw
        cJSON_bool is_true = cJSON_IsTrue(true_item);
        cJSON_bool is_raw = cJSON_IsRaw(true_item);
        
        // Print the created JSON
        char* printed = cJSON_Print(new_root);
        if (printed) {
            free(printed);
        }
        
        // Clean up
        cJSON_Delete(new_root);
        return 0;
    }
    
    // Event: Parse JSON, manipulate it, replace items, and print result
    
    // 1. Use cJSON_IsTrue - check for any true values in the JSON
    cJSON* current = root;
    while (current) {
        if (cJSON_IsTrue(current)) {
            // Found a true item
            break;
        }
        current = current->next;
    }
    
    // 2. Use cJSON_IsRaw - check for raw items (though our input likely has none)
    cJSON* raw_check = root;
    cJSON_bool has_raw = cJSON_IsRaw(raw_check);
    
    // 3. Create a new number to use for replacement
    double random_value = (double)(data[0] % 100); // Use first byte as seed
    cJSON* new_number = cJSON_CreateNumber(random_value);
    
    // 4. Use cJSON_AddItemToObject - add new item if object exists
    cJSON* format_obj = cJSON_GetObjectItem(root, "format");
    if (format_obj && cJSON_IsObject(format_obj)) {
        // Add a new item to the format object
        cJSON* extra_item = cJSON_CreateNumber(999.0);
        cJSON_AddItemToObject(format_obj, "extra_field", extra_item);
    }
    
    // 5. Use cJSON_ReplaceItemInObjectCaseSensitive
    cJSON* name_item = cJSON_GetObjectItemCaseSensitive(root, "name");
    if (name_item) {
        cJSON* new_name = cJSON_CreateString("Replaced Name");
        cJSON_ReplaceItemInObjectCaseSensitive(root, "name", new_name);
    }
    
    // 6. Use cJSON_ReplaceItemInArray (if we have arrays)
    // Create an array item if none exists
    cJSON* test_array = cJSON_GetObjectItem(root, "test_array");
    if (!test_array) {
        test_array = cJSON_CreateArray();
        cJSON_AddItemToObject(root, "test_array", test_array);
        
        // Add some initial items
        for (int i = 0; i < 3 && i < (int)size; i++) {
            cJSON* array_item = cJSON_CreateNumber((double)data[i % size]);
            cJSON_AddItemToArray(test_array, array_item);
        }
    }
    
    // Replace an item in the array if it has items
    int array_size = cJSON_GetArraySize(test_array);
    if (array_size > 0) {
        int replace_index = data[0] % array_size;
        cJSON_ReplaceItemInArray(test_array, replace_index, new_number);
    } else {
        // Clean up if not used
        cJSON_Delete(new_number);
    }
    
    // 7. Use cJSON_ReplaceItemViaPointer
    // Find an item to replace via pointer
    cJSON* item_to_replace = nullptr;
    if (cJSON_IsObject(root)) {
        item_to_replace = root->child;
        while (item_to_replace) {
            if (cJSON_IsNumber(item_to_replace)) {
                break;
            }
            item_to_replace = item_to_replace->next;
        }
    }
    
    if (item_to_replace) {
        cJSON* replacement = cJSON_CreateNumber(777.0);
        cJSON_ReplaceItemViaPointer(root, item_to_replace, replacement);
    }
    
    // 8. Use cJSON_Print to get the final JSON string
    char* final_json = cJSON_Print(root);
    if (final_json) {
        // Write to output file
        FILE* out_file = fopen("output_file", "wb");
        if (out_file) {
            fwrite(final_json, 1, strlen(final_json), out_file);
            assert_file_closed(&out_file);;
        }
        free(final_json);
    }
    
    // Also print to stderr for debugging during fuzzing
    // Uncomment for debugging:
    // fprintf(stderr, "Processed JSON of size: %zu\n", size);
    
    // Clean up
    cJSON_Delete(root);
    
    return 0;
}