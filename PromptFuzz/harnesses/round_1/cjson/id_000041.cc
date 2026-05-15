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
//<ID> 212
//<Prompt> []
/*<Combination>: [
*/
//<score> 54, nr_unique_branch: 1
//<Quality> {"density":27,"unique_branches":{"print_value":[[1427,9,1427,23,0,0,4,0]]},"library_calls":["cJSON_Parse","cJSON_ParseWithLengthOpts","cJSON_ParseWithLength","cJSON_IsArray","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_IsObject","cJSON_GetObjectItem","cJSON_GetObjectItem","cJSON_IsString","cJSON_CreateObject","cJSON_AddTrueToObject","cJSON_AddFalseToObject","cJSON_DeleteItemFromObject","cJSON_AddItemToObject","cJSON_CreateFloatArray","cJSON_IsArray","cJSON_InsertItemInArray","cJSON_Delete","cJSON_Print","cJSON_Minify","cJSON_Parse","cJSON_Delete","cJSON_free","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_Parse","cJSON_ParseWithLengthOpts","cJSON_ParseWithLength","cJSON_IsArray","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_IsObject","cJSON_GetObjectItem","cJSON_GetObjectItem","cJSON_IsString","cJSON_CreateObject","cJSON_AddTrueToObject","cJSON_AddFalseToObject","cJSON_DeleteItemFromObject","cJSON_AddItemToObject","cJSON_CreateFloatArray","cJSON_IsArray","cJSON_InsertItemInArray","cJSON_Print","cJSON_Minify","cJSON_Parse","cJSON_Delete","cJSON_free","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"visited":0}
/**/


// Include cJSON headers

extern "C" int LLVMFuzzerTestOneInput_41(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // Step 1: Create input file using fmemopen
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 2: Read the JSON data from the file
    fseek(in_file, 0, SEEK_END);
    long file_size = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);
    
    char *json_str = new char[file_size + 1];
    size_t read_size = fread(json_str, 1, file_size, in_file);
    json_str[read_size] = '\0';
    assert_file_closed(&in_file);;

    // Step 3: Parse the JSON using multiple parsing methods
    cJSON *root1 = cJSON_Parse(json_str);
    
    const char *parse_end;
    cJSON *root2 = cJSON_ParseWithLengthOpts(json_str, read_size, &parse_end, true);
    
    cJSON *root3 = cJSON_ParseWithLength(json_str, read_size);

    // Step 4: Process the parsed JSON data
    if (root1 && cJSON_IsArray(root1)) {
        int array_size = cJSON_GetArraySize(root1);
        
        for (int i = 0; i < array_size; i++) {
            cJSON *item = cJSON_GetArrayItem(root1, i);
            
            if (item && cJSON_IsObject(item)) {
                // Get object items
                cJSON *city_item = cJSON_GetObjectItem(item, "City");
                cJSON *state_item = cJSON_GetObjectItem(item, "State");
                
                if (city_item && cJSON_IsString(city_item)) {
                    // Create a new object
                    cJSON *new_obj = cJSON_CreateObject();
                    
                    // Add boolean values to the new object
                    cJSON_AddTrueToObject(new_obj, "is_valid");
                    cJSON_AddFalseToObject(new_obj, "is_processed");
                    
                    // Delete an item from the original object
                    cJSON_DeleteItemFromObject(item, "Address");
                    
                    // Add the new object to the original object
                    cJSON_AddItemToObject(item, "metadata", new_obj);
                }
            }
        }
    }

    // Step 5: Create a float array and insert it
    float float_array[] = {1.1f, 2.2f, 3.3f, 4.4f, 5.5f};
    cJSON *float_array_json = cJSON_CreateFloatArray(float_array, 5);
    
    if (root1 && cJSON_IsArray(root1)) {
        cJSON_InsertItemInArray(root1, 0, float_array_json);
    } else {
        cJSON_Delete(float_array_json);
    }

    // Step 6: Print the modified JSON
    char *printed_json = cJSON_Print(root1);
    if (printed_json) {
        // Minify the JSON
        cJSON_Minify(printed_json);
        
        // Parse the minified JSON
        cJSON *minified_root = cJSON_Parse(printed_json);
        
        // Create output file
        FILE *out_file = fopen("output_file", "wb");
        if (out_file) {
            fwrite(printed_json, strlen(printed_json), 1, out_file);
            assert_file_closed(&out_file);;
            
            // Get file descriptor for output file
            int fd = fuzz_fileno(out_file);
            (void)fd; // Suppress unused warning
            
            // Clean up minified root
            cJSON_Delete(minified_root);
        }
        
        // Free the printed JSON string
        cJSON_free(printed_json);
    }

    // Step 7: Clean up allocated resources
    delete[] json_str;
    
    if (root1) cJSON_Delete(root1);
    if (root2) cJSON_Delete(root2);
    if (root3) cJSON_Delete(root3);

    assert_file_closed(&in_file);
	return 0;
}