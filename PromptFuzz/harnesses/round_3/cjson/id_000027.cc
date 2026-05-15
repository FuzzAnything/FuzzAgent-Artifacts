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
//<ID> 150
//<Prompt> []
/*<Combination>: [
*/
//<score> 50, nr_unique_branch: 1
//<Quality> {"density":25,"unique_branches":{"cJSON_IsObject":[[3054,9,3054,21,0,0,4,0]]},"library_calls":["cJSON_ParseWithLength","cJSON_CreateObject","cJSON_Delete","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_CreateObjectReference","cJSON_AddItemToObject","cJSON_AddObjectToObject","cJSON_AddNumberToObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsArray","cJSON_GetArraySize","cJSON_malloc","cJSON_GetArrayItem","cJSON_IsNumber","cJSON_CreateIntArray","cJSON_AddItemToObject","cJSON_free","cJSON_GetObjectItemCaseSensitive","cJSON_GetObjectItemCaseSensitive","cJSON_IsBool","cJSON_CreateObject","cJSON_AddBoolToObject","cJSON_AddItemToObject","cJSON_malloc","cJSON_CreateDoubleArray","cJSON_AddItemToObject","cJSON_free","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_ParseWithLength","cJSON_CreateObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_CreateObjectReference","cJSON_AddItemToObject","cJSON_AddObjectToObject","cJSON_AddNumberToObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsArray","cJSON_GetArraySize","cJSON_malloc","cJSON_GetArrayItem","cJSON_IsNumber","cJSON_CreateIntArray","cJSON_AddItemToObject","cJSON_free","cJSON_GetObjectItemCaseSensitive","cJSON_GetObjectItemCaseSensitive","cJSON_IsBool","cJSON_CreateObject","cJSON_AddBoolToObject","cJSON_AddItemToObject","cJSON_malloc","cJSON_CreateDoubleArray","cJSON_AddItemToObject","cJSON_free","cJSON_Delete","cJSON_Delete"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_27(const uint8_t* f_data, size_t f_size) {
	if(f_size<137) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_7, fuzz_str_sz_7, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_8, fuzz_str_sz_8, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_9, fuzz_str_sz_9, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_10, fuzz_str_sz_10, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_11, fuzz_str_sz_11, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_12, fuzz_str_sz_12, fdp);
	//fuzzer shim end}




    // Parse input JSON data
    cJSON *root = cJSON_ParseWithLength(reinterpret_cast<const char*>(data), size);
    if (!root) {
        return 0; // Failed to parse, but that's valid input for fuzzing
    }

    // Create a new object to work with
    cJSON *main_object = cJSON_CreateObject();
    if (!main_object) {
        cJSON_Delete(root);
        return 0;
    }

    // Check if "Image" exists and is an object in the parsed input
    cJSON *image = cJSON_GetObjectItemCaseSensitive(root, fuzz_str_1);
    if (cJSON_IsObject(image)) {
        // Create a reference to the Image object from input
        cJSON *image_ref = cJSON_CreateObjectReference(image);
        if (image_ref) {
            // Add the reference to our main object
            cJSON_AddItemToObject(main_object, fuzz_str_2, image_ref);
        }
    }

    // Create a nested object inside main_object
    cJSON *nested_obj = cJSON_AddObjectToObject(main_object, fuzz_str_3);
    if (nested_obj) {
        // Add a number to the nested object
        // Use size modulo 1000 as a number to add some variety
        double number_to_add = static_cast<double>(size % 1000);
        cJSON_AddNumberToObject(nested_obj, fuzz_str_4, number_to_add);
    }

    // Check if "IDs" exists and is an array in the parsed input
    cJSON *ids = cJSON_GetObjectItemCaseSensitive(root, fuzz_str_5);
    if (cJSON_IsArray(ids)) {
        // Extract IDs from input to create an integer array
        int array_size = cJSON_GetArraySize(ids);
        if (array_size > 0) {
            int *int_array = static_cast<int*>(cJSON_malloc(sizeof(int) * array_size));
            if (int_array) {
                for (int i = 0; i < array_size && i < 10; i++) { // Limit to 10 elements
                    cJSON *item = cJSON_GetArrayItem(ids, fuzz_int32_t_6);
                    if (cJSON_IsNumber(item)) {
                        int_array[i] = static_cast<int>(item->valuedouble);
                    } else {
                        int_array[i] = 0;
                    }
                }
                // Create integer array from extracted values
                cJSON *int_array_obj = cJSON_CreateIntArray(int_array, array_size);
                if (int_array_obj) {
                    cJSON_AddItemToObject(main_object, fuzz_str_7, int_array_obj);
                }
                cJSON_free(int_array);
            }
        }
    }

    // Check if "Thumbnail" exists in the parsed input
    cJSON *thumbnail = cJSON_GetObjectItemCaseSensitive(root, fuzz_str_8);
    if (thumbnail) {
        // Check if "Width" in Thumbnail is a boolean (it's not in the example, but we call the API)
        cJSON *thumb_width = cJSON_GetObjectItemCaseSensitive(thumbnail, fuzz_str_9);
        if (thumb_width) {
            // This will return false since "Width" is a string in the example
            cJSON_bool is_bool = cJSON_IsBool(thumb_width);
            // Create a new object to store this check result
            cJSON *bool_check = cJSON_CreateObject();
            if (bool_check) {
                cJSON_AddBoolToObject(bool_check, fuzz_str_10, is_bool);
                cJSON_AddItemToObject(main_object, fuzz_str_11, bool_check);
            }
        }
    }

    // Create a double array using some values from the input
    // Use size and data to generate some double values
    int double_count = 5;
    double *double_array = static_cast<double*>(cJSON_malloc(sizeof(double) * double_count));
    if (double_array) {
        for (int i = 0; i < double_count; i++) {
            // Create some double values based on input data
            if (size > i) {
                double_array[i] = static_cast<double>(data[i % size]) / 256.0 * 100.0;
            } else {
                double_array[i] = static_cast<double>(i);
            }
        }
        cJSON *double_array_obj = cJSON_CreateDoubleArray(double_array, double_count);
        if (double_array_obj) {
            cJSON_AddItemToObject(main_object, fuzz_str_12, double_array_obj);
        }
        cJSON_free(double_array);
    }

    // Clean up
    cJSON_Delete(root);
    cJSON_Delete(main_object);

    return 0;
}