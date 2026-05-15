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
//<ID> 95
//<Prompt> []
/*<Combination>: [
*/
//<score> 19, nr_unique_branch: 1
//<Quality> {"density":19,"unique_branches":{"print_array":[[1606,9,1606,52,0,0,4,0]]},"library_calls":["cJSON_Parse","cJSON_CreateObject","cJSON_CreateFalse","cJSON_CreateTrue","cJSON_CreateNull","cJSON_CreateIntArray","cJSON_CreateDoubleArray","cJSON_CreateStringReference","cJSON_IsArray","cJSON_CreateArrayReference","cJSON_GetObjectItem","cJSON_IsArray","cJSON_CreateArrayReference","cJSON_CreateArrayReference","cJSON_CreateArrayReference","cJSON_AddItemToObjectCS","cJSON_Duplicate","cJSON_CreateNull","cJSON_AddItemToObjectCS","cJSON_AddItemToObjectCS","cJSON_AddItemToObjectCS","cJSON_AddItemToObjectCS","cJSON_AddItemToObjectCS","cJSON_AddItemToObjectCS","cJSON_AddItemToObjectCS","cJSON_IsTrue","cJSON_CreateBool","cJSON_AddItemToObjectCS","cJSON_Print","cJSON_free","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_Parse","cJSON_CreateObject","cJSON_CreateFalse","cJSON_CreateTrue","cJSON_CreateNull","cJSON_CreateIntArray","cJSON_CreateDoubleArray","cJSON_CreateStringReference","cJSON_IsArray","cJSON_GetObjectItem","cJSON_IsArray","cJSON_CreateArrayReference","cJSON_AddItemToObjectCS","cJSON_CreateNull","cJSON_Duplicate","cJSON_AddItemToObjectCS","cJSON_AddItemToObjectCS","cJSON_AddItemToObjectCS","cJSON_AddItemToObjectCS","cJSON_AddItemToObjectCS","cJSON_AddItemToObjectCS","cJSON_AddItemToObjectCS","cJSON_IsTrue","cJSON_CreateBool","cJSON_AddItemToObjectCS","cJSON_Print","cJSON_free","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_27(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // Create a null-terminated string from input data
    char *input_str = new char[size + 1];
    memcpy(input_str, data, size);
    input_str[size] = '\0';

    // Parse JSON from input
    cJSON *parsed = cJSON_Parse(input_str);
    
    // Create base objects
    cJSON *root = cJSON_CreateObject();
    cJSON *false_item = cJSON_CreateFalse();
    cJSON *true_item = cJSON_CreateTrue();
    cJSON *null_item = cJSON_CreateNull();

    // Create arrays
    int int_numbers[] = {1, 2, 3, 4, 5};
    double double_numbers[] = {1.1, 2.2, 3.3, 4.4, 5.5};
    
    cJSON *int_array = cJSON_CreateIntArray(int_numbers, 5);
    cJSON *double_array = cJSON_CreateDoubleArray(double_numbers, 5);
    
    // Create string reference
    const char *test_string = "test string";
    cJSON *string_ref = cJSON_CreateStringReference(test_string);
    
    // Create array reference from parsed input (if available)
    cJSON *array_ref = NULL;
    if (parsed) {
        // Try to find an array in the parsed JSON
        if (cJSON_IsArray(parsed)) {
            array_ref = cJSON_CreateArrayReference(parsed);
        } else {
            // Look for arrays in the object
            cJSON *servlet_array = cJSON_GetObjectItem(parsed, "servlet");
            if (servlet_array && cJSON_IsArray(servlet_array)) {
                array_ref = cJSON_CreateArrayReference(servlet_array);
            } else {
                // Create a dummy array reference from the int_array
                array_ref = cJSON_CreateArrayReference(int_array);
            }
        }
    } else {
        // Create reference to int_array as fallback
        array_ref = cJSON_CreateArrayReference(int_array);
    }

    // Add items to root object using case-sensitive addition
    // Use cJSON_AddItemToObjectCS for at least one item
    if (root) {
        cJSON_AddItemToObjectCS(root, "parsed_input", parsed ? cJSON_Duplicate(parsed, 1) : cJSON_CreateNull());
        cJSON_AddItemToObjectCS(root, "false_item", false_item);
        cJSON_AddItemToObjectCS(root, "true_item", true_item);
        cJSON_AddItemToObjectCS(root, "null_item", null_item);
        cJSON_AddItemToObjectCS(root, "int_array", int_array);
        cJSON_AddItemToObjectCS(root, "double_array", double_array);
        cJSON_AddItemToObjectCS(root, "string_ref", string_ref);
        cJSON_AddItemToObjectCS(root, "array_ref", array_ref);
        
        // Check if true_item is actually true
        if (cJSON_IsTrue(true_item)) {
            // Add additional verification item
            cJSON *verification = cJSON_CreateBool(1);
            cJSON_AddItemToObjectCS(root, "verification", verification);
        }
        
        // Print and free the created JSON
        char *printed = cJSON_Print(root);
        if (printed) {
            // Write to file using FILE* as requested
            FILE *out_file = fopen("output_file", "wb");
            if (out_file) {
                fwrite(printed, 1, strlen(printed), out_file);
                assert_file_closed(&out_file);;
                
                // Demonstrate file descriptor usage
                FILE *in_file = fmemopen((void *)data, size, "rb");
                if (in_file) {
                    int fd = fuzz_fileno(in_file);
                    // Read and verify file descriptor is valid (just check)
                    if (fd >= 0) {
                        // Optional: read something from the file descriptor
                        char buffer[16];
                        fread(buffer, 1, sizeof(buffer), in_file);
                    }
                    assert_file_closed(&in_file);;
                }
            }
            cJSON_free(printed);
        }
    }

    // Cleanup
    if (root) {
        cJSON_Delete(root);
    } else {
        // If root creation failed, clean up individual items
        if (false_item) cJSON_Delete(false_item);
        if (true_item) cJSON_Delete(true_item);
        if (null_item) cJSON_Delete(null_item);
        if (int_array) cJSON_Delete(int_array);
        if (double_array) cJSON_Delete(double_array);
        // Note: string_ref and array_ref don't need deletion as they are references
    }
    
    if (parsed) {
        cJSON_Delete(parsed);
    }
    
    delete[] input_str;
    
    return 0;
}