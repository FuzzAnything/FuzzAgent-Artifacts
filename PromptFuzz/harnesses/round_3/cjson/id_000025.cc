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
//<ID> 126
//<Prompt> []
/*<Combination>: [
*/
//<score> 45, nr_unique_branch: 6
//<Quality> {"density":18,"unique_branches":{"print":[[1258,9,1258,35,0,0,4,0],[1291,9,1291,31,0,0,4,0],[1297,9,1297,24,0,0,4,1]],"print_object":[[1791,9,1791,52,0,0,4,0]],"cJSON_PrintBuffered":[[1338,9,1338,31,0,0,4,0]],"print_array":[[1606,9,1606,52,0,0,4,0]]},"library_calls":["cJSON_ParseWithLengthOpts","cJSON_CreateObject","cJSON_Delete","cJSON_CreateObject","cJSON_Delete","cJSON_AddArrayToObject","cJSON_Duplicate","cJSON_AddItemToArray","cJSON_CreateArray","cJSON_AddItemToObject","cJSON_malloc","cJSON_CreateDoubleArray","cJSON_AddItemToArray","cJSON_free","cJSON_CreateBool","cJSON_AddItemToObject","cJSON_malloc","cJSON_CreateFloatArray","cJSON_AddItemToArray","cJSON_free","cJSON_CreateArrayReference","cJSON_AddItemToObject","cJSON_malloc","cJSON_AddRawToObject","cJSON_free","cJSON_Print","cJSON_free","cJSON_PrintBuffered","cJSON_free","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_ParseWithLengthOpts","cJSON_CreateObject","cJSON_AddArrayToObject","cJSON_Duplicate","cJSON_AddItemToArray","cJSON_CreateArray","cJSON_AddItemToObject","cJSON_malloc","cJSON_CreateDoubleArray","cJSON_AddItemToArray","cJSON_free","cJSON_CreateBool","cJSON_AddItemToObject","cJSON_malloc","cJSON_CreateFloatArray","cJSON_AddItemToArray","cJSON_free","cJSON_CreateArrayReference","cJSON_AddItemToObject","cJSON_malloc","cJSON_AddRawToObject","cJSON_free","cJSON_Print","cJSON_free","cJSON_PrintBuffered","cJSON_free","cJSON_Delete","cJSON_Delete"],"visited":1}
/*Here's a fuzz driver that uses the specified cJSON APIs to achieve a comprehensive JSON manipulation event:

*/


extern "C" int LLVMFuzzerTestOneInput_25(const uint8_t* f_data, size_t f_size) {
	if(f_size<95) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_7, fuzz_str_sz_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // 1. Parse the input JSON data using cJSON_ParseWithLengthOpts
    const char *parse_end = nullptr;
    cJSON *parsed_json = cJSON_ParseWithLengthOpts(
        reinterpret_cast<const char*>(data), 
        size, 
        &parse_end, 
        fuzz_int32_t_1  // Not requiring null termination
    );
    
    if (!parsed_json) {
        // If parsing fails, try to create some test data
        cJSON *test_object = cJSON_CreateObject();
        if (test_object) {
            cJSON_Delete(test_object);
        }
        return 0;
    }

    // 2. Create a new object for our operations
    cJSON *root_object = cJSON_CreateObject();
    if (!root_object) {
        cJSON_Delete(parsed_json);
        return 0;
    }

    // 3. Add an array to the object using cJSON_AddArrayToObject
    cJSON *new_array = cJSON_AddArrayToObject(root_object, fuzz_str_2);
    
    // 4. Create a duplicate of the parsed JSON using cJSON_Duplicate
    cJSON *duplicate = cJSON_Duplicate(parsed_json, fuzz_int32_t_3); // Deep copy
    if (duplicate) {
        // Add the duplicate to our array
        cJSON_AddItemToArray(new_array, duplicate);
    }

    // 5. Create a new standalone array using cJSON_CreateArray
    cJSON *standalone_array = cJSON_CreateArray();
    if (standalone_array) {
        // Add the standalone array to our root object
        cJSON_AddItemToObject(root_object, fuzz_str_4, standalone_array);
    }

    // 6. Create a double array using cJSON_CreateDoubleArray
    // Use data to create some double values
    const size_t max_double_count = 10;
    size_t double_count = size % max_double_count + 1;
    double *double_values = static_cast<double*>(cJSON_malloc(double_count * sizeof(double)));
    
    if (double_values) {
        for (size_t i = 0; i < double_count && i < size; i++) {
            double_values[i] = static_cast<double>(data[i]) / 256.0;
        }
        cJSON *double_array = cJSON_CreateDoubleArray(double_values, static_cast<int>(double_count));
        if (double_array) {
            cJSON_AddItemToArray(standalone_array, double_array);
        }
        cJSON_free(double_values);
    }

    // 7. Create a boolean using cJSON_CreateBool
    cJSON *bool_item = cJSON_CreateBool(size % 2);
    if (bool_item) {
        cJSON_AddItemToObject(root_object, fuzz_str_5, bool_item);
    }

    // 8. Create a float array using cJSON_CreateFloatArray
    // Use data to create some float values
    const size_t max_float_count = 8;
    size_t float_count = (size / 2) % max_float_count + 1;
    float *float_values = static_cast<float*>(cJSON_malloc(float_count * sizeof(float)));
    
    if (float_values) {
        for (size_t i = 0; i < float_count && i < size/2; i++) {
            float_values[i] = static_cast<float>(data[i * 2]) / 128.0f;
        }
        cJSON *float_array = cJSON_CreateFloatArray(float_values, static_cast<int>(float_count));
        if (float_array) {
            cJSON_AddItemToArray(standalone_array, float_array);
        }
        cJSON_free(float_values);
    }

    // 9. Create an array reference using cJSON_CreateArrayReference
    if (new_array) {
        cJSON *array_ref = cJSON_CreateArrayReference(new_array);
        if (array_ref) {
            cJSON_AddItemToObject(root_object, fuzz_str_6, array_ref);
        }
    }

    // 10. Add a raw JSON string using cJSON_AddRawToObject
    // Create a raw string from the input data
    size_t raw_len = size > 100 ? 100 : size;
    char *raw_string = static_cast<char*>(cJSON_malloc(raw_len + 1));
    if (raw_string) {
        for (size_t i = 0; i < raw_len && i < size; i++) {
            raw_string[i] = static_cast<char>(data[i]);
        }
        raw_string[raw_len] = '\0';
        
        // Try to add as raw JSON
        cJSON *raw_item = cJSON_AddRawToObject(root_object, fuzz_str_7, raw_string);
        
        cJSON_free(raw_string);
    }

    // 11. Write the result to a file using FILE operations
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        char *printed = cJSON_Print(root_object);
        if (printed) {
            fwrite(printed, 1, strlen(printed), out_file);
            cJSON_free(printed);
        }
        assert_file_closed(&out_file);;
    }

    // 12. Also test buffered printing
    char *buffered = cJSON_PrintBuffered(root_object, fuzz_int32_t_8, fuzz_int32_t_9);
    if (buffered) {
        cJSON_free(buffered);
    }

    // 13. Test file descriptor operations
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        // Read a byte from the input
        int fd = fuzz_fileno(in_file);
        (void)fd; // Use the file descriptor if needed
        
        // Read some data
        uint8_t buffer[16];
        size_t read_bytes = fread(buffer, 1, sizeof(buffer), in_file);
        (void)read_bytes; // Use if needed
        
        assert_file_closed(&in_file);;
    }

    // 14. Clean up all resources
    cJSON_Delete(parsed_json);
    cJSON_Delete(root_object);
    
    // Note: cJSON_Delete recursively frees all child items,
    // so we don't need to manually free items added to the tree

    assert_file_closed(&in_file);
	assert_file_closed(&out_file);
	return 0;
}