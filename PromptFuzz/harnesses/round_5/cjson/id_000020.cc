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
//<ID> 60
//<Prompt> []
/*<Combination>: [
*/
//<score> 22, nr_unique_branch: 0
//<Quality> {"density":22,"unique_branches":{},"library_calls":["cJSON_Parse","cJSON_IsArray","cJSON_GetArraySize","cJSON_CreateObject","cJSON_CreateTrue","cJSON_AddItemToObject","cJSON_AddStringToObject","cJSON_AddNumberToObject","cJSON_SetNumberHelper","cJSON_GetArrayItem","cJSON_IsBool","cJSON_IsObject","cJSON_GetObjectItem","cJSON_GetObjectItem","cJSON_GetObjectItem","cJSON_CreateStringArray","cJSON_AddItemToObjectCS","cJSON_CreateArrayReference","cJSON_AddItemToObject","cJSON_Print","cJSON_free","cJSON_Delete","cJSON_ParseWithLength","cJSON_Delete","cJSON_PrintUnformatted","cJSON_free","cJSON_PrintPreallocated","cJSON_Delete","cJSON_CreateArray","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_AddItemToArray","cJSON_CreateString","cJSON_AddItemToArray","cJSON_CreateBool","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_IsBool","cJSON_Delete"],"critical_calls":["cJSON_Parse","cJSON_IsArray","cJSON_GetArraySize","cJSON_CreateObject","cJSON_CreateTrue","cJSON_AddItemToObject","cJSON_AddStringToObject","cJSON_AddNumberToObject","cJSON_SetNumberHelper","cJSON_GetArrayItem","cJSON_IsBool","cJSON_IsObject","cJSON_GetObjectItem","cJSON_GetObjectItem","cJSON_GetObjectItem","cJSON_CreateStringArray","cJSON_AddItemToObjectCS","cJSON_CreateArrayReference","cJSON_AddItemToObject","cJSON_Print","cJSON_free","cJSON_Delete","cJSON_ParseWithLength","cJSON_Delete","cJSON_PrintUnformatted","cJSON_free","cJSON_PrintPreallocated","cJSON_Delete","cJSON_CreateArray","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_AddItemToArray","cJSON_CreateString","cJSON_AddItemToArray","cJSON_CreateBool","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_IsBool","cJSON_Delete"],"visited":0}
/*Looking at the provided APIs and the JSON input, I'll create a fuzz driver that parses the JSON data, manipulates it using the specified APIs, and tests various cjson operations. Here's my step-by-step implementation:

*/


// Include the cJSON header

extern "C" int LLVMFuzzerTestOneInput_20(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Parse the input JSON data
    if (size == 0) {
	return 0;
	}
	
    
    // Create null-terminated string from input data
    char *json_string = (char *)malloc(size + 1);
    if (!json_string) {
	return 0;
	}
	
    memcpy(json_string, data, size);
    json_string[size] = '\0';
    
    // Parse JSON using cJSON_Parse (first API)
    cJSON *parsed_json = cJSON_Parse(json_string);
    free(json_string);
    
    if (!parsed_json) {
        // Clean up any partial parsing
        const char *error_ptr = cJSON_GetErrorPtr();
        return 0;
    }
    
    // 2. Check if parsed JSON is an array (second API)
    cJSON_bool is_array = cJSON_IsArray(parsed_json);
    
    // 3. If it's an array, get its size and items
    if (is_array) {
        int array_size = cJSON_GetArraySize(parsed_json);
        
        // Create a new object (third API)
        cJSON *new_object = cJSON_CreateObject();
        
        // Create a true value (fourth API)
        cJSON *true_value = cJSON_CreateTrue();
        
        // Add true value to object (fifth API)
        cJSON_AddItemToObject(new_object, "fuzzed_bool", true_value);
        
        // Add string to object (sixth API)
        cJSON_AddStringToObject(new_object, "fuzzed_string", "fuzz_test");
        
        // Add number to object (seventh API)
        cJSON *number_item = cJSON_AddNumberToObject(new_object, "fuzzed_number", 42.5);
        
        // Use SetNumberHelper on the number item (eighth API)
        if (number_item) {
            double new_number = cJSON_SetNumberHelper(number_item, 99.9);
        }
        
        // Process each item in the array
        for (int i = 0; i < array_size && i < 10; i++) {
            cJSON *array_item = cJSON_GetArrayItem(parsed_json, i);
            
            if (array_item) {
                // Check if item is a boolean (ninth API)
                cJSON_bool is_bool = cJSON_IsBool(array_item);
                
                // Get object items from array elements
                if (cJSON_IsObject(array_item)) {
                    cJSON *city_item = cJSON_GetObjectItem(array_item, "City");
                    cJSON *lat_item = cJSON_GetObjectItem(array_item, "Latitude");
                    cJSON *precision_item = cJSON_GetObjectItem(array_item, "precision");
                    
                    // Create a string array (tenth API)
                    const char *test_strings[] = {"test1", "test2", "test3"};
                    cJSON *string_array = cJSON_CreateStringArray(test_strings, 3);
                    
                    // Add string array to new object using AddItemToObjectCS (eleventh API)
                    cJSON_AddItemToObjectCS(new_object, "string_array", string_array);
                    
                    // Create array reference (twelfth API)
                    cJSON *array_ref = cJSON_CreateArrayReference(parsed_json);
                    
                    // Add array reference to new object (thirteenth API)
                    cJSON_AddItemToObject(new_object, "array_reference", array_ref);
                    
                    // Print the new object (fourteenth API)
                    char *printed_json = cJSON_Print(new_object);
                    if (printed_json) {
                        // Free the printed string (fifteenth API)
                        cJSON_free(printed_json);
                    }
                    
                    // Break after first object to avoid excessive processing
                    break;
                }
            }
        }
        
        // Clean up
        cJSON_Delete(new_object);
    }
    
    // 4. Test with alternative parsing methods using FILE operations
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        // Get file descriptor (for demonstration)
        int fd = fuzz_fileno(in_file);
        
        // Read data from the file
        fseek(in_file, 0, SEEK_END);
        long file_size = ftell(in_file);
        fseek(in_file, 0, SEEK_SET);
        
        if (file_size > 0) {
            char *file_data = (char *)malloc(file_size + 1);
            if (file_data) {
                fread(file_data, 1, file_size, in_file);
                file_data[file_size] = '\0';
                
                // Parse with length option
                cJSON *file_parsed = cJSON_ParseWithLength(file_data, file_size);
                if (file_parsed) {
                    cJSON_Delete(file_parsed);
                }
                
                free(file_data);
            }
        }
        
        assert_file_closed(&in_file);;
    }
    
    // 5. Create output file for testing
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        int out_fd = fuzz_fileno(out_file);
        fwrite(data, 1, size, out_file);
        assert_file_closed(&out_file);;
    }
    
    // 6. Test printing the original parsed JSON
    if (parsed_json) {
        char *unformatted = cJSON_PrintUnformatted(parsed_json);
        if (unformatted) {
            cJSON_free(unformatted);
        }
        
        // Test buffered printing
        char print_buffer[1024];
        cJSON_PrintPreallocated(parsed_json, print_buffer, sizeof(print_buffer), 1);
        
        // Clean up parsed JSON
        cJSON_Delete(parsed_json);
    }
    
    // 7. Test creating additional JSON structures
    cJSON *test_array = cJSON_CreateArray();
    if (test_array) {
        cJSON_AddItemToArray(test_array, cJSON_CreateNumber(1.0));
        cJSON_AddItemToArray(test_array, cJSON_CreateString("test"));
        cJSON_AddItemToArray(test_array, cJSON_CreateBool(1));
        
        // Test array operations
        int test_array_size = cJSON_GetArraySize(test_array);
        for (int i = 0; i < test_array_size; i++) {
            cJSON *item = cJSON_GetArrayItem(test_array, i);
            if (item) {
                cJSON_bool item_is_bool = cJSON_IsBool(item);
            }
        }
        
        cJSON_Delete(test_array);
    }
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}