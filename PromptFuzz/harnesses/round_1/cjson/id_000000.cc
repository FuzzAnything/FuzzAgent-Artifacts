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
//<ID> 1
//<Prompt> []
/*<Combination>: [
*/
//<score> 60, nr_unique_branch: 7
//<Quality> {"density":20,"unique_branches":{"cJSON_AddNullToObject":[[2153,9,2153,69,0,0,4,0]],"cJSON_CreateNull":[[2464,8,2464,12,0,0,4,0]],"create_reference":[[2009,9,2009,21,0,0,4,1],[2015,9,2015,26,0,0,4,1]],"case_insensitive_strcmp":[[135,30,135,47,0,0,4,0]],"cJSON_AddItemReferenceToObject":[[2142,9,2142,25,0,0,4,1],[2142,29,2142,45,0,0,4,1]]},"library_calls":["cJSON_Parse","cJSON_GetObjectItem","cJSON_IsObject","cJSON_Delete","cJSON_GetObjectItem","cJSON_IsObject","cJSON_Delete","cJSON_GetObjectItem","cJSON_IsString","cJSON_CreateObject","cJSON_AddItemReferenceToObject","cJSON_AddNullToObject","cJSON_AddTrueToObject","cJSON_AddFalseToObject","cJSON_DeleteItemFromObject","cJSON_AddItemToObject","cJSON_Print","cJSON_Minify","cJSON_free","cJSON_ParseWithOpts","cJSON_Delete","cJSON_Delete","cJSON_ParseWithLength","cJSON_Delete"],"critical_calls":["cJSON_Parse","cJSON_GetObjectItem","cJSON_IsObject","cJSON_GetObjectItem","cJSON_IsObject","cJSON_GetObjectItem","cJSON_IsString","cJSON_CreateObject","cJSON_AddItemReferenceToObject","cJSON_AddNullToObject","cJSON_AddTrueToObject","cJSON_AddFalseToObject","cJSON_DeleteItemFromObject","cJSON_AddItemToObject","cJSON_Print","cJSON_Minify","cJSON_free","cJSON_ParseWithOpts","cJSON_Delete","cJSON_Delete","cJSON_ParseWithLength","cJSON_Delete"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_0(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }
    
    // Create a null-terminated string from the input data
    char *input_string = (char *)malloc(size + 1);
    if (!input_string) {
        return 0;
    }
    memcpy(input_string, data, size);
    input_string[size] = '\0';
    
    // Parse the JSON input
    cJSON *root = cJSON_Parse(input_string);
    if (!root) {
        free(input_string);
        return 0;
    }
    
    // Get the widget object
    cJSON *widget = cJSON_GetObjectItem(root, "widget");
    if (!widget || !cJSON_IsObject(widget)) {
        cJSON_Delete(root);
        free(input_string);
        return 0;
    }
    
    // Get the text object from widget
    cJSON *text = cJSON_GetObjectItem(widget, "text");
    if (!text || !cJSON_IsObject(text)) {
        cJSON_Delete(root);
        free(input_string);
        return 0;
    }
    
    // Check if "data" field is a string
    cJSON *data_field = cJSON_GetObjectItem(text, "data");
    if (data_field && cJSON_IsString(data_field)) {
        // This API is used as required
    }
    
    // Create a new object to hold references
    cJSON *references = cJSON_CreateObject();
    if (references) {
        // Add a reference to the data string
        if (data_field) {
            cJSON_AddItemReferenceToObject(references, "data_ref", data_field);
        }
        
        // Add null, true, and false values to the references object
        cJSON_AddNullToObject(references, "null_value");
        cJSON_AddTrueToObject(references, "true_value");
        cJSON_AddFalseToObject(references, "false_value");
        
        // Delete one of the items we just added
        cJSON_DeleteItemFromObject(references, "null_value");
        
        // Add the references object to the text object
        cJSON_AddItemToObject(text, "references", references);
    }
    
    // Create a minified version of the JSON
    char *json_string = cJSON_Print(root);
    if (json_string) {
        // Create a minified version (remove whitespace)
        cJSON_Minify(json_string);
        
        // Write the minified JSON to a file
        FILE *out_file = fopen("output_file", "wb");
        if (out_file) {
            fwrite(json_string, strlen(json_string), 1, out_file);
            assert_file_closed(&out_file);;
        }
        
        // Free the JSON string
        cJSON_free(json_string);
    }
    
    // Create a FILE* for the input data using fmemopen
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        // Get file descriptor
        int fd = fuzz_fileno(in_file);
        (void)fd; // Use fd to avoid unused variable warning
        
        // Read and parse the data again
        char *buffer = (char *)malloc(size + 1);
        if (buffer) {
            fread(buffer, 1, size, in_file);
            buffer[size] = '\0';
            
            // Parse with different options
            const char *parse_end;
            cJSON *root2 = cJSON_ParseWithOpts(buffer, &parse_end, 0);
            if (root2) {
                cJSON_Delete(root2);
            }
            free(buffer);
        }
        assert_file_closed(&in_file);;
    }
    
    // Cleanup
    cJSON_Delete(root);
    free(input_string);
    
    // Try to parse the original input again with different method
    if (size > 0) {
        char *temp_buf = (char *)malloc(size);
        if (temp_buf) {
            memcpy(temp_buf, data, size);
            // Attempt to parse as raw bytes
            cJSON *temp_json = cJSON_ParseWithLength(temp_buf, size);
            if (temp_json) {
                cJSON_Delete(temp_json);
            }
            free(temp_buf);
        }
    }
    
    assert_file_closed(&in_file);
	return 0;
}