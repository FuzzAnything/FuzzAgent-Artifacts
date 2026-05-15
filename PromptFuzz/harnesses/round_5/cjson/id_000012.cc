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
//<ID> 25
//<Prompt> []
/*<Combination>: [
*/
//<score> 20, nr_unique_branch: 2
//<Quality> {"density":20,"unique_branches":{"print_string_ptr":[[969,9,969,22,0,0,4,0],[972,13,972,27,0,0,4,1]]},"library_calls":["cJSON_ParseWithLength","cJSON_CreateObject","cJSON_Delete","cJSON_AddArrayToObject","cJSON_AddObjectToObject","cJSON_CreateObjectReference","cJSON_AddItemToArray","cJSON_CreateArray","cJSON_AddItemToArray","cJSON_AddNullToObject","cJSON_IsNull","cJSON_HasObjectItem","cJSON_DetachItemFromObject","cJSON_AddItemToObject","cJSON_DetachItemViaPointer","cJSON_Delete","cJSON_PrintUnformatted","cJSON_free","cJSON_PrintUnformatted","cJSON_free","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_ParseWithLength","cJSON_CreateObject","cJSON_AddArrayToObject","cJSON_AddObjectToObject","cJSON_CreateObjectReference","cJSON_AddItemToArray","cJSON_CreateArray","cJSON_AddItemToArray","cJSON_AddNullToObject","cJSON_IsNull","cJSON_HasObjectItem","cJSON_DetachItemFromObject","cJSON_AddItemToObject","cJSON_DetachItemViaPointer","cJSON_Delete","cJSON_PrintUnformatted","cJSON_free","cJSON_PrintUnformatted","cJSON_free","cJSON_Delete","cJSON_Delete"],"visited":1}
/*Looking at the provided JSON data and the specific APIs to use, I'll create a fuzz driver that manipulates the JSON structure by creating a new object, adding/removing elements, and verifying properties.

*/


extern "C" int LLVMFuzzerTestOneInput_12(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Parse the input JSON data
    cJSON *parsed_json = cJSON_ParseWithLength(reinterpret_cast<const char*>(data), size);
    if (parsed_json == nullptr) {
        return 0; // Invalid JSON input, exit early
    }

    // Create a new object to work with
    cJSON *new_object = cJSON_CreateObject();
    if (new_object == nullptr) {
        cJSON_Delete(parsed_json);
        return 0;
    }

    // 1. Add an array to the new object
    cJSON *new_array = cJSON_AddArrayToObject(new_object, "fuzz_array");
    
    // 2. Add an object to the new object (nested structure)
    cJSON *nested_object = cJSON_AddObjectToObject(new_object, "nested");
    
    // 3. Create an object reference to the parsed JSON
    cJSON *object_ref = cJSON_CreateObjectReference(parsed_json);
    if (object_ref) {
        // Add the reference to our array
        cJSON_AddItemToArray(new_array, object_ref);
    }
    
    // 4. Create a new array using cJSON_CreateArray()
    cJSON *standalone_array = cJSON_CreateArray();
    if (standalone_array) {
        // Add the standalone array to our main array
        cJSON_AddItemToArray(new_array, standalone_array);
    }
    
    // 5. Add a null item to the new object
    cJSON *null_item = cJSON_AddNullToObject(new_object, "null_field");
    
    // 6. Check if the null item is indeed null
    if (null_item) {
        cJSON_IsNull(null_item); // Verify it's null
    }
    
    // 7. Detach an item from the parsed JSON object if it exists
    cJSON *detached_item = nullptr;
    if (cJSON_HasObjectItem(parsed_json, "widget")) {
        detached_item = cJSON_DetachItemFromObject(parsed_json, "widget");
    }
    
    // 8. If we found and detached an item, add it to our new object
    if (detached_item) {
        cJSON_AddItemToObject(new_object, "detached_widget", detached_item);
        
        // 9. Now detach it via pointer (from the new object)
        cJSON *detached_via_pointer = cJSON_DetachItemViaPointer(new_object, detached_item);
        if (detached_via_pointer) {
            cJSON_Delete(detached_via_pointer); // Clean up
        }
    }
    
    // 10. Print the new object in unformatted JSON
    char *unformatted_json = cJSON_PrintUnformatted(new_object);
    if (unformatted_json) {
        // For fuzzing purposes, we could write this to a file or process it
        // But for now, we'll just ensure we use the output
        FILE *out_file = fopen("output_file", "wb");
        if (out_file) {
            fwrite(unformatted_json, 1, strlen(unformatted_json), out_file);
            assert_file_closed(&out_file);;
        }
        cJSON_free(unformatted_json);
    }
    
    // 11. Also print the modified parsed JSON
    char *modified_parsed = cJSON_PrintUnformatted(parsed_json);
    if (modified_parsed) {
        // Use a FILE* as instructed
        FILE *out_file2 = fopen("output_file2", "wb");
        if (out_file2) {
            fwrite(modified_parsed, 1, strlen(modified_parsed), out_file2);
            assert_file_closed(&out_file2);;
        }
        cJSON_free(modified_parsed);
    }
    
    // 12. Demonstrate file descriptor usage as instructed
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        int fd = fuzz_fileno(in_file); // Get file descriptor
        // Could read from the file descriptor here if needed
        assert_file_closed(&in_file);;
    }
    
    // Clean up all allocated resources
    cJSON_Delete(parsed_json);
    cJSON_Delete(new_object);
    
    assert_file_closed(&in_file);
	return 0;
}