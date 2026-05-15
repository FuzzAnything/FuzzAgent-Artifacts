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
//<ID> 31
//<Prompt> []
/*<Combination>: [
*/
//<score> 9, nr_unique_branch: 0
//<Quality> {"density":18,"unique_branches":{},"library_calls":["cJSON_malloc","cJSON_ParseWithLength","cJSON_free","cJSON_CreateArray","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_AddTrueToObject","cJSON_IsTrue","cJSON_GetObjectItemCaseSensitive","cJSON_IsString","cJSON_GetStringValue","cJSON_AddItemReferenceToArray","cJSON_DeleteItemFromObjectCaseSensitive","cJSON_Print","cJSON_free","cJSON_Delete","cJSON_Delete","cJSON_free"],"critical_calls":["cJSON_malloc","cJSON_ParseWithLength","cJSON_CreateArray","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_AddTrueToObject","cJSON_IsTrue","cJSON_GetObjectItemCaseSensitive","cJSON_IsString","cJSON_GetStringValue","cJSON_AddItemReferenceToArray","cJSON_DeleteItemFromObjectCaseSensitive","cJSON_Print","cJSON_free","cJSON_Delete","cJSON_Delete","cJSON_free"],"visited":1}
/**/


// Include the cJSON header

extern "C" int LLVMFuzzerTestOneInput_13(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create a FILE* from the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Read the entire input file into a buffer
    fseek(in_file, 0, SEEK_END);
    long file_size = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);
    
    char *json_buffer = (char *)cJSON_malloc(file_size + 1);
    if (!json_buffer) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    size_t bytes_read = fread(json_buffer, 1, file_size, in_file);
    json_buffer[bytes_read] = '\0';
    assert_file_closed(&in_file);;
    
    // Parse the JSON data
    cJSON *root = cJSON_ParseWithLength(json_buffer, bytes_read);
    if (!root) {
        cJSON_free(json_buffer);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create a new array
    cJSON *new_array = cJSON_CreateArray();
    
    // Get the glossary object
    cJSON *glossary = cJSON_GetObjectItemCaseSensitive(root, "glossary");
    
    if (glossary && cJSON_IsObject(glossary)) {
        // Create a true item and add it to the glossary object
        cJSON *true_item = cJSON_AddTrueToObject(glossary, "fuzzed_true_item");
        
        // Check if the newly added item is true
        if (true_item && cJSON_IsTrue(true_item)) {
            // Get the string value of the title (if it exists)
            cJSON *title = cJSON_GetObjectItemCaseSensitive(glossary, "title");
            if (title && cJSON_IsString(title)) {
                char *title_value = cJSON_GetStringValue(title);
                // title_value can be used here if needed
            }
            
            // Add a reference of the true item to the new array
            cJSON_AddItemReferenceToArray(new_array, true_item);
            
            // Delete an item from the glossary object (case sensitive)
            cJSON_DeleteItemFromObjectCaseSensitive(glossary, "title");
        }
    }
    
    // Create an output file
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // Print the modified JSON to the output file
        char *printed_json = cJSON_Print(root);
        if (printed_json) {
            fwrite(printed_json, strlen(printed_json), 1, out_file);
            cJSON_free(printed_json);
        }
        assert_file_closed(&out_file);;
    }
    
    // Clean up all allocated resources
    cJSON_Delete(new_array);
    cJSON_Delete(root);
    cJSON_free(json_buffer);
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}