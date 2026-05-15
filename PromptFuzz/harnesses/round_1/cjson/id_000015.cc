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
//<ID> 40
//<Prompt> []
/*<Combination>: [
*/
//<score> 80, nr_unique_branch: 3
//<Quality> {"density":20,"unique_branches":{"cJSON_Delete":[[268,13,268,48,0,0,4,1]],"add_item_to_object":[[2092,9,2092,21,0,0,4,0]],"cJSON_IsRaw":[[3064,9,3064,21,0,0,4,1]]},"library_calls":["cJSON_ParseWithLength","cJSON_CreateObject","cJSON_Delete","cJSON_IsArray","cJSON_AddRawToObject","cJSON_IsRaw","cJSON_IsNumber","cJSON_AddItemToObjectCS","cJSON_Duplicate","cJSON_IsNumber","cJSON_CreateNumber","cJSON_AddItemToObjectCS","cJSON_CreateArray","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_IsArray","cJSON_AddItemToObjectCS","cJSON_Print","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_ParseWithLength","cJSON_CreateObject","cJSON_IsArray","cJSON_AddRawToObject","cJSON_IsRaw","cJSON_IsNumber","cJSON_AddItemToObjectCS","cJSON_Duplicate","cJSON_IsNumber","cJSON_CreateNumber","cJSON_AddItemToObjectCS","cJSON_CreateArray","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_IsArray","cJSON_AddItemToObjectCS","cJSON_Print","cJSON_Delete","cJSON_Delete"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_15(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // Create a FILE* to read input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Read the input data as a string for parsing
    fseek(in_file, 0, SEEK_END);
    long file_size = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);
    
    char *json_string = (char *)malloc(file_size + 1);
    if (!json_string) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    size_t read_size = fread(json_string, 1, file_size, in_file);
    json_string[read_size] = '\0';
    assert_file_closed(&in_file);;

    // Parse the JSON string
    cJSON *parsed_json = cJSON_ParseWithLength(json_string, read_size);
    if (!parsed_json) {
        free(json_string);
        assert_file_closed(&in_file);
	return 0;
    }

    // Create a new JSON object for our operations
    cJSON *new_object = cJSON_CreateObject();
    if (!new_object) {
        cJSON_Delete(parsed_json);
        free(json_string);
        assert_file_closed(&in_file);
	return 0;
    }

    // Check if the parsed JSON is an array
    if (cJSON_IsArray(parsed_json)) {
        // Create a raw string from part of the input data
        size_t raw_size = size > 100 ? 100 : size;
        char *raw_data = (char *)malloc(raw_size + 1);
        if (raw_data) {
            memcpy(raw_data, data, raw_size);
            raw_data[raw_size] = '\0';
            
            // Add raw data to object using cJSON_AddRawToObject
            cJSON *raw_item = cJSON_AddRawToObject(new_object, "raw_data", raw_data);
            
            // Check if the added item is raw
            if (raw_item) {
                cJSON_IsRaw(raw_item);
            }
            
            free(raw_data);
        }
    }

    // Check for numbers in the parsed JSON
    if (parsed_json->type == cJSON_Number) {
        cJSON_IsNumber(parsed_json);
        
        // Add the parsed JSON as an item to our new object using cJSON_AddItemToObjectCS
        cJSON_AddItemToObjectCS(new_object, "number_item", cJSON_Duplicate(parsed_json, 1));
    }

    // Also traverse the parsed JSON to find numbers
    cJSON *current = parsed_json;
    while (current) {
        if (cJSON_IsNumber(current)) {
            // Create a new number item
            cJSON *number_item = cJSON_CreateNumber(42.5);
            if (number_item) {
                // Add to object using cJSON_AddItemToObjectCS
                cJSON_AddItemToObjectCS(new_object, "extra_number", number_item);
            }
            break;
        }
        current = current->next;
    }

    // Create a JSON array and check if it's an array
    cJSON *array = cJSON_CreateArray();
    if (array) {
        cJSON_AddItemToArray(array, cJSON_CreateNumber(1.0));
        cJSON_AddItemToArray(array, cJSON_CreateNumber(2.0));
        
        // Check if it's an array
        cJSON_IsArray(array);
        
        // Add array to object using cJSON_AddItemToObjectCS
        cJSON_AddItemToObjectCS(new_object, "test_array", array);
    }

    // Write the result to a file
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        char *printed = cJSON_Print(new_object);
        if (printed) {
            fwrite(printed, 1, strlen(printed), out_file);
            free(printed);
        }
        assert_file_closed(&out_file);;
    }

    // Clean up
    cJSON_Delete(parsed_json);
    cJSON_Delete(new_object);
    free(json_string);

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}