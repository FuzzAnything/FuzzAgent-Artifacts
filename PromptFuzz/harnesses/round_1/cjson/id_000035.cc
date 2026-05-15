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
//<ID> 143
//<Prompt> []
/*<Combination>: [
*/
//<score> 34, nr_unique_branch: 1
//<Quality> {"density":17,"unique_branches":{"add_item_to_object":[[2108,9,2108,44,0,0,4,1]]},"library_calls":["cJSON_Parse","cJSON_CreateObject","cJSON_Delete","cJSON_GetObjectItem","cJSON_CreateObjectReference","cJSON_AddItemToObjectCS","cJSON_CreateNumber","cJSON_AddItemToObjectCS","cJSON_AddArrayToObject","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_DetachItemViaPointer","cJSON_AddItemToObjectCS","cJSON_Print","cJSON_free","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_Parse","cJSON_CreateObject","cJSON_GetObjectItem","cJSON_CreateObjectReference","cJSON_AddItemToObjectCS","cJSON_CreateNumber","cJSON_AddItemToObjectCS","cJSON_AddArrayToObject","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_DetachItemViaPointer","cJSON_AddItemToObjectCS","cJSON_Print","cJSON_free","cJSON_Delete","cJSON_Delete"],"visited":0}
/**/


// Include cJSON headers

extern "C" int LLVMFuzzerTestOneInput_35(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create a null-terminated string from the input data
    char *json_string = static_cast<char*>(malloc(size + 1));
    if (!json_string) {
	return 0;
	}
	
    memcpy(json_string, data, size);
    json_string[size] = '\0';

    // Parse the JSON string
    cJSON *root = cJSON_Parse(json_string);
    if (!root) {
        free(json_string);
        return 0;
    }

    // Create a new object to work with
    cJSON *new_object = cJSON_CreateObject();
    if (!new_object) {
        cJSON_Delete(root);
        free(json_string);
        return 0;
    }

    // Get the "Image" object from the parsed JSON
    cJSON *image_object = cJSON_GetObjectItem(root, "Image");
    if (image_object) {
        // Create an object reference to the Image object
        cJSON *image_ref = cJSON_CreateObjectReference(image_object);
        if (image_ref) {
            // Add the reference to new_object with case-sensitive key
            cJSON_AddItemToObjectCS(new_object, "ImageRef", image_ref);
        }

        // Create a number and add it to new_object
        cJSON *number = cJSON_CreateNumber(42.5);
        if (number) {
            cJSON_AddItemToObjectCS(new_object, "AddedNumber", number);
        }

        // Add an array to new_object
        cJSON *new_array = cJSON_AddArrayToObject(new_object, "NewArray");
        if (new_array) {
            // Add some numbers to the array
            for (int i = 0; i < 3; i++) {
                cJSON_AddItemToArray(new_array, cJSON_CreateNumber(i * 1.5));
            }
        }

        // Detach the number we added earlier
        if (number) {
            cJSON *detached_number = cJSON_DetachItemViaPointer(new_object, number);
            if (detached_number) {
                // Re-add it with a different key
                cJSON_AddItemToObjectCS(new_object, "ReattachedNumber", detached_number);
            }
        }

        // Print the resulting JSON
        char *printed = cJSON_Print(new_object);
        if (printed) {
            // Do something with the printed JSON (e.g., write to file)
            FILE *out_file = fopen("output_file", "wb");
            if (out_file) {
                fwrite(printed, strlen(printed), 1, out_file);
                assert_file_closed(&out_file);;
            }
            cJSON_free(printed);
        }
    }

    // Clean up
    cJSON_Delete(new_object);
    cJSON_Delete(root);
    free(json_string);

    return 0;
}