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
//<ID> 3
//<Prompt> []
/*<Combination>: [
*/
//<score> 15, nr_unique_branch: 0
//<Quality> {"density":15,"unique_branches":{},"library_calls":["cJSON_Parse","cJSON_IsObject","cJSON_Delete","cJSON_GetObjectItem","cJSON_IsObject","cJSON_GetObjectItem","cJSON_IsString","cJSON_AddNullToObject","cJSON_AddTrueToObject","cJSON_AddFalseToObject","cJSON_AddItemReferenceToObject","cJSON_DeleteItemFromObject","cJSON_Print","cJSON_Minify","cJSON_Delete"],"critical_calls":["cJSON_Parse","cJSON_IsObject","cJSON_GetObjectItem","cJSON_IsObject","cJSON_GetObjectItem","cJSON_IsString","cJSON_AddNullToObject","cJSON_AddTrueToObject","cJSON_AddFalseToObject","cJSON_AddItemReferenceToObject","cJSON_DeleteItemFromObject","cJSON_Print","cJSON_Minify","cJSON_Delete"],"visited":0}
/**/


// Include cJSON header

extern "C" int LLVMFuzzerTestOneInput_2(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early return for empty input
    if (size == 0) {
        return 0;
    }

    // Create a null-terminated string from the input data
    char *json_string = (char *)malloc(size + 1);
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

    // Check if root is an object
    if (!cJSON_IsObject(root)) {
        cJSON_Delete(root);
        free(json_string);
        return 0;
    }

    // 1. Use cJSON_IsString to check for string items
    cJSON *widget = cJSON_GetObjectItem(root, "widget");
    if (widget && cJSON_IsObject(widget)) {
        cJSON *debug = cJSON_GetObjectItem(widget, "debug");
        if (debug && cJSON_IsString(debug)) {
            // Valid string found - do nothing special
        }
    }

    // 2. Add null, true, and false fields to the root object
    cJSON *null_field = cJSON_AddNullToObject(root, "fuzz_null");
    cJSON *true_field = cJSON_AddTrueToObject(root, "fuzz_true");
    cJSON *false_field = cJSON_AddFalseToObject(root, "fuzz_false");

    // 3. Add reference to an existing item
    if (widget) {
        // Create a reference to the widget object
        cJSON_AddItemReferenceToObject(root, "widget_reference", widget);
    }

    // 4. Delete one of the items we added
    cJSON_DeleteItemFromObject(root, "fuzz_null");

    // 5. Print and minify the JSON
    char *printed_json = cJSON_Print(root);
    if (printed_json) {
        // Minify the JSON string
        cJSON_Minify(printed_json);
        
        // Optional: Write to output file
        FILE *out_file = fopen("output_file", "wb");
        if (out_file) {
            fwrite(printed_json, strlen(printed_json), 1, out_file);
            assert_file_closed(&out_file);;
        }
        
        free(printed_json);
    }

    // Cleanup
    cJSON_Delete(root);
    free(json_string);
    
    return 0;
}