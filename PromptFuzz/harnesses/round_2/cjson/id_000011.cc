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
//<score> 10, nr_unique_branch: 1
//<Quality> {"density":20,"unique_branches":{"cJSON_CreateFloatArray":[[2694,14,2694,22,0,0,4,1]]},"library_calls":["cJSON_ParseWithLengthOpts","cJSON_Duplicate","cJSON_Delete","cJSON_AddNumberToObject","cJSON_AddBoolToObject","cJSON_GetObjectItemCaseSensitive","cJSON_GetObjectItemCaseSensitive","cJSON_IsString","cJSON_malloc","cJSON_CreateFloatArray","cJSON_AddItemToObject","cJSON_free","cJSON_GetObjectItemCaseSensitive","cJSON_GetObjectItemCaseSensitive","cJSON_IsString","cJSON_IsNumber","cJSON_SetNumberHelper","cJSON_PrintBuffered","cJSON_free","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_ParseWithLengthOpts","cJSON_Duplicate","cJSON_AddNumberToObject","cJSON_AddBoolToObject","cJSON_GetObjectItemCaseSensitive","cJSON_GetObjectItemCaseSensitive","cJSON_IsString","cJSON_malloc","cJSON_CreateFloatArray","cJSON_AddItemToObject","cJSON_free","cJSON_GetObjectItemCaseSensitive","cJSON_GetObjectItemCaseSensitive","cJSON_IsNumber","cJSON_IsString","cJSON_SetNumberHelper","cJSON_PrintBuffered","cJSON_free","cJSON_Delete","cJSON_Delete"],"visited":3}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_11(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	

    // Parse input JSON
    const char* parse_end = nullptr;
    cJSON* root = cJSON_ParseWithLengthOpts(reinterpret_cast<const char*>(data), size, &parse_end, false);
    if (!root) {
	return 0;
	}
	

    // Create a duplicate with recursion
    cJSON* duplicate = cJSON_Duplicate(root, 1);
    if (!duplicate) {
        cJSON_Delete(root);
        return 0;
    }

    // Add a number field to the duplicate object
    double rand_num = static_cast<double>(data[0]) / 255.0 * 100.0;
    cJSON_AddNumberToObject(duplicate, "fuzzed_number", rand_num);

    // Add a boolean field based on first byte
    cJSON_bool bool_val = (data[0] & 1) ? 1 : 0;
    cJSON_AddBoolToObject(duplicate, "fuzzed_bool", bool_val);

    // Modify an existing number if available
    cJSON* menu = cJSON_GetObjectItemCaseSensitive(duplicate, "menu");
    if (menu) {
        cJSON* id = cJSON_GetObjectItemCaseSensitive(menu, "id");
        if (id && cJSON_IsString(id)) {
            // Create a float array based on the string length
            int str_len = strlen(id->valuestring);
            float* float_array = static_cast<float*>(cJSON_malloc(sizeof(float) * str_len));
            if (float_array) {
                for (int i = 0; i < str_len && i < 10; ++i) {
                    float_array[i] = static_cast<float>(id->valuestring[i]) / 255.0f;
                }
                cJSON* float_arr_json = cJSON_CreateFloatArray(float_array, str_len < 10 ? str_len : 10);
                if (float_arr_json) {
                    cJSON_AddItemToObject(duplicate, "float_array", float_arr_json);
                }
                cJSON_free(float_array);
            }
        }
        
        // Try to set a number value if we find a number field
        cJSON* value = cJSON_GetObjectItemCaseSensitive(menu, "value");
        if (!value) {
            value = cJSON_GetObjectItemCaseSensitive(menu, "id");
        }
        if (value && (cJSON_IsString(value) || cJSON_IsNumber(value))) {
            double new_val = static_cast<double>(size % 100);
            cJSON_SetNumberHelper(value, new_val);
        }
    }

    // Print the modified JSON with buffering
    int buffer_size = 2048;
    char* printed = cJSON_PrintBuffered(duplicate, buffer_size, 1);
    if (printed) {
        // Write to output file
        FILE* out_file = fopen("output_file", "wb");
        if (out_file) {
            fwrite(printed, 1, strlen(printed), out_file);
            assert_file_closed(&out_file);;
        }
        cJSON_free(printed);
    }

    // Optional: Read back and verify (simulated file operations)
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (in_file) {
        int fd = fuzz_fileno(in_file);
        if (fd != -1) {
            // Simulate reading a small portion
            uint8_t buf[16];
            fread(buf, 1, size < 16 ? size : 16, in_file);
        }
        assert_file_closed(&in_file);;
    }

    // Clean up
    cJSON_Delete(duplicate);
    cJSON_Delete(root);
    
    assert_file_closed(&in_file);
	return 0;
}