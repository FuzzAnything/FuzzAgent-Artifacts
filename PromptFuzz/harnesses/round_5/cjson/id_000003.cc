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
//<score> 23, nr_unique_branch: 1
//<Quality> {"density":23,"unique_branches":{"cJSON_IsArray":[[3044,9,3044,21,0,0,4,0]]},"library_calls":["cJSON_Parse","cJSON_GetObjectItemCaseSensitive","cJSON_Delete","cJSON_GetObjectItemCaseSensitive","cJSON_IsArray","cJSON_Delete","cJSON_GetObjectItemCaseSensitive","cJSON_Delete","cJSON_GetArrayItem","cJSON_GetObjectItemCaseSensitive","cJSON_IsString","cJSON_CreateBool","cJSON_ReplaceItemInObject","cJSON_DeleteItemFromObject","cJSON_AddItemReferenceToObject","cJSON_DetachItemFromObjectCaseSensitive","cJSON_Delete","cJSON_GetArraySize","cJSON_CreateIntArray","cJSON_ReplaceItemInArray","cJSON_Print","cJSON_free","cJSON_Delete"],"critical_calls":["cJSON_Parse","cJSON_GetObjectItemCaseSensitive","cJSON_GetObjectItemCaseSensitive","cJSON_IsArray","cJSON_GetObjectItemCaseSensitive","cJSON_GetArrayItem","cJSON_GetObjectItemCaseSensitive","cJSON_IsString","cJSON_CreateBool","cJSON_ReplaceItemInObject","cJSON_DeleteItemFromObject","cJSON_AddItemReferenceToObject","cJSON_DetachItemFromObjectCaseSensitive","cJSON_Delete","cJSON_GetArraySize","cJSON_CreateIntArray","cJSON_ReplaceItemInArray","cJSON_Print","cJSON_free","cJSON_Delete"],"visited":1}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_3(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create a null-terminated string from input data
    char *json_string = (char *)malloc(size + 1);
    if (!json_string) {
	return 0;
	}
	
    memcpy(json_string, data, size);
    json_string[size] = '\0';

    // Parse JSON using cJSON
    cJSON *root = cJSON_Parse(json_string);
    if (!root) {
        free(json_string);
        return 0;
    }

    // Get web-app object
    cJSON *web_app = cJSON_GetObjectItemCaseSensitive(root, "web-app");
    if (!web_app) {
        cJSON_Delete(root);
        free(json_string);
        return 0;
    }

    // Get servlet array
    cJSON *servlet_array = cJSON_GetObjectItemCaseSensitive(web_app, "servlet");
    if (!cJSON_IsArray(servlet_array)) {
        cJSON_Delete(root);
        free(json_string);
        return 0;
    }

    // Get servlet-mapping object
    cJSON *servlet_mapping = cJSON_GetObjectItemCaseSensitive(web_app, "servlet-mapping");
    if (!servlet_mapping) {
        cJSON_Delete(root);
        free(json_string);
        return 0;
    }

    // API 1: Check if servlet-name in first servlet is a string
    cJSON *first_servlet = cJSON_GetArrayItem(servlet_array, 0);
    if (first_servlet) {
        cJSON *servlet_name = cJSON_GetObjectItemCaseSensitive(first_servlet, "servlet-name");
        if (servlet_name && cJSON_IsString(servlet_name)) {
            // API 2: Create a boolean value
            cJSON *new_bool = cJSON_CreateBool(1);
            
            // API 3: Replace servlet-name with boolean
            cJSON_ReplaceItemInObject(first_servlet, "servlet-name", new_bool);
        }
    }

    // API 4: Delete an item from servlet-mapping
    cJSON_DeleteItemFromObject(servlet_mapping, "cofaxCDS");

    // API 5: Add item reference to servlet-mapping
    if (first_servlet) {
        cJSON_AddItemReferenceToObject(servlet_mapping, "firstServletRef", first_servlet);
    }

    // API 6: Detach item from object (case sensitive)
    cJSON *detached = cJSON_DetachItemFromObjectCaseSensitive(servlet_mapping, "cofaxEmail");
    if (detached) {
        cJSON_Delete(detached);
    }

    // API 7: Replace item in array
    if (cJSON_GetArraySize(servlet_array) >= 2) {
        // API 8: Create int array
        int int_array_data[] = {100, 200, 300, 400};
        cJSON *int_array = cJSON_CreateIntArray(int_array_data, 4);
        
        // Replace second servlet with int array
        cJSON_ReplaceItemInArray(servlet_array, 1, int_array);
    }

    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // Print modified JSON to file
        char *printed = cJSON_Print(root);
        if (printed) {
            fwrite(printed, 1, strlen(printed), out_file);
            cJSON_free(printed);
        }
        
        // Get file descriptor
        int fd = fuzz_fileno(out_file);
        (void)fd; // Use fd if needed
        
        assert_file_closed(&out_file);;
    }

    // Clean up
    cJSON_Delete(root);
    free(json_string);
    
    assert_file_closed(&out_file);
	return 0;
}