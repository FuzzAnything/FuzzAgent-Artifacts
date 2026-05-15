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
//<ID> 70
//<Prompt> []
/*<Combination>: [
*/
//<score> 19, nr_unique_branch: 0
//<Quality> {"density":19,"unique_branches":{},"library_calls":["cJSON_malloc","cJSON_ParseWithLength","cJSON_free","cJSON_IsObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsString","cJSON_CreateTrue","cJSON_IsTrue","cJSON_Delete","cJSON_AddFalseToObject","cJSON_AddArrayToObject","cJSON_AddItemReferenceToArray","cJSON_AddStringToObject","cJSON_DeleteItemFromObjectCaseSensitive","cJSON_Print","cJSON_PrintUnformatted","cJSON_free","cJSON_free","cJSON_Delete","cJSON_free"],"critical_calls":["cJSON_malloc","cJSON_ParseWithLength","cJSON_IsObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsString","cJSON_CreateTrue","cJSON_IsTrue","cJSON_Delete","cJSON_AddFalseToObject","cJSON_AddArrayToObject","cJSON_AddItemReferenceToArray","cJSON_AddStringToObject","cJSON_DeleteItemFromObjectCaseSensitive","cJSON_Print","cJSON_PrintUnformatted","cJSON_free","cJSON_free","cJSON_Delete","cJSON_free"],"visited":0}
/**/


// Include cJSON header

extern "C" int LLVMFuzzerTestOneInput_23(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early return for empty input
    if (size == 0 || data == nullptr) {
        return 0;
    }
    
    // 1. First use cJSON_malloc to allocate a buffer for our data
    size_t alloc_size = size + 1;  // +1 for null terminator
    char *input_buffer = (char *)cJSON_malloc(alloc_size);
    if (!input_buffer) {
        return 0;
    }
    
    // Copy input data into buffer and null-terminate
    memcpy(input_buffer, data, size);
    input_buffer[size] = '\0';
    
    // 2. Parse the JSON data using cJSON_ParseWithLength
    cJSON *parsed_json = cJSON_ParseWithLength(input_buffer, size);
    
    // 3. Check if parsing was successful
    if (parsed_json == nullptr) {
        cJSON_free(input_buffer);
        return 0;
    }
    
    // 4. Use cJSON_IsObject to check if the parsed JSON is an object
    cJSON_bool is_object = cJSON_IsObject(parsed_json);
    
    // 5. If it's an object, try to get an item from it using cJSON_GetObjectItemCaseSensitive
    cJSON *glossary_item = nullptr;
    if (is_object) {
        glossary_item = cJSON_GetObjectItemCaseSensitive(parsed_json, "glossary");
        
        // 6. Check if the glossary item is also an object
        if (glossary_item && cJSON_IsObject(glossary_item)) {
            // 7. Check if a specific field is a string using cJSON_IsString
            cJSON *title_item = cJSON_GetObjectItemCaseSensitive(glossary_item, "title");
            cJSON_bool is_string = cJSON_IsString(title_item);
            
            // 8. Check if a true value exists using cJSON_IsTrue
            // Since our JSON doesn't have a true boolean, we'll create one and check it
            cJSON *true_item = cJSON_CreateTrue();
            cJSON_bool is_true = cJSON_IsTrue(true_item);
            cJSON_Delete(true_item);  // Clean up the temporary item
            
            // 9. Add a false boolean to the glossary object using cJSON_AddFalseToObject
            cJSON *added_false = cJSON_AddFalseToObject(glossary_item, "added_false_key");
            
            // 10. Add an array to the glossary object using cJSON_AddArrayToObject
            cJSON *added_array = cJSON_AddArrayToObject(glossary_item, "added_array_key");
            
            // 11. If we successfully added an array, add a reference to an item in it
            if (added_array && added_false) {
                cJSON_bool ref_added = cJSON_AddItemReferenceToArray(added_array, added_false);
                (void)ref_added; // Mark as used to avoid unused variable warning
            }
            
            // 12. Delete an item from the object using cJSON_DeleteItemFromObjectCaseSensitive
            // First add a temporary item to delete
            cJSON *temp_item = cJSON_AddStringToObject(glossary_item, "temp_to_delete", "value");
            if (temp_item) {
                cJSON_DeleteItemFromObjectCaseSensitive(glossary_item, "temp_to_delete");
            }
        }
    }
    
    // 13. Print the modified JSON using both formatted and unformatted versions
    char *printed_json = cJSON_Print(parsed_json);
    char *unformatted_json = cJSON_PrintUnformatted(parsed_json);
    
    // 14. Write output to a file using FILE* operations as specified
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        if (printed_json) {
            fwrite(printed_json, strlen(printed_json), 1, out_file);
        }
        assert_file_closed(&out_file);;
    }
    
    // 15. Clean up all allocated resources
    
    // Free the printed strings
    if (printed_json) {
        cJSON_free(printed_json);
    }
    if (unformatted_json) {
        cJSON_free(unformatted_json);
    }
    
    // Delete the parsed JSON structure
    cJSON_Delete(parsed_json);
    
    // Free the input buffer we allocated with cJSON_malloc
    cJSON_free(input_buffer);
    
    assert_file_closed(&out_file);
	return 0;
}