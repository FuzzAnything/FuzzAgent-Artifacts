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
//<ID> 196
//<Prompt> []
/*<Combination>: [
*/
//<score> 120, nr_unique_branch: 3
//<Quality> {"density":30,"unique_branches":{"cJSON_AddTrueToObject":[[2165,9,2165,74,0,0,4,1]],"cJSON_AddArrayToObject":[[2249,9,2249,70,0,0,4,1]],"cJSON_IsNull":[[3014,9,3014,21,0,0,4,0]]},"library_calls":["cJSON_ParseWithLength","cJSON_Delete","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_AddRawToObject","cJSON_AddArrayToObject","cJSON_AddTrueToObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsArray","cJSON_GetArraySize","cJSON_CreateObject","cJSON_AddStringToObject","cJSON_AddStringToObject","cJSON_ReplaceItemInArray","cJSON_Delete","cJSON_GetObjectItemCaseSensitive","cJSON_IsNull","cJSON_CreateNull","cJSON_IsNull","cJSON_IsObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsArray","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_IsNull","cJSON_Print","cJSON_free","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_ParseWithLength","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_AddRawToObject","cJSON_AddArrayToObject","cJSON_AddTrueToObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsArray","cJSON_GetArraySize","cJSON_CreateObject","cJSON_AddStringToObject","cJSON_AddStringToObject","cJSON_ReplaceItemInArray","cJSON_Delete","cJSON_GetObjectItemCaseSensitive","cJSON_IsNull","cJSON_CreateNull","cJSON_IsNull","cJSON_IsObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsArray","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_IsNull","cJSON_Print","cJSON_free","cJSON_Delete","cJSON_Delete"],"visited":0}
/*Here's a fuzz driver that uses the specified cJSON APIs with the provided input data structure:

*/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_40(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
	return 0;
	}
	
    
    // Create input file from fuzzer data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Read the JSON data from input
    fseek(in_file, 0, SEEK_END);
    long file_size = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);
    
    char *json_string = new char[file_size + 1];
    fread(json_string, 1, file_size, in_file);
    json_string[file_size] = '\0';
    assert_file_closed(&in_file);;
    
    // Parse the JSON data
    cJSON *root = cJSON_ParseWithLength(json_string, file_size);
    if (!root) {
        delete[] json_string;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        cJSON_Delete(root);
        delete[] json_string;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Step 1: Add a raw string to the menu object
    cJSON *menu = cJSON_GetObjectItemCaseSensitive(root, "menu");
    if (menu && cJSON_IsObject(menu)) {
        cJSON *raw_item = cJSON_AddRawToObject(menu, "raw_data", 
                                               "{\"raw_key\":\"raw_value\"}");
        
        // Check if raw item was successfully added
        if (raw_item) {
            fprintf(out_file, "Added raw data to menu\n");
        }
    }
    
    // Step 2: Add an array to the menu object
    cJSON *array_item = cJSON_AddArrayToObject(menu, "new_array");
    if (array_item) {
        fprintf(out_file, "Created new array in menu\n");
    }
    
    // Step 3: Add a true value to the menu object
    cJSON *true_item = cJSON_AddTrueToObject(menu, "new_true_value");
    if (true_item) {
        fprintf(out_file, "Added true value to menu\n");
    }
    
    // Step 4: Replace an item in the menuitem array
    cJSON *popup = cJSON_GetObjectItemCaseSensitive(menu, "popup");
    if (popup && cJSON_IsObject(popup)) {
        cJSON *menuitem = cJSON_GetObjectItemCaseSensitive(popup, "menuitem");
        if (menuitem && cJSON_IsArray(menuitem)) {
            int array_size = cJSON_GetArraySize(menuitem);
            if (array_size > 0) {
                // Create a new item to replace the first one
                cJSON *new_item = cJSON_CreateObject();
                cJSON_AddStringToObject(new_item, "value", "Replaced");
                cJSON_AddStringToObject(new_item, "onclick", "ReplacedFunc()");
                
                // Replace the first item in the array
                if (cJSON_ReplaceItemInArray(menuitem, 0, new_item)) {
                    fprintf(out_file, "Replaced first item in menuitem array\n");
                } else {
                    cJSON_Delete(new_item); // Clean up if replacement failed
                }
            }
        }
    }
    
    // Step 5: Check various null conditions using cJSON_IsNull
    // Check if specific object items might be null
    cJSON *non_existent = cJSON_GetObjectItemCaseSensitive(root, "non_existent_key");
    if (cJSON_IsNull(non_existent)) {
        fprintf(out_file, "Non-existent key is considered null (not found)\n");
    }
    
    // Create and check an actual null value
    cJSON *null_item = cJSON_CreateNull();
    if (cJSON_IsNull(null_item)) {
        fprintf(out_file, "Created item is null\n");
    }
    
    // Check all items in menuitem array for null values
    if (menu && cJSON_IsObject(menu)) {
        cJSON *popup = cJSON_GetObjectItemCaseSensitive(menu, "popup");
        if (popup && cJSON_IsObject(popup)) {
            cJSON *menuitem = cJSON_GetObjectItemCaseSensitive(popup, "menuitem");
            if (menuitem && cJSON_IsArray(menuitem)) {
                int array_size = cJSON_GetArraySize(menuitem);
                for (int i = 0; i < array_size; i++) {
                    cJSON *item = cJSON_GetArrayItem(menuitem, i);
                    if (cJSON_IsNull(item)) {
                        fprintf(out_file, "Item %d in menuitem is null\n", i);
                    }
                }
            }
        }
    }
    
    // Print modified JSON
    char *modified_json = cJSON_Print(root);
    if (modified_json) {
        fwrite(modified_json, 1, strlen(modified_json), out_file);
        fprintf(out_file, "\n");
        cJSON_free(modified_json);
    }
    
    // Get file descriptor for output file (example usage)
    int out_fd = fuzz_fileno(out_file);
    if (out_fd != -1) {
        fprintf(out_file, "Output file descriptor: %d\n", out_fd);
    }
    
    // Cleanup
    cJSON_Delete(null_item);  // Delete the null item we created
    cJSON_Delete(root);
    assert_file_closed(&out_file);;
    delete[] json_string;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	return 0;
}