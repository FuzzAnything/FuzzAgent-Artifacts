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
//<ID> 129
//<Prompt> []
/*<Combination>: [
*/
//<score> 20, nr_unique_branch: 1
//<Quality> {"density":20,"unique_branches":{"print_value":[[1454,17,1454,31,0,0,4,0]]},"library_calls":["cJSON_ParseWithLength","cJSON_CreateRaw","cJSON_AddRawToObject","cJSON_CreateTrue","cJSON_AddItemToObject","cJSON_AddArrayToObject","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_DeleteItemFromArray","cJSON_GetObjectItem","cJSON_GetObjectItem","cJSON_GetObjectItem","cJSON_IsNumber","cJSON_SetNumberHelper","cJSON_PrintBuffered","cJSON_PrintPreallocated","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_ParseWithLength","cJSON_CreateRaw","cJSON_AddRawToObject","cJSON_CreateTrue","cJSON_AddItemToObject","cJSON_AddArrayToObject","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_DeleteItemFromArray","cJSON_GetObjectItem","cJSON_GetObjectItem","cJSON_GetObjectItem","cJSON_IsNumber","cJSON_SetNumberHelper","cJSON_PrintBuffered","cJSON_PrintPreallocated","cJSON_Delete","cJSON_Delete"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_26(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Parse input JSON using data buffer
    cJSON *root = cJSON_ParseWithLength(reinterpret_cast<const char*>(data), size);
    if (root == nullptr) {
        return 0; // Invalid JSON input
    }

    // 2. Create a raw JSON object
    cJSON *raw_obj = cJSON_CreateRaw("\"raw_value\"");
    
    // 3. Add raw object to main object
    cJSON_AddRawToObject(root, "rawField", "\"raw_string\"");
    
    // 4. Create a true value and add it
    cJSON *true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "trueField", true_item);
    
    // 5. Add an array to object
    cJSON *new_array = cJSON_AddArrayToObject(root, "newArray");
    
    // Add some items to the array so we can delete one
    cJSON_AddItemToArray(new_array, cJSON_CreateNumber(1.0));
    cJSON_AddItemToArray(new_array, cJSON_CreateNumber(2.0));
    cJSON_AddItemToArray(new_array, cJSON_CreateNumber(3.0));
    
    // 6. Delete item from array (delete middle item)
    cJSON_DeleteItemFromArray(new_array, 1);
    
    // 7. Use SetNumberHelper on an existing number field
    // First find/create a number item
    cJSON *window_obj = cJSON_GetObjectItem(root, "widget");
    if (window_obj) {
        cJSON *window_data = cJSON_GetObjectItem(window_obj, "window");
        if (window_data) {
            cJSON *width_item = cJSON_GetObjectItem(window_data, "width");
            if (width_item && cJSON_IsNumber(width_item)) {
                cJSON_SetNumberHelper(width_item, 600.0);
            }
        }
    }
    
    // 8. Print using PrintBuffered
    int buffer_size = 4096;
    char *buffered_output = cJSON_PrintBuffered(root, buffer_size, true);
    if (buffered_output) {
        free(buffered_output);
    }
    
    // 9. Print using PrintPreallocated
    char prealloc_buffer[2048];
    cJSON_PrintPreallocated(root, prealloc_buffer, sizeof(prealloc_buffer), true);
    
    // 10. Create a FILE* for input (though not strictly needed for these APIs)
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        // Could read from file if needed, but APIs above don't require FILE*
        int fd = fuzz_fileno(in_file);
        assert_file_closed(&in_file);;
    }
    
    // 11. Create output file if needed
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        int out_fd = fuzz_fileno(out_file);
        assert_file_closed(&out_file);;
    }
    
    // 12. Clean up
    cJSON_Delete(raw_obj);  // Delete the raw object we created
    cJSON_Delete(root);     // Delete main object (frees all children)
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}