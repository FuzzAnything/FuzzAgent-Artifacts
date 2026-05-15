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
//<ID> 51
//<Prompt> []
/*<Combination>: [
*/
//<score> 26, nr_unique_branch: 10
//<Quality> {"density":26,"unique_branches":{"cJSON_CreateIntArray":[[2628,9,2628,20,0,0,4,1],[2628,24,2628,41,0,0,4,1],[2635,16,2635,17,0,0,4,0],[2635,21,2635,40,0,0,4,0],[2635,21,2635,40,0,0,4,1],[2638,13,2638,15,0,0,4,1],[2643,12,2643,14,0,0,4,0],[2643,12,2643,14,0,0,4,1],[2654,9,2654,10,0,0,4,0],[2654,14,2654,22,0,0,4,0]]},"library_calls":["cJSON_malloc","cJSON_Parse","cJSON_free","cJSON_CreateObject","cJSON_Delete","cJSON_free","cJSON_IsArray","cJSON_CreateIntArray","cJSON_AddItemToObject","cJSON_CreateDoubleArray","cJSON_AddItemToObject","cJSON_AddObjectToObject","cJSON_AddBoolToObject","cJSON_CreateStringReference","cJSON_AddItemToObject","cJSON_CreateNumber","cJSON_SetNumberHelper","cJSON_AddItemToObject","cJSON_IsArray","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_CreateArray","cJSON_AddItemReferenceToArray","cJSON_AddItemToObject","cJSON_GetObjectItem","cJSON_CreateString","cJSON_ReplaceItemInObject","cJSON_Print","cJSON_free","cJSON_Delete","cJSON_Delete","cJSON_free"],"critical_calls":["cJSON_malloc","cJSON_Parse","cJSON_CreateObject","cJSON_IsArray","cJSON_CreateIntArray","cJSON_AddItemToObject","cJSON_CreateDoubleArray","cJSON_AddItemToObject","cJSON_AddObjectToObject","cJSON_AddBoolToObject","cJSON_CreateStringReference","cJSON_AddItemToObject","cJSON_CreateNumber","cJSON_SetNumberHelper","cJSON_AddItemToObject","cJSON_GetArraySize","cJSON_IsArray","cJSON_GetArrayItem","cJSON_CreateArray","cJSON_AddItemReferenceToArray","cJSON_AddItemToObject","cJSON_GetObjectItem","cJSON_CreateString","cJSON_ReplaceItemInObject","cJSON_Print","cJSON_free","cJSON_Delete","cJSON_Delete","cJSON_free"],"visited":1}
/*To create a comprehensive fuzzer for the cJSON library using the specified APIs, I'll implement a fuzz driver that:

1. Parses the input JSON data
2. Performs various operations using the specified APIs
3. Handles memory allocation and cleanup properly

Here's the complete implementation:

*/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_17(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Ensure we have valid input
    if (size == 0 || data == nullptr) {
        return 0;
    }

    // Create a null-terminated string from the input data
    char *json_string = static_cast<char*>(cJSON_malloc(size + 1));
    if (!json_string) {
        return 0;
    }
    memcpy(json_string, data, size);
    json_string[size] = '\0';

    // 1. Parse the JSON string
    cJSON *root = cJSON_Parse(json_string);
    if (!root) {
        cJSON_free(json_string);
        return 0;
    }

    // 2. Create a new object to work with
    cJSON *new_object = cJSON_CreateObject();
    if (!new_object) {
        cJSON_Delete(root);
        cJSON_free(json_string);
        return 0;
    }

    // 3. Check if the root is an array using cJSON_IsArray
    if (cJSON_IsArray(root)) {
        // Create integer array
        int int_numbers[] = {1, 2, 3, 4, 5};
        cJSON *int_array = cJSON_CreateIntArray(int_numbers, 5);
        if (int_array) {
            cJSON_AddItemToObject(new_object, "int_array", int_array);
        }

        // Create double array
        double double_numbers[] = {1.1, 2.2, 3.3, 4.4, 5.5};
        cJSON *double_array = cJSON_CreateDoubleArray(double_numbers, 5);
        if (double_array) {
            cJSON_AddItemToObject(new_object, "double_array", double_array);
        }
    }

    // 4. Add an object to the new object
    cJSON *nested_object = cJSON_AddObjectToObject(new_object, "nested_object");
    if (nested_object) {
        // Add a boolean to the nested object
        cJSON_AddBoolToObject(nested_object, "is_valid", 1);
        
        // Add a string reference
        const char *ref_string = "reference_string";
        cJSON *string_ref = cJSON_CreateStringReference(ref_string);
        if (string_ref) {
            cJSON_AddItemToObject(nested_object, "string_ref", string_ref);
        }
        
        // Set number helper
        cJSON *number_item = cJSON_CreateNumber(0.0);
        if (number_item) {
            cJSON_SetNumberHelper(number_item, 42.5);
            cJSON_AddItemToObject(nested_object, "number_item", number_item);
        }
    }

    // 5. If root has array items, work with array references
    if (cJSON_IsArray(root) && cJSON_GetArraySize(root) > 0) {
        cJSON *first_item = cJSON_GetArrayItem(root, 0);
        if (first_item) {
            // Create a new array for testing
            cJSON *test_array = cJSON_CreateArray();
            if (test_array) {
                // Add item reference to array
                cJSON_AddItemReferenceToArray(test_array, first_item);
                
                // Add the test array to our object
                cJSON_AddItemToObject(new_object, "test_array", test_array);
            }
        }
    }

    // 6. Replace an item in the new object if it exists
    cJSON *existing_item = cJSON_GetObjectItem(new_object, "nested_object");
    if (existing_item) {
        // Create replacement
        cJSON *replacement = cJSON_CreateString("replaced_item");
        if (replacement) {
            cJSON_ReplaceItemInObject(new_object, "nested_object", replacement);
        }
    }

    // 7. Write to output file using FILE*
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        char *printed = cJSON_Print(new_object);
        if (printed) {
            fwrite(printed, strlen(printed), 1, out_file);
            cJSON_free(printed);
        }
        assert_file_closed(&out_file);;
    }

    // 8. Read from input file using FILE* (just for demonstration)
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        char buffer[256];
        while (fgets(buffer, sizeof(buffer), in_file)) {
            // Process buffer if needed
        }
        assert_file_closed(&in_file);;
    }

    // 9. Use file descriptors (just for demonstration)
    if (out_file) {
        int fd_out = fuzz_fileno(out_file);
        // File descriptor could be used here
        (void)fd_out; // Mark as used
    }

    // Clean up
    cJSON_Delete(root);
    cJSON_Delete(new_object);
    cJSON_free(json_string);

    assert_file_closed(&in_file);
	assert_file_closed(&out_file);
	return 0;
}